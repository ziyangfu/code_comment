#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

// Memory allocator by Kernighan and Ritchie,
// The C programming Language, 2nd ed.  Section 8.7.

typedef long Align;

union header {
  struct {
    union header *ptr;
    uint size;
  } s;
  Align x;
};

typedef union header Header;

static Header base;
static Header *freep;
// 这里的free，并没有真正将内存还给OS
void
free(void *ap)
{
  Header *bp, *p;

  bp = (Header*)ap - 1;
  for(p = freep; !(bp > p && bp < p->s.ptr); p = p->s.ptr)
    if(p >= p->s.ptr && (bp > p || bp < p->s.ptr))
      break;
  if(bp + bp->s.size == p->s.ptr){
    bp->s.size += p->s.ptr->s.size;
    bp->s.ptr = p->s.ptr->s.ptr;
  } else
    bp->s.ptr = p->s.ptr;
  if(p + p->s.size == bp){
    p->s.size += bp->s.size;
    p->s.ptr = bp->s.ptr;
  } else
    p->s.ptr = bp;
  freep = p;
}
// 用于向系统申请一块较大的内存空间，并将其加入到内存池中
static Header*
morecore(uint nu)
{
  char *p;
  Header *hp;

  if(nu < 4096)
    nu = 4096;
  p = sbrk(nu * sizeof(Header));
  if(p == (char*)-1)
    return 0;
  hp = (Header*)p;
  hp->s.size = nu;
  free((void*)(hp + 1));
  return freep;
}
/**
通义千问：

这段代码是C语言中的malloc函数实现的一部分。malloc函数用于动态分配内存空间，
其参数nbytes表示需要分配的字节数。函数内部通过一系列操作为程序分配指定大小的内存，
并返回该内存的起始地址。

首先，根据需要分配的字节数nbytes计算出需要分配的内存块的单位数nunits。
计算方法是将nbytes加上Header结构体大小减1，然后除以Header结构体大小再加1。
Header结构体用于存储内存块的相关信息。

接着，判断内存池的起始指针freep是否为空。如果为空，则初始化内存池。
将base作为内存池的起始节点，其指针ptr指向自身，大小size为0。

然后，通过循环遍历内存池中的内存块，寻找一个大小不小于nunits的空闲内存块。

如果找到了大小正好等于nunits的内存块，则将该内存块从内存池中删除，
即将其前一个内存块的指针ptr指向该内存块的下一个内存块。

如果找到了大小大于nunits的内存块，则将该内存块分割成两部分，
前一部分大小为nunits，后一部分大小为原内存块大小减去nunits。然后返回前一部分内存块的起始地址。

如果遍历完内存池中的所有内存块仍未找到合适的内存块，
则调用morecore函数从操作系统中获取更多的内存空间，并将获取到的内存空间加入到内存池中。

最后，返回分配到的内存块的起始地址。如果未能成功分配内存，则返回NULL。
*/
void*
malloc(uint nbytes)
{
  Header *p, *prevp;
  uint nunits;

  nunits = (nbytes + sizeof(Header) - 1)/sizeof(Header) + 1;
  if((prevp = freep) == 0){
    base.s.ptr = freep = prevp = &base;
    base.s.size = 0;
  }
  for(p = prevp->s.ptr; ; prevp = p, p = p->s.ptr){
    if(p->s.size >= nunits){
      if(p->s.size == nunits)
        prevp->s.ptr = p->s.ptr;
      else {
        p->s.size -= nunits;
        p += p->s.size;
        p->s.size = nunits;
      }
      freep = prevp;
      return (void*)(p + 1);
    }
    if(p == freep)
      if((p = morecore(nunits)) == 0)
        return 0;
  }
}
