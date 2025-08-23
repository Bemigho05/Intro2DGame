#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

#include "../Vector/Vec2.hpp"

class Animation {
	sf::Sprite m_sprite{};
	size_t m_frameCount = 1; size_t m_index = 0;
	size_t m_currentFrame = 0;
	size_t m_speed = 0;
	Vec2f m_size = { 1.0f, 1.0f };
	bool m_reverse = false;
	
	std::string m_name = "none";

public:
	Animation() = default;
	Animation(const std::string& name, const size_t& index, const sf::Texture& t, const size_t& frameCount, const size_t& speed, const bool& reverse)
		: m_name(name), m_sprite(t), m_frameCount(frameCount), m_speed(speed), m_index(index), m_reverse(reverse)
	{
		m_size = Vec2f(static_cast<float>(t.getSize().y), static_cast<float>(t.getSize().y));
		m_sprite.setOrigin(m_size.y / 2.0f, m_size.y / 2.0f);
		if (!m_reverse)
			m_sprite.setTextureRect(sf::IntRect(m_index * m_size.x, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
		else 
			m_sprite.setTextureRect(sf::IntRect((m_index + m_frameCount) * m_size.x, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
	}
	~Animation() = default;

	void update() {
		if (++m_currentFrame >= m_speed) {
			m_currentFrame = 0;
			if (!m_reverse) {
				int next_image = m_sprite.getTextureRect().left + static_cast<int>(m_size.x);

				if (next_image >= static_cast<int>((m_index + m_frameCount) * m_size.x)) {
					m_sprite.setTextureRect(sf::IntRect(m_index * m_size.x, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
				}
				else {
					m_sprite.setTextureRect(sf::IntRect(next_image, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
				}
			}
			else {
				int next_image = m_sprite.getTextureRect().left - static_cast<int>(m_size.x);
				if (next_image <= static_cast<int>(m_index * m_size.x)) {
					m_sprite.setTextureRect(sf::IntRect((m_index + m_frameCount) * m_size.x, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
				}
				else {
					m_sprite.setTextureRect(sf::IntRect(next_image, 0, static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
					
				}
			}
			
		}
	}

	bool hasEnded() const {
		if (!m_reverse)
			return (m_sprite.getTextureRect().left + static_cast<int>(m_size.x)) >= static_cast<int>((m_index + m_frameCount) * m_size.x);
		else
			return (m_sprite.getTextureRect().left - static_cast<int>(m_size.x)) <= static_cast<int>((m_index) * m_size.x);
	}
	const std::string& getName() const { return m_name; }
	const Vec2f& getSize() const { return m_size; }
	sf::Sprite& getSprite() { return m_sprite; }
};