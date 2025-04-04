#include "include/image.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/stb_image_write.h"
#include <fstream>

using namespace std;

Image loadImage(const string& filename) {
    Image img;
    unsigned char* data = stbi_load(filename.c_str(), &img.width, &img.height, &img.channels, 0);
    
    if (!data) {
        cerr << "Error: Tidak dapat memuat gambar " << filename << endl;
        img.width = img.height = img.channels = 0;
        return img;
    }

    img.pixels.resize(img.height, vector<Pixel>(img.width));

    for (int y = 0; y < img.height; ++y) {
        for (int x = 0; x < img.width; ++x) {
            int index = (y * img.width + x) * img.channels;
            Pixel& p = img.pixels[y][x];
            
            if (img.channels == 3) {  
                p.r = data[index];
                p.g = data[index + 1];
                p.b = data[index + 2];
                p.a = 255;
            } else if (img.channels == 4) {  
                p.r = data[index];
                p.g = data[index + 1];
                p.b = data[index + 2];
                p.a = data[index + 3];
            }
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

void saveReconstructedImage(const  string &filename, const  vector< vector<Pixel>>& image, const string& format) {
    int height = image.size();
    int width = image[0].size();

    bool isGrayscale = true;
    for (int y = 0; y < height && isGrayscale; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image[y][x];
            if (p.r != p.g || p.g != p.b) {
                isGrayscale = false;
                break;
            }
        }
    }

    int channels = isGrayscale ? 1 : 3;
    unsigned char* data = new unsigned char[width * height * channels];

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            const Pixel& p = image[y][x];
            int index = (y * width + x) * channels;

            if (isGrayscale) {
                data[index] = p.r;
            } else {
                data[index] = p.r;
                data[index + 1] = p.g;
                data[index + 2] = p.b;
            }
        }
    }

     if (format == "png") {
        if (!stbi_write_png((filename + ".png").c_str(), width, height, channels, data, width * channels)) {
             cout << "Gagal save PNG" <<  endl;
        } else {
            cout << "======================================" << endl;
            cout << endl;
            cout << "PNG berhasil disave" <<  endl;
            cout << endl;

            cout << "======================================" << endl;
        }
    } else if (format == "jpg") {
        if (!stbi_write_jpg((filename + ".jpg").c_str(), width, height, channels, data, 50)) {
             cout << "Gagal save JPG" <<  endl;
        } else {
            cout << "======================================" << endl;
            cout << endl;
             cout << "JPG berhasil disave" <<  endl;
             cout << endl;

        }
    } else if (format == "jpeg") {
        if (!stbi_write_jpg((filename + ".jpeg").c_str(), width, height, channels, data, 50)) {
             cout << "Gagal save JPEG" <<  endl;
        } else {
            cout << "======================================" << endl;
            cout << endl;
             cout << "JPEG berhasil disave" <<  endl;
             cout << endl;

        }
    } 

    delete[] data;
}

string getFileExtension(const string& filename) {
    size_t dotPos = filename.find_last_of(".");
    if (dotPos == string::npos) return "";
    return filename.substr(dotPos + 1);
}

double getFileSize(const string& fileName) {
     ifstream file(fileName,  ios::binary |  ios::ate);
    return file.tellg()/1024;
}

double calculateCompression(double originalSize, double compressedSize) {
    return (1 - (compressedSize / originalSize)) * 100;
}