//
// Created by Alex on 20/09/2024.
//

#ifndef RAYTRACER_COMMON_H
#define RAYTRACER_COMMON_H

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <atomic>
#include <mutex>
#include <functional>
#include <thread>

namespace rtc
{

	class ThreadPool
	{
		public:
			explicit ThreadPool(size_t count);
			~ThreadPool();

			void Post(std::function<void()> job);
			void Join();

		private:
			void Run() noexcept;

		private:
			std::atomic_bool mIsActive;
			std::vector<std::thread> mPool;
			std::condition_variable mCv;
			std::mutex mGuard;
			std::condition_variable mCvFinished;
			std::atomic_uint processed;
			std::deque<std::function<void()>> mPendingTasks;
	};

	template<typename T, size_t W, size_t H, size_t C = 1>
	struct HeapArray : public std::vector<T>
	{
			HeapArray() = delete;
			HeapArray(T&& value);

			static constexpr auto Width   = W;
			static constexpr auto Height  = H;
			static constexpr auto Channel = C;
	};

	template<size_t W, size_t H>
	using Image = HeapArray<unsigned char, W, H, 3>;

	template<size_t W, size_t H>
	using ImageRepr = HeapArray<glm::vec3, W, H>;

	bool AlmostEqual(float lhs, float rhs, float epsilon);
	bool AlmostEqual(glm::vec3 lhs, glm::vec3 rhs, float epsilon);

	template<size_t W, size_t H>
	void SaveImage(const Image<W, H>& image, const char* path);

	template<size_t W, size_t H>
	Image<W, H> ToneMap(const ImageRepr<W, H>& repr);

}

#include <Raytracer/Common.inl>

#endif
