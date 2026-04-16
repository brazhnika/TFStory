#include "MenuCfg.h"
#include "inicpp.hpp"
#include "Logger.h"

MenuCfg::MenuCfg(std::string file)
{
    Logger::GetLogger().Info("Reading MenuCfg cfg from " + file);
    inicpp::IniManager iniReader(file);

    listCfg = ListCfg(iniReader["general"]["listBox"].get<std::string>());
    textBoxCfg = TextBoxCfg(iniReader["general"]["textBox"].get<std::string>());
    picBoxCfg = PicBoxCfg(iniReader["general"]["picBox"].get<std::string>());
    btnCfg = ButtonCfg(iniReader["general"]["buttons"].get<std::string>());
    buttonCount = iniReader["general"]["buttonCount"].get<int>();
}