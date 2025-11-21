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

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

	if (!data) {
		std::cerr << "Failed to load texture: " << path
			<< std::endl << stbi_failure_reason() << std::endl;
		return false;
	}

	GLenum format = (channels == 4) ? GL_RGBA : (channels == 3 ? GL_RGB : GL_RED);

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

	if (generateMipmaps) {
		//glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	stbi_image_free(data);
	return true;
}

// bind(): Bind the texture for use
void Texture::bind() const {
	glBindTexture(GL_TEXTURE_2D, textureID);
	return;
}

// unbind(): Unbind the texture
void Texture::unbind() const {
	glBindTexture(GL_TEXTURE_2D, 0);
	return;
}

