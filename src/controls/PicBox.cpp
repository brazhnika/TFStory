#include "PicBox.h"
//#include <iostream>
    
PicBox::PicBox(PicBoxCfg cfg)
    : m_bkg(*TextureManager::Get().GetTexture(cfg.background))
{
    m_bkg.setTextureRect(cfg.picRect);
}

void PicBox::UpdateOnEvents(EventsState /*events*/, float /*elapsedTime*/)
{

}

void PicBox::Draw(sf::RenderWindow& window)
{
    window.draw(m_bkg);
    if(m_pic)
        window.draw(*m_pic);
}

void PicBox::SetSprite(std::shared_ptr<sf::Sprite> sprite)
{
    m_pic = sprite;
    centerPic();
}

void PicBox::SetPos(Point pos)
{
    auto floatPos = toFloat(pos);
    m_bkg.setPosition(floatPos);
    centerPic();
}

void PicBox::centerPic()
{
    if(!m_pic) return;
    
    auto center = m_bkg.getGlobalBounds().getCenter();
    const auto spriteSize = m_pic->getGlobalBounds();

    center.x -= spriteSize.size.x/2;
    center.y -= spriteSize.size.y/2;

    m_pic->setPosition(center);
}

sf::Vector2f PicBox::GetPos()
{
    return m_bkg.getGlobalBounds().position;
}

sf::Vector2f PicBox::GetSize()
{
    return m_bkg.getGlobalBounds().size;
}