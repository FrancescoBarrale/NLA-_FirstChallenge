#include <iostream>
#include "utilities.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

int main() {
    std::cout << "challenge1 is running" << std::endl;
    utilities::somma(5, 10);
    return 0;
}