#include "GravitySystem.hpp"

GravitySystem::GravitySystem() {}

GravitySystem::~GravitySystem() {}

void GravitySystem::update(const float deltaTime, sf::Vector2f scale) {
	auto view = registry->view<Force>();

	for (auto entity : view) {
		auto& force = view.get<Force>(entity);

		
	}
}
