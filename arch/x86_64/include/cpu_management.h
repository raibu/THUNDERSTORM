#include <stdint.h>
/* Input a byte from a port */
// --------------------------------------------------------------------------
// From NOPE-OS
// --------------------------------------------------------------------------
// arminb@aundb-online.de
// --------------------------------------------------------------------------

__attribute__ ( ( always_inline ) ) 
inline void io_wait(void)
{
    /* Port 0x80 is used for 'checkpoints' during POST. */
    /* The Linux kernel seems to think it is free for use :-/ */
    asm volatile ( "outb %%al, $0x80" : : "a"(0) );
}

/* Input a byte from a port */
__attribute__ ( ( always_inline ) ) 
inline unsigned char inb(uint16_t port)
{
  volatile unsigned char ret;
  asm volatile ("inb %%dx,%%al":"=a" (ret):"d" (port));
  return ret;
}

/* Output a byte to a port */

__attribute__ ( ( always_inline ) ) 
inline void outb(uint16_t port, uint8_t value)
{
  asm volatile ("outb %%al,%%dx": :"d" (port), "a" (value));
}

