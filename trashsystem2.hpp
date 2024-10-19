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

class directory_entry {
public:
	directory_entry(std::filesystem::path dep, bool isd, bool isr, bool iso):
		de_path(dep), is_dir(isd), is_reg(isr), is_other(iso) {}
	const bool &ris_reg() const { return is_reg; }
	const bool &ris_dir() const { return is_dir; }
	const bool &ris_other() const { return is_other; }
	const std::filesystem::path &rget_path() const { return de_path; }
private:
	std::filesystem::path de_path;
    bool is_dir = false;
	bool is_reg = false;
	bool is_other = false;
};

class trashsys_log_info {
public:
	trashsys_log_info(int64_t log_id, size_t log_filesize, time_t log_trashtime,
					  std::filesystem::path log_filename, std::filesystem::path log_originalpath, bool is_dir):
		ts_log_id(log_id), ts_log_filesize(log_filesize), ts_log_trashtime(log_trashtime),
		ts_log_filename(log_filename), ts_log_originalpath(log_originalpath), ts_is_dir(is_dir) { }
	const int64_t &rget_logid() const { return ts_log_id; }
	const size_t &rget_logfsz() const { return ts_log_filesize; }
	const time_t &rget_logtt() const { return ts_log_trashtime; }
	const std::filesystem::path &rget_logfn() const { return ts_log_filename; }
	const std::filesystem::path &rget_logop() const { return ts_log_originalpath; }
	const bool &rget_isdir() const { return ts_is_dir; }
private:
	int64_t ts_log_id;
	size_t ts_log_filesize;
	time_t ts_log_trashtime;
	std::filesystem::path ts_log_filename;
	std::filesystem::path ts_log_originalpath;	
	bool ts_is_dir;
};

class initial_path_info { // Initial useful strings to create before we do anything. Super useful when programming.
public:
	initial_path_info();
	bool is_fail() const { return ipi_fail; }
	const std::filesystem::path &rget_uh() const { return ts_path_user_home; };
	const std::filesystem::path &rget_uh_ws() const { return ts_path_user_home_withslash; };
	const std::filesystem::path &rget_ts() const { return ts_path_trashsys; };
	const std::filesystem::path &rget_ts_ws() const { return ts_path_trashsys_withslash; };
	const std::filesystem::path &rget_log() const { return ts_path_log; };
	const std::filesystem::path &rget_log_ws() const { return ts_path_log_withslash; };
	const std::filesystem::path &rget_trd() const { return ts_path_trashed; };
	const std::filesystem::path &rget_trd_ws() const { return ts_path_trashed_withslash; };
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
