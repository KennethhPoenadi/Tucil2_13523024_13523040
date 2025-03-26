//di include/image.hpp
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <string>

using namespace std;

struct Pixel {
    unsigned char r, g, b, a;
};

struct Image {
    int width;
    int height;
    int channels;
    vector<std::vector<Pixel>> pixels;
};

Image loadImage(const string& filename);

#endif