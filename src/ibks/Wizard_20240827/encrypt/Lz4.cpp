// Lz4.cpp: implementation of the CLz4 class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Lz4.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLz4::CLz4()
{
}

CLz4::~CLz4()
{
}

int CLz4::compress(char* srcbuf, int srclen, char* cmpbuf, int cmpsize)
{
	return _axcompress(srcbuf, srclen, cmpbuf, cmpsize);
}

int CLz4::decompress(char* srcbuf, int srclen, char* dmpbuf, int dmpsize)
{
	return _axdecompress(srcbuf, srclen, dmpbuf, dmpsize);
}

int CLz4::_axcompress(char *ibuf, unsigned int ilen, char *obuf, unsigned int osiz)
{
	char	*in_buff, *out_buff;
	char	*head_buff;
	unsigned int checkbits;
	unsigned int	compresslen = 0, srclen = 0, readlen = 0;
	unsigned int	blocksize, outsize;
	size_t	header_size;
	int	blockChecksum = 0;
	int	streamChecksum = 1;
	int	blockIndependence = 1;
	void	*streamChecksumState = NULL;
	int	blockSizeId = 4;
	
	blocksize = LZ4S_GetBlockSize_FromBlockId (blockSizeId);
	in_buff = (char *)malloc(blocksize);
	out_buff = (char *)malloc(blocksize+CACHELINE);
	head_buff = (char *)malloc(LZ4S_MAXHEADERSIZE);
	if (!in_buff || !out_buff || !head_buff)
	{
		if (in_buff != NULL)
			free(in_buff);
		if (out_buff != NULL)
			free(out_buff);
		if (head_buff != NULL)
			free(head_buff);
		return(-1);
	}
	
	if (streamChecksum)
		streamChecksumState = XXH32_init(LZ4S_CHECKSUM_SEED);
	
	*(unsigned int *)head_buff = LITTLE_ENDIAN_32(LZ4S_MAGICNUMBER);
	*(head_buff + 4)  = (1 & _2BITS) << 6;
	*(head_buff + 4) |= (blockIndependence & _1BIT) << 5;
	*(head_buff + 4) |= (blockChecksum & _1BIT) << 4;
	*(head_buff + 4) |= (streamChecksum & _1BIT) << 2;
	*(head_buff + 5)  = (char )((blockSizeId & _3BITS) << 4);
	checkbits = XXH32((head_buff+4), 2, LZ4S_CHECKSUM_SEED);
	checkbits = LZ4S_GetCheckBits_FromXXH(checkbits);
	*(head_buff + 6)  = (unsigned char )checkbits;
	header_size = 7;
	
	if (osiz < (compresslen + header_size))
	{
		free(in_buff);
		free(out_buff);
		free(head_buff);
		return (-1);
	}
	memcpy(&obuf[compresslen], head_buff, header_size);
	compresslen += header_size;
	
	readlen = blocksize<(ilen-srclen)?blocksize:(ilen-srclen);
	memcpy(in_buff, &ibuf[srclen], readlen);
	
	while (readlen > 0)
	{
		srclen += readlen;
		if (streamChecksum)
			XXH32_update(streamChecksumState, in_buff, readlen);
		outsize = LZ4_compress_limitedOutput(in_buff, out_buff+4, (int)readlen, (int)readlen-1);
		if (outsize > 0)
		{
			*(unsigned int *)out_buff = LITTLE_ENDIAN_32(outsize);
			if (blockChecksum)
			{
				unsigned int checksum = XXH32(out_buff+4, outsize, LZ4S_CHECKSUM_SEED);
				*(unsigned int *) (out_buff+4+outsize) = LITTLE_ENDIAN_32(checksum);
				outsize += (4*blockChecksum);
			}
			if (osiz < (compresslen + (outsize + 4)))
			{
				free(in_buff);
				free(out_buff);
				free(head_buff);
				return (-1);
			}
			memcpy(&obuf[compresslen], out_buff, (outsize + 4));
			compresslen += (outsize + 4);
		}
		else
		{
			if (osiz < (compresslen + 4))
			{
				free(in_buff);
				free(out_buff);
				free(head_buff);
				return (-1);
			}
			* (unsigned int *)out_buff = LITTLE_ENDIAN_32(((unsigned long)readlen)|0X80000000);
			memcpy(&obuf[compresslen], out_buff, 4);
			compresslen += 4;
			if (osiz < (compresslen + readlen))
			{
				free(in_buff);
				free(out_buff);
				free(head_buff);
				return (-1);
			}
			memcpy(&obuf[compresslen], in_buff, readlen);
			compresslen += readlen;
		}
		readlen = blocksize<(ilen-srclen)?blocksize:(ilen-srclen);
		memcpy(in_buff, &ibuf[srclen], readlen);
		
	}
	if (osiz < (compresslen + 4))
	{
		free(in_buff);
		free(out_buff);
		free(head_buff);
		return (-1);
	}
	* (unsigned int *) out_buff = LZ4S_EOS;
	memcpy(&obuf[compresslen], out_buff, 4);
	compresslen += 4;
	if (streamChecksum)
	{
		if (osiz < (compresslen + 4))
		{
			free(in_buff);
			free(out_buff);
			free(head_buff);
			return (-1);
		}
		unsigned int checksum = XXH32_digest(streamChecksumState);
		* (unsigned int *) out_buff = LITTLE_ENDIAN_32(checksum);
		memcpy(&obuf[compresslen], out_buff, 4);
		compresslen += 4;
	}
	free(in_buff);
	free(out_buff);
	free(head_buff);
	return (compresslen);
}

