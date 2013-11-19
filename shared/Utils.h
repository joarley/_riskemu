#ifndef _RISKEMULIBRARY_UTILS_H_
#define _RISKEMULIBRARY_UTILS_H_

#include "stdtypes.h"
#include <cstring>
#include <cstdio>

inline uint32 RotateL(uint32 value, int shift) {
	return (value << shift) | (value >> (sizeof (uint32) * 8 - shift));
}
inline uint32 RotateR(uint32 value, int shift) {
	return (value >> shift) | (value << (sizeof (uint32) * 8 - shift));
}
inline byte GetByte(uint32 value, int pos) {
	return (byte) (value >> (pos * 8));
}
inline uint32 EndianChange(uint32 value) {
	return (RotateR(value, 8) & 0xFF00FF00)
	  | (RotateL(value, 8) & 0x00FF00FF);	
}

#if defined(_WIN32) || defined(_WIN64)
#define strcpy_secure(dst,size,src) strcpy_s(dst, size, src)
#define strlen_secure(str, max) strnlen_s(str, max)
#define vsprintf_secure(dst, size, format, ...) vsprintf_s(dst, size, format, __VA_ARGS__)
#define sprintf_secure(dst, size, format, ...) sprintf_s(dst, size, format, __VA_ARGS__)
#define fopen_secure(file, filename, mode) fopen_s(&file, filename, mode)
#else

#endif

#endif //_RISKEMULIBRARY_UTILS_H_