#pragma once

#include <SFML/Graphics.hpp>
#include <memory>
#include "Types.h"
#include "SpriteCfg.h"
#include "SortedSpritesList.h"

class Interactable
{
public:
    Interactable(const BaseAnimationInfo& base, int framesStart, int framesEnd, bool canBeStompedOver);
    virtual ~Interactable(){};
   
    virtual void Draw(sf::RenderWindow& window, Rect viewRect);
    virtual void DrawDialogSprite(sf::RenderWindow& /*window*/) {};
    void SetGlobalPos(Point pos, Rect viewRect);

    //bool UpdateOnEvents(MouseState mouseState);

    virtual bool UpdateOnEvents(EventsState events,
        float elapsedTime,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls);

    virtual void UpdatePosOnCameraMove(Rect viewRect);

    Rect GetGlobalBoundaries(){ return {m_data.topLeft, m_data.spriteSize};}
    Rect GetGlobalInteractionBoundaries(){return m_data.interactionRect;}
    Rect GetGlobalBumpingBoundaries(){return m_data.bumpingRect;}

    bool CheckTouchingAndBumping(Rect playerBounds, bool setAsTouched = true);
    void SetTouched(bool touched) { m_touched = touched;}
    bool GetTouched(){return m_touched;}

    virtual MenuItemData GetMenuData();
    virtual std::string GetName() const = 0;
    virtual std::string GetInfo() = 0;

    virtual bool CanTalkTo() const {return false;}
    virtual bool CanBePickedUp() const {return false;}
    virtual bool SetPickedUp() = 0;

    //======= y-Sorted List support ====================================================================
    SpriteYPos<Interactable>* GetYPosListElem()
    {
        n_sceneYpos.m_sprite = this;
        return &n_sceneYpos;
    }
    int GetGlobalY() const { return m_data.topLeft.y + m_data.spriteSize.y;}
    void SetChangeHandler(std::function<void(Interactable&, int)> handler) { m_changeHandler = handler;} 
    //======= y-Sorted List support ====================================================================

protected:
   struct ViewData
   {
        int delay;
        int standStart;
        int standEnd;

        Point topLeft{0, 0};
        sf::Vector2f viewRectTopLeftPos;
        sf::Vector2f localTopLeft{0, 0};
        Point spriteSize{0, 0};

        Rect interactionRect;
        Rect bumpingRect;
        Rect calculateBumpingRect(Point newPos)
        {
            return {newPos + Point{5, (int)(spriteSize.y) - 15}, {(int)(spriteSize.x) - 10, 15}};
        }
      
        int m_index{0};
        void updateSmallAndKeepInRange(int min, int max)
        {
            ++m_index;
            if((m_index < min) || (m_index > max))
             	m_index = min;
        }
    };

    void updateCurrentSpriteIndex();
    void centerText();
    void setPositionsForSprites();

    BaseAnimationInfo m_base;
    std::vector<sf::Sprite> m_smallSprites;
    std::shared_ptr<sf::Sprite> m_selectionSprite;
    sf::Text m_nameText;

    ViewData m_data;

    sf::Clock m_Clock;
    int m_framePeriodMs;
    bool m_touched{false};
    const bool m_canBeStompedOver;
    MouseState m_processedMouseState;
    bool m_visible{true};
    bool m_pickedUp{false};
    SpriteYPos<Interactable> n_sceneYpos;
    std::function<void(Interactable&, int)> m_changeHandler;
};

using InteractablePtr = std::shared_ptr<Interactable>;

inline bool operator==(const Interactable& left, const Interactable& right)
{
    return left.GetName() == right.GetName();
}

inline bool operator!=(const Interactable& left, const Interactable& right)
{
    return !(left == right);
}
