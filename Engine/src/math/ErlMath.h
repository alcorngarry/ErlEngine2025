#ifndef ERL_MATH_H
#define ERL_MATH_H
#include<random>
#include<glm/glm.hpp>
#include<iostream>
#include<string>

namespace ErlMath {
	int generate_random(int minValue, int maxValue);
	void print_vector3(glm::vec3 vec);
	std::string vec3_to_string(glm::vec3 vec);
}
#endif // !ERL_MATH_H