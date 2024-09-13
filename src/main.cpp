//
// Created by Alex on 13/09/2024.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>
#include <fmt/base.h>

#include <cstdlib>
#include <array>
#include <optional>

template<size_t C>
using Pixel = std::array<unsigned char, C>;
template<size_t W, size_t H, size_t C>
using Image = std::array<Pixel<C>, W * H>;


struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct Sphere
{
    glm::vec3 center;
    float     radius;
};

struct Camera
{
    glm::vec3 position;
};

std::optional<float> Intersect(const Ray& ray, const Sphere& sphere)
{
    const auto oc = ray.origin - sphere.center;

    const auto b = glm::dot(oc, ray.direction);
    const auto c = glm::dot(oc, oc) - sphere.radius * sphere.radius;

    const auto delta = b * b - c;

    if (delta >= 0.f)
    {
        const auto sD = sqrt(delta);
        const auto t1 = -b - sD;
        const auto t2 = -b + sD;

        if (t1 >= 0)
        {
            return t1;
        }
        else if (t2 >= 0)
        {
            return t2;
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

int main(int /* argc */, char** /* argv */)
{
    constexpr size_t c = 3;
    constexpr size_t w = 800;
    constexpr size_t h = 600;

    [[maybe_unused]] Pixel<c> white { 255, 255, 255 };
    [[maybe_unused]] Pixel<c> green { 8,   128, 16 };
    [[maybe_unused]] Pixel<c> cyan  { 8,   96,  128 };

    Camera camera {{ 0.f, 0.f, -10000.f }};
    Sphere sphere {{ 0.f, 0.f, 200.f }, 180.f };

    Image<w, h, c> image;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const glm::vec3 pixel { (float)x * 2.f - w, (float)y * 2.f - h, 0.f };

            const Ray ray {{ pixel }, { glm::normalize(pixel - camera.position) }};

            std::optional<float> distance = Intersect(ray, sphere);
            if (distance.has_value())
            {
                auto v = (unsigned char)(distance.value());
                image[y * w + x] = { v, v, v };
            }
            else
            {
                image[y * w + x] = cyan;
            }
        }
    }

    stbi_write_png("image.png", w, h, c, &image[0][0], w * c);

    return EXIT_SUCCESS;
}