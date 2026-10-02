#include "utilities.hpp"

namespace utilities
{
    // Ex 1:
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

    // Ex 2:
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
                noisy_matrix(i, j) += distribution(generator);
            }
        }
        return noisy_matrix;
    }

    // Ex 3:
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
}