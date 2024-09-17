
#include <vector>
#include <fmt/format.h>

template<>
struct fmt::formatter<glm::vec3> : formatter<std::string>
{
    auto format(glm::vec3 const& vec, format_context& ctx) const
    {
        auto fmt = fmt::format("{} {} {}", vec.x, vec.y, vec.z);
        return formatter<std::string>::format(fmt, ctx);
    }
};

namespace rtc
{

    template<size_t W, size_t H, size_t C>
    Image<W, H, C> ToneMap(const ImageRepr<W, H, C>& repr)
    {
        auto max = *std::max_element(
            repr.begin(),
            repr.end(),
            [](const glm::vec<C, float>& lhs, const glm::vec<C, float>& rhs) {
                return glm::length(lhs) < glm::length(rhs);
            }
        );
        auto factor = 255.f / ((max / glm::length(max)) * 255.f);

//        Image<W, H, C> image;
        Image<W, H, C> image(W * H * C, 0);
        for (size_t y = 0; y < H; ++y)
        {
            for (size_t x = 0, x1 = 0; x < W; ++x, x1 += 3)
            {
                // Cout = ((Cin / Lin) * Lout) * correction_factor
                auto cout = ((repr[y * W + x] / glm::length(max)) * 255.f) * factor;
                image[y * W * C + x1 + 0] = (unsigned char)cout.x;
                image[y * W * C + x1 + 1] = (unsigned char)cout.y;
                image[y * W * C + x1 + 2] = (unsigned char)cout.z;
            }
        }

        return image;
    }

}