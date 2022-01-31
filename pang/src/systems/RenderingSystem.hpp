#pragma once

#include "System.hpp"

#include <string>
#include <unordered_map>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <entt/entt.hpp>

using TextureCache = std::unordered_map<std::string, sf::Texture>;
using SpriteCache = std::unordered_map<entt::entity, sf::Sprite>;

class RenderingSystem : public System {

public:
	RenderingSystem(entt::registry& r, sf::RenderWindow* w);
	virtual ~RenderingSystem();

	void render(sf::Vector2f scale);
	// left here to retain inheritance, deltaTime serves no purpose
	// so this only calls render()
	void update(const float deltaTime, sf::Vector2f scale) override;
	
private:
	sf::RenderWindow* window;

	TextureCache txCache;
	sf::Texture& getTexture(std::string path);

	SpriteCache sprCache;
	sf::Sprite& getSprite(entt::entity entity);

};
