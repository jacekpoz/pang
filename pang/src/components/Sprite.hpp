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

struct Sprite {
	std::string path;
	bool animated = false;
	// if the sprite is animated 
	// it's probably in a spritesheet
	// so we need those for the uhhhhh ummmmm
	int width = 0, height = 0;
	int frames = 0;
	// in seconds
	float frameTime = 0;
	float timeElapsed = 0;

	bool visible = true;
};
