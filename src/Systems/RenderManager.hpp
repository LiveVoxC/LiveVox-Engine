#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "../Components/Rendering/CircleShape.hpp"
#include "../Components/Rendering/RectangleShape.hpp"
#include "../Components/Rendering/Sprite.hpp"
#include "../Components/Core/Transform.hpp"
#include "../Components/Core/Color4.hpp"
#include "../Components/Core/Vector2f.hpp"


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