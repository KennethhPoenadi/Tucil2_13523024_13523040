#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "image.hpp"
#include <vector>

using namespace std;

class QuadTree {
    private:
        vector<vector<Pixel>> matrix;
        int x, y, sizeX, sizeY;
        QuadTree *GambarKiriAtas, *GambarKananAtas, *GambarKiriBawah, *GambarKananBawah;

    public:
        QuadTree(const vector<vector<Pixel>>& mat, int x, int y, int sizeX, int sizeY);
        vector<vector<Pixel>> getMatrix() const;
        int getX() const;
        int getY() const;
        int getSizeX() const;
        int getSizeY() const;
        QuadTree* getGambarKiriAtas() const;
        QuadTree* getGambarKananAtas() const;
        QuadTree* getGambarKiriBawah() const;
        QuadTree* getGambarKananBawah() const;
        void setGambarKiriAtas(QuadTree* node);
        void setGambarKananAtas(QuadTree* node);
        void setGambarKiriBawah(QuadTree* node);
        void setGambarKananBawah(QuadTree* node);
};

QuadTree* buildQuadTree(const vector<vector<Pixel>>& mat, int x, int y, int sizeX, int sizeY);

#endif
