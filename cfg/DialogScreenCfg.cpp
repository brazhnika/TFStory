#include "DialogScreenCfg.h"
#include "inicpp.hpp"
#include "Logger.h"


DialogScreenCfg::DialogScreenCfg(std::string file)
{
    Logger::GetLogger().Info("Reading DialogScreenCfg cfg from " + file);       
    inicpp::IniManager iniReader(file);
    background = iniReader["general"]["background"].get<std::string>();
    buttonCfg = ButtonCfg(iniReader["general"]["buttons"].get<std::string>());
    textBox = TextBoxCfg(iniReader["general"]["textBox"].get<std::string>());
}