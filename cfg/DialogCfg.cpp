#include <sstream>
#include "DialogCfg.h"
#include "inicpp.hpp"
#include <exception>
#include "ParseUtils.h"
#include "Logger.h"

DialogCfg::DialogCfg(std::string fileName)
{
    Logger::GetLogger().Info("Reading DialogCfg cfg from " + fileName);       
    inicpp::IniManager iniReader(fileName);
    int count = iniReader["general"]["count"].get<int>();
    screen = DialogScreenCfg(iniReader["general"]["screen"].get<std::string>());

    for(int i = 0; i < count; ++i)
    {
        const auto id = std::to_string(i);
        Trade trade;
        trade.id = i;
        trade.isPlayer = iniReader[id]["is_player"].get<int>() > 0;
        trade.shortReply = iniReader[id]["short"].String();
        trade.longReply = iniReader[id]["long"].String();
        trade.mood = ParseMood(iniReader[id]["mood"].String());
        trade.repliesIds = ParseListOfInts(iniReader[id]["replies"].String());
        assert(trade.repliesIds.size() >= 1);
        for(auto& replyId : trade.repliesIds)
        {
            //std::cout << " replyId " << replyId << " count " << count << "\n";
            assert(replyId < count);
        }
        trade.finish = iniReader[id]["finish"].get<int>() > 0;
        trade.quest = iniReader[id].isKeyExist("quest") ? iniReader[id]["quest"].get<std::string>() : "";
        trades.push_back(trade);
    }
}

std::string ToString(const Trade& cfg)
{
    std::stringstream ss;
    ss  << "Trade {";
    ss  << "id: " << cfg.id
        << ", isPlayer: " << (cfg.isPlayer ? "true" : "false")
        << ", shortReply: " << cfg.shortReply
        << ", longReply: " << cfg.longReply
        << ", mood: " << ToString(cfg.mood)
        << "{ ";
    for(auto id : cfg.repliesIds)
    {
        ss << id << ",";
    }
    ss  << "}, finish: " << (cfg.finish ? "true" : "false")
        << ", quest: " << cfg.quest << "}\n";
    return ss.str();
}

std::string ToString(const DialogCfg& cfg)
{
    std::stringstream ss;
    ss << "DialogCfg {";
    for(const auto& trade : cfg.trades)
       ss << ToString(trade);

    ss << "}\n";
    return ss.str();
}