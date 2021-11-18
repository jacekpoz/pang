#pragma once

#include "systems/systems.hpp"
#include "FPS.hpp"

#include <vector>
#include <string>

#include <SFML/Graphics/RenderWindow.hpp>

#include <entt/entt.hpp>

class Game {

public:
	Game(sf::VideoMode mode, std::string title, uint32_t style);
	~Game();

	void handleEvent();
	void update(sf::Time deltaTime);
	void render();

	void pause();
	void resume();

	bool isRunning();

	void addSystem(System* s);
	void addSystem(RenderingSystem* rs);

private:
	sf::RenderWindow window;
	entt::registry registry;

	std::vector<System*> systems;

	std::vector<RenderingSystem*> renderingSystems;

	FPS fps;
	
};
