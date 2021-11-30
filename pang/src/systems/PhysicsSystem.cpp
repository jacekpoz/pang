#include "PhysicsSystem.hpp"

PhysicsSystem::PhysicsSystem() {}

PhysicsSystem::~PhysicsSystem() {}

void PhysicsSystem::update(const float deltaTime, sf::Vector2f scale) {

	auto movables = registry->view<Force, Mass, Acceleration, Velocity, Position>();

	for (auto entity : movables) {
		auto& [f, m, a, v, p] = movables.get(entity);

		// the following calculations are structured like this:
		// variable = (physics shit) * computer shit
		// so that it's easier to see what's not in the formula
		// and is there just because doo doo fart computer problems
		// I did this in most systems that use some formula
		a.accel.x += (f.force.x * m.mass) * scale.x * deltaTime;
		a.accel.y += (f.force.y * m.mass) * scale.y * deltaTime;

		v.vel.x += (a.accel.x * deltaTime) * scale.x;
		v.vel.y += (a.accel.y * deltaTime) * scale.y;

		p.pos.x += (v.vel.x * deltaTime) * scale.x;
		p.pos.y += (v.vel.y * deltaTime) * scale.y;

		f.force.x = 0.f;
		f.force.y = 0.f;
	}
}
