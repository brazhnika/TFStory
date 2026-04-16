#include "SpriteCfg.h"
#include <string>
#include <sstream>
#include "Logger.h"


void BaseAnimationInfo::Fill(inicpp::IniManager& iniReader)
{
    picFile = iniReader["general"]["pic"].get<std::string>();
    selection = iniReader["general"]["selection"].get<std::string>();
    frames = iniReader["general"]["frames"].get<int>();
    delay = iniReader["general"]["delay"].get<int>();
    selectionOffset.x = iniReader["general"]["selectionOffsetX"].get<int>();
    selectionOffset.y = iniReader["general"]["selectionOffsetY"].get<int>();
    fonts = ReadFonts(iniReader["general"]["font"].get<std::string>());
}

SpriteCfg::SpriteCfg(std::string fileName)
{
    Logger::GetLogger().Info("Reading Sprite cfg from " + fileName);
    
    inicpp::IniManager iniReader(fileName);

    base.Fill(iniReader);
    
    initialMenuInfo = iniReader["initialInfo"]["info"].get<std::string>();
    
    small.standStart = iniReader["small"]["standStart"].get<int>();
    small.standEnd = iniReader["small"]["standEnd"].get<int>();

    small.leftStart = iniReader["small"]["leftStart"].get<int>();
    small.leftEnd = iniReader["small"]["leftEnd"].get<int>();

    small.rightStart = iniReader["small"]["rightStart"].get<int>();
    small.rightEnd = iniReader["small"]["rightEnd"].get<int>();

    small.downStart = iniReader["small"]["downStart"].get<int>();
    small.downEnd = iniReader["small"]["downEnd"].get<int>();

    small.upStart = iniReader["small"]["upStart"].get<int>();
    small.upEnd = iniReader["small"]["upEnd"].get<int>();

    picFile = iniReader["big"]["pic"].get<std::string>();
    bigPicFrames = iniReader["big"]["frames"].get<int>();

    /*for(const auto& mood : GetAllMoods())
    {
        assert(iniReader.isSectionExists(mood));
        Mood currentMood = ParseMood(mood);
        moodsOffset.emplace(currentMood, iniReader[mood]["id"].get<int>());
    }*/

    //for(const auto& mood : GetAllMoods())
    for(int i = 0; i < static_cast<int>(Mood::Last); ++i)
    {
        moodsSounds.push_back(iniReader["big"][ToString(static_cast<Mood>(i))].get<std::string>());
    }
}

std::string ToString(const SpriteCfg& cfg)
{
    std::stringstream ss;
    ss << "SpriteCfg {"<<"name: " << cfg.name
        << ", smallPic: " << cfg.base.picFile
        //<< ", smallframes: " << cfg.small.frames       
        << ", smalldelay: " << cfg.base.delay
        << ", picFile: " << cfg.picFile;
    //for(auto elem : cfg.moodsOffset)
     //   ss << ", mood: " << elem.first << ", offset: " << elem.second;
    ss << "}\n";
    return ss.str();
}