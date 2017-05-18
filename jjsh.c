#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <python2.7/Python.h>
#include "jjsh.h"

// List of builtin commands, followed by their corresponding functions
char *builtin_str[] = {
  "cd",
  "exit",
  "get",
  "help",
  "ipconfig",
  "pwd"
};

int (*builtin_func[]) (char **) = {
  &jjsh_cd,
  &jjsh_exit,
  &jjsh_get,
  &jjsh_help,
  &jjsh_ipconfig,
  &jjsh_pwd,
};

int jjsh_num_builtins() {
  return sizeof(builtin_str) / sizeof(char *);
}

int jjsh_cd(char **args)
{
  if (args[1] == NULL) {
    chdir(getenv("HOME"));
  }
  else if(args[1] == "..") {
    chdir("..");
  }
  else if(args[1] == ".") {
    chdir(".");
  } else {
    if (chdir(args[1]) != 0) {
      perror("jjsh");
    }
  }
  return 1;
}

int jjsh_exit(char **args)
{
  return 0;
}

int jjsh_get(char **args)
{
  char command[128];
  if (args[1] == NULL) {
    snprintf(command, sizeof(command), "/home/ubuntu/workspace/jjsh/pyscripts/get.py");
  } else {
    snprintf(command, sizeof(command), "/home/ubuntu/workspace/jjsh/pyscripts/get.py -strings %s", args[1]);
  }
  system(command);
  return 1;
}

int jjsh_help(char **args)
{
  system("/home/ubuntu/workspace/jjsh/pyscripts/help.py");
  return 1;
}

int jjsh_ipconfig(char **args)
{
  system("/home/ubuntu/workspace/jjsh/pyscripts/ipconfig.py");
  return 1;
}

int jjsh_pwd(char **args)
{

  system("/home/ubuntu/workspace/jjsh/pyscripts/pwd.py");
  return 1;
}

char *jjsh_read_line(void)
{
  int bufsize = JJSH_RL_BUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "jjsh: allocation error\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    // Read a character
    c = getchar();

    //If we hit EOF, replace it with a null character and return
    if (c == EOF || c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    // If we have exceeded the buffer, reallocate.
    if (position >= bufsize) {
      bufsize += JJSH_RL_BUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "jjsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **jjsh_split_line(char *line)
{
  int bufsize = JJSH_TOK_BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token;

  if (!tokens) {
    fprintf(stderr, "jjsh: allocaition error\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, JJSH_TOK_DELIM);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += JJSH_TOK_BUFSIZE;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
        fprintf(stderr, "jjsh: allocation error\n");
        exit(EXIT_FAILURE);
      }
    }
    token = strtok(NULL, JJSH_TOK_DELIM);
  }
  tokens[position] = NULL;
  return tokens;
}

int jjsh_launch(char **args)
{
  pid_t pid, wpid;
  int status;

  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      perror("jjsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    // Error forking
    perror("jjsh");
  } else {
    // Parent process
    do {
      wpid = waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

int jjsh_execute(char **args)
{
  int i;

  if (args[0] == NULL) {
    // An empty command was entered
    return 1;
  }

  for (i = 0; i < jjsh_num_builtins(); i++) {
    if (strcmp(args[0], builtin_str[i]) == 0) {
      return (*builtin_func[i])(args);
    }
  }
  return jjsh_launch(args);
}

void jjsh_loop(void)
{
  char *line;
  char **args;
  int status;

  do {

    if (getuid() == 0)
      system("/home/ubuntu/workspace/jjsh/pyscripts/rprompt.py");
    else
      system("/home/ubuntu/workspace/jjsh/pyscripts/uprompt.py");

    line = jjsh_read_line();
    args = jjsh_split_line(line);
    status = jjsh_execute(args);

    free(line);
    free(args);
  } while (status);
}

int main(int argc, char **argv)
{
  // Load config files, if any
  // Run command loop
  jjsh_loop();

  // Perform any shutdown/cleanup
  return EXIT_SUCCESS;
}
