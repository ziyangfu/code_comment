#include "kernel/types.h"
#include "user/user.h"

int main() {
    int pid = fork();

    int start = uptime();
    int p[2];
    if (pipe(p) < 0) {
        panic("pipe");
    }

    if (pid > 0) {
       

    }
    else if (pid == 0) {

    }


    int end = uptime();
    int cloc = end - start;


    return 0;
}