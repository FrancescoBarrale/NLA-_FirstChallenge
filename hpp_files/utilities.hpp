#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <eigen3/Eigen/Dense>
#include "stb_image.h"
#include "stb_image_write.h"
#include <random>


namespace utilities
{
    
    // Ex 1:
    Eigen::MatrixXi get_matrix_from_file(const std::string& filename);

    // Ex 2:
    Eigen::MatrixXi add_noise_to_matrix(const Eigen::MatrixXi& matrix, int noise_level);
    void get_png_image_from_matrix(const Eigen::MatrixXi& matrix, const std::string& filename);


}

#endif // UTILITIES_HPP