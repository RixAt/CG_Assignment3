// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  _   _ _____ ___ _     ___ _____ ___ _____ ____  
// | | | |_   _|_ _| |   |_ _|_   _|_ _| ____/ ___| 
// | | | | | |  | || |    | |  | |  | ||  _| \___ \ 
// | |_| | | |  | || |___ | |  | |  | || |___ ___) |
//  \___/  |_| |___|_____|___| |_| |___|_____|____/ 
// 
// ====================================================================
// File: utilities.hpp
// Description to be added later
// 
// ====================================================================
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#pragma once
#include <cmath>

// Struct: Vector3
// A simple 3D vector struct with basic operations
// Used for positions, directions, and other 3D calculations
struct Vector3 {
	float x, y, z;

	// Ctors
	Vector3() : x(0.0), y(0.0), z(0.0) {}
	Vector3(float X, float Y, float Z) : x(X), y(Y), z(Z) {}

	// Operations
	Vector3 operator+(const Vector3& v) const { return Vector3(x + v.x, y + v.y, z + v.z); }      // Vector addition
	Vector3 operator-(const Vector3& v) const { return Vector3(x - v.x, y - v.y, z - v.z); }      // Vector subtraction
	Vector3 operator*(float scalar) const { return Vector3(x * scalar, y * scalar, z * scalar); } // Scalar multiplication

	// Magnitude 
	float magnitude() const { return std::sqrt(x * x + y * y + z * z); }
	float length() const { return magnitude(); }
	
	// Dot product
	float dot(const Vector3& v) const { return x * v.x, y* v.y, z* v.z; }
	
};

// Enum (Class): RenderMode
// Controls how objects are visually drawn in a scene
// Used by draw() functions to switch between solid, wireframe, or vertex-only rebnderubg
enum class RenderMode {
	Solid,
	Wireframe,
	Vertices
};




#endif // UTILITIES_HPP
