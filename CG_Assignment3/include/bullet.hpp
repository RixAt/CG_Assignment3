// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____  _   _ _     _     _____ _____ 
// | __ )| | | | |   | |   | ____|_   _|
// |  _ \| | | | |   | |   |  _|   | |  
// | |_) | |_| | |___| |___| |___  | |  
// |____/ \___/|_____|_____|_____| |_|  
// 
// ====================================================================
// File: bullet.hpp
// Description: 
// 
// ====================================================================

#ifndef BULLET_HPP
#define BULLET_HPP

#include "utilities.hpp"

class Bullet {
public:
	Bullet();

	void spawn(const Vector3& startPos,
		const Vector3& velocity,
		float radius = 0.5f,
		float ttlSec = 2.0f);

	bool update(float dt);
	void draw(RenderMode mode) const;

	bool active() const { return m_active; };
	const Vector3& pos() const { return m_pos; };
	float radius() const { return m_radius; };
private:
	Vector3 m_pos{ 0,0,0 }; 
	Vector3 m_vel{ 0,0,0 };
	float m_radius{ 0.5f };
	float m_ttl{ 0.0f };
	bool m_active{ false };

};


#endif
