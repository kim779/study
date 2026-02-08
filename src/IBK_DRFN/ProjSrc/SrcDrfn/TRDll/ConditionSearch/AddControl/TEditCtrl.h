#if !defined(AFX_TEDITCTRL_H__CA5BB022_1277_11D2_843A_0000B43382FE__INCLUDED_)
#define AFX_TEDITCTRL_H__CA5BB022_1277_11D2_843A_0000B43382FE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TEditCtrl.h : header file
//

/************************************************************************/
/*																		*/
/*						TEdit(Dot, Mainers Control						*/
/*																		*/
/*																		*/
/*							작성일 : 2004. 11~							*/
/*						작성자 : 우동우 (두리 정보 통신)				*/
/*																		*/
/************************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CTEditCtrl window

class CTEditCtrl : public CEdit
{
	// Construction
public:
	CTEditCtrl();
	virtual ~CTEditCtrl();
	
	int m_nIntegerLength, m_nRealLength;
	BOOL Create(const RECT& rect, CWnd* pParentWnd, UINT nID, int nIntegerLength, int nRealLength);
	void SetInit(CString strText);
	void ValueWithComma(CString& strData);
	void IngnoreComma(BOOL bValue) { m_bIgnoreComma = bValue; }
	void SetTimeType(BOOL bTimeType) { m_bTimeType = bTimeType; }
	void SetBeforText(CString strText) { m_strBeforeText = strText; }
	BOOL SetTimeFormat();

private:
	COLORREF	m_clrTextBk;
	COLORREF	m_clrText;
	
	COLORREF	m_clrFace;
	COLORREF	m_clrTopLeft;
	COLORREF	m_clrBottomRight;
	BOOL		m_bIgnoreComma;
	BOOL		m_bTimeType;

private:
	BOOL		m_bEscapeKey;
	
	BOOL		m_bAllClear;
	
	BOOL           m_bDot;
	BOOL           m_bMainers;
	CStringArray   m_strEdit;
		
	BOOL		m_bFirstInput;

	CString     m_szText;
	CString		m_strBeforeText;

	void InsertComma(CString & str1);
	
public :
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTEditCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		
	// Generated message map functions
protected:
	//{{AFX_MSG(CTEditCtrl)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()

	CString GetComma(LPCSTR pData);
	CString GetNumber( LPCSTR pData );
	char* RemoveFront(LPCSTR pData, TCHAR ch);

public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEDITCTRL_H__CA5BB022_1277_11D2_843A_0000B43382FE__INCLUDED_)
