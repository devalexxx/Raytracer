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

    struct Shape {
        using IntersectFunc = std::function<IntersectionOpt(const Ray&)>;

        Shape() = delete;
		template<typename T>
		Shape(T shape);

        IntersectFunc Intersect;
    };

//	inline bool operator==(const Shape& lhs, const Shape& rhs);
//	inline bool operator!=(const Shape& lhs, const Shape& rhs);

    struct Sphere
    {
        glm::vec3 center;
        float     radius;
		Material  material;
    };
	IntersectionOpt Intersect(const Ray& ray, const Shape& shape, const Sphere& sphere);

    struct Surface
    {
		glm::vec3 center;
		glm::vec3 normal;
		Material material;
    };
    IntersectionOpt Intersect(const Ray& ray, const Shape& shape, const Surface& surface);

}

#include <Raytracer/Shape.inl>

#endif
