#include "stdafx.h"
#include "libctr.h"
#include "MainWnd.h"
#include "CX_SHA256.h"
#include "../../h/axisfire.h"
#include "Sha256.h"

__declspec(dllexport) char* APIENTRY axEncrypt(char* src, int srcL)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//AfxEnableControlContainer();
	//AfxInitRichEdit( );
	CSha256 sha256;

	Sha256_Init(&sha256);
	
	//char* str = "1234";
	
	Byte *pByte;

	pByte = (Byte*)(LPSTR)(LPCTSTR)src;
	Sha256_Update(&sha256,pByte,_tcslen(src));
	
	Byte btResult[32] = {0,};

	Sha256_Final(&sha256,btResult);

	char* base64encode = NULL;

	int base64_size = base64_encode((char*)btResult,32,&base64encode);

	CString encode(base64encode);

	CString s;
	s.Format("SHA 256 ENCODE : -> %s\n",base64encode);
	OutputDebugString(s);
	
	return base64encode;
}

__declspec(dllexport) char* APIENTRY axFileEncrypt(char* src)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	//AfxEnableControlContainer();
	//AfxInitRichEdit( );
	CSha256 sha256;
	
	Sha256_Init(&sha256);
	
	//char* str = "1234";
	FILE* pFile = ::fopen(src,"rb");

	CString s;
// 	s.Format("SHA256 FILE : [%s]\n",src);
// 	OutputDebugString(s);

	if(pFile == NULL)
	{
		s.Format("File cannot be opened!, %s\n", src);
		OutputDebugString(s);
		
		return NULL;
	}

	const size_t tBuffSize = 4 * 1024;

	Byte btBuff[4096] = {0,};
	
	while(feof(pFile) == 0)	
	{
		size_t tReadByte = ::fread(btBuff, sizeof(Byte), sizeof(btBuff), pFile);
		
// 		s.Format("File read size [%u]\n", tReadByte);
// 		OutputDebugString(s);
		
		Sha256_Update(&sha256, btBuff, tReadByte);	
	}
	
	::fclose(pFile);

	Byte btResult[32] = {0,};
	
	Sha256_Final(&sha256,btResult);
	
	char* base64encode = NULL;
	
	int base64_size = base64_encode((char*)btResult,32,&base64encode);
	
	CString encode(base64encode);
	
// 	s.Format("SHA 256 ENCODE : %s\n [%s] -> [%s]\n",src,btResult,base64encode);
// 	OutputDebugString(s);
	
	return base64encode;
}