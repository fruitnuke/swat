#include <stdint.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>


int main()
{
    const char * progname = "sample";
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return -1;
    }

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        execl(progname, progname, 0);
    }

    else {
        int status;
        waitpid(pid, &status, 0);

        if (WIFSTOPPED(status)) {
            uintptr_t addr = 0x4000c6;
            uintptr_t data = ptrace(PTRACE_PEEKTEXT, pid, (void*)addr, 0);
            printf("Original data at 0x%16lx: 0x%16lx\n", addr, data);

            uintptr_t data_with_bp = (data & 0xFFFFFFFFFFFFFF00) | 0xCC;
            ptrace(PTRACE_POKETEXT, pid, (void*)addr, (void*)data_with_bp);

            ptrace(PTRACE_CONT, pid, 0, 0);
            waitpid(pid, &status, 0);
            if (WIFSTOPPED(status)) {
                printf("breakpoint reached.\n");

                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS, pid, 0, &regs);

                // "rip" replace "eip" on x86-64.
                regs.rip -= 1;

                ptrace(PTRACE_POKETEXT, pid, (void*)addr, (void*)data);
                ptrace(PTRACE_SETREGS, pid, 0, &regs);
                ptrace(PTRACE_CONT, pid, 0, 0);
            }
        }
    }
}
