#pragma once
#include <memory>
#include <unordered_map>
#include <string>

#include "SpriteCfg.h"
#include "MapCfg.h"
#include "PlayerCfg.h"
#include "CollectableCfg.h"

#include "npc.h"
#include "map.h"
#include "Player.h"
#include "Collectable.h"
#include "NonInteractableNpc.h"

class EntitiesManager;

template <typename CfgT, typename EntityT>
class OneTypeEntitiesManager
{
public:

    std::shared_ptr<EntityT> GetEntity(const CfgT& cfg, Point viewSize)
    {
       if(m_entities.find(cfg.name) == m_entities.end())
       {
           //std::cout << "Get Entity - creating entity with name " << cfg.name << "\n";
           m_entities[cfg.name] = std::make_shared<EntityT>(cfg, viewSize);
       }
       return m_entities[cfg.name];
    }
    std::shared_ptr<EntityT> GetEntity(std::string name)
    {
        if(m_entities.find(name) == m_entities.end())
        {
            //std::cout << "Get Entity - creating entity with name " << name << "\n";
            assert(false);
            return nullptr;
        }
        return m_entities[name];
    }

    void Clear()
    {
        m_entities.clear();
    }

private:
    friend class EntitiesManager;
    OneTypeEntitiesManager() = default;
    std::unordered_map<std::string, std::shared_ptr<EntityT>> m_entities;
};

using SpritesManager = OneTypeEntitiesManager<SpriteCfg, NPC>;
using MapsManager = OneTypeEntitiesManager<MapCfg, Map>;
using CollectablesManager = OneTypeEntitiesManager<CollectableCfg, Collectable>;
using NonInteractableNPCManager = OneTypeEntitiesManager<SpriteCfg, NonInteractableNPC>;
struct QuestCfg {};
class Quest {};
using QuestsManager = OneTypeEntitiesManager<QuestCfg, Quest>;

class EntitiesManager
{
public:

    static EntitiesManager& Get();
   
    SpritesManager& NPCS() { return m_npcsManager;}

    MapsManager& MAPS() { return m_mapsManager;}

    CollectablesManager& COLLECTABLES() { return m_collectablesManager;}

    QuestsManager& QUESTS() { return m_questsManager;}

    NonInteractableNPCManager& NONINTERACTABLES() { return m_nonInteractableNPCManagerManager;}
    
    std::shared_ptr<Player> GetPlayer(const PlayerCfg& cfg, Point viewSize);
    std::shared_ptr<Player> GetPlayer() {return m_player;};

    void Clear();
private:

   EntitiesManager() = default;

   std::shared_ptr<Player> m_player {nullptr};

   SpritesManager m_npcsManager;
   MapsManager m_mapsManager;
   CollectablesManager m_collectablesManager;
   QuestsManager m_questsManager;
   NonInteractableNPCManager m_nonInteractableNPCManagerManager;   
};