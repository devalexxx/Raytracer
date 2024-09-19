
#include <fmt/format.h>

#include <vector>

template<>
struct fmt::formatter<glm::vec3> : formatter<std::string>
{
		auto format(const glm::vec3& vec, format_context& ctx) const
		{
			auto fmt = fmt::format("{} {} {}", vec.x, vec.y, vec.z);
			return formatter<std::string>::format(fmt, ctx);
		}
};

namespace rtc
{

	template<typename T, size_t W, size_t H, size_t C>
	HeapArray<T, W, H, C>::HeapArray(T&& value) :
		std::vector<T>(Width * Height * C, std::move(value))
	{}

	template<size_t W, size_t H>
	void SaveImage(const Image<W, H>& image, const char* path)
	{
		stbi_write_png(path, W, H, image.Channel, &image[0], W * image.Channel);
	}

	template<size_t W, size_t H>
	Image<W, H> ToneMap(const ImageRepr<W, H>& repr)
	{
		auto max = *std::max_element(
			repr.begin(),
			repr.end(),
			[](const glm::vec3& lhs, const glm::vec3& rhs) {
				return glm::length(lhs) < glm::length(rhs);
			}
		);
		auto factor = 255.f / ((max / glm::length(max)) * 255.f);

		Image<W, H> image(0);
		for (size_t y = 0; y < H; ++y)
		{
			for (size_t x = 0, x1 = 0; x < W; ++x, x1 += image.Channel)
			{
				// Cout = ((Cin / Lin) * Lout) * correction_factor
				auto cout                             = ((repr[y * W + x] / glm::length(max)) * 255.f) * factor;
				image[y * W * image.Channel + x1 + 0] = (unsigned char) cout.x;
				image[y * W * image.Channel + x1 + 1] = (unsigned char) cout.y;
				image[y * W * image.Channel + x1 + 2] = (unsigned char) cout.z;
			}
		}

		return image;
	}

}