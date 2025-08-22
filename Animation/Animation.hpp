#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../Vector/Vec2.hpp"

class Animation {
	sf::Sprite m_sprite;
	size_t m_frameCount = 1; size_t m_index;
	size_t m_currentFrame = 0;
	size_t m_speed = 0;
	Vec2f m_size = { 1.0f, 1.0f };
	std::string m_name = "none";

public:
	Animation() = default;
	Animation(std::string name, size_t index, const sf::Texture& t, size_t frameCount, size_t speed)
		: m_name(std::move(name)), m_sprite(t), m_frameCount(frameCount), m_speed(speed), m_index(index)
	{
		m_size = Vec2f(static_cast<float>(t.getSize().y), static_cast<float>(t.getSize().y));
		m_sprite.setOrigin(m_size.y / 2.0f, m_size.y / 2.0f);
		m_sprite.setTextureRect(sf::IntRect(std::floor(m_index * m_size.x) * m_size.x, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
	}
	~Animation() = default;

	void update() {
		m_currentFrame++;

		if (!(m_currentFrame % m_speed)) {
			int next_image = m_sprite.getTextureRect().left + static_cast<int>(m_size.x);

			if (next_image >= static_cast<int>(m_frameCount * m_size.x)) {

				m_currentFrame = 0;
				m_sprite.setTextureRect(sf::IntRect(std::floor(m_index * m_size.x) * m_size.x, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));

			}
			else {
				m_sprite.setTextureRect(sf::IntRect(m_sprite.getTextureRect().left + static_cast<int>(m_size.x), 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
			}
		}
	}

	bool hasEnded() const {
		return (m_sprite.getTextureRect().left / m_size.x) >= m_frameCount;
	}
	const std::string& getName() const { return m_name; }
	const Vec2f& getSize() const { return m_size; }
	sf::Sprite& getSprite() { return m_sprite; }
};