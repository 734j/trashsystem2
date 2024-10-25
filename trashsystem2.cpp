#include <iostream>
#include "trashsystem2.hpp"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>

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

std::vector<directory_entry> get_files_in_directory(const std::filesystem::path path) {

	std::vector<directory_entry> v_directory_entry;
	for(auto &dir_entry : std::filesystem::directory_iterator{path}) {				
		bool isother = false;
		if(!dir_entry.is_directory() && !dir_entry.is_regular_file()) {
			DEBUG_STREAM( << "determine_new_id: " << dir_entry.path() << " is other." << std::endl);
			isother = true;
		}
		
		directory_entry current(dir_entry.path(), dir_entry.is_directory(), dir_entry.is_regular_file(), isother);
		v_directory_entry.push_back(current);
	}
	
	return v_directory_entry;
}

int64_t determine_highest_id(const initial_path_info &ipi) {

	int64_t highest_id = 0;
	auto dirs = get_files_in_directory(ipi.rget_log());	
	for(auto &a : dirs) {
		std::string path_str = a.rget_path().filename();
		const char *path_cstr = path_str.c_str();
		char *endptr = nullptr;
		auto strtoll_result = std::strtoll(path_cstr, &endptr, 10); // maybe implement fail case for strtoll
		DEBUG_STREAM( << "strtoll_result: " << strtoll_result << std::endl);
		if(path_cstr == endptr) {
			DEBUG_STREAM( << "determine_highest_id: path_cstr == endptr" << std::endl);
			continue;
		}

		if(*endptr != ':') {
			DEBUG_STREAM( << "':' not found for file: " << path_cstr << std::endl);
			continue;
		}

		if(strtoll_result > highest_id) {
			highest_id = strtoll_result;
			DEBUG_STREAM( << "ID: " << strtoll_result << ", is higher than highest_id." << std::endl);
		}
	}
	
	return highest_id;
}

std::uintmax_t get_directory_size(const std::filesystem::path &dirpath) { // Does not check for file existence, check before using

	std::vector<directory_entry> de_files_only;
	std::vector<directory_entry> de_dirs_only;
	for(auto &d : std::filesystem::directory_iterator(dirpath)) {
		if(d.is_regular_file()) {
			de_files_only.push_back(directory_entry(d.path(), false, true));
		} else if(d.is_directory()) {
			de_dirs_only.push_back(directory_entry(d.path(), true));
		} else {
			continue;
		}
	}

	std::uintmax_t final_size = 0;
	for(auto &dfo : de_files_only) {
		final_size += std::filesystem::file_size(dfo.rget_path());
	}

	for(auto &ddo : de_dirs_only) {
		final_size += get_directory_size(ddo.rget_path());
	}
	
	return final_size;
}

