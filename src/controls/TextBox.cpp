#include "TextBox.h"
#include "ResourceManagers/SoundManager.h"
#include "../controls/Tokenizer.h"
#include "ResourceManagers/FontManager.h"
#include "utils.h"



static const int X_OFFSET = 10;
static const int Y_OFFSET = 7;
static const int LINE_SPACING = 1;

TextBox::TextBox(TextBoxCfg cfg)
    : m_cfg(cfg)
    , m_scrollBar(cfg.scrollBarCfg, [this](int pos){ onScaleChange(pos);})    
    , m_textBackground(std::make_shared<sf::Sprite>(*TextureManager::Get().GetTexture(cfg.pic)))
    , m_tokenizer(FontManager::Get().GetFont(cfg.font.normal.fontFile), cfg.font.normal.size, cfg.textRect.size.x - X_OFFSET * 2)
{
    m_textBackground->setTextureRect(cfg.textRect);
    auto maxLinesCount = cfg.textRect.size.y / (cfg.font.normal.size + cfg.font.normal.size / 7);
    for(size_t i = 0; i < maxLinesCount; ++i)
    {
        m_lines.emplace_back(FillFromFont(cfg.font.normal));
        m_lines.back().setPosition(toFloat(Point{cfg.textRect.position.x + X_OFFSET,
            cfg.textRect.position.y + Y_OFFSET + (int)i * (cfg.font.normal.size + LINE_SPACING )}));
        m_lines.back().setString("");
    }
}

void TextBox::SetPos(Point pos)
{
   m_textBackground->setPosition(toFloat(pos));
   for(size_t i = 0; i < m_lines.size(); ++i)
      m_lines[i].setPosition(toFloat(pos + Point{X_OFFSET,  Y_OFFSET + (int)i * (m_cfg.font.normal.size + LINE_SPACING )}));

   auto scrollBarPos = pos;
   scrollBarPos.x += m_textBackground->getGlobalBounds().size.x;
   m_scrollBar.SetPos(scrollBarPos);
   std::cout << " Scroll bar visible ? " << m_scrollBar.Visible() << "\n";
}

void TextBox::UpdateOnEvents(EventsState events, float /*elapsedTime*/)
{
   auto mousePos = toFloat(events.mouse.position);
   auto backgroundPos = m_textBackground->getGlobalBounds();
   if(!backgroundPos.contains(mousePos)) return;

   if((m_previousMouseState != events.mouse) && events.mouse.leftButtonPressed)
      ShowWholeTextAtOnce();

   if(m_previousScrollState != events.scroll)
   {
      scroll(events.scroll.wheelDirection);
   }
   m_previousMouseState = events.mouse;
   m_previousScrollState = events.scroll;
}
    
void TextBox::Draw(sf::RenderWindow& window)
{
   window.draw(*m_textBackground);
   
   if(!m_finishedPlaying
      && (m_currentPlayedLine >= m_firstVisibleStringIndex)
      && (m_currentPlayedLine < m_firstVisibleStringIndex + m_lines.size())
      && (m_currentPlayedLine < m_linesToPlay.size())
      && (m_linesToPlay[m_currentPlayedLine] != m_playedLines[m_currentPlayedLine]))
   {
      static const int CHAR_DELAY = 100;
      if (m_Clock.getElapsedTime().asMilliseconds() > CHAR_DELAY)
	   {
   	   m_Clock.restart();
         auto oldPlayedTextSize = m_playedLines[m_currentPlayedLine].size();
         m_playedLines[m_currentPlayedLine].insert(m_playedLines[m_currentPlayedLine].end(), m_linesToPlay[m_currentPlayedLine][oldPlayedTextSize]);
         m_lines[m_currentPlayedLine].setString(m_playedLines[m_currentPlayedLine]);
         if(m_linesToPlay[m_currentPlayedLine] == m_playedLines[m_currentPlayedLine])
         {
            ++m_currentPlayedLine;
            if(m_currentPlayedLine >= m_linesToPlay.size())
            {
               Stop();
               m_finishedPlaying = true;
            }
            else
            {
               const int invisibleItemsCount = m_currentPlayedLine - m_lines.size();
               if(invisibleItemsCount > 0)
               {
                  m_scrollBar.SetScaleSize(invisibleItemsCount + 1);
                  m_scrollBar.Show();
               }
            }
         }
      }
	}
   for(const auto& text : m_lines)
      window.draw(text);
   
   m_scrollBar.Draw(window);
}

void TextBox::Stop()
{
    //m_playing = false;
    SoundManager::Get().StopSound("./assets/sounds/morze3.wav");
}

void TextBox::Start()
{
    //m_playing = true;
    SoundManager::Get().PlaySound("./assets/sounds/morze3.wav", true);
}

