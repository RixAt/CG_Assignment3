// ====================================================================
/* Assignment 4: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  _____ _______  _______ _   _ ____  _____ 
// |_   _| ____\ \/ /_   _| | | |  _ \| ____|
//   | | |  _|  \  /  | | | | | | |_) |  _|  
//   | | | |___ /  \  | | | |_| |  _ <| |___ 
//   |_| |_____/_/\_\ |_|  \___/|_| \_\_____|
//
// ====================================================================
// File: texture.cpp
// Description: 
// 
// ====================================================================

#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include <stb_image.h>
#include <iostream>
#include "logger.h"

// Ctor: Initialize texture ID to 0
Texture::Texture()
	: textureID(0)
{
}

// Dtor: Delete OpenGL texture
Texture::~Texture() {
	if (textureID != 0) {
		glDeleteTextures(1, &textureID);
	}
}

// loadFromFile(): Load texture from image file
bool Texture::loadFromFile(const std::string& path, bool generateMipmaps) {
	int width, height, channels;

	LOG_INFO("Loading texture: " + path);

	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!data) {
		LOGE("Texture load FAILED: " << path
			<< " | Reason: " << stbi_failure_reason());

		// Handled by TextureManager now
		/*if (path != "assets/textures/default_texture.png") {
			return loadFromFile("assets/textures/default_texture.png", false);
		}*/
		return false;
	}

	GLenum format = (channels == 4) ? GL_RGBA : (channels == 3 ? GL_RGB : GL_RED);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
	GL_CHECK("TexImage2D");

	GLenum err = glGetError();
	if (err != GL_NO_ERROR) {
		std::cerr << "GL ERROR after glTexImage2D: " << err << "\n";
	}

	// Mipmap generation is not supported by Windows by default (and I dont feel like tackling this right now)
	//if (generateMipmaps) {
	//	//glGenerateMipmap(GL_TEXTURE_2D);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	/*}*/

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	stbi_image_free(data);

	LOGI("Texture loaded OK: " << path
		<< " (" << width << "x" << height << "), channels=" << channels);

	return true;
}

// bind(): Bind the texture for use
void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, textureID);
	GL_CHECK("BindTexture");
	return;
}

// unbind(): Unbind the texture
void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
	return;
}

