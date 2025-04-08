#include "include/quadtree.hpp"
#include <cmath>

double calculateMean(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {
    double sum = 0.0;
    int count = 0;
    
    if (x < 0 || y < 0 || sizeX <= 0 || sizeY <= 0 || y >= static_cast<int>(matrix->size())) {
        return 0.0;
    }
    
    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix)[j].size()); i++) {
            double pixelValue = 0.0;
            if (colorChannel == 0) // Red
                pixelValue = static_cast<double>((*matrix)[j][i].r);
            else if (colorChannel == 1) // Green
                pixelValue = static_cast<double>((*matrix)[j][i].g);
            else if (colorChannel == 2) // Blue
                pixelValue = static_cast<double>((*matrix)[j][i].b);
            else if (colorChannel == 3) // Alpha
                pixelValue = static_cast<double>((*matrix)[j][i].a);
            
            sum += pixelValue;
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

/* Varians */

double calculateVariance(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {
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
            else if (colorChannel == 2) // Blue
                pixelValue = (*matrix)[j][i].b;
            else if (colorChannel == 3) // Alpha
                pixelValue = (*matrix)[j][i].a;
            
            sum += (pixelValue - mean) * (pixelValue - mean);
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double calculateRGBAVariance(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {
    double varianceR = calculateVariance(matrix, x, y, sizeX, sizeY, 0);
    double varianceG = calculateVariance(matrix, x, y, sizeX, sizeY, 1);
    double varianceB = calculateVariance(matrix, x, y, sizeX, sizeY, 2);
    double varianceA = calculateVariance(matrix, x, y, sizeX, sizeY, 3);
    
    return (varianceR + varianceG + varianceB + varianceA) / 4.0;
}

/* Varians */

/* MAD */

double calculateMad(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {
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
            else if (colorChannel == 2) // Blue
                pixelValue = (*matrix)[j][i].b;
            else if (colorChannel == 3) // Alpha
                pixelValue = (*matrix)[j][i].a;
            
            double diff = pixelValue - mean;
            sum += (diff < 0) ? -diff : diff;
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double calculateRGBAMad(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {
    double MadR = calculateMad(matrix, x, y, sizeX, sizeY, 0);
    double MadG = calculateMad(matrix, x, y, sizeX, sizeY, 1);
    double MadB = calculateMad(matrix, x, y, sizeX, sizeY, 2);
    double MadA = calculateMad(matrix, x, y, sizeX, sizeY, 3);
    
    return (MadR + MadG + MadB + MadA) / 4.0;
}

/* MAD */

/* Max Pixel Difference (MPD) */

double calculateMPDMaxMin(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {
    double maxnya = -1;
    double minnya = 256;

    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix)[j].size()); i++) {
            double pixelValue;
            if (colorChannel == 0) // Red
                pixelValue = (*matrix)[j][i].r;
            else if (colorChannel == 1) // Green
                pixelValue = (*matrix)[j][i].g;
            else if (colorChannel == 2) // Blue
                pixelValue = (*matrix)[j][i].b;
            else if (colorChannel == 3) // Alpha
                pixelValue = (*matrix)[j][i].a;
            
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
    double diffA = calculateMPDMaxMin(matrix, x, y, sizeX, sizeY, 3);
    
    return (diffR + diffG + diffB + diffA) / 4.0;
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
            else if (colorChannel == 2) // Blue
                pixelValue = (*matrix)[j][i].b;
            else if (colorChannel == 3) // Alpha
                pixelValue = (*matrix)[j][i].a;
            
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

double calculateRGBAEntropyTotal(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {
    double entropyR = calculateEntropy(matrix, x, y, sizeX, sizeY, 0);
    double entropyG = calculateEntropy(matrix, x, y, sizeX, sizeY, 1);
    double entropyB = calculateEntropy(matrix, x, y, sizeX, sizeY, 2);
    double entropyA = calculateEntropy(matrix, x, y, sizeX, sizeY, 3);
    
    return (entropyR + entropyG + entropyB + entropyA) / 4.0;
}

/* Entropy */

/* Normalisasi */

Pixel getAverageColor(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY) {
    long long totalR = 0, totalG = 0, totalB = 0, totalA = 0;
    int totalPixels = sizeX * sizeY;

    for (int i = y; i < y + sizeY; ++i) {
        for (int j = x; j < x + sizeX; ++j) {
            totalR += (*mat)[i][j].r;
            totalG += (*mat)[i][j].g;
            totalB += (*mat)[i][j].b;
            totalA += (*mat)[i][j].a;
        }
    }

    Pixel avgColor;
    avgColor.r = static_cast<unsigned char>(totalR / totalPixels);
    avgColor.g = static_cast<unsigned char>(totalG / totalPixels);
    avgColor.b = static_cast<unsigned char>(totalB / totalPixels);
    avgColor.a = static_cast<unsigned char>(totalA / totalPixels);

    return avgColor;
}

/* Normalisasi */

/* SSIM (BONUS) */

const double C1 = (0.01 * 255) * (0.01 * 255);
const double C2 = (0.03 * 255) * (0.03 * 255);

double calculateCovariance(const vector<vector<Pixel>>* matrix1, const vector<vector<Pixel>>* matrix2, 
                          int x, int y, int sizeX, int sizeY, int colorChannel) {
    // Make sure we're working with integers for array indices
    double mean1 = calculateMean(matrix1, x, y, sizeX, sizeY, colorChannel);
    double mean2 = calculateMean(matrix2, x, y, sizeX, sizeY, colorChannel);
    double sum = 0.0;
    int count = 0;

    for (int j = y; j < y + sizeY && j < static_cast<int>(matrix1->size()) && j < static_cast<int>(matrix2->size()); j++) {
        for (int i = x; i < x + sizeX && i < static_cast<int>((*matrix1)[j].size()) && i < static_cast<int>((*matrix2)[j].size()); i++) {
            double pixelValue1, pixelValue2;
            
            if (colorChannel == 0) { // Red
                pixelValue1 = static_cast<double>((*matrix1)[j][i].r);
                pixelValue2 = static_cast<double>((*matrix2)[j][i].r);
            } else if (colorChannel == 1) { // Green
                pixelValue1 = static_cast<double>((*matrix1)[j][i].g);
                pixelValue2 = static_cast<double>((*matrix2)[j][i].g);
            } else if (colorChannel == 2) { // Blue
                pixelValue1 = static_cast<double>((*matrix1)[j][i].b);
                pixelValue2 = static_cast<double>((*matrix2)[j][i].b);
            } else { // Alpha
                pixelValue1 = static_cast<double>((*matrix1)[j][i].a);
                pixelValue2 = static_cast<double>((*matrix2)[j][i].a);
            }
            
            sum += (pixelValue1 - mean1) * (pixelValue2 - mean2);
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double calculateSSIM(const vector<vector<Pixel>>* original, const vector<vector<Pixel>>* compressed, 
                    int x, int y, int sizeX, int sizeY, int colorChannel) {
    if (x < 0 || y < 0 || sizeX <= 0 || sizeY <= 0 ||
        y >= static_cast<int>(original->size()) || y >= static_cast<int>(compressed->size())) {
        return 0.0;
    }
    
    double mu1 = calculateMean(original, x, y, sizeX, sizeY, colorChannel);
    double mu2 = calculateMean(compressed, x, y, sizeX, sizeY, colorChannel);
    
    double sigma1_sq = calculateVariance(original, x, y, sizeX, sizeY, colorChannel);
    double sigma2_sq = calculateVariance(compressed, x, y, sizeX, sizeY, colorChannel);
    
    double sigma12 = calculateCovariance(original, compressed, x, y, sizeX, sizeY, colorChannel);
    
    double numerator = (2 * mu1 * mu2 + C1) * (2 * sigma12 + C2);
    double denominator = (mu1 * mu1 + mu2 * mu2 + C1) * (sigma1_sq + sigma2_sq + C2);
    
    if (denominator < 1e-10) {
        return 1.0; 
    }
    
    double ssim = numerator / denominator;
    
    return max(0.0, min(1.0, ssim));
}

double calculateSSIM_RGB(const vector<vector<Pixel>>* original, const vector<vector<Pixel>>* compressed, 
                        int x, int y, int sizeX, int sizeY) {
    const double wR = 0.2125, wG = 0.7154, wB = 0.0721;
    double totalWeight = wR + wG + wB;
        
    double SSIM_R = calculateSSIM(original, compressed, x, y, sizeX, sizeY, 0);
    double SSIM_G = calculateSSIM(original, compressed, x, y, sizeX, sizeY, 1);
    double SSIM_B = calculateSSIM(original, compressed, x, y, sizeX, sizeY, 2);
    
    return (wR * SSIM_R + wG * SSIM_G + wB * SSIM_B) / totalWeight;
}

/* SSIM (BONUS) */