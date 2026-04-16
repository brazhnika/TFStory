#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <unordered_map>

class Tokenizer
{
public:
    Tokenizer(const sf::Font& font, unsigned int characterSize, int maxLineSize);
    std::vector<std::string> Tokenize(std::string str) const;
    std::vector<std::string> SplitIntoLines(std::string str);
    std::string CutWordToASize(const std::string& str);
private:
    
    float getCharacterSize(char c);

    const sf::Font m_font;
    const unsigned int m_characterSize;
    const  int m_maxLineSize;
    std::unordered_map<char, size_t> m_sizes;    
};