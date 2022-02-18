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

#include "PlayerStateSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerStateSystem::PlayerStateSystem(entt::registry& r) 
	: System(r) {}

void PlayerStateSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto vel = registry.get<Velocity>(player);

		registry.patch<Player>(player, [](auto& pl) { pl.lastSt = pl.st; });

		if (vel.vel.x == 0.f) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Standing; });

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || vel.vel.x < 0) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::WalkingLeft; });
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || vel.vel.y > 0) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::WalkingRight; });

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::A)) 
			registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
				if (vel.vel.x > 0.f) {
					vel.vel.x -= 75.f * deltaTime * scale.x;
					if (vel.vel.x < 0.f) vel.vel.x = 0.f;
				}
			});
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::D)) 
			registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
				if (vel.vel.x < 0.f) {
					vel.vel.x += 75.f * deltaTime * scale.x;
					if (vel.vel.x > 0.f) vel.vel.x = 0.f;
				}
			});
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Shooting; });


		if (vel.vel.y > 0.f) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Falling; });

		if (vel.vel.y < 0.f) 
			registry.patch<Player>(player, [](auto& pl) { pl.st = State::Climbing; });
	}
}
