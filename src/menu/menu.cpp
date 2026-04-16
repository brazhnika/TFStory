#include <iostream>
#include "menu.h"
#include "ResourceManagers/TextureManager.h"
#include "ResourceManagers/EntitiesManager.h"
#include "ResourceManagers/FontManager.h"

namespace 
{
    constexpr int LIST_START_X = 10;
    constexpr int LIST_START_Y = 13;
    constexpr int LIST_END_X = 130;
    constexpr int LIST_END_Y = 240;
    constexpr int ITEMS_COUNT = 6;
    constexpr int LIST_ADDITIONAL_OFFSET = 10;

    constexpr int BETWEEN_BTNS_OFFSET = 10;
    constexpr int BETWEEN_ELEMS_OFFSET = 4;
}

Menu::Menu(MenuCfg cfg, Point topLeft)
   : m_list(cfg.listCfg, [this](int pos){ onItemChange(pos);} )
   , m_textBox(cfg.textBoxCfg)
   , m_picBox(cfg.picBoxCfg)
   , m_topMenuButtons({Button(cfg.btnCfg), Button(cfg.btnCfg), Button(cfg.btnCfg)})
{
    m_topMenuButtons[0].SetPos(topLeft);
    m_topMenuButtons[0].SetText("BackPack");
    m_topMenuButtons[0].Show();
    const Point btnOffset{(int)m_topMenuButtons[0].GetSize().x + BETWEEN_BTNS_OFFSET, 0};
    auto nextBPos = topLeft + btnOffset;
    m_topMenuButtons[1].SetPos(nextBPos);
    m_topMenuButtons[1].SetText("Bots");
    m_topMenuButtons[1].Show();

    nextBPos += btnOffset;
    m_topMenuButtons[2].SetPos(nextBPos);
    m_topMenuButtons[2].SetText("Quests");
    m_topMenuButtons[2].Show();

    nextBPos = topLeft + Point{0, (int)m_topMenuButtons[0].GetSize().y + BETWEEN_ELEMS_OFFSET};
    m_list.SetPos(nextBPos);
    m_list.Show();


    nextBPos += {(int)m_list.GetSize().x + BETWEEN_ELEMS_OFFSET, 0};
    m_picBox.SetPos(nextBPos);

    nextBPos += {(int)m_picBox.GetSize().x + BETWEEN_ELEMS_OFFSET, 0};
    m_textBox.SetPos(nextBPos);
    m_textBox.ShowWholeTextAtOnce();
}


bool Menu::UpdateOnEvents(EventsState events, float elapsedTime)
{
    if(events.keyboard.escapePressed) return true;

    m_list.UpdateOnEvents(events);
    m_textBox.UpdateOnEvents(events, elapsedTime);

    if(m_processedMouseState == events.mouse)
        return false;
    
    m_processedMouseState = events.mouse;

    auto clickedBtn = -1;
    for(size_t i = 0; i < m_topMenuButtons.size(); ++i)
    {
        m_topMenuButtons[i].UpdateOnEvents(events.keyboard, events.mouse, elapsedTime);
        if(m_topMenuButtons[i].IsClicked())
        {
            clickedBtn = i;
            std::cout << " Clicked btn num " << i << "\n";
        }
    }

    if(clickedBtn >= 0)
    {
        selectPage(clickedBtn);
        return false;
    }

    return false;    
}

void Menu::selectPage(int pageIdx)
{
    std::cout << "Selected page " << pageIdx << "\n";
    m_selectedBtn = pageIdx;
    std::vector<std::string> strings;
    auto pageData = m_data[pageIdx];
    for(auto& item : pageData)
    {
        strings.push_back(item.m_name);
        std::cout << " List item to set " << strings.back() << "\n";
    }
    m_list.SetStrings(strings);
    if(pageData.size() > 0)
    {
        m_lastSelectedLine = 0;
        m_textBox.SetTextToPlay(pageData[m_lastSelectedLine].m_infoText, false);
        m_textBox.ShowWholeTextAtOnce();
        m_picBox.SetSprite(pageData[m_lastSelectedLine].m_pic);
    }
    else
    {
        m_textBox.SetTextToPlay("", false);
        m_picBox.SetSprite(nullptr);
    }
}

void Menu::Draw(sf::RenderWindow& window)
{
    m_list.Draw(window);
    m_textBox.Draw(window);
    m_picBox.Draw(window);
    for(auto& btn: m_topMenuButtons)
        btn.Draw(window);
}

void Menu::SetUpData()//const std::vector<std::string>& collectables, const std::vector<std::string>& npcs,
    //const std::vector<std::string>& /*quests*/)
{
    m_data.clear();
    m_data.resize(3);

    for(auto name : EntitiesManager::Get().GetPlayer()->GetCollectedCollectablesInfo())
    {
        auto item = EntitiesManager::Get().COLLECTABLES().GetEntity(name);
        std::cout << " collectable name " << name << "\n";
        m_data[0].push_back(item->GetMenuData());
    }

    for(auto name : EntitiesManager::Get().GetPlayer()->GetTalkedNPCsInfo())
    {
        auto item = EntitiesManager::Get().NPCS().GetEntity(name);
        m_data[1].push_back(item->GetMenuData());
    }

    /*for(auto name : quests)
    {
        auto item = EntitiesManager::Get().QUESTS().GetEntity(name);
        m_data[2].push_back(item->GetMenuData());
    }*/

    //m_lastSelectedLine = -1;
    m_selectedBtn = 0;
    selectPage(0);
}

void Menu::onItemChange(int itemIndex)
{
    const auto dataForSelectedBTN = m_data[m_selectedBtn];
    std::cout << " itemIndex " << itemIndex << " dataForSelectedBTN.size() " << dataForSelectedBTN.size() << "\n";    
    if(dataForSelectedBTN.empty() || (m_lastSelectedLine == itemIndex)) return;
    m_lastSelectedLine = itemIndex;

    if((itemIndex < 0) || (itemIndex >= dataForSelectedBTN.size()))
    {
        //assert(false);
        std::cout << " itemIndex " << itemIndex << " dataForSelectedBTN.size() " << dataForSelectedBTN.size() << "\n";  
        return;
    }

    m_textBox.SetTextToPlay(dataForSelectedBTN[itemIndex].m_infoText, false);
    m_textBox.ShowWholeTextAtOnce();
    m_picBox.SetSprite(dataForSelectedBTN[itemIndex].m_pic);
}