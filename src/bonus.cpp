#include "include/bonus.hpp"
#include "include/quadtree.hpp"
#include "include/utils.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

double Kompresi::testCompression(
    const Image& img,
    double blockSize,
    double threshold,
    double inputFileSize,
    const string& filename,
    const string& outputBase,
    bool useSSIM
) {
    string tempOutputFilename = outputBase + "_temp";
    const vector<vector<Pixel>>* pixelMatrix = &img.pixels;

    QuadTree* root = QuadTree::buildQuadTree(
        pixelMatrix, 0.0, 0.0, static_cast<double>(img.width), static_cast<double>(img.height), 
        blockSize, threshold, 
        false, false, false, false, useSSIM
    );

    vector<vector<Pixel>> reconstructImageMatrix(img.height, vector<Pixel>(img.width));
    root->reconstructImage(root, reconstructImageMatrix, 0.0, 0.0);

    string ext = getFileExtension(filename);
    saveReconstructedImage(tempOutputFilename, reconstructImageMatrix, ext);

    double outputFileSize = getFileSize(tempOutputFilename + "." + ext);
    double compressionRate = calculateCompression(inputFileSize, outputFileSize);

    remove((tempOutputFilename + "." + ext).c_str());
    delete root;

    return compressionRate;
}

pair<double, double> Kompresi::findOptimalParameters(
    const Image& img, 
    double targetCompressionRate,
    const string& filename, 
    const string& outputBase, 
    bool useSSIM
) {
    double inputFileSize = getFileSize(filename);

    double minBlockSize = 4.0;
    double bestThreshold = 0.0;
    double closestCompressionRate = 0.0;
    double minDifference = 100.0;

    int minDimension = min(img.width, img.height);
    double maxBlockSize = 1.0;
    while (maxBlockSize * 8 <= minDimension) {
        maxBlockSize *= 8;
    }

    //caching hasil kompresi untuk menghindari perhitungan berulang
    struct CacheEntry {
        double blockSize;
        double threshold;
        double compressionRate;
    };
    vector<CacheEntry> cache;

    //loop dari ukuran blok paling gede ke paling kecil (dibagi 44 tiap iterasi)
    for (double blockSize = maxBlockSize; blockSize >= 4.0; blockSize /= 4) {
        cout << "Block size: " << blockSize << endl;
        
        double startThreshold =  0.1;
        double endThreshold = 0.9 ;

        double compressionRate = testCompression(img, blockSize, startThreshold, 
                                              inputFileSize, filename, outputBase, useSSIM);
        cache.push_back({blockSize, startThreshold, compressionRate});
        
        cout << " Threshold: " << startThreshold 
             << ", Compression: " << compressionRate << "%" << endl;
        
        //jika hasil jauh dari target (lebih dari 10%), skip block size ini
        if (abs(compressionRate - targetCompressionRate) > 10.0) {
            cout << "  Skip. Masi Jauh dari target" << endl;
            continue;
        }
        
        double difference = abs(compressionRate - targetCompressionRate);
        if (difference < minDifference) {
            minDifference = difference;
            closestCompressionRate = compressionRate;
            minBlockSize = blockSize;
            bestThreshold = startThreshold;
        }
        
        double endCompressionRate = testCompression(img, blockSize, endThreshold, 
                                                inputFileSize, filename, outputBase, useSSIM);
        cache.push_back({blockSize, endThreshold, endCompressionRate});
        
        cout << "  End test - Threshold: " << endThreshold 
             << ", Compression: " << endCompressionRate << "%" << endl;
        
        difference = abs(endCompressionRate - targetCompressionRate);
        if (difference < minDifference) {
            minDifference = difference;
            closestCompressionRate = endCompressionRate;
            minBlockSize = blockSize;
            bestThreshold = endThreshold;
        }
        
        // Jika kedua threshold menghasilkan kompresi di sisi yang berbeda dari target,
        // maka kita bisa menemukan nilai di antaranya
        bool shouldSearch = false;
        
        if ((compressionRate < targetCompressionRate && endCompressionRate > targetCompressionRate) ||
            (compressionRate > targetCompressionRate && endCompressionRate < targetCompressionRate)) {
            shouldSearch = true;
        }
        
        if (!shouldSearch) {
            if (minDifference < 5.0) {
                cout << "  Dapat yang toleransinya <= 5%" << endl;
                return make_pair(minBlockSize, bestThreshold);
            }
            continue;
        }
        
        double step = 0.1;
        
        double currentThreshold = startThreshold;
        if (abs(endCompressionRate - targetCompressionRate) < abs(compressionRate - targetCompressionRate)) {
            currentThreshold = endThreshold - step;
        } else {
            currentThreshold = startThreshold + step;
        }
        
        while (currentThreshold > startThreshold && currentThreshold < endThreshold) {
            double currentCompressionRate = testCompression(img, blockSize, currentThreshold, 
                                                        inputFileSize, filename, outputBase, useSSIM);
            cache.push_back({blockSize, currentThreshold, currentCompressionRate});
            
            cout << "  Testing - Threshold: " << currentThreshold 
                 << ", Compression: " << currentCompressionRate << "%" << endl;
            
            difference = abs(currentCompressionRate - targetCompressionRate);
            if (difference < minDifference) {
                minDifference = difference;
                closestCompressionRate = currentCompressionRate;
                minBlockSize = blockSize;
                bestThreshold = currentThreshold;
                
                if (difference < 5.0) {
                    cout << "  Didapatkan yang percentagenya (< 5% difference)" << endl;
                    return make_pair(minBlockSize, bestThreshold);
                }
            }
            
            //nentuin arah pencarian berikutnya
            if ((useSSIM && currentCompressionRate < targetCompressionRate) || 
                (!useSSIM && currentCompressionRate > targetCompressionRate)) {
                currentThreshold += step;
            } else {
                currentThreshold -= step;
            }
        }
    }

    cout << "Aproksimasi Terbaik: " << closestCompressionRate 
         << "% (difference: " << minDifference << "%)" << endl;

    return make_pair(minBlockSize, bestThreshold);
}