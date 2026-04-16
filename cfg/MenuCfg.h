#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "ControlsCfg.h"
#include "../src/Types.h"


struct MenuCfg
{
    ListCfg listCfg;
    TextBoxCfg textBoxCfg;
    PicBoxCfg picBoxCfg;
    ButtonCfg btnCfg;

    int buttonCount;

    MenuCfg() = default;
    MenuCfg(std::string file);
};