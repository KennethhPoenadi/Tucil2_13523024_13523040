//di include/image.hpp
#ifndef IMAGE_HPP
#define IMAGE_HPP

#include <vector>
#include <string>
#include "stb_image.h"
#include "stb_image_write.h"

using namespace std;

struct Pixel {
    unsigned char r, g, b;
};

struct Image {
    int width;
    int height;
    int channels;
    vector<std::vector<Pixel>> pixels;
};

Image loadImage(const string& filename);
bool checkFile(const string& filename);

#endif