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

class filesize_unit { // not dealing with this now. Ill figure it out later.
public:
	filesize_unit() = default;
	filesize_unit(double number, bool bytes = false, bool kib = false, bool mib = false, bool gib = false):
		_number(number), _is_bytes(bytes), _is_kib(kib), _is_mib(mib), _is_gib(gib) {}
	double get_number() const { return _number; }
	bool is_bytes() const { return _is_bytes; }
	bool is_kib() const { return _is_kib; }
	bool is_mib() const { return _is_mib; }
	bool is_gib() const { return _is_gib; }
	void set_is_bytes(bool a) { _is_bytes = a; }
	void set_is_kib(bool a) { _is_kib = a; }
	void set_is_mib(bool a) { _is_mib = a; }
	void set_is_gib(bool a) { _is_gib = a; }
private:
	double _number = 0;
	bool _is_bytes = false;
	bool _is_kib = false;
	bool _is_mib = false;
	bool _is_gib = false;
};

class directory_entry {
public:
	directory_entry(std::filesystem::path dep, bool isd = false, bool isr = false, bool iso = false, TS_FUNCTION_RESULT isf = FUNCTION_SUCCESS):
		_de_path(dep), _is_dir(isd), _is_reg(isr), _is_other(iso), _function_result(isf) {}
	bool is_reg() const { return _is_reg; }
	bool is_dir() const { return _is_dir; }
	bool is_other() const { return _is_other; }
	TS_FUNCTION_RESULT get_function_result () const { return _function_result; }
	const std::filesystem::path &rget_path() const { return _de_path; }
	void set_fail() { _function_result = FUNCTION_FAILURE; }
private:
	std::filesystem::path _de_path;
    bool _is_dir = false;
	bool _is_reg = false;
	bool _is_other = false;
	TS_FUNCTION_RESULT _function_result = FUNCTION_SUCCESS;
};

class trashsys_log_info {
public:
	trashsys_log_info() = default;
	trashsys_log_info(int64_t log_id, size_t log_filesize, time_t log_trashtime,
					  std::filesystem::path log_filename, std::filesystem::path log_originalpath, bool is_dir):
		ts_log_id(log_id), ts_log_filesize(log_filesize), ts_log_trashtime(log_trashtime),
		ts_log_filename(log_filename), ts_log_originalpath(log_originalpath), ts_is_dir(is_dir) { }
	const int64_t &rget_logid() const { return ts_log_id; }
	const std::uintmax_t &rget_logfsz() const { return ts_log_filesize; }
	const time_t &rget_logtt() const { return ts_log_trashtime; }
	const std::filesystem::path &rget_logfn() const { return ts_log_filename; }
	const std::filesystem::path &rget_logop() const { return ts_log_originalpath; }
	const bool &rget_isdir() const { return ts_is_dir; }
private:
	int64_t ts_log_id;
	std::uintmax_t ts_log_filesize;
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
