#include "SFML/Graphics/CircleShape.hpp"
#include "entt/entity/fwd.hpp"
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

struct Vector
{
    float x;
    float y;
};

struct Transform
{
    Vector position;
    Vector scale;
    Vector rotation;
};

class CircleShapeSystem
{
public:
    static void init(entt::registry *registry, sf::RenderWindow *window)
    {
        m_registry = registry;
        m_window = window;
    }

    static void update()
    {
    }

    static void display()
    {
        auto view = m_registry->view<const sf::CircleShape>();

        for (auto entity : view)
        {
            auto &circleShape = view.get<const sf::CircleShape>(entity);

            m_window->draw(circleShape);
        }

        // view.each([] (sf::CircleShape& spriteRenderer) {
        //     m_window->draw(spriteRenderer);
        // });
    }

private:
    static entt::registry *m_registry;
    static sf::RenderWindow *m_window;
};

entt::registry *CircleShapeSystem::m_registry = nullptr;
sf::RenderWindow *CircleShapeSystem::m_window = nullptr;

class SystemManager
{
public:
    SystemManager() : m_window(sf::VideoMode(800, 600), "LiveVox Engine")
    {
        // default
    }

    void run()
    {
        CircleShapeSystem::init(&m_registry, &m_window);

        auto entity = m_registry.create();
        m_registry.emplace<sf::CircleShape>(entity, sf::CircleShape(5.0f));

        while (m_window.isOpen())
        {
            for (sf::Event event; m_window.pollEvent(event);)
            {
                if (event.type == sf::Event::Closed)
                {
                    m_window.close();
                }
            }

            // Update systems
            CircleShapeSystem::update();

            m_window.clear();
            CircleShapeSystem::display();
            m_window.display();
        }
    }

private:
    sf::RenderWindow m_window;
    entt::registry m_registry;
};

int main()
{
    SystemManager system;
    system.run();
}