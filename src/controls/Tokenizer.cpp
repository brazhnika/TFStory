#include "Tokenizer.h"
#include <iostream>

// void Tokenizer::Init(const std::shared_ptr<sf::Font>& font, unsigned int characterSize, int maxLineSize)
// {
//     assert(font && (characterSize > 0) && (maxLineSize > 0));
//     m_font = font;
//     m_characterSize = characterSize;
//     m_maxLineSize = maxLineSize;
//     m_initialized = true;
// }

Tokenizer::Tokenizer(const sf::Font& font, unsigned int characterSize, int maxLineSize)
    : m_font(font)
    , m_characterSize(characterSize)
    , m_maxLineSize(maxLineSize)
{
    assert((characterSize > 0) && (maxLineSize > 0));
}


std::vector<std::string> Tokenizer::Tokenize(std::string str) const
{
    std::vector<std::string> allWords;
    if(str.empty())
       return allWords;

    std::string currentWord;
    int i = 0;
    size_t start = 0;
    while(start < str.size())
    {
        auto nextSpace = str.find_first_of(' ', start);
         if(nextSpace == str.npos)
        {
            allWords.emplace_back(str.substr(start, str.size() - start));
            start = str.size();
        }
        else
        {
            allWords.emplace_back(str.substr(start, nextSpace - start));
            start = nextSpace + 1;
        }
    }
    return allWords;
}


std::vector<std::string> Tokenizer::SplitIntoLines(std::string str)
{
    std::vector<std::string> lines = {""};
    auto words = Tokenize(str);
    auto spaceSize = getCharacterSize(' ');
    float currentLineSize = 0;
    int currentLine = 0;
    for(auto& word : words)
    {
        float wordSize = 0;
        for_each(word.begin(), word.end(), [&wordSize, this](const auto& symbol){ wordSize += getCharacterSize(symbol);});

        if((currentLineSize + wordSize) < m_maxLineSize)
        {
            if(currentLineSize > 0)
            {
                lines[currentLine].append(" ");
                currentLineSize += spaceSize;
            }
            currentLineSize += wordSize;
            lines[currentLine].append(word);
        }
        else
        {
            std::cout << lines.back() << "\n";
            ++currentLine;
            currentLineSize = wordSize;
            lines.push_back(word);
        }
    }
    return lines;
}

std::string Tokenizer::CutWordToASize(const std::string& str)
{
   auto twoPointSize = 2 * getCharacterSize('.');
   auto wordSize = 0;
   auto resultLength = 0;
   for(auto ch : str)
   {
       auto currentCharSize = getCharacterSize(ch);
       auto currentCharSizePluxDots = getCharacterSize(ch) + twoPointSize;
       if((wordSize + currentCharSizePluxDots) < m_maxLineSize)
       {
           wordSize += currentCharSize;
           ++resultLength;
       }
   }
   auto result = str.substr(0, resultLength);
   result += "..";
   return result;
}

float Tokenizer::getCharacterSize(char c)
{
    if(m_sizes.find(c) == m_sizes.end())
    {
        const auto& glyph = m_font.getGlyph(c, m_characterSize, false, 0.0f);
        m_sizes[c] = glyph.advance;
    }
    return m_sizes[c];
}