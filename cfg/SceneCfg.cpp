#include <sstream>

#include "SceneCfg.h"
#include "inicpp.hpp"
#include "Logger.h"
#include <unordered_map>

namespace
{
    template<typename StreamT>
    StreamT& operator<<(StreamT& stream, const AdditionalSpriteInfo& info)
    {
        stream << "{"<< "x= " << info.x << ", y= " << info.y << ", " << ToString(info.dialog) << "}";
        return stream;
    }
}


SceneCfg::SceneCfg(std::string fileName)
{
    std::unordered_map<std::string, int> namesCount;

    auto countName = [&namesCount](const std::string& name)
    {
        if(namesCount.find(name) == namesCount.end())
            namesCount[name] = 1;
        else
            namesCount[name] += 1;

        if(namesCount[name] > 1)
        {
           Logger::GetLogger().Error("Duplicate sprite with name " + name);
           throw std::invalid_argument("Duplicate sprite with name " + name);
        }            
    };
    

    Logger::GetLogger().Info("Reading SceneCfg cfg from " + fileName);
    inicpp::IniManager iniReader(fileName);
    map = MapCfg(iniReader["common"]["map"].get<std::string>());
    player = PlayerCfg(iniReader["player"]["file"].get<std::string>());
    playerX = iniReader["player"]["x"].get<int>();
    playerY = iniReader["player"]["y"].get<int>();

    countName(player.name);

    for(int i = 0; i < iniReader["sprites"]["count"].get<int>(); ++i)
    {
        const auto id = std::to_string(i);
        const auto spriteFile = iniReader[id]["file"].get<std::string>();
        sprites.emplace_back(spriteFile);
        sprites.back().name = iniReader[id]["name"].get<std::string>();
        countName(sprites.back().name);

        spritesDialogs[sprites.back().name] = {
            iniReader[id]["x"].get<int>(),
            iniReader[id]["y"].get<int>(),
            DialogCfg(iniReader[id]["dialog"].get<std::string>())
        };
    }

    for(int i = 0; i < iniReader["collectables"]["count"].get<int>(); ++i)
    {
        const auto id = std::to_string(i);
        collectables.push_back(
            {iniReader["collectables"]["x" + id].get<int>(),
            iniReader["collectables"]["y" + id].get<int>(),
            CollectableCfg(iniReader["collectables"]["col" + id].get<std::string>())});
        collectables.back().collectable.name = iniReader["collectables"]["name" + id].get<std::string>();
        countName(collectables.back().collectable.name);
    }

    for(int i = 0; i < iniReader["NonInteractableNPCs"]["count"].get<int>(); ++i)
    {
        const auto id = std::to_string(i);
        const auto sectionId = "non" + id;
        const auto spriteFile = iniReader[sectionId]["file"].get<std::string>();
        nonInteractableSprites.emplace_back(spriteFile);
        nonInteractableSprites.back().name = iniReader[sectionId]["name"].get<std::string>();
        spritesDialogs[nonInteractableSprites.back().name] = {
            iniReader[sectionId]["x"].get<int>(),
            iniReader[sectionId]["y"].get<int>(), DialogCfg()};
        countName(nonInteractableSprites.back().name);            
    }
    
};


std::string ToString(const SceneCfg& cfg)
{
    std::stringstream ss;
    ss << "SceneCfg {"//<<"background: " << cfg.background
       << ", player: " << ToString(cfg.player)
       << ", playerX: " << cfg.playerX      
       << ", playerY: " << cfg.playerY;
    for(const auto& elem : cfg.sprites)
       ss << ToString(elem);
    for(const auto& elem: cfg.spritesDialogs)
       ss << "DialogName: " << elem.first << elem.second;

    ss << "}\n";
    return ss.str();
}