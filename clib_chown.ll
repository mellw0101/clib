; ModuleID = 'chown_example'
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@errno = external global i32
@FAILURE = external global i32

@filename = private unnamed_addr constant [14 x i8] c"example.txt\00", align 1

declare i64 @syscall(i64, ...) 

define i32 @change_owner(i32 %uid, i32 %gid) {
entry:
    %result = alloca i64, align 8

    ; Filename pointer
    %filename_ptr = getelementptr [14 x i8], [14 x i8]* @filename, i32 0, i32 0

    ; Cast uid and gid to 64-bit integers
    %uid64 = zext i32 %uid to i64
    %gid64 = zext i32 %gid to i64

    ; Inline assembly for the chown syscall (syscall number 92 on x86_64)
    call void asm sideeffect "mov $$92, %rax\n\t"
                             "mov $0, %rdi\n\t"
                             "mov $1, %rsi\n\t"
                             "mov $2, %rdx\n\t"
                             "syscall\n\t"
                             "mov %rax, $3\n\t", "=r,r,r,r"(i64* %result, i8* %filename_ptr, i64 %uid64, i64 %gid64)

    ; Check result and handle errors
    %result_val = load i64, i64* %result, align 8
    %cmp = icmp slt i64 %result_val, 0
    br i1 %cmp, label %if.then, label %if.else

if.then:
    %neg_result = neg i64 %result_val
    %errno_val = trunc i64 %neg_result to i32
    store i32 %errno_val, i32* @errno, align 4
    %failure = load i32, i32* @FAILURE, align 4
    ret i32 %failure

if.else:
    %success = trunc i64 %result_val to i32
    ret i32 %success
}