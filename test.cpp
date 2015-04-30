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

void stack_to_file(string str, stack<Matrix> dfs_stack) {
	ofstream f;
	f.open(str, ios::app);
	while(!dfs_stack.empty()) {
		f << dfs_stack.top() << endl;
		dfs_stack.pop();
	}
	f.close();
}

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

int main(int argc, char* argv[]) {
	string input_file;
	string output_file = "";
	unsigned int max_search_depth = 0;
	bool save_to_file = false;

	if(argc < 3) {
		cout << "Need input (-i)" << endl;
		cout << "Optional switches:" << endl;
		cout << " * -d for depth" << endl;
		cout << " * -o <file> for output file" << endl;
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
			e.explore(max_search_depth, output_file);
		}
		else {
			e.explore(max_search_depth);
		}
	}
	else {
		e.explore();
	}

	//cout << "Saving the stack." << endl;
	//e.save_stack(output_file);

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
