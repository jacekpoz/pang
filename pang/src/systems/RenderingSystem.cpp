#include "RenderingSystem.hpp"

#include "../util.hpp"

#include <variant>
#include <iostream>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>

RenderingSystem::RenderingSystem(sf::RenderWindow* w) : window(w) {}

RenderingSystem::~RenderingSystem() {
	sprCache.clear();
	txCache.clear();
}

void RenderingSystem::render(sf::Vector2f scale) {

	auto renderableView = registry->view<Position, Sprite>();
	for (auto& entity : renderableView) {
		auto& [pos, sprite] = renderableView.get(entity);
			
		sf::Texture& tx = getTexture(sprite.path);
		sf::Sprite& spr = getSprite(entity);

		// sets sprite origin to the center of the texture
		sf::FloatRect bounds = spr.getLocalBounds();
		sf::Vector2f localCenter{
			bounds.left + bounds.width / 2.f,
			bounds.top + bounds.height / 2.f
		};
		spr.setOrigin(localCenter);

		spr.setPosition(pos.pos);
		spr.setRotation(sprite.angle);
		spr.setScale(scale);
		
		window->draw(spr);
	
		// for debugging purposes
		// TODO make a debug mode so I don't have to comment this out every time
		if (registry->all_of<Hitbox>(entity)) {
			auto& hitbox = registry->get<Hitbox>(entity);
			if (hitbox.isRect) {
				Rect r = std::get<Rect>(hitbox.rect);
				sf::RectangleShape rect;
				rect.setOrigin(localCenter);
				rect.setPosition(pos.pos);
				rect.setSize(sf::Vector2f(r.w, r.h));
				rect.setScale(scale);
				rect.setRotation(sprite.angle);
				rect.setFillColor(sf::Color::Transparent);
				rect.setOutlineColor(sf::Color::White);
				rect.setOutlineThickness(1);
				window->draw(rect);
			} else if (!hitbox.isRect) {
				Circ c = std::get<Circ>(hitbox.circ);
				sf::CircleShape circ;
				circ.setOrigin(localCenter);
				circ.setPosition(pos.pos);
				circ.setRadius(c.r);
				circ.setScale(scale);
				circ.setRotation(sprite.angle);
				circ.setFillColor(sf::Color::Transparent);
				circ.setOutlineColor(sf::Color::White);
				circ.setOutlineThickness(1);
				window->draw(circ);
			}
		}
	}
}

void RenderingSystem::update(const float deltaTime, sf::Vector2f scale) { render(scale); }

sf::Texture& RenderingSystem::getTexture(std::string path) {

	if (txCache.find(path) == txCache.end()) {
		sf::Texture tx;
		if (!tx.loadFromFile(path)) {
			std::cout << "dupka\n";
			// TODO some fuckin logger idk man
		}
		txCache[path] = tx;
	}

	return txCache[path];
}

sf::Sprite& RenderingSystem::getSprite(entt::entity entity) {

	if (sprCache.find(entity) == sprCache.end()) {
		auto& sprite = registry->get<Sprite>(entity);
		
		sf::Sprite spr;
		spr.setTexture(getTexture(sprite.path));

		sprCache[entity] = spr;
	}

	return sprCache[entity];
}
