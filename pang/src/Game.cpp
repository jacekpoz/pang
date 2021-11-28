#include "Game.hpp"

#include "util.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

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

	origWidth = windowWidth = mode.width;
	origHeight = windowHeight = mode.height;

	scale = sf::Vector2f(
		static_cast<float>(origWidth) / static_cast<float>(windowWidth), 
		static_cast<float>(origHeight) / static_cast<float>(windowHeight)
	);

	RenderingSystem* rs = new RenderingSystem(&window);
	addSystem(rs);

	PlayerMovementSystem* pms = new PlayerMovementSystem();
	addSystem(pms);

	MovementSystem* ms = new MovementSystem();
	addSystem(ms);

	CollisionSystem* cs = new CollisionSystem();
	addSystem(cs);

	auto player = registry.create();

	registry.emplace<Sprite>(player, "res\\textures\\player.png");
	registry.emplace<Position>(player, sf::Vector2f(100, 100), sf::Vector2f(100, 100));
	registry.emplace<Player>(player);
	Hitbox playerH;
	playerH.isRect = true;
	Rect playerRect;
	playerRect.w = 64;
	playerRect.h = 128;
	playerH.rect = playerRect;
	registry.emplace<Hitbox>(player, playerH);
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
			case sf::Event::Resized:
				windowWidth = event.size.width;
				windowHeight = event.size.height;
				break;
			default:
				break;
		}
	}
}

void Game::update(sf::Time deltaTime) {
	scale = sf::Vector2f(
		static_cast<float>(origWidth) / static_cast<float>(windowWidth), 
		static_cast<float>(origHeight) / static_cast<float>(windowHeight)
	);
	
	float dt = deltaTime.asSeconds();

	fps.update();
	
	for (auto s : systems) 
		s->update(dt, scale);

}

void Game::render() {
	window.clear(sf::Color::Black);

	for (auto rs : renderingSystems) {
		rs->render(scale);
	}

	window.display();
}

void Game::pause() {
	isPaused = true;
	for (auto s : systems) {
		s->pause();
	}
}

void Game::resume() {
	isPaused = false;
	for (auto s : systems) {
		s->resume();
	}
}

void Game::togglePause() {
	if (isPaused) {
		resume();
	} else {
		pause();
	}
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

