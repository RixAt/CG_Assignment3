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
    if (it != m_cache.end())
        return *it->second;

    auto tex = std::make_unique<Texture>();
    if (!tex->loadFromFile(path, generateMipmaps)) {
        std::cerr << "[TextureManager] Failed to load " << path << "\n";
        // Still store it so we don't spam reload attempts
    }

    Texture& ref = *tex;
    m_cache[path] = std::move(tex);
    return ref;
};

void TextureManager::preload(const std::string& path, bool generateMipmaps) {
    get(path, generateMipmaps);
}

void TextureManager::clear() {
    m_cache.clear();
}