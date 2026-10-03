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
    Eigen::saveMarket(A2, "A2.mtx");
    Eigen::saveMarketVector(w, "w.mtx");
    constexpr double tolerance = 1.0e-12;

    lis_solver::Result result = lis_solver::solve(A2, w, tolerance);

    std::cout << "Tolerance: " << tolerance << std::endl;
    std::cout << "Iterations: " << result.iterations << std::endl;
    std::cout << "Lis Residual: " << result.residual << std::endl;

    // Ex 9:
    Eigen::VectorXd x = result.solution;
    Eigen::VectorXd x_clamped = utilities::clamp_image(x, 0, 255);
    Eigen::MatrixXd x_image_1 = utilities::convert_vector_to_matrix(x_clamped, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(x_image_1, "sharpened_noisy_deer_x1.png");

    // Ex 10:
    Eigen::MatrixXd Hed2(3, 3);
    Hed2 << -1. ,0.,1.,
            -2. ,0.,2.,
            -1. ,0.,1.;

    Eigen::SparseMatrix<double> A3 = utilities::buildAconvolutionoperator(image.rows(), image.cols(), Hed2);
    if (utilities::is_symmetric(A3)) {
        std::cout << "A3 is symmetric." << std::endl;
    } else {
        std::cout << "A3 is not symmetric." << std::endl;
    }

    // Ex 11:
    Eigen::VectorXd Edge_detection_image = A3 * v;
    Edge_detection_image = utilities::clamp_image(Edge_detection_image, 0, 255);
    Eigen::MatrixXd Edge_detection_image_matrix = utilities::convert_vector_to_matrix(Edge_detection_image, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(Edge_detection_image_matrix, "Edge_detection_deer.png");

    // Ex 12:
    const Eigen::Index system_size = image.rows() * image.cols();
    Eigen::SparseMatrix<double> identity(system_size, system_size);
    identity.setIdentity();
    Eigen::SparseMatrix<double> A_new = 4.0 * identity + A3;
    A_new.makeCompressed();
    if (utilities::is_symmetric(A_new)) {
        std::cout << "A_new is symmetric." << std::endl;
    } else {
        std::cout << "A_new is not symmetric." << std::endl;
    } //check if symmetric cause maybe can use better solver, but in this case is not symmetric so we use gmres
    constexpr double tolerance2 = 1.0e-10;

    lis_solver::Result result2 = lis_solver::solve(A_new, w, tolerance2);

    std::cout << "Tolerance: " << tolerance2 << std::endl;
    std::cout << "Iterations: " << result2.iterations << std::endl;
    std::cout << "Lis Residual: " << result2.residual << std::endl;

    lis_finalize();

    // Ex 13:
    Eigen::VectorXd y = result2.solution;
    Eigen::VectorXd y_clamped = utilities::clamp_image(y, 0, 255);
    Eigen::MatrixXd y_image_1 = utilities::convert_vector_to_matrix(y_clamped, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(y_image_1, "y1.png");

    return 0;
}