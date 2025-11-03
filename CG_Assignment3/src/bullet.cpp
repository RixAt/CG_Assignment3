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