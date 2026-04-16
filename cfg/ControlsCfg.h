#pragma once
#include <string>
#include <SFML/Graphics.hpp>
#include "FontCfg.h"
#include "../src/Types.h"


struct ButtonCfg
{
    std::string pic;
    Rect rect;
    FontsCfg font;
    ButtonCfg() = default;
    ButtonCfg(std::string file);
};

struct ScrollBarCfg
{
    std::string textureFile;
    Rect scrollBar;
    Rect scrollBarBall;
    Rect scrollBarBallSelected;
    ScrollBarCfg() = default;
    ScrollBarCfg(std::string file);    
};

struct ListCfg
{
    std::string textureFile;
    Rect list;
    ScrollBarCfg scrollBarCfg;
    FontsCfg font;
    ListCfg() = default;
    ListCfg(std::string file);
};

struct TextBoxCfg
{
    std::string pic;
    FontsCfg font;
    Rect textRect;
    ScrollBarCfg scrollBarCfg;

    TextBoxCfg() = default;
    TextBoxCfg(std::string file);
};

struct PicBoxCfg
{
    std::string background;
    Rect picRect;
    
    PicBoxCfg() = default;
    PicBoxCfg(std::string file);
};
