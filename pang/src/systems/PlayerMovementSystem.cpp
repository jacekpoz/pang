#include "PlayerMovementSystem.hpp"

#include <iostream>

#include <SFML/Window/Keyboard.hpp>

PlayerMovementSystem::PlayerMovementSystem() {}

PlayerMovementSystem::~PlayerMovementSystem() {}

void PlayerMovementSystem::update(const float deltaTime, sf::Vector2f scale) {

	auto players = registry->view<Player, Position>();
	
	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::A) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Left)
	) {
		for (auto entity : players) {
			auto& pos = players.get<Position>(entity);
			
			pos.pos.x -= 50 * deltaTime * scale.x;
		} 
	}

	if (
		sf::Keyboard::isKeyPressed(sf::Keyboard::D) || 
		sf::Keyboard::isKeyPressed(sf::Keyboard::Right)
	) {
		for (auto entity : players) {
			auto& pos = players.get<Position>(entity);
			
			pos.pos.x += 50 * deltaTime * scale.x;
		} 
	}
}
