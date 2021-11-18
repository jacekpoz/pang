
#include "Game.hpp"
#include <iostream>
#include <vector>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/System/Time.hpp>

int main() {

	Game* game = new Game(sf::VideoMode(800, 600), "dupka", sf::Style::Default);
	
	sf::Clock clock;
	
	while (game->isRunning()) {
		sf::Time deltaTime = clock.restart();

		game->handleEvent();
		game->update(deltaTime);
		game->render();
	}
	
	delete game;

	return 0;
}
