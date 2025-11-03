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
#include "robots.hpp"
#include "effects.hpp"
#include <vector>

class Bullet {
public:
	Bullet();

	void spawn(const Vector3& startPos,
		const Vector3& velocity,
		float radius = 0.5f,
		float ttlSec = 2.0f);

	bool update(float dt);
	void draw(RenderMode mode) const;

	void deactivate() { m_active = false; };
	void activate() { m_active = true; };

	bool active() const { return m_active; };
	const Vector3& pos() const { return m_pos; };
	float radius() const { return m_radius; };

	bool scored() const { return m_scored; };
	void markScored() { m_scored = true; };
private:
	Vector3 m_pos{ 0,0,0 }; 
	Vector3 m_origin{ 0,0,0 };
	Vector3 m_vel{ 0,0,0 };
	float m_radius{ 0.5f };
	float m_ttl{ 0.0f };
	bool m_active{ false };
	bool m_scored = false;

};

void bulletsFire(std::vector<Bullet>& pool,
	const Vector3& start,
	const Vector3& dir,
	float speed,
	float radius,
	float ttl);

void bulletsDraw(const std::vector<Bullet>& pool, RenderMode mode);

void bulletsClear(std::vector<Bullet>& pool);

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




#endif
