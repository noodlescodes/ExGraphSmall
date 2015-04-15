#include <bitset>
#include <iostream>
#include <fstream>
#include "Converter.h"

namespace VAN_MAASTRICHT {
	Converter::Converter() {
	}

	void Converter::bin_to_dec(string input_file, string output_file) {
		uint32_t N = 1 << 31;
		ifstream in;
		ofstream out;
		in.open(input_file);
		out.open(output_file);
		if(in.is_open()) {
			string line;
			while(getline(in, line)) {
				uint32_t j = 0;
				for(unsigned int k = 0; k < line.length(); k++) {
					if(line[k] == '1') {
						j |= (N >> k);
					}
				}
				out << j << endl;
			}
		}
		in.close();
		out.close();
	}

	void Converter::dec_to_bin(string input_file, string output_file) {
		ifstream in;
		ofstream out;
		in.open(input_file);
		out.open(output_file);
		if(in.is_open()) {
			string line;
			while(getline(in, line)) {
				uint32_t j = atoi(line.c_str());
				out << bitset<32>(j) << endl;
			}
		}
		in.close();
		out.close();
	}
}
