#include"ErlMath.h"

int ErlMath::generate_random(int minValue, int maxValue)
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(minValue, maxValue);

	return dis(gen);
}

void ErlMath::print_vector3(glm::vec3 vec)
{
	std::cout << vec.x << ", " << vec.y << ", " << vec.z << std::endl;
}