int CLz4::_axdecompress(char *ibuf, unsigned int ilen, char *obuf, unsigned int osiz)
{
	char	*in_buff, *out_buff, *out_start, *out_end;
	unsigned int	decomplen = 0, srclen = 0;
	unsigned int	maxblocksize = 0;
	int	decodebytes = 0;
	int	blockChecksum = 0;
	int	streamChecksum = 1;
	int	blockIndependence = 1;
	int	saveblockIndependence = 1;
	void	*streamChecksumState = NULL;
	unsigned int prefix64k = 0;
	
	if (ilen <= 7)
		return(-1);

	srclen = 4;
	{
	int version 	= (ibuf[4] >> 6) & _2BITS;
	int streamSize 	= (ibuf[4] >> 3) & _1BIT;
	int reserved1	= (ibuf[4] >> 1) & _1BIT;
	int dictionary	= (ibuf[4] >> 0) & _1BIT;
	
	int reserved2	= (ibuf[5] >> 7) & _1BIT;
	int blockSizeId = (ibuf[5] >> 4) & _3BITS;
	int reserved3	= (ibuf[5] >> 0) & _4BITS;
	int checkBits	= (ibuf[6] >> 0) & _8BITS;
	int checkBits_xxh32;
	
	blockIndependence = (ibuf[4] >> 5) & _1BIT;
	blockChecksum = (ibuf[4] >> 4) & _1BIT;
	streamChecksum = (ibuf[4] >> 2) & _1BIT;
	
	if (version != 1)
		return (-1);
	if (streamSize == 1)
		return (-1);
	if (reserved1 != 0)
		return (-1);
	if (dictionary == 1)
		return (-1);
	if (reserved2 != 0)
		return (-1);
	if (blockSizeId < 4)
		return (-1);
	if (reserved3 != 0)
		return (-1);
	
	maxblocksize = LZ4S_GetBlockSize_FromBlockId(blockSizeId);
	ibuf[5] &= 0xF0;
	checkBits_xxh32 = XXH32(&ibuf[4], 2, LZ4S_CHECKSUM_SEED);
	checkBits_xxh32 = LZ4S_GetCheckBits_FromXXH(checkBits_xxh32);
	if (checkBits != checkBits_xxh32)
		return(-1);
	}
	srclen = 7;
	
	if (!blockIndependence) { saveblockIndependence = blockIndependence; prefix64k = 64 KB; }

	{
	unsigned int outbuffSize = prefix64k + maxblocksize;
	in_buff = (char *)malloc(maxblocksize);
	if (outbuffSize < MIN_STREAM_BUFSIZE) outbuffSize = MIN_STREAM_BUFSIZE;
	out_buff = (char *)malloc(outbuffSize);
	out_end = out_buff + outbuffSize;
	out_start = out_buff + prefix64k;
	if (!in_buff || !out_buff)
		{
		if (in_buff != NULL)
			free(in_buff);
		if (out_buff != NULL)
			free(out_buff);
		return (-1);
		}
	}
	if (streamChecksum) streamChecksumState = XXH32_init(LZ4S_CHECKSUM_SEED);
	
	while (1)
		{
		unsigned int blocksize, uncompflg;
		
		if (ilen < (srclen + 4))
			{
			free(in_buff);
			free(out_buff);
			return (-1);
			}
		memcpy(&blocksize, &ibuf[srclen], 4);
		srclen += 4;
		if (blocksize == LZ4S_EOS)
			{
			break;
			}
		blocksize = LITTLE_ENDIAN_32(blocksize);
		uncompflg = blocksize >> 31;
		blocksize &= 0x7FFFFFFF;
		if (blocksize > maxblocksize)
			{
			free(in_buff);
			free(out_buff);
			return (-1);
			}
		if (ilen < (srclen + blocksize))
			{
			free(in_buff);
			free(out_buff);
			return (-1);
			}
		memcpy(in_buff, &ibuf[srclen], blocksize);
		srclen += blocksize;
		if (blockChecksum)
			{
			unsigned int checksum = XXH32(in_buff, blocksize, LZ4S_CHECKSUM_SEED);
			unsigned int readChecksum;
			if (ilen < (srclen + 4))
				{
				free(in_buff);
				free(out_buff);
				return(-1);
				}
			memcpy(&readChecksum, &ibuf[srclen], 4);
			srclen += 4;
			if (readChecksum != checksum)
				{
				free(in_buff);
				free(out_buff);
				return(-1);
				}
			
			}
		if (uncompflg)
			{
			if (osiz < (decomplen + blocksize))
				{
				free(in_buff);
				free(out_buff);
				return (-1);
				}
			memcpy(&obuf[decomplen], in_buff, blocksize);
			decomplen += blocksize;
			if (streamChecksum)
				XXH32_update(streamChecksumState, in_buff, blocksize);
			if (!blockIndependence)
				{
				if (blocksize >= prefix64k)
					{
					memcpy(out_buff, in_buff + (blocksize - prefix64k), prefix64k);
					out_start = out_buff + prefix64k;
					continue;
					}
				else
					{
					memcpy(out_start, in_buff, blocksize);
					decodebytes = blocksize;
					
					}
				}
			}
		else
			{
			if (saveblockIndependence)
				decodebytes = LZ4_decompress_safe(in_buff, out_start, blocksize, maxblocksize);
			else
				decodebytes = LZ4_decompress_safe_withPrefix64k(in_buff, out_start, blocksize, maxblocksize);

			if (decodebytes < 0)
				{
				free(in_buff);
				free(out_buff);
				return (-1);
				}
			if (streamChecksum)
				XXH32_update(streamChecksumState, out_start, decodebytes);
			if (osiz < (decomplen + decodebytes))
				{
				free(in_buff);
				free(out_buff);
				return (-1);
				}
			memcpy(&obuf[decomplen], out_start, decodebytes);
			decomplen += decodebytes;
			}
		
		if (!blockIndependence)
			{
			out_start += decodebytes;
			if ((size_t)(out_end - out_start) < (size_t)maxblocksize)
				{
				memcpy(out_buff, out_start - prefix64k, prefix64k);
				out_start = out_buff + prefix64k;
				}
			}
		}
	
	if (streamChecksum)
		{
		if (ilen < (srclen + 4))
			{
			free(in_buff);
			free(out_buff);
			return (-1);
			}
		unsigned int checksum = XXH32_digest(streamChecksumState);
		unsigned int readChecksum;
		memcpy(&readChecksum, &ibuf[srclen], 4);
		readChecksum = LITTLE_ENDIAN_32(readChecksum);
		if (checksum != readChecksum)
			{
			free(in_buff);
			free(out_buff);
			return(-1);
			}
		}
	free(in_buff);
	free(out_buff);
	if (decomplen == 0)
		return (-1);
	else
		return (decomplen);
}

