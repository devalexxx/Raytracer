//
// Created by Alex on 13/09/2024.
//

#include <glm/glm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <Raytracer/Common.h>
#include <Raytracer/Scene.h>

int main(int /* argc */, char** /* argv */)
{
    using namespace rtc;

    constexpr size_t w = 1280;
    constexpr size_t h = 720;

    Scene scene {
		 {{ 0.f, 0.f, -10000.f }},
        {
            {{-300.f, -1000.f, 800.f}, {500.f, 500.f, 500.f}},
        },
        {
            Shape(Sphere {{ 300.f, -200.f, 1000.f }, 200.f, {{ 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f }}}),
            Shape(Sphere {{ -300.f, -200.f, 1000.f }, 200.f, {{ 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f }}}),
			Shape(Surface {{ 0.f, 820.f, -500.f }, { 0.f, 1.f, 0.2f }, {{ 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f }}})
        }
    };

    ImageRepr<w, h> imageRepr(glm::vec3(0.f, 0.f, 0.f));
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            const glm::vec3 pixel { (float)x * 2.f - w, (float)y * 2.f - h, 0.f };

            const Ray ray { pixel, pixel - scene.camera.position };

            IntersectionOpt nearestIt;
            for (auto& shape: scene.shapes)
            {
                auto it = shape.Intersect(ray);
                if (it.has_value())
                {
                    if (nearestIt.has_value())
                    {
                        if (nearestIt->distance > it->distance)
                        {
                            nearestIt = it;
                        }
                    }
                    else
                    {
                        nearestIt = it;
                    }
                }
            }

            if (nearestIt.has_value())
            {
                glm::vec3 color { 0.f, 0.f, 0.f };
                for (auto& light: scene.lights)
                {
                    color += ComputeLight(*nearestIt, scene.shapes, light);
                }
                imageRepr[y * w + x] = color;
            }
            else
            {
                imageRepr[y * w + x] = { 0.f, 0.f, 0.f };
            }
        }
    }

    auto image = ToneMap<w, h>(imageRepr);
    SaveImage(image, "image.png");

    return EXIT_SUCCESS;
}