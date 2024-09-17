//
// Created by Alex on 17/09/2024.
//

#include <doctest/doctest.h>

#include <Raytracer/Lib.h>

TEST_CASE("Intersection")
{
    using namespace rtc;

    glm::vec3 origin { 0.f, 500.f, 200.f };
    Sphere sphere { { 0.f, 0.f, 200.f }, 100.f, {} };
    Ray    ray    { origin,  sphere.center - origin };

    auto it = Intersect(ray, sphere);

    REQUIRE(it.has_value());
    CHECK_EQ(it->point, sphere.center + glm::vec3 { 0.f, sphere.radius, 0.f });
}