#include "include/image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <stdexcept>

using namespace std;

Image loadImage(const string& filename) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
    
    if (!data) {
        throw runtime_error("Could not load image: " + filename);
    }

    //membuat struktur image
    Image img;
    img.width = width;
    img.height = height;
    img.channels = channels;
    
    img.pixels.resize(height, vector<Pixel>(width));
    
    //konversi data mentah menjadi array 2d pixel
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            
            img.pixels[y][x].r = data[index];
            img.pixels[y][x].g = data[index + 1];
            img.pixels[y][x].b = data[index + 2];
            
            //tambahkan alpha channel jika ada
            if (channels == 4) {
                img.pixels[y][x].a = data[index + 3];
            } else {
                img.pixels[y][x].a = 255; //default full opacity
            }
        }
    }
    
    //bebaskan memori dari stb_image
    stbi_image_free(data);
    
    return img;
}
