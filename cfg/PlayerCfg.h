#pragma once

#include "SpriteCfg.h"


struct PlayerCfg: public SpriteCfg
{
    PlayerCfg() = default;
    PlayerCfg(std::string fileName);
};