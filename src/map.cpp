#include "map.h"
#include "ResourceManagers/TextureManager.h"
#include <cassert>
#include <iostream>

Map::Map(const MapCfg& cfg, Point viewSize)
   : m_cfg(cfg)
   , m_viewSize(viewSize)
   , m_viewHalfSize{viewSize.x/2, viewSize.y/2}
{
    m_background = std::make_shared<sf::Sprite>(*(TextureManager::Get().GetTexture(cfg.background)));
    m_foreground = std::make_shared<sf::Sprite>(*(TextureManager::Get().GetTexture(cfg.foreground)));
    auto textureSize = m_background->getTexture().getSize();
    m_mapSize = {(int)textureSize.x, (int)textureSize.y};
    m_mostLeftPossibleViewX = m_mapSize.x - m_viewSize.x;
    m_mostBottomPossibleViewY = m_mapSize.y - m_viewSize.y;
    //m_insideRect = {{m_viewSize.x/2, m_viewSize.y/2}, {m_mapSize.x - m_viewSize.x, m_mapSize.y - m_viewSize.y}};
    assert(m_viewSize.x <= m_mapSize.x);
    assert(m_viewSize.y <= m_mapSize.y);
}

void Map::Draw(sf::RenderWindow& window)
{
    window.draw(*m_background);
}

void Map::DrawForeground(sf::RenderWindow& window)
{
    window.draw(*m_foreground);
}


bool Map::UpdateOnPlayerMovement(Point playerCenter)
{
    auto newLeftTop = m_leftTop;
    newLeftTop = playerCenter - m_viewHalfSize;
    if(newLeftTop.x < 0)
        newLeftTop.x = 0;
    else if(newLeftTop.x + m_viewSize.x > m_mapSize.x)
        newLeftTop.x = m_mostLeftPossibleViewX;

    if(newLeftTop.y < 0)
        newLeftTop.y = 0;
    else if(newLeftTop.y + m_viewSize.y > m_mapSize.y)
        newLeftTop.y = m_mostBottomPossibleViewY;

    if(m_leftTop != newLeftTop)
    {
        m_leftTop = newLeftTop;
        const auto viewRect = GetViewBoundaries();
        m_background->setTextureRect(viewRect);
        m_foreground->setTextureRect(viewRect);
        return true;
    }
    return false;
}

// void Map::UpdateOnEvents(EventsState events, float /*elapsedTime*/, float moveDelta)
// {
//     if(!events.keyboard.MustProcessMove()) return;

//     auto delta = moveDelta;
//     const auto viewRightBottom = m_leftTop + m_viewSize;
//     if(events.keyboard.rightPressed && (viewRightBottom.x < m_mapSize.x))
//     {
//        m_leftTop.x += delta;
//        //std::cout << "move right new m_leftTop.x " << m_leftTop.x << "\n";
//     }
//     else if(events.keyboard.leftPressed)
//     {
//         m_leftTop.x -= delta;
//         if(m_leftTop.x  < 0)
//             m_leftTop.x  = 0;

//         //std::cout << "move left new m_leftTop.x " << m_leftTop.x << "\n";
//     }

//     if(events.keyboard.upPressed)
//     {
//         m_leftTop.y -= delta;
//         if(m_leftTop.y  < 0)
//            m_leftTop.y  = 0;
//         //std::cout << "move up new m_leftTop.y " << m_leftTop.y << "\n";
//     }
//     else if(events.keyboard.downPressed && (viewRightBottom.y < m_mapSize.y))
//     {
//         m_leftTop.y += delta;
//         //std::cout << "move down  new m_leftTop.y " << m_leftTop.y << "\n";
//     }
   
//     auto viewRect = GetViewBoundaries();
//     m_background->setTextureRect(viewRect);
//     m_foreground->setTextureRect(viewRect);
// }
