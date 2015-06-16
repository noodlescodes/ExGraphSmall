#include <bitset>
#include <iostream>
#include "Matrix.h"

namespace VAN_MAASTRICHT {
	Matrix::Matrix() {
		for(unsigned int i = 0; i < size; i++) {
			mat[i] = 0;
			mask[i] = (unsigned int)~0;
		}
	}
	
	// j is the column of the last edge added.
	bool Matrix::check_squares(unsigned int j) {
		const unsigned int VERTEX_ONE_BIT = 1 << (size - 1);

		for(unsigned int i = 0; i < size; i++) {
		// 	if((i != j) && (((N >> j) & mat[i]) == 0) && __builtin_popcount(mat[i] & mat[j]) > 1) {
		// 		return true;
		// 	}
			if( i == j ) continue;
			const unsigned int V = mat[ j ];
			const unsigned int W = mat[ i ];

			if( (((VERTEX_ONE_BIT >> i) & V) == 0) && (__builtin_popcount(W & V) > 1) ) return true;
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

	void Matrix::remove_entry(uint32_t i, uint32_t j) {
		mat[i] &= ((unsigned int)~0 - (unsigned int)(1 << (size - j - 1)));
		mat[j] &= ((unsigned int)~0 - (unsigned int)(1 << (size - i - 1)));
	}
	
	// set the ith row to the uint32_t j
	void Matrix::set_row(unsigned int i, uint32_t j) {
		if(i < 0 || i >= size) {
			return;
		}
		mat[i] = j;
	}

	void Matrix::mask_remove_entry(unsigned int i, unsigned int j) {
		mask[i] &= ((unsigned int)~0 - (unsigned int)(1 << (size - j - 1)));
		mask[j] &= ((unsigned int)~0 - (unsigned int)(1 << (size - i - 1)));
	}

	void Matrix::mask_set_row(unsigned int i, uint32_t j) {
		if(i < 0 || i >= size) {
			return;
		}
		mask[i] = j;
	}

	// get the entry at i,j
	uint32_t Matrix::get_entry(unsigned int i, unsigned int j) {
		return (mat[i] & (N >> j)) >> (size - j - 1);
	}

	uint32_t Matrix::get_mask_entry(unsigned int i, unsigned int j) {
		return (mask[i] & (N >> j)) >> (size - j - 1);
	}

	// deprecated
	uint32_t Matrix::get_depth() {
		return (mat[0] & DEPTH_MASK);
	}

	uint32_t Matrix::count_ones_mask() {
		unsigned int count = 0;
		for(unsigned int i = 0; i < size; i++) {
			count += __builtin_popcount(mask[i]);
		}
		return (count >> 1);
	}

	void Matrix::calculate_mask_base() {
		for(unsigned int i = 0; i < size; i++) {
			mask[i] &= ~(1 << (size - i - 1));
		}
		
		for(unsigned int i = 0; i < size; i++) {
			for(unsigned int j = i + 1; j < size; j++) {
				if(i < 10) {
					mask_remove_entry(i, j);
				}
				if(get_entry(i, j) != 0 ) {
					mask_remove_entry(i, j);
				}
				else {
					set_entry(i, j);
					if(check_triangles(i, j)) {
						mask_remove_entry(i, j);
						remove_entry(i, j);
						continue;
					}
					if(check_squares(j)) {
						mask_remove_entry(i, j);
						remove_entry(i, j);
						continue;
					}
					remove_entry(i, j);
				}
			}
		}
	}

	void Matrix::calculate_mask(unsigned int i0, unsigned int j0) {
		mask_remove_entry(i0, j0);
		uint32_t row;
		unsigned int j;
		for(unsigned int i = 0; i < size; i++) {
			row = get_mask_row(i);
			while(row > 0) {
				j = __builtin_clz(row);
				//cout << "R: " << row << " " << bitset<32>(row) << " " << j << endl;
				if(j < i) {
					row &= ((unsigned int)~0 - (unsigned int)(1 << (size - j - 1)));
					//cout << "T: " << row << " " << bitset<32>(row) << " " << j << endl;
					continue;
				}
				set_entry(i, j);
				if(check_triangles(i, j)) {
					mask_remove_entry(i, j);
					remove_entry(i, j);
					row &= ((unsigned int)~0 - (unsigned int)(1 << (size - j - 1)));
					continue;
				}
				if(check_squares(j)) {
					mask_remove_entry(i, j);
					remove_entry(i, j);
					row &= ((unsigned int)~0 - (unsigned int)(1 << (size - j - 1)));
					continue;
				}
				remove_entry(i, j);
				row &= ((unsigned int)~0 - (unsigned int)(1 << (size - j - 1)));
				//cout << "Q: " << row << " " << bitset<32>(row) << " " << j << endl;
			}
		}
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

	// get the ith row of the mask
	const uint32_t Matrix::get_mask_row(unsigned int i) const {
		if(i >= size || i < 0) {
			return -1;
		}
		return mask[i];
	}

	// get the ith row of the mask
	uint32_t Matrix::get_mask_row(unsigned int i) {
		if(i >= size || i < 0) {
			return -1;
		}
		return mask[i];
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
		outs << "{";
		for(unsigned int i = 0; i < mat.get_size() - 1; i++) {
			outs << mat.get_row(i) << ", ";
		}
		outs << mat.get_row(mat.get_size() - 1) << "}{";
		for(unsigned int i = 0; i < mat.get_size() - 1; i++) {
			outs << mat.get_mask_row(i) << ", ";
		}
		outs << mat.get_mask_row(mat.get_size() - 1) << "}";
		return outs;
	}
}
