#include <iostream>
#include "utilities.hpp"
#include "lis_solver.hpp"

int main(int argc, char** argv) {
    const LIS_INT initialization_error = lis_initialize(&argc, &argv);
    if (initialization_error != LIS_SUCCESS) {
        std::cerr << "LIS initialization failed with error code "
                  << initialization_error << '\n';
        return 1;
    }
    
    // Ex 1:
    Eigen::MatrixXd image = utilities::get_matrix_from_file("deer.jpg");
    // Ex 2:
    Eigen::MatrixXd noisy_image = utilities::add_noise_to_matrix(image, 50);
    utilities::get_png_image_from_matrix(noisy_image, "noisy_deer.png");

    // Ex 3:
    Eigen::VectorXd v = utilities::convert_matrix_to_vector(image.cast<double>());
    Eigen::VectorXd w = utilities::convert_matrix_to_vector(noisy_image.cast<double>());
    std::cout << "image_vector v has " << v.size() << " components" << " = " << image.rows() * image.cols() << std::endl;
    std::cout << "noisy_image_vector w has " << w.size() << " components" << " = " << noisy_image.rows() * noisy_image.cols() << std::endl;
    std::cout << "Image vector euclidean norm: " << v.norm() << std::endl;

    // Ex 4:
    Eigen::MatrixXd Hav1(3, 3);
    Hav1 << 1./12., 1./12., 1./12.,
         1./12., 4./12., 1./12.,
         1./12., 1./12., 1./12.;

    Eigen::SparseMatrix<double> A1 = utilities::buildAconvolutionoperator(image.rows(), image.cols(), Hav1);
    std::cout << "Elementi non zero: " << A1.nonZeros() << std::endl;

    // Ex 5:
    Eigen::VectorXd Blurred_noisy_image = A1 * w;
    Eigen::MatrixXd blurred_noisy_image_matrix = utilities::convert_vector_to_matrix(Blurred_noisy_image, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(blurred_noisy_image_matrix, "blurred_noisy_deer.png");

    // Ex 6:
    Eigen::MatrixXd Hsh1(3, 3);
    Hsh1 << 0, -3, 0,
         -1, 9, -3,
         0, -1, 0;
    Eigen::SparseMatrix<double> A2 = utilities::buildAconvolutionoperator(image.rows(), image.cols(), Hsh1);
    std::cout << "Elementi non zero: " << A2.nonZeros() << std::endl;
    
    if (utilities::is_symmetric(A2)) {
        std::cout << "A2 is symmetric." << std::endl;
    } else {
        std::cout << "A2 is not symmetric." << std::endl;
    }

    // Ex 7:
    Eigen::VectorXd sharpened_image = A2 * v;
    sharpened_image = utilities::clamp_image(sharpened_image, 0, 255);
    Eigen::MatrixXd sharpened_image_matrix = utilities::convert_vector_to_matrix(sharpened_image, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(sharpened_image_matrix, "sharpened_deer.png");

    // Ex 8: continues 
    constexpr double tolerance = 1.0e-12;

    lis_solver::Result result = lis_solver::solve(A2, w, tolerance);

    std::cout << "Tolerance: " << tolerance << '\n';
    std::cout << "Iterations: " << result.iterations << '\n';
    std::cout << "Lis Residual: " << result.residual << '\n';

    lis_finalize();
    
    // Ex 9:
    Eigen::VectorXd x = result.solution;
    Eigen::VectorXd x_clamped = utilities::clamp_image(x, 0, 255);
    Eigen::MatrixXd x_image_1 = utilities::convert_vector_to_matrix(x_clamped, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(x_image_1, "x_image_1.png");


    // Ex 10:

    // Ex 11:

    // Ex 12:

    // Ex 13:

    return 0;
}