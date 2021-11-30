#pragma once

/* I know no components are used here
 * or in the implementation of this header
 * but components are going to be used in most, 
 * if not all of the systems so why don't I make
 * it easier for myself so I don't have to write
 * the include each fucking time in this already
 * tedious to write language
 */
#include "../components/components.hpp"

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
