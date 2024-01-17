/**
 * Created by Gregor Köhler on 1/17/24.
 *
 * Now write a program that uses wait() to wait for the child process to
 * finish in the parent.  What does wait() return?  What happens if you use
 * wait in the child?
 *
 * Waiting in child fails; it has no children processes.  It also seems to
 * move back to the parent process because the parent log happens before the
 * wait-failed log.
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
   pid_t pid = fork();
   if(pid < 0) {
       fprintf(stderr, "fork failed\n");
       exit(EXIT_FAILURE);
   } else if (pid == 0) {
        printf("in child (pid:%d), spawned by parent (pid:%d)\n", pid, getppid());
        pid_t wait_pid = wait(NULL);
        if (wait_pid < 0) {
            fprintf(stderr, "wait failed\n");
        }
   } else {
       pid_t wait_pid = wait(NULL);
       if (wait_pid < 0) {
           fprintf(stderr, "wait failed\n");
           exit(EXIT_FAILURE);
       } else {
           printf("waited for child (pid:%d), from parent (pid:%d)\n", wait_pid, getpid());
       }
   }


}
