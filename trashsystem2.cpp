#include <iostream>
#include "trashsystem2.hpp"
#include <unistd.h>
#include <cstdio>

/*
 *
 * Trashing System (Version 2)
 *
 * A hopefully improved take on the previous trash
 * system that i made in C. By reimplementing this
 * in C++ i hope to avoid the bugs and code smell
 * that eventually cropped up in the last project.
 *
 * Some of the structure is very much based on
 * Version 1 of the Trashing System. A lot of the main
 * function and usage of getopt will be very similar
 * and based off the code from Version 1.
 *
 * While i am optimistic at this point my pessimism
 * will probably prove me wrong because i will most
 * likely encounter other problems that will give
 * my code that nice funky smell.
 *
 */

extern const TS_FUNCTION_RESULT FUNCTION_FAILURE;
extern const TS_FUNCTION_RESULT FUNCTION_SUCCESS;
char *g_argv = nullptr;

int handle_ynf (const bool y_used, const bool n_used, const bool f_used) {

	int choice_mode_ynf = MODE_NORMAL;
	if (n_used == true) { choice_mode_ynf = MODE_NO; }
   	if (y_used == true) { choice_mode_ynf = MODE_YES; }
	if (f_used == true) { choice_mode_ynf = MODE_FORCE; }
	
	return choice_mode_ynf;
}


int choice (const int mode) {

    char choice;
    char modechoice;
    do {
		if (mode == MODE_NORMAL) { std::cout << "[Y / N] ? " << std::ends; }
		if (mode == MODE_YES) { std::cout << "[Y / n] ? " << std::ends; }
		if (mode == MODE_NO) { std::cout << "[y / N] ? " << std::ends;; }
		if (mode == MODE_FORCE) { return 0; }
	
		choice = std::getchar();
		if (choice == '\n' && mode == MODE_YES) { modechoice = 'Y'; choice = modechoice; goto modeskip;}
		if (choice == '\n' && mode == MODE_NO) { modechoice = 'N'; choice = modechoice; goto modeskip;}
		if (choice == '\n' && mode == MODE_NORMAL) { continue; }

		while ('\n' != std::getchar());

    } while ( (choice != 'Y') && (choice != 'y') && (choice != 'N') && (choice != 'n') );

    modeskip:
    if ((choice == 'Y') || (choice == 'y')) {
        return 0;
    }

    if ((choice == 'N') || (choice == 'n')) {
        return 1;
    }

    return FUNCTION_FAILURE; // Should never happen
}

TS_FUNCTION_RESULT compare_unixtime (const time_t deleted_time, int const difference_in_days) {

	time_t diff_converted;
	time_t current_time;
	time_t final;
	diff_converted = (time_t)difference_in_days * 86400;
	current_time = time(NULL);
	if(current_time == -1) {
		return FUNCTION_FAILURE;
	}

	final = current_time - deleted_time; 
	if(final < diff_converted) {
		DEBUG_STREAM(<< "final is not older than diff_converted\n" << std::endl);
		return FUNCTION_FAILURE;
	}
	
	DEBUG_STREAM(<< "final is older than diff_converted\n" << std::endl);
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT create_ts_dirs(const initial_path_info &ipi) {

	if(std::filesystem::exists(ipi.rget_ts())) {
		DEBUG_STREAM( << ipi.rget_ts() << " exists." << std::endl);
	} else {
		if(std::filesystem::create_directory(ipi.rget_ts()) == false) {
			return FUNCTION_FAILURE;
		} else {
			DEBUG_STREAM( << ipi.rget_ts() << " was created." << std::endl);
		}
	}

	if(std::filesystem::exists(ipi.rget_log())) {
		DEBUG_STREAM( << ipi.rget_log() << " exists." << std::endl);
	} else {
		if(std::filesystem::create_directory(ipi.rget_log()) == false) {
			return FUNCTION_FAILURE;
		} else {
			DEBUG_STREAM( << ipi.rget_log() << " was created." << std::endl);
		}
	}

	if(std::filesystem::exists(ipi.rget_trd())) {
		DEBUG_STREAM( << ipi.rget_trd() << " exists." << std::endl);
	} else {
		if(std::filesystem::create_directory(ipi.rget_trd()) == false) {
			return FUNCTION_FAILURE;
		} else {
			DEBUG_STREAM( << ipi.rget_trd() << " was created." << std::endl);
		}
	}
	
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT get_file_info(const std::filesystem::path &path) { // function that gets all information about a file

	if(path.empty()) {}
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT write_log_entry(const initial_path_info &ipi) { // function that writes logs

	if(ipi.is_fail()) {}
	return FUNCTION_SUCCESS;
}

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
	bool f_used = false;
	bool l_used = false;
	bool L_used = false;
	bool c_used = false;
	bool C_used = false;
	bool R_used = false;
	bool h_used = false;
	int opt = 0;
	while((opt = getopt(argc, argv, "ynftlLcCR:h")) != -1) {

		switch(opt) {
		case 'y':
			
			y_mtx = 1;
			y_used = true;
			
			break;
		case 'n':
			
			n_mtx = 1;
			n_used = true;
			
			break;
		case 'f':

			f_mtx = 1;
			f_used = true;
			
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

	DEBUG_STREAM(<< "Used options [ynflLcCRh]: "
				 << y_used << n_used << f_used
				 << l_used << L_used << c_used
				 << C_used << R_used << h_used << std::endl);
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
		DEBUG_STREAM( << "-h" << std::endl);
		return EXIT_SUCCESS;
	}
	
	int choice_mode = handle_ynf(y_used, n_used, f_used);
	initial_path_info ipi;
	if(ipi.is_fail()) {
		DEBUG_STREAM( << "ipi.isfail():" << ipi.is_fail() << std::endl);
		return EXIT_FAILURE;
	}

	DEBUG_STREAM( << "ipi: \n"
				  << ipi.get_uh() << "\n"
				  << ipi.get_uh_ws() << "\n"
				  << ipi.get_ts() << "\n"
				  << ipi.get_ts_ws() << "\n"
				  << ipi.get_trd() << "\n"
				  << ipi.get_trd_ws() << "\n"
				  << ipi.get_log() << "\n"
				  << ipi.get_log_ws() << std::endl);
	
	if(create_ts_dirs(ipi) == FUNCTION_FAILURE) {
		std::cerr << g_argv << ": Error: trashsys directories could not be created." << std::endl;
		DEBUG_STREAM( << "create_ts_dirs: FUNCTION_FAILURE" << std::endl);
		return EXIT_FAILURE;
	}

	if(R_used == true) {
		DEBUG_STREAM( << "-R" << std::endl);
		return EXIT_SUCCESS;
	}

	if(c_used == true) {
		DEBUG_STREAM( << "-c" << std::endl);
		return EXIT_SUCCESS;
	}

	if(C_used == true) {
		DEBUG_STREAM( << "-C" << std::endl);
		choice(choice_mode);
		return EXIT_SUCCESS;
	}

	if(l_used == true || L_used == true) {
		if(l_used) {
			DEBUG_STREAM( << "-l" << std::endl);
		}

		if(L_used) {
			DEBUG_STREAM( << "-L" << std::endl);
		}
		
		return EXIT_SUCCESS;
	}

	int index = 0;
	for (index = optind ; index < argc ; index++) { // Actual loop that trashes files etc
		std::filesystem::path file_to_trash = argv[index];

		std::cout << file_to_trash << std::endl;
	}
	
	return 0;
}
