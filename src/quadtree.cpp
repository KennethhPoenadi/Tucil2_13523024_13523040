#include "include/quadtree.hpp"
#include <iostream>
#include "include/image.hpp"
using namespace std;

QuadTree::QuadTree(const vector<vector<Pixel>>* mat, double x, double y, double sizeX, double sizeY, int minBlockSizeYes)
    : matrix(mat), x(x), y(y), sizeX(sizeX), sizeY(sizeY), 
      minBlockSize(minBlockSizeYes),
      GambarKiriAtas(nullptr), GambarKananAtas(nullptr), GambarKiriBawah(nullptr), GambarKananBawah(nullptr) {}

const vector<vector<Pixel>>* QuadTree::getMatrix() const {
    return matrix;
}

double QuadTree::getX() const { 
    return x;
}

double QuadTree::getY() const {
    return y;
}

double QuadTree::getSizeX() const {
    return sizeX;
}

double QuadTree::getSizeY() const {
    return sizeY;
}

double QuadTree::getMinBlockSize() const {
    return minBlockSize;
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

QuadTree* QuadTree::buildQuadTree(const vector<vector<Pixel>>* mat, double x, double y, double sizeX, double sizeY, double minBlockSize, double threshold, bool useVariance, bool useMPD, bool useMAD, bool useEntropy, bool useSSIM) 
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
        int startX = static_cast<int>(x);
        int startY = static_cast<int>(y);
        int blockSizeX = static_cast<int>(sizeX);
        int blockSizeY = static_cast<int>(sizeY);
        
        vector<vector<Pixel>> avgBlock(blockSizeY, vector<Pixel>(blockSizeX));
        
        Pixel avgColor = getAverageColor(mat, startX, startY, blockSizeX, blockSizeY);
                
        for (int j = 0; j < blockSizeY; j++) {
            for (int i = 0; i < blockSizeX; i++) {
                avgBlock[j][i] = avgColor;
            }
        }


        double ssim = calculateSSIM_RGB(mat, &avgBlock, startX, startY, blockSizeX, blockSizeY);
        error = 1.0 - ssim; 
    }

    int block_area = static_cast<int>(sizeX * sizeY);
    int min_block_size = static_cast<int>(minBlockSize);
    
    if ((block_area / 2) < min_block_size || block_area < min_block_size || error <= threshold) {
        QuadTree* leaf = new QuadTree(mat, x, y, sizeX, sizeY, minBlockSize);
        leaf->averageColor = getAverageColor(mat, static_cast<int>(x), static_cast<int>(y), 
                                            static_cast<int>(sizeX), static_cast<int>(sizeY));
        return leaf;
    }
    
    double midX = sizeX / 2;
    double midY = sizeY / 2;
    double sizeX1 = midX, sizeX2 = sizeX - midX;
    double sizeY1 = midY, sizeY2 = sizeY - midY;
    
    QuadTree* node = new QuadTree(mat, x, y, sizeX, sizeY, minBlockSize);
    node->averageColor = getAverageColor(mat, static_cast<int>(x), static_cast<int>(y), 
                                        static_cast<int>(sizeX), static_cast<int>(sizeY));
    
    node->setGambarKiriAtas(buildQuadTree(mat, x, y, sizeX1, sizeY1, minBlockSize, threshold, 
                                        useVariance, useMPD, useMAD, useEntropy, useSSIM));
    node->setGambarKananAtas(buildQuadTree(mat, x + sizeX1, y, sizeX2, sizeY1, minBlockSize, threshold, 
                                         useVariance, useMPD, useMAD, useEntropy, useSSIM));
    node->setGambarKiriBawah(buildQuadTree(mat, x, y + sizeY1, sizeX1, sizeY2, minBlockSize, threshold, 
                                         useVariance, useMPD, useMAD, useEntropy, useSSIM));
    node->setGambarKananBawah(buildQuadTree(mat, x + sizeX1, y + sizeY1, sizeX2, sizeY2, minBlockSize, threshold, 
                                          useVariance, useMPD, useMAD, useEntropy, useSSIM));
    
    return node;
}

