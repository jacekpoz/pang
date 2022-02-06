#include "GravitySystem.hpp"

GravitySystem::GravitySystem(entt::registry& r) 
	: System(r) {}

void GravitySystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto view = registry.view<Acceleration, Mass>();

	for (const auto entity : view) {
		const auto m = view.get<Mass>(entity);

		registry.patch<Acceleration>(entity, [this, m, deltaTime, scale](auto& a) {
			a.accel.y += (m.mass * g) * deltaTime * scale.y;
		});
	}
}
