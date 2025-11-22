// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  __  __ _____ _   _ _   _ __  __    _    _   _    _    ____ _____ ____  
// |  \/  | ____| \ | | | | |  \/  |  / \  | \ | |  / \  / ___| ____|  _ \ 
// | |\/| |  _| |  \| | | | | |\/| | / _ \ |  \| | / _ \| |  _|  _| | |_) |
// | |  | | |___| |\  | |_| | |  | |/ ___ \| |\  |/ ___ \ |_| | |___|  _ < 
// |_|  |_|_____|_| \_|\___/|_|  |_/_/   \_\_| \_/_/   \_\____|_____|_| \_\
// 
// ====================================================================
// File: menu_manager.hpp
// Description: 
// 
// ====================================================================

#pragma once
#include "menu.hpp"

class MenuManager {
public:
	void setActive(Menu* m, int winW, int winH) {
		active = m;
		if (active) {
			active->layout(winW, winH);
		}
	}

	void clear() { active = nullptr; };

	bool hasActive() const { return active != nullptr; };
	Menu* getActive() { return active; };

	void layout(int winW, int winH) {
		if (active) active->layout(winW, winH);
	};

	void draw(int winW, int winH) const {
		if (active) active->draw(winW, winH);
	};

	void onArrowUp() {
		if (active) active->onArrowUp();
	};

	void onArrowDown() {
		if (active) active->onArrowDown();
	};

	bool onEnter() {
		if (active) return active->onEnter();
		return false;
	};

	bool onMouseClick(int mouseX, int mouseYTopLeft, int winH) {
		if (active) return active->onMouseClick(mouseX, mouseYTopLeft, winH);
		return false;
	};

	void onMouseMove(int x, int y, int winH) {
		if (active) active->onMouseMove(x, y, winH);
	};


private:
	Menu* active = nullptr;
};