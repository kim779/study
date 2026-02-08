#if !defined(AFX_FILEIO_H__5F9180CB_01AD_4EA2_A832_AD68DDF90EE4__INCLUDED_)
#define AFX_FILEIO_H__5F9180CB_01AD_4EA2_A832_AD68DDF90EE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class CFileIO  
{
public:
	CFileIO();
	virtual ~CFileIO();

public:
	static bool	ReadFile(CWnd *pView, CString path, char *pBytes, int nBytes);
	static bool	WriteFile(CWnd *pView, CString path, char *pBytes, int nBytes);
	static bool	GetFilenameInPath(CString &strPath, CString name, CStringArray &aryFname);
	static BOOL	ImageWindow(CWnd *pMain, CString& path);
	static void	PrintWindow(CWnd *pMain, bool bDlgPopup);
	static HANDLE	DDBToDIB(CBitmap& bitmap, DWORD dwCompression, CPalette* pPal);
	static BOOL	WriteDIB(LPTSTR szFile, HANDLE hDIB);
};

#endif // !defined(AFX_FILEIO_H__5F9180CB_01AD_4EA2_A832_AD68DDF90EE4__INCLUDED_)
