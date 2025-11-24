// ====================================================================
/* Assignment 3: Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____    _    ____  ____  _____ _     
// | __ )  / \  |  _ \|  _ \| ____| |    
// |  _ \ / _ \ | |_) | |_) |  _| | |    
// | |_) / ___ \|  _ <|  _ <| |___| |___ 
// |____/_/   \_\_| \_\_| \_\_____|_____|
// 
// ====================================================================
// File: barrel.hpp
// Description: 
// ====================================================================
  
#pragma once
#include "utilities.hpp"
#include "gameobject.hpp"
#include "model.h"

class Barrel : public GameObject {
public:
    float radius = 10.0f;       // collision sphere radius
    bool exploded = false;
    float explodeTime = 0.0f;
    float explosionRadius = 0.0f;     // current expanding radius
    float explosionMaxRadius = 10.0f; // how big it gets
    float explosionSpeed = 12.0f;     // units per second


    // All barrels share the same model (loaded once externally)
    static Model* s_barrelModel;
    
    Barrel(const Vector3& pos)
        : GameObject(pos)
    {}

    static void setSharedModel(Model* m) {
        s_barrelModel = m;
    }

    void explode() {
        exploded = true;
        explodeTime = 0.0f;
        deactivate();     // stops drawing in normal mode
    }

    void update(float dt) override {
        if (exploded) {
            explodeTime += dt;

            explosionRadius += explosionSpeed * dt;
            if (explosionRadius > explosionMaxRadius)
                explosionRadius = explosionMaxRadius;



            if (explodeTime > 1.5f) exploded = false;
        }
    }

    void draw(RenderMode mode) const override {
        if (!active) return;
        if (!s_barrelModel) return;

        glPushMatrix();
        glTranslatef(position.x, position.y, position.z);
        glScalef(0.25f, 0.25f, 0.25f);     // same scale you used before
        glRotatef(90.f, 0, 1, 0);           // fix forward direction

        s_barrelModel->draw(mode);
        glPopMatrix();
    }

private:
};
