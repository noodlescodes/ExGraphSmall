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
		void set_row(unsigned int i, uint32_t j);
		uint32_t get_entry(unsigned int i, unsigned int j);
		
		const unsigned int get_size() const;
		unsigned int get_size();
		const uint32_t get_row(unsigned int i) const;
		uint32_t get_row(unsigned int i);

		unsigned int get_degree(unsigned int i);
		unsigned int get_number_edges();

	private:
		static const unsigned int size = 32;
		static const uint32_t N = 1 << (size - 1);
		uint32_t mat[size];
	};
	// overloaded operators
	ostream& operator <<(ostream& outs, const Matrix& mat);
}
