#pragma once

struct Health {
	int health = 0;
	bool damaged = false;
	// time left until can be damaged again
	float timeLeft = 0.f;

	bool collides = false;
};
