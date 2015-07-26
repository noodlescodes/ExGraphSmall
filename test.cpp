#include <iostream>
#include <fstream>
#include <stack>
#include <bitset>
#include <ctime>
#include <string>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <thread>
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
	unsigned int num_threads = 1;
	unsigned int search_depth = 30;
	string base_file = "base.txt";
        
	if(argc < 3) {
		cout << "Optional switches:" << endl;
		cout << "Need input -i <file> (default base.txt)" << endl;
		cout << "-t <number> for number of threads (default 1)" << endl;
		cout << "-d <number> for search depth in tree (default 30)" << endl;
	}
	for(int i = 1; i < argc; i += 2) {
		if(i + 1 != argc) {
			if(string(argv[i]) == "-d") {
				search_depth = atoi(argv[i + 1]);
				cout << "Search depth: " << search_depth << endl;
			}
			else if(string(argv[i]) == "-i") {
				base_file = argv[i + 1];
				cout << "Base file: " << base_file << endl;
			}
			else if(string(argv[i]) == "-t") {
				num_threads = atoi(argv[i + 1]);
				cout << "Number of threads: " << num_threads << endl;
			}
		}
	}

	Matrix m = read_from_file(base_file);
	Explorer e = Explorer(num_threads);
	e.breadth_first_search(m, 40 * num_threads);
	e.transfer_queue_to_stack();

	vector<thread> threads;

	//for(unsigned int i = 1; i <= 3; i++) {
	//	threads.push_back(thread(&Explorer::sub_stack_get_data, &e, i, 10));
	//}

	for(unsigned int i = 1; i <= num_threads; i++) {
		threads.push_back(thread(&Explorer::depth_first_search, &e, i, search_depth));
	}
	for(unsigned int i = 0; i < threads.size(); i++) {
		threads[i].join();
	}

	cout << "done" << endl;

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
