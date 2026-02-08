#if !defined(AFX_TEXTEDIT_H__E8501D91_CBB8_4F21_93A0_6F26742AA396__INCLUDED_)
#define AFX_TEXTEDIT_H__E8501D91_CBB8_4F21_93A0_6F26742AA396__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TextEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTextEdit window
class CTextTool;
class CDrawingSymbol;
class CTextEdit : public CEdit
{
// Construction
public:
	CTextEdit(const COLORREF &color);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTextEdit)
	public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTextEdit();

	void SetTextToolObject(CTextTool *pTextTool);
	void SetDrawingRegion(const CRect& drawingRegion);

	void	ResetWindowPos(CString strText="", BOOL bReset=FALSE);

	// Generated message map functions
protected:
	//{{AFX_MSG(CTextEdit)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnNcDestroy();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
private:
	CString m_strInputText;
	//enter의 횟수
	int m_nEnter;
	//블럭영역
	CRect m_DrawingRegion;
	//글자 하나의 크기
	CSize m_oneTextSize;
	//글자의 색
	COLORREF m_Color;
	//tool의 포인터
	CTextTool* m_pTextTool;

	//edit의 영역 얻기
	CRect GetRect();
	//text의 크기 얻기
	CSize GetTextSize(const CString& text);	
	//multiline일때 가장 긴 Text의 크기를 얻어오기.
	CString GetExtremeLongLengthofText(const CString& text);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEXTEDIT_H__E8501D91_CBB8_4F21_93A0_6F26742AA396__INCLUDED_)
