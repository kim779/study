#pragma once

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define _MSC_VER 1

#define KB *(1U<<10)
#define MB *(1U<<20)
#define GB *(1U<<30)

#define _1BIT  0x01
#define _2BITS 0x03
#define _3BITS 0x07
#define _4BITS 0x0F
#define _8BITS 0xFF

#define MAGICNUMBER_SIZE   4
#define LZ4S_MAGICNUMBER   0x184D2204
#define LZ4S_SKIPPABLE0    0x184D2A50
#define LZ4S_SKIPPABLEMASK 0xFFFFFFF0
#define LEGACY_MAGICNUMBER 0x184C2102

#define CACHELINE 64
#define LEGACY_BLOCKSIZE   (8 MB)
#define MIN_STREAM_BUFSIZE (1 MB + 64 KB)
#define LZ4S_BLOCKSIZEID_DEFAULT 7
#define LZ4S_CHECKSUM_SEED 0
#define LZ4S_EOS 0
#define LZ4S_MAXHEADERSIZE (MAGICNUMBER_SIZE+2+8+4+1)

static const int one = 1;
#define CPU_LITTLE_ENDIAN   (*(char*)(&one))
#define CPU_BIG_ENDIAN      (!CPU_LITTLE_ENDIAN)
#define LITTLE_ENDIAN_32(i) (CPU_LITTLE_ENDIAN?(i):swap32(i))


#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#if defined(_MSC_VER)    // Visual Studio
#  define swap32 _byteswap_ulong
#elif GCC_VERSION >= 403
#  define swap32 __builtin_bswap32
#else
static inline unsigned int swap32(unsigned int x)
{
	return ((x << 24) & 0xff000000 ) |
	((x <<  8) & 0x00ff0000 ) |
	((x >>  8) & 0x0000ff00 ) |
	((x >> 24) & 0x000000ff );
}
#endif

static int LZ4S_GetBlockSize_FromBlockId (int id) { return (1 << (8 + (2 * id))); }
static unsigned int LZ4S_GetCheckBits_FromXXH (unsigned int xxh) { return (xxh >> 8) & _8BITS; }


#if defined(_MSC_VER) && !defined(__cplusplus)   // Visual Studio
#  define inline __inline           // Visual C is not C99, but supports some kind of inline
#endif
	
//****************************
// Advanced Functions
//****************************
#define LZ4_MAX_INPUT_SIZE        0x7E000000   // 2 113 929 216 bytes
#define LZ4_COMPRESSBOUND(isize)  ((unsigned int)(isize) > (unsigned int)LZ4_MAX_INPUT_SIZE ? 0 : (isize) + ((isize)/255) + 16)
static inline int LZ4_compressBound(int isize)  { return LZ4_COMPRESSBOUND(isize); }
	
//**************************************
// Tuning parameters
//**************************************
// MEMORY_USAGE :
// Memory usage formula : N->2^N Bytes (examples : 10 -> 1KB; 12 -> 4KB ; 16 -> 64KB; 20 -> 1MB; etc.)
// Increasing memory usage improves compression ratio
// Reduced memory usage can improve speed, due to cache effect
// Default value is 14, for 16KB, which nicely fits into Intel x86 L1 cache
#define MEMORY_USAGE 14

// HEAPMODE :
// Select how default compression functions will allocate memory for their hash table,
// in memory stack (0:default, fastest), or in memory heap (1:requires memory allocation (malloc)).
#define HEAPMODE 0


//**************************************
// CPU Feature Detection
//**************************************
// 32 or 64 bits ?
#if (defined(__x86_64__) || defined(_M_X64) || defined(_WIN64) \
|| defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) \
|| defined(__64BIT__) || defined(_LP64) || defined(__LP64__) \
|| defined(__ia64) || defined(__itanium__) || defined(_M_IA64) )   // Detects 64 bits mode
#  define LZ4_ARCH64 1
#else
#  define LZ4_ARCH64 0
#endif

