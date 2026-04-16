#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include "DialogCfg.h"
#include "SpriteCfg.h"
#include "PlayerCfg.h"
#include "MapCfg.h"
#include "CollectableCfg.h"
#include "../src/Types.h"

struct AdditionalSpriteInfo
{
    int x;
    int y;
    DialogCfg dialog;
};

struct CollectableInfo
{
    int x;
    int y;
    CollectableCfg collectable;
};

struct SceneCfg
{
    MapCfg map;
    PlayerCfg player;
    int playerX;
    int playerY;
    std::vector<SpriteCfg> sprites;
    std::vector<SpriteCfg> nonInteractableSprites;    
    std::unordered_map<std::string, AdditionalSpriteInfo> spritesDialogs;
    std::vector<CollectableInfo> collectables;
    SceneCfg() = default;
    SceneCfg(std::string fileName);
};

std::string ToString(const SceneCfg& cfg);