#pragma once
#include <cmath>

struct Vec2 {
	float x = 0.0f;
	float y = 0.0f;


	Vec2() = default;
	Vec2(float x, float y) : x(x), y(y) {}


	Vec2 operator+(const Vec2& other) const { return { x + other.x, y + other.y }; }
	Vec2 operator-(const Vec2& other) const { return { x - other.x, y - other.y }; }
	Vec2 operator*(float scalar) const { return {x * scalar, y * scalar}; }
	Vec2 operator/(float scalar) const { return {x / scalar, y / scalar}; }

	Vec2& operator+=(const Vec2& other) { x += other.x, y += other.y; return *this; }
	Vec2& operator-=(const Vec2& other) { x -= other.x, y -= other.y; return *this; }
	Vec2& operator*=(float scalar) { x *= scalar; y *= scalar; return *this; }

	float lengthSquared() const { return x * x + y * y; }
	float length() const { return std::sqrt(lengthSquared()); }


	Vec2 normalized() const {
		float len = length();
		if (len > 0.0f) return { x / len, y / len };
		return { 0.0f, 0.0f };
	}

};