//****************************
// Compression functions
//****************************
FORCE_INLINE int LZ4_hashSequence(U32 sequence, tableType_t tableType)
{
	if (tableType == byU16)
		return (((sequence) * 2654435761U) >> ((MINMATCH*8)-(LZ4_HASHLOG+1)));
	else
		return (((sequence) * 2654435761U) >> ((MINMATCH*8)-LZ4_HASHLOG));
}

FORCE_INLINE int LZ4_hashPosition(const BYTE* p, tableType_t tableType)
{
	return LZ4_hashSequence(A32(p), tableType);
}

FORCE_INLINE void LZ4_putPositionOnHash(const BYTE* p, U32 h, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
	switch (tableType)
	{
		case byPtr: { const BYTE** hashTable = (const BYTE**) tableBase; hashTable[h] = p; break; }
		case byU32: { U32* hashTable = (U32*) tableBase; hashTable[h] = (U32)(p-srcBase); break; }
		case byU16: { U16* hashTable = (U16*) tableBase; hashTable[h] = (U16)(p-srcBase); break; }
	}
}

FORCE_INLINE void LZ4_putPosition(const BYTE* p, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
	U32 h = LZ4_hashPosition(p, tableType);
	LZ4_putPositionOnHash(p, h, tableBase, tableType, srcBase);
}

FORCE_INLINE const BYTE* LZ4_getPositionOnHash(U32 h, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
	if (tableType == byPtr) { const BYTE** hashTable = (const BYTE**) tableBase; return hashTable[h]; }
	if (tableType == byU32) { U32* hashTable = (U32*) tableBase; return hashTable[h] + srcBase; }
	{ U16* hashTable = (U16*) tableBase; return hashTable[h] + srcBase; }   // default, to ensure a return
}

FORCE_INLINE const BYTE* LZ4_getPosition(const BYTE* p, void* tableBase, tableType_t tableType, const BYTE* srcBase)
{
	U32 h = LZ4_hashPosition(p, tableType);
	return LZ4_getPositionOnHash(h, tableBase, tableType, srcBase);
}


