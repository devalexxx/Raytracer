//
// Created by Alex on 19/09/2024.
//

#include <Raytracer/Object.h>

namespace rtc
{

	IntersectionOpt Intersect(const Ray& ray, const Sphere& sphere)
	{
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
				return {{ t1 }};
			}
			else if (t2 >= 0)
			{
				return {{ t2 }};
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

	IntersectionOpt Intersect(const Ray& ray, const AABB& aabb)
	{
		auto rInv = 1.f / ray.direction;

		auto t1 = (aabb.min - ray.origin) * rInv;
		auto t2 = (aabb.max - ray.origin) * rInv;

		// x
		auto tmin = std::min(t1.x, t2.x);
		auto tmax = std::max(t1.x, t2.x);

		// y
		auto tminp = std::max(tmin, std::min(t1.y, t2.y));
		auto tmaxp = std::min(tmax, std::max(t1.y, t2.y));

		// z
		auto tminpp = std::max(tminp, std::min(t1.z, t2.z));
		auto tmaxpp = std::min(tmaxp, std::max(t1.z, t2.z));

		if (tmaxpp < tminpp)
			return std::nullopt;
		else
			return {{ tminpp }};
	}

	ObjectIntersectionOpt IntersectObject(const Ray& ray, const Sphere& sphere)
	{
		if (IntersectionOpt it = Intersect(ray, sphere); it.has_value())
		{
			auto point = glm::normalize(ray.direction) * it->distance + ray.origin;
			return {{ { it->distance }, point, glm::normalize(point - sphere.center), sphere.material }};
		}
		else
			return std::nullopt;
	}

	void Merge(AABB& src, const AABB& other)
	{
		src.min.x = std::min(src.min.x, other.min.x);
		src.min.y = std::min(src.min.y, other.min.y);
		src.min.z = std::min(src.min.z, other.min.z);
		src.max.x = std::max(src.max.x, other.max.x);
		src.max.y = std::max(src.max.y, other.max.y);
		src.max.z = std::max(src.max.z, other.max.z);
	}

}