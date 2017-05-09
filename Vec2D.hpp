#pragma once

class Vec2D
{
public:
	Vec2D();
	Vec2D operator+(const Vec2D & other);
	Vec2D operator*(double d);
	void operator =(const Vec2D & other);
};