; ModuleID = 'clib_open'
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@errno = external global i32
@FAILURE = external global i32

@filename = private unnamed_addr constant [13 x i8] c"testfile.txt\00", align 1

define i32 @open() {
entry:
    %result = alloca i64, align 8

    ; Filename pointer
    %filename_ptr = getelementptr [13 x i8], [13 x i8]* @filename, i32 0, i32 0

    ; File mode
    %flags = alloca i32, align 4
    store i32 577, i32* %flags, align 4   ; O_CREAT | O_WRONLY = 577

    ; File permissions
    %mode = alloca i32, align 4
    store i32 420, i32* %mode, align 4    ; 0644 = 420

    ; Load values for inline assembly
    %flags_val = load i32, i32* %flags, align 4
    %mode_val = load i32, i32* %mode, align 4

    ; Inline assembly for the open syscall
    call void asm sideeffect "mov $$2, %rax\n\t"    ; syscall number for open
                             "mov $0, %rdi\n\t"
                             "mov $1, %rsi\n\t"
                             "mov $2, %rdx\n\t"
                             "syscall\n\t"
                             "mov %rax, $3\n\t", "=r,r,r,r"(i64* %result, i8* %filename_ptr, i32 %flags_val, i32 %mode_val)

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