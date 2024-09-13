//
// Created by Alex on 13/09/2024.
//

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

#include <cstdlib>
#include <array>

template<size_t C>
using Pixel = std::array<unsigned char, C>;
template<size_t W, size_t H, size_t C>
using Image = std::array<Pixel<C>, W * H>;

int main(int /* argc */, char** /* argv */)
{
    constexpr size_t c = 3;
    constexpr size_t w = 800;
    constexpr size_t h = 600;

    Image<w, h, c> image;
    for (size_t y = 0; y < h; ++y) {
        for (size_t x = 0; x < w; ++x) {
            image[y * w + x][0] = 127;
            image[y * w + x][1] = 0;
            image[y * w + x][2] = 196;
        }
    }

    stbi_write_png("image.png", w, h, c, &image[0][0], w * c);

    return EXIT_SUCCESS;
}