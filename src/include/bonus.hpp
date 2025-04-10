#ifndef BONUS_HPP
#define BONUS_HPP

#include <string>
#include <utility> 
#include "quadtree.hpp"


class Kompresi {
public:
    static std::pair<double, double> findOptimalParameters(
        const Image& img, 
        double targetCompressionRate,
        const std::string& filename, 
        const std::string& outputBase, 
        bool useSSIM
    );

    static double testCompression(
        const Image& img,
        double blockSize,
        double threshold,
        double inputFileSize,
        const std::string& filename,
        const std::string& outputBase,
        bool useSSIM
    );
};

#endif // BONUS_HPP