// Pang - a remake of the arcade game for the motorola science cup 2021
// Copyright (C) 2022 jacekpoz
// 
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
// 
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "RenderingSystem.hpp"

#include <variant>
#include <iostream>

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/System/Vector2.hpp>

RenderingSystem::RenderingSystem(entt::registry& r, sf::RenderWindow& w) 
	: System(r), window(w) {}

void RenderingSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	
	const auto renderableView = registry.view<Position, Sprite>();
	for (const auto entity : renderableView) {
		const auto [pos, sprite] = renderableView.get(entity);
		
		if (sprite.animated) continue;

		const sf::Vector2f scaledPos = {pos.pos.x * scale.x, pos.pos.y * scale.y};

		sf::Sprite& spr = getSprite(entity);

		// sets sprite origin to the center of the texture
		const sf::FloatRect bounds = spr.getLocalBounds();
		const sf::Vector2f localCenter{
			bounds.left + bounds.width / 2.f,
			bounds.top + bounds.height / 2.f
		};
		spr.setOrigin(localCenter);

		spr.setPosition(scaledPos);
		spr.setScale(scale);
		
		if (sprite.visible) window.draw(spr);	
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
		const auto sprite = registry.get<Sprite>(entity);
		
		sf::Sprite spr;
		spr.setTexture(getTexture(sprite.path));

		sprCache[entity] = spr;
	}

	return sprCache[entity];
}
