// SystemTradingEncoder.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "SystemTradingEncoder.h"
#include "EncCharacter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingEncoderApp

BEGIN_MESSAGE_MAP(CSystemTradingEncoderApp, CWinApp)
	//{{AFX_MSG_MAP(CSystemTradingEncoderApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSystemTradingEncoderApp construction

CSystemTradingEncoderApp::CSystemTradingEncoderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSystemTradingEncoderApp object
CSystemTradingEncoderApp theApp;

BOOL CSystemTradingEncoderApp::EncryptionFile(BOOL bEnc, int nKey, LPCSTR szSource, LPCSTR szDestination, LPSTR lpszLine1, LPSTR lpszLine2, LPSTR lpszLine3)
{
	FILE* file = NULL;
	char line[MAX_PATH+1];
	char buf[MAX_PATH+1];
	CString szMsg;

	if(bEnc)
	{
		file = fopen(szSource, "rt");
		if(!file) return FALSE;

		FILE* fp = fopen(szDestination, "wb");
		if(!fp)
		{
			fclose(file);
			return FALSE;
		}

		if(lpszLine1)
			lEncryption(nKey, (unsigned char*)lpszLine1, (unsigned char*)buf, MAX_PATH);
		else
		{
			::memset(buf, 0x20, MAX_PATH);
			lEncryption(nKey, (unsigned char*)buf, (unsigned char*)buf, MAX_PATH);
		}
		buf[MAX_PATH]=0x00;
		fwrite(buf, MAX_PATH, 1, fp);
		if(lpszLine2)
			lEncryption(nKey, (unsigned char*)lpszLine2, (unsigned char*)buf, MAX_PATH);
		else
		{
			::memset(buf, 0x20, MAX_PATH);
			lEncryption(nKey, (unsigned char*)buf, (unsigned char*)buf, MAX_PATH);
		}
		buf[MAX_PATH]=0x00;
		fwrite(buf, MAX_PATH, 1, fp);
		if(lpszLine3)
			lEncryption(nKey, (unsigned char*)lpszLine3, (unsigned char*)buf, MAX_PATH);
		else
		{
			::memset(buf, 0x20, MAX_PATH);
			lEncryption(nKey, (unsigned char*)buf, (unsigned char*)buf, MAX_PATH);
		}
		buf[MAX_PATH]=0x00;
		fwrite(buf, MAX_PATH, 1, fp);

		while(fgets(line, MAX_PATH, file))
		{
			lEncryption(nKey, (unsigned char*)line, (unsigned char*)buf, MAX_PATH);
			buf[MAX_PATH]=0x00;
			fwrite(buf, MAX_PATH, 1, fp);
		}
		fclose(file);
		fclose(fp);

		return TRUE;
	}
	else
	{
		if( (file=fopen(szSource,"rb")) != NULL )
		{
			USHORT nLine = 0;
			FILE* fp = fopen(szDestination, "wt");
			while(fread( line, MAX_PATH, sizeof(char ), file) != NULL)
			{
				lEncryption(nKey, (unsigned char*)line, (unsigned char*)buf, MAX_PATH);
				buf[MAX_PATH]=0x00;

				if(nLine < 3)
				{
					if(lpszLine1 && nLine == 0)
						::lstrcpy(lpszLine1, buf);
					else if(lpszLine2 && nLine == 1)
						::lstrcpy(lpszLine2, buf);
					else if(lpszLine3 && nLine == 2)
						::lstrcpy(lpszLine3, buf);

					nLine++;
				}
				else if(fp) 
				{
					szMsg = buf;
					szMsg.TrimRight();

					fwrite(szMsg, 1, szMsg.GetLength(), fp);
					fwrite("\n", 1, 1, fp);
				}
			}
			if(fp) {
				fflush(fp);
				fclose(fp);
			}
			fclose(file);
			return TRUE;
		}
	}

	return FALSE;
}

void CSystemTradingEncoderApp::DecryptCharacterz(LPSTR szEncCharacter, LPSTR szDestination)
{
	for(int i = 0; i <= 42; i++)
	{
		const char** szSelCharacter = szCharacter[i];
		for(int j = 0; j < 8; j++)
		{
			if(::strcmp(szSelCharacter[j], szEncCharacter) == 0)
			{
				char szDecChar[2] = {0, };
				szDecChar[0] = i + 0x30;

				::strcat(szDestination, szDecChar);
				return;
			}
		}
	}
}

void CSystemTradingEncoderApp::EncryptCharacterz(char cCharacter, LPSTR szDestination)
{
	int nCharIndex = (UINT)cCharacter - 0x30;
	int nIndex = ::rand() % 8;

	const char** szSelCharacter = szCharacter[nCharIndex];
	::strcat(szDestination, szSelCharacter[nIndex]);
}


extern "C" __declspec(dllexport) BOOL EX_EncryptFile(LPCSTR szSource, LPCSTR szDestination, LPSTR lpszLine1, LPSTR lpszLine2, LPSTR lpszLine3)
{
	return theApp.EncryptionFile(TRUE, ENC_KEY, szSource, szDestination, lpszLine1, lpszLine2, lpszLine3);
}

extern "C" __declspec(dllexport) BOOL EX_DecryptFile(LPCSTR szSource, LPCSTR szDestination, LPSTR lpszLine1, LPSTR lpszLine2, LPSTR lpszLine3)
{
	return theApp.EncryptionFile(FALSE, ENC_KEY, szSource, szDestination, lpszLine1, lpszLine2, lpszLine3);
}

extern "C" __declspec(dllexport) BOOL EX_EncryptWord(LPSTR szSource, LPSTR szDestination)
{
	int nLenSource = ::strlen(szSource);
	if(nLenSource <= 0)
		return FALSE;

	time_t t;
	::srand((unsigned)time(&t)); 

	for(int i = 0; i < nLenSource; i++)
		theApp.EncryptCharacterz(szSource[i], szDestination);

	return TRUE;
}

extern "C" __declspec(dllexport) BOOL EX_DecryptWord(LPSTR szSource, LPSTR szDestination)
{
	int nLenSource = ::strlen(szSource);
	if(nLenSource <= 0 || (nLenSource % 8 != 0))
		return FALSE;

	char szEcryptedChar[5];
	for(int i = 0; i < nLenSource; i += 4)
	{
		::memset(szEcryptedChar, 0x00, sizeof(szEcryptedChar));
		::memcpy(szEcryptedChar, szSource + i, 4);

		theApp.DecryptCharacterz(szEcryptedChar, szDestination);
	}

	return TRUE;
}
