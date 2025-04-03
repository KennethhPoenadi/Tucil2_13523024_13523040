#include "include/image.hpp"
#include "include/quadtree.hpp"
#include "include/utils.hpp"
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

    int methodInput;
    cout << "Pilih metode error:\n";
    cout << "1: Variance\n2: MPD\n3: MAD\n4: Entropy\n5: SSIM\n";
    cout << "Pilihan: ";
    cin >> methodInput;

    double threshold;
    cout << "Masukkan threshold error: ";
    cin >> threshold;

    int minX, minY;
    cout << "Masukkan minimum block size X: ";
    cin >> minX;
    cout << "Masukkan minimum block size Y: ";
    cin >> minY;

    string outputFilename;
    cout << "Masukkan nama file gambar output: ";
    cin.ignore();
    getline(cin, outputFilename);

    auto start = chrono::high_resolution_clock::now();
    
    bool useVariance = false, useMPD = false, useMAD = false, useEntropy = false, useSSIM = false;
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
    
    const vector<vector<Pixel>>* pixelMatrix = &img.pixels;
    QuadTree* root = QuadTree::buildQuadTree(pixelMatrix, 0, 0, img.width, img.height, minX, minY, threshold, useVariance, useMPD, useMAD, useEntropy, useSSIM);

    vector<vector<Pixel>> reconstructImageMatrix(img.height, vector<Pixel>(img.width));
    root->reconstructImage(root, reconstructImageMatrix, 0, 0);

    string ext = getFileExtension(filename);

    saveReconstructedImage(outputFilename, reconstructImageMatrix, ext);
    
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast <chrono::milliseconds>(end-start);
    cout << "---------------------------------" << endl;
    cout << "Waktu kompresi      : " << duration.count() << " ms" <<endl;

    double outputFileSize = getFileSize(outputFilename + "." + ext);

    cout << "Ukuran file input   : " << inputFileSize << " KB\n";
    cout << "Ukuran file output  : " << outputFileSize << " KB\n";
    double compressionPerc = calculateCompression(inputFileSize, outputFileSize);
    cout << "Persentase kompresi : " << compressionPerc << " %\n";

    return 0;
}
