#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "ControlsCfg.h"
#include "ResourceManagers/TextureManager.h"
#include "../Types.h"
#include "Interactable.h"


class PicBox
{
public:
    PicBox(PicBoxCfg cfg);
    void UpdateOnEvents(EventsState events, float elapsedTime);
    void Draw(sf::RenderWindow& window);
    void SetSprite(std::shared_ptr<sf::Sprite> sprite);
    void SetPos(Point pos);
    sf::Vector2f GetPos();

    sf::Vector2f GetSize();

private:
 
    void centerPic();

    sf::Sprite m_bkg;
    std::shared_ptr<sf::Sprite> m_pic;
};