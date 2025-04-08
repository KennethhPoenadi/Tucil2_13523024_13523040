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

    // Clean up
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
    //ambil ukuran file input buat hitung compression rate nanti
    double inputFileSize = getFileSize(filename);

    //set nilai default
    double minBlockSize = 4.0;
    double bestThreshold = 0.0;
    double closestCompressionRate = 0.0;
    double minDifference = 100.0; // Biar ada angka pembanding awal

    //cari ukuran blok max yang masih pangkat 2 dari ukuran gambar paling kecil
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

    //loop dari ukuran blok paling gede ke paling kecil (dibagi 8 tiap iterasi)
    for (double blockSize = maxBlockSize; blockSize >= 4.0; blockSize /= 4) {
        cout << "Block size: " << blockSize << endl;
        
        //set range threshold awal, beda kalau pake SSIM
        //diubah sesuai permintaan, mulai dari 0.1
        double startThreshold = useSSIM ? 0.1 : 5.0;
        double endThreshold = useSSIM ? 0.9 : 50.0;

        // Test threshold awal
        double compressionRate = testCompression(img, blockSize, startThreshold, 
                                              inputFileSize, filename, outputBase, useSSIM);
        cache.push_back({blockSize, startThreshold, compressionRate});
        
        cout << " Threshold: " << startThreshold 
             << ", Compression: " << compressionRate << "%" << endl;
        
        // Jika hasil jauh dari target (lebih dari 10%), skip block size ini
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
        
        // Test threshold akhir
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
            // Jika tidak perlu search, periksa apakah salah satu hasil sudah cukup baik
            if (minDifference < 5.0) {
                cout << "  Dapat yang toleransinya <= 5%" << endl;
                return make_pair(minBlockSize, bestThreshold);
            }
            continue;
        }
        
        // Lakukan pencarian dengan step size 0.1 (atau 5.0 untuk non-SSIM)
        double step = useSSIM ? 0.1 : 5.0;
        
        // Mulai dari threshold yang lebih dekat ke target
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
                
                // Jika hasil sudah cukup dekat dengan target, selesai
                if (difference < 5.0) {
                    cout << "  Found very good result (< 5% difference)" << endl;
                    return make_pair(minBlockSize, bestThreshold);
                }
            }
            
            // Tentukan arah pencarian berikutnya
            if ((useSSIM && currentCompressionRate < targetCompressionRate) || 
                (!useSSIM && currentCompressionRate > targetCompressionRate)) {
                currentThreshold += step;
            } else {
                currentThreshold -= step;
            }
        }
    }

    cout << "Best approximation found: " << closestCompressionRate 
         << "% (difference: " << minDifference << "%)" << endl;

    return make_pair(minBlockSize, bestThreshold);
}