// Little Endian or Big Endian ?
// Overwrite the #define below if you know your architecture endianess
#if defined (__GLIBC__)
#  include <endian.h>
#  if (__BYTE_ORDER == __BIG_ENDIAN)
#     define LZ4_BIG_ENDIAN 1
#  endif
#elif (defined(__BIG_ENDIAN__) || defined(__BIG_ENDIAN) || defined(_BIG_ENDIAN)) && !(defined(__LITTLE_ENDIAN__) || defined(__LITTLE_ENDIAN) || defined(_LITTLE_ENDIAN))
#  define LZ4_BIG_ENDIAN 1
#elif defined(__sparc) || defined(__sparc__) \
|| defined(__powerpc__) || defined(__ppc__) || defined(__PPC__) \
|| defined(__hpux)  || defined(__hppa) \
|| defined(_MIPSEB) || defined(__s390__)
#  define LZ4_BIG_ENDIAN 1
#else
// Little Endian assumed. PDP Endian and other very rare endian format are unsupported.
#endif

// Unaligned memory access is automatically enabled for "common" CPU, such as x86.
// For others CPU, such as ARM, the compiler may be more cautious, inserting unnecessary extra code to ensure aligned access property
// If you know your target CPU supports unaligned memory access, you want to force this option manually to improve performance
#if defined(__ARM_FEATURE_UNALIGNED)
#  define LZ4_FORCE_UNALIGNED_ACCESS 1
#endif

// Define this parameter if your target system or compiler does not support hardware bit count
#if defined(_MSC_VER) && defined(_WIN32_WCE)            // Visual Studio for Windows CE does not support Hardware bit count
#  define LZ4_FORCE_SW_BITCOUNT
#endif

// BIG_ENDIAN_NATIVE_BUT_INCOMPATIBLE :
// This option may provide a small boost to performance for some big endian cpu, although probably modest.
// You may set this option to 1 if data will remain within closed environment.
// This option is useless on Little_Endian CPU (such as x86)
//#define BIG_ENDIAN_NATIVE_BUT_INCOMPATIBLE 1


//**************************************
// Compiler Options
//**************************************
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L)   // C99
/* "restrict" is a known keyword */
#else
#  define restrict // Disable restrict
#endif

#ifdef _MSC_VER    // Visual Studio
#  define FORCE_INLINE static __forceinline
#  include <intrin.h>                    // For Visual 2005
#  if LZ4_ARCH64   // 64-bits
#    pragma intrinsic(_BitScanForward64) // For Visual 2005
#    pragma intrinsic(_BitScanReverse64) // For Visual 2005
#  else            // 32-bits
#    pragma intrinsic(_BitScanForward)   // For Visual 2005
#    pragma intrinsic(_BitScanReverse)   // For Visual 2005
#  endif
#  pragma warning(disable : 4127)        // disable: C4127: conditional expression is constant
#else
#  ifdef __GNUC__
#    define FORCE_INLINE static inline __attribute__((always_inline))
#  else
#    define FORCE_INLINE static inline
#  endif
#endif

#ifdef _MSC_VER
#  define lz4_bswap16(x) _byteswap_ushort(x)
#else
#  define lz4_bswap16(x) ((unsigned short int) ((((x) >> 8) & 0xffu) | (((x) & 0xffu) << 8)))
#endif

#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#if (GCC_VERSION >= 302) || (__INTEL_COMPILER >= 800) || defined(__clang__)
#  define expect(expr,value)    (__builtin_expect ((expr),(value)) )
#else
#  define expect(expr,value)    (expr)
#endif

#define likely(expr)     expect((expr) != 0, 1)
#define unlikely(expr)   expect((expr) != 0, 0)


//**************************************
// Memory routines
//**************************************
#include <stdlib.h>   // malloc, calloc, free
#define ALLOCATOR(n,s) calloc(n,s)
#define FREEMEM        free
#include <string.h>   // memset, memcpy
#define MEM_INIT       memset

