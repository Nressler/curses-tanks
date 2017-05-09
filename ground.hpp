#pragma once
#include <vector>

class Ground
{
public:
	void InitializeGround();
	void Draw();
	bool ground_break;

	std::vector<int> ground;
};
