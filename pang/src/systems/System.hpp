#pragma once

#include <SFML/System/Vector2.hpp>

#include <entt/entity/registry.hpp>

class System {

public:
	System();
	virtual ~System();

	void init(entt::registry& r);

	virtual void update(const float deltaTime, sf::Vector2f scale) = 0;
	void pause();
	void resume();
	void togglePause();

protected:
	entt::registry* registry;
	
	bool isPaused;
	
};
