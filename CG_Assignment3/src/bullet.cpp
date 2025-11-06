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
// File: bullet.cpp
// Description: Implementation of the Bullet class and bullet 
// management functions. Handles spawning, updating, drawing, etc.
// ====================================================================

#include "bullet.hpp"
#include "render_util.hpp"
#include "effects.hpp"
#include <algorithm>
#include "sound.hpp"

// Ctor: Initialize bullet to inactive state
Bullet::Bullet() = default;

// spawn(): Initializes bullet properties and activates it
// Also marks it as active so it will be updated and drawn
void Bullet::spawn(const Vector3& startPos,
	const Vector3& velocity,
	float radius,
	float ttlSec)
{
	m_pos = startPos;
	m_vel = velocity;
	m_radius = radius;
	m_ttl = std::max(0.0f, ttlSec);
	m_active = (m_ttl > 0.0f);
	m_scored = false;
}

// update(): Updates bullet position and TTL
// If TTL expires, deactivates the bullet and
// its returned to the pool
// Returns true if still active, false if expired this frame
bool Bullet::update(float dt) {
	// If not active, dont update
	if (!m_active) return false;

	// Update position based on velocity and dt
	m_pos = m_pos + m_vel * dt;

	// Decrease TTL
	m_ttl -= dt;
	if (m_ttl <= 0.0f) {
		m_active = false;
		return false; // Expired
	}
	return true; // Still active
}

// draw(): Renders the bullet if active
// Draws a sphere at the bullet position using the specified render mode
void Bullet::draw(RenderMode mode)const {
	if (!m_active) return;
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawSphere(mode, m_radius);	
	glPopMatrix();
}

// bulletsFire(): Fires a bullet from the pool from a given start position
// in the specified direction with speed, radius, and TTL
// Reuses an inactive bullet from the pool if available, otherwise adds a new one
void bulletsFire(std::vector<Bullet>& pool,
	const Vector3& start,
	const Vector3& dir,
	float speed,
	float radius,
	float ttl)
{
	// Compute velocity vector
	const Vector3 vel = dir * speed;

	// Try to find an inactive bullet to reuse
	for (auto& b : pool) {
		if (!b.active()) { 
			b.spawn(start, vel, radius, ttl); 
			return; 
		}
	}

	// No inactive bullet found, create a new one
	Bullet b;
	b.spawn(start, vel, radius, ttl);
	pool.push_back(b);
}

// bulletsDraw(): Draws all active bullets in the pool using the specified render mode
void bulletsDraw(const std::vector<Bullet>& pool, RenderMode mode) {
	for (const auto& b : pool) {
		b.draw(mode);
	}
}

// bulletsClear(): Clears all bullets from the pool
void bulletsClear(std::vector<Bullet>& pool) {
	pool.clear();
}

// bulletsUpdate(): Updates all bullets in the pool
int bulletsUpdate(std::vector<Bullet>& pool,
	float dt,
	std::vector<Robot*>& robots,
	int& robotsKilled,
	int& shotsHit,
	int& score,
	float distBonusMultiplier,
	int baseScore,
	std::vector<ImpactFX>* impacts
) {
	int hits = 0;

	for (auto& b : pool) {
		// Skip inactive bullets
		if (!b.active()) continue;

		// Update bullet position/TTL; if expired, handle miss
		bool stillActive = b.update(dt);
		if (!stillActive) {
			if (!b.scored()) {
				score -= baseScore + 50; // Penalty for missing (-150 total)
				if (score < 0) score = 0;
				b.markScored();
			}
			continue;
		}
		// Check for collisions with robots
		for (auto& r : robots) {
			if (!r || !r->isAlive()) continue;

			if (r->isAlive() && r->checkHit(b.pos(), b.radius())) {
				// Hit detected; handle robot death and scoring
				r->kill();
				++hits;
				++shotsHit;
				++robotsKilled;
				score += baseScore;
				// Spawn particle impact effect
				if (impacts) effectsSpawnImpact(*impacts, b.pos());

				sound::playSFX("assets/hit.ogg", 1.0f);
				sound::playSFX("assets/death.ogg", 0.25f);

				b.deactivate();
				b.markScored();
				break; // Bullet can only hit one robot
			}
		}
	}
	return hits;
}