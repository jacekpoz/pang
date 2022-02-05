#pragma once

#include "../components/components.hpp"

#include <SFML/System/Vector2.hpp>

#include <entt/entity/registry.hpp>

#define FMT_HEADER_ONLY
#define FMTLOG_HEADER_ONLY
#include "../fmtlog.h"

class System {

public:
	System(entt::registry& r);

	virtual void update(const float deltaTime, const sf::Vector2f scale, const bool debug) = 0;
	void pause();
	void resume();
	void togglePause();

	bool isPaused;

protected:
	entt::registry& registry;
};
