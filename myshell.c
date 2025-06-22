#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#define MY_MAX_INPUT 1024
#define MAX_ARGS 100

extern char **environ;

void print_welcome() {
    printf("*********************************************\n");
    printf("*                                           *\n");
    printf("*           Welcome to myshell!             *\n");
    printf("*                                           *\n");
    printf("*   Project by: Nedaa Khater, Hanin Ostaz,  *\n");
    printf("*             Raghad Altalaa                *\n");
    printf("*                                           *\n");
    printf("*   Type 'help' to see available commands.  *\n");
    printf("*                                           *\n");
    printf("*********************************************\n\n");
}

void parse_input(char* input, char** args, int* arg_count) {
    *arg_count = 0;
    char* token = strtok(input, " \t\n");
    while(token != NULL && *arg_count < MAX_ARGS - 1) {
        args[(*arg_count)++] = token;
        token = strtok(NULL, " \t\n");
    }
    args[*arg_count] = NULL;
}

void cmd_cd(char** args, int arg_count) {
    if (arg_count == 1) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("%s\n", cwd);
        }
    } else {
        if (chdir(args[1]) != 0) {
            perror("cd");
        } else {
            setenv("PWD", args[1], 1);
        }
    }
}

void cmd_clr() {
    printf("\033[H\033[J");
}

void cmd_dir(char** args, int arg_count) {
    char* dir = ".";
    if (arg_count > 1) dir = args[1];

    DIR* d = opendir(dir);
    if (d == NULL) {
        perror("dir");
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(d)) != NULL) {
        printf("%s\n", entry->d_name);
    }
    closedir(d);
}

void cmd_environ() {
    char** env = environ;
    while (*env) {
        printf("%s\n", *env);
        env++;
    }
}

void cmd_echo(char** args, int arg_count) {
    for (int i=1; i<arg_count; i++) {
        printf("%s", args[i]);
        if (i < arg_count -1) printf(" ");
    }
    printf("\n");
}

void cmd_help() {
    if (fork() == 0) {
        execlp("more", "more", "readme", NULL);
        perror("help");
        exit(1);
    } else {
        wait(NULL);
    }
}

void cmd_pause() {
    printf("Shell paused. Press Enter to continue...\n");
    while (getchar() != '\n');
}

void execute_external(char** args, int background, char* input_file, char* output_file, int append) {
    pid_t pid = fork();
    if (pid == 0) {
        if (input_file) {
            int fd = open(input_file, O_RDONLY);
            if (fd < 0) {
                perror("input redirection");
                exit(1);
            }
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        if (output_file) {
            int fd;
            if (append)
                fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
            else
                fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

            if (fd < 0) {
                perror("output redirection");
                exit(1);
            }
            dup2(fd, STDOUT_FILENO);
            close(fd);
        }

        execvp(args[0], args);
        perror("exec");
        exit(1);
    } else if (pid > 0) {
        if (!background) {
            waitpid(pid, NULL, 0);
        } else {
            printf("Process running in background with PID %d\n", pid);
        }
    } else {
        perror("fork");
    }
}

int main(int argc, char* argv[]) {
    print_welcome(); // رسالة الترحيب

    char input[MY_MAX_INPUT];
    char* args[MAX_ARGS];
    int arg_count;
    FILE* input_stream = stdin;

    if (argc == 2) {
        input_stream = fopen(argv[1], "r");
        if (!input_stream) {
            perror("Batch file");
            return 1;
        }
    }

    char shell_path[1024];
    if (getcwd(shell_path, sizeof(shell_path)) != NULL) {
        strcat(shell_path, "/myshell");
        setenv("shell", shell_path, 1);
    }

    while (1) {
        if (input_stream == stdin) {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL) {
                printf("%s myshell> ", cwd);
            } else {
                printf("myshell> ");
            }
            fflush(stdout);
        }

        if (fgets(input, sizeof(input), input_stream) == NULL) break;
        if (strcmp(input, "\n") == 0) continue;
        input[strcspn(input, "\n")] = 0;

        parse_input(input, args, &arg_count);
        if (arg_count == 0) continue;

        char* input_file = NULL;
        char* output_file = NULL;
        int append = 0;
        int background = 0;

        for (int i=0; i < arg_count; i++) {
            if (strcmp(args[i], "<") == 0 && i+1 < arg_count) {
                input_file = args[i+1];
                for (int j=i; j+2 <= arg_count; j++) args[j] = args[j+2];
                arg_count -= 2;
                i--;
            } else if (strcmp(args[i], ">") == 0 && i+1 < arg_count) {
                output_file = args[i+1];
                append = 0;
                for (int j=i; j+2 <= arg_count; j++) args[j] = args[j+2];
                arg_count -= 2;
                i--;
            } else if (strcmp(args[i], ">>") == 0 && i+1 < arg_count) {
                output_file = args[i+1];
                append = 1;
                for (int j=i; j+2 <= arg_count; j++) args[j] = args[j+2];
                arg_count -= 2;
                i--;
            } else if (strcmp(args[i], "&") == 0) {
                background = 1;
                for (int j=i; j+1 <= arg_count; j++) args[j] = args[j+1];
                arg_count--;
                i--;
            }
        }
        args[arg_count] = NULL;

        if (strcmp(args[0], "quit") == 0) {
            break;
        } else if (strcmp(args[0], "cd") == 0) {
            cmd_cd(args, arg_count);
        } else if (strcmp(args[0], "clr") == 0) {
            cmd_clr();
        } else if (strcmp(args[0], "dir") == 0) {
            int saved_stdout = -1;
            if (output_file) {
                saved_stdout = dup(STDOUT_FILENO);
                int fd;
                if (append)
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd >= 0) {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }
            cmd_dir(args, arg_count);
            if (output_file && saved_stdout != -1) {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
        } else if (strcmp(args[0], "environ") == 0) {
            int saved_stdout = -1;
            if (output_file) {
                saved_stdout = dup(STDOUT_FILENO);
                int fd;
                if (append)
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd >= 0) {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }
            cmd_environ();
            if (output_file && saved_stdout != -1) {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
        } else if (strcmp(args[0], "echo") == 0) {
            int saved_stdout = -1;
            if (output_file) {
                saved_stdout = dup(STDOUT_FILENO);
                int fd;
                if (append)
                    fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                else
                    fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (fd >= 0) {
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
            }
            cmd_echo(args, arg_count);
            if (output_file && saved_stdout != -1) {
                dup2(saved_stdout, STDOUT_FILENO);
                close(saved_stdout);
            }
        } else if (strcmp(args[0], "help") == 0) {
            cmd_help();
        } else if (strcmp(args[0], "pause") == 0) {
            cmd_pause();
        } else {
            execute_external(args, background, input_file, output_file, append);
        }
    }

    if (input_stream != stdin) fclose(input_stream);

    printf("Exiting myshell.\n");
    return 0;
}


 
   
          

