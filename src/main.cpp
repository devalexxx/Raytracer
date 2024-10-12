//
// Created by Alex on 13/09/2024.
//

#include <glm/glm.hpp>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <Raytracer/Common.h>
#include <Raytracer/Scene.h>

//#include <thread>
#include <random>

rtc::Scene BuildBenchmarkScene(int n)
{
	using namespace rtc;

	Scene scene {
		.camera = { { 0.f, 0.f, -10000.f } },
		.lights = {
			{ { 5000.f, 0.f, 0.f }, { 400000.f, 400000.f, 400000.f } },
			{ { 1.f, -1000.f, 0.f }, { 100000.f, 0.f, 0.f } },
			{ { -1000.f, 1000.f, 0.f }, { 0.f, 100000.f, 0.f } } },
		.objects = Leaf {},
		.materials = {
			{ { 1.f, 1.f, 1.f }, { 0.f, 0.f, 0.f } }
		}
	};

	auto d = 300.f / (float) n;
	auto r = 80.f / (float) n;
	for (int x = -n; x < n; ++x)
	{
		for (int y = -n; y < n; ++y)
		{
			for (int z = -n; z < n; ++z)
			{
				AddObject(scene.objects, { glm::vec3((float) x * d, (float) y * d, 500.f + (float) z * d), r, &scene.materials[0] });
			}
		}
	}
	BuildObjectHierarchy(scene.objects);

	fmt::print("Build end with {} sphere\n", glm::pow<unsigned long>(n * 2, 3));

	return scene;
}

rtc::Scene BuildBasicScene()
{
	using namespace rtc;
	Scene scene {
		.camera = { { 0.f, 0.f, -10000.f } },
		.lights = {
			{ { 0.f, 1000.f, 500.f }, { 500.0f, 500.0f, 500.0f } },
		},
		.objects = Leaf {},
		.materials = {
			{ { 1.f, 1.f, 1.f }, { 0.0f, 0.0f, 0.0f } }
		}
	};

	AddObject(scene.objects, { glm::vec3{ 0.f, 0.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ 500.f, 500.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ 500.f, 0.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ 0.f, 500.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ -500.f, -500.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ -500.f, 0.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ 0.f, -500.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ 500.f, -500.f, 1000.f }, 200.f, &scene.materials[0] });
	AddObject(scene.objects, { glm::vec3{ -500.f, 500.f, 1000.f }, 200.f, &scene.materials[0] });

	BuildObjectHierarchy(scene.objects);

	return scene;
}

auto Gen() -> std::default_random_engine&
{
	static thread_local std::default_random_engine gen;
	return gen;
}

int main(int /* argc */, char** /* argv */)
{
	using namespace rtc;

	constexpr size_t w = 1280;
	constexpr size_t h = 720;

//	Scene scene = BuildBasicScene();
	Scene scene = BuildBenchmarkScene(50);

	auto ComputePixelFunctor = [&scene](size_t x, size_t y, glm::vec3* v) -> std::function<void()>
	{
		return [=, &scene] -> void
		{
			std::uniform_real_distribution<float> dist(.0f, .99f);
			for (size_t k = 0; k < 10; ++k)
			{
				const glm::vec3 pixel { (float) x * 2.f + dist(Gen()) - w, (float) y * 2.f + dist(Gen()) - h, 0.f };
				const Ray       ray { pixel, pixel - scene.camera.position };

				auto it = Intersect(ray, scene.objects);
				if (it.has_value())
				{
					*v += ComputeLight(*it, scene);
				}
			}
		};
	};

	ImageRepr<w, h> imageRepr(glm::vec3(0.f, 0.f, 0.f));
	ThreadPool      pool(std::thread::hardware_concurrency());

	auto start = std::chrono::steady_clock::now();
	for (size_t y = 0; y < h; ++y)
	{
		for (size_t x = 0; x < w; ++x)
		{
			pool.Post(ComputePixelFunctor(x, y, &imageRepr[y * w + x]));
		}
	}
	pool.Join();

	fmt::print("{}\n", std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count());

	auto image = ToneMap<w, h>(imageRepr);
	SaveImage(image, "image.png");

	return EXIT_SUCCESS;
}
