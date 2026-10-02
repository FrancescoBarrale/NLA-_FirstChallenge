#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "utilities.hpp"
#include <algorithm>

namespace utilities
{
    Eigen::MatrixXd get_matrix_from_file(const std::string& filename)
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
            return Eigen::MatrixXd();
        }

        // Create an Eigen matrix from the loaded image
        Eigen::MatrixXd matrix(height, width);
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

    Eigen::MatrixXd add_noise_to_matrix(const Eigen::MatrixXd& matrix, int noise_level)
    {
        static std::random_device seed;
        static std::mt19937 generator(seed());
        std::uniform_int_distribution<int> distribution(-noise_level, noise_level);
        Eigen::MatrixXd noisy_matrix = matrix;

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

    void get_png_image_from_matrix(const Eigen::MatrixXd& matrix, const std::string& filename)
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

    Eigen::VectorXd convert_matrix_to_vector(const Eigen::MatrixXd& matrix)
    {
        Eigen::VectorXd vector(matrix.size());
        for (int i = 0; i < matrix.rows(); ++i)
        {
            for (int j = 0; j < matrix.cols(); ++j)
            {
                vector(i * matrix.cols() + j) = matrix(i, j);
            }
        }
        return vector;
    }

    Eigen::MatrixXd convert_vector_to_matrix(const Eigen::VectorXd& vector, int rows, int cols)
    {
        if (vector.size() != rows * cols)
        {
            std::cerr << "Error: Vector size does not match the specified matrix dimensions." << std::endl;
            return Eigen::MatrixXd(); // Return an empty matrix if dimensions do not match
        }

        Eigen::MatrixXd matrix(rows, cols);
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < cols; ++j)
            {
                matrix(i, j) = vector(i * cols + j);
            }
        }
        return matrix;
    }

    Eigen::SparseMatrix<double> buildAconvolutionoperator(int n, int m, Eigen::MatrixXd H)
    {
        if (H.rows() != H.cols())
        {
            std::cerr << "Error: H must be a square matrix." << std::endl;
            return Eigen::SparseMatrix<double>(); // Return zero matrix if H is not square
        }
        
        Eigen::SparseMatrix<double> A(n*m, n*m);
        std::vector<Eigen::Triplet<double>> tripletList;

        int l=H.rows();
        tripletList.reserve(n * m * l * l);

        for (int i = 0; i < n ; ++i)
        {
            for (int j = 0; j< m; ++j)
            {
                for (int h = 0; h < l; ++h)
                {   
                    for (int k = 0; k < l; ++k)
                    {
                        int row_H = i + h - l/2;
                        int col_H = j + k - l/2;
                        if (row_H >= 0 && row_H < n && col_H >= 0 && col_H < m){

                            tripletList.push_back(Eigen::Triplet<double>(row_H*m + col_H, i*m + j, H(h, k)));
                        }
                    }
                }
            }
        }
        A.setFromTriplets(tripletList.begin(), tripletList.end());
        return A;
    }
}