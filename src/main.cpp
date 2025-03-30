#include "include/image.hpp"
#include "include/varians.hpp"
#include <iostream>
#include <string>

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
    
    cout << "Dimensi Gambar: " << img.width << " x " << img.height << "\n";
    
    int x, y, regionWidth, regionHeight;
    cout << "Masukkan koordinat x region: ";
    cin >> x;
    cout << "Masukkan koordinat y region: ";
    cin >> y;
    cout << "Masukkan lebar region: ";
    cin >> regionWidth;
    cout << "Masukkan tinggi region: ";
    cin >> regionHeight;
    
    // Hitung mean dan variansi untuk tiap kanal warna
    for (int ch = 0; ch < 3; ch++) {
        double mean = calculateMean(&img.pixels, x, y, regionWidth, regionHeight, ch);
        double var  = calculateMad(&img.pixels, x, y, regionWidth, regionHeight, ch);
        string channel = (ch == 0) ? "Red" : (ch == 1) ? "Green" : "Blue";
        cout << "Channel " << channel << " - Mean: " << mean << ", Mad: " << var << "\n";
    }
    
    double rgbMad = calculateRGBMad(&img.pixels, x, y, regionWidth, regionHeight);
    cout << "Rata-rata RGB Mad: " << rgbMad << "\n";
    
    return 0;
}
