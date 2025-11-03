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
// Description: 
// 
// ====================================================================

#include "bullet.hpp"
#include "render_util.hpp"
#include "effects.hpp"
#include <algorithm>

Bullet::Bullet() = default;

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
}

bool Bullet::update(float dt) {
	// If not active, dont update
	if (!m_active) return false;

	m_pos = m_pos + m_vel * dt;
	m_ttl -= dt;
	if (m_ttl <= 0.0f) {
		m_active = false;
		return false;
	}
	return true;
}

void Bullet::draw(RenderMode mode)const {
	if (!m_active) return;
	glPushMatrix();
	glTranslatef(m_pos.x, m_pos.y, m_pos.z);
	glColor3f(1.0f, 1.0f, 1.0f);
	DrawSphere(mode, m_radius);	
	glPopMatrix();
}

void bulletsFire(std::vector<Bullet>& pool,
	const Vector3& start,
	const Vector3& dir,
	float speed,
	float radius,
	float ttl)
{
	const Vector3 vel = dir * speed;
	//// Find an inactive bullet in the pool
	//auto it = std::find_if(pool.begin(), pool.end(),
	//	[](const Bullet& b) { return !b.active(); });
	//if (it != pool.end()) {
	//	Vector3 velocity = dir * speed;
	//	it->spawn(start, velocity, radius, ttl);
	//}

	for (auto& b : pool) {
		if (!b.active()) { 
			b.spawn(start, vel, radius, ttl); 
			return; 
		}
	}

	Bullet b;
	b.spawn(start, vel, radius, ttl);
	pool.push_back(b);
}

void bulletsDraw(const std::vector<Bullet>& pool, RenderMode mode) {
	for (const auto& b : pool) {
		b.draw(mode);
	}
}

void bulletsClear(std::vector<Bullet>& pool) {
	pool.clear();
}

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
		if (!b.active()) continue;
		bool stillActive = b.update(dt);
		if (!stillActive) continue;
		// Check for collisions with robots
		for (auto& r : robots) {
			if (!r || !r->isAlive()) continue;
			if (r->isAlive() && r->checkHit(b.pos(), b.radius())) {
				r->kill();
				++hits;
				++shotsHit;
				++robotsKilled;
				// Calculate distance bonus
				float dist = (b.pos() - b.pos()).magnitude();
				int distBonus = static_cast<int>(dist * distBonusMultiplier);
				score += baseScore + distBonus;
				if (impacts) effectsSpawnImpact(*impacts, b.pos());

				b.deactivate();
				break; // Bullet can only hit one robot
			}
		}
	}
	return hits;
}