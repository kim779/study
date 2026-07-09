#include "stdafx.h"
#include "Crypt.h"
#include "seed_kisa.tab"

#define	 BLOCK_SIZE	16

BYTE	pbUserKey[16] = { 0x60, 0x23, 0x7F, 0xAE, 0x4B, 0x8A, 0x43, 0x8C,
		       0xD9, 0x78, 0x3E, 0x84, 0x23, 0x4D, 0xF3, 0x4D };

static char __base64_table[] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
	'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/', '\0'
};

static char __base64_pad = '=';


//
// Macros for Encryption and Decryption
//
#define GetB0(A)  ((BYTE)((A)    ))
#define GetB1(A)  ((BYTE)((A)>> 8))
#define GetB2(A)  ((BYTE)((A)>>16))
#define GetB3(A)  ((BYTE)((A)>>24))


//
// Round function F and adding output of F to L.
// L0, L1 : left input values at each round
// R0, R1 : right input values at each round
// K : round keys at each round
//
#define SeedRound(L0, L1, R0, R1, K) {             \
    T0 = R0 ^ (K)[0];                              \
    T1 = R1 ^ (K)[1];                              \
    T1 ^= T0;                                      \
    T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^         \
         SS2[GetB2(T1)] ^ SS3[GetB3(T1)];          \
    T0 += T1;                                      \
    T0 = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^         \
         SS2[GetB2(T0)] ^ SS3[GetB3(T0)];          \
    T1 += T0;                                      \
    T1 = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^         \
         SS2[GetB2(T1)] ^ SS3[GetB3(T1)];          \
    T0 += T1;                                      \
    L0 ^= T0; L1 ^= T1;                            \
}

//
// Constants for Key schedule
// KC0 = golden ratio; KCi = ROTL(KCi-1, 1) 
//
#define KC0     0x9e3779b9UL
#define KC1     0x3c6ef373UL
#define KC2     0x78dde6e6UL
#define KC3     0xf1bbcdccUL
#define KC4     0xe3779b99UL
#define KC5     0xc6ef3733UL
#define KC6     0x8dde6e67UL
#define KC7     0x1bbcdccfUL
#define KC8     0x3779b99eUL
#define KC9     0x6ef3733cUL
#define KC10    0xdde6e678UL
#define KC11    0xbbcdccf1UL
#define KC12    0x779b99e3UL
#define KC13    0xef3733c6UL
#define KC14    0xde6e678dUL
#define KC15    0xbcdccf1bUL

//
// Macros for Key schedule
//
#define RoundKeyUpdate0(K, A, B, C, D, KC) {		\
	T0 = A + C - KC;				\
	T1 = B + KC - D;				\
	(K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^	\
	     SS2[GetB2(T0)] ^ SS3[GetB3(T0)];		\
	(K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^	\
	     SS2[GetB2(T1)] ^ SS3[GetB3(T1)];		\
	T0 = A;						\
	A = (A>>8) ^ (B<<24);				\
	B = (B>>8) ^ (T0<<24);				\
}

#define RoundKeyUpdate1(K, A, B, C, D, KC) {		\
	T0 = A + C - KC;				\
	T1 = B + KC - D;				\
	(K)[0] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^	\
	     SS2[GetB2(T0)] ^ SS3[GetB3(T0)];		\
	(K)[1] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^	\
	     SS2[GetB2(T1)] ^ SS3[GetB3(T1)];		\
	T0 = C;						\
	C = (C<<8) ^ (D>>24);				\
	D = (D<<8) ^ (T0>>24);				\
}

void SeedEncrypt(		/* encryption function */
		BYTE *pbData, 				// [in,out]	data to be encrypted
		DWORD *pdwRoundKey			// [in]			round keys for encryption
		)
{
	DWORD L0, L1, R0, R1;		// Iuput/output values at each rounds
	DWORD T0, T1;				// Temporary variables for round function F
	DWORD *K = pdwRoundKey;		// Pointer of round keys

// Set up input values for first round
    L0 = ((DWORD *)pbData)[0];
    L1 = ((DWORD *)pbData)[1];
	R0 = ((DWORD *)pbData)[2];
    R1 = ((DWORD *)pbData)[3];
   
// Reorder for big endian 
// Because SEED use little endian order in default
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

    SeedRound(L0, L1, R0, R1, K   ); 	// Round 1
    SeedRound(R0, R1, L0, L1, K+ 2); 	// Round 2
    SeedRound(L0, L1, R0, R1, K+ 4); 	// Round 3
    SeedRound(R0, R1, L0, L1, K+ 6); 	// Round 4
    SeedRound(L0, L1, R0, R1, K+ 8); 	// Round 5
    SeedRound(R0, R1, L0, L1, K+10); 	// Round 6
    SeedRound(L0, L1, R0, R1, K+12); 	// Round 7
    SeedRound(R0, R1, L0, L1, K+14); 	// Round 8
    SeedRound(L0, L1, R0, R1, K+16); 	// Round 9
    SeedRound(R0, R1, L0, L1, K+18); 	// Round 10
    SeedRound(L0, L1, R0, R1, K+20); 	// Round 11
    SeedRound(R0, R1, L0, L1, K+22); 	// Round 12
    SeedRound(L0, L1, R0, R1, K+24); 	// Round 13
    SeedRound(R0, R1, L0, L1, K+26); 	// Round 14
    SeedRound(L0, L1, R0, R1, K+28); 	// Round 15
    SeedRound(R0, R1, L0, L1, K+30); 	// Round 16

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

// Copying output values from last round to pbData
    ((DWORD *)pbData)[0] = R0;
    ((DWORD *)pbData)[1] = R1;
    ((DWORD *)pbData)[2] = L0;
    ((DWORD *)pbData)[3] = L1;
}

