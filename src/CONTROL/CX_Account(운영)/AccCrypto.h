#pragma once

// 선언만! 정의는 AccCrypto.cpp에
extern const BYTE s_xorKey[];
extern const int  s_keyLen;

CString BytesToHex(const CByteArray& arr);
void    HexToBytes(const CString& sHex, CByteArray& arr);

std::vector<BYTE> DeriveKeyFromRegkey(const CString& regkey);
CString AesEncrypt(const CString& plainText, const std::vector<BYTE>& key);
CString AesDecrypt(const CString& encText, const std::vector<BYTE>& key);

//class AccCrypto
//{
//};

