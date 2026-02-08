#include "StdAfx.h"
#include "Utils.h"

#include "../TRDll/DR_Include/hana_TypeDef.h"

CString CUtils::Parser(CString &sSrc, CString sDelimiter)
{
	if (sSrc.Find(sDelimiter) == -1)
	{
		CString sTmp = sSrc;
		sSrc.Empty();
		return sTmp;
	}
	else
	{
		CString sTmp = sSrc.Left(sSrc.Find(sDelimiter));
		sSrc = sSrc.Mid(sSrc.Find(sDelimiter) + sDelimiter.GetLength());
		return sTmp;
	}
	
	return "";
}

int CUtils::GetBtnWidth(int nTextLen)
{
	return (int)BTN_WIDTH_BASE + (int)BTN_WIDTH_STEP * nTextLen;
}

int CUtils::MakeRecentRtMap(const char *pszData, CMapStringToString &mapRt, BOOL bTrace/* = FALSE*/)
{
	CString sData = pszData;
	CString sFirst = Parser(sData, "\n");

//	while (sFirst.GetLength())
	if (sFirst.GetLength())
	{
		CString sKey = "CODE";
		CString sCode = Parser(sFirst, char(CH_TAB));
//		if (sCode.IsEmpty())
//			break;
		
		if (sCode.GetLength())
		{
			sCode.TrimLeft(); sCode.TrimRight();
			mapRt.SetAt(sKey, sCode);

			if (bTrace)
				TRACE("\t\t [%s] : [%s]\n", sKey, sCode);

			for ( ; ; )
			{
				sKey = Parser(sFirst, char(CH_TAB));
				if (sKey.IsEmpty())
					break;

				CString sVal = Parser(sFirst, char(CH_TAB));
				sVal.TrimLeft(); sVal.TrimRight();
				mapRt.SetAt(sKey, sVal);
				
				if (bTrace)
					TRACE("\t\t [%s] : [%s]\n", sKey, sVal);
			}
		}

//		sFirst = CUtils::Parser(sData, "\n");
	}

	return mapRt.GetCount();
}

CString CUtils::PeekCode(CString sData)
{
	CString sFirst = Parser(sData, "\n");
	
	if (sFirst.GetLength())
	{
		CString sCode = Parser(sFirst, char(CH_TAB));
		return sCode;
	}

	return "";
}

CString CUtils::AddComma(CString &sSrc)
{
	CString sNewVal = sSrc;
	sNewVal.TrimLeft(); sNewVal.TrimRight();
	
	BOOL bMinus = FALSE;
	if (atof(sSrc) < 0)
		bMinus = TRUE;
	
	sNewVal.Remove(',');
	sNewVal.Remove('-');
	sNewVal.Remove('+');
	
	CString sLeft;
	int nMark = sNewVal.Find(".");
	if (nMark >= 0)
		sLeft = sNewVal.Left(nMark);
	else
		sLeft = sNewVal;
	
	int nLen = sLeft.GetLength();
	if (nLen <= 3)
		return sSrc;
	
	int nCnt = nLen / 3;
	int nFirstIdx = nLen % 3;
	if (nFirstIdx > 0)
		sNewVal.Insert(nFirstIdx, ',');
	
	for (int i = 0; i < nCnt - 1; i++)
	{
		int nNextIdx;
		if (nFirstIdx > 0)
			nNextIdx = nFirstIdx + ((i + 1) * 3) + (i + 1);
		else
			nNextIdx = ((i + 1) * 3) + i;
		
		sNewVal.Insert(nNextIdx, ',');	
	}
	
	if (bMinus)
		return "-" + sNewVal;
	else
		return sNewVal;
}

/*
int	CUtils::H2NL(int n)
{
	return htonl(n);
}
*/

long CUtils::GetLong(const char *p, int nSize)
{
	CString sTmp = GetString(p, nSize, TRUE);
	if (sTmp.GetLength() <= 0)
		sTmp = "0";

	return atol(sTmp);
}

CString CUtils::GetString(const char *p, int nSize, BOOL bTrim/*= FALSE*/)
{
	CString sRet;
	
	char *pTmp = new char[nSize + 1];
	memset(pTmp, 0x00, nSize + 1);
	memcpy(pTmp, p, nSize);

	sRet = pTmp;
	delete pTmp;

	if (bTrim)
	{
		sRet.TrimLeft();
		sRet.TrimRight();
	}

	return sRet;
}

//20120112 이문수 리소스체크기능추가 >>
DWORD (__stdcall *imp_GetGuiResources)(HANDLE, DWORD) = NULL;

BOOL CUtils::CheckResource()
{
	static DWORD _guiResGDIPeak = 0;
	static DWORD _guiResUSERPeak = 0;

	HKEY hkey = NULL;
	DWORD  p = sizeof(DWORD), type = REG_DWORD;

	DWORD GdiLimit, UserLimit;

	if(_guiResGDIPeak == 0)
	{
		_guiResGDIPeak = 10000;

		if(RegOpenKey(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WINDOWS"), &hkey) == ERROR_SUCCESS)
		{
			RegQueryValueEx(hkey, TEXT("GDIProcessHandleQuota"), NULL, &type,(LPBYTE)&_guiResGDIPeak, &p);
			RegCloseKey(hkey);
		}

	}

	if(_guiResUSERPeak == 0)
	{
		_guiResUSERPeak = 10000;

		if(RegOpenKey(HKEY_LOCAL_MACHINE,TEXT("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\WINDOWS"), &hkey) == ERROR_SUCCESS)
		{
			RegQueryValueEx(hkey, TEXT("USERProcessHandleQuota"), NULL, &type,(LPBYTE)&_guiResUSERPeak, &p);
			RegCloseKey(hkey);
		}
	}

	//if(bMulti)
	{
		GdiLimit = _guiResGDIPeak - 1500;
		UserLimit = _guiResUSERPeak - 1500;
	}

	
	HMODULE hModule		= NULL;
	imp_GetGuiResources = NULL;
	hModule = GetModuleHandle("USER32.DLL");
	if (hModule != NULL)
		imp_GetGuiResources = (DWORD (__stdcall *)(HANDLE, DWORD))GetProcAddress(hModule, "GetGuiResources");

	if(!imp_GetGuiResources)
		return TRUE;

	DWORD _guiResGDICount	= imp_GetGuiResources(::GetCurrentProcess(), 0);
	DWORD _guiResUSERCount	= imp_GetGuiResources(::GetCurrentProcess(), 1);

	BOOL	bGDI	= (_guiResGDICount		> GdiLimit);
	BOOL	bUSER	= (_guiResUSERCount		> UserLimit);

	if(bGDI || bUSER)
	{
		CString str;
		str.Format("리소스가 부족하여 화면을 열 수 없습니다.(U%d, G%d)\n사용하지 않은 화면을 닫아 주시기 바랍니다.\n",_guiResUSERCount,  _guiResGDICount);
		AfxMessageBox(str);
		return FALSE;
	}

	return TRUE;
}
//20120112 이문수 <<