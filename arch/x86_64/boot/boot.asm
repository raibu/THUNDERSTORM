; Copyright (C) 2018  Igor Muravyov <igor.muravyov.2015@gmail.com>
; CPU initialization for THUNDERSTORM kernel 
; USES MULTIBOOT 2 SPEC AND VGA OUTPUT
; constants:
extern kputstr_32
extern kputchar_to
extern kernel_phys_base
extern early_kernel_init
extern tss_table
extern die
extern p4_table
extern p3_table
extern p2_table
extern p1_table

global GDT_tss_desc; <-- for further initialization
global GDT_tss_sel
global init_paging; 
global boot_magic
global boot_info

global exc_stack_bottom
global exc_stack_top

global stack_bottom; Main stack
global stack_top

GREEN equ 0x2
RED equ 0x4f

STK_SIZE equ 4096; 4KB for kernel initialization stack
PG_SIZE equ (4096); in bytes
PG_SIZE_QW equ PG_SIZE/8
global _start; EBX <-- pointer to boot information format
bits 32
section .text
_start:
        cli
        mov esp,            stack_top ; creating stack:
        mov si,             GREEN ;<- <cl> color info;
        mov [boot_magic],   eax ; Store BOOT MAGIC and put as 1st arg
        mov [boot_info],    ebx ; Store BOOT info and put as 2nd arg
section .rodata
thinfo:         db "THUNDERSTORM 0.0 Embedded system x86_64 port layer",0
chk_cpuid:      db "cpuid checked!",0
section .text
        mov   edi,    thinfo
        call  kputstr_32
        call  check_cpuid
        mov   edi,    chk_cpuid
        call  kputstr_32
        call  check_long_mode
    ;start cpu reinitialization
        call set_paging
        call init_paging
        call pause
        lgdt [GDT64.pointer]
        call pause
    ; already x86_64 here
        jmp  GDT64.code:.init64; update CS

.init64:
bits 64
; clearing all legacy garbage
        mov ax, 0
        mov ss, ax
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov gs, ax
        jmp early_kernel_init

bits 32
section .data
; GDT long mode initialization
        align 8
