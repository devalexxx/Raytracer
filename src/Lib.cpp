//
// Created by Alex on 17/09/2024.
//

#include <Raytracer/Lib.h>

#include <fmt/base.h>

namespace rtc
{


    bool AlmostEqual(float lhs, float rhs, float epsilon)
    {
        return std::fabs(lhs - rhs) <= epsilon;
    }

    bool AlmostEqual(glm::vec3 lhs, glm::vec3 rhs, float epsilon)
    {
        return AlmostEqual(lhs.x, rhs.x, epsilon) && AlmostEqual(lhs.y, rhs.y, epsilon) && AlmostEqual(lhs.z, rhs.z, epsilon);
    }

    std::optional<Intersection> Intersect(const Ray& ray, const Sphere& sphere)
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
                return Intersection { glm::normalize(ray.direction) * t1 + ray.origin, t1 };
            }
            else if (t2 >= 0)
            {
                return Intersection { glm::normalize(ray.direction) * t2 + ray.origin, t2 };
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

    glm::vec3 ComputeLight(const Intersection& it, const Sphere& sphere, const Light& light)
    {
        Ray ray { light.position, it.point - light.position };
        if (std::optional<Intersection> it2 = Intersect(ray, sphere); it2.has_value())
        {
            // L_o = L_e + V(P, L_p) * L_emit / D ^ 2 * Albedo * | N . L_i |
            auto le     = sphere.material.emission;
            auto vplp   = AlmostEqual(it.point, it2->point, .1f) ? 1.f : 0.f;
            auto d2     = it2->distance * it2->distance;
            auto albedo = sphere.material.albedo;
            auto lemit  = light.quantity;
            auto normal = glm::normalize(it.point - sphere.center);
            auto li     = glm::normalize(it.point - light.position);

            auto lo = le + vplp * (lemit / d2) * albedo * glm::abs(glm::dot(normal, li));

            return lo;
        }

        return glm::vec3 { 0.f };
    }

}