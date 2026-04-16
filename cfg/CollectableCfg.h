#pragma once
#include <string>
#include "SpriteCfg.h"

struct CollectableCfg
{
    std::string name;
    BaseAnimationInfo base;
    std::string initialMenuInfo;
    CollectableCfg(std::string file);
};