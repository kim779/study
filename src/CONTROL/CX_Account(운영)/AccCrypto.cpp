#include "stdafx.h"
#include "AccCrypto.h"

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
		throw std::runtime_error("CryptAcquireContext failed");

	if (!CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash)) {
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptCreateHash failed");
	}

	// m_regkey 문자열을 해시 입력으로
	if (!CryptHashData(hHash, (BYTE*)(LPCSTR)regkey, regkey.GetLength(), 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptHashData failed");
	}

	DWORD keyLen = 32;
	std::vector<BYTE> key(keyLen);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, key.data(), &keyLen, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptGetHashParam failed");
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
		throw std::runtime_error("CryptAcquireContext failed");

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
		throw std::runtime_error("CryptImportKey failed");
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
		throw std::runtime_error("CryptEncrypt failed");
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
		throw std::runtime_error("Invalid encrypted data");

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
		throw std::runtime_error("CryptAcquireContext failed");

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
		throw std::runtime_error("CryptImportKey failed");
	}

	DWORD mode = CRYPT_MODE_CBC;
	CryptSetKeyParam(hKey, KP_MODE, (BYTE*)&mode, 0);
	CryptSetKeyParam(hKey, KP_IV, iv, 0);

	DWORD dataLen = (DWORD)data.size();
	if (!CryptDecrypt(hKey, 0, TRUE, 0, data.data(), &dataLen)) {
		CryptDestroyKey(hKey);
		CryptReleaseContext(hProv, 0);
		throw std::runtime_error("CryptDecrypt failed");
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