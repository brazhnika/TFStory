#pragma once


#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include "SceneCfg.h"
#include "map.h"
#include "Player.h"
#include "npc.h"
#include "Collectable.h"
#include "../controls/TextBox.h"
#include "../controls/Button.h"

class Dlg;

class IState
{
public:
    enum class States
    {
        WordsArePlaying,
        ChoosesNext,
        Count
    };

    virtual std::shared_ptr<IState> UpdateOnEvents(Dlg& dlg, KeybordState keyboardState, MouseState mouseState, float elapsedTime) = 0;
    //bool Finished();
protected:
};

class WordsArePlayingState : public IState
{
public:
    std::shared_ptr<IState> UpdateOnEvents(Dlg& dlg, KeybordState keyboardState, MouseState mouseState, float elapsedTime) override;
};

class ChoosesNextState : public IState
{
public:
    std::shared_ptr<IState> UpdateOnEvents(Dlg& dlg, KeybordState keyboardState, MouseState mouseState, float elapsedTime) override;
};


class Dlg
{
public:
    //Dlg();
    Dlg(DialogCfg cfg, PlayerPtr player, NPCPtr npc, MouseState state, Rect viewRect);
    void UpdateOnEvents(EventsState events, float /*elapsedTime*/);
    void Draw(sf::RenderWindow& window);
    void Show();
    void Hide();
    bool Visible();

    //void SetUpDlg(PlayerPtr player, NPCPtr npc, Trade lastTrade);
    Trade GetLastTrade() { return m_trade;}
    bool IsFinished(){return m_finish;}

private:

    friend class WordsArePlayingState;
    friend class WordsPlayedState;
    friend class ChoosesNextState;

    void addButtonsFromPos(int count, Point startPoint);
    void setUpCurrentTrade(bool playSound = true);
    void hideAllButtons();
    void showSelectionButtons();


    DialogCfg m_cfg;
    bool showDialog = false;

    PlayerPtr m_player;
    std::shared_ptr<NPC> m_npc;

    TextBox m_screen;
    std::vector<std::shared_ptr<Button>> m_playerChoiceButtons;

    std::array<std::shared_ptr<IState>, static_cast<size_t>(IState::States::Count)> m_allStates{
        std::make_shared<WordsArePlayingState>(),
        std::make_shared<ChoosesNextState>()};

    std::shared_ptr<IState> m_lastState;

    Trade m_trade;
    Trade m_initialTrade;
    MouseState m_lastMouseState;    
    bool m_finish;
};

using DlgPtr = std::shared_ptr<Dlg>;