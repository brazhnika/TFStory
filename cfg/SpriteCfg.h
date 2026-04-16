#pragma once
#include <string>
#include <unordered_map>
#include "inicpp.hpp"
#include "../src/Types.h"
#include "FontCfg.h"

struct BaseAnimationInfo
{
    std::string picFile;
    std::string selection;
    FontsCfg fonts;
    int frames;
    int delay;
    Point selectionOffset;
    void Fill(inicpp::IniManager& iniReader);
};

struct AnimationInfo
{
    int standStart;
    int standEnd;
    int leftStart;
    int leftEnd;
    int rightStart;
    int rightEnd;
    int downStart;
    int downEnd;
    int upStart;
    int upEnd;
};

struct SpriteCfg
{
    std::string name;
    BaseAnimationInfo base;
    std::string initialMenuInfo;

    AnimationInfo small;
    std::string picFile;
    //std::unordered_map<Mood, int> moodsOffset;
    int bigPicFrames;
    std::vector<std::string> moodsSounds;

    SpriteCfg() = default;
    SpriteCfg(std::string fileName);
};

std::string ToString(const SpriteCfg& cfg);
