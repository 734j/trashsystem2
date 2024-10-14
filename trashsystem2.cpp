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
	
	int R_mut = 0;
	int C_mut = 0;
	int c_mut = 0;
	int L_mut = 0;
	int l_mut = 0;
	int y_mut = 0;
	int n_mut = 0;
	int f_mut = 0;
	int h_mut = 0;
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
    int opt;
	unsigned long long optarg_converted;
	bool R_failed = false;
	while ((opt = getopt(argc, argv, "ynvfatlLcCR:h")) != -1) {
        switch (opt) {
        case 'y':

			y_mut = 1;
			y_used = true; // YES on enter

        break;
        case 'n':

			n_mut = 1;
            n_used = true; // NO on enter

        break;
        case 'v':
			
            v_used = true; // Verbose debug mode

        break;
        case 'f':

			f_mut = 1;
            f_used = true; // choice will not ask, it will just say yes by default thus basically "forcing" it

        break;
	    case 't':

			t_used = true;
			
		break;
		case 'l':

			l_mut = 1;
			l_used = true;
			
		break;
		case 'L':

			L_mut = 1;
			L_used = true;
			
		break;
		case 'c':

			c_mut = 1;
			c_used = true;
			
		break;
		case 'C':

			C_mut = 1;
			C_used = true;
			
		break;
		case 'R': {

			R_mut = 1;
			R_used = true;
			
		}
		break;
		case 'h':

			h_mut = 1;
			h_used = true;

		break;
        }
    }

	std::cout << "Smoketest" << std::endl;
	std::cout << argc << std::endl;
	std::cout << argv[0] << std::endl;
	std::cout << BUF_SIZE_1MIB << std::endl;
	return 0;
}
