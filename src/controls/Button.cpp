#include "Button.h"
#include "ResourceManagers/FontManager.h"
#include "utils.h"
#include <iostream>
    
Button::Button(ButtonCfg cfg)
    : m_cfg(cfg)
    , m_visible(false)
    , m_state(State::Normal)
    , m_text(FillFromFont(cfg.font.normal))
{
    auto buttonBckTexture = TextureManager::Get().GetTexture(cfg.pic);
    const auto buttonWidth = static_cast<int>(cfg.rect.size.x / allBtnStates);

    for(size_t i = 0; i < allBtnStates; ++i)
    {
        m_background[i] = std::make_shared<sf::Sprite>(*buttonBckTexture);
        m_background[i]->setTextureRect({{cfg.rect.position.x + buttonWidth*(int)i, cfg.rect.position.y}, {buttonWidth, (int)cfg.rect.size.y}});
        m_background[i]->setPosition({0.f,0.f});
    }
}

void Button::UpdateOnEvents(KeybordState /*keyboardState*/, MouseState mouseState, float /*elapsedTime*/)
{
    auto oldState = m_state;
    if(!m_background[static_cast<int>(m_state)]->getGlobalBounds().contains(toFloat(mouseState.position)))
    {
        m_state = State::Normal;
        FillFromFont(m_text, m_cfg.font.normal);
        m_beenClicked = oldState == State::Pressed;
        return;
    }
    
    if(mouseState.leftButtonPressed)
    {
        m_state = State::Pressed;
        FillFromFont(m_text, m_cfg.font.selected);
    }
    else
    {
        m_state = State::Hovered;
        FillFromFont(m_text, m_cfg.font.hovered);
    }

    centerText();
    m_beenClicked = (oldState == State::Pressed) && !mouseState.leftButtonPressed;
}

void Button::Draw(sf::RenderWindow& window)
{
    if(!m_visible) return;

    window.draw(*m_background[static_cast<int>(m_state)]);
    window.draw(m_text);
}

void Button::SetText(std::string str)
{
    m_text.setString(str);
    centerText();
}
void Button::Show() {m_visible = true;}
void Button::Hide() {m_visible = false;}
bool Button::Visible() {return m_visible;}
sf::Vector2f Button::GetSize() {return m_background[static_cast<int>(m_state)]->getGlobalBounds().size;}
bool Button::IsClicked() {return m_beenClicked;}

void Button::SetPos(Point pos)
{
    //std::cout << "Button::SetPos " << pos <<"\n";
    auto floatP = toFloat(pos);
    for(size_t i = 0; i < allBtnStates; ++i)
        m_background[i]->setPosition(floatP);

    centerText();
}

sf::Vector2f Button::GetPos()
{
    return m_background[0]->getPosition();
}

void Button::centerText()
{
    auto textPos = m_text.getGlobalBounds();
    auto backgroundLeftTop = m_background[static_cast<int>(m_state)]->getGlobalBounds();
    auto backgroundCenter = backgroundLeftTop.getCenter();
    auto additionalOffset = m_state == State::Pressed ? 2 : 0;
    m_text.setPosition({backgroundCenter.x - textPos.size.x/2 + additionalOffset,
        backgroundLeftTop.position.y + 9 + additionalOffset});

    //std::cout << "Button::  center { " << backgroundCenter.x <<"," <<backgroundCenter.y<<"}, "
    //<<  m_text.getGlobalBounds().position.x << " "<< m_text.getGlobalBounds().position.y <<"\n";
}
