set disassembly-flavor intel
target remote localhost:1234
file boot/THkernel
break _start
break start_kernel
layout reg
