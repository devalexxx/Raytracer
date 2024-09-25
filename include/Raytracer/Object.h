//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_OBJECT_H
#define RAYTRACER_OBJECT_H

#include <Raytracer/Material.h>
#include <Raytracer/Ray.h>

#include <glm/glm.hpp>

#include <functional>
#include <variant>

namespace rtc
{

	struct Sphere
	{
			glm::vec3       center;
			float           radius;
			const Material& material;
	};

	struct AABB
	{
			glm::vec3 min, max;
	};

	IntersectionOpt Intersect(const Ray& ray, const Sphere& sphere);
	IntersectionOpt Intersect(const Ray& ray, const AABB& aabb);

	ObjectIntersectionOpt IntersectObject(const Ray& ray, const Sphere& sphere);

	void Merge(AABB& src, const AABB& other);

}

#endif
