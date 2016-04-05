/* Set and execute a breakpoint on a child process.
 *
 * Works by manually writing the int 3 opcode at the desired memory
 * location in the child program's code. When the interrupt is reached
 * the parent is signaled, which then replaces the original
 * instruction before continuing.
 *
 * This code is 64-bit specific due to the difference in the
 * definition of the user_regs_struct between 32-bit and 64-bit
 * systems.
 */

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>


int main()
{
    const char *progname = "sample";
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
            intptr_t addr = 0x4000c6;
            intptr_t data;

            data = (intptr_t)ptrace(PTRACE_PEEKTEXT, pid, (void *)addr, 0);

            if (data == -1 && errno) {
                perror("peektext");
                return -1;
            }

            printf("Original data at %p: 0x%x\n", (void *)addr, (int)(data & 0xFF));

            intptr_t mask = UINTPTR_MAX - 0xFF;
            intptr_t data_with_bp = (data & mask) | 0xCC;

            ptrace(PTRACE_POKETEXT, pid, (void *)addr, (void *)data_with_bp);

            ptrace(PTRACE_CONT, pid, 0, 0);
            waitpid(pid, &status, 0);
            if (WIFSTOPPED(status)) {
                printf("breakpoint reached.\n");

                struct user_regs_struct regs;
                ptrace(PTRACE_GETREGS, pid, 0, &regs);

                // Name of the ip register on the user_regs_struct on 64-bit systems.
                regs.rip -= 1;

                ptrace(PTRACE_POKETEXT, pid, (void *)addr, (void *)data);
                ptrace(PTRACE_SETREGS, pid, 0, &regs);
                ptrace(PTRACE_CONT, pid, 0, 0);
            }
        }
    }
}
