#include <cstdlib>
#include <stack>
#include <ctime>
#include "Matrix.h"

namespace VAN_MAASTRICHT {
	class Explorer {
	public:
		Explorer();

		void explore(bool backup);
		void explore(unsigned int max_search_depth, bool backup);
		void explore(unsigned int max_search_depth, string out_file, bool backup);
		void save_stack(string str);
		void save_stack_intermediate(string str);
		void save_stack(string str, stack<Matrix> s);
		void read_stack(string str);
		void generate_children();
		
		bool continue_heuristics();
		bool degree_sat();
		void check_valid(Matrix &m);
		bool triangles_exist();
		bool squares_exist();

		time_t get_current_time(); // returns current time since midnight

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
