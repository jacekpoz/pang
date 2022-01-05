#pragma once

#include "System.hpp"

class CollisionSystem : public System {

public:
	CollisionSystem();
	~CollisionSystem();

	void update(const float deltaTime, sf::Vector2f scale);

private:
	entt::observer movedEntities;
};
