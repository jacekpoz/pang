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
#include <filesystem>

#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>

Game::Game(sf::VideoMode mode, std::string title, uint32_t style) {
	window.create(mode, title, style);
	window.setVerticalSyncEnabled(true);

	font.loadFromFile("res/fonts/PublicPixel.ttf");

	pangText.setFont(font);
	pangText.setStyle(sf::Text::Bold);
	pangText.setFillColor(sf::Color::White);
	pangText.setCharacterSize(30);
	pangText.setString("Pang");

	fpsText.setFont(font);
	fpsText.setStyle(sf::Text::Bold);
	fpsText.setFillColor(sf::Color::White);
	fpsText.setPosition(10.f, 10.f);

	gameOverText.setFont(font);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setString("GAME OVER");

	pauseText.setFont(font);
	pauseText.setFillColor(sf::Color::White);
	pauseText.setString("PAUSED");

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

	const std::filesystem::path levels("res/levels");
	int level = 0;
	for (const auto &file : std::filesystem::directory_iterator(levels)) {
		++level;
		Button b;
		b.baseColor = sf::Color(99, 102, 106);
		b.hoverColor = sf::Color(217, 217, 214);
		b.levelPath = file.path();
		b.text.setString(std::to_string(level));
		b.text.setFillColor(sf::Color::White);
		b.text.setFont(font);
		const float x = ((windowWidth / 2.f - 25.f) + ((level % 3) * 25.f)) * scale.x;
		const float y = ((windowHeight / 2.f - 100.f) + ((level / 3) * 50.f)) * scale.y;
		const float w = 25.f * scale.x;
		const float h = 25.f * scale.y;

		b.text.setPosition(x, y);
		b.bounds = sf::FloatRect{x, y, w, h};

		buttons.push_back(b);
	}

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
					case sf::Keyboard::X: mainMenu = true; isGameOver = false; break;
					default: break;
				}
				break;
			case sf::Event::MouseMoved:
				if (!mainMenu) break;
				for (Button b : buttons) 
					b.isHover = b.bounds.contains(event.mouseMove.x * scale.x, event.mouseMove.y * scale.y);
				break;
			case sf::Event::MouseButtonPressed:
				if (!mainMenu) break;
				for (Button b : buttons) {
					if (b.bounds.contains(event.mouseButton.x * scale.x, event.mouseButton.y * scale.y)) {
						mainMenu = false;
						parseLevel(registry, b.levelPath, sf::Vector2f{100.f, 100.f});
						resume();
					}
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

	if (mainMenu) {
		// quick and dirty workaround for destroying all entities
		// all the entities should have Position XdXdXd
		for (auto entity : registry.view<Position>()) registry.destroy(entity);
		pangText.setPosition(windowWidth / 2.f * scale.x, 25.f * scale.y);
		pangText.setScale(scale);
		int i = 0;
		for (Button b : buttons) {
			++i;
			const float x = ((windowWidth / 2.f - 100.f) + ((i % 3) * 100.f)) * scale.x;
			const float y = ((windowHeight / 2.f - 100.f) + ((i / 3) * 50.f)) * scale.y;
			const float w = 25.f * scale.x;
			const float h = 25.f * scale.y;

			b.bounds = sf::FloatRect{x, y, w, h};

			b.text.setPosition(x, y);
			b.text.setScale(scale);
		}
		return;
	}
	
	fps.update();
	fpsText.setString("FPS: " + std::to_string(fps.getFPS()));
	fpsText.setScale(scale);
	gameOverText.setPosition(sf::Vector2f(windowWidth / 2 * scale.x, windowHeight / 2 * scale.y));
	gameOverText.setScale(scale);
	pauseText.setPosition(sf::Vector2f(windowWidth / 2 * scale.x, windowHeight / 2 * scale.y));
	pauseText.setScale(scale);

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

	if (mainMenu) {

		window.draw(pangText);

		for (Button b : buttons) {
			sf::RectangleShape btn;
			btn.setPosition(b.bounds.left, b.bounds.top);
			btn.setSize(sf::Vector2f{b.bounds.width, b.bounds.height});
			btn.setFillColor(b.isHover ? b.hoverColor : b.baseColor);
			window.draw(btn);
			window.draw(b.text);
		}

		window.display();
		return;
	}

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

	if (isPaused) {
		window.draw(pauseText);
	}

	window.display();
}

void Game::pause() {
	isPaused = true;
	pauseSystems();
}

void Game::resume() {
	if (isGameOver) return;
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
