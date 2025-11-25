// StaticLibrary.cpp : Defines the functions for the static library.
//

#include "pch.h"
#include "framework.h"
#include <cstring>
#include <algorithm>

// filament → strlen
extern "C" __declspec(dllexport) int filament(char* str, int* result)
{
	if (str == nullptr)
	{
		*result = 0;
		return 0;
	}
	*result = (int)strlen(str);
	return 0;
}

// consolidate → strcat
extern "C" __declspec(dllexport) int consolidate(char* dest, char* src, char* result)
{
	if (dest == nullptr || src == nullptr || result == nullptr)
		return -1;
	strcpy_s(result, 256, dest);
	strcat_s(result, 256, src);
	return 0;
}

// compare_scrolls → strcmp
extern "C" __declspec(dllexport) int compare_scrolls(char* str1, char* str2, int* result)
{
	if (str1 == nullptr || str2 == nullptr)
	{
		*result = 0;
		return -1;
	}
	int cmp = strcmp(str1, str2);
	*result = (cmp == 0) ? 1 : 0; // возвращаем 1 если равны, 0 если не равны
	return 0;
}

// mightiness → max
extern "C" __declspec(dllexport) int mightiness(int a, int b, int* result)
{
	*result = (a > b) ? a : b;
	return 0;
}

// TODO: This is an example of a library function
void fnStaticLibrary()
{
}
