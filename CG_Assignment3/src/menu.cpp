// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  __  __ _____ _   _ _   _ 
// |  \/  | ____| \ | | | | |
// | |\/| |  _| |  \| | | | |
// | |  | | |___| |\  | |_| |
// |_|  |_|_____|_| \_|\___/ 
// 
// ====================================================================
// File: menu.cpp
// Description: 
// 
// ====================================================================

#include "menu.hpp"
#include "render_util.hpp"

void Menu::drawBackground(int winW, int winH) const {
    glPushAttrib(GL_ENABLE_BIT | GL_CURRENT_BIT | GL_TEXTURE_BIT | GL_LIGHTING_BIT);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.f, 0.f, 0.f, 0.7f);

    glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(winW, 0);
    glVertex2f(winW, winH);
    glVertex2f(0, winH);
    glEnd();

    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glPopAttrib();
}

void Menu::drawTitle() const {
    glColor3f(1, 1, 1);
    DrawTextCentered(m_cx, m_cy + m_gap, titleText(), GLUT_BITMAP_HELVETICA_18);
}

void Menu::drawItems() const {
    for (int i = 0; i < (int)m_items.size(); i++) {
        int y = m_cy - i * m_gap;

        if (i == m_selected) {
            glColor3f(1.0f, 1.0f, 0.2f);
            DrawText2D(m_cx - 80, y, ">", GLUT_BITMAP_HELVETICA_18);
        }
        else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }
        DrawTextCentered(m_cx, y, m_items[i].c_str(), GLUT_BITMAP_HELVETICA_18);
    }
}

void Menu::drawHint() const {
    glColor3f(0.8f, 0.8f, 0.8f);
    DrawText2D(10, 10, "Up/Down + Enter or click option. ESC to close.",
        GLUT_BITMAP_HELVETICA_12);
}