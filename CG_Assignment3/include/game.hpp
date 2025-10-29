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


class Game {
public:
	// World state
	std::vector<Robot*> robots;

	// Temporary camera
	Vector3 cameraPos;
	float cameraYaw;

	// Rendering state
	RenderMode g_renderMode;
	bool showAxes;
	bool showColliders;
	bool motionEnabled;

	Game();

	void init();
	void update(float dt);
	void draw(int winW, int winH) const;



};


#endif // !GAME_HPP
