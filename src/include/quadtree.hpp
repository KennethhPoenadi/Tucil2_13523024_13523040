#ifndef QUADTREE_HPP
#define QUADTREE_HPP

#include "image.hpp"
#include "utils.hpp"

using namespace std;

class QuadTree {
    private:
        const vector<vector<Pixel>>* matrix;
        double x, y;
        double sizeX, sizeY;
        double minBlockSize;
        QuadTree *GambarKiriAtas, *GambarKananAtas, *GambarKiriBawah, *GambarKananBawah;
        Pixel averageColor;
        
    public:
        QuadTree(const vector<vector<Pixel>>* mat, double x, double y, double sizeX, double sizeY, int minBlockSize);
        const vector<vector<Pixel>>* getMatrix() const;
        double getX() const;
        double getY() const;
        double getSizeX() const;
        double getSizeY() const;
        double getMinBlockSize() const;
        QuadTree* getGambarKiriAtas() const;
        QuadTree* getGambarKananAtas() const;
        QuadTree* getGambarKiriBawah() const;
        QuadTree* getGambarKananBawah() const;
        void setGambarKiriAtas(QuadTree* node);
        void setGambarKananAtas(QuadTree* node);
        void setGambarKiriBawah(QuadTree* node);
        void setGambarKananBawah(QuadTree* node);
        static QuadTree* buildQuadTree(const vector<vector<Pixel>>* mat, double x, double y, double sizeX, double sizeY, double minBlockSize, double threshold, bool useVariance, bool useMPD, bool useMAD, bool useEntropy, bool useSSIM);
        bool isLeaf(QuadTree* node);
        void reconstructImage(QuadTree* node, vector<vector<Pixel>>& pixelMatrix, double x, double y);
        static int depthTree(QuadTree* node, int depth=0);
        static long countNode(QuadTree* node, long nodes=0);
        void reconstructImageFrame(QuadTree* node, vector<vector<Pixel>>& pixelMatrix, double offsetX, double offsetY, int depth, int maxDepth);
};

#endif