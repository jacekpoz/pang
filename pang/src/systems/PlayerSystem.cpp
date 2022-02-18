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

#include "PlayerSystem.hpp"

PlayerSystem::PlayerSystem(entt::registry& r) 
	: System(r) {}

void PlayerSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {

	const auto players = registry.view<Player>();

	for (const auto player : players) {	
		const auto [pl] = players.get(player);

		switch (pl.st) {
			case State::WalkingRight:
				registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
					vel.vel.x += 500.f * deltaTime * scale.x;
				});
				break;
			case State::WalkingLeft:
				registry.patch<Velocity>(player, [deltaTime, scale](auto& vel) {
					vel.vel.x -= 500.f * deltaTime * scale.x;
				});
				break;
			case State::Shooting:

			default:
				break;
		}

		if (pl.st == pl.lastSt) continue;

		switch (pl.st) {
			case State::WalkingRight:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = true;
					s.path = "res/textures/player_walking_right.png";
					s.width = 64; s.height = 128; s.frames = 10;
					s.frameTime = 0.1f;
				});
				break;
			case State::WalkingLeft:
				registry.patch<Sprite>(player, [](auto &s) {
					s.animated = true;
					s.path = "res/textures/player_walking_left.png";
					s.width = 64; s.height = 128; s.frames = 10;
					s.frameTime = 0.1f;
				});
				break;
			case State::Standing:
				registry.patch<Sprite>(player, [](auto& s) {
					s.animated = false;
					s.path = "res/textures/player.png";
				});
				break;
			default:
				break;
		}
	}
}