FORCE_INLINE int LZ4_compress_generic(
				      void* ctx,
				      const char* source,
				      char* dest,
				      int inputSize,
				      int maxOutputSize,
				      
				      limitedOutput_directive limitedOutput,
				      tableType_t tableType,
				      prefix64k_directive prefix)
{
	const BYTE* ip = (const BYTE*) source;
	const BYTE* const base = (prefix==withPrefix) ? ((LZ4_Data_Structure*)ctx)->base : (const BYTE*) source;
	const BYTE* const lowLimit = ((prefix==withPrefix) ? ((LZ4_Data_Structure*)ctx)->bufferStart : (const BYTE*)source);
	const BYTE* anchor = (const BYTE*) source;
	const BYTE* const iend = ip + inputSize;
	const BYTE* const mflimit = iend - MFLIMIT;
	const BYTE* const matchlimit = iend - LASTLITERALS;
	
	BYTE* op = (BYTE*) dest;
	BYTE* const oend = op + maxOutputSize;
	
	int length;
	const int skipStrength = SKIPSTRENGTH;
	U32 forwardH;
	
	// Init conditions
	if ((U32)inputSize > (U32)LZ4_MAX_INPUT_SIZE) return 0;                                // Unsupported input size, too large (or negative)
	if ((prefix==withPrefix) && (ip != ((LZ4_Data_Structure*)ctx)->nextBlock)) return 0;   // must continue from end of previous block
	if (prefix==withPrefix) ((LZ4_Data_Structure*)ctx)->nextBlock=iend;                    // do it now, due to potential early exit
	if ((tableType == byU16) && (inputSize>=LZ4_64KLIMIT)) return 0;                       // Size too large (not within 64K limit)
	if (inputSize<LZ4_minLength) goto _last_literals;                                      // Input too small, no compression (all literals)
	
	// First Byte
	LZ4_putPosition(ip, ctx, tableType, base);
	ip++; forwardH = LZ4_hashPosition(ip, tableType);
	
	// Main Loop
	for ( ; ; )
		{
		int findMatchAttempts = (1U << skipStrength) + 3;
		const BYTE* forwardIp = ip;
		const BYTE* ref;
		BYTE* token;
		
		// Find a match
		do {
			U32 h = forwardH;
			int step = findMatchAttempts++ >> skipStrength;
			ip = forwardIp;
			forwardIp = ip + step;
			
			if unlikely(forwardIp > mflimit) { goto _last_literals; }
			
			forwardH = LZ4_hashPosition(forwardIp, tableType);
			ref = LZ4_getPositionOnHash(h, ctx, tableType, base);
			LZ4_putPositionOnHash(ip, h, ctx, tableType, base);
			
		} while ((ref + MAX_DISTANCE < ip) || (A32(ref) != A32(ip)));
		
		// Catch up
		while ((ip>anchor) && (ref > lowLimit) && unlikely(ip[-1]==ref[-1])) { ip--; ref--; }
		
		// Encode Literal length
		length = (int)(ip - anchor);
		token = op++;
		if ((limitedOutput) && unlikely(op + length + (2 + 1 + LASTLITERALS) + (length/255) > oend)) return 0;   // Check output limit
		if (length>=(int)RUN_MASK)
			{
				int len = length-RUN_MASK;
				*token=(RUN_MASK<<ML_BITS);
				for(; len >= 255 ; len-=255) *op++ = 255;
				*op++ = (BYTE)len;
			}
		else *token = (BYTE)(length<<ML_BITS);
		
		// Copy Literals
		{ BYTE* end=(op)+(length); LZ4_WILDCOPY(op,anchor,end); op=end; }
		
		_next_match:
		// Encode Offset
		LZ4_WRITE_LITTLEENDIAN_16(op,(U16)(ip-ref));
		
		// Start Counting
		ip+=MINMATCH; ref+=MINMATCH;    // MinMatch already verified
		anchor = ip;
		while likely(ip<matchlimit-(STEPSIZE-1))
			{
			size_t diff = AARCH(ref) ^ AARCH(ip);
			if (!diff) { ip+=STEPSIZE; ref+=STEPSIZE; continue; }
			ip += LZ4_NbCommonBytes(diff);
			goto _endCount;
			}
		if (LZ4_ARCH64) if ((ip<(matchlimit-3)) && (A32(ref) == A32(ip))) { ip+=4; ref+=4; }
		if ((ip<(matchlimit-1)) && (A16(ref) == A16(ip))) { ip+=2; ref+=2; }
		if ((ip<matchlimit) && (*ref == *ip)) ip++;
		_endCount:
		
		// Encode MatchLength
		length = (int)(ip - anchor);
		if ((limitedOutput) && unlikely(op + (1 + LASTLITERALS) + (length>>8) > oend)) return 0;    // Check output limit
		if (length>=(int)ML_MASK)
			{
				*token += ML_MASK;
				length -= ML_MASK;
				for (; length > 509 ; length-=510) { *op++ = 255; *op++ = 255; }
				if (length >= 255) { length-=255; *op++ = 255; }
				*op++ = (BYTE)length;
			}
		else *token += (BYTE)(length);
		
		// Test end of chunk
		if (ip > mflimit) { anchor = ip;  break; }
		
		// Fill table
		LZ4_putPosition(ip-2, ctx, tableType, base);
		
		// Test next position
		ref = LZ4_getPosition(ip, ctx, tableType, base);
		LZ4_putPosition(ip, ctx, tableType, base);
		if ((ref + MAX_DISTANCE >= ip) && (A32(ref) == A32(ip))) { token = op++; *token=0; goto _next_match; }
		
		// Prepare next loop
		anchor = ip++;
		forwardH = LZ4_hashPosition(ip, tableType);
		}
	
_last_literals:
	// Encode Last Literals
	{
        int lastRun = (int)(iend - anchor);
        if ((limitedOutput) && (((char*)op - dest) + lastRun + 1 + ((lastRun+255-RUN_MASK)/255) > (U32)maxOutputSize)) return 0;   // Check output limit
        if (lastRun>=(int)RUN_MASK) { *op++=(RUN_MASK<<ML_BITS); lastRun-=RUN_MASK; for(; lastRun >= 255 ; lastRun-=255) *op++ = 255; *op++ = (BYTE) lastRun; }
        else *op++ = (BYTE)(lastRun<<ML_BITS);
        memcpy(op, anchor, iend - anchor);
        op += iend-anchor;
	}
	
	// End
	return (int) (((char*)op)-dest);
}


int CLz4::LZ4_compress(const char* source, char* dest, int inputSize)
{
#if (HEAPMODE)
	void* ctx = ALLOCATOR(HASHNBCELLS4, 4);   // Aligned on 4-bytes boundaries
#else
	U32 ctx[1U<<(MEMORY_USAGE-2)] = {0};           // Ensure data is aligned on 4-bytes boundaries
#endif
	int result;
	
	if (inputSize < (int)LZ4_64KLIMIT)
		result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, 0, notLimited, byU16, noPrefix);
	else
		result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, 0, notLimited, (sizeof(void*)==8) ? byU32 : byPtr, noPrefix);
	
