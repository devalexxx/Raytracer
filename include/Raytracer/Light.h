//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_LIGHT_H
#define RAYTRACER_LIGHT_H

#include <glm/glm.hpp>

#include <vector>

namespace rtc
{

	struct ObjectIntersection;
	struct Scene;

	struct Light
	{
			glm::vec3 position;
			glm::vec3 emission;
	};

	glm::vec3 ComputeLight(const ObjectIntersection& it, const Scene& scene);

}

#endif
