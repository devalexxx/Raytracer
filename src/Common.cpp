//
// Created by Alex on 20/09/2024.
//

#include <Raytracer/Common.h>

namespace rtc
{

	ThreadPool::ThreadPool(size_t count) :
		mIsActive(true), processed(0)
	{
		for (size_t i = 0; i < count; i++)
		{
			mPool.emplace_back(&ThreadPool::Run, this);
		}
	}

	ThreadPool::~ThreadPool()
	{
		mIsActive = false;
		mCv.notify_all();
		for (auto& th: mPool)
		{
			th.join();
		}
	}

	void ThreadPool::Post(std::function<void()> job)
	{
		std::unique_lock lock(mGuard);
		mPendingTasks.emplace_back(std::move(job));
		mCv.notify_one();
	}

	void ThreadPool::Join()
	{
		std::unique_lock lock(mGuard);
		mCvFinished.wait(lock, [&] { return mPendingTasks.empty() && processed == 0; });
	}

	void ThreadPool::Run() noexcept
	{
		while (mIsActive)
		{
			thread_local std::function<void()> job;
			{
				std::unique_lock lock(mGuard);
				mCv.wait(lock, [&] { return !mPendingTasks.empty() || !mIsActive; });
				if (!mIsActive) break;
				processed++;
				mCvFinished.notify_one();
				job.swap(mPendingTasks.front());
				mPendingTasks.pop_front();
			}
			job();
			{
				std::unique_lock lock(mGuard);
				processed--;
				mCvFinished.notify_one();
			}
		}
	}

	bool AlmostEqual(float lhs, float rhs, float epsilon)
	{
		return std::fabs(lhs - rhs) <= epsilon;
	}

	bool AlmostEqual(glm::vec3 lhs, glm::vec3 rhs, float epsilon)
	{
		return AlmostEqual(lhs.x, rhs.x, epsilon) && AlmostEqual(lhs.y, rhs.y, epsilon) && AlmostEqual(lhs.z, rhs.z, epsilon);
	}

}