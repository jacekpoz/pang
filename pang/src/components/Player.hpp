#pragma once

#include <string>

enum struct State {
	Standing, 
	// actively walking (pressing A or D)
	WalkingLeft, 
	WalkingRight, 
	Climbing,
	Falling, 
	Shooting, 
	Damaged, 
};

struct Weapon {

	enum class Type {
		Hook, 
		DoubleHook, 
		PowerHook, 
		SingleShot, 
		DoubleShot, 
	}; 

	Type type;
	std::string path;
};

struct Player {
	Weapon wpn;
	State st;
	State lastSt;
};
