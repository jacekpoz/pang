#include "GravitySystem.hpp"

GravitySystem::GravitySystem() {}

GravitySystem::~GravitySystem() {}

void GravitySystem::update(const float deltaTime, sf::Vector2f scale) {
	auto view = registry->view<Force, Mass>();

	for (auto entity : view) {
		auto [f, m] = view.get(entity);

		f.force.y += (m.mass * g) * deltaTime * scale.y;
	}
}
