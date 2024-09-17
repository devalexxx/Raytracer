//
// Created by Alex on 17/09/2024.
//

#ifndef RAYTRACER_LIB_H
#define RAYTRACER_LIB_H

#include <glm/glm.hpp>

#include <array>
#include <optional>

namespace rtc
{

    template<size_t W, size_t H, size_t C>
    using Image = std::vector<unsigned char>;
    template<size_t W, size_t H, size_t C>
    using ImageRepr = std::vector<glm::vec<C, float>>;

//    template<size_t W, size_t H, size_t C>
//    using Image = std::array<unsigned char, C * W * H>;
//    template<size_t W, size_t H, size_t C>
//    using ImageRepr = std::array<glm::vec<C, float>, W * H>;

    struct Ray
    {
        glm::vec3 origin;
        glm::vec3 direction;
    };

    struct Intersection
    {
        glm::vec3 point;
        float     distance;
    };

    struct Material
    {
        glm::vec3 albedo;
        glm::vec3 emission;
    };

    struct Sphere
    {
        glm::vec3 center;
        float     radius;
        Material  material;
    };;

    struct Camera
    {
        glm::vec3 position;
    };

    struct Light
    {
        glm::vec3 position;
        glm::vec3 quantity;
    };

    bool AlmostEqual(float lhs, float rhs, float epsilon);
    bool AlmostEqual(glm::vec3 lhs, glm::vec3 rhs, float epsilon);

    std::optional<Intersection> Intersect(const Ray& ray, const Sphere& sphere);
    glm::vec3 ComputeLight(const Intersection& it, const Sphere& sphere, const Light& light);

    template<size_t W, size_t H, size_t C>
    Image<W, H, C> ToneMap(const ImageRepr<W, H, C>& repr);

}

#include <Raytracer/Lib.inl>

#endif
