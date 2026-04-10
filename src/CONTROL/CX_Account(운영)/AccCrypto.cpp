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
