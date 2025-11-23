// ====================================================================
/* Robot Hunter
   Ricky Atkinson
   Computer Graphics, Fall 2025
   Kent State University                                             */
// ====================================================================
//  _____ _______  _______ _   _ ____  _____      
// |_   _| ____\ \/ /_   _| | | |  _ \| ____|     
//   | | |  _|  \  /  | | | | | | |_) |  _|       
//   | | | |___ /  \  | | | |_| |  _ <| |___      
//  _|_|_|_____/_/\_\ |_|  \___/|_|_\_\_____|___  
// |  \/  |  / \  | \ | |  / \  / ___| ____|  _ \ 
// | |\/| | / _ \ |  \| | / _ \| |  _|  _| | |_) |
// | |  | |/ ___ \| |\  |/ ___ \ |_| | |___|  _ < 
// |_|  |_/_/   \_\_| \_/_/   \_\____|_____|_| \_\
// 
// ====================================================================
// File: texture_manager.hpp
// Description: 
// 
// ====================================================================

#pragma once

#include "texture.h"
#include "logger.h"
#include <string>
#include <unordered_map>
#include <memory>

class TextureManager {
public:
	Texture& get(const std::string& path, bool generateMipmaps = false);

	bool preload(const std::string& path, bool generateMipmaps = false);

	void clear();

	bool loadDefault(const std::string& path);
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_cache;
	std::unique_ptr<Texture> m_default; // Fallback default texture
};