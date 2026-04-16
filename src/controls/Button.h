#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "ControlsCfg.h"
#include "ResourceManagers/TextureManager.h"
#include "../Types.h"


class Button
{
public:
    Button(ButtonCfg cfg);
    void UpdateOnEvents(KeybordState /*keyboardState*/, MouseState mouseState, float /*elapsedTime*/);
    void Draw(sf::RenderWindow& window);

    void SetText(std::string str);
    void Show();
    void Hide();
    bool Visible();
    void SetPos(Point pos);
    sf::Vector2f GetPos();

    sf::Vector2f GetSize();
    bool IsClicked();

private:
 
    void centerText();

    enum class State
    {
        Normal,
        Hovered,
        Pressed,
        Last
    };

    static constexpr size_t allBtnStates = static_cast<size_t>(State::Last);

    ButtonCfg m_cfg;

    bool m_visible = false;
    bool m_beenClicked = false;
    State m_state{State::Normal};
    std::array<std::shared_ptr<sf::Sprite>, allBtnStates> m_background;

    sf::Text m_text;
};