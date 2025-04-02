#ifndef VARIANS_HPP
#define VARIANS_HPP

#include "quadtree.hpp"

using namespace std;

double calculateMean(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateVariance(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateRGBVariance(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY);

double calculateMad(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateRGBMad(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY);

double calculateMPDMaxMin(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateMPD(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY);

double calculateEntropy(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateRGBEntropyTotal(const vector<vector<Pixel>>* matrix, int x, int y, int sizeX, int sizeY);

Pixel getAverageColor(const vector<vector<Pixel>>* mat, int x, int y, int sizeX, int sizeY);

double calculateCovariance(const vector<vector<Pixel>>* matrix1, const vector<vector<Pixel>>* matrix2, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateSSIM(const vector<vector<Pixel>>* original, const vector<vector<Pixel>>* compressed, int x, int y, int sizeX, int sizeY, int colorChannel);
double calculateSSIM_RGB(const vector<vector<Pixel>>* original, const vector<vector<Pixel>>* compressed, int x, int y, int sizeX, int sizeY);

#endif
