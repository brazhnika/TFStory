#include "Dlg.h"
#include "ResourceManagers/SoundManager.h"

//#include "../Tokenizer.h"
//#include "ResourceManagers/FontManager.h"

namespace
{
    constexpr int BETWEEN_SPACE = 10;
    constexpr int BUTTON_OFFCET_Y = 55;
}
Dlg::Dlg(DialogCfg cfg, PlayerPtr player, NPCPtr npc, MouseState state, Rect viewRect)
    : m_cfg(cfg)
    , showDialog(false)
    , m_player(player)
    , m_npc(npc)
    , m_screen(cfg.screen.textBox)
    , m_trade(cfg.trades[0])
    , m_lastMouseState(state)
{
    m_initialTrade = m_cfg.trades[0];
    m_lastState = m_allStates[0];
    
    const auto textX = (viewRect.size.x - cfg.screen.textBox.textRect.size.x) >> 1;
    std::cout << "viewRect " << viewRect << "\n";
    std::cout << "textRect " << cfg.screen.textBox.textRect << "\n";
    std::cout << " viewRect.size.x / 2 " << viewRect.size.x/2 << "\n";
    std::cout << " cfg.screen.textBox.textRect.size.x / 2 " << cfg.screen.textBox.textRect.size.x/2 << "\n";

    const auto textY = viewRect.size.y - cfg.screen.textBox.textRect.size.y - BUTTON_OFFCET_Y;
    auto bottomStartPos = Point{textX, textY};
    m_screen.SetPos(bottomStartPos);
    
    bottomStartPos -= Point{10, 10} + Point{0, BUTTON_OFFCET_Y + BETWEEN_SPACE};
    //cfg.getTextRect().position - Point{10, 10} - Point{0, BUTTON_OFFCET_Y + BETWEEN_SPACE};

    const int minRepliesCount = m_trade.repliesIds.size() > 3 ? m_trade.repliesIds.size() : 3;
    addButtonsFromPos(minRepliesCount, bottomStartPos);

    m_player->ChangeMood(Mood::Neutral, true);
    m_npc->ChangeMood(Mood::Neutral, true);

    //setUpCurrentTrade();
}


void Dlg::addButtonsFromPos(int count, Point startPoint)
{
    for(int i = 0; i < count; ++i)
    {
        m_playerChoiceButtons.emplace_back(std::make_shared<Button>(m_cfg.screen.buttonCfg));
        m_playerChoiceButtons.back()->SetPos({startPoint.x, startPoint.y - (BUTTON_OFFCET_Y + BETWEEN_SPACE) * i });
        m_playerChoiceButtons.back()->Hide();
    }
}


void Dlg::setUpCurrentTrade(bool playSound)
{
    //std::cout << " setUpCurrentTrade " << m_trade.id <<"\n";

    if(m_trade.isPlayer)
    {
        m_player->ChangeMood(m_trade.mood);
        //m_npc->ChangeMood(Mood::Neutral, true);
    }
    else
    {
        //m_player->ChangeMood(Mood::Neutral, true);        
        m_npc->ChangeMood(m_trade.mood);
    }

    m_screen.SetTextToPlay(m_trade.longReply, playSound);

    hideAllButtons();

    if(m_trade.repliesIds.size() == 1)
        return;

    const int additionalButtons = m_trade.repliesIds.size() - m_playerChoiceButtons.size();
    addButtonsFromPos(additionalButtons, toInt(m_playerChoiceButtons.back()->GetPos()));

    for(size_t i = 0; i < m_trade.repliesIds.size(); ++i)
    {
        const auto& nextTrade = m_cfg.trades[m_trade.repliesIds[i]];
        m_playerChoiceButtons[i]->SetText(nextTrade.shortReply);
        //std::cout << " i " << i << " short "<< nextTrade.shortReply << "\n";
    }
}

void Dlg::hideAllButtons()
{
    for(auto& button : m_playerChoiceButtons)
        button->Hide();
}

