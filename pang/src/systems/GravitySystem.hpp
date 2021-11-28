#pragma once

#include "System.hpp"

class GravitySystem : public System {

public:
	GravitySystem();
	~GravitySystem();

	void update(const float deltaTime, sf::Vector2f scale);
};
