// MyEdit.h: interface for the CMyEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VBSCRIPTEDIT_H__CB7CC424_D971_420C_BA02_D9ABA5BDDB9D__INCLUDED_)
#define AFX_VBSCRIPTEDIT_H__CB7CC424_D971_420C_BA02_D9ABA5BDDB9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CCrystalEditView.h"
#include "CCrystalTextBuffer.h"
#include "../../awCommon/FormItem.h"

#include "../../AutoCmplDlg.h"
#include "../../AutoToolTip.h"

class CVBScriptEdit : public CCrystalEditView  
{
public:
	CCrystalTextBuffer	m_buf;
	LOGFONT			m_lf;

	CMap	<int, int, CString, CString>	m_AutoListMap;
	CMapStringToPtr	m_publicStrMap, m_ScriptStrMap;

	CString		m_sControlName;
	
	COLORREF	m_clBkGnd, m_clText;
	COLORREF	m_clSymbol;
	COLORREF	m_clComment;
	COLORREF	m_clConstant;
	COLORREF	m_clKeyword;
	COLORREF	m_clString;
protected:
	struct _mapH *m_mapH;

	CAutoCmplDlg	*m_pAutoDlg;
	CAutoToolTip	*m_pAutoTip;
private:
	BOOL		m_bEmpty;
	bool		m_bPythonMode;

public:
	void SetPythonMode(bool bPython);
	void SetSel(CPoint startPnt, CPoint endPnt);
	void LoadColors();
	COLORREF GetColor(int nColorIndex);
	void ProcessEmpty();
	BOOL OnComment();
	void showTip(CString sKey);
	CString GetControlInfo(CString str);
	void showList(int kind);
	int IsSymbol(CString &sym);
	void hidePopup();
	int GetLineCnt();
	CString GetLineString(int nLine);
	void Initialize(_mapH *mapH);
	void Initialize();
	CString Parser(CString &srcstr, CString substr);
	CString GetFrontWord(LPCTSTR pszChars, int nPos);
	void SetScriptText(CString sText);
	CString GetScriptText();
	int IsVBSNumber(LPCTSTR pszChars, int nLength);
	int IsVBSKeyword(LPCTSTR pszChars, int nLength);
	virtual DWORD ParseLine(DWORD dwCookie, int nLineIndex, TEXTBLOCK *pBuf, int &nActualItems);
	void PostNcDestroy();
	virtual CCrystalTextBuffer *LocateTextBuffer();
	CVBScriptEdit();
	virtual ~CVBScriptEdit();

	CString	m_strKeywords, m_strKeywordsLower;
	CString	m_strConstants, m_strConstantsLower;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCrystalEditView)
	public:
	virtual BOOL PreTranslateMessage(MSG *pMsg);
	//}}AFX_VIRTUAL
protected:
	BOOL IsInSymbol(CString sKey);
	CString GetVtType(VARTYPE var);
	CString GetTypeInfoString(ITypeInfo *pTypeInfo, TYPEATTR *pTypeAttr);
	CString GetBaseControlInfo(int kind);
	// Generated message map functions
	//{{AFX_MSG(CVBScriptEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnEditTab();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_VBSCRIPTEDIT_H__CB7CC424_D971_420C_BA02_D9ABA5BDDB9D__INCLUDED_)
