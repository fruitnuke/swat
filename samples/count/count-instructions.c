/* Count the number of instructions executed during the run of a
 * program.
 *
 * From http://eli.thegreenplace.net/2011/01/23/how-debuggers-work-part-1.
 *
 * Demonstrates using ptrace to run a new process under this
 * "debugger" process, and then using single-step to take some action
 * each time an instruction in the "debuggee" is executed.
 */

#include <stdio.h>


int main(int argc, char** argv)
{
    if (argc < 2) {
        fprintf(stderr, "Expected a program name as argument.\n");
        return -1;
    }
}
