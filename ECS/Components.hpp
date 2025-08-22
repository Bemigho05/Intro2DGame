#pragma once

#include "../Vector/Vec2.hpp"
#include "Animation.hpp"



#include <utility>



class Component {
public:
	bool exists = false;
};

class CTransform : public Component {
public:
	Vec2f pos = { 0.0, 0.0 };
	Vec2f prevPos = { 0.0, 0.0 };
	Vec2f velocity = { 0.0, 0.0 };
	Vec2f scale = { 1.0, 1.0 };
	float angle = 0;

	CTransform() = default;

	explicit CTransform(const Vec2f& p) : pos(p) {}

	CTransform(const Vec2f& p, const Vec2f& speed, const Vec2f& s, float a)
		: pos(p), prevPos(p), velocity(speed), scale(s), angle(a) {
	}
};

class CLifespan : public Component {
public:
	int lifespan = 0;
	int frameCreated = 0;

	CLifespan() = default;

	explicit CLifespan(int duration, int frame)
		: lifespan(duration), frameCreated(frame) {
	}
};

class CInput : public Component {
public:
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool shoot = false;
	bool canShoot = true;
	bool canJump = true;

	CInput() = default;
};

class CBoundingBox : public Component {
public:
	Vec2f size;
	Vec2f halfSize;

	CBoundingBox() = default;

	explicit CBoundingBox(const Vec2f& s) : size(s), halfSize(s.x / 2.0f, s.y / 2.0f) {}
};

class CAnimation : public Component {
public:
	Animation animation;
	bool repeat = false;

	CAnimation() = default;

	CAnimation(Animation a, bool r) : animation(std::move(a)), repeat(r) {}
};

class CGravity : public Component {
public:
	float gravity = 0;

	CGravity() = default;

	explicit CGravity(float g) : gravity(g) {}
};

class CState : public Component {
public:
	std::string state = "jumping";
	std::string previousState = "jumping";
	bool changeAnimation = false;

	CState() = default;

	explicit CState(std::string s) : state(std::move(s)) {}
};
