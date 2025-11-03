// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  _____ _____ _____ _____ ____ _____ ____  
// | ____|  ___|  ___| ____/ ___|_   _/ ___| 
// |  _| | |_  | |_  |  _|| |     | | \___ \ 
// | |___|  _| |  _| | |__| |___  | |  ___) |
// |_____|_|   |_|   |_____\____| |_| |____/ 
// 
// ====================================================================
// File: effects.hpp
// Description: 
// 
// ====================================================================

#ifndef EFFECTS_HPP
#define EFFECTS_HPP

#include <vector>
#include "utilities.hpp"
#include "render_util.hpp"

struct ImpactFX {
	Vector3 pos;
	float t = 0.0f; // Time elapsed
	float ttl = 0.25f; // Time to live
	float startR = 0.2f;
	float endR = 2.0f;
	Vector3 color = Vector3(1.0f, 0.8f, 0.2f);
	bool active() const { return t < ttl; };

};

inline void effectsSpawnImpact(std::vector<ImpactFX>& list, const Vector3& p) {
	ImpactFX fx;
	fx.pos = p;
	list.push_back(fx);
}

inline void effectsUpdate(std::vector<ImpactFX>& list, float dt) {
	for (auto& e : list) { e.t += dt; }
	list.erase(std::remove_if(list.begin(), list.end(),
		[](const ImpactFX& e) { return !e.active(); }),
		list.end());
}

inline void effectsDrawImpacts(const std::vector<ImpactFX>& list) {
	glPushAttrib(GL_ENABLE_BIT | GL_POLYGON_BIT | GL_CURRENT_BIT);
	for (const auto& e : list) {
		if (!e.active()) continue;
		const float u = e.t / e.ttl;
		const float r = e.startR + u * (e.endR - e.startR);
		const float a = 1.0f - u;

		glColor4f(e.color.x, e.color.y, e.color.z, a);
		glPushMatrix();
		glTranslatef(e.pos.x, e.pos.y, e.pos.z);
		DrawSphere(RenderMode::Wireframe, r);
		glScalef(1.0f, 0.25f, 1.0f);
		DrawSphere(RenderMode::Wireframe, r * 0.7);
		glPopMatrix();
	}
	glPopAttrib();
}


#endif // EFFECTS_HPP