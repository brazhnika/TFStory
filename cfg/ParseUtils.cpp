#include "ParseUtils.h"

std::vector<int> ParseListOfInts(std::string str)
{
    std::vector<int> vec;
    if(str.empty())
        return vec;
    size_t i = 0;
    while(i < str.size())
    {
        while((i < str.size()) && (str[i] == ' ')) ++i;

        int num = 0;
        while((i < str.size() ) && (str[i] != ','))
        {
            int digit = str[i] - '0';

            if((digit < 0) || (digit > 9))
                std::invalid_argument("Invalid reply list" + str);
            num = num * 10 + digit;
            ++i;
        }
        vec.push_back(num);
        ++i;
    }
    return vec;
 }


sf::Color ParseColor(std::string str)
{
    auto ints = ParseListOfInts(str);
    assert(ints.size() == 4);
    return sf::Color(ints[0], ints[1], ints[2], ints[3]);
}


Rect readRect(inicpp::IniManager& iniReader, std::string sectionName, std::string rectName)
{
    auto topX = iniReader[sectionName][rectName + "TopX"].get<int>();
    auto topY = iniReader[sectionName][rectName + "TopY"].get<int>();
    auto bottomX = iniReader[sectionName][rectName + "BottomX"].get<int>();
    auto bottomY = iniReader[sectionName][rectName + "BottomY"].get<int>();
    assert(topX < bottomX);
    assert(topY < bottomY);
    return {{topX, topY}, {bottomX - topX, bottomY - topY}};
}