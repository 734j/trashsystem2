#define BUF_SIZE_2 2048
#define BUF_SIZE_4 4096
#define BUF_SIZE_1MIB 1048576
#define USAGE std::string(g_argv) + " [-y][-n][-f][-l][-L][-c][-C][-h][-R id] [FILE(s)]\n"
#define LONG_USAGE std::string(g_argv) + " [options] filename(s)\n"	\
	               "\n"\
	               "OPTIONS:\n"\
	               "  -y                      Answer 'yes' when pressing return on all [Y / N] prompts.\n"\
	               "  -n                      Answer 'no' when pressing return on all [Y / N] prompts.\n"\
	               "  -f                      Force answer 'yes' on all [Y / N] prompts. Prompt will not show.\n"\
   	               "  -l                      List all trashed files.\n"\
	               "  -L                      List all trashed files with more details.\n"\
	               "  -c                      Clear all trashed files that are older than the configured time limit.\n"\
	               "  -C                      Clear all trashed files regardless of age. Will prompt with a [Y / N] prompt.\n"\
	               "  -h                      Display this help message.\n"\
	               "  -R id path(optional)    Restore a file by ID. Use -l or -L to find the ID associated with the file.\n"\
	               "  -i                      Check for inconsistencies in the logs and trashed files (maybe implement)\n"\
	               "\n"
#define MODE_NORMAL -1
#define MODE_YES 0
#define MODE_NO 1
#define MODE_FORCE 2
typedef int TS_FUNCTION_RESULT;
const TS_FUNCTION_RESULT FUNCTION_FAILURE = -1;
const TS_FUNCTION_RESULT FUNCTION_SUCCESS = 0;
#ifdef DEBUG_S // Stream for debugging, pass -DDEBUG_S and then compile to turn on
#define DEBUG_STREAM(x) std::cerr x
#else
#define DEBUG_STREAM(x)
#endif

#ifndef TRASHSYSTEM2_HPP
#define TRASHSYSTEM2_HPP
#include <filesystem>
#include <string>
#include <vector>
#include <cstdlib>
class initial_path_info { // Initial useful strings to create before we do anything. Super useful when programming.
public:
	initial_path_info();
	bool is_fail() { return ipi_fail; }
	std::filesystem::path &rget_uh() { return ts_path_user_home; };
	std::filesystem::path &rget_uh_ws() { return ts_path_user_home_withslash; };
	std::filesystem::path &rget_ts() { return ts_path_trashsys; };
	std::filesystem::path &rget_ts_ws() { return ts_path_trashsys_withslash; };
	std::filesystem::path &rget_log() { return ts_path_log; };
	std::filesystem::path &rget_log_ws() { return ts_path_log_withslash; };
	std::filesystem::path &rget_trd() { return ts_path_trashed; };
	std::filesystem::path &rget_trd_ws() { return ts_path_trashed_withslash; };
	std::filesystem::path get_uh() { return ts_path_user_home; };
	std::filesystem::path get_uh_ws() { return ts_path_user_home_withslash; };
	std::filesystem::path get_ts() { return ts_path_trashsys; };
	std::filesystem::path get_ts_ws() { return ts_path_trashsys_withslash; };
	std::filesystem::path get_log() { return ts_path_log; };
	std::filesystem::path get_log_ws() { return ts_path_log_withslash; };
	std::filesystem::path get_trd() { return ts_path_trashed; };
	std::filesystem::path get_trd_ws() { return ts_path_trashed_withslash; };
private:
	std::filesystem::path ts_path_user_home; //
	std::filesystem::path ts_path_trashsys; //
	std::filesystem::path ts_path_log; //
	std::filesystem::path ts_path_trashed;
	std::filesystem::path ts_path_user_home_withslash; //
	std::filesystem::path ts_path_trashsys_withslash; //
	std::filesystem::path ts_path_log_withslash; //
	std::filesystem::path ts_path_trashed_withslash;
	bool ipi_fail = false;
};

initial_path_info::initial_path_info() {

	char *home = std::getenv("HOME");
	if(home == nullptr) {
		ipi_fail = true;
	}

	std::string homestr = home;
	ts_path_user_home = homestr;
	ts_path_user_home_withslash = homestr+"/";
	home = nullptr;
	ts_path_trashsys = homestr+"/.trashsys";
	ts_path_trashsys_withslash = homestr+"/.trashsys/";
	ts_path_log = homestr+"/.trashsys/log";
	ts_path_log_withslash = homestr+"/.trashsys/log/";
	ts_path_trashed = homestr+"/.trashsys/trashed";
	ts_path_trashed_withslash = homestr+"/.trashsys/trashed/";
}

#endif