#if (HEAPMODE)
	FREEMEM(ctx);
#endif
	return result;
}

int CLz4::LZ4_compress_continue (void* LZ4_Data, const char* source, char* dest, int inputSize)
{
	return LZ4_compress_generic(LZ4_Data, source, dest, inputSize, 0, notLimited, byU32, withPrefix);
}


int CLz4::LZ4_compress_limitedOutput(const char* source, char* dest, int inputSize, int maxOutputSize)
{
#if (HEAPMODE)
	void* ctx = ALLOCATOR(HASHNBCELLS4, 4);   // Aligned on 4-bytes boundaries
#else
	U32 ctx[1U<<(MEMORY_USAGE-2)] = {0};           // Ensure data is aligned on 4-bytes boundaries
#endif
	int result;
	
	if (inputSize < (int)LZ4_64KLIMIT)
		result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, maxOutputSize, limited, byU16, noPrefix);
	else
		result = LZ4_compress_generic((void*)ctx, source, dest, inputSize, maxOutputSize, limited, (sizeof(void*)==8) ? byU32 : byPtr, noPrefix);
	
#if (HEAPMODE)
	FREEMEM(ctx);
#endif
	return result;
}

int CLz4::LZ4_compress_limitedOutput_continue (void* LZ4_Data, const char* source, char* dest, int inputSize, int maxOutputSize)
{
	return LZ4_compress_generic(LZ4_Data, source, dest, inputSize, maxOutputSize, limited, byU32, withPrefix);
}


//****************************
// Stream functions
//****************************

FORCE_INLINE void LZ4_init(LZ4_Data_Structure* lz4ds, const BYTE* base)
{
	MEM_INIT(lz4ds->hashTable, 0, sizeof(lz4ds->hashTable));
	lz4ds->bufferStart = base;
	lz4ds->base = base;
	lz4ds->nextBlock = base;
}


void* CLz4::LZ4_create (const char* inputBuffer)
{
	void* lz4ds = ALLOCATOR(1, sizeof(LZ4_Data_Structure));
	LZ4_init ((LZ4_Data_Structure*)lz4ds, (const BYTE*)inputBuffer);
	return lz4ds;
}


int CLz4::LZ4_free (void* LZ4_Data)
{
	FREEMEM(LZ4_Data);
	return (0);
}


char* CLz4::LZ4_slideInputBuffer (void* LZ4_Data)
{
	LZ4_Data_Structure* lz4ds = (LZ4_Data_Structure*)LZ4_Data;
	size_t delta = lz4ds->nextBlock - (lz4ds->bufferStart + 64 KB);
	
	if ( (lz4ds->base - delta > lz4ds->base)                          // underflow control
	    || ((size_t)(lz4ds->nextBlock - lz4ds->base) > 0xE0000000) )   // close to 32-bits limit
		{
		size_t deltaLimit = (lz4ds->nextBlock - 64 KB) - lz4ds->base;
		int nH;
		
		for (nH=0; nH < HASHNBCELLS4; nH++)
			{
			if ((size_t)(lz4ds->hashTable[nH]) < deltaLimit) lz4ds->hashTable[nH] = 0;
			else lz4ds->hashTable[nH] -= (U32)deltaLimit;
			}
		memcpy((void*)(lz4ds->bufferStart), (const void*)(lz4ds->nextBlock - 64 KB), 64 KB);
		lz4ds->base = lz4ds->bufferStart;
		lz4ds->nextBlock = lz4ds->base + 64 KB;
		}
	else
		{
		memcpy((void*)(lz4ds->bufferStart), (const void*)(lz4ds->nextBlock - 64 KB), 64 KB);
		lz4ds->nextBlock -= delta;
		lz4ds->base -= delta;
		}
	
	return (char*)(lz4ds->nextBlock);
}


//****************************
// Decompression functions
//****************************

