#define BUF_SIZE_2 2048
#define BUF_SIZE_4 4096
#define BUF_SIZE_1MIB 1048576
#define USAGE std::string(g_argv) + " [-vt] [-y][-n][-f][-l][-L][-c][-C][-h][-R id] [FILE(s)]\n"
#define LONG_USAGE std::string(g_argv) + " [options] filename(s)\n"	\
	               "\n"\
	               "OPTIONS:\n"\
	               "  -t                      /tmp mode. tsr will use /tmp instead of the user's $HOME.\n"\
	               "  -y                      Answer 'yes' when pressing return on all [Y / N] prompts.\n"\
	               "  -n                      Answer 'no' when pressing return on all [Y / N] prompts.\n"\
	               "  -f                      Force answer 'yes' on all [Y / N] prompts. Prompt will not show.\n"\
   	               "  -l                      List all trashed files.\n"\
	               "  -L                      List all trashed files with more details.\n"\
	               "  -c                      Clear all trashed files that are older than the configured time limit.\n"\
	               "  -C                      Clear all trashed files regardless of age. Will prompt with a [Y / N] prompt.\n"\
	               "  -h                      Display this help message.\n"\
	               "  -R id path(optional)    Restore a file by ID. Use -l or -L to find the ID associated with the file.\n"\
	               "  -v                      Verbose mode, not recommended unless you are a developer.\n"\
	               "  -i                      Check for inconsistencies in the logs and trashed files (maybe implement)\n"\
	               "\n"
#define MODE_NORMAL -1
#define MODE_YES 0
#define MODE_NO 1
#define MODE_FORCE 2
#define FUNCTION_FAILURE -1 // redesign
#define FUNCTION_SUCCESS 0 // redesign
#ifdef DEBUG_S // Stream for debugging, pass -DDEBUG_S and then compile to turn on
#define DEBUG_STREAM(x) std::cerr x
#else
#define DEBUG_STREAM(x)
#endif
