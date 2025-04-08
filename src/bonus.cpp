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
    while (maxBlockSize * 2 <= minDimension) {
        maxBlockSize *= 2;
    }

    //caching hasil kompresi untuk menghindari perhitungan berulang
    struct CacheEntry {
        double blockSize;
        double threshold;
        double compressionRate;
    };
    vector<CacheEntry> cache;

    //loop dari ukuran blok paling gede ke paling kecil (dibagi 2 tiap iterasi)
    for (double blockSize = maxBlockSize; blockSize >= 4.0; blockSize /= 2) {
        cout << "Block size: " << blockSize << endl;
        
        //set range threshold awal, beda kalau pake SSIM
        //diubah sesuai permintaan, mulai dari 0.1
        double startThreshold = useSSIM ? 0.1 : 5.0;
        double endThreshold = useSSIM ? 0.9 : 50.0;

        //test threshold pertengahan dulu untuk mendapatkan gambaran awal
        double midThreshold = (startThreshold + endThreshold) / 2;
        double midCompressionRate = -1.0;
        
        //cek cache dulu
        bool foundInCache = false;
        for (const auto& entry : cache) {
            if (entry.blockSize == blockSize && entry.threshold == midThreshold) {
                midCompressionRate = entry.compressionRate;
                foundInCache = true;
                break;
            }
        }
        
        if (!foundInCache) {
            midCompressionRate = testCompression(img, blockSize, midThreshold, 
                                                inputFileSize, filename, outputBase, useSSIM);
            cache.push_back({blockSize, midThreshold, midCompressionRate});
        }
        
        cout << "  Initial test - Threshold: " << midThreshold 
             << ", Compression: " << midCompressionRate << "%" << endl;
        
        double difference = abs(midCompressionRate - targetCompressionRate);
        if (difference < minDifference) {
            minDifference = difference;
            closestCompressionRate = midCompressionRate;
            minBlockSize = blockSize;
            bestThreshold = midThreshold;
            
            if (difference < 5) {
                cout << "Telah Ditemukan parameters dengan perbedaan 5% of target compression rate." << endl;
                return make_pair(minBlockSize, bestThreshold);
            }
        }
        
        // Tentukan arah pencarian - gunakan binary search
        double lowerBound = startThreshold;
        double upperBound = endThreshold;
        
        if (midCompressionRate > targetCompressionRate) {
            //kompresi terlalu tinggi, kurangi threshold (untuk SSIM: tingkatkan threshold)
            if (useSSIM) {
                lowerBound = midThreshold;
            } else {
                upperBound = midThreshold;
            }
        } else {
            //kompresi terlalu rendah, tingkatkan threshold (untuk SSIM: kurangi threshold)
            if (useSSIM) {
                upperBound = midThreshold;
            } else {
                lowerBound = midThreshold;
            }
        }
        
        for (int iteration = 0; iteration < 5; iteration++) {  // Batasi iterasi
            double nextThreshold = (lowerBound + upperBound) / 2;
            
            if (fabs(nextThreshold - midThreshold) < (useSSIM ? 0.01 : 0.5)) {
                break;
            }
            
            double compressionRate = -1.0;
            foundInCache = false;
            
            for (const auto& entry : cache) {
                if (entry.blockSize == blockSize && fabs(entry.threshold - nextThreshold) < 0.001) {
                    compressionRate = entry.compressionRate;
                    foundInCache = true;
                    break;
                }
            }
            
            if (!foundInCache) {
                compressionRate = testCompression(img, blockSize, nextThreshold, 
                                                inputFileSize, filename, outputBase, useSSIM);
                cache.push_back({blockSize, nextThreshold, compressionRate});
            }
            
            cout << "  Binary search - Threshold: " << nextThreshold 
                 << ", Compression: " << compressionRate << "%" << endl;
            
            difference = abs(compressionRate - targetCompressionRate);
            if (difference < minDifference) {
                minDifference = difference;
                closestCompressionRate = compressionRate;
                minBlockSize = blockSize;
                bestThreshold = nextThreshold;
                
                if (difference < 1.5) {
                    cout << "Telah Ditemukan parameters dengan perbedaan 1.5% of target compression rate." << endl;
                    return make_pair(minBlockSize, bestThreshold);
                }
            }
            
            if ((useSSIM && compressionRate > targetCompressionRate) || 
                (!useSSIM && compressionRate < targetCompressionRate)) {
                lowerBound = nextThreshold;
            } else {
                upperBound = nextThreshold;
            }
            
            //jika sudah cukup dekat dengan target, lakukan pencarian yang lebih halus
            if (difference < 5.0) {
                break;
            }
        }
        
        //fine tuning - cari dengan langkah lebih kecil di sekitar threshold terbaik
        double fineStep = useSSIM ? 0.01 : 0.5;
        double fineRange = useSSIM ? 0.05 : 2.5;
        
        double fineLowerBound = max(startThreshold, bestThreshold - fineRange);
        double fineUpperBound = min(endThreshold, bestThreshold + fineRange);
        
        cout << " Fine tuning dari range " << fineLowerBound << " sampai " << fineUpperBound 
             << " dengan step " << fineStep << endl;
        
        for (double threshold = fineLowerBound; threshold <= fineUpperBound; threshold += fineStep) {
            //skip jika threshold terlalu dekat dengan yang sudah diuji
            bool skipThisThreshold = false;
            for (const auto& entry : cache) {
                if (entry.blockSize == blockSize && fabs(entry.threshold - threshold) < (fineStep * 0.9)) {
                    skipThisThreshold = true;
                    break;
                }
            }
            
            if (skipThisThreshold) continue;
            
            double compressionRate = testCompression(img, blockSize, threshold, 
                                                  inputFileSize, filename, outputBase, useSSIM);
            cache.push_back({blockSize, threshold, compressionRate});
            
            cout << "  Fine tuning - Threshold: " << threshold 
                 << ", Compression: " << compressionRate << "%" << endl;
            
            difference = abs(compressionRate - targetCompressionRate);
            if (difference < minDifference) {
                minDifference = difference;
                closestCompressionRate = compressionRate;
                minBlockSize = blockSize;
                bestThreshold = threshold;
                
                if (difference < 5) {
                    cout << "Telah Ditemukan parameters dengan perbedaan 5% of target compression rate." << endl;
                    return make_pair(minBlockSize, bestThreshold);
                }
            }
        }
    }

    cout << "Best approximation found: " << closestCompressionRate 
         << "% (difference: " << minDifference << "%)" << endl;

    return make_pair(minBlockSize, bestThreshold);
}