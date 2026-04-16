#include "GameManager.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "../cfg/Config.h"
#include "Logger.h"
#include "EventDispatcher.h"
#include "Scene.h"

#include "ResourceManagers/EntitiesManager.h"
#include "ResourceManagers/FontManager.h"
#include "ResourceManagers/SoundManager.h"
#include "ResourceManagers/TextureManager.h"
#include "menu/menu.h"


namespace
{
    int WINDOW_WIDTH = 800;
    int WINDOW_HEIGHT = 600;
}

void GameManager::Run()
{
    try
    {
        start();


        sf::RenderWindow window(sf::VideoMode({(unsigned int)WINDOW_WIDTH, (unsigned int)WINDOW_HEIGHT}), "SFML window");
        sf::View view2({0.f, 0.f}, {800.f, 600.f});
        //window.setView(view2);

        // Play the music
        Scene scene(SceneCfg("./cfg/scene1/scene1.ini"), {WINDOW_WIDTH, WINDOW_HEIGHT}, m_menu);
        scene.SetViewPos({0, 0});

        sf::Clock clock;
        std::shared_ptr<EventDispatcher> eventsDispatcher = std::make_shared<EventDispatcher>();
        while (window.isOpen())
        {
            window.clear();
            eventsDispatcher->HandleEvents(window);
            if(eventsDispatcher->IsClosed())
            {
                window.close();
                std::cout << "Window is closed \n";
                break;
            }
            auto elapsedTime = clock.restart().asSeconds();
            scene.UpdateOnEvents(eventsDispatcher->GetEvents(), elapsedTime);
                 
            scene.Draw(window);
            window.display();
        }
     }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        Logger::GetLogger().Error(e.what());
        Logger::GetLogger().Flush();
    }
    stop();
}

void GameManager::start()
{
    //Menu menu()
    m_menu = std::make_shared<Menu>(MenuCfg("./cfg/controls/menu.ini"), Point{50, 20});
}

void GameManager::stop()
{
    m_menu.reset();
    EntitiesManager::Get().Clear();
    SoundManager::Get().Clear();
    TextureManager::Get().Clear();
    FontManager::Get().Clear();    
}