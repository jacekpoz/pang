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

		sf::Vertex point(scaledPos, sf::Color::Green);
		window.draw(&point, 1, sf::Points);
	}
}
