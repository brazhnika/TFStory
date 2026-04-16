#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "MenuCfg.h"
#include "ResourceManagers/TextureManager.h"
#include "../Types.h"


class ScrollBar
{
public:
    ScrollBar(ScrollBarCfg cfg, std::function<void(int)> onScaleChange);
    void SetScaleSize(int count);
    void UpdateOnEvents(EventsState events);
    void Draw(sf::RenderWindow& window);

    void Show();
    void Hide();
    bool Visible();

    void SetPos(Point pos);
    void Scroll(int direction);
    sf::Vector2f GetRightTopPos();
    sf::Vector2f GetSize();

private:
    void setBallPositionByIndex(int index);
    void setBallPosByYAndRecalculateIndex(float y);
    //void scroll(int dy);

    ScrollBarCfg m_cfg;
    std::function<void(int)> m_onScaleChangeHandler;
    sf::Sprite m_scrollBar;
    sf::Sprite m_ball;
    sf::Sprite m_selectedBall;
    int m_ballOffsetX;
    int m_maxLocalScrollBarBalY;
    int m_positionCount;
    int m_currentPosition;
    int m_dYBetweenPositions;
    std::vector<int> m_ballPositions;
    bool m_visible;
    bool m_ballHovered;
    bool m_grabbed;
    MouseState m_previousMouseState;
};