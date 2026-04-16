#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "ControlsCfg.h"
#include "FontCfg.h"
#include "Types.h"


struct DialogScreenCfg
{
    std::string background;
    ButtonCfg buttonCfg;
    TextBoxCfg textBox;
    
    DialogScreenCfg() = default;
    DialogScreenCfg(std::string file);
};