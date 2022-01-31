#include "GravitySystem.hpp"

GravitySystem::GravitySystem(entt::registry& r) : System(r) {}

GravitySystem::~GravitySystem() {}

void GravitySystem::update(const float deltaTime, sf::Vector2f scale) {
	auto view = registry->view<Acceleration, Mass>();

	for (auto entity : view) {
		auto m = view.get<Mass>(entity);

		registry->patch<Acceleration>(entity, [this, m, deltaTime, scale](auto& a) {
			a.accel.y += (m.mass * g) * deltaTime * scale.y;
		});
	}
}
