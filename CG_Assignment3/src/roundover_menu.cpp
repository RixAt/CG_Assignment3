// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____   ___  _   _ _   _ ____   _____     _______ ____  
// |  _ \ / _ \| | | | \ | |  _ \ / _ \ \   / / ____|  _ \ 
// | |_) | | | | | | |  \| | | | | | | \ \ / /|  _| | |_) |
// |  _ <| |_| | |_| | |\  | |_| | |_| |\ V / | |___|  _ < 
// |_| \_\\___/_\___/|_| \_|____/ \___/  \_/  |_____|_| \_\
// |  \/  | ____| \ | | | | |                              
// | |\/| |  _| |  \| | | | |                              
// | |  | | |___| |\  | |_| |                              
// |_|  |_|_____|_| \_|\___/       
//                        
// ====================================================================
// File: roundover_menu.cpp
// Description: 
// 
// ====================================================================

#include "roundover_menu.hpp"
#include "render_util.hpp"

void RoundOverMenu::draw(int winW, int winH) const {
    drawBackground(winW, winH);

    glColor3f(1, 1, 1);
    DrawText2D(m_cx - 70, m_cy + 110,
        m_success ? "MISSION SUCCESS" : "MISSION FAILED",
        GLUT_BITMAP_HELVETICA_18);

    DrawText2D(m_cx - 90, m_cy + 80, m_stat1.c_str(), GLUT_BITMAP_HELVETICA_12);
    DrawText2D(m_cx - 90, m_cy + 60, m_stat2.c_str(), GLUT_BITMAP_HELVETICA_12);

    drawItems();
    drawHint();
}
