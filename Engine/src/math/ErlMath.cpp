#include"ErlMath.h"

int ErlMath::generate_random(int minValue, int maxValue)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(minValue, maxValue);

	return dis(gen);
}