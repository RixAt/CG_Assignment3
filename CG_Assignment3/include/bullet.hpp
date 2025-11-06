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
// Description: Defines the bullet class and bullet management functions
// Bullets are projectiles fired by the player to hit robots and gain
// points.
// ====================================================================

#ifndef BULLET_HPP
#define BULLET_HPP

#include "utilities.hpp"
#include "robots.hpp"
#include "effects.hpp"
#include <vector>

// Class: Bullet
// Represents a single projectile fired by the player
// Each bullet has position, velocity, radius, time-to-live (TTL),
// and state flags for active and scored.
class Bullet {
public:
	// Ctor
	Bullet();

	// Spawns a bullet at startPos with given velocity, radius, and TTL
	// in seconds
	void spawn(const Vector3& startPos,
		const Vector3& velocity,
		float radius = 0.5f,
		float ttlSec = 12.0f);

	// Updates bullet position based on velocity and decreases TTL
	// Returns true if still active, false if TTL expired
	bool update(float dt);

	// Draws the bullet if active
	void draw(RenderMode mode) const;

	// Deactivates or activates the bullet
	void deactivate() { m_active = false; };
	void activate() { m_active = true; };

	// Status accessors
	bool active() const { return m_active; };
	const Vector3& pos() const { return m_pos; };
	float radius() const { return m_radius; };
	bool scored() const { return m_scored; };

	// Marks the bullet as having scored (hit or miss)
	void markScored() { m_scored = true; };
private:
	// Current position in world state
	Vector3 m_pos{ 0,0,0 }; 
	// Origin position where bullet was spawned
	Vector3 m_origin{ 0,0,0 };
	// Velocity vector (units/sec)
	Vector3 m_vel{ 0,0,0 };
	// Collision radius for hit detection
	float m_radius{ 0.5f };
	// Time-to-live in seconds
	float m_ttl{ 0.0f };
	// Active state
	bool m_active = false; // Whether bullet is active in scene
	bool m_scored = false; // Whether bullet has scored (hit or miss)

};

// Helpers for managing a pool of bullets
// 
// Fires a bullet from start position in given direction with speed, radius, and TTL
// Adds to pool or reuses inactive bullet
void bulletsFire(std::vector<Bullet>& pool,
	const Vector3& start,
	const Vector3& dir,
	float speed,
	float radius,
	float ttl);

// Draws all active bullets in the pool
void bulletsDraw(const std::vector<Bullet>& pool, RenderMode mode);

// Clears all bullets from the pool
void bulletsClear(std::vector<Bullet>& pool);

// Updates all bullets in the pool, checking for collisions with robots
// Returns number of hits detected
int bulletsUpdate(std::vector<Bullet>& pool,
	float dt,
	std::vector<Robot*>& robots,
	int& robotsKilled,
	int& shotsHit,
	int& score,
	float distBonusMultiplier = 2.0f,
	int baseScore = 100,
	std::vector<ImpactFX>* impactList = nullptr
);

#endif //BULLET_HPP
