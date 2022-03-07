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

#include "ProjectileSystem.hpp"

#include <SFML/Graphics/Rect.hpp>

ProjectileSystem::ProjectileSystem(entt::registry& r) 
	: System(r) {}

void ProjectileSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto projectiles = registry.view<Projectile>();

	for (const auto projectile : projectiles) {
		const auto proj = registry.get<Projectile>(projectile);

		if (proj.dead) {
			registry.destroy(projectile);
			continue;
		}

		switch (proj.type) {
			// no need for this as there's no other projectiles
			case Projectile::Type::Hook: {
				const float projSpeed = 100.f;
				registry.patch<Hitbox>(projectile, [deltaTime, scale, projSpeed](auto& h) 
						{ h.h += projSpeed * deltaTime * scale.y * 4.f; });
				registry.patch<Position>(projectile, [deltaTime, scale, projSpeed](auto& p) 
						{ p.pos.y -= projSpeed * deltaTime * scale.y * 2.f; });
			}break;
			default:{
				
			}break;
		}
	}
}
