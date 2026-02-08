// SavedHead.h: interface for the CSavedHead class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SAVEDHEAD_H__F6B22AAD_3386_48A4_9E34_7A9DC7472A78__INCLUDED_)
#define AFX_SAVEDHEAD_H__F6B22AAD_3386_48A4_9E34_7A9DC7472A78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//----------------------------------------------------------------------------------
//
//	Head for all saved file (저장되는 모든 파일에 대한 Head)
//
//----------------------------------------------------------------------------------
struct _savedHead {
	DWORD	dwVersion;		// verstion
	DWORD	dwTotalLength;		// = SZ_SAVEDHEAD + dwDataLength
	DWORD	dwDataLength;		// = dwDataSize * dwDataCnt
	DWORD	dwOneDataSize;		// = one data size
	DWORD	dwDataCnt;		// = data count
	char	acDate[8];		// YYYYMMDD
	char	acTime[6];		// HHMMSS
	char	acFileName[16];		// file name
	char	acReserved[78];
};
#define SZ_SAVEDHEAD	sizeof(struct _savedHead)	// = always 128

class CSavedHead  
{
public:
	CSavedHead();
	virtual ~CSavedHead();

public:
	struct _savedHead	m_sh;

private:
	char*	m_pcBuf;

public:

	char*	Read(CString strFullPathFileName, DWORD& dwReadLen);
	char*	Make(DWORD dwVerstion, DWORD dwDataCount, DWORD dwOneDataSize);
//	bool	Write(CString strFullPathFileName, DWORD dwVerstion, DWORD dwDataCount, DWORD dwOneDataSize, char* pcBuf);
	bool	Write(CString strFullPathFileName);

private:
//	struct _savedHead*	Make(DWORD dwVerstion, DWORD dwDataCount, DWORD dwOneDataSize, char* pcBuf = NULL);
	char*	Extract(char* pcBuf);

	char*	ReadFile(CString strFullPathFileName, DWORD& dwReadLen);
//	bool	ReadFile(CString strFullPathFileName, DWORD dwBufLen, char* pcBuf);
	bool	WriteFile(CString strFullPathFileName, void* pvBuf, DWORD dwWriteLen);


};

#endif // !defined(AFX_SAVEDHEAD_H__F6B22AAD_3386_48A4_9E34_7A9DC7472A78__INCLUDED_)
