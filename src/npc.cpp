#include "npc.h"
#include "ResourceManagers/TextureManager.h"
#include "ResourceManagers/RandomGenerator.h"
#include "ResourceManagers/SoundManager.h"
#include <iostream>

namespace
{
   const float START_SPEED = 200;
};

NPC::NPC(const SpriteCfg& cfg, Point viewSize, bool dialogIsOnTheLeftSide)
   : SpriteCfg(cfg)
   , Interactable(cfg.base, cfg.small.standStart, cfg.small.standEnd, false)
{
   int allMoodsCount = static_cast<int>(Mood::Last);
   int allBigPicFrames = cfg.bigPicFrames != allMoodsCount ? cfg.bigPicFrames : allMoodsCount;
   auto bigSpriteTexture = TextureManager::Get().GetTexture(cfg.picFile);
   const auto [width, height] = bigSpriteTexture->getSize();
   
   m_moodSpriteSize.y = height;
   m_moodSpriteSize.x = width / allBigPicFrames;
   
   auto bigPicPos = dialogIsOnTheLeftSide ? toFloat(Point{0, viewSize.y - m_moodSpriteSize.y})
      : toFloat(Point{viewSize.x - m_moodSpriteSize.x, viewSize.y - m_moodSpriteSize.y});
      
   for(int i = 0; i < allBigPicFrames; ++i)
   {
        m_moodSprites.emplace_back(*bigSpriteTexture);
        m_moodSprites.back().setTextureRect({ {i * m_moodSpriteSize.x, 0}, m_moodSpriteSize});
        m_moodSprites.back().setPosition(bigPicPos);
   }
}

void NPC::ChangeMood(Mood mood, bool silent)
{
   m_currentMood = static_cast<Mood>(static_cast<int>(mood) % m_moodSprites.size());

   if(!silent)
      SoundManager::Get().PlaySound(moodsSounds[static_cast<int>(mood)]);
}

void NPC::Draw(sf::RenderWindow& window, Rect viewRect)
{
   Interactable::Draw(window, viewRect);
}

void NPC::DrawDialogSprite(sf::RenderWindow& window)
{
    assert(m_moodSprites.size() > static_cast<size_t>(m_currentMood));
    window.draw(m_moodSprites[static_cast<int>(m_currentMood)]);
}
