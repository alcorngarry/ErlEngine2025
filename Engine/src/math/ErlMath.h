#ifndef ERL_MATH_H
#define ERL_MATH_H
#include<random>
#include<glm/glm.hpp>
#include<iostream>

namespace ErlMath {
	int generate_random(int minValue, int maxValue);
	void print_vector3(glm::vec3 vec);
}
#endif // !ERL_MATH_H