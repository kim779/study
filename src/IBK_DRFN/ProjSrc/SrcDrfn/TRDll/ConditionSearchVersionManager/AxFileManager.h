////////////////////////////////////////////////////////////////////////////////
// 01. 작  성  자      : {이준옥}
// 02. 작 성 일 자     : {1998.10.03 ~ }
// 03. 소       속     : {(주)두리정보통신}
////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_AXFILEMANAGER_H__A21F8422_3207_11D2_AE73_00A0C98C4FD2__INCLUDED_)
#define AFX_AXFILEMANAGER_H__A21F8422_3207_11D2_AE73_00A0C98C4FD2__INCLUDED_

// #include "AxFileManager.h"
#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <direct.h>
//#include <Lzexpand.h>

//#pragma comment(lib,"lz32.lib") 
//#pragma message("Automatically linking with lz32.dll") 
#include ".\Zip\ZipArchive.h"

class CAxFileManager
{
// Variable
public:
	CAxFileManager() {m_bFileMove = TRUE;};

	BOOL			m_bFileMove;
	CZipArchive		m_zip;
	CString			m_szDefaultPath;
	CString			m_szCommonDir;

// Implementation
public:	
	CString GetErrorMessage() { return m_szLastMsg;}
//	int		lzCopy(LPSTR pszFromFile, LPSTR pszToFile);
//	int		lzMove(LPSTR pszFromFile, LPSTR pszToFile);
	int		zipCopy(LPSTR pszFromFile, LPSTR pszToFile);
	int		ExtractZipFile(LPCSTR szFromFile, LPCSTR szToDir);
	void	Open(LPCTSTR szFile);
	void	Extract(LPCTSTR szTargetDir);
	void	Close();
	BOOL	GetDirectory(LPSTR pszSrc, LPSTR pszDes);
	BOOL	GetFileName(LPSTR pszSrc, LPSTR pszDes);
	BOOL	FreeAnyModule(LPSTR LibFile);
	BOOL	MakeDirectory(LPSTR pszSrc);
	BOOL	RemoveDirectory(LPSTR pszSrc);
	BOOL	CreateSubDirectory(const CString& baseDir, const CString& strPath);
	void	Exec(LPCSTR pszExec, int showCmd=SW_SHOW) ;
	BOOL	FileCheck(LPCSTR szPath);	// 파일 존재여부 체크
	int		DLLRegister(LPCSTR szFileName);
	BOOL	FileCopy(LPSTR pszSrc, LPSTR pszDes);
	BOOL	FileMove(LPSTR pszSrc, LPSTR pszDes);
	BOOL	FileRemove(LPSTR pszSrc, BOOL bChangeAttr=TRUE);
	int		DirFileRemove(LPCSTR pszDir, LPCSTR pszExt="*.*", BOOL bSub=TRUE, BOOL _bDirRemove=FALSE);

	int		ReadFile2Data(LPCSTR pszFileName, CStringList& szData);
	int		DirectoryMove(LPCSTR pszFromDir, LPCSTR pszToDir, LPCSTR pszExt="*.*", BOOL bSub=TRUE);
	int		DirectoryIfReMove(LPCSTR aTarDir,LPCSTR pszExt="*.*", BOOL bSub=TRUE);
	BOOL	CheckDirectory(LPCSTR szDir);

private:
	CString	 m_szLastMsg;
};

#endif //AFX_AXFILEMANAGER_H__A21F8422_3207_11D2_AE73_00A0C98C4FD2__INCLUDED_
