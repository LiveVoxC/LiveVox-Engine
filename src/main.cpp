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

struct Sprite
{
    const sf::Texture* texture;
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
         
        this->renderCircleShape();
        this->renderRectangleShape();
        this->renderSprite();

        m_targetWindow->display();
    }

private:
    void renderRectangleShape()
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

    void renderCircleShape()
    {
        auto view = m_registry->view<CircleShape, Transform>();

        for (auto entity : view)
        {
            auto &cc = view.get<CircleShape>(entity);
            auto &tc = view.get<Transform>(entity);

            sf::CircleShape shape;
            shape.setRadius(cc.radius);
            shape.setFillColor(sf::Color(cc.color.r, cc.color.g, cc.color.b, cc.color.a));
            shape.setPosition({ tc.position.x, tc.position.y });
            shape.setScale({ tc.scale.x, tc.scale.y });
            shape.setRotation(tc.rotation);

            m_targetWindow->draw(shape);
        }
    }

    void renderSprite()
    {
        auto view = m_registry->view<Sprite, Transform>();

        for (auto entity : view)
        {
            auto &sc = view.get<Sprite>(entity);
            auto &tc = view.get<Transform>(entity);

            sf::Sprite sprite;
            sprite.setTexture(*sc.texture);
            sprite.setPosition(tc.position.x, tc.position.y);
            sprite.setScale(tc.scale.x, tc.scale.y);
            sprite.setRotation(tc.rotation);

            m_targetWindow->draw(sprite);
        }
    }

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