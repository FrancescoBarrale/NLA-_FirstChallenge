#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <eigen3/Eigen/Dense>
#include "stb_image.h"
#include "stb_image_write.h"
#include <random>


namespace utilities
{
    Eigen::MatrixXi get_matrix_from_file(const std::string& filename);

    Eigen::MatrixXi add_noise_to_matrix(const Eigen::MatrixXi& matrix, int noise_level);
    void get_png_image_from_matrix(const Eigen::MatrixXi& matrix, const std::string& filename);

    Eigen::VectorXi convert_matrix_to_vector(const Eigen::MatrixXi& matrix);


}

#endif // UTILITIES_HPP