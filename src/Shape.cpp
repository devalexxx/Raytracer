//
// Created by Alex on 19/09/2024.
//

#include <Raytracer/Shape.h>

namespace rtc
{

	IntersectionOpt Intersect(const Ray& ray, const Shape& shape, const Sphere& sphere)
	{
		auto GenerateIntersection = [&](float t) -> Intersection {
			auto point = glm::normalize(ray.direction) * t + ray.origin;
			return { point, t, glm::normalize(point - sphere.center), sphere.material, shape };
        };

		const auto oc = ray.origin - sphere.center;

		const auto b = glm::dot(oc, glm::normalize(ray.direction));
		const auto c = glm::dot(oc, oc) - sphere.radius * sphere.radius;

		const auto delta = b * b - c;

		if (delta >= 0.f)
		{
			const auto sD = sqrt(delta);
			const auto t1 = -b - sD;
			const auto t2 = -b + sD;

			if (t1 >= 0)
			{
				return GenerateIntersection(t1);
			}
			else if (t2 >= 0)
			{
				return GenerateIntersection(t2);
			}
			else
			{
				return std::nullopt;
			}
		}
		else
		{
			return std::nullopt;
		}
	}

	IntersectionOpt Intersect(const Ray& ray, const Shape& shape, const Surface& surface)
	{
		float denon = glm::dot(surface.normal, glm::normalize(ray.direction));
		if (denon > 1e-6)
		{
			float t = glm::dot(surface.center - ray.origin, surface.normal) / denon;
			if (t >= 0)
			{
				auto point = glm::normalize(ray.direction) * t + ray.origin;
				return Intersection { point, t, surface.normal, surface.material, shape };
			}
			else
			{
				return std::nullopt;
			}
		}
		return std::nullopt;
	}
}