#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>
#include <memory>


class SoundManager
{
public:
   static SoundManager& Get()
   {
       static SoundManager manager;
       return manager;
   }

   void PlaySound(std::string sound, bool loop = false);
   void StopSound(std::string sound);   
   void StopLastSound();
   void PlayBackground(std::string back);
   void StopLastBackground();
   void Clear();

private:
   struct Sound
   {
       std::shared_ptr<sf::SoundBuffer> buffer;
       std::shared_ptr<sf::Sound> sound;   
   };

   SoundManager() = default;

   std::unordered_map<std::string, Sound> m_music;
   std::unordered_map<std::string, Sound> m_sounds;

   std::string m_lastSound;
   std::string m_lastBackground;
};
