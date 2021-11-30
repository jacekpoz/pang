#include "PlayerMovementSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerMovementSystem::PlayerMovementSystem() {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(const float deltaTime, sf::Vector2f scale) {

	auto players = registry->view<Player, Position, Mass, Force, Acceleration, Velocity>();

	// yeah I know this isn't the best way to handle this
	// but I haven't learned event dispatchers in entt
	// or whatever they're called Idk maybe I'll do that soon
	// but it's not that high of a priority right know
	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
	) {
		for (auto entity : players) {
			auto& pos = players.get<Position>(entity);
			
			pos.pos.x -= 50.f * deltaTime * scale.x;
		} 
	}

	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
	) {
		for (auto entity : players) {
			auto& pos = players.get<Position>(entity);
			
			pos.pos.x += 50.f * deltaTime * scale.x;
		} 
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		if (!isSpacePressed) {
			isSpacePressed = true;
			for (auto entity : players) {
				auto& f = players.get<Force>(entity);
				auto& m = players.get<Mass>(entity);
				f.force.y -= 2000.f * m.mass * deltaTime * scale.y;
			}
		}
	} else {
		isSpacePressed = false;
	}
}
