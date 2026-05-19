// folderBrowser.h: interface for the CFolderBrowser class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FOLDERBROWSER_H__784D1412_7077_4F81_A007_9F42393747C2__INCLUDED_)
#define AFX_FOLDERBROWSER_H__784D1412_7077_4F81_A007_9F42393747C2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "shlobj.h"

class CFolderBrowser;

//------------------------------------------------------------------------------------------
//	CFolderBrowserSub
//	intercepts messages from child controls
//
class CFolderBrowserSub : public CWnd
{
	friend CFolderBrowser;
public:
	CFolderBrowser*	m_folder;

protected:
	//{{AFX_MSG(CFormat)
	afx_msg	void OnOK();		// OK button clicked
	afx_msg	void OnChangePath();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


//------------------------------------------------------------------------------------------
//	CFolderBrowser
//
class CFolderBrowser  
{
	friend	CFolderBrowserSub;

// Construction
public:
	CFolderBrowser(CString initPath = _T(""), CWnd* pParent = NULL);
	virtual ~CFolderBrowser();

	CString GetPathName();
	virtual	int	DoModal();

	static	int	Touch(CString path, bool validate = TRUE);
	static	int	MakeSurePathExists(CString path);
	static	bool	IsFileNameValid(CString files);

private:
	static	int	CALLBACK BrowseCallbackProc(HWND hwnd,UINT uMsg,LPARAM lParam, LPARAM pData);

	CString	m_caption;
	CString	m_title;
	CString	m_initPath;
	char	m_path[MAX_PATH];

	BROWSEINFO m_bi;
	HWND	m_hWnd;
	CWnd*	m_parent;
	bool	m_parentDisabled;
	bool	m_getSuccess;

	CFolderBrowserSub m_folderSub;

};

#endif // !defined(AFX_FOLDERBROWSER_H__784D1412_7077_4F81_A007_9F42393747C2__INCLUDED_)
