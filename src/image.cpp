#include "include/image.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"

using namespace std;

Image loadImage(const string& filename) {
    Image img;
    unsigned char* data = stbi_load(filename.c_str(), &img.width, &img.height, &img.channels, 4);
    
    if (!data) {
        cerr << "Error: Tidak dapat memuat gambar " << filename << endl;
        img.width = img.height = img.channels = 0;
        return img;
    }
    
    img.channels = 4;
    img.pixels.resize(img.height, vector<Pixel>(img.width));
    
    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int index = (y * img.width + x) * img.channels;
            img.pixels[y][x].r = data[index];     // Red
            img.pixels[y][x].g = data[index+1];   // Green
            img.pixels[y][x].b = data[index+2];   // Blue
            img.pixels[y][x].a = data[index+3];   // Alpha
        }
    }
    
    stbi_image_free(data);
    return img;
}

bool checkFile(const string& filename) {
    FILE* file = fopen(filename.c_str(), "rb");
    if (file) {
        fclose(file);
        return true;
    }
    return false;
}