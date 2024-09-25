//
// Created by Alex on 25/09/2024.
//

#include <Raytracer/ObjectHierarchy.h>

#include <algorithm>

#include <fmt/base.h>

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
				if (Intersect(ray, leaf.aabb).has_value())
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
			   if (Intersect(ray, node.aabb).has_value())
			   {
				   assert(node.left != nullptr && node.right != nullptr);
				   auto leftIt  = Intersect(ray, *node.left);
				   auto rightIt = Intersect(ray, *node.right);
				   if (leftIt.has_value())
				   {
					   if (rightIt.has_value())
					   {
						   if (leftIt->distance < rightIt->distance)
							   ret.emplace(*leftIt);
						   else
							   ret.emplace(*rightIt);
					   }
					   else
					   {
						   ret.emplace(*leftIt);
					   }
				   }
				   else
				   {
					   if (rightIt.has_value())
						   ret.emplace(*rightIt);
				   }
			   }
		    }
	    }, oh);

		return ret;
	}

	void BuildObjectHierarchy(ObjectHierarchy& oh)
	{
		std::visit(overloaded {
		   [&](Leaf& leaf) {
//			   fmt::print("{}\n", leaf.spheres.size());
			   if (leaf.spheres.size() > 10)
			   {
				   Node node;
				   node.aabb = leaf.aabb;

				   switch (LongestAxis(node.aabb))
				   {
					   case Axis::X:
						   std::sort(leaf.spheres.begin(), leaf.spheres.end(), [](const Sphere& lhs, const Sphere& rhs) {
							   return lhs.center.x > rhs.center.x;
						   });
						   break;
					   case Axis::Y:
						   std::sort(leaf.spheres.begin(), leaf.spheres.end(), [](const Sphere& lhs, const Sphere& rhs) {
							   return lhs.center.y < rhs.center.y;
						   });
						   break;
					   case Axis::Z:
						   std::sort(leaf.spheres.begin(), leaf.spheres.end(), [](const Sphere& lhs, const Sphere& rhs) {
							   return lhs.center.z < rhs.center.z;
						   });
						   break;
				   }

				   Leaf left, right;
				   for (size_t i = 0; i < leaf.spheres.size() / 2; ++i)
				   {
					   AddObject(left, std::move(leaf.spheres[i]));
				   }
				   for (size_t i = leaf.spheres.size() / 2; i < leaf.spheres.size(); ++i)
				   {
					   AddObject(right, std::move(leaf.spheres[i]));
				   }

				   node.left  = std::make_unique<ObjectHierarchy>(std::move(left));
				   node.right = std::make_unique<ObjectHierarchy>(std::move(right));

				   BuildObjectHierarchy(*node.left);
				   BuildObjectHierarchy(*node.right);

				   oh.emplace<Node>(std::move(node));
			   }
		   },
		   [](const Node&) {
			   assert(false);
		   }
	   }, oh);
	}

	void AddObject(Leaf& leaf, Sphere&& sphere)
	{
		leaf.spheres.push_back(sphere);
		AABB sAabb { sphere.center - sphere.radius, sphere.center + sphere.radius };
		Merge(leaf.aabb, sAabb);
	}

	void AddObject(ObjectHierarchy& oh, Sphere&& sphere)
	{
		std::visit(overloaded {
			[&](Leaf& leaf)
			{
			   AddObject(leaf, std::move(sphere));
			},
			[](const Node&)
			{
			   assert(false);
			}
		}, oh);
	}

}