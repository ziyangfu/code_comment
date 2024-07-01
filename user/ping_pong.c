#include "kernel/types.h"
#include "user/user.h"
// 测试使用管道进行进程间通信

# define MESSAGE "ping"
int main() {
    //int start = uptime();
    int p[2];  // 两个描述符，0 读 1 写
    int pid = fork();
    if (pipe(p) < 0) {  // 创建管道
        //panic("pipe");
        printf("pipe error\n");
    }

    if (pid > 0) {  // 父进程
       close(p[0]); // 关闭读端
       write(p[1], MESSAGE, sizeof(MESSAGE));
       printf("Parent send %s \n", "ping");
       close(p[1]); // 写入完毕
       sleep(2);
    }
    else if (pid == 0) {  // 子进程
        close(p[1]); // 关闭写端
        char buf[10];
        memset(buf, 0, 10);
        while (read(p[0], buf, sizeof(buf)) > 0)// 读管道
        {
            printf("Child recv %s \n", buf); 
        }
        close(p[0]);
          
    }
    //int end = uptime();
    //int cloc = end - start;
    //printf("time: %d\n", cloc);


    return 0;
}