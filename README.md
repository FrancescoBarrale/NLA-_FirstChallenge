# Numerical Linear Algebra — Challenge 1

Implementation of the first Numerical Linear Algebra challenge. The project
uses [Eigen](https://eigen.tuxfamily.org/) for dense and sparse linear algebra
and [LIS](https://www.ssisc.org/lis/) for iterative linear-system solvers.

## Github repository

You can find this challenge on github on the following link:

```link
https://github.com/FrancescoBarrale/NLA-_FirstChallenge.git
```

## Students

- Francesco Barrale (10889288)
- Anna Adele Gusmeroli (10837456)
- Alessandra Mantovani (10809658)

## Overview

The program processes `deer.jpg` as a grayscale image and completes all 13
challenge exercises. Results will appear in the terminal after running the code.

## Requirements

The project is intended to run in the course container with:

- GCC and `mpicxx`;
- Eigen 3;
- the LIS library;
- GNU Make.

Load the course environment before building:

```bash
source /u/sw/etc/bash.bashrc
module load gcc-glibc
module load lis
```

## Build and run

Change to the directory containing this README, then run:

```bash
make
./main
```

To remove the executable and intermediate object files and rebuild from
scratch:

```bash
make clean
make
```

Additional build targets are available for development:

```bash
make debug    # clean build with debug symbols and no optimisation
make release  # clean optimised build
```

## Generated files

During execution, progress and solver statistics for all 13 exercises are
printed to the terminal.

The following image files are created in the folder `png_files/`:

- `noisy_deer.png` — noisy input image;
- `blurred_noisy_deer.png` — smoothed noisy image;
- `sharpened_deer.png` — sharpened image;
- `x.png` — image reconstructed with the LIS solver;
- `Edge_detection_deer.png` — Sobel edge-detection result;
- `y.png` — image reconstructed with the Eigen solver.

The sparse matrix and right-hand-side vector (not required by this code) are also
exported in Matrix Market format:

- `A2.mtx`;
- `w.mtx`.

The PNG output directory is created automatically if it does not already
exist.

## Project structure

```text
.
├── main.cpp                 # Entry point and challenge exercises
├── cpp_files/
│   ├── lis_solver.cpp       # LIS solver implementation
│   └── utilities.cpp        # Image and sparse-matrix implementations
├── hpp_files/
│   ├── lis_solver.hpp       # LIS solver declarations
│   ├── utilities.hpp        # Utility declarations
│   ├── stb_image.h          # read image
│   └── stb_image_write.h    # write image

├── deer.jpg                 # Input image
├── Makefile
└── png_files/               # Generated PNG images
```

Files stb_image.h and stb_image_write.h, included int the hpp_files folder, are taken from the laboratories of the course.

## Troubleshooting

- **The input image cannot be loaded:** run `./main` from the project root,
  where `deer.jpg` is located.