void TextBox::SetTextToPlay(std::string text, bool playSound, bool showWholeTextAtOnce)
{
   m_playOneByOneLetter = !showWholeTextAtOnce;
 
   m_linesToPlay = m_tokenizer.SplitIntoLines(text);

   m_visibleStringsCount = m_lines.size() > m_linesToPlay.size() ? m_linesToPlay.size() : m_lines.size();
   m_firstVisibleStringIndex = 0;

   std::cout << " TextBox  m_visibleStringsCount " << m_visibleStringsCount <<"\n";
   std::cout << " TextBox  m_linesToPlay " << m_linesToPlay.size() <<"\n";
   std::cout << text << "\n";

   if(m_playOneByOneLetter)
   {
      m_playedLines.clear();
      m_playedLines.resize(m_linesToPlay.size(), "");
      m_currentPlayedLine = 0;
      m_finishedPlaying = false;

      for(auto& line : m_lines)
         line.setString("");

      if(!showWholeTextAtOnce && playSound) Start();
   }
   else
   {
      ShowWholeTextAtOnce();
      // m_playedLines = m_linesToPlay;
      // m_finishedPlaying = true;
      // for(int i = 0; i < m_visibleStringsCount; ++i)
      //    m_lines[i].setString(m_playedLines[i]);
   }
}

void TextBox::ShowWholeTextAtOnce()
{
   m_playOneByOneLetter = false;
   m_finishedPlaying = true;
   m_playedLines = m_linesToPlay;
   m_currentPlayedLine = m_linesToPlay.size();

   for(int i = 0; i < m_visibleStringsCount; ++i)
   {
      m_lines[i].setString(m_linesToPlay[i]);
   }

   const int invisibleItemsCount = m_linesToPlay.size() - m_lines.size();
   std::cout << "ShowWholeTextAtOnce  invisibleItemsCount " << invisibleItemsCount << "\n";
   if(invisibleItemsCount > 0)
   {
      m_scrollBar.SetScaleSize(invisibleItemsCount + 1);
      m_scrollBar.Show();
   }
   else
      m_scrollBar.Hide();

   Stop();
}

void TextBox::onScaleChange(int newCurrentPos)
{
   scrollToNewFirstVisibleIndex(newCurrentPos, false /*scrollScrollBarToo*/);
}

bool TextBox::isValidListIndex(int idx)
{
    return (idx >= 0) && /*(idx < (int)m_visibleListItems.size()) &&*/ (idx < m_visibleStringsCount);
}

void TextBox::scroll(WheelScrolDirection direction)
{
   std::cout << " Scroll " << (direction == WheelScrolDirection::WheelDown ? "down" : "up" ) << "\n";
    if((m_currentPlayedLine < m_lines.size()) || (m_finishedPlaying && (m_playedLines.size() < m_lines.size())))
    {
       std::cout << " Scroll m_currentPlayedLine " << m_currentPlayedLine << " m_lines.size() " << m_lines.size() << "\n";
       std::cout << " Scroll m_finishedPlaying " << m_finishedPlaying << " m_playedLines.size() " << m_playedLines.size() << "\n";
        return;
    }
    
    const auto scrollDy = direction == WheelScrolDirection::WheelUp ? -1 : 1;

    auto newFirstVisibleIdx = m_firstVisibleStringIndex + scrollDy;
    scrollToNewFirstVisibleIndex(newFirstVisibleIdx, true /*scrollScrollBarToo*/);
}

void TextBox::scrollToNewFirstVisibleIndex(int newFirstVisibleIndex, bool scrollScrollBarToo)
{
   const bool scrollUp = m_firstVisibleStringIndex > newFirstVisibleIndex;
   int lastPossibleVisibleIdx;
   if(m_finishedPlaying)
      lastPossibleVisibleIdx = m_playedLines.size() - m_lines.size();
   else
   {
      lastPossibleVisibleIdx = m_currentPlayedLine - m_lines.size();
      if(m_linesToPlay.size() < m_playedLines.size())
         lastPossibleVisibleIdx += 1;
   }

   if(newFirstVisibleIndex < 0) newFirstVisibleIndex = 0;
   else if (newFirstVisibleIndex > lastPossibleVisibleIdx) newFirstVisibleIndex = lastPossibleVisibleIdx;

   if (m_firstVisibleStringIndex == newFirstVisibleIndex) return;
   if(scrollScrollBarToo)
   {
      std::cout << "newFirstVisibleIndex " << newFirstVisibleIndex << "\n";
      std::cout << "m_firstVisibleStringIndex " << m_firstVisibleStringIndex << "\n";
      std::cout << " set scroll bar to " << newFirstVisibleIndex - m_firstVisibleStringIndex << "\n";
      
      m_scrollBar.Scroll(newFirstVisibleIndex - m_firstVisibleStringIndex);
   }

   m_firstVisibleStringIndex = newFirstVisibleIndex;

   if(lastPossibleVisibleIdx >= m_playedLines.size())
   {
      assert(!scrollUp);
      for(int i = m_currentPlayedLine; i < lastPossibleVisibleIdx; ++i)
         m_playedLines[i] = m_linesToPlay[i];
      m_currentPlayedLine = lastPossibleVisibleIdx;
      m_playedLines[m_currentPlayedLine].insert(m_playedLines[m_currentPlayedLine].end(), m_linesToPlay[m_currentPlayedLine][0]);
      
      const int invisibleItemsCount = m_currentPlayedLine - m_lines.size();
      if(invisibleItemsCount > 0)
      {
         m_scrollBar.SetScaleSize(invisibleItemsCount + 1);
         m_scrollBar.Show();
      }
   }
   if(!m_finishedPlaying)
   {
      if(m_currentPlayedLine > m_firstVisibleStringIndex + m_lines.size() - 1)
         Stop();
      else
         Start();
   }

   for(int i = 0; i < m_visibleStringsCount; ++i)
         m_lines[i].setString(m_playedLines[m_firstVisibleStringIndex + i]);
}