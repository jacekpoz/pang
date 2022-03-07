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

#include "AnimatedRenderingSystem.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

AnimatedRenderingSystem::AnimatedRenderingSystem(entt::registry& r, sf::RenderWindow& w) 
	: RenderingSystem(r, w) {}

void AnimatedRenderingSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto animated = registry.view<Position, Sprite>();

	for (const auto entity : animated) {
		const auto [pos, sprite] = animated.get(entity);
		if (!sprite.animated) continue;

		sprite.timeElapsed += deltaTime;

		if (sprite.timeElapsed >= sprite.frames * sprite.frameTime) 
			sprite.timeElapsed = 0.f;

		const sf::Vector2f scaledPos = {pos.pos.x * scale.x, pos.pos.y * scale.y};

		sf::Sprite& spr = getSprite(entity);
		spr.setTexture(getTexture(sprite.path));

		// stolen from https://stackoverflow.com/questions/52655335/animating-sprites-in-sfml-from-a-sprite-sheet/52656103#52656103
		float progress = sprite.timeElapsed;
		for (int f = 0; f < sprite.frames; ++f) {
			progress -= sprite.frameTime;

			if (progress <= 0.f || f == sprite.frames - 1) {
				spr.setTextureRect(sf::IntRect(f * sprite.width, 0, sprite.width, sprite.height));
				break;
			}
		}

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
