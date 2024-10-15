#include <iostream>
#include "trashsystem2.hpp"
#include <unistd.h>

/*
 *
 * Trashing System (Version 2)
 *
 * A hopefully improved take on the previous trash
 * system that i made in C. By reimplementing this
 * in C++ i hope to avoid the bugs and code smell
 * that eventually cropped up in the last project.
 *
 * While i am optimistic at this point my pessimism
 * will probably prove me wrong because i will most
 * likely encounter other problems that will give
 * my code that nice funky smell.
 *
 */

char *g_argv = nullptr;

inline void usage_out(std::ostream &out) {
	
	out << USAGE;
}

inline void usage_out_long(std::ostream &out) {
	
	out << LONG_USAGE;
}

int main (int argc, char **argv) {

	g_argv = argv[0];
	if (argc == 1) {
		usage_out(std::cerr);
        return EXIT_FAILURE;
    }

	int r_mtx = 0;
	int C_mtx = 0;
	int c_mtx = 0;
	int L_mtx = 0;
	int l_mtx = 0;
	int y_mtx = 0;
	int n_mtx = 0;
	int f_mtx = 0;
	int h_mtx = 0;
	bool y_used;
	bool n_used;
	bool v_used;
	bool f_used;
	bool t_used;
	bool l_used;
	bool L_used;
	bool c_used;
	bool C_used;
	bool R_used;
	bool h_used;
	int opt = 0;
	while((opt = getopt(argc, argv, "ynvfatlLcCR:h")) != -1) {

		switch(opt) {
		case 'y':
			
			break;
		case 'n':

			break;
		case 'v':

			break;
		case 'f':
			
			break;
		case 't':

			break;
		case 'l':

			break;
		case 'L':

			break;
		case 'c':
			
			break;
		case 'C':

			break;
		case 'R':

			break;
		case 'h':

			break;
		}
	}
	
	std::cout << "Smoketest" << std::endl;
	std::cout << argc << std::endl;
	std::cout << argv[0] << std::endl;
	std::cout << BUF_SIZE_1MIB << std::endl;
	return 0;
}
