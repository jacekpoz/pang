#include "Game.hpp"

#include "levelparser.hpp"
#include "util.hpp"
#include "components/components.hpp"
#include "systems/systems.hpp"

#include <iostream>
#include <random>
#include <variant>
#include <vector>
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
	window.setVerticalSyncEnabled(false);

	origWidth = windowWidth = mode.width;
	origHeight = windowHeight = mode.height;

	scale = sf::Vector2f(
		static_cast<float>(origWidth) / static_cast<float>(windowWidth), 
		static_cast<float>(origHeight) / static_cast<float>(windowHeight)
	);

	RenderingSystem* rs = new RenderingSystem(registry, &window);
	addSystem(rs);

	PlayerMovementSystem* pms = new PlayerMovementSystem(registry);
	addSystem(pms);

	GravitySystem* gs = new GravitySystem(registry);
	addSystem(gs);

	CollisionSystem* cs = new CollisionSystem(registry);
	addSystem(cs);

	auto player = registry.create();

	registry.emplace<Sprite>(player, "res/textures/player.png");
	registry.emplace<Position>(player, sf::Vector2f(200.f, 200.f));
	registry.emplace<Mass>(player, 50.f);
	registry.emplace<Acceleration>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f));
	registry.emplace<Velocity>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(500.f, 500.f));
	registry.emplace<Player>(player);
	Hitbox playerH;
	playerH.w = 64;
	playerH.h = 128;
	registry.emplace<Hitbox>(player, playerH);

	std::vector tiles = parseLevel("res/levels/lvl1.txt");
	for (Wall w : tiles) {
		if (w.type == Wall::Type::None) continue;
		
		auto tile = registry.create();

		registry.emplace<Wall>(tile, w);
		registry.emplace<Sprite>(tile, "res/textures/wall.png");
		Hitbox wallH;
		wallH.w = 64;
		wallH.h = 64;
		registry.emplace<Hitbox>(tile, wallH);
		registry.emplace<Position>(tile, sf::Vector2f(w.pos.x * wallH.w + 50, w.pos.y * wallH.h + 50));
	}
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
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Q) window.close();
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
	std::cout << "fps: " << fps.getFPS() << " "/* << "\n"*/;
	
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
	systems.push_back(s);
}

void Game::addSystem(RenderingSystem* rs) {
	renderingSystems.push_back(rs);
}

