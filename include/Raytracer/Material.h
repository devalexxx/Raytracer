//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include <glm/glm.hpp>

namespace rtc
{

	struct Material
	{
			glm::vec3 albedo;
			glm::vec3 emission;
	};

}

#endif
