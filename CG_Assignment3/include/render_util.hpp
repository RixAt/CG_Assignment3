// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____  _____ _   _ ____  _____ ____              
// |  _ \| ____| \ | |  _ \| ____|  _ \             
// | |_) |  _| |  \| | | | |  _| | |_) |            
// |  _ <| |___| |\  | |_| | |___|  _ <             
// |_| \_\_____|_|_\_|____/|_____|_|_\_\_____ ____  
// | | | |_   _|_ _| |   |_ _|_   _|_ _| ____/ ___| 
// | | | | | |  | || |    | |  | |  | ||  _| \___ \ 
// | |_| | | |  | || |___ | |  | |  | || |___ ___) |
//  \___/  |_| |___|_____|___| |_| |___|_____|____/ 
// 
// ====================================================================
// File: render_util.hpp
// Description: Header file for
// 
// ====================================================================
#ifndef RENDER_UTIL_HPP
#define RENDER_UTIL_HPP


#include <GL/glut.h>
#include "utilities.hpp"
#include "camera.hpp"

// Primitive geometry drawing functions
void DrawCube(RenderMode mode, float size = 1.0f);
void DrawSphere(RenderMode mode, float radius = 1.0f);
void DrawPlane(RenderMode mode, float width, float depth, const Vector3& color);

// Draw a ground plane with grid lines
void DrawGround(float size = 200.0f, float spacing = 10.0f);
// Draw coordinate axes at the origin
void DrawAxes(float size = 5.0f);

// Draw a camera marker and frustum
void DrawCameraMarker(const Camera& cam, float size = 1.0f, const Vector3& color = Vector3(1.0f,1.0f,1.0f));
void DrawCameraFrustum(const Camera& cam, float aspect, float scale = 1.0f, const Vector3& color = Vector3(1.0f, 1.0f, 1.0f));


#endif //RENDER_UTIL_HPP