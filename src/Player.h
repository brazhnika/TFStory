
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "npc.h"
#include "Collectable.h"



class Player : public NPC
{
public:
   Player(const SpriteCfg& cfg, Point viewSize);
   ~Player() override{};

   void Draw(sf::RenderWindow& window, Rect viewRect) override;
   
   bool UpdateOnEvents(EventsState events,
        float elapsedTime,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls) override;

   bool isBumpingIntoSmth(){return m_isBumpingIntoSmth;}
   
   void Pickup(std::string npc);
  
   const std::vector<std::string>& GetCollectedCollectablesInfo() const { return m_collectables;}
   const std::vector<std::string>& GetTalkedNPCsInfo() const { return m_talkedNPCs; }

   std::string GetName() const override { return "Player52234512974231235537354";}

   bool SetPickedUp() override { return false; }

private:
   
   void updateCurrentSpriteIndex();
   Point updatePositionOnKeyboardState(KeybordState keyboardState, float elapsedTime, Point mapSize);

   bool m_isBumpingIntoSmth{false};
   KeybordState m_lastKeyboardState;
   std::vector<std::string> m_collectables;
   std::vector<std::string>  m_talkedNPCs;

};

using PlayerPtr = std::shared_ptr<Player>;