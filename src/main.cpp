#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/Transform.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>
#include <iostream>
#include <stdint.h>
/*
1st Iteration 
=============

- [ ] Renderer System
- [ ] Audio System
- [ ] Physics System
- [ ] Input System

Success Indicator:

[ ] Graphics & Input
Display a simple white rectangle shape, you're able to make it jump using by
pressing space.

[ ] Audio & Physics
The rectangle would play a sound when it starts jumping and when
it struck the floor.

*/

struct Vector2f
{
    float x;
    float y;
};

struct Transform
{
    Vector2f position;
    Vector2f scale;
    uint32_t rotation;
};

struct Color4
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

struct RectangleShape
{
    Vector2f size;
    Color4 color;
};

struct CircleShape
{
    float radius;
    Color4 color;
};

struct SpriteRenderer
{
    const sf::Texture* texture;
    sf::IntRect textureRect;
};

class RenderManager
{
public:
    void startUp(entt::registry* registry, sf::RenderWindow* window)
    {
        m_registry = registry;
        m_targetWindow = window;
    }

    void update()
    {
        m_targetWindow->clear();

        // Handle sprite drawables
        
        // {
        //     auto view = m_registry->view<SpriteRenderer, Transform>();
        //     sf::Vertex vertices[4];

        //     for (auto entity : view)
        //     {
        //         auto &sc = view.get<SpriteRenderer>(entity);
        //         auto &tc = view.get<Transform>(entity);

        //         // Calculate sprite's rectangle
        //         const auto width = std::abs(sc.textureRect.width);
        //         const auto height = std::abs(sc.textureRect.height);

        //         vertices[0].position = sf::Vector2f(0, 0); 
        //         vertices[1].position = sf::Vector2f(0, height);
        //         vertices[2].position = sf::Vector2f(width, 0);                
        //         vertices[3].position = sf::Vector2f(width, height);

        //         sf::Transformable transform;
        //         transform.setPosition(tc.position.x, tc.position.y);
        //         transform.setScale(tc.scale.x, tc.scale.y);
        //         transform.setRotation(tc.rotation);

        //         sf::RenderStates states;
        //         states.texture = sc.texture;
        //         states.transform = transform.getTransform();

        //         m_targetWindow->draw(vertices, 4, sf::PrimitiveType::TriangleStrip, states);
        //     }
        // }
         
        // Handle rectangle shape drawables
        {
            auto view = m_registry->view<RectangleShape, Transform>();

            for (auto entity : view)
            {
                auto &rs = view.get<RectangleShape>(entity);
                auto &tc = view.get<Transform>(entity);

                sf::RectangleShape shape;
                shape.setSize({ rs.size.x, rs.size.y });
                shape.setScale({ tc.scale.x, tc.scale.y });
                shape.setPosition({ tc.position.x, tc.position.y });
                shape.setFillColor(sf::Color(rs.color.r, rs.color.g, rs.color.b, rs.color.a));

                m_targetWindow->draw(shape);
            }
        }

        // Handle all other drawables aside from sprite

        m_targetWindow->display();
    }

private:
    entt::registry* m_registry;
    sf::RenderWindow* m_targetWindow;
};

class Application
{
public:
    Application()
    {
        // default
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
        
        auto entity = m_registry.create();

        auto& rc = m_registry.emplace<Transform>(entity);
        rc.position = { 50.f, 50.f };
        rc.scale = { 1.f, 1.f };
        rc.rotation = 0;

        auto& rs = m_registry.emplace<RectangleShape>(entity);
        rs.size = { 50.f, 50.f };
        rs.color = { 255, 255, 255, 255 };

        // auto entity = m_registry.create();
        // auto& transform = m_registry.emplace<Transform>(entity);
        // auto& spriteRenderer = m_registry.emplace<SpriteRenderer>(entity);

        // transform.position = { 50.f, 50.f };

        // if (!spriteRenderer.texture.loadFromFile(R"(..\assets\sfml-icon-small.png)")) {
        //     std::cerr << "read failed" << std::endl;
        //     return;
        // }

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