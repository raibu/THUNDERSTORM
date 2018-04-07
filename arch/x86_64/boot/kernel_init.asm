bits 64
global kernel_init
extern kputstr_to

GREEN equ 0x2
; kernel x64 port early initialization
section .rodata
  verify_x64: db " Long mode initialized", 0
section .text
kernel_init:
  mov rax, verify_x64
  mov cl, GREEN
  call kputstr_to
  hlt
