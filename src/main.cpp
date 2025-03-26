#include <iostream>
#include <string>
#include "include/image.hpp"

using namespace std;

int main() {
    string imagePath;
    
    cout << "Masukkan path gambar: ";
    getline(cin, imagePath);

    try {
        Image img = loadImage(imagePath);
        
        cout << "Gambar berhasil dimuat:" << endl;
        cout << "Width: " << img.width << endl;
        cout << "Height: " << img.height << endl;
        cout << "Channels: " << img.channels << endl;
        
        // Menampilkan beberapa pixel pertama sebagai contoh
        int maxRows = min(5, img.height); // Maksimal 5 baris biar ga kepenuhan
        int maxCols = min(10, img.width); // Maksimal 10 kolom

        cout << "\nPixel Data (R, G, B, A jika ada):\n";
        for (int y = 0; y < maxRows; y++) {
            for (int x = 0; x < maxCols; x++) {
                Pixel p = img.pixels[y][x];
                cout << "(" << static_cast<int>(p.r) << ", "
                     << static_cast<int>(p.g) << ", "
                     << static_cast<int>(p.b);
                
                if (img.channels == 4) { // Kalau ada Alpha channel
                    cout << ", " << static_cast<int>(p.a);
                }
                
                cout << ") ";
            }
            cout << endl;
        }

        cout << "\n... (Dibatasi untuk tampilan lebih rapi)" << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