// This generic decompression function cover all use cases.
// It shall be instanciated several times, using different sets of directives
// Note that it is essential this generic function is really inlined,
// in order to remove useless branches during compilation optimisation.
FORCE_INLINE int LZ4_decompress_generic(
					const char* source,
					char* dest,
					int inputSize,          //
					int outputSize,         // If endOnInput==endOnInputSize, this value is the max size of Output Buffer.
					
					int endOnInput,         // endOnOutputSize, endOnInputSize
					int prefix64k,          // noPrefix, withPrefix
					int partialDecoding,    // full, partial
					int targetOutputSize    // only used if partialDecoding==partial
					)
{
	// Local Variables
	const BYTE* restrict ip = (const BYTE*) source;
	const BYTE* ref;
	const BYTE* const iend = ip + inputSize;
	
	BYTE* op = (BYTE*) dest;
	BYTE* const oend = op + outputSize;
	BYTE* cpy;
	BYTE* oexit = op + targetOutputSize;
	
	const size_t dec32table[] = {0, 3, 2, 3, 0, 0, 0, 0};   // static reduces speed for LZ4_decompress_safe() on GCC64
	static const size_t dec64table[] = {0, 0, 0, (size_t)-1, 0, 1, 2, 3};
	
	
	// Special cases
	if ((partialDecoding) && (oexit> oend-MFLIMIT)) oexit = oend-MFLIMIT;                        // targetOutputSize too high => decode everything
	if ((endOnInput) && unlikely(outputSize==0)) return ((inputSize==1) && (*ip==0)) ? 0 : -1;   // Empty output buffer
	if ((!endOnInput) && unlikely(outputSize==0)) return (*ip==0?1:-1);
	
	
	// Main Loop
	while (1)
		{
		unsigned token;
		size_t length;
		
		// get runlength
		token = *ip++;
		if ((length=(token>>ML_BITS)) == RUN_MASK)
			{
				unsigned s=255;
				while (((endOnInput)?ip<iend:1) && (s==255))
					{
						s = *ip++;
						length += s;
					}
			}
		
		// copy literals
		cpy = op+length;
		if (((endOnInput) && ((cpy>(partialDecoding?oexit:oend-MFLIMIT)) || (ip+length>iend-(2+1+LASTLITERALS))) )
		    || ((!endOnInput) && (cpy>oend-COPYLENGTH)))
			{
			if (partialDecoding)
				{
				if (cpy > oend) goto _output_error;                           // Error : write attempt beyond end of output buffer
				if ((endOnInput) && (ip+length > iend)) goto _output_error;   // Error : read attempt beyond end of input buffer
				}
			else
				{
				if ((!endOnInput) && (cpy != oend)) goto _output_error;       // Error : block decoding must stop exactly there
				if ((endOnInput) && ((ip+length != iend) || (cpy > oend))) goto _output_error;   // Error : input must be consumed
				}
			memcpy(op, ip, length);
			ip += length;
			op += length;
			break;                                       // Necessarily EOF, due to parsing restrictions
			}
		LZ4_WILDCOPY(op, ip, cpy); ip -= (op-cpy); op = cpy;
		
		// get offset
		LZ4_READ_LITTLEENDIAN_16(ref,cpy,ip); ip+=2;
		if ((prefix64k==noPrefix) && unlikely(ref < (BYTE* const)dest)) goto _output_error;   // Error : offset outside destination buffer
		
		// get matchlength
		if ((length=(token&ML_MASK)) == ML_MASK)
			{
				while ((!endOnInput) || (ip<iend-(LASTLITERALS+1)))   // Ensure enough bytes remain for LASTLITERALS + token
					{
					unsigned s = *ip++;
					length += s;
					if (s==255) continue;
					break;
					}
			}
		
		// copy repeated sequence
		if unlikely((op-ref)<(int)STEPSIZE)
			{
			const size_t dec64 = dec64table[(sizeof(void*)==4) ? 0 : op-ref];
			op[0] = ref[0];
			op[1] = ref[1];
			op[2] = ref[2];
			op[3] = ref[3];
			op += 4, ref += 4; ref -= dec32table[op-ref];
			A32(op) = A32(ref);
			op += STEPSIZE-4; ref -= dec64;
			} else { LZ4_COPYSTEP(op,ref); }
		cpy = op + length - (STEPSIZE-4);
		
		if unlikely(cpy>oend-COPYLENGTH-(STEPSIZE-4))
			{
			if (cpy > oend-LASTLITERALS) goto _output_error;    // Error : last 5 bytes must be literals
			LZ4_SECURECOPY(op, ref, (oend-COPYLENGTH));
			while(op<cpy) *op++=*ref++;
			op=cpy;
			continue;
			}
		LZ4_WILDCOPY(op, ref, cpy);
		op=cpy;   // correction
		}
	
	// end of decoding
	if (endOnInput)
		return (int) (((char*)op)-dest);     // Nb of output bytes decoded
	else
		return (int) (((char*)ip)-source);   // Nb of input bytes read
	
	// Overflow error detected
_output_error:
	return (int) (-(((char*)ip)-source))-1;
}


int CLz4::LZ4_decompress_safe(const char* source, char* dest, int inputSize, int maxOutputSize)
{
	return LZ4_decompress_generic(source, dest, inputSize, maxOutputSize, endOnInputSize, noPrefix, full, 0);
}

int CLz4::LZ4_decompress_safe_withPrefix64k(const char* source, char* dest, int inputSize, int maxOutputSize)
{
	return LZ4_decompress_generic(source, dest, inputSize, maxOutputSize, endOnInputSize, withPrefix, full, 0);
}

int CLz4::LZ4_decompress_safe_partial(const char* source, char* dest, int inputSize, int targetOutputSize, int maxOutputSize)
{
	return LZ4_decompress_generic(source, dest, inputSize, maxOutputSize, endOnInputSize, noPrefix, partial, targetOutputSize);
}

int CLz4::LZ4_decompress_fast_withPrefix64k(const char* source, char* dest, int outputSize)
{
	return LZ4_decompress_generic(source, dest, 0, outputSize, endOnOutputSize, withPrefix, full, 0);
}

int CLz4::LZ4_decompress_fast(const char* source, char* dest, int outputSize)
{
#ifdef _MSC_VER   // This version is faster with Visual
	return LZ4_decompress_generic(source, dest, 0, outputSize, endOnOutputSize, noPrefix, full, 0);
#else
	return LZ4_decompress_generic(source, dest, 0, outputSize, endOnOutputSize, withPrefix, full, 0);
#endif
}
	
// Modify the local functions below should you wish to use some other memory related routines
// for malloc(), free()
#include <stdlib.h>
forceinline void* XXH_malloc(size_t s) { return malloc(s); }
forceinline void  XXH_free  (void* p)  { free(p); }
// for memcpy()
#include <string.h>
forceinline void* XXH_memcpy(void* dest, const void* src, size_t size) { return memcpy(dest,src,size); }

