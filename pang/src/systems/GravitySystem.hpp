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

/*
 * yes the sole purpose of this system is to handle gravity
 * it's literally gonna be like 3 lines of actual code that gets ran but I don't care
 * if you have a problem with it go cry about it
 */
class GravitySystem : public System {

public:
	GravitySystem(entt::registry& r);

	void update(const float deltaTime, const sf::Vector2f scale, const bool debug);

private:
	// gravitational constant
	const float g = 9.81f;
};
