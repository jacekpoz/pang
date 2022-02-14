#include "HitboxRenderingSystem.hpp"

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Color.hpp>

HitboxRenderingSystem::HitboxRenderingSystem(entt::registry& r, sf::RenderWindow& w) 
	: RenderingSystem(r, w) {}

void HitboxRenderingSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	if (!debug) return;

	const auto entities = registry.view<Position, Hitbox>();

	for (const auto entity : entities) {
		const auto [p, h] = entities.get(entity);

		const sf::Vector2f scaledPos = {p.pos.x * scale.x, p.pos.y * scale.y};

		sf::Sprite& spr = getSprite(entity);
		const sf::FloatRect bounds = spr.getLocalBounds();

		const sf::Vector2f localCenter{
			bounds.left + bounds.width / 2.f,
			bounds.top + bounds.height / 2.f
		};

		sf::RectangleShape rect;
		rect.setOrigin(localCenter);
		rect.setPosition(scaledPos);
		rect.setSize(sf::Vector2f(h.w, h.h));
		rect.setScale(scale);
		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color::White);
		rect.setOutlineThickness(1);
		window.draw(rect);
	}
}