forceinline U32 XXH_readLE32_align(const U32* ptr, XXH_endianess endian, XXH_alignment align)
{
	if (align==XXH_unaligned)
		return endian==XXH_littleEndian ? A32(ptr) : XXH_swap32(A32(ptr));
	else
		return endian==XXH_littleEndian ? *ptr : XXH_swap32(*ptr);
}

forceinline U32 XXH_readLE32(const U32* ptr, XXH_endianess endian) { return XXH_readLE32_align(ptr, endian, XXH_unaligned); }


//****************************
// Simple Hash Functions
//****************************
forceinline U32 XXH32_endian_align(const void* input, int len, U32 seed, XXH_endianess endian, XXH_alignment align)
{
	const BYTE* p = (const BYTE*)input;
	const BYTE* const bEnd = p + len;
	U32 h32;
	
#ifdef XXH_ACCEPT_NULL_INPUT_POINTER
	if (p==NULL) { len=0; p=(const BYTE*)(size_t)16; }
#endif
	
	if (len>=16)
		{
		const BYTE* const limit = bEnd - 16;
		U32 v1 = seed + PRIME32_1 + PRIME32_2;
		U32 v2 = seed + PRIME32_2;
		U32 v3 = seed + 0;
		U32 v4 = seed - PRIME32_1;
		
		do
			{
			v1 += XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v1 = XXH_rotl32(v1, 13); v1 *= PRIME32_1; p+=4;
			v2 += XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v2 = XXH_rotl32(v2, 13); v2 *= PRIME32_1; p+=4;
			v3 += XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v3 = XXH_rotl32(v3, 13); v3 *= PRIME32_1; p+=4;
			v4 += XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_2; v4 = XXH_rotl32(v4, 13); v4 *= PRIME32_1; p+=4;
			} while (p<=limit);
		
		h32 = XXH_rotl32(v1, 1) + XXH_rotl32(v2, 7) + XXH_rotl32(v3, 12) + XXH_rotl32(v4, 18);
		}
	else
		{
		h32  = seed + PRIME32_5;
		}
	
	h32 += (U32) len;
	
	while (p<=bEnd-4)
		{
		h32 += XXH_readLE32_align((const U32*)p, endian, align) * PRIME32_3;
		h32  = XXH_rotl32(h32, 17) * PRIME32_4 ;
		p+=4;
		}
	
	while (p<bEnd)
		{
		h32 += (*p) * PRIME32_5;
		h32 = XXH_rotl32(h32, 11) * PRIME32_1 ;
		p++;
		}
	
	h32 ^= h32 >> 15;
	h32 *= PRIME32_2;
	h32 ^= h32 >> 13;
	h32 *= PRIME32_3;
	h32 ^= h32 >> 16;
	
	return h32;
}


U32 CLz4::XXH32(const void* input, int len, U32 seed)
{
#if 0
	// Simple version, good for code maintenance, but unfortunately slow for small inputs
	void* state = XXH32_init(seed);
	XXH32_update(state, input, len);
	return XXH32_digest(state);
#else
	XXH_endianess endian_detected = (XXH_endianess)XXH_CPU_LITTLE_ENDIAN;
	
#  if !defined(XXH_USE_UNALIGNED_ACCESS)
	if ((((size_t)input) & 3))   // Input is aligned, let's leverage the speed advantage
		{
		if ((endian_detected==XXH_littleEndian) || XXH_FORCE_NATIVE_FORMAT)
			return XXH32_endian_align(input, len, seed, XXH_littleEndian, XXH_aligned);
		else
			return XXH32_endian_align(input, len, seed, XXH_bigEndian, XXH_aligned);
		}
#  endif
	
	if ((endian_detected==XXH_littleEndian) || XXH_FORCE_NATIVE_FORMAT)
		return XXH32_endian_align(input, len, seed, XXH_littleEndian, XXH_unaligned);
	else
		return XXH32_endian_align(input, len, seed, XXH_bigEndian, XXH_unaligned);
#endif
}


//****************************
// Advanced Hash Functions
//****************************

struct XXH_state32_t
{
	U64 total_len;
	U32 seed;
	U32 v1;
	U32 v2;
	U32 v3;
	U32 v4;
	int memsize;
	char memory[16];
};


int CLz4::XXH32_sizeofState()
{
	XXH_STATIC_ASSERT(XXH32_SIZEOFSTATE >= sizeof(struct XXH_state32_t));   // A compilation error here means XXH32_SIZEOFSTATE is not large enough
	return sizeof(struct XXH_state32_t);
}


XXH_errorcode CLz4::XXH32_resetState(void* state_in, U32 seed)
{
	struct XXH_state32_t * state = (struct XXH_state32_t *) state_in;
	state->seed = seed;
	state->v1 = seed + PRIME32_1 + PRIME32_2;
	state->v2 = seed + PRIME32_2;
	state->v3 = seed + 0;
	state->v4 = seed - PRIME32_1;
	state->total_len = 0;
	state->memsize = 0;
	return XXH_OK;
}


void* CLz4::XXH32_init (U32 seed)
{
	void* state = XXH_malloc (sizeof(struct XXH_state32_t));
	XXH32_resetState(state, seed);
	return state;
}