TS_FUNCTION_RESULT get_file_info(const std::filesystem::path &path,
								 trashsys_log_info &tli,
								 const initial_path_info &ipi) {

	if(!std::filesystem::exists(path)) {
		DEBUG_STREAM( << path << " does not exist" << std::endl);
		std::cerr << g_argv << ": '"<< std::string(path) << "' No such file or directory." << std::endl;
		return FUNCTION_FAILURE;
	}
		
	if(!std::filesystem::is_regular_file(path) && !std::filesystem::is_directory(path)) {
		DEBUG_STREAM( << path << " is not a directory or regular file." << std::endl);
		std::cerr << g_argv << ": '"<< std::string(path) << "' is not a directory or a regular file." << std::endl;
		return FUNCTION_FAILURE;
	}
	
	auto id = determine_highest_id(ipi);	
	auto canon_path = std::filesystem::canonical(path);
	//std::cout << canon_path << std::endl;
	decltype(get_directory_size(path)) filesize = 0;
	decltype(std::filesystem::is_directory(path)) isdir = false;
	if(std::filesystem::is_regular_file(path)) {
		filesize = std::filesystem::file_size(path);
	} else if (std::filesystem::is_directory(path)) {
		isdir = true;
		filesize = get_directory_size(path);
	}
	
	auto trashtime = std::time(nullptr); // maybe check if time fails?
	auto file_name = path.filename();
	trashsys_log_info _tli(id+1,filesize,trashtime,file_name,canon_path,isdir);
	tli = _tli;
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT write_log_entry(const initial_path_info &ipi, const trashsys_log_info &tli) { // function that writes logs

	std::string filename = tli.rget_logfn();
	std::string id_and_filename = std::to_string(tli.rget_logid())+":"+filename;
	std::string id_and_filename_log = id_and_filename+".log";
	std::ofstream log_out(std::string(ipi.rget_log_ws())+id_and_filename_log);
	if(!log_out) {
		return FUNCTION_FAILURE;
	}
	
	log_out << tli.rget_logid() << " "
			<< std::string(tli.rget_logfn()) << " "
			<< id_and_filename << " "
			<< tli.rget_logfsz() << " "
			<< tli.rget_logtt() << " "
			<< std::string(tli.rget_logop()) << " "
			<< tli.rget_isdir() << " ";
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT get_file_info_from_log(const initial_path_info &ipi,
										  std::vector<trashsys_log_info> &vtli) {

	/*
	  LOG FILE FORMAT:
	  1. ID (example 92)
	  2. FILENAME (example this_is_a_file.txt)
	  3. ID + FILENAME (example 92:this_is_a_file.txt) (not used in trashsys_log_info class, read but discarded)
	  4. FILESIZE (in bytes)
	  5. Time of trashing (unix epoch time. Example: 1729671387)
	  6. ORIGINAL PATH (includes filename: /home/johndoe/this/is/the/full/path/to/where/the/file/was/deleted/this_is_a_file.txt)
	  7. IS DIR? (simple boolean true or false)
	 */
	auto files_v = get_files_in_directory(ipi.rget_log());
	for(auto &a : files_v) {
		std::ifstream file_ifs (a.rget_path());
		if(!file_ifs) {
			DEBUG_STREAM( << "get_file_info_from_log: cannot open file: '" << a.rget_path() << "'\n");
			continue;
		}

		int64_t id;
		std::uintmax_t filesize;
		time_t trashtime;
		std::string log_filename;
		std::string log_id_and_filename;
		std::string log_originalpath;
		bool is_dir;
		std::string line;
		while(std::getline(file_ifs, line)) {
			std::istringstream to_tli(line);
			if(to_tli >> id >> log_filename >> log_id_and_filename/* <--- Not used!!! */ >>
			   filesize >> trashtime >> log_originalpath >> is_dir) {
				trashsys_log_info tli(id, filesize, trashtime, log_filename, log_originalpath, is_dir);
				vtli.push_back(tli);
				DEBUG_STREAM( << "get_file_info_from_log: push_back successful: '" << std::string(a.rget_path()) << "' date pushed back to vtli.\n");
			} else {
				DEBUG_STREAM( << "get_file_info_from_log: logfile: '" << a.rget_path()
							  << "' is incomplete. push_back not successful, continued.\n");				
				continue;
			}
		}		
	}
	
	return FUNCTION_SUCCESS;
}

filesize_unit to_filesize_unit(std::uintmax_t bytes) {

	double d_bytes = bytes;
	int count = 0;
	std::vector<int> unit = {1, 2, 3, 4};
	while(d_bytes >= 1024) {
		d_bytes = d_bytes / 1024;
		count++;
	}

	filesize_unit size(d_bytes);
	if(unit[count] == 1) { size.set_is_bytes(true); }
	
	if(unit[count] == 2) { size.set_is_kib(true); }
	
	if(unit[count] == 3) { size.set_is_mib(true); }
	
	if(unit[count] == 4) { size.set_is_gib(true); }
	
	return size;
}

TS_FUNCTION_RESULT readable_time(time_t epochtime, std::string &time) {

	struct tm *tmp = NULL;
	char *pretty_time = (char*)malloc(sizeof(char) * 512);
	tmp = localtime(&epochtime);
	if(strftime(pretty_time, 512, "%F", tmp) == 0) {
		free(pretty_time);
		return FUNCTION_FAILURE;
	}

	time = pretty_time;
	free(pretty_time);
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT list_trashed(const std::vector<trashsys_log_info> &vtli) {

	for(auto &a : vtli) {
		filesize_unit fu = to_filesize_unit(a.rget_logfsz());
		std::string unit;
		std::string pretty_time;
		if(readable_time(a.rget_logtt(), pretty_time) == FUNCTION_FAILURE) {
			return FUNCTION_FAILURE;
		}

		std::string file_or_directory = "File";
		if(a.rget_isdir()) {
			file_or_directory = "Directory";
		}
		
		if(fu.is_bytes()) { unit = "B"; }
		if(fu.is_kib()) { unit = "KiB"; }
		if(fu.is_mib()) { unit = "MiB"; }
		if(fu.is_gib()) { unit = "GiB"; }
		std::cout << "ID: " << a.rget_logid() << "   "
				  << std::string(a.rget_logfn()) << "   "
				  << fu.get_number() << " " << unit << "   "
				  << "Trashed at: " << pretty_time << "   "
				  << file_or_directory << std::endl;
	}
	
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT long_list_trashed(const std::vector<trashsys_log_info> &vtli) {

	for(auto &a : vtli) {
		filesize_unit fu = to_filesize_unit(a.rget_logfsz());
		std::string unit;
		std::string pretty_time;
		if(readable_time(a.rget_logtt(), pretty_time) == FUNCTION_FAILURE) {
			return FUNCTION_FAILURE;
		}

		std::string file_or_directory = "File";
		if(a.rget_isdir()) {
			file_or_directory = "Directory";
		}
		
		if(fu.is_bytes()) { unit = "B"; }
		if(fu.is_kib()) { unit = "KiB"; }
		if(fu.is_mib()) { unit = "MiB"; }
		if(fu.is_gib()) { unit = "GiB"; }
		std::cout << "ID: " << a.rget_logid() << "   "
				  << std::string(a.rget_logfn()) << "   "
				  << fu.get_number() << " " << unit << "   "
				  << a.rget_logfsz() << " B" << "   "
				  << "Trashed at: " << pretty_time << "   "
				  << a.rget_logtt() << "   "
				  << file_or_directory << std::endl;
	}
	
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT put_in_trash(const initial_path_info &ipi, const trashsys_log_info &tli) {

	std::string id_and_filename = std::to_string(tli.rget_logid())+":"+std::string(tli.rget_logfn());
	std::string prepend_path = std::string(ipi.rget_trd_ws())+id_and_filename;
	std::filesystem::rename(tli.rget_logop(), prepend_path);
	return FUNCTION_SUCCESS;
}

TS_FUNCTION_RESULT r_argument_validation(std::string argument) {

	std::stringstream ss(argument);	
	int64_t number = 0;
	auto iter = argument.cbegin();
	if(iter != argument.cend()) {
		if(*iter == '0') {		   
			return FUNCTION_FAILURE;
		}
	}
	
	ss >> number;	
	if(ss.eof()) {		
		return number;
	}

	return FUNCTION_FAILURE;
}

directory_entry find_by_id(const initial_path_info &ipi, TS_FUNCTION_RESULT id) {

	directory_entry foundfile("");
	auto vdirentry = get_files_in_directory(ipi.rget_log());
	for(auto &a : vdirentry) {
		if(a.is_dir() || a.is_other()) {
			DEBUG_STREAM( << "restore_file: Non-regular file found in log. Skipping." << std::endl);
			continue;
		}

		int64_t number = 0;
		std::ifstream ifs(a.rget_path());
		if(!ifs) {
			DEBUG_STREAM( << "restore_file: File cannot be opened." << std::endl);
			continue;
		}
		
		ifs >> number;
		if(ifs.fail()) {
			DEBUG_STREAM( << "restore_file: Incorrect file format found in a log file. Skipping." << std::endl);
			continue;
		}

		std::cout << number << std::endl;
	    if(id == number) {
			foundfile = a;
			return foundfile;
		}
	}

	DEBUG_STREAM( << "restore_file: ID not found" << std::endl);
	foundfile.set_fail();
	return foundfile;
}

TS_FUNCTION_RESULT restore_file(const directory_entry &de, const initial_path_info &ipi) {

	std::ifstream ifs(de.rget_path());
	int id; // discard
	std::string filename; // discard
	std::string idfilename; // discard
	std::uintmax_t filesize; // discard
	time_t trashedtime; // discard
	std::filesystem::path restorepath;
	bool isdir; // discard
	while(ifs >> id >> filename >> idfilename >> filesize >> trashedtime >> restorepath >> isdir) {}
	std::string from = std::string(ipi.rget_trd_ws())+idfilename;
	std::cout << from << std::endl;
	if(ifs.eof()) {
		DEBUG_STREAM( << de.rget_path() << std::endl);
		DEBUG_STREAM( << restorepath << std::endl);
		std::filesystem::rename(from, restorepath);
		std::filesystem::remove(de.rget_path());
		std::cout << "File was restored to: " << restorepath << std::endl;
		return FUNCTION_SUCCESS;
	}

	return FUNCTION_FAILURE;
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
	std::string r_arg;
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
			r_arg = optarg;
			
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
		auto argnumber = r_argument_validation(r_arg);
		if(argnumber == FUNCTION_FAILURE) {
			std::cerr << g_argv << ": Error: Invalid argument. Please provide a valid ID." << std::endl;
			return EXIT_FAILURE;
		}
		
		auto de = find_by_id(ipi, argnumber);
		if(de.get_function_result() == FUNCTION_FAILURE) {
			std::cerr << g_argv << ": Error: File with ID '" << argnumber << "' was not found." << std::endl;
			return EXIT_FAILURE;
		}

		if(restore_file(de, ipi) == FUNCTION_FAILURE) {
			std::cerr << g_argv << ": Error: " << std::endl;
			return EXIT_FAILURE;
		}
		
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
		std::vector<trashsys_log_info> vtli;
		if(get_file_info_from_log(ipi, vtli) == FUNCTION_FAILURE) { // actually impossible case but covering it now either way
			return EXIT_FAILURE;
		}
		
		if(l_used) {
			DEBUG_STREAM( << "-l" << std::endl);
			if(list_trashed(vtli) == FUNCTION_FAILURE) {
				return EXIT_FAILURE;
			}
		}

		if(L_used) {
			DEBUG_STREAM( << "-L" << std::endl);
			if(long_list_trashed(vtli) == FUNCTION_FAILURE) {
				return EXIT_FAILURE;
			}
		}
		
		return EXIT_SUCCESS;
	}

	int index = 0;
	for (index = optind ; index < argc ; index++) {
		std::filesystem::path precheck = argv[index];		
		std::string check = argv[index];
		if(std::filesystem::is_directory(precheck)) {
			if(check.cbegin() != check.cend()) {
				auto checkrbeg = check.rbegin();
				if(*checkrbeg == '/') {
					check.pop_back();
				}
			}
		}
		
		std::filesystem::path file_to_trash = check;
		trashsys_log_info tli;
		if(get_file_info(file_to_trash, tli, ipi) == FUNCTION_FAILURE) {
			continue;
		}

		if(write_log_entry(ipi, tli)) {
			return EXIT_FAILURE;
		}

		put_in_trash(ipi, tli);
	}
	
	return 0;
}
