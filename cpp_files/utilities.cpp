#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#endif

#include "utilities.hpp"

#if defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

#include <filesystem>

// Function definitions
namespace utilities
{
    // Function to read an image file and return it as an Eigen matrix
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

    // Function to add noise to an Eigen matrix
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
                // Add noise to the current element, keeping the value within the range [0, 255]
                int noisy_value = noisy_matrix(i, j) + distribution(generator);
                noisy_matrix(i, j) = std::clamp(noisy_value, 0, 255);
            }
        }
        return noisy_matrix;
    }

    // Function to save an Eigen matrix as a PNG image
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

        // Create the output directory if it doesn't exist
        const std::filesystem::path output_directory = "png_files";
        std::filesystem::create_directories(output_directory);
        const std::filesystem::path output_path = output_directory / filename;

        if (stbi_write_png(output_path.string().c_str(), width, height, 1, image, width) == 0)
        {
            std::cerr << "Error: could not write PNG file "
                      << output_path << std::endl;
        }
        delete[] image;
    }

    // Function to convert an Eigen matrix to a vector
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

    // Function to convert a vector back to an Eigen matrix
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

    // Function to build a convolution operator, given the dimensions of the image and the convolution kernel H
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

        // Loop over each output pixel in the image
        for (int i = 0; i < n ; ++i)
        {
            for (int j = 0; j< m; ++j)
            {
                // Loop over each element in the convolution kernel H
                for (int h = 0; h < l; ++h)
                {   
                    for (int k = 0; k < l; ++k)
                    {
                        // Calculate the corresponding input pixel for the
                        // current output pixel and kernel element.
                        int row = i + h - l/2;
                        int col = j + k - l/2;
                        // Ignore out-of-bounds input pixels, as required by
                        // the zero-padding convention in the challenge.
                        if (row >= 0 && row < n && col >= 0 && col < m && H(h, k) != 0.0){
                            // A(output, input) = H(h, k).
                            tripletList.push_back(
                                Eigen::Triplet<double>(
                                    i*m + j,
                                    row*m + col,
                                    H(h, k)
                                )
                            );
                        }
                    }
                }
            }
        }
        // Set the triplet list to the sparse matrix A
        A.setFromTriplets(tripletList.begin(), tripletList.end());
        return A;
    }

    // Function to clamp the values of an Eigen vector within a specified range [min_value, max_value]
    Eigen::VectorXd clamp_image(Eigen::VectorXd& image_vector, int min_value, int max_value)
    {
        Eigen::VectorXd clamped_vector = image_vector;
        for (int i = 0; i < clamped_vector.size(); ++i)
        {
            clamped_vector(i) = std::clamp(clamped_vector(i), static_cast<double>(min_value), static_cast<double>(max_value));
        }
        return clamped_vector;
    }

    // Function to check if a sparse matrix is symmetric
    bool is_symmetric(const Eigen::SparseMatrix<double>& m)
    {
        Eigen::SparseMatrix<double> m_t = m.transpose();

    Eigen::SparseMatrix<double> diff = m_t - m;
    
    return diff.norm() < 1e-12;
    }
}