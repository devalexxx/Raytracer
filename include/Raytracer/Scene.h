//
// Created by Alex on 19/09/2024.
//

#ifndef RAYTRACER_SCENE_H
#define RAYTRACER_SCENE_H

#include <Raytracer/Light.h>
#include <Raytracer/ObjectHierarchy.h>

#include <vector>

namespace rtc
{

	struct Camera
	{
			glm::vec3 position;
	};

	struct Scene
	{
			Camera    		   camera;
			std::vector<Light> lights;
			ObjectHierarchy    objects;
	};

}

#endif