void SeedDecrypt(		/* decryption function */
		BYTE *pbData, 				// [in,out]	data to be decrypted
		DWORD *pdwRoundKey			// [in]			round keys for decryption
		)
{
	DWORD L0, L1, R0, R1;		// Iuput/output values at each rounds
	DWORD T0, T1;				// Temporary variables for round function F
	DWORD *K = pdwRoundKey;		// Pointer of round keys

// Set up input values for first round
    L0 = ((DWORD *)pbData)[0];
    L1 = ((DWORD *)pbData)[1];
    R0 = ((DWORD *)pbData)[2];
    R1 = ((DWORD *)pbData)[3];

// Reorder for big endian 
#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

    SeedRound(L0, L1, R0, R1, K+30); 	// Round 1
    SeedRound(R0, R1, L0, L1, K+28); 	// Round 2 
    SeedRound(L0, L1, R0, R1, K+26); 	// Round 3 
    SeedRound(R0, R1, L0, L1, K+24); 	// Round 4 
    SeedRound(L0, L1, R0, R1, K+22); 	// Round 5 
    SeedRound(R0, R1, L0, L1, K+20); 	// Round 6 
    SeedRound(L0, L1, R0, R1, K+18); 	// Round 7 
    SeedRound(R0, R1, L0, L1, K+16); 	// Round 8 
    SeedRound(L0, L1, R0, R1, K+14); 	// Round 9 
    SeedRound(R0, R1, L0, L1, K+12); 	// Round 10
    SeedRound(L0, L1, R0, R1, K+10); 	// Round 11
    SeedRound(R0, R1, L0, L1, K+ 8); 	// Round 12
    SeedRound(L0, L1, R0, R1, K+ 6); 	// Round 13
    SeedRound(R0, R1, L0, L1, K+ 4); 	// Round 14
    SeedRound(L0, L1, R0, R1, K+ 2); 	// Round 15
    SeedRound(R0, R1, L0, L1, K+ 0); 	// Round 16

#ifndef BIG_ENDIAN
    L0 = EndianChange(L0);
    L1 = EndianChange(L1);
    R0 = EndianChange(R0);
    R1 = EndianChange(R1);
#endif

// Copy output values from last round to pbData
    ((DWORD *)pbData)[0] = R0;
    ((DWORD *)pbData)[1] = R1;
    ((DWORD *)pbData)[2] = L0;
    ((DWORD *)pbData)[3] = L1;
}

//----------------------------------------------------------------------------------
#ifndef lint
static const char rcsid[] =
	"$Id: sha256.c 680 2003-07-25 21:57:49Z asaddi $";
#endif /* !lint */

#define ROTL(x, n) (((x) << (n)) | ((x) >> (32 - (n))))
#define ROTR(x, n) (((x) >> (n)) | ((x) << (32 - (n))))

#define Ch(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define Maj(x, y, z) (((x) & ((y) | (z))) | ((y) & (z)))
#define SIGMA0(x) (ROTR((x), 2) ^ ROTR((x), 13) ^ ROTR((x), 22))
#define SIGMA1(x) (ROTR((x), 6) ^ ROTR((x), 11) ^ ROTR((x), 25))
#define sigma0(x) (ROTR((x), 7) ^ ROTR((x), 18) ^ ((x) >> 3))
#define sigma1(x) (ROTR((x), 17) ^ ROTR((x), 19) ^ ((x) >> 10))

#define DO_ROUND() { \
  t1 = h + SIGMA1(e) + Ch(e, f, g) + *(Kp++) + *(W++); \
  t2 = SIGMA0(a) + Maj(a, b, c); \
  h = g; \
  g = f; \
  f = e; \
  e = d + t1; \
  d = c; \
  c = b; \
  b = a; \
  a = t1 + t2; \
}

