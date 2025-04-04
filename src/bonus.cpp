#include "include/bonus.hpp"
#include "include/quadtree.hpp"
#include "include/utils.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

pair<int, double> Kompresi::findOptimalParameters(
    const Image& img, 
    double targetCompressionRate,
    const string& filename, 
    const string& outputBase, 
    bool useSSIM
) {
    double inputFileSize = getFileSize(filename);
    int minSize = 1;
    double bestThreshold = 0.0;
    double closestCompressionRate = 0.0;
    double minDifference = 100.0; 
    double previousCompressionRate = -1.0; 
    
    int minDimension = min(img.width, img.height);
    int maxBlockSize = 1;
    while (maxBlockSize * 2 <= minDimension) {
        maxBlockSize *= 2;
    }
    
    for (int blockSize = maxBlockSize; blockSize >= 1; blockSize /= 2) {
        cout << "Block size: " << blockSize << "x" << blockSize << endl;
        
        previousCompressionRate = -1.0;
        
        double startThreshold = useSSIM ? 0.9 : 50.0;
        double endThreshold = useSSIM ? 0.1 : 5.0;
        double step = useSSIM ? -0.1 : -5.0;
        
        for (double threshold = startThreshold; 
             useSSIM ? (threshold >= endThreshold) : (threshold >= endThreshold); 
             threshold += step) {
            
            string tempOutputFilename = outputBase + "_temp";
            const vector<vector<Pixel>>* pixelMatrix = &img.pixels;
            
            QuadTree* root = QuadTree::buildQuadTree(
                pixelMatrix, 0, 0, img.width, img.height, 
                blockSize, blockSize, threshold, 
                false, false, false, false, useSSIM
            );
            
            vector<vector<Pixel>> reconstructImageMatrix(img.height, vector<Pixel>(img.width));
            root->reconstructImage(root, reconstructImageMatrix, 0, 0);
            
            string ext = getFileExtension(filename);
            saveReconstructedImage(tempOutputFilename, reconstructImageMatrix, ext);
            
            double outputFileSize = getFileSize(tempOutputFilename + "." + ext);
            double compressionRate = calculateCompression(inputFileSize, outputFileSize);
            
            cout << "  Threshold: " << threshold 
                 << ", Compression: " << compressionRate << "%" << endl;

            //heuristik buat ngecek dia sama kek sebelumnya ato ga, soalnya kalo untuk block size tinggi sering mirip.
            if (previousCompressionRate >= 0 && fabs(compressionRate - previousCompressionRate) < 0.01) {
                cout << "  Compression rate unchanged from previous threshold. Skipping to next block size." << endl;
                string tempFile = tempOutputFilename + "." + ext;
                remove(tempFile.c_str());
                delete root;
                break; 
            }
            //heuristik buat ngecek dia sama kek sebelumnya ato ga, soalnya kalo untuk block size tinggi sering mirip.
            previousCompressionRate = compressionRate;
            
            double difference = abs(compressionRate - targetCompressionRate);
            if (difference < minDifference) {
                minDifference = difference;
                closestCompressionRate = compressionRate;
                minSize = blockSize;
                bestThreshold = threshold;
                
                //toleransi kebedaan ya 1.5 aja
                if (difference < 1.5) {
                    cout << "Telah Ditemukan parameters within 1.5% of target compression rate." << endl;
                    string tempFile = tempOutputFilename + "." + ext;
                    remove(tempFile.c_str());
                    delete root;
                    return make_pair(minSize, bestThreshold);
                }
            }
            
            string tempFile = tempOutputFilename + "." + ext;
            remove(tempFile.c_str());
            delete root;
        }
    }
    
    cout << "Best approximation found: " << closestCompressionRate 
         << "% (difference: " << minDifference << "%)" << endl;
    
    return make_pair(minSize, bestThreshold);
}