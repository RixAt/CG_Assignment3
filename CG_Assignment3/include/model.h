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
// File: model.h
// Description: 
// 
// ====================================================================
// 

#pragma once

#include <vector>
#include <string>
#include <memory>
#include <GL/glew.h>
#include <GL/glut.h>
#include "utilities.hpp"
#include "texture_manager.hpp"

// Struct: VertexPTN
// Vertex with position, normal, and texture coordinates
struct VertexPTN {
	Vector3 pos;
	Vector3 normal;
	float u, v;
};

class Mesh {
public:
	std::vector<VertexPTN> vertices;
	std::vector<unsigned int> indices;
	Texture* diffuseTexture = nullptr;

	void draw(RenderMode mode) const;


};

class Model {
public:
	bool load(const std::string& modelPath, TextureManager& texMgr);
	void draw(RenderMode mode) const;

	Vector3 position{ 0,0,0 };
	float scale = 1.0f;
	Vector3 rotationDegrees{ 0,0,0 };

private:
	std::vector<std::unique_ptr<Mesh>> meshes;
	std::string directory;
};