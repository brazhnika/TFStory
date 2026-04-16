#include "Scene.h"
#include <cassert>
#include "ResourceManagers/EntitiesManager.h"

//#include "Tokenizer.h"



Scene::Scene(const SceneCfg& cfg, Point viewSize, const std::shared_ptr<Menu>& menu)
   : SceneCfg(cfg)
   , m_viewSize(viewSize)
   , m_menu(menu)
{
   m_map = EntitiesManager::Get().MAPS().GetEntity(cfg.map, viewSize);
   m_player = EntitiesManager::Get().GetPlayer(cfg.player, viewSize);
   const auto viewWindow = m_map->GetViewBoundaries();
   m_player->SetGlobalPos({playerX, playerY}, viewWindow);
   m_yOrderedSprites.insert(m_player->GetYPosListElem());
   // for(auto& npcCfg : sprites)
   // {
   //    m_npcs.push_back(EntitiesManager::Get().NPCS().GetEntity(npcCfg, viewSize));
   //    auto additionalInfo = spritesDialogs[npcCfg.name];
   //    m_npcs.back()->SetGlobalPos({additionalInfo.x, additionalInfo.y}, viewWindow);
   // }
   // for(auto& elem : collectables)
   // {
   //    m_npcs.push_back(EntitiesManager::Get().COLLECTABLES().GetEntity(elem.collectable, viewSize));
   //    m_npcs.back()->SetGlobalPos({elem.x, elem.y}, viewWindow);
   // }
   // for(auto& elem : nonInteractableSprites)
   // {
   //    m_npcs.push_back(EntitiesManager::Get().NONINTERACTABLES().GetEntity(elem, viewSize));
   //    auto additionalInfo = spritesDialogs[elem.name];
   //    m_npcs.back()->SetGlobalPos({additionalInfo.x, additionalInfo.y}, viewWindow);
   // }
   // m_npcs.push_back(m_player);

   for(auto& npcCfg : sprites)
   {
      auto sprite = EntitiesManager::Get().NPCS().GetEntity(npcCfg, viewSize);
      auto additionalInfo = spritesDialogs[npcCfg.name];
      sprite->SetGlobalPos({additionalInfo.x, additionalInfo.y}, viewWindow);
      m_yOrderedSprites.insert(sprite->GetYPosListElem());
   }
   for(auto& elem : collectables)
   {
      auto sprite = EntitiesManager::Get().COLLECTABLES().GetEntity(elem.collectable, viewSize);
      sprite->SetGlobalPos({elem.x, elem.y}, viewWindow);
      m_yOrderedSprites.insert(sprite->GetYPosListElem());
   }
   for(auto& elem : nonInteractableSprites)
   {
      auto sprite = EntitiesManager::Get().NONINTERACTABLES().GetEntity(elem, viewSize);
      auto additionalInfo = spritesDialogs[elem.name];
      sprite->SetGlobalPos({additionalInfo.x, additionalInfo.y}, viewWindow);
      m_yOrderedSprites.insert(sprite->GetYPosListElem());
   }
   m_yOrderedSprites.print(true);
   m_yOrderedSprites.print(false);
   //m_npcs.push_back(m_player);

   //SoundManager::Get().PlayBackground("backgroundScary");
}
void Scene::SetViewPos(Point pos)
{
    m_viewPos = pos;
    m_map->SetGlobalPos(pos);
}

void Scene::Draw(sf::RenderWindow& window)
{
   m_map->Draw(window);
   const auto viewWindow = m_map->GetViewBoundaries();
   
   for(auto sprite : m_yOrderedSprites)
      sprite->Draw(window, viewWindow);
 
   m_map->DrawForeground(window);

   if(m_dialog && m_dialog->Visible())
      m_dialog->Draw(window);
   else if (m_showMenu)
      m_menu->Draw(window);
}

void Scene::updateNPCsOnMapMove()
{
   const auto viewWindow = m_map->GetViewBoundaries();
   for(auto sprite : m_yOrderedSprites)
      sprite->UpdatePosOnCameraMove(viewWindow);
}

Interactable* Scene::updateNPCsOnEvents(EventsState events, float eTime)
{
   Interactable* lastClicked = nullptr;
   for(auto sprite : m_yOrderedSprites)
      if(sprite->UpdateOnEvents(events, eTime, m_map->GetMapSize(), m_yOrderedSprites, m_map->GetWalls()))
         lastClicked = sprite;
   return lastClicked;
}

void Scene::UpdateOnEvents(EventsState events, float eTime)
{
   if (m_dialog && m_dialog->Visible())
   {
      m_processedMouseState = events.mouse;
      if(events.keyboard.escapePressed || m_dialog->IsFinished())
         m_dialog->Hide();
      else
      {
         m_dialog->UpdateOnEvents(events, eTime);
         if(m_dialog->IsFinished())
            m_dialog->Hide();
      }
   }
   else
   {
      auto prevShowMenu = m_showMenu;
      if(events.keyboard.menuPressed && !m_showMenu)
         m_showMenu = true;

      if(events.keyboard.escapePressed && m_showMenu)
         m_showMenu = false;
      
      if(m_showMenu)
      {
         if(!prevShowMenu)
            m_menu->SetUpData();

         m_showMenu = !m_menu->UpdateOnEvents(events, eTime);
      }
      else
      {
         auto lastClicked = updateNPCsOnEvents(events, eTime);
         const auto playerCenter = m_player->GetGlobalBoundaries().getCenter();
         if(m_map->UpdateOnPlayerMovement(playerCenter))
            updateNPCsOnMapMove();

         if(lastClicked)
         {
            if(lastClicked->CanTalkTo() && lastClicked->GetTouched())
            {
               m_dialog = getDialog(lastClicked, events.mouse);
               m_dialog->Show();
            }
            else if (lastClicked->GetTouched() && lastClicked->CanBePickedUp())
            {
               m_player->Pickup(lastClicked->GetName());
               // auto it = std::remove_if(m_npcs.begin(), m_npcs.end(),
               //    [lastClicked](const auto& elem){ return lastClicked == elem;});
               // m_npcs.erase(it);
            }
         }
      }
   }

}

std::shared_ptr<Dlg> Scene::getDialog(Interactable* npcPtr, MouseState mouse)
{
   auto npc = npcPtr->GetName();
   assert(spritesDialogs.find(npc) != spritesDialogs.end());
   std::shared_ptr<Dlg> dlg = nullptr;

   if(m_dialogs.find(npc) == m_dialogs.end())
   {
      auto dialog = spritesDialogs[npc].dialog;
      dlg = std::make_shared<Dlg>(dialog, m_player, EntitiesManager::Get().NPCS().GetEntity(npc), mouse, Rect{m_viewPos, m_viewSize});
      m_dialogs[npc] = dlg;
      return dlg;
   }
   else
      return m_dialogs[npc];
}

void Scene::Shutdown()
{
   m_map.reset();
   m_player.reset();
   //m_npcs.clear();
   //m_drones.clear();
   m_dialogs.clear();
   m_dialog.reset();
}