#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include "MapCfg.h"
#include "Types.h"

class Map 
{
public:
    Map(const MapCfg& cfg, Point viewSize);
    void Draw(sf::RenderWindow& window);
    void DrawForeground(sf::RenderWindow& window);
    //void UpdateOnEvents(EventsState events, float elapsedTime, float moveDelta);
    bool UpdateOnPlayerMovement(Point playerCenter);

    void SetGlobalPos(Point pos){ m_leftTop = pos;}
    Rect GetViewBoundaries(){ return {m_leftTop, m_viewSize};}
    //Rect GetInsideRect(){ return m_insideRect;}
    Point GetMapSize(){ return m_mapSize;}

    std::vector<Rect>& GetWalls(){return m_cfg.walls;}

private:
    MapCfg m_cfg;
    
    const Point m_viewSize{0, 0};
    const Point m_viewHalfSize{0, 0};

    Point m_leftTop{0, 0};
    Point m_mapSize{0, 0};
    int m_mostLeftPossibleViewX;
    int m_mostBottomPossibleViewY;
  

    //Rect m_insideRect;

    std::shared_ptr<sf::Sprite> m_background;
    std::shared_ptr<sf::Sprite> m_foreground;
};

using MapPtr = std::shared_ptr<Map>;