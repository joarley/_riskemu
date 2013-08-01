#ifndef _RISKEMULIBRARY_UTILS_H_
#define _RISKEMULIBRARY_UTILS_H_

#include "stdtypes.h"

static inline uint32 RotateL(uint32 value, int shift) {
	return (value << shift) | (value >> (sizeof (uint32) * 8 - shift));
}
static inline uint32 RotateR(uint32 value, int shift) {
	return (value >> shift) | (value << (sizeof (uint32) * 8 - shift));
}
static inline byte GetByte(uint32 value, int pos) {
	return (byte) (value >> (pos * 8));
}
static inline uint32 EndianChange(uint32 value) {
	return (RotateR(value, 8) & 0xFF00FF00)
	  | (RotateL(value, 8) & 0x00FF00FF);
}

#endif //_RISKEMULIBRARY_UTILS_H_