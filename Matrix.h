#ifndef VM_MATRIX
#define VM_MATRIX

#include <cstdlib>
#include <ostream>
#include <cstdint>

using namespace std;

namespace VAN_MAASTRICHT {
	class Matrix {
	public:
		// constructor
		Matrix();

		bool check_squares(unsigned int j);
		bool check_triangles(unsigned int i, unsigned int j);
		void set_entry(uint32_t i, uint32_t j);
		void remove_entry(uint32_t i, uint32_t j);
		void set_row(unsigned int i, uint32_t j);
		void mask_remove_entry(unsigned int i, unsigned int j);
		void mask_set_row(unsigned int i, uint32_t j);
		void set_number_edges(unsigned int num);
		uint32_t get_entry(unsigned int i, unsigned int j);
		uint32_t get_mask_entry(unsigned int i, unsigned int j);
		uint32_t get_depth();
		uint32_t count_ones_mask();
		uint32_t get_mask_row_first_one();
		void calculate_mask_base();
		void calculate_mask(unsigned int i0, unsigned int j0); // i0 and j0 are the coordinates of the last added edge
		
		const unsigned int get_size() const;
		unsigned int get_size();
		const uint32_t get_row(unsigned int i) const;
		uint32_t get_row(unsigned int i);
		const uint32_t get_mask_row(unsigned int i) const;
		uint32_t get_mask_row(unsigned int i);

		unsigned int get_degree(unsigned int i);
		unsigned int get_number_edges();
		void calculate_number_edges();

	private:
		static const unsigned int size = 32;
		static const uint32_t N = 1 << (size - 1);
		static const uint32_t DEPTH_MASK = (~((uint32_t) 0) >> (32 - 22));
		uint32_t mat[size];
		uint32_t mask[size];
		uint32_t number_of_edges; // 2 * total edges in base graph

		friend istream& operator >>(istream&, Matrix& );
	}; // class Matrix
	
	// overloaded operators
	// ostream& operator >>(ostream&, const Matrix& );
	ostream& operator <<(ostream& outs, const Matrix& mat);
}
#endif
