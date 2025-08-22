#pragma once

#include <SFML/Graphics.hpp>
#include <math.h>
#include <cassert>

template<typename T>
class Vec2
{
public:
	T x = 0;
	T y = 0;

	Vec2() = default;
	Vec2(T xin, T yin) : x(xin), y(yin) {}

	Vec2(const T& angle_r) { x = cosf(angle_r); y = sinf(angle_r); }

	Vec2(const sf::Vector2<T>& vec) : x(vec.x), y(vec.y) {}

	operator sf::Vector2<T>() { return sf::Vector2<T>(x, y); }

	Vec2(const Vec2& arg) : x(arg.x), y(arg.y) {}

	Vec2 operator + (const Vec2& rhs) const { return Vec2(x + rhs.x, y + rhs.y); }
	Vec2 operator - (const Vec2& rhs) const { return Vec2(x - rhs.x, y - rhs.y); }
	Vec2 operator / (const Vec2& rhs) const {
		assert(rhs.x != 0 && rhs.y != 0);
		return Vec2(x / rhs.x, y / rhs.y);
	}
	Vec2 operator * (const Vec2& rhs) const { return Vec2(x * rhs.x, y * rhs.y); }
	Vec2& operator += (const Vec2& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Vec2& operator -= (const Vec2& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	Vec2& operator /= (const Vec2& rhs) {
		assert(rhs.x != 0 && rhs.y != 0);
		x /= rhs.x;
		y /= rhs.y;
		return *this;
	}
	Vec2& operator *= (const Vec2& rhs) {
		x *= rhs.x;
		y *= rhs.y;
		return *this;
	}

	Vec2 operator*(const T& scalar) const {
		return Vec2(x * scalar, y * scalar);
	}

	Vec2& operator*=(const T& scalar) const {
		x *= scalar;
		y *= scalar;

		return *this;
	}


	float dist(const Vec2& rhs) const {
		T dx = x - rhs.x;
		T dy = y - rhs.y;
		return sqrtf(dx * dx + dy * dy);
	}

	float magnitude() const {
		return sqrtf(x * x + y * y);
	}

	Vec2 normalized() const {
		float len = magnitude();
		if (len != 0) {
			return Vec2(x / len, y / len);
		}
		return *this;
	}

	Vec2 normalized_d(const T& dist) {
		if (dist != 0) {
			return Vec2(x / dist, y / dist);
		}
		return *this;
	}

};

template<typename T>
Vec2<T> abs(const Vec2<T>& v) {
	return Vec2<T>(std::abs(v.x), std::abs(v.y));
}

template<typename T>
Vec2<T> operator*(const T& lhs, const Vec2<T>& rhs) {
	return rhs * lhs;
}

using Vec2f = Vec2<float>;