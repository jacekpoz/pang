#include "RenderingSystem.hpp"

#include <variant>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

RenderingSystem::RenderingSystem(entt::registry& r, sf::RenderWindow& w) 
	: System(r), window(w) {}

void RenderingSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	
	auto renderableView = registry.view<Position, Sprite>();
	for (auto& entity : renderableView) {
		auto [pos, sprite] = renderableView.get(entity);
		
		if (sprite.animated) continue;

		sf::Vector2f scaledPos = {pos.pos.x * scale.x, pos.pos.y * scale.y};

		sf::Sprite& spr = getSprite(entity);

		// sets sprite origin to the center of the texture
		sf::FloatRect bounds = spr.getLocalBounds();
		sf::Vector2f localCenter{
			bounds.left + bounds.width / 2.f,
			bounds.top + bounds.height / 2.f
		};
		spr.setOrigin(localCenter);

		spr.setPosition(scaledPos);
		spr.setScale(scale);
		
		if (sprite.visible) window.draw(spr);
	
		if (debug && registry.all_of<Hitbox>(entity)) {
			auto& h = registry.get<Hitbox>(entity);
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
}

sf::Texture& RenderingSystem::getTexture(std::string path) {

	if (txCache.find(path) == txCache.end()) {
		sf::Texture tx;
		if (!tx.loadFromFile(path)) {
			logw("texture not found: {}", path);
			tx.loadFromFile("res/textures/null.png");
		}
		txCache[path] = tx;
	}

	return txCache[path];
}

sf::Sprite& RenderingSystem::getSprite(entt::entity entity) {

	if (sprCache.find(entity) == sprCache.end()) {
		auto& sprite = registry.get<Sprite>(entity);
		
		sf::Sprite spr;
		spr.setTexture(getTexture(sprite.path));

		sprCache[entity] = spr;
	}

	return sprCache[entity];
}
