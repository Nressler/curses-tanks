#pragma once
#include "ground.hpp"

enum Side
{
	LEFT,
	RIGHT
};

class Player
{
public:
	Player();
	void Draw(Ground & ground);
	void Initialize(int column, Side side);
	void DrawSettings(int turn);
	void PowerUp();
	void PowerDown();
	void AngleUp();
	void AngleDown();
	int col;
	int line;
	double power;
	double angle;
	int health;
	bool Win_check;
	Side s;

	static const int power_increment;
	static const double angle_increment;
};
