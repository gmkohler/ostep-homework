//
// Created by Gregor Köhler on 1/16/24.
//

/**
 * Write a program that opens a file (with the `open()` system call) and
 * then calls `fork()` to create a new process.  Can both the child and the
 * parent access the file descriptor returned by `open()`?  What happens to
 * the when they are writing to the file concurrently, i.e., at the same
 * time?
 */

/**
 * Child and parent can both access the open file descriptor
 * Writing at the same time means both write to the file; order would be
 * non-deterministic unless the parent waits on the child.
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int fd = open("./output.txt", O_CREAT|O_WRONLY|O_TRUNC|S_IRWXU);
    int rc = fork();

    if (rc < 0) {
        fprintf(stderr, "fork failed\n");
        exit(EXIT_FAILURE);
    } else if (rc == 0) {
        char *childBuf = "howdely\n";
        write(fd,childBuf,strlen(childBuf));
    } else {
        wait(NULL);
        char *parentBuf = "doodely\n";
        write(fd,parentBuf,strlen(parentBuf));
    }

    return EXIT_SUCCESS;
}