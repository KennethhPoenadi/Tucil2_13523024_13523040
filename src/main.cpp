#include "include/image.hpp"
#include "include/quadtree.hpp"
#include <iostream>
#include <string>

using namespace std;

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
    
    // Membuat QuadTree dengan mempertimbangkan ukuran blok minimum
    // Perlu menggunakan alamat dari matriks piksel untuk pointer
    const vector<vector<Pixel>>* pixelMatrix = &img.pixels;
    QuadTree* root = buildQuadTree(pixelMatrix, 0, 0, img.width, img.height, minX, minY);
    
    cout << "\nStruktur Quadtree:\n";
    printQuadTree(root);
    
    return 0;
}