#pragma once
// OptionWnd.h : header file
//

#include "ColorButton.h"
#include "BkUtil.h"
#include "shareMsg.h"

/////////////////////////////////////////////////////////////////////////////
// COptionWnd window

class COptionWnd : public CWnd, public CBkUtil
{
// Construction
public:
	COptionWnd(CMapDlg *pParent);

// Attributes
public:
	class CMapDlg		*m_pMapDlg;

	CFont			*m_pFont;
	
	CString			m_sRoot;
	CString			m_sUser;
	CString			m_sFile;
	CString			m_sSection;

	// 색상
	COLORREF		m_clrHanNews;			
	COLORREF		m_clrEDaily;
	COLORREF		m_clrInfoStock;
	COLORREF		m_clrMoneyToday;
	COLORREF		m_clrYounhap;			
	COLORREF		m_clrMaekyoung;
	COLORREF		m_clrEToday;
	COLORREF		m_clrNewsp;

	// 본문조회
	BOOL			m_bContextSearch;	// TRUE : 본문조회 포함, FALSE : 제목만 조회
	
	// Color Picker
	CColorButton		*m_pHanNews_Picker;
	CColorButton		*m_pEDaily_Picker;
	CColorButton		*m_pInfoStock_Picker;
	CColorButton		*m_pMoneytoday_Picker;
	CColorButton		*m_pYounhap_Picker;
	CColorButton		*m_pMaekyoung_Picker;
	CColorButton		*m_pEToday_Picker;
	CColorButton		*m_pNewsp_Picker;

	CStatic			*m_pHanNew_Static;
	CStatic			*m_pEDaily_Static;
	CStatic			*m_pInfoStock_Static;
	CStatic			*m_pMoneytoday_Static;
	CStatic			*m_pYounhap_Static;
	CStatic			*m_pMaekyoung_Static;
	CStatic			*m_pEToday_Static;
	CStatic			*m_pNewsp_Static;

	CRect			m_rcHanNesw_Letter;
	CRect			m_rcEDaily_Letter;
	CRect			m_rcInfoStock_Letter;
	CRect			m_rcMoneyStock_Letter;
	CRect			m_rcYounhap_Letter;
	CRect			m_rcMaekyoung_Letter;
	CRect			m_rcEToday_Letter;
	CRect			m_rcNewsp_Letter;


	CButton			*m_pContext_Check;
	BOOL			m_bDaetoo;
// Operations
private:
	// ini File
	COLORREF		GetColorData(idPicker idContent);			// Read
	void			SetColorData(COLORREF color, idPicker idContent);	// Write
	void			LoadCheckContext();
	void			SaveCheckContext();
	
public:
	void			Init(CString root, CString user);	
	BOOL			GetCheckContext();			// Get 본문조회 체크
	COLORREF		GetNewsColor(idPicker idContent);	// Get 뉴스별 색상
	void			SetGubn(BOOL bDaetoo) { m_bDaetoo = bDaetoo; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionWnd)
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LONG OnSelEndOK(UINT lParam, LONG wParam);
	DECLARE_MESSAGE_MAP()
};

