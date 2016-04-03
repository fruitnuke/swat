/* Count the number of instructions executed during the run of a
 * program.
 *
 * From http://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1.
 *
 * Demonstrates using ptrace to run a new process under this
 * "debugger" process, and then using single-step to take some action
 * each time an instruction in the "debuggee" is executed.
 */

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <signal.h>


/* Print messages to stdout, prefixed with the processes pid. */
void procmsg(const char* format, ...)
{
    printf("[%u] ", getpid());
    va_list ap;
    va_start(ap, format);
    vprintf(format, ap);
    va_end(ap);
}


int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Expected a program name as argument.\n");
        return -1;
    }

    const char* progname = argv[1];
    pid_t pid;

    if ((pid = fork()) < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        // Child process, the debuggee.
        procmsg("starting %s.\n", progname);

        execl(progname, progname, 0);
    }
    else {
        // Parent process, the debugger.
        procmsg("starting debugger.\n");

        int status;
        waitpid(pid, &status, 0);

        procmsg("debugger exiting.\n");
    }
}
