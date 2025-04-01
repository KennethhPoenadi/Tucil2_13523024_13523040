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

double calculateMad(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {//berdasarkan color channel yg di mau
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
            
            sum += (pixelValue - mean);
            if (sum < 0) {
                sum *= -1;
            }
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double calculateRGBMad(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {//total
    double MadR = calculateMad(matrix, x, y, sizeX, sizeY, 0);
    double MadG = calculateMad(matrix, x, y, sizeX, sizeY, 1);
    double MadB = calculateMad(matrix, x, y, sizeX, sizeY, 2);
    
    return (MadR + MadG + MadB) / 3.0;
}

/* MAD */

/* Max Pixel Difference (MPD) */

double calculateMPDMaxMin(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel){
    double maxnya = -1;
    double minnya = 256;

    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix)[j].size()); i++) {
            double pixelValue;
            if (colorChannel == 0) // Red
                pixelValue = (*matrix)[j][i].r;
            else if (colorChannel == 1) // Green
                pixelValue = (*matrix)[j][i].g;
            else // Blue
                pixelValue = (*matrix)[j][i].b;
            
            if (pixelValue > maxnya) {
                maxnya = pixelValue;
            }

            if (pixelValue < minnya) {
                minnya = pixelValue;
            }
        }
    }

    return (maxnya - minnya);
}

double calculateMPD(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {
    double diffR = calculateMPDMaxMin(matrix, x, y, sizeX, sizeY, 0);
    double diffG = calculateMPDMaxMin(matrix, x, y, sizeX, sizeY, 1);
    double diffB = calculateMPDMaxMin(matrix, x, y, sizeX, sizeY, 2);
    
    return (diffR + diffG + diffB) / 3.0;
}

/* Max Pixel Difference (MPD) */

/* Entropy */


double calculateEntropy(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {
    //pertama, bikin dl histogram isinya pixel value yang possible
    vector<int> histogram(256, 0);
    int totalPixels = 0;
    
    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix)[j].size()); i++) {
            int pixelValue;
            if (colorChannel == 0) // Red
                pixelValue = (*matrix)[j][i].r;
            else if (colorChannel == 1) // Green
                pixelValue = (*matrix)[j][i].g;
            else // Blue
                pixelValue = (*matrix)[j][i].b;
            
            histogram[pixelValue]++;
            totalPixels++;
        }
    }
    
    // H = -sum(p(i) * log2(p(i)))
    double entropy = 0.0;
    
    for (int i = 0; i < 256; i++) {
        if (histogram[i] > 0) {
            double probability = static_cast<double>(histogram[i]) / totalPixels;
            entropy -= probability * log2(probability);
        }
    }
    
    return entropy;
}

double calculateRGBEntropyTotal(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {
    double entropyR = calculateEntropy(matrix, x, y, sizeX, sizeY, 0);
    double entropyG = calculateEntropy(matrix, x, y, sizeX, sizeY, 1);
    double entropyB = calculateEntropy(matrix, x, y, sizeX, sizeY, 2);
    
    return (entropyR + entropyG + entropyB) / 3.0;
}

/* Entropy */

/* Normalisasi */

Pixel getAverageColor(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY) {
    long long totalR = 0, totalG = 0, totalB = 0;
    int totalPixels = sizeX * sizeY;

    for (int i = y; i < y + sizeY; ++i) {
        for (int j = x; j < x + sizeX; ++j) {
            totalR += (*mat)[i][j].r;
            totalG += (*mat)[i][j].g;
            totalB += (*mat)[i][j].b;
        }
    }

    Pixel avgColor;
    avgColor.r = static_cast<unsigned char>(totalR / totalPixels);
    avgColor.g = static_cast<unsigned char>(totalG / totalPixels);
    avgColor.b = static_cast<unsigned char>(totalB / totalPixels);

    return avgColor;
}

/* Normalisasi */