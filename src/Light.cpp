//
// Created by Alex on 20/09/2024.
//

#include <Raytracer/Light.h>
#include <Raytracer/Ray.h>
#include <Raytracer/Shape.h>
#include <Raytracer/Common.h>

namespace rtc
{

	glm::vec3 ComputeLight(const Intersection& it, std::span<Shape> shapes, const Light& light)
	{
		Ray ray { light.position, it.point - light.position };
		IntersectionOpt lightToPoint = it.shape.get().Intersect(ray);
		bool apply = true;
		if (lightToPoint.has_value())
		{
			for (auto& shape: shapes)
			{
				auto lightToShape = shape.Intersect(ray);
				if (lightToShape.has_value())
				{
					if (lightToPoint->distance > lightToShape->distance)
					{
						apply = false;
						break;
					}
				}
			}
		}
		if (apply)
		{
			// L_o = L_e + V(P, L_p) * L_emit / D ^ 2 * Albedo * | N . L_i |
			auto vplp   = AlmostEqual(it.point, lightToPoint->point, .1f) ? 1.f : 0.f;
			auto d2     = lightToPoint->distance * lightToPoint->distance;
			auto lemit  = light.quantity;
			auto li     = glm::normalize(it.point - light.position);

			auto lo = it.material.get().emission + vplp * (lemit / d2) * it.material.get().albedo * glm::abs(glm::dot(it.normal, li));

			return lo;
		}

		return glm::vec3 { 0.f };
	}

}