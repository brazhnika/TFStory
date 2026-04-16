#include <iostream>
#include "Config.h"
#include "inicpp.hpp"


Config& Config::GetConfig()
{
   static Config s_cfg{};
   return s_cfg;
}

Config::Config()
{
   Logger::GetLogger().Debug("config creation");

   inicpp::IniManager iniReader("./../cfg/config.ini");

   const int count = iniReader["main"]["count"].get<int>();
   Logger::GetLogger().Debug("reading count ");
   for(int i = 0; i < count; ++i)
      m_scenes.emplace_back(iniReader[std::to_string(i)]["file"].get<std::string>());
}
