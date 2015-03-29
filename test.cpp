#include <iostream>
#include <fstream>
#include <stack>
#include <bitset>
#include "Matrix.h"
#include "Explorer.h"

using namespace std;
using namespace VAN_MAASTRICHT;

unsigned int number_of_solutions = 0;

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
	/*Matrix m = Matrix();
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
	}*/

	Explorer e = Explorer();
	e.read_stack("test.txt");
	e.explore();
	e.save_stack("test2.txt");

	/*stack<Matrix> dfs_stack;
	Matrix m = Matrix();
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
