#pragma once

#include "System.hpp"

#include <string>
#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <entt/entt.hpp>

class RenderingSystem : public System {

public:
	RenderingSystem(entt::registry& r, sf::RenderWindow& w);

	virtual void update(const float deltaTime, const sf::Vector2f scale, const bool debug);
	
protected:
	sf::RenderWindow& window;

	std::unordered_map<std::string, sf::Texture> txCache;
	sf::Texture& getTexture(std::string path);

	std::unordered_map<entt::entity, sf::Sprite> sprCache;
	sf::Sprite& getSprite(entt::entity entity);

};