//**************************************
// Basic Types
//**************************************
#if defined (__STDC_VERSION__) && __STDC_VERSION__ >= 199901L   // C99
# include <stdint.h>
typedef  uint8_t BYTE;
typedef uint16_t U16;
typedef uint32_t U32;
typedef  int32_t S32;
typedef uint64_t U64;
#else
typedef unsigned char       BYTE;
typedef unsigned short      U16;
typedef unsigned int        U32;
typedef   signed int        S32;
typedef unsigned long long  U64;
#endif

#if defined(__GNUC__)  && !defined(LZ4_FORCE_UNALIGNED_ACCESS)
#  define _PACKED __attribute__ ((packed))
#else
#  define _PACKED
#endif

#if !defined(LZ4_FORCE_UNALIGNED_ACCESS) && !defined(__GNUC__)
#  if defined(__IBMC__) || defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#    pragma pack(1)
#  else
#    pragma pack(push, 1)
#  endif
#endif

typedef struct { U16 v; }  _PACKED U16_S;
typedef struct { U32 v; }  _PACKED U32_S;
typedef struct { U64 v; }  _PACKED U64_S;
typedef struct {size_t v;} _PACKED size_t_S;

#if !defined(LZ4_FORCE_UNALIGNED_ACCESS) && !defined(__GNUC__)
#  if defined(__SUNPRO_C) || defined(__SUNPRO_CC)
#    pragma pack(0)
#  else
#    pragma pack(pop)
#  endif
#endif

#define A16(x)   (((U16_S *)(x))->v)
#define A32(x)   (((U32_S *)(x))->v)
#define A64(x)   (((U64_S *)(x))->v)
#define AARCH(x) (((size_t_S *)(x))->v)


//**************************************
// Constants
//**************************************
#define LZ4_HASHLOG   (MEMORY_USAGE-2)
#define HASHTABLESIZE (1 << MEMORY_USAGE)
#define HASHNBCELLS4  (1 << LZ4_HASHLOG)

#define MINMATCH 4

#define COPYLENGTH 8
#define LASTLITERALS 5
#define MFLIMIT (COPYLENGTH+MINMATCH)
const int LZ4_minLength = (MFLIMIT+1);

#define LZ4_64KLIMIT ((1<<16) + (MFLIMIT-1))
#define SKIPSTRENGTH 6     // Increasing this value will make the compression run slower on incompressible data

#define MAXD_LOG 16
#define MAX_DISTANCE ((1 << MAXD_LOG) - 1)

#define ML_BITS  4
#define ML_MASK  ((1U<<ML_BITS)-1)
#define RUN_BITS (8-ML_BITS)
#define RUN_MASK ((1U<<RUN_BITS)-1)

#define KB *(1U<<10)
#define MB *(1U<<20)
#define GB *(1U<<30)


//**************************************
// Structures and local types
//**************************************

typedef struct {
	U32 hashTable[HASHNBCELLS4];
	const BYTE* bufferStart;
	const BYTE* base;
	const BYTE* nextBlock;
} LZ4_Data_Structure;

typedef enum { notLimited = 0, limited = 1 } limitedOutput_directive;
typedef enum { byPtr, byU32, byU16 } tableType_t;

typedef enum { noPrefix = 0, withPrefix = 1 } prefix64k_directive;

typedef enum { endOnOutputSize = 0, endOnInputSize = 1 } endCondition_directive;
typedef enum { full = 0, partial = 1 } earlyEnd_directive;


//**************************************
// Architecture-specific macros
//**************************************
#define STEPSIZE                  sizeof(size_t)
#define LZ4_COPYSTEP(d,s)         { AARCH(d) = AARCH(s); d+=STEPSIZE; s+=STEPSIZE; }
#define LZ4_COPY8(d,s)            { LZ4_COPYSTEP(d,s); if (STEPSIZE<8) LZ4_COPYSTEP(d,s); }
#define LZ4_SECURECOPY(d,s,e)     { if ((STEPSIZE==4)||(d<e)) LZ4_WILDCOPY(d,s,e); }

