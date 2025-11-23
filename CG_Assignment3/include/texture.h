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


#endif // !TEXTURE_H

