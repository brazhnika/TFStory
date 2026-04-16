#include "EventDispatcher.h"

void EventDispatcher::HandleEvents(sf::RenderWindow& window)
{
    const auto onKeyPressed = [this](const sf::Event::KeyPressed& keyPressed){ processKeyPressed(keyPressed);};
    const auto onKeyReleased = [this](const sf::Event::KeyReleased& keyReleased){ processKeyReleased(keyReleased); };
    const auto onMousePressed = [this](const sf::Event::MouseButtonPressed& mousePressed){processMousePressed(mousePressed);};
    const auto onMouseReleased = [this](const sf::Event::MouseButtonReleased& mouseReleased){ processMouseReleased(mouseReleased);};
    const auto onMouseMoved = [this](const sf::Event::MouseMoved& mouseMoved){ processMouseMoved(mouseMoved);};
    const auto onClosed = [this](const sf::Event::Closed&){ m_closed = true;};
    const auto onMouseScrolled = [this](const sf::Event::MouseWheelScrolled& mouseScrolled){ processMouseScrolled(mouseScrolled);};
    
    window.handleEvents(onKeyPressed, onKeyReleased, onMousePressed, onMouseReleased, onMouseMoved, onClosed, onMouseScrolled);
}


void EventDispatcher::processKeyPressed(const sf::Event::KeyPressed& keyPressed)
{
    switch(keyPressed.scancode)
    {
    case sf::Keyboard::Scancode::Escape:
        m_keyboardState.escapePressed = true;
        break;
    case sf::Keyboard::Scancode::D:
        m_keyboardState.rightPressed = true;
        break;
    case sf::Keyboard::Scancode::A:
        m_keyboardState.leftPressed = true;
        break;
    case sf::Keyboard::Scancode::W:
        m_keyboardState.upPressed = true;
        break;
    case sf::Keyboard::Scancode::S:
        m_keyboardState.downPressed = true;
        break;
    case sf::Keyboard::Scancode::Space:
        m_keyboardState.spacePressed = true;
        break;
    case sf::Keyboard::Scancode::E:
        m_keyboardState.menuPressed = true;
        break;
     default:
        (void)5;
    };
}

void EventDispatcher::processKeyReleased(const sf::Event::KeyReleased& keyPressed)
{
    switch(keyPressed.scancode)
    {
    case sf::Keyboard::Scancode::Escape: 
        m_keyboardState.escapePressed = false;
        break;
    case sf::Keyboard::Scancode::D:
        m_keyboardState.rightPressed = false;
        break;
    case sf::Keyboard::Scancode::A:
        m_keyboardState.leftPressed = false;
        break;
    case sf::Keyboard::Scancode::W:
        m_keyboardState.upPressed = false;
        break;
    case sf::Keyboard::Scancode::S:
        m_keyboardState.downPressed = false;
        break;
    case sf::Keyboard::Scancode::Space:
        m_keyboardState.spacePressed = false;
        break;
    case sf::Keyboard::Scancode::E:
        m_keyboardState.menuPressed = false;
        break;
    default:
        (void)5;
            break;            
    };
}

void EventDispatcher::processMousePressed(const sf::Event::MouseButtonPressed& mousePressed)
{
    switch(mousePressed.button)
    {
        case sf::Mouse::Button::Left:
            m_mouseState.leftButtonPressed = true;
            break;
        case sf::Mouse::Button::Right:
            m_mouseState.rightButtonPressed = true;
            break;
        default:
            (void)4;
            break;
    };         
    m_mouseState.position = mousePressed.position;
    m_mouseState.timestamp = m_clock.getElapsedTime();
 }

 void EventDispatcher::processMouseReleased(const sf::Event::MouseButtonReleased& mouseReleased)
 {
    switch(mouseReleased.button)
    {
        case sf::Mouse::Button::Left:
            m_mouseState.leftButtonPressed = false;
            break;
        case sf::Mouse::Button::Right:
            m_mouseState.rightButtonPressed = false;
            break;
        default:
            (void)4;
            break;
    };  
    m_mouseState.position = mouseReleased.position;
    m_mouseState.timestamp = m_clock.getElapsedTime();    
 }

 void EventDispatcher::processMouseMoved(const sf::Event::MouseMoved& mouseMoved)
 {
    m_mouseState.position = mouseMoved.position;
 };

 void EventDispatcher::processMouseScrolled(const sf::Event::MouseWheelScrolled& mouseScrolled)
 {
    if(mouseScrolled.delta > 0)
       m_scrollState.wheelDirection = WheelScrolDirection::WheelUp;
    else if(mouseScrolled.delta < 0)
       m_scrollState.wheelDirection = WheelScrolDirection::WheelDown;
    m_scrollState.timestamp = m_clock.getElapsedTime();    
 }