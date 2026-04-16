
#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include "Interactable.h"

class NonInteractableNPC : public SpriteCfg, public Interactable
{
public:
   NonInteractableNPC(const SpriteCfg& cfg, Point viewSize);
   ~NonInteractableNPC() override {};

   void Draw(sf::RenderWindow& window, Rect viewRect) override;
   
   // void UpdatePosition(float elapsedTime, Point mapSize,
   //      const std::vector<InteractablePtr>& npcs,
   //      const std::vector<Rect>& walls);

   bool UpdateOnEvents(EventsState events,
        float elapsedTime,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls) override;

   std::string GetName() const override { return name;}
   std::string GetInfo() override { return initialMenuInfo; }        
   bool SetPickedUp() override { return false; }
private:
   void updateCurrentSpriteIndex();
   Point updatePosition(float elapsedTime, Point mapSize);

   void updateMovement();

   float m_passedTime = 0;
   int m_secToKeepCurrentMovement;
   int m_speed;
   int m_dx;
   int m_dy;
   int m_lastDir{0};

};

using NonInteractableNPCPtr = std::shared_ptr<NonInteractableNPC>;
