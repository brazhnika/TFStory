#pragma once
#include <memory>
#include "menu/menu.h"

class GameManager
{
public:
    static GameManager& Get()
    {
        static GameManager manager;
        return manager;
    }

    void Run();

private:

    void start();
    void stop();

    std::shared_ptr<Menu> m_menu {nullptr};
};