#if LZ4_ARCH64   // 64-bit
#  define HTYPE                   U32
#  define INITBASE(base)          const BYTE* const base = ip
#else            // 32-bit
#  define HTYPE                   const BYTE*
#  define INITBASE(base)          const int base = 0
#endif

#if (defined(LZ4_BIG_ENDIAN) && !defined(BIG_ENDIAN_NATIVE_BUT_INCOMPATIBLE))
#  define LZ4_READ_LITTLEENDIAN_16(d,s,p) { U16 v = A16(p); v = lz4_bswap16(v); d = (s) - v; }
#  define LZ4_WRITE_LITTLEENDIAN_16(p,i)  { U16 v = (U16)(i); v = lz4_bswap16(v); A16(p) = v; p+=2; }
#else      // Little Endian
#  define LZ4_READ_LITTLEENDIAN_16(d,s,p) { d = (s) - A16(p); }
#  define LZ4_WRITE_LITTLEENDIAN_16(p,v)  { A16(p) = v; p+=2; }
#endif


//**************************************
// Macros
//**************************************
#define LZ4_WILDCOPY(d,s,e)     { do { LZ4_COPY8(d,s) } while (d<e); }           // at the end, d>=e;


//****************************
// Private functions
//****************************
#if LZ4_ARCH64

FORCE_INLINE int LZ4_NbCommonBytes (register U64 val)
{
# if defined(LZ4_BIG_ENDIAN)
#   if defined(_MSC_VER) && !defined(LZ4_FORCE_SW_BITCOUNT)
	unsigned long r = 0;
	_BitScanReverse64( &r, val );
	return (int)(r>>3);
#   elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
	return (__builtin_clzll(val) >> 3);
#   else
	int r;
	if (!(val>>32)) { r=4; } else { r=0; val>>=32; }
	if (!(val>>16)) { r+=2; val>>=8; } else { val>>=24; }
	r += (!val);
	return r;
#   endif
# else
#   if defined(_MSC_VER) && !defined(LZ4_FORCE_SW_BITCOUNT)
	unsigned long r = 0;
	_BitScanForward64( &r, val );
	return (int)(r>>3);
#   elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
	return (__builtin_ctzll(val) >> 3);
#   else
	static const int DeBruijnBytePos[64] = { 0, 0, 0, 0, 0, 1, 1, 2, 0, 3, 1, 3, 1, 4, 2, 7, 0, 2, 3, 6, 1, 5, 3, 5, 1, 3, 4, 4, 2, 5, 6, 7, 7, 0, 1, 2, 3, 3, 4, 6, 2, 6, 5, 5, 3, 4, 5, 6, 7, 1, 2, 4, 6, 4, 4, 5, 7, 2, 6, 5, 7, 6, 7, 7 };
	return DeBruijnBytePos[((U64)((val & -(long long)val) * 0x0218A392CDABBD3FULL)) >> 58];
#   endif
# endif
}

#else

FORCE_INLINE int LZ4_NbCommonBytes (U32 val)
{
# if defined(LZ4_BIG_ENDIAN)
#   if defined(_MSC_VER) && !defined(LZ4_FORCE_SW_BITCOUNT)
	unsigned long r = 0;
	_BitScanReverse( &r, val );
	return (int)(r>>3);
#   elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
	return (__builtin_clz(val) >> 3);
#   else
	int r;
	if (!(val>>16)) { r=2; val>>=8; } else { r=0; val>>=24; }
	r += (!val);
	return r;
#   endif
# else
#   if defined(_MSC_VER) && !defined(LZ4_FORCE_SW_BITCOUNT)
	unsigned long r;
	_BitScanForward( &r, val );
	return (int)(r>>3);
#   elif defined(__GNUC__) && (GCC_VERSION >= 304) && !defined(LZ4_FORCE_SW_BITCOUNT)
	return (__builtin_ctz(val) >> 3);
#   else
	static const int DeBruijnBytePos[32] = { 0, 0, 3, 0, 3, 1, 3, 0, 3, 2, 2, 1, 3, 2, 0, 1, 3, 3, 1, 2, 2, 2, 2, 0, 3, 1, 2, 0, 1, 0, 1, 1 };
	return DeBruijnBytePos[((U32)((val & -(S32)val) * 0x077CB531U)) >> 27];
#   endif
# endif
}

