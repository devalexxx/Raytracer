//
// Created by Alex on 13/09/2024.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>
#include <fmt/base.h>

#include <cstdlib>
#include <array>
#include <vector>
#include <optional>

template<size_t W, size_t H, size_t C>
using Image = std::array<unsigned char, C * W * H>;
template<size_t W, size_t H, size_t C>
using ImageRepr = std::array<float, C * W * H>;

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
};

struct Sphere
{
    glm::vec3 center;
    float     radius;
    Material  material;
};

struct Camera
{
    glm::vec3 position;
};

struct Light
{
    glm::vec3 position;
    glm::vec3 quantity;
};

std::optional<Intersection> Intersect(const Ray& ray, const Sphere& sphere)
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
            return Intersection { ray.direction * t1 + ray.origin, t1 };
        }
        else if (t2 >= 0)
        {
            return Intersection { ray.direction * t2 + ray.origin, t2 };
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
    Ray ray { light.position, glm::normalize(it.point - light.position) };
    if (Intersect(ray, sphere).has_value())
    {
        // L_o = L_e + V(P, L_p) * L_emit / D ^ 2 * Albedo * | N . L_i |
        // Le = 0, admettons que la sphere n'émet rien
        // V(P, L_p) = 1 Admettons qu'il y a une intersection
        // Lampe_e = light.strength
        // D**2 = distance2
        // Albedo = material.albedo
        auto normal   = glm::normalize(it.point - sphere.center);
        auto distance = glm::distance(it.point, light.position);
        auto d2 = distance * distance;

        auto color = (
            glm::vec3(1.f) +
            1.f *
            (light.quantity / d2) *
            sphere.material.albedo *
            glm::abs(glm::dot(normal, ray.origin + ray.direction))
        );
        return color;
    }

    return glm::vec3 { 0.f };
}

template<size_t W, size_t H>
Image<W, H, 3> ToneMap(const ImageRepr<W, H, 3>& repr)
{
    auto max = *std::max_element(repr.begin(), repr.end());
    auto transform = [&max](float value) -> unsigned char
    {
        return (unsigned char)((255 * value) / max);
    };

    Image<W, H, 3> ret;
    for (size_t y = 0; y < H; ++y) {
        for (size_t x = 0; x < W * 3; x += 3) {
            ret[y * W * 3 + x + 0] = transform(repr[y * W * 3 + x + 0]);
            ret[y * W * 3 + x + 1] = transform(repr[y * W * 3 + x + 1]);
            ret[y * W * 3 + x + 2] = transform(repr[y * W * 3 + x + 2]);
        }
    }

    return ret;
}

int main(int /* argc */, char** /* argv */)
{
    constexpr size_t c = 3;
    constexpr size_t w = 800;
    constexpr size_t h = 600;

    Camera camera {{ 0.f, 0.f, -10000.f }};
    Sphere sphere {{ 0.f, 0.f, 200.f }, 200.f, {{ 1.f, 1.f, 1.f }} };

    std::vector<Light> lights = {
        {{ 1000.f, 0.f, 200.f }, { 90000.f, 90000.f, 90000.f } },
        {{ -1000.f, 0.f, 200.f }, { 90000.f, 90000.f, 90000.f }},
    };

    ImageRepr<w, h, c> imageRepr;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0, x1 = 0; x < w * c; x += c, x1++) {
            const glm::vec3 pixel { (float)x1 * 2.f - w, (float)y * 2.f - h, 0.f };

            const Ray ray { pixel, glm::normalize(pixel - camera.position) };

            auto it = Intersect(ray, sphere);
            if (it.has_value())
            {
                glm::vec3 color { 0.f };
                for (auto& light: lights)
                {
                    color += ComputeLight(*it, sphere, light);
                }
                imageRepr[y * w * c + x + 0] = color.x;
                imageRepr[y * w * c + x + 1] = color.y;
                imageRepr[y * w * c + x + 2] = color.z;
            }
            else
            {
                imageRepr[y * w * c + x + 0] = 0.f;
                imageRepr[y * w * c + x + 1] = 0.f;
                imageRepr[y * w * c + x + 2] = 0.f;
            }
        }
    }

    auto image = ToneMap<w, h>(imageRepr);
    stbi_write_png("image.png", w, h, c, &image[0], w * c);

    return EXIT_SUCCESS;
}