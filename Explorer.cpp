#include <iostream>
#include <algorithm>
#include <bitset>
#include <queue>
#include <thread>
#include "Explorer.h"

using namespace std;

namespace VAN_MAASTRICHT {
	Explorer::Explorer(unsigned int minEdges, unsigned int maxEdges, unsigned int minDegree, unsigned int maxDegree, unsigned int number_of_threads) 
		: MINEDGES( minEdges ), MAXEDGES( maxEdges ), MINDEGREE( minDegree ), MAXDEGREE( maxDegree )
	{
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

	void Explorer::breadth_first_search(queue<Matrix> &IQ, unsigned int min_stack_size, unsigned int max_depth) {
		Matrix matrix;
		// initial_queue.push(m);
		// Copy the contents of IQ to initial_queue.
		initial_queue = IQ;

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
					// this is a solution (but not necessarily maximal)

					// Use the mutex to make sure we are the only process updating the solutions queue, and that no other process is updating MINEDGES.
					solnMtx.lock();

					// First, make sure we want to keep this solution. 
					if( matrix.get_number_edges() < MINEDGES ) { solnMtx.unlock(); continue; }

					// First, make sure we haven't found a new, better solution. This *should* be impossible, but the code is left here to be sure.
					if( matrix.get_number_edges() > MINEDGES ) {
						// We have found a better solution than the current crop.
						cout << "New Solution with " << matrix.get_number_edges() << " edges" << endl;
						// Update the minimum edges for the rest of the search.
						MINEDGES = matrix.get_number_edges();
						// Clear the queue to put in our new best solutions.
						queue<Matrix> empty;
						swap(initial_queue, empty);
					}

					// Put the solution into the initial_queue.
					matrix.set_row(0, matrix.get_row(0) - matrix.get_depth());
					initial_queue.push( matrix );

					// Unlock the mutex.
					solnMtx.unlock();
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
		
		// cout << "Nodes searched by thread " << thread_id << ": " << nodes_searched << endl;
	}

    void Explorer::generate_children_queue(Matrix &m) {

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

		err |= (edgecount + m.count_ones_mask() < MINEDGES); // SHOULD THIS BE edgecount + m.count_ones_mask() - 1 ?? (Since the mask appears not to have been modified).

		err |= (m.get_degree(row) > MAXDEGREE);
		err |= (m.get_degree(col) > MAXDEGREE);

		err |= (m.get_degree(row) + __builtin_popcount(m.get_mask_row(row)) < MINDEGREE);

		if(!err) {
			m.calculate_mask(row, col);

			m.set_row(0, m.get_row(0) + depth);
			initial_queue.push(m);

			// Check to see if we've found a grpah with more edges than previously found.
			if( m.get_number_edges() > MINEDGES ) {
				solnMtx.lock();
				// Check again, just in case another thread beat us to the punch.
				if( m.get_number_edges() > MINEDGES ) {
					cout << "(Q) New minimum: " << m.get_number_edges() << " edges." << endl;
					// Update the minimum edges for the rest of the search.
					MINEDGES = m.get_number_edges();
					// Clear the solutions queue.
					queue<Matrix> empty;
					swap(initial_queue, empty);
				}
				solnMtx.unlock();
			}
		}
	}

	void Explorer::generate_children_stack(Matrix &m, unsigned int thread_id) {
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
			// if( m.check_connected() ) {
			// 	m.set_row(0, m.get_row(0) + depth);
				stacks[thread_id].push(m);
			// }
			// else {
			// 	// mtx.lock(); cout << "(T" << thread_id << ") Pruning disconnected graph" << endl; mtx.unlock();
			// 	m.set_row(0, m.get_row(0) + depth);
			// }
		}

		// right child
		err = false;
		m.set_row(0, m.get_row(0) - depth);

		m.set_entry(row, col);
		m.set_number_edges((edgecount << 1) + 2);
		++edgecount;

		err |= (edgecount > MAXEDGES);

		err |= (edgecount + m.count_ones_mask() < MINEDGES); // This needs to be moved to after the calculate_mask( ) function.

		err |= (m.get_degree(row) > MAXDEGREE);
		err |= (m.get_degree(col) > MAXDEGREE);

		err |= (m.get_degree(row) + __builtin_popcount(m.get_mask_row(row)) < MINDEGREE);

		if(!err) {
			m.calculate_mask(row, col); 


			m.set_row(0, m.get_row(0) + depth);
			stacks[thread_id].push(m);

			// Check to see if we've found a grpah with more edges than previously found.
			if( m.get_number_edges() > MINEDGES ) {
				solnMtx.lock();
				// Check again, just in case somebody beat us to the punch.
				if( m.get_number_edges() > MINEDGES ) {
					cout << "(T" << thread_id << ") New minimum: " << m.get_number_edges() << " edges." << endl;
					// Update the minimum edges for the rest of the search.
					MINEDGES = m.get_number_edges();
					// Clear the solutions queue.
					queue<Matrix> empty;
					swap(initial_queue, empty);
				}
				solnMtx.unlock();
			}
		}
	}
} // namespace VAN_MAASTRICHT
