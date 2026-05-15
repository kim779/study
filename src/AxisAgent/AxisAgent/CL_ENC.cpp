#include "pch.h"
#include "CL_ENC.h"
#pragma comment(lib, "Crypt32.lib")
// 여기서만 정의
const BYTE s_xorKey[] = { 0x4A, 0x7F, 0x3C, 0xB2, 0x91, 0xE5, 0x2D, 0x68 };
const int  s_keyLen = sizeof(s_xorKey);

CString BytesToHex(const CByteArray& arr)
{
    CString sResult;
    for (int i = 0; i < arr.GetSize(); i++)
    {
        CString sByte;
        sByte.Format(_T("%02X"), arr[i]);
        sResult += sByte;
    }
    return sResult;
}

void HexToBytes(const CString& sHex, CByteArray& arr)
{
    arr.RemoveAll();
    for (int i = 0; i + 1 < sHex.GetLength(); i += 2)
    {
        CString sByte = sHex.Mid(i, 2);
        arr.Add((BYTE)_tcstol(sByte, nullptr, 16));
    }
}

std::vector<BYTE> DeriveKeyFromRegkey(const CString& regkey)
{
	HCRYPTPROV hProv = 0;
	HCRYPTHASH hHash = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptAcquireContext failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
		return std::vector<BYTE>{};
	}

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptCreateHash failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
		return std::vector<BYTE>{};
	}

	// m_regkey 문자열을 해시 입력으로
	if (!CryptHashData(hHash, (BYTE*)(LPCSTR)regkey, regkey.GetLength(), 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptHashData failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	DWORD keyLen = 32;
	std::vector<BYTE> key(keyLen);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, key.data(), &keyLen, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptGetHashParam failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	return key;  // SHA-256 = 32바이트 = AES-256 키로 딱 맞음
}

CString AesEncrypt(const CString& plainText, const std::vector<BYTE>& key)
{
	HCRYPTPROV hProv = 0;
	HCRYPTKEY  hKey = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptAcquireContext failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	// 키 임포트용 구조체
	struct {
		BLOBHEADER hdr;
		DWORD      keyLen;
		BYTE       keyData[32];
	} keyBlob = {};

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_256;
	keyBlob.keyLen = 32;
	memcpy(keyBlob.keyData, key.data(), 32);

	if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey)) {
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptImportKey failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	// CBC 모드 설정
	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);

	// IV 랜덤 생성 (16바이트)
	BYTE iv[16] = {};
	CryptGenRandom(hProv, 16, iv);
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	// CP949 바이트 기준으로 복사 (한글 2바이트 정확히 처리)
	LPCSTR pszPlain = (LPCSTR)plainText;
	DWORD  plainLen = (DWORD)strlen(pszPlain);  // 실제 바이트 수

	// 암호화 버퍼: AES 블록(16바이트) 단위 올림 + 여유
	DWORD bufLen = ((plainLen / 16) + 1) * 16;
	std::vector<BYTE> data(bufLen, 0);
	memcpy(data.data(), pszPlain, plainLen);

	DWORD dataLen = plainLen;
	if (!CryptEncrypt(hKey, 0, TRUE, 0, data.data(), &dataLen, bufLen)) {
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptEncrypt failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	CryptDestroyKey(hKey);
	CryptReleaseContext(hProv, 0);

	// IV(16바이트) + 암호문 합쳐서 Base64 인코딩
	std::vector<BYTE> ivAndData;
	ivAndData.reserve(16 + dataLen);
	ivAndData.insert(ivAndData.end(), iv, iv + 16);
	ivAndData.insert(ivAndData.end(), data.begin(), data.begin() + dataLen);

	// 민감 데이터 정리
	SecureZeroMemory(data.data(), data.size());
	SecureZeroMemory(keyBlob.keyData, 32);

	DWORD b64Len = 0;
	CryptBinaryToStringA(ivAndData.data(), (DWORD)ivAndData.size(),
		CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &b64Len);
	CString b64;
	CryptBinaryToStringA(ivAndData.data(), (DWORD)ivAndData.size(),
		CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
		b64.GetBuffer(b64Len), &b64Len);
	b64.ReleaseBuffer();

	return b64;
}

