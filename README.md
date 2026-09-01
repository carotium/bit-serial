This repository provides a python package for bit-serial matrix multiplication implemented with C++.

It provides two functions for bit-serial matrix multiplication:
## p2s_matrix(mat, bits, umat)
which bit-serializes **mat** input matrix with **bits** number of bits, from unsigned or signed integers, depending on boolean input **umat**. 

## serial_mat_mul(L_matrix, R_matrix, M, K, N)
which computes the dot product of two bit-serialized matrices **L_matrix** and **R_matrix** where the number of elements for **L_matrix** is **[M, K]** and for **R_matrix** is **[K, N]**.

# Operation


![Bit-serial algorithm](https://github.com/user-attachments/assets/91d556b1-3bd7-410e-acf4-fef9dc416a83)

# References
[1] Yaman Umuroglu, Davide Conficconi, Lahiru Rasnayake, Thomas B. Preusser, and Magnus Själander. 2019. Optimizing Bit-Serial Matrix Multiplication for Reconfigurable Computing. ACM Trans. Reconfigurable Technol. Syst. 12, 3, Article 15 (September 2019), 24 pages. https://doi.org/10.1145/3337929
