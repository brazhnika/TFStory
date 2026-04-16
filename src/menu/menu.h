#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "../Types.h"
#include "../controls/ClickableList.h"
#include "MenuCfg.h"
#include "../controls/Button.h"
#include "../controls/TextBox.h"
#include "../controls/PicBox.h"

class Menu
{
public:

    Menu(MenuCfg cgf, Point topLeft);

    void Draw(sf::RenderWindow& window);
    bool UpdateOnEvents(EventsState events, float elapsedTime);
    void SetUpData();//const std::vector<std::string>& collectables, const std::vector<std::string>& npcs,
       //const std::vector<std::string>& quests);

private:    
    Menu() = delete;
    void onItemChange(int newCurrentPos);
    void selectPage(int pageIdx);

    ClickableList m_list;
    TextBox m_textBox;
    PicBox m_picBox;
    std::array<Button, 3> m_topMenuButtons;

    int m_selectedBtn = 0;
    MouseState m_processedMouseState;

    std::vector<std::vector<MenuItemData>> m_data;
    int m_lastSelectedLine = -1;

};