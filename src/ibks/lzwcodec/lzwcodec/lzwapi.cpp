#include "pch.h"
#include "lzwcodec.h"
#include "Compress.h"
#include "Zip.h"
#include <cstring>

// 원본을 지우기 전에 <파일명>_<YYMMDDHHMM>.<확장자> 이름으로 미리 복사해두기 위한 경로 조립
static void BuildBackupPath(const char* ofname, char* outPath, size_t outSize)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	char stamp[16];
	sprintf_s(stamp, sizeof(stamp), "_%02d%02d%02d%02d%02d",
		st.wYear % 100, st.wMonth, st.wDay, st.wHour, st.wMinute);

	const char* lastSlash = strrchr(ofname, '\\');
	const char* lastSlash2 = strrchr(ofname, '/');
	if (lastSlash2 && (!lastSlash || lastSlash2 > lastSlash))
		lastSlash = lastSlash2;
	const char* fileNameStart = lastSlash ? lastSlash + 1 : ofname;
	const char* dot = strrchr(fileNameStart, '.');

	size_t dirLen  = fileNameStart - ofname;
	size_t baseLen = dot ? (size_t)(dot - fileNameStart) : strlen(fileNameStart);

	outPath[0] = '\0';
	strncat_s(outPath, outSize, ofname, dirLen);
	strncat_s(outPath, outSize, fileNameStart, baseLen);
	strcat_s(outPath, outSize, stamp);
	if (dot)
		strcat_s(outPath, outSize, dot);
}

HLZW LZW_Open(int bits)
{
	CCompress* p = new CCompress();
	// TODO: bits(13/16)를 실제로 반영하는 건 다음 단계(코어 통합)에서 처리
	return (HLZW)p;
}

int LZW_Compress(HLZW h, const unsigned char* in, int inLen, unsigned char* out)
{
	if (!h) return -1;
	return ((CCompress*)h)->compress((BYTE*)in, inLen, out);
}

int LZW_Decompress(HLZW h, const unsigned char* in, int inLen, unsigned char* out)
{
	if (!h) return -1;
	return ((CCompress*)h)->decompress((BYTE*)in, inLen, out);
}

void LZW_Close(HLZW h)
{
	if (h) delete (CCompress*)h;
}

int LZW_CompressFile(const char* ofname, const char* cfname, bool keepOriginal)
{
	if (keepOriginal)
	{
		char backupPath[MAX_PATH] = { 0 };
		BuildBackupPath(ofname, backupPath, sizeof(backupPath));
		CopyFileA(ofname, backupPath, FALSE);   // 압축(및 CZip 내부의 원본삭제) 전에 먼저 안전하게 백업
	}

	CZip* zip = new CZip();
	int ret = zip->Compress((char*)ofname, (char*)cfname) ? 1 : 0;
	delete zip;
	return ret;
}

int LZW_DecompressFile(const char* ofname, const char* cfname, bool keepOriginal)
{
	if (keepOriginal)
	{
		char backupPath[MAX_PATH] = { 0 };
		BuildBackupPath(ofname, backupPath, sizeof(backupPath));
		CopyFileA(ofname, backupPath, FALSE);
	}

	CZip* zip = new CZip();
	int ret = zip->Decompress((char*)ofname, (char*)cfname) ? 1 : 0;
	delete zip;
	return ret;
}