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

#include "PlayerWeaponSystem.hpp"

#include <iostream>

#include <SFML/System/Vector2.hpp>

PlayerWeaponSystem::PlayerWeaponSystem(entt::registry& r) 
	: System(r) {}

void PlayerWeaponSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto pl = registry.get<Player>(player);

		if (pl.st != State::Shooting) continue;

		const auto plPos = registry.get<Position>(player);
		const auto plHitbox = registry.get<Hitbox>(player);

		// unnecessary switch - no time to implement other weapons but who cares
		switch (pl.wpn.type) {
			case Weapon::Type::Hook: {
				if (pl.wpn.projNum > 0 || pl.timeShooting != 0.f) break;

				registry.patch<Player>(player, [](auto &pl) { ++pl.wpn.projNum; });

				auto hook = registry.create();
				registry.emplace<Projectile>(hook, Projectile::Type::Hook);
				registry.emplace<Sprite>(hook, "res/textures/hook.png");
				registry.emplace<Position>(hook, sf::Vector2f{
					plPos.pos.x,
					plPos.pos.y + plHitbox.h / 2.f - 10.f * scale.y
				});
				registry.emplace<Hitbox>(hook, 20.f, 10.f);

			}break;
			default: {

			}break;
		}
	}
}
