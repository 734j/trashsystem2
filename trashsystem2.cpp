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
	
	out << USAGE << std::ends;
}

inline void usage_out_long(std::ostream &out) {
	
	out << LONG_USAGE << std::ends;
}

int main (int argc, char **argv) {
	
	g_argv = argv[0];
	if (argc == 1) {
		usage_out(std::cerr);
        return EXIT_FAILURE;
    }
	
	int R_mtx = 0;
	int C_mtx = 0;
	int c_mtx = 0;
	int L_mtx = 0;
	int l_mtx = 0;
	int y_mtx = 0;
	int n_mtx = 0;
	int f_mtx = 0;
	int h_mtx = 0;
	bool y_used = false;
	bool n_used = false;
	bool v_used = false;
	bool f_used = false;
	bool t_used = false;
	bool l_used = false;
	bool L_used = false;
	bool c_used = false;
	bool C_used = false;
	bool R_used = false;
	bool h_used = false;
	int opt = 0;
	while((opt = getopt(argc, argv, "ynvfatlLcCR:h")) != -1) {

		switch(opt) {
		case 'y':
			
			y_mtx = 1;
			y_used = true;
			
			break;
		case 'n':
			
			n_mtx = 1;
			n_used = true;
			
			break;
		case 'v':

			v_used = true;
			
			break;
		case 'f':

			f_mtx = 1;
			f_used = true;
			
			break;
		case 't':

			t_used = true;
			
			break;
		case 'l':

			l_mtx = 1;
			l_used = true;
			
			break;
		case 'L':

			L_mtx = 1;
			L_used = true;
			
			break;
		case 'c':

			c_mtx = 1;
			c_used = true;
			
			break;
		case 'C':
			
			C_mtx = 1;
			C_used = true;
			
			break;
		case 'R':
			
			R_mtx = 1;
			R_used = true;
			
			break;
		case 'h':

			h_mtx = 1;
			h_used = true;
			
			break;
		}
	}

	DEBUG_STREAM(<< "Used options [ynvftlLcCRh]: "
				 << y_used << n_used << v_used
				 << f_used << t_used << l_used
				 << L_used << c_used << C_used
				 << R_used << h_used << std::endl);
	if((R_mtx + C_mtx + c_mtx + L_mtx + l_mtx + h_mtx) > 1) {
		usage_out(std::cerr);
		return EXIT_FAILURE;
	}

	if((y_mtx + n_mtx + f_mtx + h_mtx) > 1) {
		usage_out(std::cerr);
		return EXIT_FAILURE;
	}

	if(optind == argc && (l_used || L_used || C_used || c_used || h_used || R_used) == false) {
		usage_out(std::cerr);
		return EXIT_FAILURE;
	}

	if(h_used == true) {
		usage_out_long(std::cout);
		return EXIT_SUCCESS;
	}

	if(y_used || n_used || v_used || f_used || t_used) {}
	std::cout << "Smoketest" << std::endl;
	std::cout << argc << std::endl;
	std::cout << argv[0] << std::endl;
	std::cout << BUF_SIZE_1MIB << std::endl;
	return 0;
}
