#include "Game.hpp"

#include "levelparser.hpp"
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

	auto rs = std::make_unique<RenderingSystem>(registry, window);
	addRenderingSystem(std::move(rs));

	auto ars = std::make_unique<AnimatedRenderingSystem>(registry, window);
	addRenderingSystem(std::move(ars));

	auto pms = std::make_unique<PlayerMovementSystem>(registry);
	addSystem(std::move(pms));

	auto gs = std::make_unique<GravitySystem>(registry);
	addSystem(std::move(gs));

	auto cs = std::make_unique<CollisionSystem>(registry);
	addSystem(std::move(cs));

	auto hs = std::make_unique<HealthSystem>(registry);
	addSystem(std::move(hs));

	auto player = registry.create();

	registry.emplace<Sprite>(player, "res/textures/player.png");
	registry.emplace<Health>(player, 5);
	registry.emplace<Position>(player, sf::Vector2f(400.f, 200.f));
	registry.emplace<Mass>(player, 50.f);
	registry.emplace<Acceleration>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f));
	registry.emplace<Velocity>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(500.f, 500.f));
	registry.emplace<Player>(player);
	registry.emplace<Hitbox>(player, 64.f, 128.f);

	std::vector tiles = parseLevel("res/levels/lvl1.txt");
	for (Wall w : tiles) {
		if (w.type == Wall::Type::None) continue;
		
		auto tile = registry.create();

		registry.emplace<Wall>(tile, w);
		registry.emplace<Sprite>(tile, "res/textures/wall.png");
		registry.emplace<Hitbox>(tile, 64.f, 64.f);
		registry.emplace<Position>(tile, sf::Vector2f(w.pos.x * 64.f + 50.f, w.pos.y * 64.f + 50.f));
	}

	for (int i = 0; i < 10; ++i) {
		auto ball = registry.create();

		registry.emplace<Sprite>(ball, "res/textures/ball1.png");
		registry.emplace<Position>(ball, sf::Vector2f(i * 25.f + 100.f, 200.f));
		registry.emplace<Mass>(ball, 10.f);
		registry.emplace<Acceleration>(ball, sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f));
		registry.emplace<Velocity>(ball, sf::Vector2f(50.f, 50.f), sf::Vector2f(500.f, 500.f));
		registry.emplace<Ball>(ball, 1);
		registry.emplace<Hitbox>(ball, 20.f, 20.f);
	}
}

Game::~Game() {
	window.close();

	systems.clear();
	renderingSystems.clear();

	std::cout << "dupka" << std::endl;
}

void Game::handleEvent() {
	sf::Event event;
	while (window.pollEvent(event)) {
		switch (event.type) {
			case sf::Event::Closed:
				isRunning = false;
				break;
			case sf::Event::LostFocus:
				unfocus();
				break;
			case sf::Event::GainedFocus:
				focus();
				break;
			case sf::Event::Resized:
				windowWidth = event.size.width;
				windowHeight = event.size.height;
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code) {
					case sf::Keyboard::Q: isRunning = false; break;
					case sf::Keyboard::G: debug = !debug; break;
					case sf::Keyboard::P: togglePause();  break;
					default: break;
				}
				break;
			default:
				break;
		}
	}
}

void Game::update(const float deltaTime) {
	scale = sf::Vector2f(
		static_cast<float>(origWidth) / static_cast<float>(windowWidth), 
		static_cast<float>(origHeight) / static_cast<float>(windowHeight)
	);
	
	fps.update();
	if (!isPaused && isFocused) std::cout << "fps: " << fps.getFPS() << " "/* << "\n"*/;
	
	for (auto& s : systems) 
		if (!s->isPaused)
			s->update(deltaTime, scale, debug);

}

void Game::render(const float deltaTime) {
	window.clear(sf::Color::Black);

	for (auto& rs : renderingSystems) 
		if (!rs->isPaused)
			rs->update(deltaTime, scale, debug);

	window.display();
}

void Game::pause() {
	isPaused = true;
	pauseSystems();
}

void Game::resume() {
	isPaused = false;
	resumeSystems();
}

void Game::togglePause() {
	if (isPaused) {
		resume();
	} else {
		pause();
	}
}

void Game::focus() {
	isFocused = true;
	if (!isPaused) resumeSystems();
}

void Game::unfocus() {
	isFocused = false;
	pauseSystems();
}

void Game::addSystem(std::unique_ptr<System> s) {
	systems.push_back(std::move(s));
}

void Game::addRenderingSystem(std::unique_ptr<RenderingSystem> rs) {
	renderingSystems.push_back(std::move(rs));
}

void Game::pauseSystems() {
	for (auto& s : systems) 
		s->pause();
}

void Game::resumeSystems() {
	for (auto& s : systems) 
		s->resume();
}
