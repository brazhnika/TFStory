#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "MenuCfg.h"
#include "ResourceManagers/TextureManager.h"
#include "../Types.h"
#include "ScrollBar.h"
#include "Tokenizer.h"


class ClickableList
{
public:
    ClickableList(ListCfg cfg, std::function<void(int)> onSelectedItemChange);
    void SetStrings(std::vector<std::string> strings);
    void UpdateOnEvents(EventsState events);
    void Draw(sf::RenderWindow& window);

    void Show();
    void Hide();
    bool Visible();
    void SetPos(Point pos);
    Point GetPos();
    sf::Vector2f GetSize();

private:

    void scroll(WheelScrolDirection direction);
    void onScaleChange(int newCurrentPos);
    void updateVisibleTextFonts();
    bool isValidListIndex(int idx);
    void scrollToNewFirstVisibleIndex(int newFirstVisibleIndex, bool scrollScrollBarToo);
 
    ListCfg m_cfg;
    bool m_visible = false;
    Rect m_listRect;
    int m_fontSize;
    sf::Sprite m_listBackground;
    ScrollBar m_scrollBar;
    Tokenizer m_tokenizer;

    int m_selectedItem;
    int m_hoveredItem;
    int m_firstVisibleStringIndex;
    int m_visibleStringsCount;
    MouseState m_previousMouseState;
    MouseScrollState m_previousScrollState;
    std::vector<std::string> m_strings;
    
    std::vector<sf::Text> m_visibleListItems;
    std::function<void(int)> m_onSelectedItemChange;
};