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

#include "Game.hpp"
#include <iostream>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

#define FMT_HEADER_ONLY
#define FMTLOG_HEADER_ONLY
#include "fmtlog.h"

int main() {

	fmtlog::setLogFile("logs/pang.log", true);

	Game game = Game(sf::VideoMode(800, 600), "dupka", sf::Style::Default);
	
	sf::Clock clock;
	
	while (game.isRunning) {
		sf::Time deltaTime = clock.restart();

		game.handleEvent();
		game.update(deltaTime.asSeconds());
		game.render(deltaTime.asSeconds());
	}
	
	return 0;
}
