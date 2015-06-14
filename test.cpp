#include <iostream>
#include <fstream>
#include <stack>
#include <bitset>
#include <ctime>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include "Matrix.h"
#include "Explorer.h"
#include "Converter.h"

using namespace std;
using namespace VAN_MAASTRICHT;

stack<Matrix> stack_from_file(string str) {
	stack<Matrix> tmp_stack;
	Matrix m = Matrix();
	ifstream f;
	f.open(str);
	if(f.is_open()) {
		string line;
		while(getline(f, line)) {
			line = line.substr(1, line.length() - 2);
			line.erase(remove(line.begin(), line.end(), ' '), line.end());
			stringstream lineStream(line);
			string num;
			for(uint i = 0; i < 32; i++) {
				getline(lineStream, num, ',');
				m.set_row(i, atoi(num.c_str()));
			}
			cout << m << endl;
			tmp_stack.push(m);
		}
	}
	f.close();

	stack<Matrix> dfs_stack;
	while(!tmp_stack.empty()) {
		dfs_stack.push(tmp_stack.top());
		tmp_stack.pop();
	}

	return dfs_stack;
}

Matrix read_from_file(string str) {
	Matrix m = Matrix();
	ifstream f;
	f.open(str);
	if(f.is_open()) {
		string line, mat, mask;
		getline(f, line);
		unsigned int pos = line.find("}") + 1;
		mat  = line.substr(0, pos);
		mask = line.substr(pos);
		mat = mat.substr(1, mat.length() - 2);
		mat.erase(remove(mat.begin(), mat.end(), ' '), mat.end());
		mask = mask.substr(1, mask.length() - 2);
		mask.erase(remove(mask.begin(), mask.end(), ' '), mask.end());
		stringstream mat_stream(mat);
		stringstream mask_stream(mask);
		string num;
		for(unsigned int i = 0; i < 32; i++) {
			getline(mat_stream, num, ',');
			m.set_row(i, atoi(num.c_str()));
			getline(mask_stream, num, ',');
			m.mask_set_row(i, atoi(num.c_str()));
		}
	}

	return m;
}

int main(int argc, char* argv[]) {
	Matrix m = read_from_file("base.txt");

	for(unsigned int i = 16; i < 32; i++) {
		for(unsigned int j = i; j < 32; j++) {
			if(m.get_mask_entry(i, j) == 1) {
				m.mask_remove_entry(i, j);
			}
		}
	}
	for(unsigned int j = 31; j < 32; j++) {
		if(m.get_mask_entry(15, j) == 1) {
			m.mask_remove_entry(15, j);
		}
	}
	
	cout << m << endl;

	cout << "    ";
	for(unsigned int i = 0; i < 32; i++) {
		cout << i % 10;
	}
	cout << "   ";
	for(unsigned int i = 0; i < 32; i++) {
		cout << i % 10;
	}
	cout << endl << endl;
	for(unsigned int i = 0; i < 32; i++) {
		if(i < 10) {
		cout << i << "   " << bitset<32>(m.get_row(i)) << "   " << bitset<32>(m.get_mask_row(i)) << endl;
		}
		else {
			cout << i << "  " << bitset<32>(m.get_row(i)) << "   " << bitset<32>(m.get_mask_row(i)) << endl;
		}
	}
	cout << endl << "Num ones: " << m.count_ones_mask() << endl;

	Explorer e = Explorer();
	e.add_matrix_to_stack(m);
	e.explore(false);
	
	/*string input_file;
	string output_file = "";
	unsigned int max_search_depth = 0;
	bool save_to_file = false;

	if(argc < 3) {
		cout << "Need input (-i) <file>" << endl;
		cout << "Optional switches:" << endl;
		cout << " * -d <number> for depth" << endl;
		cout << " * -o <file> for output file" << endl;
		cout << " * -s <true/false> to make a backup of the main stack every hour" << endl;
		return 0;
	}

	for(int i = 1; i < argc; i += 2) {
		if(i + 1 != argc) {
			if(string(argv[i]) == "-d") {
				max_search_depth = atoi(argv[i + 1]);
				cout << "search depth: " << max_search_depth << endl;
			}
			else if(string(argv[i]) == "-i") {
				input_file = argv[i + 1];
				cout << "input file: " << input_file << endl;
			}
			else if(string(argv[i]) == "-o") {
				output_file = argv[i + 1];
				cout << "output file: " << output_file << endl;
			}
			else if(string(argv[i]) == "-s") {
				if(string(argv[i + 1]) == "true") {
					save_to_file = true;
				}
				if(save_to_file) {
					cout << "Save to file: true" << endl;
				}
				else {
					cout << "Save to file: false" << endl;
				}
			}
		}
	}

	cout << endl;
	
	Explorer e = Explorer();
	cout << "Reading from base.txt." << endl;
	e.read_stack(input_file);
	cout << "Starting to explore." << endl;
	if(max_search_depth > 0) {
		if(output_file.length() > 0) {
			e.explore(max_search_depth, output_file, save_to_file);
		}
		else {
			e.explore(max_search_depth, save_to_file);
		}
	}
	else {
		e.explore(save_to_file);
	}

	if(output_file.length() > 0 && max_search_depth == 0) {
		cout << "Saving the stack." << endl;
		e.save_stack(output_file);
		}*/

	//Converter c = Converter();
	//cout << "Converting the output file." << endl;
	//c.bin_to_dec("d56_stack.txt", "d56_stack_dec.txt");
	//cout << "Fin" << endl;

	/*Matrix m = Matrix();
	m.set_row(0,130023424);
	m.set_row(1,4063232);
	m.set_row(2,126976);
	m.set_row(3,3968);
	m.set_row(4,124);
	m.set_row(5,2149648448);
	m.set_row(6,2148566048);
	m.set_row(7,2148024848);
	m.set_row(8,2147754248);
	m.set_row(9,2147618948);
	m.set_row(10,1140850688);
	m.set_row(11,1107296256);
	m.set_row(12,1090519040);
	m.set_row(13,1082130432);
	m.set_row(14,1077936128);
	m.set_row(15,603979776);
	m.set_row(16,570425344);
	m.set_row(17,553648128);
	m.set_row(18,545259520);
	m.set_row(19,541065216);
	m.set_row(20,335544320);
	m.set_row(21,301989888);
	m.set_row(22,285212672);
	m.set_row(23,276824064);
	m.set_row(24,272629760);
	m.set_row(25,201326592);
	m.set_row(26,167772160);
	m.set_row(27,150994944);
	m.set_row(28,142606336);
	m.set_row(29,138412032);
	m.set_row(30,0);
	m.set_row(31,0);

	dfs_stack.push(m);
	stack_to_file("test.txt", dfs_stack);*/
	
	return 0;
}
