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

#include <string>

enum struct State {
	Standing, 
	WalkingLeft, 
	WalkingRight, 
	Climbing,
	Falling, 
	Shooting, 
	Damaged, 
};

struct Weapon {

	enum class Type {
		Hook, 
		// the rest are unimplemented due to lack of time
		DoubleHook, 
		PowerHook, 
		SingleShot, 
		DoubleShot, 
	}; 

	Type type;
	// how many projectiles are shot like alive and shit
	int projNum = 0;
};

struct Player {
	Weapon wpn;
	State st;
	State lastSt;

	float timeShooting = 0.f;

	int score = 0;

	bool isGameOver = false;
};
