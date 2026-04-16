#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <unordered_map>

class TextureManager
{
public:
   static TextureManager& Get();

   std::shared_ptr<sf::Texture> GetTexture(std::string str);

   void Clear();

private:

   TextureManager() = default;
   std::unordered_map<std::string, std::shared_ptr<sf::Texture>> m_textures;
};