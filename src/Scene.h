#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <unordered_map>
#include "SceneCfg.h"
#include "map.h"
#include "Player.h"
#include "npc.h"
#include "Collectable.h"
#include "dlg/Dlg.h"
#include "NonInteractableNpc.h"
#include "menu/menu.h"
#include "SortedSpritesList.h"


class Scene : public SceneCfg
{
public:
    Scene(const SceneCfg& cfg, Point viewSize, const std::shared_ptr<Menu>& menu);

    void Draw(sf::RenderWindow& window);
    void UpdateOnEvents(EventsState events, float elapsedTime);
    void UpdateBeforeDrawing();
    void SetViewPos(Point pos);

    void Shutdown();
private:
   void updateNPCsOnMapMove();
   Interactable* updateNPCsOnEvents(EventsState events, float eTime);   
   DlgPtr getDialog(Interactable* npc, MouseState mouse);

   Point m_viewPos{0, 0};
   Point m_viewSize{0, 0};

   MapPtr m_map;

   PlayerPtr m_player;
   //std::vector<InteractablePtr> m_npcs;

   //std::vector<std::pair<InteractablePtr>> m_tempNpcs;

   //std::vector<NonInteractableNPCPtr> m_drones;
   std::unordered_map<std::string, DlgPtr> m_dialogs;
   DlgPtr m_dialog;
   bool showDialog = false;
   MouseState m_processedMouseState;
   KeybordState m_processedKeyboardState;   
   bool m_showMenu = false;
   std::shared_ptr<Menu> m_menu;
   SortedSpritesList<Interactable> m_yOrderedSprites;
};