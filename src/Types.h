#pragma once
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Logger.h"

constexpr float START_SPEED = 400;
constexpr float UP_SPEED = 200;

using Point = sf::Vector2i;
using Rect = sf::IntRect;

template<typename StreamT>
StreamT& operator<<(StreamT& stream, Point p)
{
    stream << "{ " << p.x << ", " << p.y << "}\n";
    return stream;
}

template<typename StreamT>
StreamT& operator<<(StreamT& stream, Rect r)
{
    stream << "{ " << r.position.x << ", " << r.position.y << ", " << r.size.x << ", " << r.size.y<< "}\n";
    return stream;
}

template<typename T>
inline sf::Vector2f toFloat(const T& point) { return {(float)point.x, (float)point.y};}

template<typename T>
inline sf::Vector2i toInt(const T& point) { return {(int)point.x, (int)point.y};}

enum class Mood
{
    Neutral,
    Sad,
    Angry,
    Concerned,
    Frowning,
    Smiling,
    Last
};

std::vector<std::string>& GetAllMoods();

Mood ParseMood(std::string mood);
std::string ToString(Mood mood);

template<typename StreamT>
StreamT& operator<<(StreamT& stream, Mood mood)
{
    stream << ToString(mood);
    return stream;
}

struct KeybordState
{
    bool escapePressed = {false};
    bool rightPressed = {false};
    bool leftPressed = {false};
    bool upPressed = {false};
    bool downPressed = {false};
    bool spacePressed = {false};
    bool menuPressed = {false};
    bool MustProcessMove() {return rightPressed || leftPressed || upPressed || downPressed;}
};

enum class WheelScrolDirection
{
    WheelUp,
    WheelDown
};

struct MouseState
{
    bool rightButtonPressed = {false};
    bool leftButtonPressed = {false};
    sf::Vector2i position = {0, 0};
    sf::Time timestamp;
};

inline bool operator==(const MouseState& ls, const MouseState& rs)
{
    return (ls.rightButtonPressed == rs.rightButtonPressed)
        && (ls.leftButtonPressed == rs.leftButtonPressed)
        && (ls.position == rs.position)
        && (ls.timestamp == rs.timestamp);
}

inline bool operator!=(const MouseState& ls, const MouseState& rs)
{
    return !(ls == rs);
}

struct MouseScrollState
{
    WheelScrolDirection wheelDirection;
    sf::Time timestamp;
};

inline bool operator==(const MouseScrollState& ls, const MouseScrollState& rs)
{
    return (ls.wheelDirection == rs.wheelDirection) && (ls.timestamp == rs.timestamp);
}

inline bool operator!=(const MouseScrollState& ls, const MouseScrollState& rs)
{
    return !(ls == rs);
}

struct EventsState
{
    KeybordState keyboard;
    MouseState mouse;
    MouseScrollState scroll;
};

struct WindowState
{
    bool focusGained = {false};
    bool focusLost = {false};
    bool closed = {false};
};

struct MenuItemData
{
    std::shared_ptr<sf::Sprite> m_pic;
    std::string m_name;
    std::string m_infoText;
};
