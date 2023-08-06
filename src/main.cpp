#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>

#include "Systems/RenderManager.hpp"

class Application
{
public:
    Application()
    {
        // 
    }

    void init()
    {
        sf::RenderWindow* window = this->createPrimaryWindow();

        m_renderManager.startUp(&m_registry, window);
        
        m_isInitialized = true;
    }

    void run()
    {
        if (!m_isInitialized) {
            throw std::runtime_error("Application not initialized!");         
        }

        // Sample entity spawning (we'll move this somewhere in the future)
        this->spawnTestComponents();

        auto view = m_registry.view<sf::RenderWindow>();

        for (auto entity : view)
        {
            auto &window = view.get<const sf::RenderWindow>(entity);            
            
            while (window.isOpen())
            {
                for (sf::Event event; window.pollEvent(event);)
                {
                    if (event.type == sf::Event::Closed)
                    {
                        window.close();
                    }
                }

                m_renderManager.update();
            }
        }
    }

private:
    void spawnTestComponents()
    {
        // Circle Shape Test
        {
            auto circle = m_registry.create();

            auto& tc = m_registry.emplace<Transform>(circle);
            tc.position = { 50.f, 100.f };
            tc.scale = { 1.f, 1.f };
            tc.rotation = 0;

            auto& cc = m_registry.emplace<CircleShape>(circle);
            cc.radius = 25.f;
            cc.color = { 255, 0, 0, 255 };
        }
        
        // Retangle Shape test
        {
            auto rectangle = m_registry.create();

            auto& rc = m_registry.emplace<Transform>(rectangle);
            rc.position = { 200.f, 50.f };
            rc.scale = { 1.f, 1.f };
            rc.rotation = 45;

            auto& rs = m_registry.emplace<RectangleShape>(rectangle);
            rs.size = { 50.f, 50.f };
            rs.color = { 0, 255, 0, 255 };
        }

        // Sprite Shape test
        {
            static sf::Texture texture;
        
            if (!texture.loadFromFile(R"(..\assets\sfml-icon-small.png)")) {
                std::cerr << "read failed" << std::endl;
                return;
            }

            auto sprite = m_registry.create();

            auto& tc = m_registry.emplace<Transform>(sprite);
            tc.position = { 100.f, 200.f };
            tc.scale = { 0.5f, 0.5f };
            tc.rotation = 0;

            auto& sc = m_registry.emplace<Sprite>(sprite);
            sc.texture = &texture;
        }    
    }

    bool m_isInitialized = 0;

    entt::registry m_registry;

    RenderManager m_renderManager;

    sf::RenderWindow* createPrimaryWindow()
    {
        auto entity = m_registry.create();   

        auto& window = m_registry.emplace<sf::RenderWindow>(entity, sf::VideoMode(800, 600), "LiveVox Engine");

        return &window;
    }
};

int main()
{
    Application app;
    app.init();
    app.run();
}