# **🌳 Tucil 2 Strategi Algoritma 2025 – Kompresi Citra Menggunakan Quadtree**

## **📌 Deskripsi Singkat**
Program ini mengimplementasikan algoritma kompresi gambar berbasis struktur **Quadtree**. Citra warna warni direpresentasikan sebagai matriks RGB, kemudian dikompresi dengan cara membagi matriks menjadi semacam quadtree sehingga menjadi kuadran hingga memenuhi ambang batas kedalaman atau warna homogen. Kalian dapat mengatur tingkat persentase kompresi yang kalian inginkan dengan error += 5% dari yang kalian mau! Program ini juga dapat menghasilkan GIF yang dapat memperlihatkan anda bagaimana program ini bekerja dengan quadtree.

Terdapat beberapa perhitungan yang dapat digunakan pada program untuk perhitungan error pada gambar:
1. Varians: Mengukur seberapa tersebar nilai piksel dari rata-rata warna di blok tertentu.
2. MAD (Mean Absolute Deviation): Rata-rata dari nilai mutlak selisih setiap piksel terhadap rata-ratanya.
3. MPD (Max Pixel Difference): Selisih maksimum antara piksel mana pun terhadap rata-rata atau piksel lain.
4. Entropy: Ukuran ketidakteraturan distribusi warna (informasi). Semakin tinggi entropi, semakin kompleks gambar.
5. SSIM (Structural Similarity Index Measure): Mengukur kesamaan struktural antara blok asli dan blok hasil kompresi. Nilainya berada antara **0 (berbeda sekali)** hingga **1 (identik secara struktur)**. 
- **Semakin tinggi SSIM**, berarti blok tersebut **semakin mirip dengan versi aslinya**, sehingga **tidak perlu dibelah lagi**.
- Dalam implementasi, **blok dibelah** jika nilai SSIM **kurang dari threshold** yang ditentukan, misalnya input threshold 0.1 sehingga ssim akan dibagi bila masi kurang dari 0.9.

# *----------------------------------------------------------------*


<div align="center">
  
![Screenshot 2025-04-06 230240](https://github.com/user-attachments/assets/c7cb6366-5ebf-491c-8a20-8caa701579fb)
![Screenshot 2025-04-06 230001](https://github.com/user-attachments/assets/0d3b4491-342b-4c3f-9766-d38e7cbb42ba)
![image](https://github.com/user-attachments/assets/19154c12-678b-40d9-8b81-6a1a38d7f630)

</div>

# *----------------------------------------------------------------*
## **Gambaran GIF Visualisasi Rekonstruksi Quadtree**
<div align="center">

![test](https://github.com/user-attachments/assets/82413028-8afa-4194-af20-ac5c20032695)

</div>

## **🧠 Konsep Algoritma**
- Kompresi dilakukan secara **rekursif** dengan prinsip divide-and-conquer.
- Setiap node quadtree mewakili suatu bagian dari gambar.
- Jika seluruh bagian memiliki nilai piksel yang sama atau kedalaman maksimum tercapai, node menjadi daun (*leaf*).
- Jika tidak, bagian dibagi menjadi 4 kuadran: **Top-Left**, **Top-Right**, **Bottom-Left**, dan **Bottom-Right**.

## **📁 Struktur Folder**
```
C:.
│   makefile
│   
├───.vscode
│       settings.json
│
├───bin
│       main.exe
│       test.gif
│       test.jpg
│
├───doc
│       Tucil2_13523024_13523040.pdf
│
├───src
│   │   bonus.cpp
│   │   gifer.cpp
│   │   image.cpp
│   │   main.cpp
│   │   quadtree.cpp
│   │   utils.cpp
│   │
│   └───include
│           bonus.hpp
│           gif.h
│           gifer.hpp
│           image.hpp
│           quadtree.hpp
│           stb_image.h
│           stb_image_write.h
│           utils.hpp
│
└───test
        test.txt

```

## *Bahasa yang digunakan untuk pembuatan program adalah:*

<div align="center">

![C++](https://img.shields.io/badge/C%2B%2B-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![C](https://img.shields.io/badge/C-283593?style=for-the-badge&logo=c&logoColor=white)
![Makefile](https://img.shields.io/badge/Makefile-064F8C?style=for-the-badge&logo=gnu&logoColor=white)
![STB Image](https://img.shields.io/badge/STB_Image-FFDD00?style=for-the-badge&logo=stackoverflow&logoColor=black)
![GIF Encoder](https://img.shields.io/badge/GIF_Encoder-FFD700?style=for-the-badge&logo=imagemagick&logoColor=black)

</div>

## 📦 Requirements

Untuk dapat menjalankan program ini, pastikan kamu telah menginstal:

- **Compiler C++** (disarankan: `g++`, minimal versi C++17)
- **WSL** (disarankan, bisa juga tidak menggunakan WSL)
- **Make** (untuk menjalankan Makefile)
Jalankan command dibawah untuk pengguna WSL
```bash
sudo apt update && sudo apt install build-essential
```

## 🔧 Compiling

Untuk mengcompile kode, gunakan pada root directory
```bash
g++ -Wall -std=c++17 -Isrc/include src/main.cpp src/image.cpp src/quadtree.cpp src/utils.cpp src/bonus.cpp src/gifer.cpp -o bin/main.exe
```
lalu 
```bash
cd bin
./main
```
atau apabila menggunakan **WSL**, jalankan perintah pada root directory
```bash
make run
```

## Usage Flow

1. **Input**: alamat absolut gambar yang akan dikompresi (tanpa ekstensi)

2. **Input**: target persentase kompresi (float, contoh: `1.0 = 100%`)
   - Masukkan `0` jika ingin mode manual (tanpa target otomatis)

3. **Jika mode manual (target = 0):**
   - **Input** metode error (pilih salah satu dengan angka):
     ```
     1: Variance
     2: MPD
     3: MAD
     4: Entropy
     5: SSIM
     ```
   - **Input** threshold error (nilai disesuaikan dengan metode error)
   - **Input** ukuran minimum block size

4. **Input**: path file gambar output (absolut, tanpa ekstensi)

5. **Input**: buat GIF atau tidak (`y/n`)
   - Jika `y`: input path output GIF (absolut, tanpa ekstensi)

---

### 💾 Output

Program akan menghasilkan:

- Waktu eksekusi
- Ukuran gambar sebelum kompresi
- Ukuran gambar setelah kompresi
- Persentase kompresi
- Kedalaman pohon quadtree
- Jumlah simpul pada pohon
- File gambar hasil kompresi
- File GIF (jika dipilih)

File hasil akan disimpan sesuai path yang dimasukkan.


## 😁 Author
**Nama: Richard Christian**  
**NIM: 13523024**  
**Email: 13523024@std.stei.itb.ac.id**

**Nama: Kenneth Poenadi**  
**NIM: 13523040**  
**Email: 13523040@std.stei.itb.ac.id**  





