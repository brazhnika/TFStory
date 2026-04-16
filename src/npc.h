#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "SpriteCfg.h"
#include "Interactable.h"


class NPC : public SpriteCfg, public Interactable
{
public:
   NPC(const SpriteCfg& cfg, Point viewSize, bool dialogIsOnTheLeftSide = false);
   ~NPC() override {};
   std::string GetName() const override { return name;}
   std::string GetInfo() override { return initialMenuInfo; }

   void ChangeMood(Mood mood, bool silent = false);

   void DrawDialogSprite(sf::RenderWindow& window) override;

   bool CanTalkTo() const override {return true;}

   void Draw(sf::RenderWindow& window, Rect viewRect) override;

   bool SetPickedUp() override { return false; }

protected:
 
   std::vector<sf::Sprite> m_moodSprites;
   Mood m_currentMood{Mood::Neutral};
   Point m_moodSpriteSize;
};


using NPCPtr = std::shared_ptr<NPC>;