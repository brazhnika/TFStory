#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include "DialogCfg.h"
#include "SpriteCfg.h"
#include "PlayerCfg.h"
#include "../src/Types.h"
#include "DialogScreenCfg.h"

struct Trade
{
    int id;

    bool isPlayer{false};
    std::string shortReply;
    std::string longReply;
    Mood mood;
    std::vector<int> repliesIds;
    bool finish;
    std::string quest;
};

struct DialogCfg
{
    std::vector<Trade> trades;
    DialogScreenCfg screen;
    
    DialogCfg() = default;
    DialogCfg(const DialogCfg&) = default;
    DialogCfg& operator=(const DialogCfg&) = default;
    DialogCfg(std::string fileName);

    Rect getTextRect(){ return screen.textBox.textRect; }
};

std::string ToString(const Trade& cfg);

std::string ToString(const DialogCfg& cfg);

