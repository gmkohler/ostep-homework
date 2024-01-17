//
// Created by Gregor Köhler on 1/16/24.
//

/**
 * Write another program using fork().  The child process should print
 * "hello", the parent process should print "goodbye".  You should try to
 * ensure the parent process always prints first; can you do this _without_
 * calling wait() in the parent?
 */

/**
 * we can use kill to send a signal to the parent
 * could also figure out how to use pipes.  going to lean on some solutions
 * and examples to type those out as I'm inexperienced using them.
 */

#include <getopt.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static sig_atomic_t sigflag = 0;

static void sig_handler(int signum) {
    sigflag = 1;
}

void use_signal();

void use_pipe();

int main(int argc, char *argv[]) {
    int opt;
    struct option options[] = {
            {"signal", no_argument, NULL, 's'},
            {"pipe",   no_argument, NULL, 'p'},
    };
    if ((opt = getopt_long(argc, argv, "sp", options, NULL)) != -1) {
        switch (opt) {
            case 's':
                use_signal();
                break;
            case 'p':
                use_pipe();
                break;
            default:
                fprintf(
                        stderr,
                        "Usage: %s [-s|--signal] [-p|--pipe]\n",
                        argv[0]
                );
                exit(EXIT_FAILURE);
        }
    } else {
        fprintf(stderr, "Usage: %s [-s|--signal] [-p|--pipe]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

// https://github.com/xxyzz/ostep-hw/blob/master/5/3.c
// https://www.gnu.org/software/libc/manual/html_node/Signal-Handling.html
void use_signal() {
    printf("using signal\n");
    struct sigaction action;
    action.sa_handler = sig_handler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;

    if (sigaction(SIGUSR1, &action, NULL) < 0) {
        fprintf(stderr, "signal failure\n");
        exit(EXIT_FAILURE);
    }

    sigset_t sig_mask, old_mask;
    sigemptyset(&sig_mask);
    sigaddset(&sig_mask, SIGUSR1);
    if (sigprocmask(SIG_BLOCK, &sig_mask, &old_mask) < 0) {
        fprintf(stderr, "signal failure 2\n");
        exit(EXIT_FAILURE);
    }

    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (rc == 0) {
        printf("hello\n");
        kill(getppid(), SIGUSR1);
    } else {
        while (sigflag == 0) {
            sigsuspend(&old_mask);
        }

        printf("goodbye\n");
    }
}

// https://www.gnu.org/software/libc/manual/html_node/Creating-a-Pipe.html
void use_pipe() {
    printf("using pipe\n");
    pid_t pid;
    int mypipe[2];
    if (pipe(mypipe)) {
        fprintf(stderr, "pipe failed.\n");
    }
    pid = fork();
    if (pid < (pid_t) 0) {
        fprintf(stderr, "fork failed.\n");
        exit(EXIT_FAILURE);
    } else if (pid == (pid_t) 0) {
        close(mypipe[0]);

        FILE *stream;
        stream = fdopen(mypipe[1], "w");
        fprintf(stream, "hello\n");
        fclose(stream);
    } else {
        close(mypipe[1]);

        FILE *stream;
        int c;
        stream = fdopen(mypipe[0], "r");
        while ((c = fgetc(stream)) != EOF) {
            putchar(c);
        }
        fclose(stream);
        printf("goodbye\n");
    }
}
