#ifndef BONUS_HPP
#define BONUS_HPP

#include "image.hpp"
#include <string>
#include <utility>

/**
 * Class untuk mengoptimalkan parameter kompresi berdasarkan target persentase kompresi
 */
class Kompresi {
public:
    /**
     * Mencari parameter optimal (block size dan threshold) untuk mencapai target kompresi
     * 
     * @param img Gambar yang akan dikompresi
     * @param targetCompressionRate Target persentase kompresi (0-100)
     * @param filename Nama file input gambar
     * @param outputBase Nama dasar untuk file output sementara
     * @param useSSIM True jika menggunakan metode SSIM, false untuk Variance
     * @return Pair berisi (minBlockSize, threshold) optimal
     */
    static std::pair<int, double> findOptimalParameters(
        const Image& img, 
        double targetCompressionRate, 
        const std::string& filename, 
        const std::string& outputBase, 
        bool useSSIM
    );
};

#endif