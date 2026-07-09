#pragma once
#include <stdint.h>

#define	NoRounds	16		// the number of rounds
#define	NoRoundKeys	(NoRounds*2)	// the number of round-keys
#define	SeedBlockSize	16		// block length in bytes
#define	SeedBlockLen	128		// block length in bits

#define SHA256_HASH_SIZE 32

/* Hash size in 32-bit words */
#define SHA256_HASH_WORDS 8

struct _SHA256Context {
  uint64_t totalLength;
  uint32_t hash[SHA256_HASH_WORDS];
  uint32_t bufferLength;
  union {
    uint32_t words[16];
    uint8_t bytes[64];
  } buffer;
#ifdef RUNTIME_ENDIAN
  int littleEndian;
#endif /* RUNTIME_ENDIAN */
};

#define EndianChange(dwS)                       \
    ( (ROTL((dwS),  8) & (DWORD)0x00ff00ff) |   \
      (ROTL((dwS), 24) & (DWORD)0xff00ff00) )


typedef struct _SHA256Context SHA256Context;


/******************************** Common Macros *******************************/

// macroses for left or right rotations
#if defined(_MSC_VER)
    #define ROTL(x, n)     (_lrotl((x), (n)))		// left rotation
    #define ROTR(x, n)     (_lrotr((x), (n)))		// right rotation
#else
    #define ROTL(x, n)     (((x) << (n)) | ((x) >> (32-(n))))		// left rotation
    #define ROTR(x, n)     (((x) >> (n)) | ((x) << (32-(n))))		// right rotation
#endif

// macroses for converting endianess
#define EndianChange(dwS)                       \
    ( (ROTL((dwS),  8) & (DWORD)0x00ff00ff) |   \
      (ROTL((dwS), 24) & (DWORD)0xff00ff00) )


/*************************** Function Declarations ****************************/

class Crypt
{
public:
	Crypt(void);
	~Crypt(void);

	BYTE	m_outB[30000];
	BYTE    m_data;
	CStringArray  m_arrdata, m_arrindex;
	int     icnt ;
	int	Encrypt(BYTE* inB, int inL);
	int	Decrypt(BYTE* inB, int inL);
	int	SEEDEncrypt(BYTE* inB, int inL);
	int	SEEDDecrypt(BYTE* inB, int inL);
	int	Sha256Encrypt(char* inB, int inL);	
	int encode(BYTE* dec, int ch, int mod, int cnt);

	void	encrypt(BYTE* pbData, DWORD* dwKey);	// [in,out] data to be encrypted
								// [in]     round keys for encryption
	void	decrypt(BYTE* pbData, DWORD* dwKey); 	// [in,out] data to be decrypted
								// [in]     round keys for decryption
	void	roundKey(DWORD* dwKey, BYTE* pKey);	// [out]    round keys for encryption or decryption
								// [in]     secret user key 
	int	base64_encode(BYTE* enc,  BYTE* str, int len);
	int	base64_decode(BYTE* dec,  BYTE* str, int len);
	int base64_decode(unsigned char * _szIn, unsigned char* &_szOutPtr, int _nSize, int nn);
	int l_seedenc(BYTE *enc, BYTE *txt, int len);
	int l_seeddec(BYTE *dec,  BYTE *enc, int len);

	void SHA256Init (SHA256Context *sc);
	void SHA256Update (SHA256Context *sc, const void *data, uint32_t len);
	void SHA256Final (SHA256Context *sc, uint8_t hash[SHA256_HASH_SIZE]);
};