static const uint32_t K[64] = {
  0x428a2f98L, 0x71374491L, 0xb5c0fbcfL, 0xe9b5dba5L,
  0x3956c25bL, 0x59f111f1L, 0x923f82a4L, 0xab1c5ed5L,
  0xd807aa98L, 0x12835b01L, 0x243185beL, 0x550c7dc3L,
  0x72be5d74L, 0x80deb1feL, 0x9bdc06a7L, 0xc19bf174L,
  0xe49b69c1L, 0xefbe4786L, 0x0fc19dc6L, 0x240ca1ccL,
  0x2de92c6fL, 0x4a7484aaL, 0x5cb0a9dcL, 0x76f988daL,
  0x983e5152L, 0xa831c66dL, 0xb00327c8L, 0xbf597fc7L,
  0xc6e00bf3L, 0xd5a79147L, 0x06ca6351L, 0x14292967L,
  0x27b70a85L, 0x2e1b2138L, 0x4d2c6dfcL, 0x53380d13L,
  0x650a7354L, 0x766a0abbL, 0x81c2c92eL, 0x92722c85L,
  0xa2bfe8a1L, 0xa81a664bL, 0xc24b8b70L, 0xc76c51a3L,
  0xd192e819L, 0xd6990624L, 0xf40e3585L, 0x106aa070L,
  0x19a4c116L, 0x1e376c08L, 0x2748774cL, 0x34b0bcb5L,
  0x391c0cb3L, 0x4ed8aa4aL, 0x5b9cca4fL, 0x682e6ff3L,
  0x748f82eeL, 0x78a5636fL, 0x84c87814L, 0x8cc70208L,
  0x90befffaL, 0xa4506cebL, 0xbef9a3f7L, 0xc67178f2L
};

#ifndef RUNTIME_ENDIAN

#ifdef WORDS_BIGENDIAN

#define BYTESWAP(x) (x)
#define BYTESWAP64(x) (x)

#else /* WORDS_BIGENDIAN */

#define BYTESWAP(x) ((ROTR((x), 8) & 0xff00ff00L) | \
		     (ROTL((x), 8) & 0x00ff00ffL))
#define BYTESWAP64(x) _byteswap64(x)

static inline uint64_t _byteswap64(uint64_t x)
{
  uint32_t a = x >> 32;
  uint32_t b = (uint32_t) x;
  return ((uint64_t) BYTESWAP(b) << 32) | (uint64_t) BYTESWAP(a);
}

#endif /* WORDS_BIGENDIAN */

#else /* !RUNTIME_ENDIAN */

#define BYTESWAP(x) _byteswap(sc->littleEndian, x)
#define BYTESWAP64(x) _byteswap64(sc->littleEndian, x)

#define _BYTESWAP(x) ((ROTR((x), 8) & 0xff00ff00L) | \
		      (ROTL((x), 8) & 0x00ff00ffL))
#define _BYTESWAP64(x) __byteswap64(x)

static inline uint64_t __byteswap64(uint64_t x)
{
  uint32_t a = x >> 32;
  uint32_t b = (uint32_t) x;
  return ((uint64_t) _BYTESWAP(b) << 32) | (uint64_t) _BYTESWAP(a);
}

static inline uint32_t _byteswap(int littleEndian, uint32_t x)
{
  if (!littleEndian)
    return x;
  else
    return _BYTESWAP(x);
}

static inline uint64_t _byteswap64(int littleEndian, uint64_t x)
{
  if (!littleEndian)
    return x;
  else
    return _BYTESWAP64(x);
}

static inline void setEndian(int *littleEndianp)
{
  union {
    uint32_t w;
    uint8_t b[4];
  } endian;

  endian.w = 1L;
  *littleEndianp = endian.b[0] != 0;
}

#endif /* !RUNTIME_ENDIAN */

