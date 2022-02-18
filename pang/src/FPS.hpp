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

#include <SFML/System/Clock.hpp>

// stolen from https://stackoverflow.com/questions/66787248/cant-get-my-fps-number-to-show-with-sfml

class FPS {

public:

	const unsigned int getFPS() const;
	void update();
	
private:
	unsigned int mFrame = 0;
	unsigned int mFPS = 0;
	sf::Clock mClock;
};
