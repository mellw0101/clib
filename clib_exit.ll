; ModuleID = 'clib_exit'
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

define void @exit(i8 %status) #0 {
entry:
    %conv = zext i8 %status to i32
    call void asm "movl $$1, %eax\n\t",
                  "movl $0, %ebx\n\t"
                  "int $$0x80\n\t"
                  "hlt\n\t", "r,~{eax},~{ebx},~{memory}"(i32 %conv)
    unreachable
}