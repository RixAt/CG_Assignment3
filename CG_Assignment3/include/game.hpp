// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//   ____    _    __  __ _____ 
//  / ___|  / \  |  \/  | ____|
// | |  _  / _ \ | |\/| |  _|  
// | |_| |/ ___ \| |  | | |___ 
//  \____/_/   \_\_|  |_|_____|
// 
// ====================================================================
// File: game.hpp
// Description: 
// 
// ====================================================================

#ifndef GAME_HPP
#define GAME_HPP

#include <vector>
#include "utilities.hpp"
#include "render_util.hpp"
#include "robots.hpp"
#include "camera.hpp"


class Game {
public:
	// World state
	std::vector<Robot*> robots;

	CameraSystem cams;

	// Rendering state
	RenderMode g_renderMode;
	bool showAxes;
	bool showColliders;
	bool motionEnabled;

	Game();

	void init();
	void update(float dt);
	void draw(int winW, int winH) const;

	void drawWorld() const;

	// Keyboard input handling
	void handleKey(unsigned char key);
	void handleSpecialKey(int key);

	struct ViewportsLayout {
		Viewport vpMain;
		Viewport vpInset;
		Viewport vpHUD;
	};

	ViewportsLayout computeViewports(int winW, int winH) const;
	void drawMainViewport(const Viewport& vp) const;
	void drawInsetViewport(const Viewport& vp) const;
	void drawHUDViewport(const Viewport& vp) const;

	bool showCameraFrustums = true;
	bool showCameraMarkers = true;
	void drawCameraDebugLines(const Camera* exclude, float aspect) const;

};


#endif // !GAME_HPP
