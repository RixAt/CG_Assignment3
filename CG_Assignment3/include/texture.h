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
// File: texture.h
// Description: 
// 
// ====================================================================

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <GL/glut.h>
#include "logger.h"

class Texture {
public:
	Texture();
	~Texture();

	Texture(const Texture&) = delete;


	bool loadFromFile(const std::string& filename, bool generateMipmaps = true);
	void bind() const;
	void unbind() const;

	GLuint id() const { return textureID; }

private:
	GLuint textureID = 0;
};


inline void GLCheck(const char* label, const char* file, int line) {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		LOGE("GL error at " << label
			<< " code=" << err);
	}
}
#define GL_CHECK(label) GLCheck(label, __FILE__, __LINE__)

#endif // !TEXTURE_H

