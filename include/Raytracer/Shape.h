//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_SHAPE_H
#define RAYTRACER_SHAPE_H

#include <Raytracer/Material.h>
#include <Raytracer/Ray.h>

#include <glm/glm.hpp>

#include <functional>

namespace rtc
{

	struct Sphere
	{
			glm::vec3 center;
			float     radius;
			Material  material;
	};
	IntersectionOpt Intersect(const Ray& ray, const Sphere& sphere);

}

#include <Raytracer/Shape.inl>

#endif
