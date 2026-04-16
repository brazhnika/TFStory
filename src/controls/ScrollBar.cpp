#include "ClickableList.h"
#include "utils.h"
#include <iostream>

namespace
{
    constexpr int BETWEEN_ITEMS_SPACE = 3;
    constexpr int BORDER_OFFSET = 5;
}

ScrollBar::ScrollBar(ScrollBarCfg cfg, std::function<void(int)> onScaleChange)
    : m_cfg(cfg)
    , m_onScaleChangeHandler(onScaleChange)
    , m_scrollBar(*TextureManager::Get().GetTexture(cfg.textureFile))
    , m_ball(*TextureManager::Get().GetTexture(cfg.textureFile))
    , m_selectedBall(*TextureManager::Get().GetTexture(cfg.textureFile))
    , m_positionCount(0)
    , m_currentPosition(0)
    , m_visible(false)
    , m_ballHovered(false)
    , m_grabbed(false)
{
    m_scrollBar.setTextureRect(cfg.scrollBar);
    m_ball.setTextureRect(cfg.scrollBarBall);
    m_selectedBall.setTextureRect(cfg.scrollBarBallSelected);
    m_ballOffsetX = ((int)cfg.scrollBarBall.size.x >> 2) - ((int)cfg.scrollBar.size.x >> 2);
    m_maxLocalScrollBarBalY = cfg.scrollBar.size.y - cfg.scrollBarBall.size.y;
}

void ScrollBar::SetScaleSize(int count)
{
    assert(count > 1);
    m_positionCount = count;
    m_currentPosition = 0;
    m_dYBetweenPositions = m_cfg.scrollBar.size.y / (count - 1);
    m_ballPositions.clear();
    for(int i = 0; i < m_positionCount - 1; ++i)
        m_ballPositions.emplace_back(i * m_dYBetweenPositions);
    
    m_ballPositions.emplace_back(m_maxLocalScrollBarBalY);     
}

void ScrollBar::UpdateOnEvents(EventsState events)
{
    if(!m_visible || m_previousMouseState == events.mouse)
       return;
    m_ballHovered = false;

    auto floatPos = toFloat(events.mouse.position);
    if(m_grabbed || m_ball.getGlobalBounds().contains(floatPos))
    {
        if(!m_grabbed && events.mouse.leftButtonPressed)
            m_grabbed = true;
        else if(m_grabbed && !events.mouse.leftButtonPressed)
            m_grabbed = false;
        
        m_ballHovered = true;
        if(m_grabbed)
            setBallPosByYAndRecalculateIndex(events.mouse.position.y);
    }
    else  if (m_scrollBar.getGlobalBounds().contains(floatPos) && events.mouse.leftButtonPressed)
    {
        auto currentBallPosY = m_ball.getGlobalBounds().position.y;
        setBallPositionByIndex(m_currentPosition + (floatPos.y < currentBallPosY ? -1 : 1));
    }
    m_previousMouseState = events.mouse;
}

void ScrollBar::Draw(sf::RenderWindow& window)
{
    if(!m_visible) return;
    window.draw(m_scrollBar);
    if(m_ballHovered) window.draw(m_selectedBall);
    else window.draw(m_ball);
}

void ScrollBar::Show(){ assert(m_positionCount > 0); m_visible = true;}
void ScrollBar::Hide() { m_visible = false;}
bool ScrollBar::Visible() {return m_visible;}

void ScrollBar::SetPos(Point pos)
{
    auto floatPos = toFloat(pos);
    m_scrollBar.setPosition(floatPos);
    m_ball.setPosition(floatPos);
    m_selectedBall.setPosition(floatPos);
    m_currentPosition = 0;
}

sf::Vector2f ScrollBar::GetRightTopPos()
{
    const auto bounds = m_ball.getGlobalBounds();
    return sf::Vector2f{{bounds.position.x + bounds.size.x + 5}, {m_scrollBar.getGlobalBounds().position.y}};
}

sf::Vector2f ScrollBar::GetSize()
{
    return {m_ball.getGlobalBounds().size.x, m_scrollBar.getGlobalBounds().size.y};
}

void ScrollBar::Scroll(int direction)
{
    setBallPositionByIndex(m_currentPosition + direction);
}

void ScrollBar::setBallPositionByIndex(int index)
{
    std::cout << "ScrollBar::setBallPositionByIndex  m_currentPosition "  << m_currentPosition <<"\n";
    m_currentPosition = index;

    if(m_currentPosition < 0) m_currentPosition = 0;
    else if(m_currentPosition >= m_positionCount) m_currentPosition = m_positionCount - 1;
    std::cout << "ScrollBar::setBallPositionByIndex  m_currentPosition after correction "  << m_currentPosition <<"\n";
    auto scrollbarRect = m_scrollBar.getGlobalBounds();
    scrollbarRect.position.y += m_ballPositions[m_currentPosition];

    m_ball.setPosition(scrollbarRect.position);
    m_selectedBall.setPosition(scrollbarRect.position);
}

void ScrollBar::setBallPosByYAndRecalculateIndex(float y)
{
    const auto oldCurrentPosition = m_currentPosition;
    const auto scrollbarRect = m_scrollBar.getGlobalBounds();
    auto newY = y;
    if(newY < scrollbarRect.position.y)
    {
        newY = scrollbarRect.position.y;
        m_currentPosition = 0;
    }
    else if (newY > (scrollbarRect.position.y + m_maxLocalScrollBarBalY))
    {
        newY = scrollbarRect.position.y + m_maxLocalScrollBarBalY;
        m_currentPosition = m_positionCount - 1;
    }
    else
        m_currentPosition  = (newY - scrollbarRect.position.y) / m_dYBetweenPositions;

    
    m_ball.setPosition({scrollbarRect.position.x, newY});
    m_selectedBall.setPosition(m_ball.getPosition());

    if(oldCurrentPosition != m_currentPosition)
        m_onScaleChangeHandler(m_currentPosition);
}
