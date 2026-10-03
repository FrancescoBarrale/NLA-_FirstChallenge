# challenge1

A small C++ project template for the first challenge.

## Before starting

Run these commands inside the course container (after entering the project
directory):

```bash
source /u/sw/etc/bash.bashrc
module load gcc-glibc
module load lis
```

The Makefile uses `mpicxx` and links LIS. Eigen is found through
`mkEigenInc`; if that variable is not set, the standard local path
`/usr/include/eigen3` is used.

## Running

From the project root:

```bash
make
./main
```

To rebuild from scratch:

```bash
make clean && make
```
