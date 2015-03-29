#include <cstdlib>
#include <stack>
#include "Matrix.h"

namespace VAN_MAASTRICHT {
	class Explorer {
	public:
		Explorer();

		void explore();
		void save_stack(string str);
		void read_stack(string str);
		void generate_children();
		
		bool continue_heuristics();
		bool degree_sat();
		void check_valid(Matrix &m);
		bool triangles_exist();
		bool squares_exist();

	private:
		unsigned int max_edges_found = 0;
		unsigned int min_edges_allowed = 87;
		unsigned int max_edges_allowed = 89;
		unsigned int number_of_solutions = 0;
		unsigned int max_depth = 190;
		unsigned int max_degree = 5;
		unsigned int min_degree = 3;
		stack<Matrix> dfs_stack;
	};
}