void Dlg::UpdateOnEvents(EventsState events, float time)
{
    if(m_lastMouseState == events.mouse)
       return;

    m_lastMouseState = events.mouse;

    if(showDialog)
    {
        m_lastState = m_lastState->UpdateOnEvents(*this, events.keyboard, events.mouse, time);
    }
}

    
void Dlg::Draw(sf::RenderWindow& window)
{
    if(showDialog)
    {
        m_screen.Draw(window);        
        m_player->DrawDialogSprite(window);
        m_npc->DrawDialogSprite(window);
        
        if(m_trade.repliesIds.size() > 1)
            for(size_t i = 0; i < m_trade.repliesIds.size(); ++i)
                m_playerChoiceButtons[i]->Draw(window);
    }
}

void Dlg::Show()
{
    m_trade = m_initialTrade;
    setUpCurrentTrade(true);
    if(m_trade.isPlayer)
        m_npc->ChangeMood(Mood::Neutral, true);
    else
        m_player->ChangeMood(Mood::Neutral, true);        

    m_finish = false;
    assert(!m_trade.finish);
    m_screen.Start();
    showDialog = true;
}

void Dlg::Hide()
{
    showDialog = false;
    m_screen.Stop();
}


bool Dlg::Visible(){return showDialog;}

void Dlg::showSelectionButtons()
{

}

std::shared_ptr<IState> WordsArePlayingState::UpdateOnEvents(Dlg& dlg, KeybordState /*keys*/, MouseState mouse, float /*time*/)
{
    if(!mouse.leftButtonPressed) return dlg.m_allStates[static_cast<int>(States::WordsArePlaying)];

    //std::cout << " current state State::NPCsWordsArePlaying \n";
    auto wordsArePlayingState = dlg.m_allStates[static_cast<int>(States::WordsArePlaying)]; 

    if(!dlg.m_screen.IsFinishedPlaying())
    {
        dlg.m_screen.ShowWholeTextAtOnce();
        dlg.m_screen.Stop();
        return wordsArePlayingState;
    }

    if(dlg.m_trade.finish)
    {
        assert(dlg.m_trade.repliesIds.size() == 1);
        dlg.m_trade = dlg.m_cfg.trades[dlg.m_trade.repliesIds[0]];
        dlg.m_initialTrade = dlg.m_trade;
        dlg.m_finish = true;
        //std::cout  << "Set dlg.m_finish to true \n";
        return wordsArePlayingState;
    }

    if(dlg.m_trade.repliesIds.size() == 1)
    {
        dlg.m_trade = dlg.m_cfg.trades[dlg.m_trade.repliesIds[0]];
        dlg.setUpCurrentTrade(true);
        return wordsArePlayingState;
    }

    for(size_t i = 0; i < dlg.m_trade.repliesIds.size(); ++i)
        dlg.m_playerChoiceButtons[i]->Show();

    return dlg.m_allStates[static_cast<int>(States::ChoosesNext)];
}

std::shared_ptr<IState> ChoosesNextState::UpdateOnEvents(Dlg& dlg, KeybordState keys, MouseState mouse, float time)
{
    //std::cout << " current state State::PlayerChooses \n"; 
    assert(dlg.m_trade.repliesIds.size() > 0);
    assert(!dlg.m_trade.finish);
    
    int pressedButton = -1;
    for(size_t i = 0; i < dlg.m_trade.repliesIds.size(); ++i)
    {
        dlg.m_playerChoiceButtons[i]->UpdateOnEvents(keys, mouse, time);
        if(dlg.m_playerChoiceButtons[i]->IsClicked())
            pressedButton = i;
    } 

    if(pressedButton < 0)
        return dlg.m_allStates[static_cast<int>(States::ChoosesNext)];

    dlg.m_trade = dlg.m_cfg.trades[dlg.m_trade.repliesIds[pressedButton]];
    dlg.setUpCurrentTrade(true);
    return dlg.m_allStates[static_cast<int>(States::WordsArePlaying)];
}
