#pragma once

#include "../Vector/Vec2.hpp"
#include "../Animation/Animation.hpp"


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
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;

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
public:
	Animation animation;
	bool repeat = false;
	bool reverse = false;
	CAnimation() = default;
	CAnimation(const Animation& animation, bool r) : animation(animation), repeat(r) {}
};

class CGravity : public Component {
public:
	float gravity = 0;
	CGravity() = default;
	CGravity(float g) : gravity(g) {}
};

class CState : public Component {
public:
	std::string state = "default";
	CState() = default;
	CState(const std::string& s) : state(s) {}
};

class CShape : public Component
{
public:
	sf::RectangleShape rect;

	CShape() = default;

	CShape(float width, float height, const sf::Color& fill, const sf::Color& outline, float thickness)
	{
		rect.setSize(sf::Vector2f(width, height));
		rect.setFillColor(fill);
		rect.setOutlineColor(outline);
		rect.setOutlineThickness(thickness);
		rect.setOrigin(width / 2.0f, height / 2.0f);
	}
};