//
// Created by Gregor Köhler on 1/16/24.
//

/**
 * Write a program that calls `fork()`.  Before calling `fork()`, have the
 *  main process access a variable (e.g., `x`) and set its value to
 *  something (e.g., `100`).  What value is the variable in the child
 *  process?  What happens to the variable when both the child and parent
 *  change the value of x?
 */

 /**
  * Variable in the child process remains the same as it was set before forking.
  * Setting the variable in the child process only (change not seen in the
  * parent process).
  */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main(int argc, char *argv[]) {
    int x;
    x = 100;
    int rc = fork();
    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        x = 20;
        printf("value of x in child: %d\n", x);
    } else {
        printf("value of x in parent: %d\n", x);
    }

    return EXIT_SUCCESS;
}