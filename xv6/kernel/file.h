struct file {
  enum { FD_NONE, FD_PIPE, FD_INODE, FD_DEVICE } type;
  int ref; // reference count 引用计数
  char readable;      // 读权限
  char writable;      // 写权限
  struct pipe *pipe; // FD_PIPE
  struct inode *ip;  // FD_INODE and FD_DEVICE
  uint off;          // FD_INODE
  short major;       // FD_DEVICE
};

#define major(dev)  ((dev) >> 16 & 0xFFFF)  // 取dev的前32位
#define minor(dev)  ((dev) & 0xFFFF)        // 取dev的后32位
#define	mkdev(m,n)  ((uint)((m)<<16| (n)))

// in-memory copy of an inode
// 描述保存给定文件的元数据的结构
struct inode {
  uint dev;           // Device number
  uint inum;          // Inode numberds 
  int ref;            // Reference count
  struct sleeplock lock; // protects everything below here
  int valid;          // inode has been read from disk?

  short type;         // copy of disk inode
  short major;
  short minor;
  short nlink;
  uint size;
  uint addrs[NDIRECT+1];
};

// map major device number to device functions.
struct devsw {
  int (*read)(int, uint64, int);
  int (*write)(int, uint64, int);
};

extern struct devsw devsw[];

#define CONSOLE 1
