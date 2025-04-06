#ifndef GIF_HPP
#define GIF_HPP

#include "image.hpp"

class QuadTree;

void gifMaker(int width, int height, const char* gifOutput, int maxDepth, QuadTree* root, std::vector<std::vector<Pixel>>& reconstructImageMatrix);

#endif
