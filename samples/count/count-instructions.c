/* Count the number of instructions executed during the run of a
 * program.
 *
 * From http://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1.
 *
 * Demonstrates using ptrace to run a new process under this
 * "debugger" process, and then using single-step to take some action
 * each time an instruction in the "debuggee" is executed.
 *
 * Usage: ./count-instructions sample01
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
        procmsg("starting debugee: %s.\n", progname);

        if (ptrace(PTRACE_TRACEME, 0, 0, 0) < 0) {
            perror("ptrace");
            return -1;
        }

        execl(progname, progname, 0);
        perror("debugee failed to run");
    }

    else {
        procmsg("starting debugger.\n");

        int status;
        unsigned int count = 0;

        waitpid(pid, &status, 0);

        while (WIFSTOPPED(status)) {
            ptrace(PTRACE_SINGLESTEP, pid, 0, 0);
            waitpid(pid, &status, 0);
            ++count;
        }

        waitpid(pid, &status, 0);

        procmsg("debugee executed %d instructions.\n", count);
    }
}
