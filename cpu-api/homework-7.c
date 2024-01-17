/**
 * Created by Gregor Köhler on 1/17/24.
 *
 * Write a program that creates a child process, and then in the child
 *  closes standard output (STDOUT_FILENO).  What happens if the child
 *  calls printf() to print some output after closing the descriptor?
 *
 *  The text being printed doesn't get printed to the screen, but the call
 *  does not fail.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork failed\n");
    } else if (pid == 0) {
        int result = close(STDOUT_FILENO);
        if (result < 0) {
            fprintf(stderr, "closing stdout failed\n");
        }
        printf("this should fail\n");
    } else {
        wait(NULL);
        printf("back in parent\n");
    }

    return EXIT_SUCCESS;
}
