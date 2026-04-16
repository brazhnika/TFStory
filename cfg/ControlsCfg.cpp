#include "ControlsCfg.h"
#include "inicpp.hpp"
#include "ParseUtils.h"
#include <iostream>


ButtonCfg::ButtonCfg(std::string file)
{
    inicpp::IniManager iniReader(file);
    pic = iniReader["general"]["pic"].get<std::string>();
    font = ReadFontConfig(iniReader);
    rect = readRect(iniReader, "general", "");
}

ScrollBarCfg::ScrollBarCfg(std::string file)
{
    inicpp::IniManager iniReader(file);
    textureFile = iniReader["general"]["pic"].get<std::string>();
    scrollBar = readRect(iniReader, "general", "scrollBar");
    scrollBarBall = readRect(iniReader, "general", "scrollBarBall");
    scrollBarBallSelected = readRect(iniReader, "general", "scrollBarBallSelected");
}

ListCfg::ListCfg(std::string file)
{
    inicpp::IniManager iniReader(file);
    textureFile = iniReader["general"]["pic"].get<std::string>();
    font = ReadFontConfig(iniReader);
    scrollBarCfg = ScrollBarCfg(iniReader["general"]["scrollBar"].get<std::string>());
    list = readRect(iniReader, "general", "list");
}

TextBoxCfg::TextBoxCfg(std::string file)
{
    inicpp::IniManager iniReader(file);
    pic = iniReader["general"]["pic"].get<std::string>();
    font = ReadFontConfig(iniReader);
    textRect = readRect(iniReader, "general", "Text");
    scrollBarCfg = ScrollBarCfg(iniReader["general"]["scrollBar"].get<std::string>());
    //std::cout << " textRect " << textRect <<"\n";
}

PicBoxCfg::PicBoxCfg(std::string file)
{
    inicpp::IniManager iniReader(file);
    background = iniReader["general"]["pic"].get<std::string>();
    picRect = readRect(iniReader, "general", "");
    //std::cout << " picRect " << picRect <<"\n";
}