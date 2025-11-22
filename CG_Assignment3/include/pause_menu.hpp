// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  ____   _   _   _ ____  _____     __  __ _____ _   _ _   _ 
// |  _ \ / \ | | | / ___|| ____|   |  \/  | ____| \ | | | | |
// | |_) / _ \| | | \___ \|  _|     | |\/| |  _| |  \| | | | |
// |  __/ ___ \ |_| |___) | |___    | |  | | |___| |\  | |_| |
// |_| /_/   \_\___/|____/|_____|___|_|  |_|_____|_| \_|\___/ 
//                             |_____|                        
// ====================================================================
// File: pause_menu.hpp
// Description: 
// 
// ====================================================================

#pragma once
#include "menu.hpp"

class PauseMenu : public Menu {
public:
	PauseMenu() {
		setItems({ "NEW GAME", "RESUME", "EXIT" });
		setSelectedIndex(1); // default highlight Resume
	};

protected:
	const char* titleText() const override { return "Paused"; }
	const char* hintText()  const override { return "Up/Down + Enter or click option. ESC to resume."; }
};