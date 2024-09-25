//
// Created by Alex on 13/09/2024.
//

#include <glm/glm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <Raytracer/Common.h>
#include <Raytracer/Scene.h>

#include <thread>
#include <future>

int main(int /* argc */, char** /* argv */)
{
	using namespace rtc;

	constexpr size_t w = 1280;
	constexpr size_t h = 720;

	Material sphereMaterial {{ 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f }};

	Scene scene
	{
		.camera={ { 0.f, 0.f, -10000.f } },
		.lights={
			{ { 5000.f, 0.f, 0.f }, { 400000.f, 400000.f, 400000.f } },
			{ { 1.f, -1000.f, 0.f }, { 100000.f, 0.f, 0.f } },
			{ { -1000.f, 1000.f, 0.f }, { 0.f, 100000.f, 0.f } }
		},
		.objects=Leaf{}
	};

	auto n = 15;
	auto d = 300.f / (float)n;
	auto r = 80.f / (float)n;
	for (int x = -n; x < n; ++x)
	{
		for (int y = -n; y < n; ++y)
		{
			for (int z = -n; z < n; ++z)
			{
				AddObject(scene.objects, { glm::vec3((float)x * d, (float)y * d, 500.f + (float)z * d), r, &sphereMaterial });
			}
		}
	}
	BuildObjectHierarchy(scene.objects);

	fmt::print("Build end with {} sphere\n", glm::pow<unsigned long>(n * 2, 3));

	ImageRepr<w, h> imageRepr(glm::vec3(0.f, 0.f, 0.f));
	for (size_t y = 0; y < h; ++y)
	{
		for (size_t x = 0; x < w; ++x)
		{
			const glm::vec3 pixel { (float) x * 2.f - w, (float) y * 2.f - h, 0.f };
			const Ray ray { pixel, pixel - scene.camera.position };

			auto it = Intersect(ray, scene.objects);
			if (it.has_value())
			{
				imageRepr[y * w + x] = ComputeLight(*it, scene);
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