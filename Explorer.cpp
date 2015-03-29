#include <cstdlib>
#include <iostream>
#include <fstream>
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

	void Explorer::read_stack(string str) {
		uint32_t N = 1 << 31;
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

		while(!tmp_stack.empty()) {
			dfs_stack.push(tmp_stack.top());
			tmp_stack.pop();
		}
	}

	void Explorer::explore() {
		/*while(number_of_solutions == 0 && !dfs_stack.empty()) {
			Matrix matrix = dfs_stack.top();
			dfs_stack.pop();
			check_valid(matrix);
		}*/
		Matrix matrix;
		for(int i = 0; i < 1; i++) {
			matrix = dfs_stack.top();
			dfs_stack.pop();
			check_valid(matrix);
		}

		cout << dfs_stack.size() << endl;
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

		unsigned int depth = (m.get_row(0) & DEPTHMASK);
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
		bool err = false;
		err |= (edgecount + remainingedges < MINEDGES);

		err |= ((col == VERTEXCOUNT - 1) && (m.get_degree(row)));

		if(err) {
			m.set_row(0, m.get_row(0) | FAILMASK);
		}

		m.set_row(0, m.get_row(0) + depth);
		if(m.get_entry(0, 0) == 0) {
			dfs_stack.push(m);
		}

		// now for right child
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
			cout << "right err" << endl;
		}
		
		m.set_row(0, m.get_row(0) + depth);
		if(m.get_entry(0, 0) == 0) {
			dfs_stack.push(m);
		}
	}
}
