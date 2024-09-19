//
// Created by Alex on 20/09/2024.
//

#include <Raytracer/Common.h>

namespace rtc
{

	bool AlmostEqual(float lhs, float rhs, float epsilon)
	{
		return std::fabs(lhs - rhs) <= epsilon;
	}

	bool AlmostEqual(glm::vec3 lhs, glm::vec3 rhs, float epsilon)
	{
		return AlmostEqual(lhs.x, rhs.x, epsilon) && AlmostEqual(lhs.y, rhs.y, epsilon) && AlmostEqual(lhs.z, rhs.z, epsilon);
	}

}