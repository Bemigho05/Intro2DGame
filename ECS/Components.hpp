#pragma once

#include "../Vector/Vec2.hpp"
#include "../Animation/Animation.hpp"
#include <memory>


class Component {
public:
	bool exists = false;
};

class CTransform : public Component {
public:
	Vec2f pos = { 0.0, 0.0 };
	Vec2f prevPos = { 0.0, 0.0 };
	Vec2f velocity = { 0.0, 0.0 };
	Vec2f scale = { 1.0, 1.0f };
	float angle = 0;

	CTransform() = default;
	CTransform(const Vec2f& p) : pos(p) {}
	CTransform(const Vec2f& p, const Vec2f& sp, const Vec2f& sc, float a)
		: pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {
	}
};


class CLifespan : public Component {
public:
	int lifespan = 0;
	int frameCreated = 0;
	CLifespan() = default;
	CLifespan(int duration, int frame) : lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component
{
public:
	bool up = false;
	bool left = false;
	bool right = false;
	bool attack = false;
	bool dash = false;
	bool heavyAttack = false;

	bool attacking = false;
	bool dashing = false;
	bool heavyAttacking = false;
	
	CInput() = default;
};

class CBoundingBox : public Component {
public:
	Vec2f size = { 0.0, 0.0 };
	Vec2f halfSize = { 0.0, 0.0 };
	CBoundingBox() = default;
	CBoundingBox(const Vec2f& s) : size(s), halfSize(s.x / 2, s.y / 2) {}
};

class CAnimation : public Component {
	std::unique_ptr<Animation> animation = nullptr;
public:
	bool mustFinish = false;
	bool repeat = false;
	CAnimation() = default;
	~CAnimation() = default;

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Animation, T>>>
	CAnimation(const T& anim) : animation(std::make_unique<T>(anim)) {}

	template<typename T, typename = std::enable_if_t<std::is_base_of_v<Animation, T>>>
	CAnimation& operator=(const T& anim) {
		animation = std::make_unique<T>(anim);
		return *this;
	}

	CAnimation(const CAnimation&) = delete;
	CAnimation& operator=(const CAnimation&) = delete;

	CAnimation(CAnimation&& other) noexcept
		: animation(std::move(other.animation)), mustFinish(other.mustFinish), repeat(other.repeat) {
	}

	CAnimation& operator=(CAnimation&& other) noexcept {
		if (this != &other) {
			animation = std::move(other.animation);
			mustFinish = other.mustFinish;
			repeat = other.repeat;
		}
		return *this;
	}

	void update() const { if (animation) animation->update(); }
	const bool hasEnded() const { return animation ? animation->hasEnded() : true; }
	const std::string& getName() const {
		static std::string empty = "none";
		return animation ? animation->getName() : empty;
	}
	const Vec2f& getSize() const {
		static Vec2f zero = { 0.0f, 0.0f };
		return animation ? animation->getSize() : zero;
	}
	sf::Sprite& getSprite() const {
		static sf::Sprite dummy;
		return animation ? animation->getSprite() : dummy;
	}
};


class CGravity : public Component {
public:
	float gravity = 0;
	bool canJump = false;
	CGravity() = default;
};

class CShape : public Component
{
public:
	sf::RectangleShape rect{};

	CShape() = default;

	CShape(float width, float height, const sf::Color& fill, const sf::Color& outline, float thickness)
	{
		rect.setSize(sf::Vector2f(width, height));
		rect.setFillColor(fill);
		rect.setOutlineColor(outline);
		rect.setOutlineThickness(thickness);
		rect.setOrigin(width / 2.0f, height / 2.0f);
	}
	void setPosition(const Vec2f& pos) {
		rect.setPosition(pos.x, pos.y);
	}

	Vec2f getPosition() const {
		return Vec2f(rect.getPosition().x, rect.getPosition().y);
	}
};