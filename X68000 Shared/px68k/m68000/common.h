#ifndef winx68k_common_h
#define winx68k_common_h

#include "windows.h"
#include <stdio.h>
#include <string.h>

#ifdef __APPLE__
#include "TargetConditionals.h"
#endif

#define	TRUE		1
#define	FALSE		0
#define	SUCCESS		0
#define	FAILURE		1

#define FASTCALL
#define STDCALL
#define	LABEL
#define	__stdcall

typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int	UINT32;
typedef signed char	INT8;
typedef signed short	INT16;
typedef signed int	INT32;

typedef union {
	struct {
		BYTE l;
		BYTE h;
	} b;
	WORD w;
} PAIR;

#ifdef __cplusplus
extern "C" {
#endif

void Error(const char* s);
void p6logd(const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif //winx68k_common_h
