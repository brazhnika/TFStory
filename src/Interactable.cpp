#include "Interactable.h"
#include "ResourceManagers/TextureManager.h"
#include "ResourceManagers/RandomGenerator.h"
#include "ResourceManagers/FontManager.h"
#include "utils.h"

#include <iostream>

namespace
{
   const float START_SPEED = 200;
   constexpr sf::Vector2f SELECTION_OFFSET{5.f, 5.f};
};

Interactable::Interactable(const BaseAnimationInfo& base, int standStart, int standEnd, bool canBeStompedOver)
   : m_base(base)
   , m_nameText(FillFromFont(base.fonts.normal))
   , m_canBeStompedOver(canBeStompedOver)   
   //, n_scenePosition(std::make_shared<SpriteYPos<Interactable>>())
{
    m_data.delay = base.delay;
    m_data.standStart = standStart;
    m_data.standEnd = standEnd;

    auto smallSpritesTexture = TextureManager::Get().GetTexture(base.picFile);
    auto selectionTexture = TextureManager::Get().GetTexture(base.selection);
   
    m_selectionSprite = std::make_shared<sf::Sprite>(*selectionTexture);
    auto selectionSpriteSize = selectionTexture->getSize();

    m_selectionSprite->setTextureRect({{0,0}, {(int)selectionSpriteSize.x, (int)selectionSpriteSize.y}});

    assert(smallSpritesTexture);
    const auto [width, height] = smallSpritesTexture->getSize();
    m_data.spriteSize = {(int)(width / base.frames), (int)height};
    m_data.interactionRect = {{0,0}, {(int)(m_data.spriteSize.x), (int)(height)}};
    m_data.bumpingRect = m_data.calculateBumpingRect({0, 0});

    for(int i = 0; i < base.frames; ++i)
    {
        m_smallSprites.emplace_back(*smallSpritesTexture);
        m_smallSprites.back().setTextureRect({ {i * m_data.spriteSize.x, 0}, m_data.spriteSize});
    }

    m_data.m_index = (standEnd - standStart) *  RandomGenerator::Get().GetProbability();
    
    auto fps = 24;
	m_framePeriodMs = 1000 / fps;

	m_Clock.restart();
}

void Interactable::SetGlobalPos(Point pos, Rect viewRect)
{
    m_data.topLeft = pos;
    m_data.localTopLeft = toFloat(pos - viewRect.position);
    m_data.interactionRect.position = pos;
    m_data.bumpingRect = m_data.calculateBumpingRect(pos);

    const auto name = GetName();
    m_nameText.setString(name);
    centerText();
}

void Interactable::centerText()
{
    const auto spriteBounds = m_smallSprites[m_data.m_index].getGlobalBounds();
    m_nameText.setPosition({spriteBounds.getCenter().x - ((int)m_nameText.getGlobalBounds().size.x >> 1),
        spriteBounds.position.y - m_base.fonts.normal.size - 3});
}

void Interactable::Draw(sf::RenderWindow& window, Rect viewRect)
{
    if(!m_visible || !viewRect.findIntersection(GetGlobalBoundaries()))
        return;
   
    if (m_Clock.getElapsedTime().asMilliseconds() > m_data.delay)
	{
		m_data.updateSmallAndKeepInRange(m_data.standStart, m_data.standEnd);
        //setPositionsForSprites();
		m_Clock.restart();
	}

    const auto localLeftTop = toFloat(m_data.topLeft - viewRect.position);

    if(m_touched)
    {
        m_selectionSprite->setPosition(localLeftTop - SELECTION_OFFSET - toFloat(m_base.selectionOffset));
        window.draw(*m_selectionSprite);
    }
    m_smallSprites[m_data.m_index].setPosition(localLeftTop);
    window.draw(m_smallSprites[m_data.m_index]);
    window.draw(m_nameText);
}


bool Interactable::CheckTouchingAndBumping(Rect playerBounds, bool setAsTouched)
{
    //bool iAmTouchedByPlayer = playerBounds.findIntersection(GetGlobalInteractionBoundaries()).has_value();
    bool iAmTouchedByPlayer = playerBounds.findIntersection(GetGlobalInteractionBoundaries()).has_value();
    if(setAsTouched)
        SetTouched(iAmTouchedByPlayer);

    return !m_canBeStompedOver && playerBounds.findIntersection(GetGlobalBumpingBoundaries()); 
}

    
MenuItemData Interactable::GetMenuData()
{
    MenuItemData data;
    data.m_pic = std::make_shared<sf::Sprite>(m_smallSprites[m_data.m_index]);
    data.m_name = GetName();
    data.m_infoText = GetInfo();
    return data;
}

bool Interactable::UpdateOnEvents(EventsState events,
        float /*elapsedTime*/,
        Point /*mapSize*/,
        SortedSpritesList<Interactable>& /*npcs*/,
        const std::vector<Rect>& /*walls*/)
{
    //std::cout << GetName() << " clicked m_pickedUp " << m_pickedUp <<"\n";
    if(m_pickedUp) return false;
    if((m_processedMouseState == events.mouse) || !events.mouse.leftButtonPressed)
        return false;
  
    bool iAmClicked = m_smallSprites[m_data.m_index].getGlobalBounds().contains(toFloat(events.mouse.position));
   
    m_processedMouseState = events.mouse;
    return iAmClicked;
}

void Interactable::UpdatePosOnCameraMove(Rect viewRect)
{
    if(!m_visible || !viewRect.findIntersection(GetGlobalBoundaries()))
        return;
       
    m_data.localTopLeft = toFloat(m_data.topLeft - viewRect.position);
    m_data.viewRectTopLeftPos = toFloat(viewRect.position);
    setPositionsForSprites();
}

void Interactable::setPositionsForSprites()
{
    //if(m_touched)
    m_selectionSprite->setPosition(m_data.localTopLeft - SELECTION_OFFSET - toFloat(m_base.selectionOffset));

    m_smallSprites[m_data.m_index].setPosition(m_data.localTopLeft);
    centerText();    
}

//std::shared_ptr<SpriteYPos<Interactable>> Interactable::GetYPosListElem()
//{
 //   assert(n_scenePosition);
//    return n_scenePosition;
//