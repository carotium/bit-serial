#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/eigen.h>

#include <stdexcept>

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

using namespace Eigen;

typedef Matrix<int64_t, 1, Dynamic> VectorX64_t;
typedef Matrix<int64_t, Dynamic, Dynamic> MatrixX64_t;

MatrixX64_t p2s_matrix(MatrixX64_t mat, int bits, bool umat = false) {

    if(umat) bits += 1;

    MatrixX64_t bits_mat(bits, 1);

    int mat_size = mat.size();
    if(mat_size > 64) {
	int new_size = mat_size / 64 + 1;
	bits_mat.resize(bits, new_size);
    }

    bits_mat.setZero();

    for(int i = 0; i < mat.rows(); i++) {
	for(int j = 0; j < mat.cols(); j++) {
	    int64_t val_int = mat(i, j);
    	    for(int b = 0; b < bits; b++) {
		int64_t val_bit = (val_int & (1ULL << b)) > 0;
		int bit_idx = (i * mat.cols() + j) / 64;
		bits_mat(b, bit_idx) |= (val_bit << ((i * mat.cols() + j) % 64));
	    }
	}
    }

    return bits_mat;
}

MatrixX64_t serial_mat_mul(MatrixX64_t L_mat, MatrixX64_t R_mat, int M, int K, int N) {
    int l = L_mat.rows();
    int r = R_mat.rows();

    MatrixX64_t P_mat(M, N);
    P_mat.setZero();

    int i, j, sgnL, sgnR, weight, m, n, k, L_iter, R_iter, L_bit, R_bit, bit_idx_l, bit_idx_r;

    for(i = 0; i < l; i++) {
	for(j = 0; j < r; j++) {
	    sgnL = (i == (l - 1)) ? -1 : 1;
	    sgnR = (j == (r - 1)) ? -1 : 1;
	    weight = (sgnL * sgnR) << (i + j);
	    for(m = 0; m < M; m++) {
		for(n = 0; n < N; n++) {
		    for(k = 0; k < K; k++) {
			L_iter = m * K + k;
			R_iter = k * N + n;
			bit_idx_l = L_iter / 64;
			bit_idx_r = R_iter / 64;
			L_bit = (L_mat(i, bit_idx_l) & (1ULL << (L_iter % 64))) > 0;
			R_bit = (R_mat(j, bit_idx_r) & (1ULL << (R_iter % 64))) > 0;
			P_mat(m, n) += weight * (L_bit & R_bit);
		    }
		}
	    }
	}
    }
    return P_mat;
}

PYBIND11_MODULE(bit_serial, m, py::mod_gil_not_used()) {
    m.doc() = "Bit-serial computation plugin";
    m.def("p2s_matrix", &p2s_matrix, "A function that bit-serializes data that is parallel.",
	py::arg("mat"), py::arg("bits"), py::arg("umat")=false);
    m.def("serial_mat_mul", &serial_mat_mul, "A function that computes bit-serial dot product P = L x R.",
	py::arg("L_mat"), py::arg("R_mat"), py::arg("M"), py::arg("K"), py::arg("N"));

#ifdef VERSION_INFO
    m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
    m.attr("__version__") = "dev";
#endif
}
