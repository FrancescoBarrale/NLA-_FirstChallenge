#include <iostream>
#include "utilities.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    
    // Ex 1:
    Eigen::MatrixXi image = utilities::get_matrix_from_file("deer.jpg");
    // Ex 2:
    Eigen::MatrixXi noisy_image = utilities::add_noise_to_matrix(image, 50);
    utilities::get_png_image_from_matrix(noisy_image, "noisy_deer.png");


    return 0;
}