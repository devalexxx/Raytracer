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

	struct Leaf
	{
			AABB                aabb;
			std::vector<Sphere> spheres;
	};

	struct Node
	{
			AABB                             aabb;
			std::unique_ptr<ObjectHierarchy> left;
			std::unique_ptr<ObjectHierarchy> right;
	};

	ObjectIntersectionOpt Intersect(const Ray& ray, const ObjectHierarchy& oh);

	void BuildObjectHierarchy(ObjectHierarchy& oh);
	void AddObject(ObjectHierarchy& oh, Sphere&& sphere);
}

#endif