CString AesDecrypt(const CString& encText, const std::vector<BYTE>& key)
{
	// Base64 디코딩
	DWORD blobLen = 0;
	CryptStringToBinaryA(encText, encText.GetLength(), CRYPT_STRING_BASE64,
		nullptr, &blobLen, nullptr, nullptr);
	if (blobLen < 16)
	{
		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("Invalid encrypted data: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	std::vector<BYTE> ivAndData(blobLen);
	CryptStringToBinaryA(encText, encText.GetLength(), CRYPT_STRING_BASE64,
		ivAndData.data(), &blobLen, nullptr, nullptr);

	// IV(앞 16바이트) / 암호문 분리
	BYTE iv[16] = {};
	memcpy(iv, ivAndData.data(), 16);
	std::vector<BYTE> data(ivAndData.begin() + 16, ivAndData.end());

	HCRYPTPROV hProv = 0;
	HCRYPTKEY  hKey = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
	{
		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptAcquireContext failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}
		
	struct {
		BLOBHEADER hdr;
		DWORD      keyLen;
		BYTE       keyData[32];
	} keyBlob = {};

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.reserved = 0;
	keyBlob.hdr.aiKeyAlg = CALG_AES_256;
	keyBlob.keyLen = 32;
	memcpy(keyBlob.keyData, key.data(), 32);

	if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey)) {
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptImportKey failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	DWORD dataLen = (DWORD)data.size();
	if (!CryptDecrypt(hKey, 0, TRUE, 0, data.data(), &dataLen)) {
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);

		DWORD dwErr = GetLastError();
		CString slog;
		slog.Format(_T("CryptDecrypt failed: 0x%08X"), dwErr);
		OutputDebugString(slog);
	}

	CryptDestroyKey(hKey);
	CryptReleaseContext(hProv, 0);
	SecureZeroMemory(keyBlob.keyData, 32);

	// ★ null terminator 공간 확보
	data.resize(dataLen + 1, 0);
	data[dataLen] = 0;
	CString result((LPCSTR)data.data());

	SecureZeroMemory(data.data(), data.size());
	return result;
}





static const BYTE AES_KEY[32] =
{
	'1','q','a','z','2','w','s','x',
	'3','e','d','c','4','r','f','v',
	'5','t','g','b','6','y','h','n',
	'7','u','j','m','8','i','k','l'
};

static CString Base64Encode(const BYTE* data, DWORD len)
{
	DWORD outLen = 0;
	CryptBinaryToStringA(data, len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, NULL, &outLen);

	CStringA out;
	CryptBinaryToStringA(data, len, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF,
		out.GetBuffer(outLen), &outLen);
	out.ReleaseBuffer();

	return CString(out);
}

static std::vector<BYTE> Base64Decode(const CString& base64)
{
	CStringA b64(base64);

	DWORD outLen = 0;
	CryptStringToBinaryA(b64, 0, CRYPT_STRING_BASE64, NULL, &outLen, NULL, NULL);

	std::vector<BYTE> out(outLen);
	CryptStringToBinaryA(b64, 0, CRYPT_STRING_BASE64, out.data(), &outLen, NULL, NULL);
	out.resize(outLen);

	return out;
}

CString Aes256EncryptBase64(const CStringA& plain)
{
	HCRYPTPROV hProv = 0;
	HCRYPTKEY hKey = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		return _T("");

	struct KEYBLOB
	{
		BLOBHEADER hdr;
		DWORD keyLen;
		BYTE keyData[32];
	} keyBlob = {};

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.aiKeyAlg = CALG_AES_256;
	keyBlob.keyLen = 32;
	memcpy(keyBlob.keyData, AES_KEY, 32);

	if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey))
	{
		CryptReleaseContext(hProv, 0);
		return _T("");
	}

	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);

	BYTE iv[16] = { 0 };
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	DWORD blockLen = 16;
	DWORD dataLen = plain.GetLength();
	DWORD bufLen = dataLen + blockLen;

	std::vector<BYTE> buffer(bufLen);
	memcpy(buffer.data(), plain.GetString(), dataLen);

	if (!CryptEncrypt(hKey, 0, TRUE, 0, buffer.data(), &dataLen, bufLen))
	{
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);
		return _T("");
	}

	CString result = Base64Encode(buffer.data(), dataLen);

	CryptDestroyKey(hKey);
	CryptReleaseContext(hProv, 0);

	return result;
}

CString Aes256DecryptBase64(const CString& base64Cipher)
{
	HCRYPTPROV hProv = 0;
	HCRYPTKEY hKey = 0;

	if (!CryptAcquireContextA(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT))
		return _T("");

	struct KEYBLOB
	{
		BLOBHEADER hdr;
		DWORD keyLen;
		BYTE keyData[32];
	} keyBlob = {};

	keyBlob.hdr.bType = PLAINTEXTKEYBLOB;
	keyBlob.hdr.bVersion = CUR_BLOB_VERSION;
	keyBlob.hdr.aiKeyAlg = CALG_AES_256;
	keyBlob.keyLen = 32;
	memcpy(keyBlob.keyData, AES_KEY, 32);

	if (!CryptImportKey(hProv, (BYTE*)&keyBlob, sizeof(keyBlob), 0, 0, &hKey))
	{
		CryptReleaseContext(hProv, 0);
		return _T("");
	}

	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);

	BYTE iv[16] = { 0 };
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	std::vector<BYTE> buffer = Base64Decode(base64Cipher);
	DWORD dataLen = (DWORD)buffer.size();

	if (!CryptDecrypt(hKey, 0, TRUE, 0, buffer.data(), &dataLen))
	{
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);
		return _T("");
	}

	CStringA plainA((LPCSTR)buffer.data(), dataLen);
	CString result(plainA);

	CryptDestroyKey(hKey);
	CryptReleaseContext(hProv, 0);

	return result;
}