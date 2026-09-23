#pragma once

#ifdef  LZWCODEC_EXPORTS
#define LZW_API extern "C" __declspec(dllexport)
#else
#define LZW_API extern "C" __declspec(dllimport)
#endif

typedef void* HLZW; //호출자는 내부 구조를 몰라도 됨

LZW_API HLZW LZW_Open(int bits);
LZW_API int  LZW_Compress(HLZW h, const unsigned char* in, int inLen, unsigned char* out);
LZW_API int  LZW_Decompress(HLZW h, const unsigned char* in, int inLen, unsigned char* out);
LZW_API void LZW_Close(HLZW h);

LZW_API int LZW_CompressFile(const char* ofname, const char* cfname, bool keepOriginal);
LZW_API int LZW_DecompressFile(const char* ofname, const char* cfname, bool keepOriginal);
