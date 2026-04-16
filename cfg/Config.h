#pragma once

#include "SceneCfg.h"


class Config
{
public:
    static Config& GetConfig();
    std::vector<SceneCfg> m_scenes;
private:
    Config();
};