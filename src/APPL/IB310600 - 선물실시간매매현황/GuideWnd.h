#pragma once
// GuideWnd.h : header file
//

#include "MapWnd.h"

/////////////////////////////////////////////////////////////////////////////
// CGuideWnd window

class CGuideWnd : public CWnd
{
// Construction
public:
	CGuideWnd(class CMapWnd *pParent);

// Attributes
public:

protected:
	class CMapWnd *m_pParent;

	CFont *m_pFont;

	std::unique_ptr<CfxImgButton>	m_pBtnSigned;	//주문체결내역버튼
	std::unique_ptr<CfxImgButton>	m_pBtnBalance;	//신용잔고 버튼
	std::unique_ptr<CfxImgButton>	m_pBtnReceiveAcc;	//예수금 현황 버튼
	std::unique_ptr<CfxImgButton>   m_pBtnAttention;	//주의사항 버튼

	HBITMAP		m_hBmpBtn4;			//4글자 버튼
	HBITMAP		m_hBmpBtn4_dn;
	HBITMAP		m_hBmpBtn4_hv;

	HBITMAP		m_hBmpBtn5;			//5글자 버튼
	HBITMAP		m_hBmpBtn5_dn;
	HBITMAP		m_hBmpBtn5_hv;

	HBITMAP		m_hBmpBtn9;			//9글자 버튼
	HBITMAP		m_hBmpBtn9_dn;
	HBITMAP		m_hBmpBtn9_hv;

	HBITMAP		m_hBmpGuide;
	HBITMAP		m_hBmpRoundGuide[4];

	CString		m_strGuide;
	CString		m_strPal;
	
// Operations
public:
	void SetGuide(CString strGuide);
	void SetBitmap();
	void SetPalette(CString strPal);

protected:
	void CreateControl();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGuideWnd)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGuideWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CGuideWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

