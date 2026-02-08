#if !defined(AFX_TOP10DIALOG_H__A0682BD1_8C75_4B3B_A452_2958E25D1DB6__INCLUDED_)
#define AFX_TOP10DIALOG_H__A0682BD1_8C75_4B3B_A452_2958E25D1DB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TOP10Dialog.h : header file
//
#include "BmpButton.h"
/////////////////////////////////////////////////////////////////////////////
// CTOP10Dialog dialog
#define WD_TOP10			(WM_USER+0x8001)

class CTOP10Dialog : public CDialog
{
// Construction
public:
	CTOP10Dialog(CWnd* pParent = NULL);   // standard constructor

public:
	bool	m_bShown;
	bool	m_bAlways;

public:
	void SetBaseRect(CRect rect);
	void SetData(const CStringArray& arr, const CStringArray& arcodes);
	CBmpButton* CreateBmpButton(UINT buttonID, const char* bmpFileName, BOOL check);
	BOOL IsData();
	void SetCurrent(int index);
	void CreateBoldFont(CFont& font, int point, BOOL bUnderline,const char* fontName);
// Dialog Data
	//{{AFX_DATA(CTOP10Dialog)
	enum { IDD = IDD_TOP10 };
	CStatic	m_static;
	CString	m_dateStatic;
	//}}AFX_DATA

private:
	HBITMAP		SetBitMap(CString name);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTOP10Dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CRect m_BaseRect;
	HBITMAP m_bitmap;
	CStringArray m_arData;
	CStringArray m_arCodes;
	CStringArray m_rateColor;
	CStringArray m_arNew;
	CMapStringToString m_mapData;
	CFont	m_font,m_selfont,m_ufont;
	CBrush  m_brush;
	CWnd*	m_pParent;
	BOOL m_bTracking;
	BOOL m_bOverBtn;
	CString m_sDate;
	CBmpButton* m_checkBtn;
	BOOL	m_bMouseIn;


	int m_nPrevious, m_nCurrent;
	//CArray <CBmpButton*, CBmpButton*> m_bmpButtons;
	CArray <CBmpButton*, CBmpButton*> m_updownButtons;
	//int m_nPrevious,m_nCurrent;  //vc2019 m_nCurrent <-- 프로그램이 죽는다
	// Generated message map functions
	//{{AFX_MSG(CTOP10Dialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButtonL1();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnButtonL2();
	afx_msg void OnButtonL3();
	afx_msg void OnButtonL4();
	afx_msg void OnButtonL5();
	afx_msg void OnButtonL6();
	afx_msg void OnButtonL7();
	afx_msg void OnButtonL8();
	afx_msg void OnButtonL9();
	afx_msg void OnButtonL10();
	afx_msg void OnButtonHelp();
	afx_msg void OnButtonClose();
	afx_msg void OnButtonL11();
	afx_msg void OnButtonL12();
	afx_msg void OnButtonL13();
	afx_msg void OnButtonL14();
	afx_msg void OnButtonL15();
	afx_msg void OnButtonL16();
	afx_msg void OnButtonL17();
	afx_msg void OnButtonL18();
	afx_msg void OnButtonL19();
	afx_msg void OnButtonL20();
	afx_msg void OnButtonL21();
	afx_msg void OnButtonL22();
	afx_msg void OnButtonL23();
	afx_msg void OnButtonL24();
	afx_msg void OnButtonL25();
	afx_msg void OnButtonL26();
	afx_msg void OnButtonL27();
	afx_msg void OnButtonL28();
	afx_msg void OnButtonL29();
	afx_msg void OnButtonL30();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOP10DIALOG_H__A0682BD1_8C75_4B3B_A452_2958E25D1DB6__INCLUDED_)
