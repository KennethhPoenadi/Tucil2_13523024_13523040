#include "include/image.hpp" 
#include "include/quadtree.hpp" 
#include "include/utils.hpp" 
#include "include/bonus.hpp"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

int main() {
    string filename;
    cout << "Masukkan nama file gambar: ";
    getline(cin, filename);
    
    if (!checkFile(filename)) {
        cerr << "Error: File tidak ditemukan!" << endl;
        return 1;
    }
    
    Image img = loadImage(filename);
    if (img.width == 0 || img.height == 0) {
        cerr << "Error: Gagal memuat gambar!" << endl;
        return 1;
    }
    
    double inputFileSize = getFileSize(filename);
    cout << "Dimensi Gambar: " << img.width << " x " << img.height << "\n";
    
    int compressionChoice;
    cout << "Pilih metode kompresi:\n";
    cout << "1: Manual (pilih metode error dan threshold)\n";
    cout << "2: Otomatis (berdasarkan target persentase kompresi)\n";
    cout << "Pilihan: ";
    cin >> compressionChoice;
    
    string outputFilename;
    bool useVariance = false, useMPD = false, useMAD = false, useEntropy = false, useSSIM = false;
    int minX = 1, minY = 1;
    double threshold = 0.0;
    
    if (compressionChoice == 1) {
        int methodInput;
        cout << "Pilih metode error:\n";
        cout << "1: Variance\n2: MPD\n3: MAD\n4: Entropy\n5: SSIM" << endl;
        cout << "Pilihan: ";
        cin >> methodInput;
        
        cout << "Masukkan threshold error: ";
        cin >> threshold;
        
        cout << "Masukkan minimum block size X: ";
        cin >> minX;
        cout << "Masukkan minimum block size Y: ";
        cin >> minY;
        
        cout << "Masukkan nama file gambar output: ";
        cin.ignore();
        getline(cin, outputFilename);
        
        switch (methodInput) {
            case 1: useVariance = true; break;
            case 2: useMPD = true; break;
            case 3: useMAD = true; break;
            case 4: useEntropy = true; break;
            case 5: useSSIM = true; break;
            default: 
                cout << "Input tidak valid. Default ke Varians.\n";
                useVariance = true; 
                break;
        }
    } else if (compressionChoice == 2) {
        double targetCompressionRate;
        cout << "Masukkan target persentase kompresi (contoh: 60 untuk 60%): ";
        cin >> targetCompressionRate;
        
        int errorMethod;
        errorMethod = 2;
        
        if (errorMethod == 2) {
            useSSIM = true;
        } else {
            useVariance = true;
        }
        
        cout << "Masukkan nama file gambar output: ";
        cin.ignore();
        getline(cin, outputFilename);
        
        cout << "\nMencari parameter optimal untuk target kompresi " << targetCompressionRate << "%...\n";
        
        auto optimalParams = Kompresi::findOptimalParameters(
            img, targetCompressionRate, filename, outputFilename, useSSIM
        );
        
        minX = minY = optimalParams.first;
        threshold = optimalParams.second;
        
        cout << "\nParameter optimal ditemukan:\n";
        cout << "- Minimum block size: " << minX << "x" << minY << endl;
        cout << "- Threshold: " << threshold << endl;
    } else {
        cout << "Pilihan tidak valid. Program berhenti.\n";
        return 1;
    }
    
    auto start = chrono::high_resolution_clock::now();
    
    const vector<vector<Pixel>>* pixelMatrix = &img.pixels;
    QuadTree* root = QuadTree::buildQuadTree(
        pixelMatrix, 0, 0, img.width, img.height, 
        minX, minY, threshold, 
        useVariance, useMPD, useMAD, useEntropy, useSSIM
    );
    
    vector<vector<Pixel>> reconstructImageMatrix(img.height, vector<Pixel>(img.width));
    root->reconstructImage(root, reconstructImageMatrix, 0, 0);
    
    string ext = getFileExtension(filename);
    saveReconstructedImage(outputFilename, reconstructImageMatrix, ext);
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);
    
    cout << "---------------------------------" << endl;
    cout << "Waktu eksekusi : " << duration.count() << " ms" << endl;
    
    double outputFileSize = getFileSize(outputFilename + "." + ext);
    cout << "Ukuran file input : " << inputFileSize << " KB\n";
    cout << "Ukuran file output : " << outputFileSize << " KB\n";
    
    double compressionPerc = calculateCompression(inputFileSize, outputFileSize);
    cout << "Persentase kompresi : " << compressionPerc << " %\n";
    
    cout << "----------------------------------" << endl;
    cout << "Kedalaman Quadtree: " << QuadTree::depthTree(root) << endl;
    cout << "Jumlah simpul pada Tree: " << QuadTree::countNode(root) << endl;
    cout << "----------------------------------" << endl;
    
    delete root;
    return 0;
}