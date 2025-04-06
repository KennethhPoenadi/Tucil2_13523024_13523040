#include "include/gifer.hpp"
#include "include/quadtree.hpp"
#include "gif.h"

#include <iostream>

void gifMaker(int width, int height, const char* gifOutput, int maxDepth, QuadTree* root, std::vector<std::vector<Pixel>>& reconstructImageMatrix) {
    GifWriter writer;


    if (!GifBegin(&writer, gifOutput, width, height, 10)) {
        std::cerr << "Gagal membuat GIF\n";
        return;
    }

    for (int i = maxDepth; i >= 0; i--) {
        root->reconstructImageFrame(root, reconstructImageMatrix, 0, 0, i, maxDepth + 1);

        std::vector<uint8_t> frameData(width * height * 4);
        for (int y = 0; y < height;++y) {
            for (int x = 0; x < width;++x) {
                int idx = (y * width + x) * 4;
                const Pixel& p = reconstructImageMatrix[y][x];
                frameData[idx + 0] = p.r;
                frameData[idx + 1] = p.g;
                frameData[idx + 2] = p.b;
                frameData[idx + 3] = 255;
            }
        }

        GifWriteFrame(&writer, frameData.data(), width, height, 50);
    }

    GifEnd(&writer);

    cout << endl;
    cout<< "GIF berhasil dibuat" << endl;
}
