#include "quadtree.hpp"
#include <iostream>
using namespace std;

double ratarata(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) { //buat itung rata-rata dari channel yang sesuai dengan int color chanel
    double sum = 0.0;
    int count = 0;
    
    for (int j = y; j < y + sizeY && j < matrix->size(); j++) {
        for (int i = x; i < x + sizeX && i < (*matrix)[j].size(); i++) {
            if (colorChannel == 0) //R
                sum += (*matrix)[j][i].r;
            else if (colorChannel == 1) //G
                sum += (*matrix)[j][i].g;
            else if (colorChannel == 2) //B
                sum += (*matrix)[j][i].b;
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double variansBlok(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel) {//varians dari color channel yg dimau y
    double mean = ratarata(matrix, x, y, sizeX, sizeY, colorChannel);
    
    double sum = 0.0;
    int count = 0;
    
    for (int j = y; j < y + sizeY && j < matrix->size(); j++) {
        for (int i = x; i < x + sizeX && i < (*matrix)[j].size(); i++) {
            double pixelValue;
            if (colorChannel == 0) //red
                pixelValue = (*matrix)[j][i].r;
            else if (colorChannel == 1) //green
                pixelValue = (*matrix)[j][i].g;
            else //blue
                pixelValue = (*matrix)[j][i].b;
            
            sum += (pixelValue - mean) * (pixelValue - mean);
            count++;
        }
    }
    
    return (count > 0) ? (sum / count) : 0.0;
}

double calculateRGBTotal(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY) {
    double varianceR = variansBlok(matrix, x, y, sizeX, sizeY, 0);
    double varianceG = variansBlok(matrix, x, y, sizeX, sizeY, 1);
    double varianceB = variansBlok(matrix, x, y, sizeX, sizeY, 2);
    
    return (varianceR + varianceG + varianceB) / 3.0;
}