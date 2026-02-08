// SystemTradingDoc.h : interface of the CSystemTradingDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMTRADINGDOC_H__A5D92252_E3F6_471F_87A3_AF089B2CE3D1__INCLUDED_)
#define AFX_SYSTEMTRADINGDOC_H__A5D92252_E3F6_471F_87A3_AF089B2CE3D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CrystalTextBuffer.h"

class CScriptSecurity;
class CSystemTradingDoc : public CDocument
{
protected: // create from serialization only
	CSystemTradingDoc();
	DECLARE_DYNCREATE(CSystemTradingDoc)

// Attributes
public:
	class CSystemTradingTextBuffer : public CCrystalTextBuffer
	{
	private:
		CSystemTradingDoc *m_pOwnerDoc;
	public:
		CSystemTradingTextBuffer(CSystemTradingDoc *pDoc) 
		{ 
			m_pOwnerDoc = pDoc; 
		}
		virtual void SetModified(BOOL bModified = TRUE)
		{ 
			m_pOwnerDoc->SetModifiedFlag(bModified);
		}
	};

	CSystemTradingTextBuffer m_xTextBuffer;
	LOGFONT m_lf;

// Operations
public:
	UINT	GetParseFileType() {return m_nFileType;}
	LPCSTR	GetParseFileName() {return m_strPathName;}
	LPSTR	GetScriptName() {return m_szScriptName;}
	UINT	GetScriptType() {return m_nScriptType;}
	CScriptSecurity*	GetScriptSecurity() {return m_pScriptSec;}

	void	SetScriptName(LPCTSTR lpszPathName);
	void	ChangeCompiledOption(UINT nCompiled);
	void	SelectLine(UINT nLineNumber);
	void	SelectLine(UINT nLineNumber, UINT nColumn);
	void	ClearAllExecution();
	void	SetScriptType(BOOL bRedraw);
	void	SetSystemTradingFontLog();
	void	ApplyClientConfig();
	void	ProcessSaveFile(LPCSTR lpszFile);
	BOOL	SetScriptProperty(BOOL bRedraw, BOOL bUsePassword, CString strPassword, BOOL bUseUserID, CString strUserID, BOOL bUseExpires, CString strExpires);

	void	SaveFile();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemTradingDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	virtual void DeleteContents();
	virtual void SetTitle(LPCTSTR lpszTitle);
	protected:
	virtual BOOL SaveModified();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSystemTradingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT	m_nFileType;
	UINT	m_nScriptType;
	char	m_szScriptName[MAX_PATH];

	CScriptSecurity*	m_pScriptSec;

// Generated message map functions
protected:
	//{{AFX_MSG(CSystemTradingDoc)
	afx_msg void OnBuildCompile();
	afx_msg void OnBuildSingleCompile();
	afx_msg void OnUpdateBuildCompile(CCmdUI* pCmdUI);
	afx_msg void OnViewProperty();
	afx_msg void OnBuildCompileDll();
	afx_msg void OnUpdateBuildCompileDll(CCmdUI* pCmdUI);
	afx_msg void OnBuildSingleCompileDll();
	afx_msg void OnUpdateBuildSingleCompile(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBuildSingleCompileDll(CCmdUI* pCmdUI);
	afx_msg void OnBuildStop();
	afx_msg void OnUpdateBuildStop(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMTRADINGDOC_H__A5D92252_E3F6_471F_87A3_AF089B2CE3D1__INCLUDED_)