bool QuadTree::isLeaf(QuadTree* node) {
    if ((node->getGambarKiriAtas() == nullptr) && (node->getGambarKananAtas() == nullptr) && (node->getGambarKiriBawah() == nullptr) && (node->getGambarKananBawah() == nullptr)) return true;
    else return false;
}

void QuadTree::reconstructImage(QuadTree* node, vector<vector<Pixel>>& pixelMatrix, double offsetX, double offsetY){
    if(!node) return;

    if (isLeaf(node)) {
        for (int i = 0; i < node -> getSizeY(); i++) {
            for (int j = 0; j < node -> getSizeX(); j++) {
                double imgX = offsetX + j;
                double imgY = offsetY + i;
                pixelMatrix[imgY][imgX] = node->averageColor;
            }
        }
        return;
    }
     if (node -> getGambarKiriAtas()) {
         reconstructImage(node -> getGambarKiriAtas(), pixelMatrix, offsetX, offsetY);
     }
     if (node -> getGambarKananAtas()) {
         reconstructImage(node -> getGambarKananAtas(), pixelMatrix, offsetX + node->getSizeX() /2, offsetY);
     }
     if (node -> getGambarKiriBawah()) {
         reconstructImage(node -> getGambarKiriBawah(), pixelMatrix, offsetX, offsetY + node->getSizeY() /2);
     }
     if (node -> getGambarKananBawah()) {
         reconstructImage(node -> getGambarKananBawah(), pixelMatrix, offsetX + node->getSizeX() /2, offsetY + node->getSizeY() /2);
     }
}

void QuadTree::reconstructImageFrame(QuadTree* node, vector<vector<Pixel>>& pixelMatrix, double offsetX, double offsetY, int depth, int maxDepth){
    if(!node) return;

    if (depth < maxDepth) {
        for (int i = 0; i < node -> getSizeY(); i++) {
            for (int j = 0; j < node -> getSizeX(); j++) {
                double imgX = offsetX + j;
                double imgY = offsetY + i;
                pixelMatrix[imgY][imgX] = node->averageColor;
            }
        }
     if (node -> getGambarKiriAtas()) {
         reconstructImageFrame(node -> getGambarKiriAtas(), pixelMatrix, offsetX, offsetY, depth + 1, maxDepth);
     }
     if (node -> getGambarKananAtas()) {
         reconstructImageFrame(node -> getGambarKananAtas(), pixelMatrix, offsetX + node->getSizeX() /2, offsetY, depth + 1, maxDepth);
     }
     if (node -> getGambarKiriBawah()) {
         reconstructImageFrame(node -> getGambarKiriBawah(), pixelMatrix, offsetX, offsetY + node->getSizeY() /2, depth + 1, maxDepth);
     }
     if (node -> getGambarKananBawah()) {
         reconstructImageFrame(node -> getGambarKananBawah(), pixelMatrix, offsetX + node->getSizeX() /2, offsetY + node->getSizeY() /2, depth + 1, maxDepth);
     }
    }

}


int QuadTree::depthTree(QuadTree* node, int depth) {
    if (!node) return 0;
    
    if (node->isLeaf(node)) {
        return depth;
    }
    
    int leftTopDepth = depthTree(node->getGambarKiriAtas(), depth + 1);
    int rightTopDepth = depthTree(node->getGambarKananAtas(), depth + 1);
    int leftBottomDepth = depthTree(node->getGambarKiriBawah(), depth + 1);
    int rightBottomDepth = depthTree(node->getGambarKananBawah(), depth + 1);
    
    int maxDepth = max(max(leftTopDepth, rightTopDepth), max(leftBottomDepth, rightBottomDepth));
    
    return maxDepth;
}

long QuadTree::countNode(QuadTree* node, long nodes) {
    if (!node) return nodes;
    
    long count = nodes + 1;

    count = countNode(node->getGambarKiriAtas(), count);
    count = countNode(node->getGambarKananAtas(), count);
    count = countNode(node->getGambarKiriBawah(), count);
    count = countNode(node->getGambarKananBawah(), count);
    
    return count;
}