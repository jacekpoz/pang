#include "MovementSystem.hpp"

#include "../components/components.hpp"

MovementSystem::MovementSystem() {}

MovementSystem::~MovementSystem() {}

void MovementSystem::update(const float deltaTime, sf::Vector2f scale) {

	auto movables = registry->view<Position>();

	for (auto entity : movables) {
		auto& pos = movables.get<Position>(entity);

		// do some stuff

		pos.pos = pos.nextPos;
	}
}
