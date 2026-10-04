#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <unsupported/Eigen/SparseExtra>
#include "stb_image.h"
#include "stb_image_write.h"
#include <random>
#include <algorithm>

// Function declarations
namespace utilities
{
    Eigen::MatrixXd get_matrix_from_file(const std::string& filename);

    Eigen::MatrixXd add_noise_to_matrix(const Eigen::MatrixXd& matrix, int noise_level);
    void get_png_image_from_matrix(const Eigen::MatrixXd& matrix, const std::string& filename);

    Eigen::VectorXd convert_matrix_to_vector(const Eigen::MatrixXd& matrix);
    Eigen::MatrixXd convert_vector_to_matrix(const Eigen::VectorXd& vector, int rows, int cols);

    Eigen::SparseMatrix<double> buildAconvolutionoperator(int n, int m, Eigen::MatrixXd H);
    Eigen::VectorXd clamp_image(Eigen::VectorXd& image_vector, int min_value, int max_value);

    bool is_symmetric(const Eigen::SparseMatrix<double>& m);


}

#endif // UTILITIES_HPP