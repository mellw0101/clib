extern char* asm_strspy(char* dest, const char* src);

char* clib_strcpy(char* dest, const char* src) {
    return asm_strspy(dest, src);
}