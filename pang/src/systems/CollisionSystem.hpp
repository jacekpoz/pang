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

#pragma once

#include "System.hpp"

// I decided to merge together PhysicsSystem and this
// cause I don't know if I'm doing it the wrong way or
// my code is just shitty, I couldn't figure out a way to do this
// without splitting collision and updating the physics apart

class CollisionSystem : public System {

public:
	CollisionSystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
	// could do those 2 in 1 method with a boolean but too lazy for now
	void updateX(const float deltaTime, const sf::Vector2f scale, const entt::entity entity);
	void updateY(const float deltaTime, const sf::Vector2f scale, const entt::entity entity);

private:
	bool collides(const Position p1, const Hitbox h1, const Position p2, const Hitbox h2);

};
