//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include <glm/glm.hpp>

#include <optional>

namespace rtc
{

	struct Material;
	struct Shape;

	struct Ray
	{
			glm::vec3 origin;
			glm::vec3 direction;
	};

	struct Intersection
	{
			glm::vec3                              point;
			float                                  distance;
			glm::vec3                              normal;
			std::reference_wrapper<const Material> material;
			std::reference_wrapper<const Shape>    shape;
	};

	using IntersectionOpt = std::optional<Intersection>;

}

#endif