GDT64:                           ; Global Descriptor Table (64-bit).
    .null: equ $ - GDT64         ; The null descriptor.
        dw 0xFFFF                    ; Limit (low).
        dw 0                         ; Base (low).
        db 0                         ; Base (middle)
        db 0                         ; Access.
        db 1                         ; Granularity.
        db 0                         ; Base (high).
    .code: equ $ - GDT64         ; The code descriptor.
        dw 0                         ; Limit (low).
        dw 0                         ; Base (low).
        db 0                         ; Base (middle)
        db 10011010b                 ; Access (exec/read).
        db 10101111b                 ; Granularity, 64 bits flag, limit19:16.
        db 0                         ; Base (high).
    .data: equ $ - GDT64         ; The data descriptor.
        dw 0                         ; Limit (low).
        dw 0                         ; Base (low).
        db 0                         ; Base (middle)
        db 10010010b                 ; Access (read/write).
        db 00000000b                 ; Granularity.
        db 0                         ; Base (high).
    ;tss descriptor will be initialized after,
    ; now it`s filled with NULL descriptors
    .tss: equ $ - GDT64          ; TSS descriptor
    .tss_late_init:
        dw 0xFFFF                    ; Limit (low).
        dw 0                         ; Base (low).  
        db 0                         ; Base (middle)
        db 0                         ; Access.
        db 1                         ; Granularity.
        db 0                         ; Base (high).
        dw 0xFFFF                    ; Limit (low).
        dw 0                         ; Base (low).  
        db 0                         ; Base (middle)
        db 0                         ; Access.
        db 1                         ; Granularity.
        db 0                         ; Base (high).

    .pointer:                    ; The GDT-pointer.
        dw $ - GDT64 - 1             ; Limit.
        dq GDT64                     ; Base.
        dq 0
section .text
        hlt

init_paging:
        push eax
        push ecx
        mov eax, p4_table
        mov cr3, eax
  ; PAE for huge pages use
        mov eax, cr4
        or eax, 1<<5
        mov cr4, eax
        mov ecx, 0xC0000080
        rdmsr
        or eax, 1<<8
        wrmsr
        mov eax, cr0
        or eax, 1<<31
        mov cr0, eax
        pop ecx
        pop eax
        ret
set_paging:
; p4[0] -> p3[0] -> p2[0...PG_SIZE] -> 2GB - 2MB memory

;p4[0] -> p3[0]
        mov eax,  p3_table
        or  eax,  0b11; writable, exists
        mov [p4_table], eax

;p3[0] -> p2[0]
        mov eax,  p2_table
        or  eax,  0b11; writable, exists
        mov [p3_table], eax

;p2[0...PG_SIZE] => address space[0; 2GB];
        mov ebx,  0; map 1st region to p2[0], 2MB to the second...
.map:
        mov eax,  2<<20; 2MB
        mul ebx
        or  eax,  0b10000011; writable, exists, extended
        mov [p2_table + ebx * 8], eax
        add ebx,  1; sizeof QW
        cmp ebx,  PG_SIZE_QW;
        jne .map
        ret

.no_multiboot:
        mov edi, .nomultiboot_err
        jmp die 
section .rodata
.nomultiboot_err: 
        db "OOPs: Invalid multiboot2 magic data", 0
section .text

check_cpuid:
        push ecx
    ; Check if CPUID is supported by attempting to flip the ID bit (bit 21)
    ; in the FLAGS register. If we can flip it, CPUID is available.

    ; Copy FLAGS in to EAX via stack
        pushfd
        pop eax

    ; Copy to ECX as well for comparing later on
        mov ecx, eax

    ; Flip the ID bit
        xor eax, 1 << 21

    ; Copy EAX to FLAGS via the stack
        push eax
        popfd

    ; Copy FLAGS back to EAX (with the flipped bit if CPUID is supported)
        pushfd
        pop eax

    ; Restore FLAGS from the old version stored in ECX (i.e. flipping the
    ; ID bit back if it was ever flipped).
        push ecx
        popfd

    ; Compare EAX and ECX. If they are equal then that means the bit
    ; wasn't flipped, and CPUID isn't supported.
        cmp eax, ecx
        je .no_cpuid
        pop ecx
        ret
.no_cpuid:
section .rodata
chk_cpuid_failed: db "CPUID is not supported ",0
section .text
        mov edi, chk_cpuid_failed
        jmp die

check_long_mode:
    ; test if extended processor info in available
        mov eax, 0x80000000    ; implicit argument for cpuid
        cpuid                  ; get highest supported argument
        cmp eax, 0x80000001    ; it needs to be at least 0x80000001
        jb .no_long_mode       ; if it's less, the CPU is too old for long mode

    ; use extended info to test if long mode is available
        mov eax, 0x80000001    ; argument for extended processor info
        cpuid                  ; returns various feature bits in ecx and edx
        test edx, 1 << 29      ; test if the LM-bit is set in the D-register
        jz .no_long_mode       ; If it's not set, there is no long mode
        ret
.no_long_mode:
section .rodata
chk_long_failed: db "this CPU is not supported by this OS",0
section .text
        mov edi, chk_long_failed
        jmp die
warning:
        mov si, RED
        call kputstr_32
        mov si, GREEN
        ret

pause:
        mov ax, -1
.jmp:
        sub ax, 1
        cmp ax, 0
        jne .jmp
        ret

;;;;;;;;;;;;;;;;;;; GLOBAL ;;;;;;;;;;;;;;;;;;;;;;;;
; constants:
section .rodata
GDT_tss_desc:   dq GDT64.tss_late_init
GDT_tss_sel:    dq GDT64.tss
; data:
section .data
boot_magic:     dq  0
boot_info:      dq  0
; bss:
section .bss

 ; memory, reserved for STACK:
  alignb PG_SIZE
stack_bottom: ; 
  resb STK_SIZE
  alignb PG_SIZE
stack_top:
  resb PG_SIZE
