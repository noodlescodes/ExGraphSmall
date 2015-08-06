#include <iostream>
#include <algorithm>
#include <bitset>
#include <queue>
#include <thread>
#include "Explorer.h"

using namespace std;

namespace VAN_MAASTRICHT {
	Explorer::Explorer(unsigned int number_of_threads) {
		// stacks[0] is the main stack, every stack
		// after that is a thread stack
		stacks.resize(1 + number_of_threads);
	}

	void Explorer::pretty_print(Matrix &m) {
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
			else cout << i << "  " << bitset<32>(m.get_row(i)) << "   " << bitset<32>(m.get_mask_row(i)) << endl;
		}
		cout << endl << "Num ones: " << m.count_ones_mask() << endl;
	}

	void Explorer::add_matrix_to_stack(Matrix &m) {
		stacks[0].push(m);
	}

	void Explorer::transfer_queue_to_stack() {
		while(!initial_queue.empty()) {
			stacks[0].push(initial_queue.front());
			initial_queue.pop();
		}
	}

	void Explorer::sub_stack_get_data(unsigned int thread_id, unsigned int number_of_elements) {
		mtx.lock();
		//cout << "Thread " << thread_id << " getting data" << endl;
		while(!stacks[0].empty() && stacks[thread_id].size() < number_of_elements) {
			stacks[thread_id].push(stacks[0].top());
			stacks[0].pop();
		}
		//cout << "Thread id: " << thread_id << endl;
		//for(unsigned int i = 0; i < stacks.size(); i++) {
		//	cout << "Stack[" << i << "] size: " << stacks[i].size() << endl;
		//}
		//cout << "stacks[0] size: " << stacks[0].size() << endl;
		//cout << "Stack[" << thread_id << "] size: " << stacks[thread_id].size() << endl;
		mtx.unlock();
	}

	void Explorer::breadth_first_search(Matrix &m, unsigned int min_stack_size, unsigned int max_depth) {
		Matrix matrix;
		initial_queue.push(m);
		while(initial_queue.size() < min_stack_size && !initial_queue.empty()) {
			matrix = initial_queue.front();
			initial_queue.pop();
			if(matrix.get_depth() < max_depth) {
				generate_children_queue(matrix);
			}
		}
	}

	void Explorer::depth_first_search(unsigned int thread_id, unsigned int max_depth) {
		if(thread_id > stacks.size()) {
			cout << "You broke everything! thread_id > number of stacks" << endl;
			return;
		}

		Matrix matrix;
		unsigned int nodes_searched = 0;
		while(!stacks[0].empty() || !stacks[thread_id].empty()) {
			while(!stacks[thread_id].empty()) {
				matrix = stacks[thread_id].top();
				stacks[thread_id].pop();
				if(matrix.count_ones_mask() == 0) {
					// this is a solution
				}
				else if(matrix.get_depth() == max_depth) {
					// do nothing
				}
				else {
					generate_children_stack(matrix, thread_id);
				}
				nodes_searched++;
			}
			sub_stack_get_data(thread_id, 10);
		}
		
		//cout << "Nodes searched by thread " << thread_id << ": " << nodes_searched << endl;
	}

        void Explorer::generate_children_queue(Matrix &m) {
		const unsigned int MINEDGES = 82;
		const unsigned int MAXEDGES = 84;
		const unsigned int MINDEGREE = 3;
		const unsigned int MAXDEGREE = 5;

		unsigned int depth = m.get_depth();
		m.set_row(0, m.get_row(0) - depth);
		depth++;

		unsigned int edgecount = m.get_number_edges();

		unsigned int row = m.get_mask_row_first_one();
		unsigned int col = __builtin_clz(m.get_mask_row(row));

		m.mask_remove_entry(row, col);

		// left child
		bool err = false;

		err |= (edgecount + m.count_ones_mask() < MINEDGES);
		err |= (m.get_degree(row) + __builtin_popcount(m.get_mask_row(row)) < MINDEGREE);

		m.set_row(0, m.get_row(0) + depth);
		if(!err) {
			initial_queue.push(m);
		}

		// right child
		err = false;
		m.set_row(0, m.get_row(0) - depth);

		m.set_entry(row, col);
		m.set_number_edges((edgecount << 1) + 2);
		++edgecount;

		err |= (edgecount > MAXEDGES);

		err |= (edgecount + m.count_ones_mask() < MINEDGES);

		err |= (m.get_degree(row) > MAXDEGREE);
		err |= (m.get_degree(col) > MAXDEGREE);

		err |= (m.get_degree(row) + __builtin_popcount(m.get_mask_row(row)) < MINDEGREE);

		if(!err) {
			m.calculate_mask(row, col);
			m.set_row(0, m.get_row(0) + depth);
			initial_queue.push(m);
		}
	}

	void Explorer::generate_children_stack(Matrix &m, unsigned int thread_id) {
		const unsigned int MINEDGES = 82;
		const unsigned int MAXEDGES = 84;
		const unsigned int MINDEGREE = 3;
		const unsigned int MAXDEGREE = 5;

		unsigned int depth = m.get_depth();
		m.set_row(0, m.get_row(0) - depth);
		depth++;

		unsigned int edgecount = m.get_number_edges();

		unsigned int row = m.get_mask_row_first_one();
		unsigned int col = __builtin_clz(m.get_mask_row(row));

		m.mask_remove_entry(row, col);

		// left child
		bool err = false;

		err |= (edgecount + m.count_ones_mask() < MINEDGES);
		err |= (m.get_degree(row) + __builtin_popcount(m.get_mask_row(row)) < MINDEGREE);

		m.set_row(0, m.get_row(0) + depth);
		if(!err) {
			stacks[thread_id].push(m);
		}

		// right child
		err = false;
		m.set_row(0, m.get_row(0) - depth);

		m.set_entry(row, col);
		m.set_number_edges((edgecount << 1) + 2);
		++edgecount;

		err |= (edgecount > MAXEDGES);

		err |= (edgecount + m.count_ones_mask() < MINEDGES);

		err |= (m.get_degree(row) > MAXDEGREE);
		err |= (m.get_degree(col) > MAXDEGREE);

		err |= (m.get_degree(row) + __builtin_popcount(m.get_mask_row(row)) < MINDEGREE);

		if(!err) {
			m.calculate_mask(row, col);
			m.set_row(0, m.get_row(0) + depth);
			stacks[thread_id].push(m);
		}
	}
}
