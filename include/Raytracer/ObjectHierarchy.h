//
// Created by Alex on 24/09/2024.
//

#ifndef RAYTRACER_OBJECTHIERARCHY_H
#define RAYTRACER_OBJECTHIERARCHY_H

#include <Raytracer/Object.h>

#include <memory>
#include <variant>

namespace rtc
{

	struct Leaf;
	struct Node;

	using ObjectHierarchy = std::variant<Leaf, Node>;

	using flim = std::numeric_limits<float>;

	struct Leaf
	{
			AABB                aabb { glm::vec3(flim::max()), glm::vec3(flim::lowest()) };
			std::vector<Sphere> spheres;
	};

	struct Node
	{
			AABB                             aabb { glm::vec3(flim::max()), glm::vec3(flim::lowest()) };
			std::unique_ptr<ObjectHierarchy> left;
			std::unique_ptr<ObjectHierarchy> right;
	};

	ObjectIntersectionOpt Intersect(const Ray& ray, const ObjectHierarchy& oh);

	void BuildObjectHierarchy(ObjectHierarchy& oh);
	void AddObject(Leaf& oh, Sphere&& sphere);
	void AddObject(ObjectHierarchy& oh, Sphere&& sphere);
}

#endif
