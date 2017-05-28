#ifndef JJSH_HEADER
#define JJSH_HEADER

#include <errno.h>

#define JJSH_RL_BUFSIZE 1024
#define JJSH_HIST_BUFSIZE 1024
#define JJSH_EXTRA_BUFSIZE 1024
#define JJSH_TOK_BUFSIZE 64
#define JJSH_TOK_DELIM " \t\r\n\a"
#define JJSH_MAX_CMD_LEN 128
#define JJSH_HISTORY_COUNT 50

void jjsh_loop(void);
char *jjsh_read_line(void);
char **jjsh_split_line(char *line);
int jjsh_execute(char **args);

// jjsh built-in functions
int jjsh_cd(char **args);
int jjsh_help(char **args);
int jjsh_exit(char **args);
int jjsh_pwd(char **args);
int jjsh_ipconfig(char **args);
int jjsh_get(char **args);

#endif
