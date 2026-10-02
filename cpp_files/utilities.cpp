#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utilities.hpp"
#include <algorithm>

namespace utilities
{
    Eigen::MatrixXi get_matrix_from_file(const std::string& filename)
    {
        int width;
        int height;
        int channels;

        unsigned char* image = stbi_load(
            filename.c_str(),
            &width,
            &height,
            &channels,
            1 
        );

        if (!image)
        {
            std::cerr << "Error: " << stbi_failure_reason() << std::endl;
            return Eigen::MatrixXi();
        }

        // Create an Eigen matrix from the loaded image
        Eigen::MatrixXi matrix(height, width);
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                matrix(i, j) = image[i * width + j];
            }
        }

        stbi_image_free(image);
        return matrix;
    }

    Eigen::MatrixXi add_noise_to_matrix(const Eigen::MatrixXi& matrix, int noise_level)
    {
        static std::random_device seed;
        static std::mt19937 generator(seed());
        std::uniform_int_distribution<int> distribution(-noise_level, noise_level);
        Eigen::MatrixXi noisy_matrix = matrix;

        for (int i = 0; i < noisy_matrix.rows(); ++i)
        {
            for (int j = 0; j < noisy_matrix.cols(); ++j)
            {   
                int noisy_value = noisy_matrix(i, j) + distribution(generator);
                noisy_matrix(i, j) = std::clamp(noisy_value, 0, 255);
            }
        }
        return noisy_matrix;
    }

    void get_png_image_from_matrix(const Eigen::MatrixXi& matrix, const std::string& filename)
    {
        int width = matrix.cols();
        int height = matrix.rows();
        unsigned char* image = new unsigned char[width * height];

        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < width; j++)
            {
                image[i * width + j] = static_cast<unsigned char>(matrix(i, j));
            }
        }

        stbi_write_png(filename.c_str(), width, height, 1, image, width);
        delete[] image;
    }

    Eigen::VectorXi convert_matrix_to_vector(const Eigen::MatrixXi& matrix)
    {
        Eigen::VectorXi vector(matrix.size());
        for (int i = 0; i < matrix.rows(); ++i)
        {
            for (int j = 0; j < matrix.cols(); ++j)
            {
                vector(i * matrix.cols() + j) = matrix(i, j);
            }
        }
        return vector;
    }
}