#ifndef BONUS_HPP
#define BONUS_HPP

#include <string>
#include <utility> // For std::pair
#include "quadtree.hpp" // For Image struct and QuadTree class

/**
 * Class for handling image compression functionality
 */
class Kompresi {
public:
    /**
     * Finds optimal parameters for image compression to achieve a target compression rate
     * 
     * @param img Image to be compressed
     * @param targetCompressionRate Desired compression rate in percentage
     * @param filename Original image filename
     * @param outputBase Base name for output files
     * @param useSSIM Whether to use SSIM (true) or MSE (false) for compression quality measurement
     * @return A pair containing the optimal block size and threshold
     */
    static std::pair<double, double> findOptimalParameters(
        const Image& img, 
        double targetCompressionRate,
        const std::string& filename, 
        const std::string& outputBase, 
        bool useSSIM
    );

    /**
     * Tests compression with specific parameters and returns the compression rate
     * 
     * @param img Image to be compressed
     * @param blockSize Block size parameter for QuadTree
     * @param threshold Threshold parameter for compression
     * @param inputFileSize Size of the input file
     * @param filename Original image filename
     * @param outputBase Base name for output files
     * @param useSSIM Whether to use SSIM (true) or MSE (false) for compression quality measurement
     * @return The achieved compression rate as a percentage
     */
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