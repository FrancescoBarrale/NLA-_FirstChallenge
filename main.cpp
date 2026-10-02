#include <iostream>
#include "utilities.hpp"

int main() {
    
    // Ex 1:
    Eigen::MatrixXi image = utilities::get_matrix_from_file("deer.jpg");
    // Ex 2:
    Eigen::MatrixXi noisy_image = utilities::add_noise_to_matrix(image, 50);
    utilities::get_png_image_from_matrix(noisy_image, "noisy_deer.png");

    // Ex 3:
    Eigen::VectorXi image_vector = utilities::convert_matrix_to_vector(noisy_image);
    Eigen::VectorXi noisy_image_vector = utilities::convert_matrix_to_vector(noisy_image);
    std::cout << "Image vector euclidean norm: " << image_vector.norm() << std::endl;

    // Ex 4:

    return 0;
}