#include <cstdlib>
#include <stack>
#include <queue>
#include <vector>
#include <mutex>
#include "Matrix.h"

namespace VAN_MAASTRICHT {
	class Explorer {
	public:
		Explorer(unsigned int minEdges, unsigned int maxEdges, unsigned int minDegree, unsigned int maxDegree, unsigned int number_of_threads = 1);

		// Functions related to explorering the tree
		void breadth_first_search(queue<Matrix> &IQ, unsigned int min_stack_size, unsigned int max_depth);
		void depth_first_search(unsigned int thread_id, unsigned int max_depth);
		void generate_children_queue(Matrix &m);
		void generate_children_stack(Matrix &m, unsigned int thread_id);

		// stack manipulation functions
		void add_matrix_to_stack(Matrix &m);
		void transfer_queue_to_stack();
		void sub_stack_get_data(unsigned int thread_id, unsigned int number_of_elements);
		
		// IO functions
		static void pretty_print(Matrix &m);

		// Solution Return
		queue<Matrix> solutions( ) { return initial_queue; }
		unsigned int get_min_edges( ) const { return MINEDGES; }

		// never need to copy and if I don't have a copy constructor the mutex chucks a fit.
		// yes, this is bad practise and will come back to bite me.f
		// Sorry future Nathan
		Explorer(const Explorer& e)
			: MINEDGES( e.MINEDGES ), MAXEDGES( e.MAXEDGES ), MINDEGREE( e.MINDEGREE ), MAXDEGREE( e.MAXDEGREE ) {}

	private:
		vector<stack<Matrix> > stacks;
		queue<Matrix> initial_queue; // this is a global variable so it doesn't need to be passed around functions
		mutex mtx, solnMtx;
		unsigned int number_of_solutions;

		unsigned int MINEDGES;
		const unsigned int MAXEDGES;
		const unsigned int MINDEGREE;
		const unsigned int MAXDEGREE;

	};
}
