#pragma once

#include "systems/systems.hpp"
#include "FPS.hpp"

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

	bool isPaused = false;
	bool isFocused = true;
	bool debug = false;

	unsigned int origWidth, origHeight;
	unsigned int windowWidth, windowHeight;

	sf::Vector2f scale;

	void pauseSystems();
	void resumeSystems();
};
