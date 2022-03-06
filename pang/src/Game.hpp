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

#include "systems/systems.hpp"
#include "FPS.hpp"
#include "gui/Button.hpp"

#include <vector>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

#include <entt/entt.hpp>

class Game {

public:
	Game(sf::VideoMode mode, std::string title, uint32_t style);
	~Game();

	void handleEvent();
	void update(const float deltaTime);
	void render(const float deltaTime);

	void pause();
	void resume();
	void togglePause();

	void focus();
	void unfocus();

	void addSystem(std::unique_ptr<System> s);
	void addRenderingSystem(std::unique_ptr<RenderingSystem> rs);

	bool isRunning = true;

private:
	sf::RenderWindow window;
	entt::registry registry;

	std::vector<std::unique_ptr<System>> systems;

	std::vector<std::unique_ptr<RenderingSystem>> renderingSystems;

	sf::Font font;

	FPS fps;
	sf::Text fpsText;

	bool isGameOver = false;
	sf::Text gameOverText;

	bool isPaused = false;
	bool isFocused = true;
	bool debug = false;

	unsigned int origWidth, origHeight;
	unsigned int windowWidth, windowHeight;

	sf::Vector2f scale;

	void pauseSystems();
	void resumeSystems();

	bool mainMenu = true;
	sf::Text pangText;
	std::vector<Button> buttons;
};
