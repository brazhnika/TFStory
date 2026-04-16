#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "ControlsCfg.h"
#include "ResourceManagers/TextureManager.h"
#include "Tokenizer.h"
#include "ScrollBar.h"


class TextBox
{
public:
    TextBox(TextBoxCfg cfg);
    void SetTextToPlay(std::string text, bool playSound = true, bool showWholeTextAtOnce = false);
    void UpdateOnEvents(EventsState events, float /*elapsedTime*/);
    void Draw(sf::RenderWindow& window);
    void ShowWholeTextAtOnce();
    void Stop();
    void Start();
    //bool IsFinishedPlaying() {return repliesDisplayer->IsFinishedPlaying();}
    bool IsFinishedPlaying() {return m_finishedPlaying;}
    void SetPos(Point pos);
private:

    void onScaleChange(int newCurrentPos);
    void scroll(WheelScrolDirection direction);    
    bool isValidListIndex(int idx);
    void scrollToNewFirstVisibleIndex(int newFirstVisibleIndex, bool scrollScrollBarToo);

    TextBoxCfg m_cfg;
    ScrollBar m_scrollBar;
    std::shared_ptr<sf::Sprite> m_textBackground;
    bool m_playOneByOneLetter{true};
    sf::Clock m_Clock;

    std::vector<sf::Text> m_lines;
    std::vector<std::string> m_linesToPlay;
    std::vector<std::string> m_playedLines;

    size_t m_currentPlayedLine = 0;
    Tokenizer m_tokenizer;
    //int m_maxLineSize;
    //size_t m_maxLinesCount;
    bool m_finishedPlaying = false;
    bool m_playing;
    int m_firstVisibleStringIndex;
    int m_visibleStringsCount;
    MouseState m_previousMouseState;
    MouseScrollState m_previousScrollState;    
};