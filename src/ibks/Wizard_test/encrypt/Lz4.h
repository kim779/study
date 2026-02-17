// Lz4.h: interface for the CLz4 class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Lz4define.h"

class CLz4
{
public:
	CLz4();
	virtual ~CLz4();

public:
	int	compress(char* srcbuf, int srclen, char* cmpbuf, int cmpsize);
	int	decompress(char* srcbuf, int srclen, char* dmpbuf, int dmpsize);

protected:
	int	_axcompress(char *ibuf, unsigned int ilen, char *obuf, unsigned int osiz);
	int	_axdecompress(char *ibuf, unsigned int ilen, char *obuf, unsigned int osiz);

private:
	int LZ4_compress(const char* source, char* dest, int inputSize);
	int LZ4_decompress_safe(const char* source, char* dest, int inputSize, int maxOutputSize);

	int LZ4_compress_limitedOutput(const char* source, char* dest, int inputSize, int maxOutputSize);
	int LZ4_decompress_fast(const char* source, char* dest, int outputSize);
	int LZ4_decompress_safe_partial(const char* source, char* dest, int inputSize, int targetOutputSize, int maxOutputSize);

	void* LZ4_create(const char* inputBuffer);
	int   LZ4_compress_continue(void* LZ4_Data, const char* source, char* dest, int inputSize);
	int   LZ4_compress_limitedOutput_continue(void* LZ4_Data, const char* source, char* dest, int inputSize, int maxOutputSize);
	char* LZ4_slideInputBuffer(void* LZ4_Data);
	int   LZ4_free(void* LZ4_Data);

	int LZ4_decompress_safe_withPrefix64k(const char* source, char* dest, int inputSize, int maxOutputSize);
	int LZ4_decompress_fast_withPrefix64k(const char* source, char* dest, int outputSize);

	unsigned int  XXH32(const void* input, int len, unsigned int seed);
	void*         XXH32_init(unsigned int seed);
	XXH_errorcode XXH32_update(void* state, const void* input, int len);
	unsigned int  XXH32_digest(void* state);
	int           XXH32_sizeofState();
	XXH_errorcode XXH32_resetState(void* state, unsigned int seed);
	unsigned int XXH32_intermediateDigest(void* state);
};

