
#pragma once
#include <SFML/Graphics.hpp>
#include "Types.h"
#include <iostream>

class EventDispatcher
{
public:

    void HandleEvents(sf::RenderWindow& window);
    //KeybordState GetKeyboardState() { return m_keyboardState;}
    //MouseState GetMouseState() {return m_mouseState;}

    EventsState GetEvents(){ return {m_keyboardState, m_mouseState, m_scrollState};}
    bool IsClosed(){return m_closed; }

private:
    void processKeyPressed(const sf::Event::KeyPressed& keyPressed);
    void processKeyReleased(const sf::Event::KeyReleased& keyPressed);
    void processMousePressed(const sf::Event::MouseButtonPressed& mousePressed);
    void processMouseReleased(const sf::Event::MouseButtonReleased& mouseReleased);
    void processMouseMoved(const sf::Event::MouseMoved& mouseMoved);
    void processMouseScrolled(const sf::Event::MouseWheelScrolled& mouseScrolled);
private:
    KeybordState m_keyboardState;
    MouseState m_mouseState;
    MouseScrollState m_scrollState;
    bool m_closed{false};
    sf::Clock m_clock;
};