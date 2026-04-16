#include "ClickableList.h"
#include "ResourceManagers/FontManager.h"
#include "utils.h"
#include <iostream>

namespace
{
    constexpr int BETWEEN_ITEMS_SPACE = 3;
    constexpr int BORDER_OFFSET = 5;
}

    
ClickableList::ClickableList(ListCfg cfg, std::function<void(int)> onSelectedItemChange)
    : m_cfg(cfg)
    , m_fontSize(cfg.font.normal.size)
    , m_listBackground(*TextureManager::Get().GetTexture(cfg.textureFile))
    , m_scrollBar(cfg.scrollBarCfg, [this](int pos){ onScaleChange(pos);})
    , m_selectedItem(-1)  
    , m_onSelectedItemChange(onSelectedItemChange)
    , m_tokenizer(FontManager::Get().GetFont(cfg.font.normal.fontFile), cfg.font.normal.size, cfg.list.size.x - BORDER_OFFSET * 2)
{
    m_listBackground.setTextureRect(cfg.list);

    const auto listItemsCount = cfg.list.size.y / cfg.font.normal.size;

    for(int i = 0; i < listItemsCount; ++i)
        m_visibleListItems.emplace_back(FillFromFont(cfg.font.normal));
}

void ClickableList::SetStrings(std::vector<std::string> strings)
{
    m_strings = strings;
    for(auto& str : m_strings)
    {
        auto oldStr = str;
        str = m_tokenizer.CutWordToASize(str);
        //std::cout << " str " << oldStr << " is cut to str " << str<< "\n";
    }
    const int invisibleItemsCount = strings.size() - m_visibleListItems.size();
    m_firstVisibleStringIndex = 0;
    if(invisibleItemsCount > 0)
    {
        m_scrollBar.SetScaleSize(invisibleItemsCount + 1);
        m_scrollBar.Show();
    }
    else
        m_scrollBar.Hide();

    m_visibleStringsCount = strings.size() > m_visibleListItems.size() ? m_visibleListItems.size() : strings.size();
    //std::cout << "ClickableList::m_visibleStringsCount " << m_visibleStringsCount << "\n";
    for(int i = 0; i < m_visibleStringsCount; ++i)
        m_visibleListItems[i].setString(m_strings[i]);
    
    m_selectedItem = 0;
    m_hoveredItem = -1;
    m_firstVisibleStringIndex = 0;
    updateVisibleTextFonts();
}

void ClickableList::SetPos(Point pos)
{
    auto floatPos = toFloat(pos);
    m_listBackground.setPosition(floatPos);    
    for(size_t i = 0; i < m_visibleListItems.size(); ++i)
    {
        m_visibleListItems[i].setPosition(floatPos + sf::Vector2f{BORDER_OFFSET, (float)i * m_fontSize + BETWEEN_ITEMS_SPACE});
    }

    auto scrollBarPos = pos;
    scrollBarPos.x += m_listBackground.getGlobalBounds().size.x;
    m_scrollBar.SetPos(scrollBarPos);
    auto listSize = m_listBackground.getGlobalBounds().size;
}

void ClickableList::UpdateOnEvents(EventsState events)
{
    if(m_visibleStringsCount <= 0) return;
    
    auto prevHoveredItem = m_hoveredItem;
    auto prevSelectedItem = m_selectedItem;

    bool updateFonts = true;
    m_hoveredItem = -1;
    //m_scrollBarHovered = false;
    auto mousePos = toFloat(events.mouse.position);
    auto backgroundPos = m_listBackground.getGlobalBounds();
    if(backgroundPos.contains(mousePos))
    {
        m_hoveredItem = (events.mouse.position.y - backgroundPos.position.y) / (m_fontSize);
        if(events.mouse.leftButtonPressed && (m_previousMouseState != events.mouse))
            m_selectedItem = (m_hoveredItem + m_firstVisibleStringIndex);
        if(m_previousScrollState != events.scroll)
        {
            scroll(events.scroll.wheelDirection);
            updateFonts = false;
        }
    }
    else
    {
        m_scrollBar.UpdateOnEvents(events);
    }
    m_previousMouseState = events.mouse;
    m_previousScrollState = events.scroll;
    if(updateFonts)
    {
        updateVisibleTextFonts();
        if(prevHoveredItem != m_hoveredItem)
        {
            FillFromFont(m_visibleListItems[m_hoveredItem], m_cfg.font.hovered);
            if(isValidListIndex(prevHoveredItem) && (prevHoveredItem != m_selectedItem))
                FillFromFont(m_visibleListItems[prevHoveredItem], m_cfg.font.normal);
        }
        if(prevSelectedItem != m_selectedItem)
        {
            auto selectedItem = m_selectedItem - m_firstVisibleStringIndex;
            if(isValidListIndex(selectedItem))
            {
                FillFromFont(m_visibleListItems[selectedItem], m_cfg.font.selected);
                m_onSelectedItemChange(m_selectedItem);
            }
            
            selectedItem = prevSelectedItem - m_firstVisibleStringIndex;
            if(isValidListIndex(selectedItem))
            {
                if(selectedItem != m_hoveredItem)
                   FillFromFont(m_visibleListItems[selectedItem], m_cfg.font.normal);
            }
        }
    }
}

