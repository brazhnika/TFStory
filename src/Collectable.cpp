#include "Collectable.h"

Collectable::Collectable(const CollectableCfg& cfg, Point /*viewSize*/)
   : CollectableCfg(cfg)
   , Interactable(cfg.base, 0, cfg.base.frames - 1, true)
{
}


bool Collectable::UpdateOnEvents(EventsState events,
        float elapsedTime,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls)
{
   if(m_pickedUp || !m_touched) return false;

   if(events.mouse.leftButtonPressed && m_smallSprites[m_data.m_index].getGlobalBounds().contains(toFloat(events.mouse.position)))
   {
      std::cout << " Collectable " << GetName() << "clicked \n";
      m_pickedUp = true;
      m_visible = false;
      return true;
   }
   return false;
   //return Interactable::UpdateOnEvents(events, elapsedTime, mapSize, npcs, walls);
}
        