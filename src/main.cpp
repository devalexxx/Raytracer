//
// Created by Alex on 13/09/2024.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>
#include <glm/glm.hpp>

#include <Raytracer/Lib.h>

int main(int /* argc */, char** /* argv */)
{
    using namespace rtc;

    constexpr size_t c = 3;
    constexpr size_t w = 1280;
    constexpr size_t h = 720;

    Camera camera {{ 0.f, 0.f, -10000.f }};
    Sphere sphere {{ 0.f, 0.f, 1000.f }, 100.f, {{ 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f }} };

    std::vector<Light> lights = {
        {{0.f, 1000.f, 1000.f}, {500.f, 500.f, 500.f}},
        {{0.f, -1000.f, 1000.f}, {500.f, 500.f, 0.f}},
    };

    ImageRepr<w, h, c> imageRepr(h * w, glm::vec3(0.f, 0.f, 0.f));
//    ImageRepr<w, h, c> imageRepr;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const glm::vec3 pixel { (float)x * 2.f - w, (float)y * 2.f - h, 0.f };

            const Ray ray { pixel, pixel - camera.position };

            auto it = Intersect(ray, sphere);
            if (it.has_value())
            {
                glm::vec3 color { 0.f, 0.f, 0.f };
                for (auto& light: lights)
                {
                    color += ComputeLight(*it, sphere, light);
                }
                imageRepr[y * w + x] = color;
            }
            else
            {
                imageRepr[y * w + x] = { 0.f, 0.f, 0.f };
            }
        }
    }

    auto image = ToneMap<w, h, c>(imageRepr);
    stbi_write_png("image.png", w, h, c, &image[0], w * c);

    return EXIT_SUCCESS;
}