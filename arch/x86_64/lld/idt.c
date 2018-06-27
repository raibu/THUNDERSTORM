#include <stdint.h>
#include <gcc_opt.h>
#include <x86_64/idt.h>
#define x64_LOW(X) ( (uint64_t) (X) & 0xFFFF )
#define x64_MIDDLE(X) ( (uint64_t) (X) >> 16 & 0xFFFF )
#define x64_HIGH(X) ( (uint64_t) (X) >> 32 )
struct IDTdesc idt_table[NUM_OF_DESC];
UNLIKELY void 
idt_set_vector(uint8_t num, uint64_t addr, 
    uint16_t sel, uint8_t attr, uint8_t ist)
{
  idt_table[num] = (struct IDTdesc)
  {
    .offset_0_15  = x64_LOW(addr),
    .selector     = sel,
    .ist          = ist,
    .type_attr    = attr,
    .offset_16_31 = x64_MIDDLE(addr),
    .offset_32_63 = x64_HIGH(addr),
    .zero         = 0
  };
}
