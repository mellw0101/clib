; ModuleID = 'clib_perror'
target triple = "x86_64-pc-linux-gnu"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"

@.str = private unnamed_addr constant [22 x i8] c"Custom close fail\00", align 1

declare void @perror(i8*)

define void @call_perror() {
entry:
    %str = getelementptr inbounds [22 x i8], [22 x i8]* @.str, i32 0, i32 0
    call void @perror(i8* %str)
    ret void
}