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

#ifndef MENU_HPP
#define MENU_HPP

#include <vector>
#include <string>
#include <functional>

struct Rect {
	int x;
	int y;
	int width;
	int height;

	bool contains(int px, int py) const {
		return (px >= x) && (px <= x + width) &&
			   (py >= y) && (py <= y + height);
	}
};

class Menu {
public:
	Menu() = default;
	virtual ~Menu() = default;

	void setItems(const std::vector<std::string>& items) {
		m_items = std::move(items);
		m_itemRects.resize(m_items.size());
		m_actions.resize(m_items.size());
		if (m_selected >= (int)m_items.size()) m_selected = (int)m_items.size() - 1;
		if (m_selected < 0) m_selected = 0;
	};

	void setAction(int index, std::function<void()> fn) {
		if (index >= 0 && index < static_cast<int>(m_actions.size())) {
			m_actions[index] = std::move(fn);
		}
	};

	void setSelectedIndex(int index) {
		if (m_items.empty()) { m_selected = 0; return; }
		if (index < 0) index = 0;
		if (index >= (int)m_items.size()) index = (int)m_items.size() - 1;
		m_selected = index;
	};

	int selectedIndex() const { return m_selected; }

	virtual void layout(int winW, int winH) {
		m_cx = winW / 2;
		m_cy = winH / 2 + 50;
		m_gap = 40;

		const int rectW = 320;
		const int rectH = 30;

		for (int i = 0; i < (int)m_items.size(); i++) {
			int itemY = m_cy - i * m_gap;
			m_itemRects[i] = { m_cx - rectW / 2, itemY - rectH / 2, rectW, rectH };
		}
	};

	virtual void draw(int winW, int winH) const {
		drawBackground(winW, winH);
		drawTitle();
		drawItems();
		drawHint();
	};

	virtual void onArrowUp() {
		if (m_items.empty()) return;
		m_selected = (m_selected + (int)m_items.size() - 1) % (int)m_items.size();
	}

	virtual void onArrowDown() {
		if (m_items.empty()) return;
		m_selected = (m_selected + 1) % (int)m_items.size();
	}

	virtual bool onEnter() {
		if (m_selected >= 0 && m_selected < (int)m_actions.size() && m_actions[m_selected]) {
			m_actions[m_selected]();
			return true;
		}
		return false;
	}

	// mouseYTopLeft is GLUT mouse y (top-left origin)
	virtual bool onMouseClick(int mouseX, int mouseYTopLeft, int winH) {
		int mouseY = winH - mouseYTopLeft; // convert to bottom-left origin
		for (int i = 0; i < (int)m_itemRects.size(); i++) {
			if (m_itemRects[i].contains(mouseX, mouseY)) {
				m_selected = i;
				return onEnter();
			}
		}
		return false;
	}
protected:
	virtual const char* titleText() const { return "Menu"; }
	virtual const char* hintText()  const { return "Up/Down + Enter or click option. ESC to close."; }
	virtual void drawBackground(int winW, int winH) const;
	virtual void drawTitle() const;
	virtual void drawItems() const;
	virtual void drawHint() const;

	int m_cx = 0;
	int m_cy = 0;
	int m_gap = 40;

	std::vector<std::string> m_items;
	std::vector<Rect> m_itemRects;
	std::vector<std::function<void()>> m_actions;
	int m_selected = 0;
};

#endif // !MENU_HPP



