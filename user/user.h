struct stat;

// system calls
/**
 * man syscall的结果：
 * 可以看到 risc-v的syscall指令为 ecall， xv6中采用usys.pl脚本生成， 生成代码是usys.S
       Arch/ABI    Instruction           System  Ret  Ret  Error    Notes
                                         call #  val  val2
       ───────────────────────────────────────────────────────────────────
       alpha       callsys               v0      v0   a4   a3       1, 6
       arc         trap0                 r8      r0   -    -
       arm/OABI    swi NR                -       a1   -    -        2
       arm/EABI    swi 0x0               r7      r0   r1   -
       arm64       svc #0                x8      x0   x1   -
       blackfin    excpt 0x0             P0      R0   -    -
       i386        int $0x80             eax     eax  edx  -
       ia64        break 0x100000        r15     r8   r9   r10      1, 6
       m68k        trap #0               d0      d0   -    -
       microblaze  brki r14,8            r12     r3   -    -
       mips        syscall               v0      v0   v1   a3       1, 6
       nios2       trap                  r2      r2   -    r7
       parisc      ble 0x100(%sr2, %r0)  r20     r28  -    -
       powerpc     sc                    r0      r3   -    r0       1
       powerpc64   sc                    r0      r3   -    cr0.SO   1
       riscv       ecall                 a7      a0   a1   -
       s390        svc 0                 r1      r2   r3   -        3
       s390x       svc 0                 r1      r2   r3   -        3
       superh      trap #0x17            r3      r0   r1   -        4, 6
       sparc/32    t 0x10                g1      o0   o1   psr/csr  1, 6
       sparc/64    t 0x6d                g1      o0   o1   psr/csr  1, 6
       tile        swint1                R10     R00  -    R01      1
       x86-64      syscall               rax     rax  rdx  -        5
       x32         syscall               rax     rax  rdx  -        5
       xtensa      syscall               a2      a2   -    -
*/
// 以下是系统调用
int fork(void);   // 创建一个新的进程，返回值为新进程的pid
int exit(int) __attribute__((noreturn)); // 终止当前进程的执行，并返回指定的退出状态码
int wait(int*); // 等待子进程结束，并将子进程的退出状态码存储在指定的整型指针中
int pipe(int*); // 创建一个管道，返回两个文件描述符，分别用于读和写
int write(int, const void*, int); // 向指定的文件描述符写入指定长度的数据
int read(int, void*, int);  //从指定的文件描述符读取指定长度的数据
int close(int);  //关闭指定的文件描述符
int kill(int);  // 发送一个信号给指定的进程
int exec(const char*, char**);  // 用新的程序替换当前程序
int open(const char*, int);  // 打开指定的文件，返回文件描述符
int mknod(const char*, short, short);  // 创建一个特殊文件或设备文件
int unlink(const char*);   // 删除指定的文件
int fstat(int fd, struct stat*);  // 获取指定文件描述符对应的文件状态
int link(const char*, const char*);  // 创建一个指向指定文件的新链接
int mkdir(const char*);  // 创建一个目录
int chdir(const char*); // 将当前工作目录更改为指定的目录
int dup(int);           // 复制指定的文件描述符
int getpid(void);       // 获取当前进程的ID
char* sbrk(int);        // 用于虚拟内存分配
int sleep(int);         // 使当前进程暂停指定的秒数
int uptime(void);       // 返回系统运行时间

// ulib.c
// 以下是C库函数
int stat(const char*, struct stat*); // 用于获取文件或目录的相关信息，将信息存储在结构体stat中
char* strcpy(char*, const char*); //  将源字符串复制到目标字符串中
void *memmove(void*, const void*, int);//  用于将内存区域中的数据从一个位置移动到另一个位置，可以进行重叠区域的移动
char* strchr(const char*, char c); //在字符串中查找指定字符第一次出现的位置，并返回指向该位置的指针
int strcmp(const char*, const char*); // 比较两个字符串的大小，返回值小于、等于、大于0表示第一个不相同字符的大小关系
void fprintf(int, const char*, ...); // 实现在printf.c文件中，格式化输出函数，将数据输出到指定的文件描述符中
void printf(const char*, ...); // 格式化输出函数，将数据输出到标准输出中
char* gets(char*, int max); // 从标准输入中读取一行字符，并存储到指定的字符串中
uint strlen(const char*); // 计算字符串的长度，不包括结尾的空字符
void* memset(void*, int, uint); // 将指定内存区域的每个字节设置为指定的值
void* malloc(uint); // 分配指定大小的内存空间，并返回指向分配的内存空间的指针
void free(void*);   // 释放指定的内存空间
int atoi(const char*);  // 将字符串转换为整数
int memcmp(const void *, const void *, uint); // 比较两个内存区域的内容，返回值小于、等于、大于0表示第一个不相同字符的大小关系
void *memcpy(void *, const void *, uint); // 将内存区域中的数据从一个位置复制到另一个位置
