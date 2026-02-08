#include "stdafx.h"
#include "libctr.h"
#include "CX_AES.h"
#include "../../h/axisfire.h"
#include "Rijndael.h"

//48바이트 길이만 됨
__declspec(dllexport) char* APIENTRY axEncrypt(char* src, int srcL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//AfxEnableControlContainer();
	//AfxInitRichEdit( );
	CString s;
	
	s.Format("%s",src);
	
	if(s.Find("AM99") > -1)
	{
//		OutputDebugString("OK\n");
	}

	CRijndael oRjnd;
	oRjnd.MakeKey("abcdefghijklmno", CRijndael::sm_chain0, 16, 16);


	char szDataIn1[49];
	s.Format("AES INIT KEY [%d]\n",oRjnd.m_bKeyInit);
//	OutputDebugString(s);
	
	//strcpy(szDataIn1,src);
	memcpy(szDataIn1,src,sizeof(szDataIn1));
	s.Format("AES INIT KEY1 [%d]\n",oRjnd.m_bKeyInit);
	OutputDebugString(s);	

	char szDataIn[49];
	char szDataOut[49];

	memset(szDataIn, ' ', 49);
	memset(szDataOut, ' ', 49);


	oRjnd.ResetChain();
	strcpy(szDataIn,szDataIn1);
	memset(szDataOut,0,49);
	oRjnd.Encrypt(szDataIn,szDataOut,48,CRijndael::CBC);

	s.Format("CX_AES SRC [%s]  ENC [%s]\n",szDataIn,szDataOut);
 	OutputDebugString(s);

// 	CString t;
// 	t.Format("%s",szDataOut);
// 
// 	char* ct = (LPSTR)(LPCTSTR)t;
// 
// 	memcpy(szDataOut,ct,49);
// 
// 	oRjnd.ResetChain();
// 	memset(szDataIn,0,49);
// 	oRjnd.Decrypt(szDataOut,szDataIn,48,CRijndael::CBC);
// 
// 	s.Format("SRC [%s]  DEC [%s]\n",szDataOut,szDataIn);
//  	OutputDebugString(s);

// 	CString s;
// 	s.Format("SRC [%s] ENC [%s] DEC [%s]\n",szDataIn,szDataOut,szDataIn);
// 	OutputDebugString(s);
	char szHex[200]{};
	oRjnd.CharStr2HexStr((unsigned char*)szDataOut,szHex,48);

   return szHex;
}

__declspec(dllexport) char* APIENTRY axDecrypt(char* src)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString slog;
	slog.Format("[cx_aes] 1. axDecrypt [%s] \r\n", src);
	OutputDebugString(slog);

	//AfxEnableControlContainer();
	//AfxInitRichEdit( );

	CRijndael oRjnd;
	oRjnd.MakeKey("abcdefghijklmno", CRijndael::sm_chain0, 16, 16);

	slog.Format("[cx_aes] 2. axDecrypt \r\n");
	OutputDebugString(slog);

	char szDataIn[49] = "";
	char szDataOut[49];
	memset(szDataIn, ' ', 49);
	memset(szDataOut, ' ', 49);

	oRjnd.HexStr2CharStr(src,(unsigned char*)szDataIn,48);

	slog.Format("[cx_aes] 3. axDecrypt \r\n");
	OutputDebugString(slog);

	//memcpy(szDataIn,src,49);
	
	oRjnd.ResetChain();
	memset(szDataOut,0,49);
	oRjnd.Decrypt(szDataIn,szDataOut,48,CRijndael::CBC);

	slog.Format("[cx_aes] 4. axDecrypt [%s][%d]\r\n", szDataOut, strlen(szDataOut));
	OutputDebugString(slog);

	//slog.Format("SRC [%s]  DEC [%s]\n",szDataIn,szDataOut);
	//OutputDebugString(slog);
	char* pRet = new char[strlen(szDataOut) + 1];
	memset(pRet, 0x00, strlen(szDataOut) + 1);
	memcpy(pRet, szDataOut, strlen(szDataOut));

	slog.Format("[cx_aes] 5. axDecrypt [%s]\r\n", pRet);
	OutputDebugString(slog);

	return pRet;
//	return szDataOut;
}
