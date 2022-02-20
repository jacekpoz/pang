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

PlayerWeaponSystem::PlayerWeaponSystem(entt::registry& r) 
	: System(r) {}

void PlayerWeaponSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto pl = registry.get<Player>(player);

		if (pl.st != State::Shooting) continue;

		const auto plPos = registry.get<Position>(player);

		switch (pl.wpn.type) {
			case Weapon::Type::Hook:
				if (pl.wpn.isProjShot) break;
	
				auto hook = registry.create<Projectile>(Projectile::Hook);
				registry.emplace<Position>(hook, plPos.pos);
				registry.emplace<Hitbox>(hook, 20.f, 0.f);
				// TODO work out how to make the hooks move and also how to split the textures into parts yeah

				break;

		}
	}
}
