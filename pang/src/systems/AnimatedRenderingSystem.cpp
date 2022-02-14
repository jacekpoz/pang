#include "AnimatedRenderingSystem.hpp"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <iostream>

AnimatedRenderingSystem::AnimatedRenderingSystem(entt::registry& r, sf::RenderWindow& w) 
	: RenderingSystem(r, w) {}

void AnimatedRenderingSystem::update(const float deltaTime, const sf::Vector2f scale, const bool debug) {
	const auto animated = registry.view<Position, Sprite>();

	for (const auto entity : animated) {
		const auto [pos, sprite] = animated.get(entity);
		if (!sprite.animated) continue;

		sprite.timeElapsed += deltaTime;

		if (sprite.timeElapsed >= sprite.frames * sprite.frameTime) 
			sprite.timeElapsed = 0.f;

		const sf::Vector2f scaledPos = {pos.pos.x * scale.x, pos.pos.y * scale.y};

		sf::Sprite& spr = getSprite(entity);

		// stolen from https://stackoverflow.com/questions/52655335/animating-sprites-in-sfml-from-a-sprite-sheet/52656103#52656103
		float progress = sprite.timeElapsed;
		for (int f = 0; f < sprite.frames; ++f) {
			progress -= sprite.frameTime;

			if (progress <= 0.f || f == sprite.frames - 1) {
				spr.setTextureRect(sf::IntRect(f * sprite.width, 0, sprite.width, sprite.height));
				break;
			}
		}

		const sf::FloatRect bounds = spr.getLocalBounds();
		const sf::Vector2f localCenter{
			bounds.left + bounds.width / 2.f,
			bounds.top + bounds.height / 2.f
		};
		spr.setOrigin(localCenter);

		spr.setPosition(scaledPos);
		spr.setScale(scale);
		
		if (sprite.visible) window.draw(spr);
	}
}