static const uint8_t padding[64] = {
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define BSIZE  sizeof(__base64_table)


void SeedRoundKey(		/* key scheduling function */
		DWORD *pdwRoundKey, 		// [out]		round keys for encryption or decryption
		BYTE *pbUserKey				// [in]			secret user key 
		)
	{
	DWORD A, B, C, D;				// Iuput/output values at each rounds
	DWORD T0, T1;					// Temporary variable
	DWORD *K = pdwRoundKey;			// Pointer of round keys

// Set up input values for Key Schedule	
	A = ((DWORD *)pbUserKey)[0];
	B = ((DWORD *)pbUserKey)[1];
	C = ((DWORD *)pbUserKey)[2];
	D = ((DWORD *)pbUserKey)[3];
	 
// Reorder for big endian 
#ifndef BIG_ENDIAN
	A = EndianChange(A);
	B = EndianChange(B);
	C = EndianChange(C);
	D = EndianChange(D);
#endif

// i-th round keys( K_i,0 and K_i,1 ) are denoted as K[2*(i-1)] and K[2*i-1], respectively
	
    RoundKeyUpdate0(K   , A, B, C, D, KC0 );	// K_1,0 and K_1,1
    RoundKeyUpdate1(K+ 2, A, B, C, D, KC1 );	// K_2,0 and K_2,1
    RoundKeyUpdate0(K+ 4, A, B, C, D, KC2 );	// K_3,0 and K_3,1
    RoundKeyUpdate1(K+ 6, A, B, C, D, KC3 );	// K_4,0 and K_4,1
    RoundKeyUpdate0(K+ 8, A, B, C, D, KC4 );	// K_5,0 and K_5,1
    RoundKeyUpdate1(K+10, A, B, C, D, KC5 );	// K_6,0 and K_6,1
    RoundKeyUpdate0(K+12, A, B, C, D, KC6 );	// K_7,0 and K_7,1
    RoundKeyUpdate1(K+14, A, B, C, D, KC7 );	// K_8,0 and K_8,1
    RoundKeyUpdate0(K+16, A, B, C, D, KC8 );	// K_9,0 and K_9,1
    RoundKeyUpdate1(K+18, A, B, C, D, KC9 );	// K_10,0 and K_10,1
    RoundKeyUpdate0(K+20, A, B, C, D, KC10);	// K_11,0 and K_11,1
    RoundKeyUpdate1(K+22, A, B, C, D, KC11);	// K_12,0 and K_12,1
    RoundKeyUpdate0(K+24, A, B, C, D, KC12);	// K_13,0 and K_13,1
    RoundKeyUpdate1(K+26, A, B, C, D, KC13);	// K_14,0 and K_14,1
    RoundKeyUpdate0(K+28, A, B, C, D, KC14);	// K_15,0 and K_15,1

    T0 = A + C - KC15;
    T1 = B - D + KC15;
    K[30] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^	// K_16,0
           SS2[GetB2(T0)] ^ SS3[GetB3(T0)];
    K[31] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^	// K_16,1
           SS2[GetB2(T1)] ^ SS3[GetB3(T1)];

}

Crypt::Crypt(void)
{
}


Crypt::~Crypt(void)
{
}


int Crypt::Sha256Encrypt(char* inB, int inL)
{
	SHA256Context foo;
	uint8_t hash[SHA256_HASH_SIZE];
	memset(hash,0,SHA256_HASH_SIZE);
	char buf[1000];
	int	rem;

	void* ppass;
	ppass = inB;
	
	SHA256Init (&foo);
    SHA256Update (&foo, ppass, 4);
    SHA256Final (&foo, hash);

	CString str, strtemp;
	BYTE endata[SHA256_HASH_SIZE + 1];
	
	BYTE* enc = new BYTE[strlen(inB)];
	memset(enc, 0x00, strlen(inB));

	rem = strlen(inB) % BLOCK_SIZE;
	int block = strlen(inB) / BLOCK_SIZE;
	block = (rem == 0) ? (block) : (block + 1);

	int len = base64_encode(m_outB, hash, block * BLOCK_SIZE * 2);
	return len;
}


int Crypt::Encrypt(BYTE* inB, int inL)
{
	int	block;
	BYTE*	encB;
	DWORD	dwKey[32];

	block = inL / BLOCK_SIZE;
	block = (inL % BLOCK_SIZE) ? block+1 : block;

	encB = new BYTE[block * BLOCK_SIZE + 16];
	ZeroMemory(encB, block * BLOCK_SIZE + 16);
	CopyMemory(encB, inB, inL);

	roundKey(dwKey, pbUserKey);
	for (int ii = 0 ; ii < block ; ii++)
		encrypt(&encB[ii * BLOCK_SIZE], dwKey);

	ZeroMemory(m_outB, sizeof(m_outB));
	inL = base64_encode(m_outB, encB, block * BLOCK_SIZE);

	delete[] encB;
	return inL;
}

int Crypt::Decrypt(BYTE* inB, int inL)
{
	int	block;
	DWORD	dwKey[32];

	ZeroMemory(m_outB, sizeof(m_outB));
	inL = base64_decode(m_outB, inB, inL);

	block = inL / BLOCK_SIZE;
	roundKey(dwKey, pbUserKey);
	for (int ii = 0 ; ii < block ; ii++)
		decrypt(&m_outB[ii * BLOCK_SIZE], dwKey);
	
	return inL;
}


int Crypt::SEEDEncrypt(BYTE* inB, int inL)
{
	int	block;
	BYTE*	encB;
	DWORD	dwKey[32];

	block = inL / BLOCK_SIZE;
	block = (inL % BLOCK_SIZE) ? block+1 : block;

	encB = new BYTE[block * BLOCK_SIZE + 16];
	ZeroMemory(encB, block * BLOCK_SIZE + 16);
	CopyMemory(encB, inB, inL);
	
	inL = l_seedenc(m_outB, inB, inL);  //enc 암호화 결과  txt 암호화할 평문

	delete[] encB;
	return inL;
}

int Crypt::SEEDDecrypt(BYTE* inB, int inL)
{
	int	block;
	DWORD	dwKey[32];

	ZeroMemory(m_outB, sizeof(m_outB));
	inL = l_seeddec(m_outB, inB, inL);	
	return inL;
}

void Crypt::encrypt(BYTE* pbData, DWORD* dwKey)
{
	DWORD	L0, L1, R0, R1;	// input/output values at each rounds
	DWORD	T0, T1;		// temporary variables for round function F
	DWORD*	K = dwKey;	// pointer of round keys

	// Set up input values for first round
	L0 = ((DWORD *)pbData)[0];
	L1 = ((DWORD *)pbData)[1];
	R0 = ((DWORD *)pbData)[2];
	R1 = ((DWORD *)pbData)[3];

	SeedRound(L0, L1, R0, R1, K   ); 	// Round 1
	SeedRound(R0, R1, L0, L1, K+ 2); 	// Round 2
	SeedRound(L0, L1, R0, R1, K+ 4); 	// Round 3
	SeedRound(R0, R1, L0, L1, K+ 6); 	// Round 4
	SeedRound(L0, L1, R0, R1, K+ 8); 	// Round 5
	SeedRound(R0, R1, L0, L1, K+10); 	// Round 6
	SeedRound(L0, L1, R0, R1, K+12); 	// Round 7
	SeedRound(R0, R1, L0, L1, K+14); 	// Round 8
	SeedRound(L0, L1, R0, R1, K+16); 	// Round 9
	SeedRound(R0, R1, L0, L1, K+18); 	// Round 10
	SeedRound(L0, L1, R0, R1, K+20); 	// Round 11
	SeedRound(R0, R1, L0, L1, K+22); 	// Round 12
	SeedRound(L0, L1, R0, R1, K+24); 	// Round 13
	SeedRound(R0, R1, L0, L1, K+26); 	// Round 14
	SeedRound(L0, L1, R0, R1, K+28); 	// Round 15
	SeedRound(R0, R1, L0, L1, K+30); 	// Round 16

	// Copying output values from last round to pbData
	((DWORD *)pbData)[0] = R0;
	((DWORD *)pbData)[1] = R1;
	((DWORD *)pbData)[2] = L0;
	((DWORD *)pbData)[3] = L1;
}

void Crypt::decrypt(BYTE* pbData, DWORD* dwKey)
{
	DWORD	L0, L1, R0, R1;	// input/output values at each rounds
	DWORD	T0, T1;		// temporary variables for round function F
	DWORD*	K = dwKey;	// pointer of round keys

	// Set up input values for first round
	L0 = ((DWORD *)pbData)[0];
	L1 = ((DWORD *)pbData)[1];
	R0 = ((DWORD *)pbData)[2];
	R1 = ((DWORD *)pbData)[3];

	SeedRound(L0, L1, R0, R1, K+30); 	// Round 1
	SeedRound(R0, R1, L0, L1, K+28); 	// Round 2 
	SeedRound(L0, L1, R0, R1, K+26); 	// Round 3 
	SeedRound(R0, R1, L0, L1, K+24); 	// Round 4 
	SeedRound(L0, L1, R0, R1, K+22); 	// Round 5 
	SeedRound(R0, R1, L0, L1, K+20); 	// Round 6 
	SeedRound(L0, L1, R0, R1, K+18); 	// Round 7 
	SeedRound(R0, R1, L0, L1, K+16); 	// Round 8 
	SeedRound(L0, L1, R0, R1, K+14); 	// Round 9 
	SeedRound(R0, R1, L0, L1, K+12); 	// Round 10
	SeedRound(L0, L1, R0, R1, K+10); 	// Round 11
	SeedRound(R0, R1, L0, L1, K+ 8); 	// Round 12
	SeedRound(L0, L1, R0, R1, K+ 6); 	// Round 13
	SeedRound(R0, R1, L0, L1, K+ 4); 	// Round 14
	SeedRound(L0, L1, R0, R1, K+ 2); 	// Round 15
	SeedRound(R0, R1, L0, L1, K+ 0); 	// Round 16

	// Copy output values from last round to pbData
	((DWORD *)pbData)[0] = R0;
	((DWORD *)pbData)[1] = R1;
	((DWORD *)pbData)[2] = L0;
	((DWORD *)pbData)[3] = L1;
}


void Crypt::roundKey(DWORD* dwKey, BYTE* pKey)
{
	DWORD	A, B, C, D;	// iuput/output values at each rounds
	DWORD	T0, T1;		// temporary variable
	DWORD*	K = dwKey;	// pointer of round keys

	// Set up input values for Key Schedule	
	A = ((DWORD *)pKey)[0];
	B = ((DWORD *)pKey)[1];
	C = ((DWORD *)pKey)[2];
	D = ((DWORD *)pKey)[3];

	// i-th round keys( K_i,0 and K_i,1 ) are denoted as K[2*(i-1)] and K[2*i-1], respectively
	RoundKeyUpdate0(K   , A, B, C, D, KC0 );	// K_1,0 and K_1,1
	RoundKeyUpdate1(K+ 2, A, B, C, D, KC1 );	// K_2,0 and K_2,1
	RoundKeyUpdate0(K+ 4, A, B, C, D, KC2 );	// K_3,0 and K_3,1
	RoundKeyUpdate1(K+ 6, A, B, C, D, KC3 );	// K_4,0 and K_4,1
	RoundKeyUpdate0(K+ 8, A, B, C, D, KC4 );	// K_5,0 and K_5,1
	RoundKeyUpdate1(K+10, A, B, C, D, KC5 );	// K_6,0 and K_6,1
	RoundKeyUpdate0(K+12, A, B, C, D, KC6 );	// K_7,0 and K_7,1
	RoundKeyUpdate1(K+14, A, B, C, D, KC7 );	// K_8,0 and K_8,1
	RoundKeyUpdate0(K+16, A, B, C, D, KC8 );	// K_9,0 and K_9,1
	RoundKeyUpdate1(K+18, A, B, C, D, KC9 );	// K_10,0 and K_10,1
	RoundKeyUpdate0(K+20, A, B, C, D, KC10);	// K_11,0 and K_11,1
	RoundKeyUpdate1(K+22, A, B, C, D, KC11);	// K_12,0 and K_12,1
	RoundKeyUpdate0(K+24, A, B, C, D, KC12);	// K_13,0 and K_13,1
	RoundKeyUpdate1(K+26, A, B, C, D, KC13);	// K_14,0 and K_14,1
	RoundKeyUpdate0(K+28, A, B, C, D, KC14);	// K_15,0 and K_15,1

	T0 = A + C - KC15;
	T1 = B - D + KC15;
	K[30] = SS0[GetB0(T0)] ^ SS1[GetB1(T0)] ^ SS2[GetB2(T0)] ^ SS3[GetB3(T0)];
	K[31] = SS0[GetB0(T1)] ^ SS1[GetB1(T1)] ^ SS2[GetB2(T1)] ^ SS3[GetB3(T1)];
}

int Crypt::base64_encode(BYTE* enc, const BYTE* str, int len)
{
	int	ii = 0;
	const BYTE* current = str;

	while (len > 2)
	{
		enc[ii++] = __base64_table[current[0] >> 2];
		enc[ii++] = __base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
		enc[ii++] = __base64_table[((current[1] & 0x0f) << 2) + (current[2] >> 6)];
		enc[ii++] = __base64_table[current[2] & 0x3f];

		current += 3;
		len -=3;
	}

	if (len != 0)
	{
		enc[ii++] = __base64_table[current[0] >> 2];
		if (len > 1)
		{
			enc[ii++] = __base64_table[((current[0] & 0x03) << 4) + (current[1] >> 4)];
			enc[ii++] = __base64_table[((current[1] & 0x0f) << 2)];
			enc[ii++] = __base64_pad;
			//enc[ii++] = __base64_table[current[2] & 0x3f];
		}
		else
		{
			enc[ii++] = __base64_table[(current[0] & 0x03) << 4];
			enc[ii++] = __base64_pad;
			enc[ii++] = __base64_pad;
		}
	}
	enc[ii] = '\0';

	return ii;
}

int Crypt::base64_decode(BYTE* dec, const BYTE* str, int len)
{
	int	ch, ii, jj, kk;
	static short	reverse_table[256];
	static int	table_built = 0;
	const BYTE* current = str;

	ii = jj = 0;
	if (++table_built == 1)
	{
		char*	chp;
		for (ch = 0 ; ch < 256; ch++)
		{
			chp = strchr(__base64_table, ch);
			if (chp)
				reverse_table[ch] = chp - __base64_table;
			else
				reverse_table[ch] = -1;
		}
	}

	while ((ch = *current++) != '\0')
	{
		if (ch == __base64_pad)
			break;
	
		if (ch == ' ')
			ch = '+';

		ch = reverse_table[ch];
		if (ch < 0)
			continue;

		switch (ii % 4)
		{
		case 0:	dec[jj] = ch << 2;
			break;

		case 1:	dec[jj++] |= ch >> 4;
			dec[jj] = (ch & 0x0f) << 4;
			break;

		case 2:	dec[jj++] |= ch >> 2;
			dec[jj] = (ch & 0x03) << 6;
			break;

		case 3:	dec[jj++] |= ch;
			break;
		}
		ii++;
	}

	kk = jj;
	if (ch == __base64_pad)
	{
		switch (ii % 4)
		{
		case 0:
		case 1:
			return 0;
		case 2:
			kk++;
		case 3:
			dec[kk++] = 0;
			break;
		}
	}
	dec[kk] = '\0';

	return jj;
}


void Crypt::SHA256Init (SHA256Context *sc)
{
	#ifdef RUNTIME_ENDIAN
	  setEndian (&sc->littleEndian);
	#endif /* RUNTIME_ENDIAN */
	/*
	  sc->totalLength = 0LL;
	  sc->hash[0] = 0x6a09e667L;
	  sc->hash[1] = 0xbb67ae85L;
	  sc->hash[2] = 0x3c6ef372L;
	  sc->hash[3] = 0xa54ff53aL;
	  sc->hash[4] = 0x510e527fL;
	  sc->hash[5] = 0x9b05688cL;
	  sc->hash[6] = 0x1f83d9abL;
	  sc->hash[7] = 0x5be0cd19L;
	  sc->bufferLength = 0L;
	*/
	  sc->totalLength = 0;
	  sc->hash[0] = 0x6a09e667;
	  sc->hash[1] = 0xbb67ae85;
	  sc->hash[2] = 0x3c6ef372;
	  sc->hash[3] = 0xa54ff53a;
	  sc->hash[4] = 0x510e527f;
	  sc->hash[5] = 0x9b05688c;
	  sc->hash[6] = 0x1f83d9ab;
	  sc->hash[7] = 0x5be0cd19;
	  sc->bufferLength = 0;
}

static void burnStack (int size)
{
  char buf[128];

  memset (buf, 0, sizeof (buf));
  size -= sizeof (buf);
  if (size > 0)
    burnStack (size);
}


static void SHA256Guts (SHA256Context *sc, const uint32_t *cbuf)
{
  uint32_t buf[64];
  uint32_t *W, *W2, *W7, *W15, *W16;
  uint32_t a, b, c, d, e, f, g, h;
  uint32_t t1, t2;
  const uint32_t *Kp;
  int i;

  W = buf;

  for (i = 15; i >= 0; i--) {
    *(W++) = BYTESWAP(*cbuf);
    cbuf++;
  }

  W16 = &buf[0];
  W15 = &buf[1];
  W7 = &buf[9];
  W2 = &buf[14];

  for (i = 47; i >= 0; i--) {
    *(W++) = sigma1(*W2) + *(W7++) + sigma0(*W15) + *(W16++);
    W2++;
    W15++;
  }

  a = sc->hash[0];
  b = sc->hash[1];
  c = sc->hash[2];
  d = sc->hash[3];
  e = sc->hash[4];
  f = sc->hash[5];
  g = sc->hash[6];
  h = sc->hash[7];

  Kp = K;
  W = buf;

#ifndef SHA256_UNROLL
#define SHA256_UNROLL 1
#endif /* !SHA256_UNROLL */

#if SHA256_UNROLL == 1
  for (i = 63; i >= 0; i--)
    DO_ROUND();
#elif SHA256_UNROLL == 2
  for (i = 31; i >= 0; i--) {
    DO_ROUND(); DO_ROUND();
  }
#elif SHA256_UNROLL == 4
  for (i = 15; i >= 0; i--) {
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  }
#elif SHA256_UNROLL == 8
  for (i = 7; i >= 0; i--) {
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  }
#elif SHA256_UNROLL == 16
  for (i = 3; i >= 0; i--) {
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  }
#elif SHA256_UNROLL == 32
  for (i = 1; i >= 0; i--) {
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
    DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  }
#elif SHA256_UNROLL == 64
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
  DO_ROUND(); DO_ROUND(); DO_ROUND(); DO_ROUND();
#else
#error "SHA256_UNROLL must be 1, 2, 4, 8, 16, 32, or 64!"
#endif

  sc->hash[0] += a;
  sc->hash[1] += b;
  sc->hash[2] += c;
  sc->hash[3] += d;
  sc->hash[4] += e;
  sc->hash[5] += f;
  sc->hash[6] += g;
  sc->hash[7] += h;
}

void Crypt::SHA256Update (SHA256Context *sc, const void *vdata, uint32_t len)
{
//const uint8_t *data = vdata;
  const uint8_t *data = (uint8_t *)vdata;
  uint32_t bufferBytesLeft;
  uint32_t bytesToCopy;
  int needBurn = 0;

#ifdef SHA256_FAST_COPY
  if (sc->bufferLength) {
    bufferBytesLeft = 64L - sc->bufferLength;

    bytesToCopy = bufferBytesLeft;
    if (bytesToCopy > len)
      bytesToCopy = len;

    memcpy (&sc->buffer.bytes[sc->bufferLength], data, bytesToCopy);

    sc->totalLength += bytesToCopy * 8L;

    sc->bufferLength += bytesToCopy;
    data += bytesToCopy;
    len -= bytesToCopy;

    if (sc->bufferLength == 64L) {
      SHA256Guts (sc, sc->buffer.words);
      needBurn = 1;
      sc->bufferLength = 0L;
    }
  }

  while (len > 63L) {
    sc->totalLength += 512L;

    SHA256Guts (sc, data);
    needBurn = 1;

    data += 64L;
    len -= 64L;
  }

  if (len) {
    memcpy (&sc->buffer.bytes[sc->bufferLength], data, len);

    sc->totalLength += len * 8L;

    sc->bufferLength += len;
  }
#else /* SHA256_FAST_COPY */
  while (len) {
    bufferBytesLeft = 64L - sc->bufferLength;

    bytesToCopy = bufferBytesLeft;
    if (bytesToCopy > len)
      bytesToCopy = len;

    memcpy (&sc->buffer.bytes[sc->bufferLength], data, bytesToCopy);

    sc->totalLength += bytesToCopy * 8L;

    sc->bufferLength += bytesToCopy;
    data += bytesToCopy;
    len -= bytesToCopy;

    if (sc->bufferLength == 64L) {
      SHA256Guts (sc, sc->buffer.words);
      needBurn = 1;
      sc->bufferLength = 0L;
    }
  }
#endif /* SHA256_FAST_COPY */

  if (needBurn)
    burnStack (sizeof (uint32_t[74]) + sizeof (uint32_t *[6]) + sizeof (int));
}


void Crypt::SHA256Final (SHA256Context *sc, uint8_t hash[SHA256_HASH_SIZE])
{
  uint32_t bytesToPad;
  uint64_t lengthPad;
  int i;

  bytesToPad = 120L - sc->bufferLength;
  if (bytesToPad > 64L)
    bytesToPad -= 64L;

  lengthPad = BYTESWAP64(sc->totalLength);

  SHA256Update (sc, padding, bytesToPad);
  SHA256Update (sc, &lengthPad, 8L);

  if (hash) {
    for (i = 0; i < SHA256_HASH_WORDS; i++) {
#ifdef SHA256_FAST_COPY
      *((uint32_t *) hash) = BYTESWAP(sc->hash[i]);
#else /* SHA256_FAST_COPY */
      hash[0] = (uint8_t) (sc->hash[i] >> 24);
      hash[1] = (uint8_t) (sc->hash[i] >> 16);
      hash[2] = (uint8_t) (sc->hash[i] >> 8);
      hash[3] = (uint8_t) sc->hash[i];
#endif /* SHA256_FAST_COPY */
      hash += 4;
    }
  }
}

int Crypt::l_seedenc(BYTE *enc, BYTE *txt, int len)   //enc 암호화 결과  txt 암호화할 평문
{
	int	ii;
	int	jj;
	int	block;
	int	rem;
	BYTE	*encb;
	DWORD	pdwRoundKey[32];
		
	block = len / BLOCK_SIZE;
	rem = len % BLOCK_SIZE;

	block = (rem == 0) ? (block) : (block + 1);

	//encb = malloc(block * BLOCK_SIZE+16);
	encb = (unsigned char*)malloc(block * BLOCK_SIZE+16);
	if(encb == NULL)
	{
		return(-1);
	}

	memset(encb, 0x00, block * BLOCK_SIZE);
	memset(enc,  0x00, sizeof(enc));

	memcpy(encb, txt, len);   //

	SeedRoundKey(pdwRoundKey, pbUserKey);

	for(ii = 0 ; ii < block ; ii++)
	{
		SeedEncrypt(&encb[ii * BLOCK_SIZE], pdwRoundKey);
	}

	len = base64_encode(enc, encb, block * BLOCK_SIZE);

#if 0
	len = dec2hex(enc, encb, block * BLOCK_SIZE);
#endif

	free(encb);

	return(len);
}

int Crypt::l_seeddec(BYTE *dec,  BYTE *enc, int len)  //dec 복호화된 결과  enc 복호화할 암호문
{	
	int	ii;
	int	block;
	int	rem;
	int	declen;
	DWORD	pdwRoundKey[32];


	declen = base64_decode(dec, enc, len);
#if 0
	declen = hex2dec(dec, enc, len);
#endif

	block = declen / BLOCK_SIZE;

	SeedRoundKey(pdwRoundKey, pbUserKey);

	for(ii = 0 ; ii < block ; ii++)
	{
		SeedDecrypt(&dec[ii * BLOCK_SIZE], pdwRoundKey);
	}
	
	return(declen);
}
