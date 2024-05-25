; ModuleID = 'clib_close'
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@errno = external global i32
@FAILURE = external global i32
@SUCCESS = external global i32

define i32 @close(i32 %fd) {
entry:
    %result = alloca i64, align 8
    %fd_addr = alloca i32, align 4
    store i32 %fd, i32* %fd_addr, align 4

    %0 = load i32, i32* %fd_addr, align 4
    %conv = zext i32 %0 to i64

    call void asm sideeffect "mov $$3, %rax\n\t"
                             "mov $0, %rdi\n\t"
                             "syscall\n\t"
                             "mov %rax, $1\n\t", "=r,r"(i64* %result, i64 %conv)

    %1 = load i64, i64* %result, align 8
    %cmp = icmp slt i64 %1, 0
    br i1 %cmp, label %if.then, label %if.else

if.then:
    %2 = neg i64 %1
    %conv1 = trunc i64 %2 to i32
    store i32 %conv1, i32* @errno, align 4
    %3 = load i32, i32* @FAILURE, align 4
    ret i32 %3

if.else:
    %4 = load i32, i32* @SUCCESS, align 4
    ret i32 %4
}