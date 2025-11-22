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
// File: intro_menu.hpp
// Description: 
// 
// ====================================================================

#pragma once

#include "menu.hpp"

class IntroMenu : public Menu {
public:
    IntroMenu() {
        setItems({ "START" });
        setSelectedIndex(0);
    }

    void setLines(std::vector<std::string> lines) {
        m_lines = std::move(lines);
    }

    void draw(int winW, int winH) const override;

protected:
    const char* titleText() const override { return "ROBOT HUNTER"; }
    const char* hintText()  const override { return "Press I / Enter to start, ESC for menu."; }

private:
    std::vector<std::string> m_lines;
};