#endif


//****************************
// Type
//****************************
typedef enum { XXH_OK=0, XXH_ERROR } XXH_errorcode;

#define       XXH32_SIZEOFSTATE 48
typedef struct { long long ll[(XXH32_SIZEOFSTATE+(sizeof(long long)-1))/sizeof(long long)]; } XXH32_stateSpace_t;


//****************************
// Deprecated function names
//****************************
// The following translations are provided to ease code transition
// You are encouraged to no longer this function names
#define XXH32_feed   XXH32_update
#define XXH32_result XXH32_digest
#define XXH32_getIntermediateResult XXH32_intermediateDigest


//**************************************
// Tuning parameters
//**************************************
// Unaligned memory access is automatically enabled for "common" CPU, such as x86.
// For others CPU, the compiler will be more cautious, and insert extra code to ensure aligned access is respected.
// If you know your target CPU supports unaligned memory access, you want to force this option manually to improve performance.
// You can also enable this parameter if you know your input data will always be aligned (boundaries of 4, for U32).
#if defined(__ARM_FEATURE_UNALIGNED) || defined(__i386) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64)
#  define XXH_USE_UNALIGNED_ACCESS 1
#endif

//**************************************
// Compiler Specific Options
//**************************************
// Disable some Visual warning messages
#ifdef _MSC_VER  // Visual Studio
#  pragma warning(disable : 4127)      // disable: C4127: conditional expression is constant
#endif

#ifdef _MSC_VER    // Visual Studio
#  define forceinline static __forceinline
#else
#  ifdef __GNUC__
#    define forceinline static inline __attribute__((always_inline))
#  else
#    define forceinline static inline
#  endif
#endif


//***************************************
// Compiler-specific Functions and Macros
//***************************************
#define GCC_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

// Note : although _rotl exists for minGW (GCC under windows), performance seems poor
#if defined(_MSC_VER)
#  define XXH_rotl32(x,r) _rotl(x,r)
#else
#  define XXH_rotl32(x,r) ((x << r) | (x >> (32 - r)))
#endif

#if defined(_MSC_VER)     // Visual Studio
#  define XXH_swap32 _byteswap_ulong
#elif GCC_VERSION >= 403
#  define XXH_swap32 __builtin_bswap32
#else
static inline U32 XXH_swap32 (U32 x) {
	return  ((x << 24) & 0xff000000 ) |
        ((x <<  8) & 0x00ff0000 ) |
        ((x >>  8) & 0x0000ff00 ) |
        ((x >> 24) & 0x000000ff );}
#endif

//**************************************
// Constants
//**************************************
#define PRIME32_1   2654435761U
#define PRIME32_2   2246822519U
#define PRIME32_3   3266489917U
#define PRIME32_4    668265263U
#define PRIME32_5    374761393U


//**************************************
// Architecture Macros
//**************************************
typedef enum { XXH_bigEndian=0, XXH_littleEndian=1 } XXH_endianess;
#ifndef XXH_CPU_LITTLE_ENDIAN   // It is possible to define XXH_CPU_LITTLE_ENDIAN externally, for example using a compiler switch
static const int bitone = 1;
#   define XXH_CPU_LITTLE_ENDIAN   (*(char*)(&bitone))
#endif

//**************************************
// Macros
//**************************************
#define XXH_STATIC_ASSERT(c)   { enum { XXH_static_assert = 1/(!!(c)) }; }    // use only *after* variable declarations

//****************************
// Memory reads
//****************************
typedef enum { XXH_aligned, XXH_unaligned } XXH_alignment;

#define XXH_FORCE_NATIVE_FORMAT 0
