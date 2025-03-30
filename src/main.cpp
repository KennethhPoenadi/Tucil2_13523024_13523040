#include "include/image.hpp"
#include "include/utils.hpp"
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
    
    // Validasi region
    if (x < 0 || y < 0 || x + regionWidth > img.width || y + regionHeight > img.height) {
        cerr << "Error: Region yang dipilih berada di luar dimensi gambar!" << endl;
        return 1;
    }
    
    // Hitung entropy untuk tiap kanal warna
    for (int ch = 0; ch < 3; ch++) {
        double entropy = calculateEntropy(&img.pixels, x, y, regionWidth, regionHeight, ch);
        string channel = (ch == 0) ? "Red" : (ch == 1) ? "Green" : "Blue";
        cout << "Channel " << channel << " - Entropy: " << entropy << "\n";
    }
    
    double rgbEntropy = calculateRGBEntropyTotal(&img.pixels, x, y, regionWidth, regionHeight);
    cout << "Rata-rata RGB Entropy: " << rgbEntropy << "\n";
    
    // Untuk perbandingan, tampilkan juga hasil dari metode lain
    double rgbVariance = calculateRGBVariance(&img.pixels, x, y, regionWidth, regionHeight);
    double rgbMad = calculateRGBMad(&img.pixels, x, y, regionWidth, regionHeight);
    double rgbMaxDiff = calculateMPD(&img.pixels, x, y, regionWidth, regionHeight);
    
    cout << "\nPerbandingan metode pengukuran error:\n";
    cout << "Variance: " << rgbVariance << "\n";
    cout << "MAD: " << rgbMad << "\n";
    cout << "Max Pixel Difference: " << rgbMaxDiff << "\n";
    cout << "Entropy: " << rgbEntropy << "\n";
    
    return 0;
}