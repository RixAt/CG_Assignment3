// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//   ____    _    __  __ _____ ___  ____      _ _____ ____ _____ 
//  / ___|  / \  |  \/  | ____/ _ \| __ )    | | ____/ ___|_   _|
// | |  _  / _ \ | |\/| |  _|| | | |  _ \ _  | |  _|| |     | |  
// | |_| |/ ___ \| |  | | |__| |_| | |_) | |_| | |__| |___  | |  
//  \____/_/   \_\_|  |_|_____\___/|____/ \___/|_____\____| |_|  
// 
// ====================================================================
// File: gameobject.hpp
// Description: Header file for GameObject class
// 
// ====================================================================


#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "utilities.hpp"

class GameObject {
protected:
	Vector3 position;	// Position in world
	bool active;		// Whether GameObject participates in update/draw

public:
	GameObject() : position(0.0f, 0.0f, 0.0f), active(true) {};
	GameObject(const Vector3& initPos) : position(initPos), active(true) {};
	virtual ~GameObject() = default;

	virtual void update(float dt) = 0;
	virtual void draw(RenderMode mode) const = 0;

	const Vector3& getPosition() const { return position; }
	void setPosition(const Vector3& p) { position = p; }

	bool isActive() const { return active; }
	void deactivate() { active = false; }
	void activate() { active = true; }
};



#endif //GAMEOBJECT_HPP
