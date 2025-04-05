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
    //ambil ukuran file input buat hitung compression rate nanti
    double inputFileSize = getFileSize(filename);

    //set nilai default
    int minSize = 1;
    double bestThreshold = 0.0;
    double closestCompressionRate = 0.0;
    double minDifference = 100.0; //biar ada angka pembanding awal

    //cari ukuran blok max yang masih pangkat 2 dari ukuran gambar paling kecil
    int minDimension = min(img.width, img.height);
    int maxBlockSize = 1;
    while (maxBlockSize * 2 <= minDimension) {
        maxBlockSize *= 2;
    }

    //loop dari ukuran blok paling gede ke paling kecil (dibagi 2 tiap iterasi)
    for (int blockSize = maxBlockSize; blockSize >= 1; blockSize /= 2) {
        cout << "Block size: " << blockSize << "x" << blockSize << endl;
        
        double previousCompressionRate = -1.0; //buat ngecek perubahan compression rate
        
        //set range threshold awal, beda kalau pake SSIM
        double startThreshold = useSSIM ? 0.9 : 50.0;
        double endThreshold = useSSIM ? 0.1 : 5.0;
        double step = useSSIM ? -0.1 : -5.0;

        //variabel buat refined search kalo range udah ketemu
        double upperThreshold = startThreshold;
        double lowerThreshold = startThreshold + step;
        double upperCompression = -1.0;
        double lowerCompression = -1.0;
        bool foundRange = false;

        //loop nyari threshold terbaik dari range awal
        for (double threshold = startThreshold; threshold >= endThreshold; threshold += step) {
            string tempOutputFilename = outputBase + "_temp";
            const vector<vector<Pixel>>* pixelMatrix = &img.pixels;

            //bangun QuadTree pake parameter blok & threshold skrg
            QuadTree* root = QuadTree::buildQuadTree(
                pixelMatrix, 0, 0, img.width, img.height, 
                blockSize, blockSize, threshold, 
                false, false, false, false, useSSIM
            );

            //rekonstruksi gambar dari QuadTree
            vector<vector<Pixel>> reconstructImageMatrix(img.height, vector<Pixel>(img.width));
            root->reconstructImage(root, reconstructImageMatrix, 0, 0);

            //simpan gambar sementara
            string ext = getFileExtension(filename);
            saveReconstructedImage(tempOutputFilename, reconstructImageMatrix, ext);

            //hitung ukuran file output & compression rate
            double outputFileSize = getFileSize(tempOutputFilename + "." + ext);
            double compressionRate = calculateCompression(inputFileSize, outputFileSize);

            cout << "  Threshold: " << threshold 
                 << ", Compression: " << compressionRate << "%" << endl;

            //skip kalo perubahan compression rate kecil banget
            if (previousCompressionRate >= 0 && fabs(compressionRate - previousCompressionRate) < 0.01) {
                cout << " Skip. Compression Rate Beda Dikit. " << endl;
                remove((tempOutputFilename + "." + ext).c_str());
                delete root;
                break; 
            }

            //cek compression rate paling deket ke target
            double difference = abs(compressionRate - targetCompressionRate);
            if (difference < minDifference) {
                minDifference = difference;
                closestCompressionRate = compressionRate;
                minSize = blockSize;
                bestThreshold = threshold;

                //kalo udah deket banget langsung return
                if (difference < 1.5) {
                    cout << "Telah Ditemukan parameters dengan perbedaan 1.5% of target compression rate." << endl;
                    remove((tempOutputFilename + "." + ext).c_str());
                    delete root;
                    return make_pair(minSize, bestThreshold);
                }
            }

            //cek kalo udah nyebrang target compression rate
            if (previousCompressionRate > targetCompressionRate && compressionRate < targetCompressionRate) {
                //simpan range buat refined search
                cout << "Dapet Range Yang Ada target: " << previousCompressionRate 
                     << "%, current gives " << compressionRate << "%" << endl;
                upperThreshold = threshold - step;
                upperCompression = previousCompressionRate;
                lowerThreshold = threshold;
                lowerCompression = compressionRate;
                foundRange = true;
                remove((tempOutputFilename + "." + ext).c_str());
                delete root;
                break;
            } else if (compressionRate > targetCompressionRate) {
                upperThreshold = threshold;
                upperCompression = compressionRate;
            } else if (compressionRate < targetCompressionRate && upperCompression > targetCompressionRate) {
                lowerThreshold = threshold;
                lowerCompression = compressionRate;
                foundRange = true;
                remove((tempOutputFilename + "." + ext).c_str());
                delete root;
                break;
            }

            previousCompressionRate = compressionRate;
            remove((tempOutputFilename + "." + ext).c_str());
            delete root;
        }

        //kalo udah nemu range target, refine search pake step lebih kecil
        if (foundRange) {
            cout << " Cari dari range " << upperThreshold << " dan " << lowerThreshold 
                 << " (compression rates " << upperCompression << "% - " << lowerCompression << "%)" << endl;

            double fineStep = useSSIM ? -0.01 : -0.5;

            for (double threshold = upperThreshold; threshold >= lowerThreshold; threshold += fineStep) {
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

                cout << "  (cari 0.0..) - Threshold: " << threshold 
                     << ", Compression: " << compressionRate << "%" << endl;

                double difference = abs(compressionRate - targetCompressionRate);
                if (difference < minDifference) {
                    minDifference = difference;
                    closestCompressionRate = compressionRate;
                    minSize = blockSize;
                    bestThreshold = threshold;

                    if (difference < 4) {
                        cout << "Telah Ditemukan parameters dengan perbedaan 1.5% of target compression rate." << endl;
                        remove((tempOutputFilename + "." + ext).c_str());
                        delete root;
                        return make_pair(minSize, bestThreshold);
                    }
                }

                remove((tempOutputFilename + "." + ext).c_str());
                delete root;
            }
        }
    }

    cout << "Best approximation found: " << closestCompressionRate 
         << "% (difference: " << minDifference << "%)" << endl;

    return make_pair(minSize, bestThreshold);
}
