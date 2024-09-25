//
// Created by Alex on 20/09/2024.
//

#include <Raytracer/Light.h>
#include <Raytracer/Scene.h>
#include <Raytracer/Ray.h>

namespace rtc
{

	glm::vec3 ComputeLight(const ObjectIntersection& it, const Scene& scene)
	{
		glm::vec3 color = it.material.emission;
		for (auto& light: scene.lights)
		{
			auto toLight  = light.position - it.point;
			auto distance = glm::length(toLight);
			auto cos      = glm::clamp(glm::dot(glm::normalize(toLight), it.normal), 0.f, 1.f);

			auto v = cos / distance * it.material.albedo * light.emission;

			auto shadowDirection = glm::normalize(toLight);
			auto originWithDelta = it.point + 0.1f * shadowDirection;
			Ray shadowRay { originWithDelta, shadowDirection};

			float visibility = 1.f;
			if (auto it2 = Intersect(shadowRay, scene.objects); it2.has_value())
			{
				if (it2->distance < distance)
					visibility = 0.f;
			}

			color += visibility * v;
		}

		return color;
	}

}