#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include <vector>
#include "image.hpp"

using namespace std;

struct QuadtreeNode {
    int x, y, size; //koordinat & ukuran blok
    Pixel avgColor; //warna rata-rata blok
    bool isLeaf; //true kalau ini leaf node

    QuadtreeNode* children[4]; //empat sub-blok

    QuadtreeNode(int x, int y, int size, Pixel color);
    ~QuadtreeNode(); //destructor buat free memori
};

class Quadtree {
public:
    QuadtreeNode* root;
    int threshold; //ambang batas error

    Quadtree(int threshold);
    ~Quadtree();

    void build(const Image& img);
    void compress(QuadtreeNode* node, const Image& img);
    void reconstruct(Image& img);
};

#endif
