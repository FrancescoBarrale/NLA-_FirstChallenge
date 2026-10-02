#include <iostream>
#include "utilities.hpp"

int main() {
    
    // Ex 1:
    Eigen::MatrixXi image = utilities::get_matrix_from_file("deer.jpg");
    // Ex 2:
    Eigen::MatrixXi noisy_image = utilities::add_noise_to_matrix(image, 50);
    utilities::get_png_image_from_matrix(noisy_image, "noisy_deer.png");

    // Ex 3:
    Eigen::VectorXi image_vector = utilities::convert_matrix_to_vector(image);
    Eigen::VectorXi noisy_image_vector = utilities::convert_matrix_to_vector(noisy_image);
    std::cout << "image_vector has " << image_vector.size() << " components" << " = " << image.rows() * image.cols() << std::endl;
    std::cout << "noisy_image_vector has " << noisy_image_vector.size() << " components" << " = " << noisy_image.rows() * noisy_image.cols() << std::endl;
    std::cout << "Image vector euclidean norm: " << image_vector.norm() << std::endl;

    // Ex 4:

    // Ex 5:

    // Ex 6:

    // Ex 7:

    // Ex 8:

    // Ex 9:

    // Ex 10:

    // Ex 11:

    // Ex 12:

    // Ex 13:

    return 0;
}