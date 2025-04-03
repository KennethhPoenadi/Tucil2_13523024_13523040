#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "image.hpp"
#include "utils.hpp"

using namespace std;

class QuadTree {
    private:
        const vector<vector<Pixel>>* matrix;
        int x, y, sizeX, sizeY;
        int minBlockSizeX, minBlockSizeY;
        QuadTree *GambarKiriAtas, *GambarKananAtas, *GambarKiriBawah, *GambarKananBawah;
        Pixel averageColor;
        
    public:
        QuadTree(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY, int minX, int minY);
        const vector<vector<Pixel>>* getMatrix() const;
        int getX() const;
        int getY() const;
        int getSizeX() const;
        int getSizeY() const;
        int getMinBlockSizeX() const;
        int getMinBlockSizeY() const;
        QuadTree* getGambarKiriAtas() const;
        QuadTree* getGambarKananAtas() const;
        QuadTree* getGambarKiriBawah() const;
        QuadTree* getGambarKananBawah() const;
        void setGambarKiriAtas(QuadTree* node);
        void setGambarKananAtas(QuadTree* node);
        void setGambarKiriBawah(QuadTree* node);
        void setGambarKananBawah(QuadTree* node);
        static QuadTree* buildQuadTree(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY, int minX, int minY, double threshold, bool useVariance, bool useMPD, bool useMAD, bool useEntropy, bool useSSIM);
        bool isLeaf(QuadTree* node);
        void reconstructImage(QuadTree* node, vector<vector<Pixel>>& pixelMatrix, int x, int y);
};

#endif