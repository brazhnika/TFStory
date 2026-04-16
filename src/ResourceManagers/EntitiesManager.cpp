#include <cassert>
#include "EntitiesManager.h"

EntitiesManager& EntitiesManager::Get()
{
    static EntitiesManager manager;
    return manager;
}

std::shared_ptr<Player> EntitiesManager::GetPlayer(const PlayerCfg& cfg, Point viewSize)
{
    if(!m_player) m_player = std::make_shared<Player>(cfg, viewSize);
    return m_player;
}

void EntitiesManager::Clear()
{
    m_player.reset();
    m_npcsManager.Clear();
    m_mapsManager.Clear();
    m_collectablesManager.Clear();
    m_questsManager.Clear();
    m_nonInteractableNPCManagerManager.Clear();   
}