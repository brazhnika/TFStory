#include "NonInteractableNpc.h"
#include "ResourceManagers/RandomGenerator.h"
#include "Player.h"
#include <iostream>
#include "ResourceManagers/SoundManager.h"



NonInteractableNPC::NonInteractableNPC(const SpriteCfg& cfg, Point /*viewSize*/)
    : SpriteCfg(cfg)
    , Interactable(cfg.base, cfg.small.standStart, cfg.small.standEnd, false)
{
    updateMovement();
}

void NonInteractableNPC::updateMovement()
{
    m_passedTime = 0;
    m_secToKeepCurrentMovement = std::floor(5 * (RandomGenerator::Get().GetProbability() + 0.2));
    m_speed = std::floor(150 * (RandomGenerator::Get().GetProbability() + 1));
    m_lastDir = (m_lastDir + 1) % 5;
    m_dx = 0;
    m_dy = 0;
    int dir = 1;
    if(m_lastDir == 1)
        m_dx = -dir;
    else if(m_lastDir == 2)
        m_dy = -dir;
    else if(m_lastDir == 3)
        m_dx = dir;
    else if(m_lastDir == 4)
        m_dy = dir;
}


Point NonInteractableNPC::updatePosition(float elapsedTime, Point mapSize)
{
    m_passedTime += elapsedTime;
    //std::cout << " m_passedTime " << m_passedTime << "\n";
    if(m_passedTime > m_secToKeepCurrentMovement)
    {
        updateMovement();
    }

    int delta = std::round(m_speed * elapsedTime);
    auto nextPos = m_data.topLeft + Point{delta * m_dx, delta * m_dy };


   if(nextPos.x < 0)
   {
       nextPos.x = 0;
       m_dx = -m_dx;
   }
   else if(nextPos.x > mapSize.x)
   {
       nextPos.x = mapSize.x;
       m_dx = -m_dx;
   }

   if(nextPos.y < 0)
   {
       nextPos.y = 0;
       m_dy = -m_dy;
   }
   else if(nextPos.y > mapSize.y)
   {
       nextPos.y = mapSize.y;
       m_dy = -m_dy;
   }

   return nextPos;
}

bool NonInteractableNPC::UpdateOnEvents(EventsState /*events*/,
        float elapsedTime,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls)
{
   auto prevPos = m_data.topLeft;

   auto reverse = [this](){
      m_dx = -m_dx;
      m_dy = -m_dy;
   };
   auto nextPos = updatePosition(elapsedTime, mapSize);
   if(nextPos == m_data.topLeft) return false;

   Rect newBounds{nextPos,  m_data.spriteSize};
   for(auto& rect : walls)
   {
      if(newBounds.findIntersection(rect))
      {
         reverse();
         return false;
      }
   }
   for(auto npc : npcs)
   {
      if(npc->CanTalkTo() && npc->CheckTouchingAndBumping(newBounds, false /*setAsTouched*/))
      {
         reverse();
         return false;
      }
   }

   auto oldY = GetGlobalY();
   m_data.topLeft = nextPos;
   m_changeHandler(*this, oldY);
   return false;
}

void NonInteractableNPC::updateCurrentSpriteIndex()
{
   if(m_dx > 0)
      m_data.updateSmallAndKeepInRange(small.rightStart, small.rightEnd);
   else if(m_dx < 0)
      m_data.updateSmallAndKeepInRange(small.leftStart, small.leftEnd);
   else if(m_dy < 0)
      m_data.updateSmallAndKeepInRange(small.upStart, small.upEnd);
   else if(m_dy > 0)
      m_data.updateSmallAndKeepInRange(small.downStart, small.downEnd);
   else
   {
      //std::cout << " m_dx " << m_dx << " ";
      m_data.updateSmallAndKeepInRange(small.standStart, small.standEnd);
   }
}


void NonInteractableNPC::Draw(sf::RenderWindow& window, Rect viewRect)
{
   if(!m_visible || !viewRect.findIntersection(GetGlobalBoundaries()))
      return;

   if (m_Clock.getElapsedTime().asMilliseconds() > base.delay)
	{
	   updateCurrentSpriteIndex();
      //setPositionsForSprites();
	 	m_Clock.restart();
	}
   const auto localLeftTop = toFloat(m_data.topLeft - viewRect.position);
   m_smallSprites[m_data.m_index].setPosition(localLeftTop);
   centerText();

   window.draw(m_smallSprites[m_data.m_index]);
   window.draw(m_nameText);
}

