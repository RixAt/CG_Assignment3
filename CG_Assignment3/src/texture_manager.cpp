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
// File: texture_manager.cpp
// Description: 
// 
// ====================================================================

#include "texture_manager.hpp"
#include <iostream>

Texture& TextureManager::get(const std::string& path, bool generateMipmaps) {
    auto it = m_cache.find(path);
    if (it != m_cache.end()) {
        if (!it->second) {
            if (!m_default) {
                m_default = std::make_unique<Texture>();
                m_default->loadFromFile("assets/textures/default_texture.png", false);
            }
            return *m_default;
        }
        return *it->second;
    }

    auto tex = std::make_unique<Texture>();

    if (!tex->loadFromFile(path, generateMipmaps)) {
        //std::cerr << "[TextureManager] Failed to load " << path << "\n";
        LOG_WARN("Texture load failed: " + path + " -> falling back to default");
        if (!m_default) {
            m_default = std::make_unique<Texture>();
            m_default->loadFromFile("assets/textures/default_texture.png", false);
        }
        m_cache[path] = nullptr;

        return *m_default;
    }

    LOG_INFO("Texture loaded: " + path);

	// Store in cache and return reference
    Texture& ref = *tex;
    m_cache[path] = std::move(tex);
    return ref;
};

// preload(): Preload texture into cache
// Returns true if successful, false if failed (uses default)
bool TextureManager::preload(const std::string& path, bool generateMipmaps) {
    Texture& tex = get(path, generateMipmaps);
    return (&tex != m_default.get());
}

// clear(): Clear texture cache
void TextureManager::clear() {
    m_cache.clear();
}

// loadDefault(): Load default fallback texture
// Returns true if successful, false if failed
bool TextureManager::loadDefault(const std::string& path) {
    m_default = std::make_unique<Texture>();
    if (!m_default->loadFromFile(path, false)) {
        LOG_ERROR("Failed to load default texture: " + path);
        m_default.reset();
        return false;
	}

    LOG_INFO("Default texture loaded: " +  path);
    return true;
}