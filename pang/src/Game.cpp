#include "Game.hpp"

#include "util.hpp"

#include <iostream>
#include <random>
#include <variant>
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/System/Vector2.hpp>

Game::Game(sf::VideoMode mode, std::string title, uint32_t style) {
	window.create(mode, title, style);
	window.setVerticalSyncEnabled(true);

	RenderingSystem* rs = new RenderingSystem(&window);
	addSystem(rs);

	
}

Game::~Game() {

	for (auto s : systems) 
		delete s;
	systems.clear();

	for (auto rs : renderingSystems) 
		delete rs;
	renderingSystems.clear();

	window.close();
	
	std::cout << "dupka" << std::endl;
}

void Game::handleEvent() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::LostFocus:
				pause();
				break;
			case sf::Event::GainedFocus:
				resume();
				break;
			default:
				break;
		}
	}
}

void Game::update(sf::Time deltaTime) {
	float dt = deltaTime.asSeconds();

	fps.update();
	
	for (auto s : systems) 
		s->update(dt);

}

void Game::render() {
	window.clear(sf::Color::Black);

	for (auto rs : renderingSystems) {
		rs->render();
	}

	window.display();
}

void Game::pause() {
	
}

void Game::resume() {
	
}

bool Game::isRunning() {
	return window.isOpen();
}

void Game::addSystem(System* s) {
	s->init(registry);
	systems.push_back(s);
}

void Game::addSystem(RenderingSystem* rs) {
	rs->init(registry);
	renderingSystems.push_back(rs);
}