forceinline XXH_errorcode XXH32_update_endian (void* state_in, const void* input, int len, XXH_endianess endian)
{
	struct XXH_state32_t * state = (struct XXH_state32_t *) state_in;
	const BYTE* p = (const BYTE*)input;
	const BYTE* const bEnd = p + len;
	
#ifdef XXH_ACCEPT_NULL_INPUT_POINTER
	if (input==NULL) return XXH_ERROR;
#endif
	
	state->total_len += len;
	
	if (state->memsize + len < 16)   // fill in tmp buffer
		{
		XXH_memcpy(state->memory + state->memsize, input, len);
		state->memsize +=  len;
		return XXH_OK;
		}
	
	if (state->memsize)   // some data left from previous update
		{
		XXH_memcpy(state->memory + state->memsize, input, 16-state->memsize);
			{
			const U32* p32 = (const U32*)state->memory;
			state->v1 += XXH_readLE32(p32, endian) * PRIME32_2; state->v1 = XXH_rotl32(state->v1, 13); state->v1 *= PRIME32_1; p32++;
			state->v2 += XXH_readLE32(p32, endian) * PRIME32_2; state->v2 = XXH_rotl32(state->v2, 13); state->v2 *= PRIME32_1; p32++;
			state->v3 += XXH_readLE32(p32, endian) * PRIME32_2; state->v3 = XXH_rotl32(state->v3, 13); state->v3 *= PRIME32_1; p32++;
			state->v4 += XXH_readLE32(p32, endian) * PRIME32_2; state->v4 = XXH_rotl32(state->v4, 13); state->v4 *= PRIME32_1; p32++;
			}
		p += 16-state->memsize;
		state->memsize = 0;
		}
	
	if (p <= bEnd-16)
		{
		const BYTE* const limit = bEnd - 16;
		U32 v1 = state->v1;
		U32 v2 = state->v2;
		U32 v3 = state->v3;
		U32 v4 = state->v4;
		
		do
			{
			v1 += XXH_readLE32((const U32*)p, endian) * PRIME32_2; v1 = XXH_rotl32(v1, 13); v1 *= PRIME32_1; p+=4;
			v2 += XXH_readLE32((const U32*)p, endian) * PRIME32_2; v2 = XXH_rotl32(v2, 13); v2 *= PRIME32_1; p+=4;
			v3 += XXH_readLE32((const U32*)p, endian) * PRIME32_2; v3 = XXH_rotl32(v3, 13); v3 *= PRIME32_1; p+=4;
			v4 += XXH_readLE32((const U32*)p, endian) * PRIME32_2; v4 = XXH_rotl32(v4, 13); v4 *= PRIME32_1; p+=4;
			} while (p<=limit);
		
		state->v1 = v1;
		state->v2 = v2;
		state->v3 = v3;
		state->v4 = v4;
		}
	
	if (p < bEnd)
		{
		XXH_memcpy(state->memory, p, bEnd-p);
		state->memsize = (int)(bEnd-p);
		}
	
	return XXH_OK;
}

XXH_errorcode CLz4::XXH32_update (void* state_in, const void* input, int len)
{
	XXH_endianess endian_detected = (XXH_endianess)XXH_CPU_LITTLE_ENDIAN;
	
	if ((endian_detected==XXH_littleEndian) || XXH_FORCE_NATIVE_FORMAT)
		return XXH32_update_endian(state_in, input, len, XXH_littleEndian);
	else
		return XXH32_update_endian(state_in, input, len, XXH_bigEndian);
}



forceinline U32 XXH32_intermediateDigest_endian (void* state_in, XXH_endianess endian)
{
	struct XXH_state32_t * state = (struct XXH_state32_t *) state_in;
	const BYTE * p = (const BYTE*)state->memory;
	BYTE* bEnd = (BYTE*)state->memory + state->memsize;
	U32 h32;
	
	if (state->total_len >= 16)
		{
		h32 = XXH_rotl32(state->v1, 1) + XXH_rotl32(state->v2, 7) + XXH_rotl32(state->v3, 12) + XXH_rotl32(state->v4, 18);
		}
	else
		{
		h32  = state->seed + PRIME32_5;
		}
	
	h32 += (U32) state->total_len;
	
	while (p<=bEnd-4)
		{
		h32 += XXH_readLE32((const U32*)p, endian) * PRIME32_3;
		h32  = XXH_rotl32(h32, 17) * PRIME32_4;
		p+=4;
		}
	
	while (p<bEnd)
		{
		h32 += (*p) * PRIME32_5;
		h32 = XXH_rotl32(h32, 11) * PRIME32_1;
		p++;
		}
	
	h32 ^= h32 >> 15;
	h32 *= PRIME32_2;
	h32 ^= h32 >> 13;
	h32 *= PRIME32_3;
	h32 ^= h32 >> 16;
	
	return h32;
}


U32 CLz4::XXH32_intermediateDigest (void* state_in)
{
	XXH_endianess endian_detected = (XXH_endianess)XXH_CPU_LITTLE_ENDIAN;
	
	if ((endian_detected==XXH_littleEndian) || XXH_FORCE_NATIVE_FORMAT)
		return XXH32_intermediateDigest_endian(state_in, XXH_littleEndian);
	else
		return XXH32_intermediateDigest_endian(state_in, XXH_bigEndian);
}


U32 CLz4::XXH32_digest (void* state_in)
{
	U32 h32 = XXH32_intermediateDigest(state_in);
	
	XXH_free(state_in);
	
	return h32;
}