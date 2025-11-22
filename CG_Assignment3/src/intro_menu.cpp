// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ___ _   _ _____ ____   ___  
// |_ _| \ | |_   _|  _ \ / _ \ 
//  | ||  \| | | | | |_) | | | |
//  | || |\  | | | |  _ <| |_| |
// |___|_| \_|_|_| |_| \_\\___/ 
// |  \/  | ____| \ | | | | |   
// | |\/| |  _| |  \| | | | |   
// | |  | | |___| |\  | |_| |   
// |_|  |_|_____|_| \_|\___/     
//                        
// ====================================================================
// File: intro_menu.cpp
// Description: 
// 
// ====================================================================

#include "intro_menu.hpp"
#include "render_util.hpp"

void IntroMenu::draw(int winW, int winH) const {
    drawBackground(winW, winH);

    glColor3f(1, 1, 1);
    DrawTextCentered(m_cx, m_cy + 100, titleText(), GLUT_BITMAP_HELVETICA_18);

    int y = m_cy - 155;
    for (auto& s : m_lines) {
        DrawTextCentered(m_cx, y, s.c_str(), GLUT_BITMAP_HELVETICA_12);
        y -= 18;
    }

    // Draw single "START" item using base list rendering
    drawItems();
    drawHint();
}
