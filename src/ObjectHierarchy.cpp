//
// Created by Alex on 25/09/2024.
//

#include <Raytracer/ObjectHierarchy.h>

namespace rtc
{

	template<class... Ts>
	struct overloaded : Ts... { using Ts::operator()...; };

	template<class... Ts>
	overloaded(Ts...) -> overloaded<Ts...>;

	ObjectIntersectionOpt Intersect(const Ray& ray, const ObjectHierarchy& oh)
	{
		ObjectIntersectionOpt ret = std::nullopt;

		std::visit(overloaded {
			[&](const Leaf& leaf) {
				if (auto it = Intersect(ray, leaf.aabb); it.has_value())
				{
					for (auto& object: leaf.spheres)
					{
						if (auto it2 = IntersectObject(ray, object); it2.has_value())
						{
							if (ret.has_value())
							{
								if (it2->distance < ret->distance)
									ret.emplace(*it2);
							}
							else
							{
								ret.emplace(*it2);
							}
						}
					}
				}
		    },
		    [&](const Node& node) {
			   if (auto it = Intersect(ray, node.aabb); it.has_value())
			   {
				   assert(node.left != nullptr && node.right != nullptr);
				   if (auto leftIt = Intersect(ray, *node.left); leftIt.has_value())
				   {
						ret.emplace(*leftIt);
				   }
				   else
				   {
					   if (auto rightIt = Intersect(ray, *node.right); rightIt.has_value())
						   ret.emplace(*rightIt);
				   }
			   }
		    }
	    }, oh);

		return ret;
	}

	void BuildObjectHierarchy(ObjectHierarchy& oh)
	{}

	void AddObject(ObjectHierarchy& oh, Sphere&& sphere)
	{
		std::visit(overloaded {
			[&](Leaf& leaf)
			{
				leaf.spheres.push_back(sphere);
				AABB sAabb { sphere.center - sphere.radius, sphere.center + sphere.radius };
				Merge(leaf.aabb, sAabb);
			},
			[](const Node&)
			{
			   assert(false);
			}
		}, oh);
	}

}