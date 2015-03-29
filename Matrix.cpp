#include <bitset>
#include "Matrix.h"

namespace VAN_MAASTRICHT {
	Matrix::Matrix() {
		for(unsigned int i = 0; i < size; i++) {
			mat[i] = 0;
		}
	}
	
	// j is the column of the last edge added.
	bool Matrix::check_squares(unsigned int j) {
		for(unsigned int i = 0; i < size; i++) {
			if((i != j) && (((N >> j) & mat[i]) == 0) && __builtin_popcount(mat[i] & mat[j]) > 1) {
				return true;
			}
		}
		return false;
	}

	// i,j is the last edge added
	bool Matrix::check_triangles(unsigned int i, unsigned int j) {
		if((mat[i] & mat[j]) > 0) {
			return true;
		}
		return false;
	}

	// set the i,j entry
	void Matrix::set_entry(uint32_t i, uint32_t j) {
		if(i < 0 || i >= size || j < 0 || j >= size) {
			return;
		}
		mat[i] |= (1 << (size - j - 1));
		mat[j] |= (1 << (size - i - 1));
	}
	
	// set the ith row to the uint32_t j
	void Matrix::set_row(unsigned int i, uint32_t j) {
		if(i < 0 || i >= size) {
			return;
		}
		mat[i] = j;
	}

	// get the entry at i,j
	uint32_t Matrix::get_entry(unsigned int i, unsigned int j) {
		return (mat[i] & (N >> j)) >> (size - j - 1);
	}

	// get the ith row
	const uint32_t Matrix::get_row(unsigned int i) const {
		if(i >= size || i < 0) {
			return -1;
		}
		return mat[i];
	}
	
	// get the ith row
	uint32_t Matrix::get_row(unsigned int i) {
		return mat[i];
	}

	// get the size of the graph
	const unsigned int Matrix::get_size() const {
		return size;
	}

	// get the size of the graph
	unsigned int Matrix::get_size() {
		return size;
	}

	// get the degree of vertex i
	unsigned int Matrix::get_degree(unsigned int i) {
		if(i < 0 || i >= size) {
			return -1;
		}
		return __builtin_popcount(mat[i]);
	}

	unsigned int Matrix::get_number_edges() {
		unsigned int n = 0;
		for(unsigned int i = 0; i < size; i++) {
			n += __builtin_popcount(mat[i]);
		}
		return n >> 1;
	}
	
	// overloaded output operator. Useful for debugging and writing to file.
	ostream& operator <<(ostream& outs, const Matrix& mat) {
		for(unsigned int i = 0; i < mat.get_size() - 1; i++) {
			bitset<32> x(mat.get_row(i));
			outs << x << endl;
		}
		outs << bitset<32>(mat.get_row(mat.get_size() - 1));
		return outs;
	}
}
