#ifndef VM_CONVERTER
#define VM_CONVERTER

#include <cstdlib>
#include <cstdint>

using namespace std;

namespace VAN_MAASTRICHT {
	class Converter {
	public:
		Converter();
		void bin_to_dec(string input_file, string output_file);
		void dec_to_bin(string input_file, string output_file);
	private:
	};
}
#endif
