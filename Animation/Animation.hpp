#pragma once

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>

#include "../Vector/Vec2.hpp"

class Animation {
protected:
	sf::Sprite m_sprite{};
	size_t m_end = 0; size_t m_begin = 0; size_t m_pointer = 0;
	size_t m_speed = 6;
	size_t m_currentFrame = m_speed;
	Vec2f m_size = { 1.0f, 1.0f };
	std::string m_name = "none";
public:
	Animation() = default;
	Animation(const sf::Texture& t, const std::string& name, const size_t& begin, const size_t& end)
		: m_name(name), m_sprite(t), m_begin(begin), m_end(end) {};

	virtual ~Animation() = default;
	virtual void update() = 0;
	const bool& hasEnded() const { return m_pointer >= m_end;}
	const std::string& getName() const { return m_name; }
	const Vec2f& getSize() const { return m_size; }
	sf::Sprite& getSprite() { return m_sprite; }
};

class PlayerAnimation : public Animation {
	
public:
	PlayerAnimation() = default;
	PlayerAnimation(const sf::Texture& t, const std::string& name, const size_t& begin, const size_t& end)
		: Animation(t, name, begin, end)
	{
		m_pointer = m_begin;
		m_size = Vec2f(69.f, 44.f);
		m_sprite.setOrigin(m_size.x / 2.0f, m_size.y / 2.0f);
		update();
	}
	~PlayerAnimation() = default;

	void update () override {
		m_currentFrame++;
		if (m_currentFrame >= m_speed) {
			m_currentFrame = 0;
			if (m_pointer >= m_end) m_pointer = m_begin;
			size_t row{}, col{};
			col = m_pointer % 6;
			row = m_pointer / 6;
			m_sprite.setTextureRect(sf::IntRect(static_cast<int>(col * m_size.x), static_cast<int>(row * m_size.y), static_cast<int>(m_size.x), static_cast<int>(m_size.y)));
			m_pointer++;
		}
	}
};