#include "include/image.hpp"
#include "include/quadtree.hpp"
#include <iostream>

using namespace std;

void printQuadTree(QuadTree* node, int depth = 0) {
    if (!node) return;
    
    for (int i = 0; i < depth; ++i) cout << "  ";
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
    
    QuadTree* root = buildQuadTree(img.pixels, 0, 0, img.width, img.height);
    
    cout << "\nStruktur Quadtree:\n";
    printQuadTree(root);
    
    return 0;
}
