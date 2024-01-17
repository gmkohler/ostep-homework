/**
 * Created by Gregor Köhler on 1/17/24.
 *
 * Write a program that calls `fork()` and then calls some form of `exec()`
 * to run the program `/bin/ls`.  See if you can try all the variants of exec(),
 * including (on Linux) `execl()`, `execle()`, `execv()`, `execvp()`, and
 * `execvpe()`.  Why do you think there are so many variants of the same
 * basic call?
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
    char *args[6] = {
            strdup("ls"),
            strdup("-a"),
            strdup("-h"),
            strdup("-l"),
            strdup("./cpu-api"),
            (char *) NULL
    };
    char *env[2] = {strdup("PATH=/bin:/usr/bin"), (char *) NULL};

    pid_t pid;
    /**
     * ==== execv ====
     */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("execv:\n");
        if (execv("/bin/ls", args) == -1) {
            fprintf(stderr, "execv failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if (wait(NULL) == -1) {
        fprintf(stderr, "wait failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");

    /**
     * ==== execvp ====
     */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("execvp:\n");
        if (execvp("ls", args) == -1) {
            fprintf(stderr, "execvp failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if (wait(NULL) == -1) {
        fprintf(stderr, "wait failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");

    /**
     * ==== execvP ====
     */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("execvP:\n");
        if (execvP("ls", "/bin:/usr/bin", args) == -1) {
            fprintf(stderr, "execvP failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if (wait(NULL) == -1) {
        fprintf(stderr, "wait failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");

    /**
     * ==== execl ====
     */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("execl:\n");
        if (execl("/bin/ls", args[0], args[1], args[2], args[3], args[4],
                  args[5]) == -1) {
            fprintf(stderr, "execl failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if (wait(NULL) == -1) {
        fprintf(stderr, "wait failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");

    /**
     * ==== execle ====
     */
    pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        printf("execle:\n");
        if (execle("/bin/ls", args[0], args[1], args[2], args[3], args[4],
                   args[5], env) == -1) {
            fprintf(stderr, "execle failed\n");
            exit(EXIT_FAILURE);
        }
    }
    if (wait(NULL) == -1) {
        fprintf(stderr, "wait failed\n");
        exit(EXIT_FAILURE);
    }
    printf("\n");

    return EXIT_SUCCESS;
}
