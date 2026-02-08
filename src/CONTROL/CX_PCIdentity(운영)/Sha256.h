/* Sha256.h -- SHA-256 Hash
2010-06-11 : Igor Pavlov : Public domain */
#ifndef __CRYPTO_SHA256_H
#define __CRYPTO_SHA256_H

#ifndef EXTERN_C_BEGIN
#ifdef __cplusplus
#define EXTERN_C_BEGIN extern "C" {
#define EXTERN_C_END }
#else
#define EXTERN_C_BEGIN
#define EXTERN_C_END
#endif
#endif

#include <stdlib.h>
#include <stdio.h>

typedef unsigned char Byte;
typedef short Int16;
typedef unsigned short UInt16;

typedef int Int32;
typedef unsigned int UInt32;

#if defined(_MSC_VER) || defined(__BORLANDC__)
	typedef __int64 Int64;
	typedef unsigned __int64 UInt64;
	#define UINT64_CONST(n) n
#else
	typedef long long int Int64;
	typedef unsigned long long int UInt64;
	#define UINT64_CONST(n) n ## ULL
#endif

EXTERN_C_BEGIN

typedef struct
{
	UInt32	state[8];
	UInt64	count;
	Byte	buffer[64];
} CSha256;

void	Sha256_Init(CSha256 *p);
void	Sha256_Update(CSha256 *p, const Byte *data, size_t size);
void	Sha256_Final(CSha256 *p, Byte *digest);
int	base64_encode(char *text, int numBytes, char **encodedText);

EXTERN_C_END

#endif
