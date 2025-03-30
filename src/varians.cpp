#include "include/quadtree.hpp"
#include <cmath>

double calculateMean(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {//berdasarkan color channel yg dia mau
    double sum = 0.0;
    int count = 0;
    
    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix)[j].size()); i++) {
            if (colorChannel == 0) // Red
                sum += (*matrix)[j][i].r;
            else if (colorChannel == 1) // Green
                sum += (*matrix)[j][i].g;
            else if (colorChannel == 2) // Blue
                sum += (*matrix)[j][i].b;
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}


/* Varians */
double calculateVariance(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {//berdasarkan color channel yg di mau
    double mean = calculateMean(matrix, x, y, sizeX, sizeY, colorChannel);
    
    double sum = 0.0;
    int count = 0;
    
    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix)[j].size()); i++) {
            double pixelValue;
            if (colorChannel == 0) // Red
                pixelValue = (*matrix)[j][i].r;
            else if (colorChannel == 1) // Green
                pixelValue = (*matrix)[j][i].g;
            else // Blue
                pixelValue = (*matrix)[j][i].b;
            
            sum += (pixelValue - mean) * (pixelValue - mean);
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double calculateRGBVariance(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {//total
    double varianceR = calculateVariance(matrix, x, y, sizeX, sizeY, 0);
    double varianceG = calculateVariance(matrix, x, y, sizeX, sizeY, 1);
    double varianceB = calculateVariance(matrix, x, y, sizeX, sizeY, 2);
    
    return (varianceR + varianceG + varianceB) / 3.0;
}

/* Varians */

/* MAD */



/* MAD */