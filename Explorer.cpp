#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <sstream>
#include "Explorer.h"

using namespace std;

namespace VAN_MAASTRICHT {
	Explorer::Explorer() {
	}

	void Explorer::save_stack(string str) {
		ofstream f;
		f.open(str, ios::app);
		while(!dfs_stack.empty()) {
			f << dfs_stack.top() << endl;
			dfs_stack.pop();
		}
		f.close();
	}

	void Explorer::save_stack_intermediate(string str) {
		stack<Matrix> tmp;
		ofstream f;
		f.open(str);
		Matrix m;
		while(!dfs_stack.empty()) {
			f << dfs_stack.top() << endl;
			m = dfs_stack.top();
			tmp.push(m);
			dfs_stack.pop();
		}

		while(!tmp.empty()) {
			m = tmp.top();
			dfs_stack.push(m);
			tmp.pop();
		}
	}

	void Explorer::save_stack(string str, stack<Matrix> s) {
		ofstream f;
		f.open(str, ios::app);
		while(!s.empty()) {
			f << s.top() << endl;
			s.pop();
		}
		f.close();
	}

	void Explorer::read_stack(string str) {
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
				tmp_stack.push(m);
			}
		}
		f.close();

		while(!tmp_stack.empty()) {
			dfs_stack.push(tmp_stack.top());
			tmp_stack.pop();
		}
	}

	time_t Explorer::get_current_time() {
		time_t t1, t2;
		struct tm tms;
		time(&t1);
		localtime_r(&t1, &tms);
		tms.tm_hour = 0;
		tms.tm_min = 0;
		tms.tm_sec = 0;
		t2 = mktime(&tms);
		return t1 - t2;
	}

	void Explorer::explore() {
		/*while(number_of_solutions == 0 && !dfs_stack.empty()) {
			Matrix matrix = dfs_stack.top();
			dfs_stack.pop();
			check_valid(matrix);
		}*/
		Matrix matrix;
		/*for(int i = 0; i < 25000000; i++) {
			matrix = dfs_stack.top();
			dfs_stack.pop();
			check_valid(matrix);
		}
		cout << "Elements in stack: " << dfs_stack.size() << endl;
		cout << "Depth of top element: " << (matrix.get_row(0) & (~((uint32_t) 0) >> (32 - 22))) << endl;*/
	
		//stack<Matrix> d_stack;
		unsigned long nodes_searched = 0;
		unsigned long nodes_at_130 = 0;
		//int hour = 1;
		while(!dfs_stack.empty()) {
			matrix = dfs_stack.top();
			dfs_stack.pop();
			if(matrix.get_depth() == 90) {
				//d_stack.push(matrix);
				nodes_at_130++;
			}
			else {
				check_valid(matrix);
			}
			nodes_searched++;
			/*if(nodes_searched % 50000000 == 0) {
				cout << "Nodes searched (probably overflow): " << nodes_searched << endl;
				if((hour > 0) && (hour * 3600) < get_current_time()) {
					cout << "saving stack " << hour << endl;
					save_stack_intermediate("dfs_stack" + to_string(hour) + ".txt");
					hour++;
					cout << "stack " << hour - 1 << " saved" << endl;
				}
				else if((hour == 0) && (get_current_time() < 3600)) {
					cout << "saving stack " << hour << endl;
					save_stack_intermediate("dfs_stack" + to_string(hour) + ".txt");
					hour++;
					cout << "stack " << hour - 1 << " saved" << endl;
				}
				}*/
		}
		//matrix = d_stack.top();
		
		//save_stack("d55_stack.txt", d_stack);
		//cout << "d_stack saved to \"d55_stack.txt\"" << endl;
		cout << "Elements in stack: " << nodes_at_130 << endl;
		//cout << "Depth of top element: " << (matrix.get_depth()) << endl;
		cout << "Nodes searched: " << nodes_searched << endl;
	}

	void Explorer::check_valid(Matrix &m) {
		const unsigned int VERTEXCOUNT = 32;
		const unsigned int DEPTHMASK = (~((uint32_t) 0) >> (32 - 22));
		const unsigned int MAXDEPTH = 190;
		const unsigned int MINEDGES = 82;
		const unsigned int MAXEDGES = 84;
		const unsigned int MINDEGREE = 3;
		const unsigned int MAXDEGREE = 5;

		const unsigned int VERTEX_ONE_BIT = 1 << (VERTEXCOUNT - 1);
		const unsigned int FAILMASK = VERTEX_ONE_BIT;

		if(m.get_entry(0, 0) == 1) {
			return;
		}

		const unsigned int RESTOREMASK = (~FAILMASK) | (FAILMASK & m.get_row(0));

		// cout << m << endl << endl;

		//unsigned int depth = (m.get_row(0) & DEPTHMASK);
		uint32_t depth = m.get_depth();
		m.set_row(0, m.get_row(0) & (~DEPTHMASK));
		
		unsigned int edgecount = m.get_number_edges(); // gives number of edges without double counting
		
		unsigned int row = 10;
		unsigned int col = 15;
		for(unsigned int i = depth, K = 17; K > 0; i -= 5 * K, K -= 5, row += 5, col += 5) {
			if(i < 5 * K) {
				row += i / K;
				col += (i % K);
				break;
			}
		}
		if(col > 31) {
			col = 31;
		}

		unsigned int remainingedges = MAXDEPTH - depth;
		++depth;

		// left child
		/*		bool err = false;
		err |= (edgecount + remainingedges < MINEDGES);

		err |= ((col == VERTEXCOUNT - 1) && (m.get_degree(row) < MINDEGREE));

		if(err) {
			m.set_row(0, m.get_row(0) | FAILMASK);
		}

		m.set_row(0, m.get_row(0) + depth);
		if(m.get_entry(0, 0) == 0) {
			// cout << '[';
			dfs_stack.push(m);
		}
		// else cout << ']';
		*/
		bool err = false;
		if(!(edgecount + remainingedges < MINEDGES)) {
			if(((col == VERTEXCOUNT - 1) && (m.get_degree(row) < MINDEGREE))) {
				err = true;
			}
		}
		else {
			err = true;
		}
		if(err) {
			m.set_row(0, m.get_row(0) | FAILMASK);
		}

		m.set_row(0, m.get_row(0) + depth);
		if(m.get_entry(0, 0) == 0) {
			dfs_stack.push(m);
		}

		// now for right child
		m.set_row(0, m.get_row(0) - depth);
		err = false;
		m.set_row(0, m.get_row(0) & RESTOREMASK);

		m.set_entry(row, col);
		++edgecount;

		err |= (edgecount > MAXEDGES);

		err |= (edgecount + remainingedges < MINEDGES);

		err |= (m.get_degree(row) > MAXDEGREE);
		err |= (m.get_degree(col) > MAXDEGREE);

		err |= ((col == VERTEXCOUNT - 1) && (m.get_degree(row) < MINDEGREE));

		err |= m.check_triangles(row, col);

		err |= m.check_squares(col);

		if(err) {
			m.set_row(0, m.get_row(0) | FAILMASK);
		}
		
		m.set_row(0, m.get_row(0) + depth);
		if(m.get_entry(0, 0) == 0) {
			// cout << '(';
			dfs_stack.push(m);
		}
		// else cout << ')';
	}
}
