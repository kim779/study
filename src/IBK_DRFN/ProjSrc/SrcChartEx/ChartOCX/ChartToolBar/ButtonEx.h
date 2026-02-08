#if !defined(AFX_BUTTONEX_H__25280FC2_5348_4128_9D02_4829CDFC5EB0__INCLUDED_)
#define AFX_BUTTONEX_H__25280FC2_5348_4128_9D02_4829CDFC5EB0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButtonEx.h : header file
// 툴바에 등록하는 버튼클래스. 
// (비트맵이미지를 로드하여 Draw, 툴팁에 텍스트 세팅해서 보여주기가 구현됨)

#include "ToolOption.h"

/////////////////////////////////////////////////////////////////////////////
// CButtonEx window
class CObjTool;
class CToolBarDlg;

class CButtonEx : public CButton
{
// Construction
public:
	CButtonEx();

// Attributes
protected:
	CToolBarDlg* m_pParent;
	CToolOptionInfo::TOOLOPTION m_enumToolOption;
	CString m_strOption1;
	CString m_strOption2;
	CString m_strParam;
	bool	m_bState;	// 토글버튼은 체크박스로 구현되어야 하나 여기선 버튼클래스로 구현했다. 
						// 토글흉내내기 위한 변수.
private:
	CToolTipCtrl m_ToolTip;
	CString m_strToolTipText;

// Operations
public:
	CToolOptionInfo::TOOLOPTION GetToolOptionInfo();

	void SetParentWnd(CToolBarDlg* pParent){ m_pParent = pParent;};
	void SetToolObject(CObjTool* pObjTool, const CString& strParam = _T(""));	// ToolButton 정보 오브젝트를 세팅한다.
	void SetToolTipText(const CString& strText);	// 툴팁 문자열을 세팅한다.
	void SetState(bool bState = true);	// 버튼 상태를 설정한다. true-누름, false-안누름.
	bool GetState();	// 버튼의 상태를 리턴한다.
	const CString& GetOption1();	// 버튼의 첫번째 옵션값 리턴
	const CString& GetOption2();	// 버튼의 두번째 옵션값 리턴
	const CString& GetParam();		// 버튼의 파라미터값 리턴.

private:
	void AddToolInTooltip(const CString& strText, const bool bActivate);
	void SetToolTipText(const CString& strText, const bool bActivate);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonEx)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonEx)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONEX_H__25280FC2_5348_4128_9D02_4829CDFC5EB0__INCLUDED_)
