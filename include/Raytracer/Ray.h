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

	struct Intersection
	{
			float distance;
	};
	using IntersectionOpt = std::optional<Intersection>;

	struct ObjectIntersection : public Intersection
	{
			glm::vec3 point;
			glm::vec3 normal;

			const Material& material;
	};
	using ObjectIntersectionOpt = std::optional<ObjectIntersection>;

	struct Ray
	{
			glm::vec3 origin;
			glm::vec3 direction;
	};

}

#endif
