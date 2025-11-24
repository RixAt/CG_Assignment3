// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  __  __  ___  ____  _____ _     
// |  \/  |/ _ \|  _ \| ____| |    
// | |\/| | | | | | | |  _| | |    
// | |  | | |_| | |_| | |___| |___ 
// |_|  |_|\___/|____/|_____|_____|
// 
// ====================================================================
// File: model.cpp
// Description: 
// 
// ====================================================================

#include "model.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include "logger.h"

static std::string GetDir(const std::string& path) {
	size_t pos = path.find_last_of("/\\");
	return (std::string::npos == pos) ? "" : path.substr(0, pos);
}

static bool IsImageFile(const std::string& p) {
    auto lower = p;
    for (char& c : lower) c = (char)tolower(c);
    return lower.find(".png") != std::string::npos ||
        lower.find(".jpg") != std::string::npos ||
        lower.find(".jpeg") != std::string::npos ||
        lower.find(".tga") != std::string::npos ||
        lower.find(".bmp") != std::string::npos;
}

static Texture* LoadMaterialDiffuse(const aiMaterial* mat,
    const std::string& modelDir,
    TextureManager& texMgr)
{
    if (!mat) return nullptr;

    auto tryType = [&](aiTextureType type)->Texture* {
        if (mat->GetTextureCount(type) == 0) return nullptr;

        aiString texPath;
        if (mat->GetTexture(type, 0, &texPath) != AI_SUCCESS) return nullptr;

        std::string rel = texPath.C_Str();
        for (auto& c : rel) if (c == '\\') c = '/';

        // FBX in your case returns "..\\models" (a folder)
        if (!IsImageFile(rel)) {
            LOG_WARN("Material texture path is not a file: " + rel);
            return nullptr;
        }

        std::string full = modelDir + "/" + rel;
        return &texMgr.get(full);
        };

    // Classic + PBR cases
    if (auto* t = tryType(aiTextureType_DIFFUSE))    return t;
    if (auto* t = tryType(aiTextureType_BASE_COLOR)) return t;
    if (auto* t = tryType(aiTextureType_UNKNOWN))    return t;

    std::string fallback = modelDir + "/Barrel_LP001_Barrel_BaseColor.png";
    LOG_WARN("Using manual fallback diffuse: " + fallback);
    return &texMgr.get(fallback);
}


static void ProcessMesh(const aiMesh* mesh,
    const aiScene* scene,
    const std::string& dir,
    TextureManager& texMan,
    std::vector<std::unique_ptr<Mesh>>& out)
{
    auto m = std::make_unique<Mesh>();

    m->vertices.reserve(mesh->mNumVertices);
    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        VertexPTN v{};
        v.pos = Vector3(mesh->mVertices[i].x,
            mesh->mVertices[i].y,
            mesh->mVertices[i].z);

        if (mesh->HasNormals()) {
            v.normal = Vector3(mesh->mNormals[i].x,
                mesh->mNormals[i].y,
                mesh->mNormals[i].z);
        }
        else {
            v.normal = Vector3(0, 1, 0);
        }

        if (mesh->mTextureCoords[0]) {
            v.u = mesh->mTextureCoords[0][i].x;
            v.v = mesh->mTextureCoords[0][i].y;
        }
        else {
            v.u = v.v = 0.0f;
        }

        m->vertices.push_back(v);
    }

    for (unsigned int f = 0; f < mesh->mNumFaces; f++) {
        const aiFace& face = mesh->mFaces[f];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            m->indices.push_back(face.mIndices[j]);
    }

    if (mesh->mMaterialIndex >= 0 && scene->mMaterials) {
        const aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
        m->diffuseTexture = LoadMaterialDiffuse(mat, dir, texMan);
    }

    out.push_back(std::move(m));
}

static void ProcessNode(const aiNode* node,
    const aiScene* scene,
    const std::string& dir,
    TextureManager& texMan,
    std::vector<std::unique_ptr<Mesh>>& out)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        const aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        ProcessMesh(mesh, scene, dir, texMan, out);
    }
    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene, dir, texMan, out);
    }
}

void Mesh::draw(RenderMode mode) const {
    glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_CURRENT_BIT | GL_LIGHTING_BIT);

    // preserve your render modes
    switch (mode) {
    case RenderMode::Wireframe:
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    case RenderMode::Vertices:
        glDisable(GL_TEXTURE_2D);
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        break;
    case RenderMode::Solid:
    default:
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        if (diffuseTexture && diffuseTexture->id() != 0) {
            glEnable(GL_TEXTURE_2D);
            diffuseTexture->bind();
            glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
            glColor3f(1, 1, 1);
        }
        else {
            glDisable(GL_TEXTURE_2D);
        }
        break;
    }

    glBegin(GL_TRIANGLES);
    for (unsigned int idx : indices) {
        const auto& v = vertices[idx];
        glNormal3f(v.normal.x, v.normal.y, v.normal.z);
        glTexCoord2f(v.u, v.v);
        glVertex3f(v.pos.x, v.pos.y, v.pos.z);
    }
    glEnd();

    if (diffuseTexture) diffuseTexture->unbind();
    glPopAttrib();
};

bool Model::load(const std::string& modelPath, TextureManager& texMan) {
    directory = GetDir(modelPath);

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        modelPath,
        aiProcess_Triangulate |
        aiProcess_FlipUVs |
        aiProcess_GenNormals
    );

    if (!scene || !scene->mRootNode) {
        std::cerr << "Assimp load failed: " << importer.GetErrorString() << "\n";
        return false;
    }

    meshes.clear();
    ProcessNode(scene->mRootNode, scene, directory, texMan, meshes);

    std::cout << "Model loaded: " << modelPath << " meshes=" << meshes.size() << "\n";
    return true;
}

void Model::draw(RenderMode mode) const {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotationDegrees.x, 1, 0, 0);
    glRotatef(rotationDegrees.y, 0, 1, 0);
    glRotatef(rotationDegrees.z, 0, 0, 1);
    glScalef(scale, scale, scale);

    for (auto& m : meshes) m->draw(mode);

    glPopMatrix();
}