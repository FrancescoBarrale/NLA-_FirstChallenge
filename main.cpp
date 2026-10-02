#include <iostream>
#include "utilities.hpp"

int main() {
    
    // Ex 1:
    Eigen::MatrixXd image = utilities::get_matrix_from_file("deer.jpg");
    // Ex 2:
    Eigen::MatrixXd noisy_image = utilities::add_noise_to_matrix(image, 50);
    utilities::get_png_image_from_matrix(noisy_image, "noisy_deer.png");

    // Ex 3:
    Eigen::VectorXd image_vector = utilities::convert_matrix_to_vector(image.cast<double>());
    Eigen::VectorXd noisy_image_vector = utilities::convert_matrix_to_vector(noisy_image.cast<double>());
    std::cout << "image_vector has " << image_vector.size() << " components" << " = " << image.rows() * image.cols() << std::endl;
    std::cout << "noisy_image_vector has " << noisy_image_vector.size() << " components" << " = " << noisy_image.rows() * noisy_image.cols() << std::endl;
    std::cout << "Image vector euclidean norm: " << image_vector.norm() << std::endl;

    // Ex 4:
    Eigen::MatrixXd H(3, 3);
    H << 1./12., 1./12., 1./12.,
         1./12., 4./12., 1./12.,
         1./12., 1./12., 1./12.;

    Eigen::SparseMatrix<double> A = utilities::buildAconvolutionoperator(image.rows(), image.cols(), H);
    std::cout << "Elementi non zero: " << A.nonZeros() << std::endl;

    // Ex 5:
    Eigen::VectorXd Blurred_noise_image = A * image_vector.cast<double>();
    Eigen::MatrixXd blurred_noisy_image_matrix = utilities::convert_vector_to_matrix(Blurred_noise_image, image.rows(), image.cols());
    utilities::get_png_image_from_matrix(blurred_noisy_image_matrix, "blurred_noisy_deer.png");

    // Ex 6:
    Eigen::MatrixXd Hsh1(3, 3);
    Hsh1 << 0, -3, 0,
         -1, 9, -3,
         0, -1, 0;
    Eigen::SparseMatrix<double> Ash1 = utilities::buildAconvolutionoperator(image.rows(), image.cols(), Hsh1);
    std::cout << "Elementi non zero: " << Ash1.nonZeros() << std::endl;
    Eigen::SparseMatrix<double> Ash1_t = Ash1.transpose();

    Eigen::SparseMatrix<double> diff = Ash1_t - Ash1;
    if (diff.norm() < 1e-12)
    {
        std::cout << "Ash1 is symmetric" << std::endl;
    }
    else
    {
        std::cout << "Ash1 is not symmetric" << std::endl;
    }


    // Ex 7:

    // Ex 8:

    // Ex 9:

    // Ex 10:

    // Ex 11:

    // Ex 12:

    // Ex 13:

    return 0;
}