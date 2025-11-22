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
// File: roundover_menu.hpp
// Description: 
// 
// ====================================================================

#pragma once
#include "menu.hpp"

class RoundOverMenu : public Menu {
public:
    RoundOverMenu() {
        setItems({ "RETRY", "RESUME VIEW", "EXIT" });
        setSelectedIndex(0);
    }

    void setSuccess(bool s) { m_success = s; }
    void setStats(std::string l1, std::string l2) {
        m_stat1 = std::move(l1);
        m_stat2 = std::move(l2);
    }

    void draw(int winW, int winH) const override;

protected:
    const char* titleText() const override { return "ROUND OVER"; };
    const char* hintText()  const override { return "Up/Down + Enter or click option."; };

private:
    bool m_success = false;
    std::string m_stat1, m_stat2;

};