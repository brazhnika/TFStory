#include "CollectableCfg.h"

#include <sstream>

#include "inicpp.hpp"


CollectableCfg::CollectableCfg(std::string fileName)
{
    inicpp::IniManager iniReader(fileName);
    base.Fill(iniReader);
    initialMenuInfo = iniReader["general"]["info"].String();
    // name = iniReader["general"]["name"].get<std::string>();
    // base.picFile = iniReader["general"]["pic"].get<std::string>();
    // base.selection = iniReader["general"]["selection"].get<std::string>();
    // base.frames = iniReader["general"]["frames"].get<int>();
    // base.delay = iniReader["general"]["delay"].get<int>();
}