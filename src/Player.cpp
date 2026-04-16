#include "Player.h"
#include <iostream>
#include "ResourceManagers/SoundManager.h"



Player::Player(const SpriteCfg& cfg, Point viewSize)
   : NPC(cfg, viewSize, true /*dialogIsOnTheLeftSide*/)
 {}

Point Player::updatePositionOnKeyboardState(KeybordState keyboardState, float elapsedTime, Point mapSize)
{
   auto nextPos = m_data.topLeft;
   int delta = std::round(START_SPEED * elapsedTime);

   if(keyboardState.rightPressed)
   {
      //m_hitBoxPoint = {nextPos.x + m_data.spriteSize.x, nextPos.y + (m_data.spriteSize.y >> 2)};
      nextPos.x += delta;
      if((nextPos.x + m_data.spriteSize.x) >= mapSize.x) return m_data.topLeft;
   }
   else if(keyboardState.leftPressed)
   {
      //m_hitBoxPoint = {nextPos.x, nextPos.y + (m_data.spriteSize.y >> 2)};
      nextPos.x -= delta;
      if(nextPos.x  < 0) return m_data.topLeft;
   }

   if(keyboardState.upPressed)
   {
      //m_hitBoxPoint = {nextPos.x + (m_data.spriteSize.x >> 2), nextPos.y};
      nextPos.y -= delta;
      if(nextPos.y  < 0) return m_data.topLeft;
   }
   else if(keyboardState.downPressed)
   {
      //m_hitBoxPoint = {nextPos.x + (m_data.spriteSize.x >> 2), nextPos.y + m_data.spriteSize.y};
      nextPos.y += delta;
      if((nextPos.y + m_data.spriteSize.y) >= mapSize.y) return m_data.topLeft;
   }

   return nextPos;
}

bool Player::UpdateOnEvents(EventsState events,
        float elapsedTime,
        //Point /*viewSize*/,
        Point mapSize,
        SortedSpritesList<Interactable>& npcs,
        const std::vector<Rect>& walls)
{
   auto prevBumping = m_isBumpingIntoSmth;
   m_isBumpingIntoSmth = false;
   m_lastKeyboardState = events.keyboard;

   if(!events.keyboard.MustProcessMove()) return false; 

   auto nextPos = updatePositionOnKeyboardState(events.keyboard, elapsedTime, mapSize);
   if(nextPos == m_data.topLeft) return false;

   Rect newBounds{nextPos,  m_data.spriteSize};
   for(auto& rect : walls)
   {
      if(newBounds.findIntersection(rect))
      {
         if(!prevBumping)
            SoundManager::Get().PlaySound("./assets/sounds/cork.wav");

         m_isBumpingIntoSmth = true;
         return false;
      }
   }

   m_isBumpingIntoSmth = false;
   newBounds =  m_data.calculateBumpingRect(nextPos);
   for(auto npc : npcs)
   {
      if(*npc == *this) continue;
      
      bool beenTouched = npc->GetTouched();
      if(npc->CheckTouchingAndBumping(newBounds))
         m_isBumpingIntoSmth = true;

      if(!beenTouched && npc->GetTouched())
         SoundManager::Get().PlaySound("./assets/sounds/click.wav");
   }

   if(m_isBumpingIntoSmth) return false;

   auto oldY = GetGlobalY();
   m_data.topLeft = nextPos;
   m_changeHandler(*this, oldY);

   return false;
}

void Player::updateCurrentSpriteIndex()
{
   if(m_lastKeyboardState.rightPressed)
      m_data.updateSmallAndKeepInRange(small.rightStart, small.rightEnd);
   else if(m_lastKeyboardState.leftPressed)
      m_data.updateSmallAndKeepInRange(small.leftStart, small.leftEnd);
   else if(m_lastKeyboardState.upPressed)
      m_data.updateSmallAndKeepInRange(small.upStart, small.upEnd);
   else if(m_lastKeyboardState.downPressed)
      m_data.updateSmallAndKeepInRange(small.downStart, small.downEnd);
   else
      m_data.updateSmallAndKeepInRange(small.standStart, small.standEnd);
}


void Player::Draw(sf::RenderWindow& window, Rect viewRect)
{
   if (m_Clock.getElapsedTime().asMilliseconds() > base.delay)
	{
	   updateCurrentSpriteIndex();
	 	m_Clock.restart();
	}
   const auto localLeftTop = toFloat(m_data.topLeft - viewRect.position);
   m_smallSprites[m_data.m_index].setPosition(localLeftTop);
   centerText();
   window.draw(m_smallSprites[m_data.m_index]);
   window.draw(m_nameText);
}

void Player::Pickup(std::string npc)
{
   SoundManager::Get().PlaySound("./assets/sounds/reload.wav");
   m_collectables.push_back(std::move(npc));
}