bool ClickableList::isValidListIndex(int idx)
{
    return (idx >= 0) && /*(idx < (int)m_visibleListItems.size()) &&*/ (idx < m_visibleStringsCount);
}

void ClickableList::scroll(WheelScrolDirection direction)
{
    if(m_strings.size() <= m_visibleListItems.size())
        return;
    
    const auto scrollDy = direction == WheelScrolDirection::WheelUp ? -1 : 1;

    auto newFirstVisibleIdx = m_firstVisibleStringIndex + scrollDy;
    scrollToNewFirstVisibleIndex(newFirstVisibleIdx, true /*scrollScrollBarToo*/);
}

void ClickableList::scrollToNewFirstVisibleIndex(int newFirstVisibleIndex, bool scrollScrollBarToo)
{
    int lastPossibleVisibleIdx = m_strings.size() - m_visibleListItems.size();

    if(newFirstVisibleIndex < 0) newFirstVisibleIndex = 0;
    else if (newFirstVisibleIndex > lastPossibleVisibleIdx) newFirstVisibleIndex = lastPossibleVisibleIdx;

    //std::cout << " newFirstVisibleIdx " << newFirstVisibleIndex << " m_firstVisibleStringIndex " << m_firstVisibleStringIndex <<"\n";
    if (m_firstVisibleStringIndex == newFirstVisibleIndex) return;

    if(scrollScrollBarToo)
       m_scrollBar.Scroll(newFirstVisibleIndex - m_firstVisibleStringIndex);

    m_firstVisibleStringIndex = newFirstVisibleIndex;

    for(int i = 0; i < m_visibleStringsCount; ++i)
        m_visibleListItems[i].setString(m_strings[m_firstVisibleStringIndex + i]);

    updateVisibleTextFonts();
}


void ClickableList::updateVisibleTextFonts()
{
    const auto currentSelectedItem = m_selectedItem - m_firstVisibleStringIndex;
    for(int i = 0; i < m_visibleStringsCount; ++i)
    {
        if(currentSelectedItem == i)
            FillFromFont(m_visibleListItems[i], m_cfg.font.selected);
        else
        {
            if(m_hoveredItem == i)
                FillFromFont(m_visibleListItems[i], m_cfg.font.hovered);
            else
                FillFromFont(m_visibleListItems[i], m_cfg.font.normal);
        }
    }
}

void ClickableList::Draw(sf::RenderWindow& window)
{
    if(!m_visible) return;

    window.draw(m_listBackground);

    for(int i = 0; i < m_visibleStringsCount; ++i)
        window.draw(m_visibleListItems[i]);
    m_scrollBar.Draw(window);       
}

void ClickableList::Show() {m_visible = true;}
void ClickableList::Hide() {m_visible = false;}
bool ClickableList::Visible() {return m_visible;}


Point ClickableList::GetPos()
{
    return m_listRect.position;
}

void ClickableList::onScaleChange(int newCurrentPos)
{
    scrollToNewFirstVisibleIndex(newCurrentPos, false /*scrollScrollBarToo*/);
}
 
sf::Vector2f ClickableList::GetSize()
{
    auto totalSize = m_listBackground.getGlobalBounds().size;
    totalSize.x += m_scrollBar.GetSize().x;
    return totalSize;
}