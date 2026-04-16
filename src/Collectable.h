#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "CollectableCfg.h"
#include "Interactable.h"


class Collectable : public CollectableCfg, public Interactable
{
public:
   Collectable(const CollectableCfg& cfg, Point viewSize);
   ~Collectable() override{}

   std::string GetName() const override { return name;}
   std::string GetInfo() override { return initialMenuInfo; }
   bool CanBePickedUp() const override {return true;}
   bool SetPickedUp() override
   {
      m_pickedUp = true;
      m_visible = false;
      return true;
   }

   bool UpdateOnEvents(EventsState events,
        float elapsedTime,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls) override;
        
protected:
};

using CollectablePtr = std::shared_ptr<Collectable>;