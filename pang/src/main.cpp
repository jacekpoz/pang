
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
