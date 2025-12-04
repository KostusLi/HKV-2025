#include "pch.h"
#include <cstring>
#include <cstdio>

// Force rebuild - updated for stdcall compatibility

extern "C" int __stdcall filament(char* str, int* result)
{
    if (!str) { *result = 0; return 0; }
    *result = (int)strlen(str);
    return 0;
}

extern "C" int __stdcall consolidate(char* dest, char* src, char* result)
{
    if (!dest || !src || !result) return -1;
    strcpy_s(result, 256, dest);
    strcat_s(result, 256, src);
    return 0;
}

extern "C" int __stdcall compare_scrolls(char* a, char* b, int* result)
{
    if (!a || !b) { *result = 0; return -1; }
    *result = (strcmp(a, b) == 0) ? 1 : 0;
    return 0;
}

extern "C" int __stdcall mightiness(int a, int b, int* result)
{
    *result = (a > b) ? a : b;
    return 0;
}

extern "C" void __stdcall outrad(char* str)
{
    if (str) {
        printf("%s", str);
        fflush(stdout);
    }
}

extern "C" void __stdcall confession(int data, int type)
{
    switch (type)
    {
    case 1: // INT (squire) - целое число
        printf("%d", data);
        break;
    case 2: // STR (scroll) - строка
        // data интерпретируется как указатель на строку
        if ((char*)data != nullptr)
        {
            printf("%s", (char*)data);
        }
        break;
    case 4: // CHAR (rune) - символ
        // data интерпретируется как указатель на строку с одним символом
        if ((char*)data != nullptr)
        {
            printf("%c", ((char*)data)[0]);
        }
        break;
    }
    fflush(stdout);
}
