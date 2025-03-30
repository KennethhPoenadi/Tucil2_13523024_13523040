#include "include/image.hpp"
#include "include/quadtree.hpp"
#include "include/utils.hpp"
#include <iostream>
#include <string>

using namespace std;

/* masi testing ya wkwk */

void printQuadTree(QuadTree* node, int depth = 0) {
    if (!node) return;
    
    for (int i = 0; i < depth; ++i)
        cout << "  ";
    cout << "Node at (" << node->getX() << ", " << node->getY() << ") - Size: "
         << node->getSizeX() << "x" << node->getSizeY() << "\n";
    
    printQuadTree(node->getGambarKiriAtas(), depth + 1);
    printQuadTree(node->getGambarKananAtas(), depth + 1);
    printQuadTree(node->getGambarKiriBawah(), depth + 1);
    printQuadTree(node->getGambarKananBawah(), depth + 1);
}

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
    
    cout << "Dimensi Gambar: " << img.width << " x " << img.height << "\n";
    
    int minX, minY;
    cout << "Masukkan minimum block size X: ";
    cin >> minX;
    cout << "Masukkan minimum block size Y: ";
    cin >> minY;
    
    double threshold;
    cout << "Masukkan threshold error: ";
    cin >> threshold;
    
    int methodInput;
    cout << "Pilih metode error:\n";
    cout << "0: Variance\n1: MPD\n2: MAD\n3: Entropy\n";
    cout << "Pilihan: ";
    cin >> methodInput;
    
    // Set parameter boolean sesuai pilihan (asumsikan hanya satu yang true)
    bool useVariance = false, useMPD = false, useMAD = false, useEntropy = false;
    switch (methodInput) {
        case 0: useVariance = true; break;
        case 1: useMPD = true; break;
        case 2: useMAD = true; break;
        case 3: useEntropy = true; break;
        default:
            cout << "Input tidak valid. Default ke Variance.\n";
            useVariance = true;
            break;
    }
    
    // Pastikan untuk mengirim alamat dari matriks piksel
    const vector<vector<Pixel>>* pixelMatrix = &img.pixels;
    QuadTree* root = buildQuadTree(pixelMatrix, 0, 0, img.width, img.height, minX, minY, threshold, useVariance, useMPD, useMAD, useEntropy);
    
    cout << "\nStruktur Quadtree:\n";
    printQuadTree(root);
    
    return 0;
}
