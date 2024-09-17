//
// Created by Alex on 17/09/2024.
//

#include <doctest/doctest.h>

#include <Raytracer/Lib.h>

TEST_CASE("Light")
{
    using namespace rtc;

    glm::vec3 origin { 0.f, 500.f, 200.f };
    Sphere sphere    {{ 0.f, 0.f, 200.f }, 100.f, {{ 1.f, 1.f, 1.f }}};
    Ray    ray       { origin,  sphere.center - origin };
    Light  light     {{ 0.f, 200.f, 200.f }, { 10000.f, 10000.f, 10000.f }};

    auto it = Intersect(ray, sphere);

    REQUIRE(it.has_value());
    CHECK_EQ(it->point, sphere.center + glm::vec3 { 0.f, sphere.radius, 0.f });
    CHECK_EQ(it->distance, glm::distance(it->point, origin));

    ray.origin    = light.position;
    ray.direction = it->point - light.position;

    it = Intersect(ray, sphere);
    REQUIRE(it.has_value());
    CHECK_EQ(it->point, sphere.center + glm::vec3 { 0.f, sphere.radius, 0.f });
    CHECK_EQ(it->distance, glm::distance(it->point, light.position));

    auto normal = glm::normalize(it->point - sphere.center);
    fmt::print("normal {}\n", normal);
    fmt::print("dotprod {}\n", glm::dot(normal, glm::normalize(ray.origin + ray.direction)));

    normal = glm::normalize(it->point - glm::vec3(0.f, 200.f, 0.f) - sphere.center);
    fmt::print("normal {}\n", normal);
    fmt::print("dotprod {}\n", glm::dot(normal, glm::normalize(ray.origin + ray.direction)));

    glm::vec3 color = ComputeLight(*it, sphere, light);
    fmt::print("{}\n", color);

}