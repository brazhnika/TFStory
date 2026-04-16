#include "Types.h"
#include <string>
#include <iostream>

namespace
{
    std::vector<std::string> sc_moods = {
    "neutral",
    "sad",
    "angry",
    "concerned",
    "frowning",
    "smiling"
    };

}

std::vector<std::string>& GetAllMoods()
{
    return sc_moods;
}

Mood ParseMood(std::string mood)
{
    const auto size_of_moods = sc_moods.size();
    assert(sc_moods.size() == static_cast<int>(Mood::Last));

    for(size_t i = 0; i < size_of_moods; ++i)
    {
        if(sc_moods[i] == mood)
            return static_cast<Mood>(i);
    }    
    Logger::GetLogger().Error("Uknown sprite mood %s", mood);
    assert(false);
    return Mood::Neutral;
}


std::string ToString(Mood mood)
{
    assert(sc_moods.size() == static_cast<int>(Mood::Last));
    return sc_moods[static_cast<int>(mood)];
}
