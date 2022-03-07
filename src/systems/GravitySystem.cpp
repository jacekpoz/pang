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

#include "GravitySystem.hpp"

GravitySystem::GravitySystem(entt::registry& r) 
	: System(r) {}

void GravitySystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto view = registry.view<Acceleration, Mass>();

	for (const auto entity : view) {
		const auto m = view.get<Mass>(entity);

		registry.patch<Acceleration>(entity, [this, m, deltaTime, scale](auto& a) {
			a.accel.y += (m.mass * g) * deltaTime * scale.y;
		});
	}
}
