/**
 * Created by Gregor Köhler on 1/17/24.
 *
 * Write a program that creates two children, and connects the standard
 * output of one to the standard input of the other, using the pipe() system
 * call.
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int mypipe[2];
    if (pipe(mypipe)) {
        fprintf(stderr, "pipe failed\n");
        exit(EXIT_FAILURE);
    }

    pid_t pid_writer = fork();
    if (pid_writer < 0) {
        fprintf(stderr, "fork of writer failed\n");
        exit(EXIT_FAILURE);
    } else if (pid_writer == 0) {
        close(mypipe[0]);
        if (mypipe[1] != STDOUT_FILENO) {
            if (dup2(mypipe[1], STDOUT_FILENO) != STDOUT_FILENO) {
                fprintf(stderr,
                        "connecting pipe to stdout in writer failed\n");
                exit(EXIT_FAILURE);
            }
            close(mypipe[1]);
        }
        printf("hello from writer process\n");
    } else {
        pid_t pid_reader = fork();
        if (pid_reader < 0) {
            fprintf(stderr, "fork of reader failed\n");
            exit(EXIT_FAILURE);
        } else if (pid_reader == 0) {
            close(mypipe[1]);
            if (mypipe[0] != STDIN_FILENO) {
                if (dup2(mypipe[0], STDIN_FILENO) != STDIN_FILENO) {
                    fprintf(stderr,
                            "connecting pipe to stdin in reader failed\n");
                    exit(EXIT_FAILURE);
                }
                close(mypipe[0]);
            }

            char buf[BUFSIZ];
            read(STDIN_FILENO, &buf, BUFSIZ);
            printf("%s", buf);
            printf("hello from reader process\n");
        } else {
            if (waitpid(pid_writer, NULL, 0) < 0) {
                fprintf(stderr, "waiting on writer to finish failed\n");
                exit(EXIT_FAILURE);
            }
            if (waitpid(pid_reader, NULL, 0) < 0) {
                fprintf(stderr, "waiting on reader to finish failed\n");
                exit(EXIT_FAILURE);
            }
        }
    }
    exit(EXIT_SUCCESS);
}