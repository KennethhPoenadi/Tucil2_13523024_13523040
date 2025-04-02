#include "include/quadtree.hpp"
#include <iostream>
using namespace std;

QuadTree::QuadTree(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY, int minX, int minY)
    : matrix(mat), x(x), y(y), sizeX(sizeX), sizeY(sizeY), 
      minBlockSizeX(minX), minBlockSizeY(minY),
      GambarKiriAtas(nullptr), GambarKananAtas(nullptr), GambarKiriBawah(nullptr), GambarKananBawah(nullptr) {}

const vector<vector<Pixel>>* QuadTree::getMatrix() const {
    return matrix;
}

int QuadTree::getX() const { 
    return x;
}

int QuadTree::getY() const {
    return y;
}

int QuadTree::getSizeX() const {
    return sizeX;
}

int QuadTree::getSizeY() const {
    return sizeY;
}

int QuadTree::getMinBlockSizeX() const {
    return minBlockSizeX;
}

int QuadTree::getMinBlockSizeY() const {
    return minBlockSizeY;
}

QuadTree* QuadTree::getGambarKiriAtas() const { 
    return GambarKiriAtas;
}

QuadTree* QuadTree::getGambarKananAtas() const {
    return GambarKananAtas;
}

QuadTree* QuadTree::getGambarKiriBawah() const {
    return GambarKiriBawah;
}

QuadTree* QuadTree::getGambarKananBawah() const {
    return GambarKananBawah;
}

void QuadTree::setGambarKiriAtas(QuadTree* node) {
    GambarKiriAtas = node;
}

void QuadTree::setGambarKananAtas(QuadTree* node) {
    GambarKananAtas = node;
}

void QuadTree::setGambarKiriBawah(QuadTree* node) {
    GambarKiriBawah = node;
}

void QuadTree::setGambarKananBawah(QuadTree* node) {
    GambarKananBawah = node;
}

QuadTree* buildQuadTree(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY, int minX, int minY, double threshold, bool useVariance, bool useMPD, bool useMAD, bool useEntropy, bool useSSIM) 
{
    double error = 0.0;
    
    if (useVariance) {
        error = calculateRGBAVariance(mat, x, y, sizeX, sizeY);
    } else if (useMPD) {
        error = calculateMPD(mat, x, y, sizeX, sizeY);

    } else if (useMAD) {
        error = calculateRGBAMad(mat, x, y, sizeX, sizeY);

    } else if (useEntropy) {
        error = calculateRGBAEntropyTotal(mat, x, y, sizeX, sizeY); 
    } else if (useSSIM) {
        vector<std::vector<Pixel>> avgBlock(sizeY, vector<Pixel>(sizeX));
        Pixel avgColor = getAverageColor(mat, x, y, sizeX, sizeY);
                
        for (int j = 0; j < sizeY; j++) {
            for (int i = 0; i < sizeX; i++) {
                avgBlock[j][i] = avgColor;
            }
        }

        double ssim = calculateSSIM_RGBA(mat, &avgBlock, x, y, sizeX, sizeY);
        error = 1.0 - ssim;

    }
    
    //berentikan pembagian jika ukuran blok sudah mencapai minimum atau error kurang dari atau sama dengan threshold
    if (sizeX <= minX || sizeY <= minY || error < threshold) {
        return new QuadTree(mat, x, y, sizeX, sizeY, minX, minY);
    }
    
    int midX = sizeX / 2;
    int midY = sizeY / 2;
    int sizeX1 = midX, sizeX2 = sizeX - midX;
    int sizeY1 = midY, sizeY2 = sizeY - midY;
    
    QuadTree* node = new QuadTree(mat, x, y, sizeX, sizeY, minX, minY);
    node->setGambarKiriAtas(buildQuadTree(mat, x, y, sizeX1, sizeY1, minX, minY, threshold, useVariance, useMPD, useMAD, useEntropy, useSSIM));
    node->setGambarKananAtas(buildQuadTree(mat, x + sizeX1, y, sizeX2, sizeY1, minX, minY, threshold, useVariance, useMPD, useMAD, useEntropy,useSSIM));
    node->setGambarKiriBawah(buildQuadTree(mat, x, y + sizeY1, sizeX1, sizeY2, minX, minY, threshold, useVariance, useMPD, useMAD, useEntropy,useSSIM));
    node->setGambarKananBawah(buildQuadTree(mat, x + sizeX1, y + sizeY1, sizeX2, sizeY2, minX, minY, threshold, useVariance, useMPD, useMAD, useEntropy,useSSIM));
    
    return node;
}