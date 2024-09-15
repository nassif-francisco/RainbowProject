#include <SFML/Graphics.hpp>
#include <map>
#include <string>

#include "texture_manager.hpp"

void TextureManager::loadTexture(const std::string& name, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    this->textures[name] = tex;

    return;
}

void TextureManager::loadBrushTexture(const std::string& name, const std::string& filename)
{
    /* Load the texture */
    sf::Texture tex;
    tex.loadFromFile(filename);

    /* Add it to the list of textures */
    this->brushTextures[name] = tex;

    return;
}

sf::Texture& TextureManager::getRef(const std::string& texture)
{
    return this->textures.at(texture);
}

sf::Texture& TextureManager::getBrushRef(const std::string& texture)
{
    return this->brushTextures.at(texture);
}