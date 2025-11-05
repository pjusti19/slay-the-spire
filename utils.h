#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdlib.h>

void must_init(_Bool test, const char* description);

int NumToDigits(int n);

void ClearKeyboardKeys(unsigned char* keyboard_keys);

void ShuffleArray(int* array, int size);

#endif