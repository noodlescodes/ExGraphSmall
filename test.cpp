#include <iostream>
#include <fstream>
#include <stack>
#include <bitset>
#include "Matrix.h"

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
	uint32_t N = 1 << 31;
	cout << "N: " << bitset<32>(N) << endl;
	stack<Matrix> tmp_stack;
	Matrix m = Matrix();
	ifstream f;
	f.open(str);
	if(f.is_open()) {
		string line;
		unsigned int i = 0;
		while(getline(f, line)) {
			uint32_t j = 0;
			for(unsigned int k = 0; k < line.length(); k++) {
				if(line[k] == '1') {
					j |= (N >> k);
				}
			}
			m.set_row(i, j);
			i++;
			i %= 32;
			if(i == 0) {
				tmp_stack.push(m);
			}
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
	Matrix m = Matrix();
	m.set_entry(0,29);
	m.set_entry(0,31);
	m.set_entry(2,29);
	m.set_entry(2,31);
	
	stack<Matrix> dfs_stack;
	dfs_stack.push(m);
	m.set_entry(0,0);
	dfs_stack.push(m);

	stack_to_file("test.txt", dfs_stack);

	while(!dfs_stack.empty()) {
		//cout << dfs_stack.top() << endl << endl;
		dfs_stack.pop();
	}

	stack<Matrix> test = stack_from_file("test.txt");

	cout << "test" << endl;
	while(!test.empty()) {
		cout << test.top() << endl << endl;
		test.pop();
	}

	return 0;
}
