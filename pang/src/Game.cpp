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

#include "levelparser.hpp"
#include "entityhelper.hpp"
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
	window.setVerticalSyncEnabled(true);

	font.loadFromFile("res/fonts/PublicPixel.ttf");

	fpsText.setFont(font);
	fpsText.setStyle(sf::Text::Bold);
	fpsText.setFillColor(sf::Color::White);
	fpsText.setPosition(10.f, 10.f);

	gameOverText.setFont(font);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setString("GAME OVER");

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

	auto hrs = std::make_unique<HitboxRenderingSystem>(registry, window);
	addRenderingSystem(std::move(hrs));

	auto is = std::make_unique<InfoSystem>(registry, window);
	addRenderingSystem(std::move(is));

	auto psts = std::make_unique<PlayerStateSystem>(registry);
	addSystem(std::move(psts));

	auto ps = std::make_unique<PlayerSystem>(registry);
	addSystem(std::move(ps));

	auto pws = std::make_unique<PlayerWeaponSystem>(registry);
	addSystem(std::move(pws));

	auto prs = std::make_unique<ProjectileSystem>(registry);
	addSystem(std::move(prs));

	auto gs = std::make_unique<GravitySystem>(registry);
	addSystem(std::move(gs));

	auto cs = std::make_unique<CollisionSystem>(registry);
	addSystem(std::move(cs));

	auto hs = std::make_unique<HealthSystem>(registry);
	addSystem(std::move(hs));

	auto player = registry.create();

	registry.emplace<Sprite>(player, "res/textures/player_standing.png");
	registry.emplace<Health>(player, 5);
	registry.emplace<Position>(player, sf::Vector2f(400.f, 600.f));
	registry.emplace<Mass>(player, 50.f);
	registry.emplace<Acceleration>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(50.f, 50.f));
	registry.emplace<Velocity>(player, sf::Vector2f(0.f, 0.f), sf::Vector2f(500.f, 500.f));
	Weapon wpn{Weapon::Type::Hook};
	registry.emplace<Player>(player, wpn);
	registry.emplace<Hitbox>(player, 64.f, 128.f);

	std::vector tiles = parseLevel("res/levels/lvl1.txt");
	for (Wall w : tiles) {
		if (w.type == Wall::Type::None) continue;
		
		auto tile = registry.create();

		registry.emplace<Wall>(tile, w);
		registry.emplace<Sprite>(tile, "res/textures/wall.png");
		registry.emplace<Hitbox>(tile, 16.f, 16.f);
		registry.emplace<Position>(tile, sf::Vector2f(w.pos.x * 16.f + 100.f, w.pos.y * 16.f + 250.f));
	}

	for (int i = 0; i < 4; ++i) 
		createBall(registry, sf::Vector2f(i * 25.f + 300.f, 400.f), sf::Vector2f(100.f, 50.f), 25.f, i + 1);
}

Game::~Game() {
	window.close();

	systems.clear();
	renderingSystems.clear();
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
	fpsText.setString("FPS: " + std::to_string(fps.getFPS()));
	fpsText.setScale(scale);
	gameOverText.setPosition(sf::Vector2f(windowWidth / 2, windowHeight / 2));
	gameOverText.setScale(scale);

	for (auto& s : systems) 
		if (!s->isPaused)
			s->update(deltaTime, scale, debug);

	const auto players = registry.view<Player>();

	for (const auto player : players) {
		const auto pl = registry.get<Player>(player);

		if (pl.isGameOver) isGameOver = true;
	}
}

void Game::render(const float deltaTime) {
	window.clear(sf::Color::Black);

	for (auto& rs : renderingSystems) 
		if (!rs->isPaused)
			rs->update(deltaTime, scale, debug);

	if (debug) {
		window.draw(fpsText);
	}

	if (isGameOver) {
		pause();
		window.draw(gameOverText);
	}

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
