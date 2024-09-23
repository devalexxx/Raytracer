//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include <glm/glm.hpp>

#include <span>

namespace rtc
{

	struct Intersection;
	struct Sphere;

	struct Light
	{
			glm::vec3 position;
			glm::vec3 quantity;
	};

	glm::vec3 ComputeLight(const Intersection& it, const std::vector<Sphere>& spheres, const Light& light);

}

#endif
