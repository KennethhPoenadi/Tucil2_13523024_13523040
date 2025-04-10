#include "include/image.hpp" 
#include "include/quadtree.hpp" 
#include "include/utils.hpp" 
#include "include/bonus.hpp"
#include "include/gifer.hpp"
#include <iostream>
#include <string>
#include <chrono>

using namespace std;

int main() {

    //buat waktu
    chrono::high_resolution_clock::time_point start;

    //input file
    string filename;
    cout << "======================================" << endl;
    cout << "WELCOME TO COMPRESS QUADTREE TERGACOR!" << endl;
    cout << "======================================" << endl;
    cout << "Silahkan masukkan path file gambar: ";
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
    cout << "======================================" << endl;
    cout << "       Dimensi Gambar: " << img.width << " x " << img.height << "" << endl;
    cout << "======================================" << endl;

    double targetCompressionRate;
    cout << "Masukkan target kompresi (0.01 - 1): ";
    cin >> targetCompressionRate;
    
    while (targetCompressionRate < 0 || targetCompressionRate > 1) {
        cout << "Target kompresi harus antara 0-1, silahkan ulangi input: ";
        cin >> targetCompressionRate;
    }
    
    string outputFilename;
    bool useVariance = false, useMPD = false, useMAD = false, useEntropy = false, useSSIM = false;
    double min;
    double threshold;
    char togifornottogif;
    string gifOutput;
    
    //jika targetCompressionRate 0, maka user input threshold dan min block size
    if (targetCompressionRate == 0) {
        int methodInput;
        cout << "======================================" << endl;
        cout << "Pilih metode error:" << endl;
        cout << "1: Variance\n2: MPD\n3: MAD\n4: Entropy\n5: SSIM" << endl;
        cout << "======================================" << endl;
        cout << "Pilihan: ";
        cin >> methodInput;
        while (methodInput < 1 || methodInput > 5) {
            cout << "Input antara 1 - 5, silahkan ulangi input: ";
            cin >> methodInput;
        }
        cout << "======================================" << endl;

        cout << "Masukkan threshold error: ";
        cin >> threshold;
        while (threshold <= 0) {
            cout << "Threshold tidak boleh 0 atau dibawah 0, silahkan ulangi input: ";
            cin >> threshold;
        }
        
        cout << "Masukkan minimum block size: ";
        cin >> min;
        while (min < 4 || min > (img.height * img.width)) {
            cout << "Ulangi input, luas blok minimum tidak boleh kurang dari 4 (minimal 2 x 2) atau melebihi luas gambar: ";
            cin >> min;
        }
        cout << "======================================" << endl;
        
        cout << "Masukkan path file gambar output: ";
        cin.ignore();
        getline(cin, outputFilename);

        cout << "Apakah anda ingin membuat GIF? (y/n): ";
        cin >> togifornottogif;

        if (togifornottogif == 'y' || togifornottogif == 'Y') {
            cout << "Masukkan path file GIF output: ";
            cin.ignore();
            getline(cin, gifOutput);
            
            gifOutput += ".gif";
        } else if (togifornottogif == 'n' || togifornottogif == 'N'){
            cout << "Tidak membuat gif" << endl;
        }
        //waktu dimulai

        start = chrono::high_resolution_clock::now();
        
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
    } else { //jika targetCompressionRate 1, maka masuk ke alur target %
        targetCompressionRate *= 100;
        
        useSSIM = true;
        
        cout << "Masukkan nama file gambar output: ";
        cin.ignore();
        getline(cin, outputFilename);
        cout << "======================================" << endl;
        cout << endl;

        cout << "Apakah anda ingin membuat GIF? (y/n): ";
        cin >> togifornottogif;

        if (togifornottogif == 'y' || togifornottogif == 'Y') {
             cout << "Masukkan path file GIF output: ";
             cin.ignore();
             getline(cin, gifOutput);
           
           gifOutput += ".gif";
        } else if (togifornottogif == 'n' || togifornottogif == 'N'){
            cout << "Tidak membuat gif" << endl;
        }
        
        cout << "\nMencari parameter optimal untuk target kompresi " << targetCompressionRate << "%...\n";

        //waktu dimulai
        start = chrono::high_resolution_clock::now();
        
        auto optimalParams = Kompresi::findOptimalParameters(
            img, targetCompressionRate, filename, outputFilename, useSSIM
        );
        min = optimalParams.first;   
        threshold = optimalParams.second;
        
        cout << "\nParameter optimal ditemukan:\n";
        cout << "- Block size: " << min << endl;
        cout << "- Threshold: " << threshold << endl;
    }
    
    
    const vector<vector<Pixel>>* pixelMatrix = &img.pixels;
    QuadTree* root = QuadTree::buildQuadTree(
        pixelMatrix, 0, 0, img.width, img.height, 
        min, threshold, 
        useVariance, useMPD, useMAD, useEntropy, useSSIM
    );
    
    //untuk menyimpan hasil rekonstruksi
    vector<vector<Pixel>> reconstructImageMatrix(img.height, vector<Pixel>(img.width));
    root->reconstructImage(root, reconstructImageMatrix, 0, 0);
    
    string ext = getFileExtension(filename);
    int maxDepth = QuadTree::depthTree(root);
    
    saveReconstructedImage(outputFilename, reconstructImageMatrix, ext);

    //kalau buat gif (masuk di timer)
    if (togifornottogif == 'y' || togifornottogif == 'Y') {
        gifMaker(img.width, img.height, gifOutput.c_str(), maxDepth, root, reconstructImageMatrix);
    }
    
    //buat timer ms
    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-start);

    //output terminal
    
    double outputFileSize = getFileSize(outputFilename + "." + ext);
    
    double compressionPerc = calculateCompression(inputFileSize, outputFileSize);
    cout << "\n=====================================" << endl;
    cout << "            HASIL KOMPRESI           " << endl;
    cout << "=====================================" << endl;

    cout << " Waktu eksekusi: " << duration.count() << " ms" << endl;
    cout << " Ukuran file input: " << inputFileSize << " KB" << endl;
    cout << " Ukuran file output: " << outputFileSize << " KB" << endl;
    cout << " Persentase kompresi: " << compressionPerc << " %" << endl;

    cout << "--------------------------------------" << endl;
    cout << " Kedalaman Quadtree: " << QuadTree::depthTree(root) << endl;
    cout << " Jumlah simpul pada Tree: " << QuadTree::countNode(root) << endl;
    cout << "=====================================" << endl;

    
    delete root;
    return 0;
}