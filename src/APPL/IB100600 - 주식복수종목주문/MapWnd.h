#pragma once
// MapWnd.h : header file
//

#include "ExpandWnd.h"
#include "ModCanWnd.h"
#include "OrderWnd.h"
#include "WarningWnd.h"
#include "../../control/fx_misc/fxTab.h"
#include "../../control/fx_misc/fxStatic.h"
#include "../../control/fx_misc/fxImgButton.h"

/////////////////////////////////////////////////////////////////////////////
// CMapWnd window


class CMapWnd : public CWnd
{
// Construction
public:
	CMapWnd(CWnd* pParent = NULL);

// Attributes
public:

// Operations
public:
	void Procedure(CString strProc);
	void DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMapWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	std::unique_ptr<CModCanWnd>	m_pModCanWnd;
	std::unique_ptr<COrderWnd>	m_pOrderWnd;
	CWnd*		m_pCmdWnd;
	void CreateModCanWnd();
	inline CFont* GetNormalFont() { return m_pNormalFont; };
	inline CFont* GetDefNormalFont() { return m_pDefNormalFont; };
	inline CFont* GetBoldFont() { return m_pBoldFont; };
	inline CString GetAxRoot() { return m_strRoot; };
	CBitmap* GetAxBitmap(CString strPath);
	COLORREF GetIndexColor(UINT nIndex);
	CString Variant(int nComm, CString strData = _T(""));
	CFont* GetAxFont(CString strName, int nPt, bool bItalic, int nBold);
	void Init();
	BOOL SendTR(CString strTR, CString strData, int nKey, int nStat);
	void SetGuide(CString strMsg);
	virtual ~CMapWnd();
	void		SaveBuffer(CString sKey, CString sData);
	CString		GetBuffer(CString sKey);
	void	LoadRoundBitmap();

	BOOL CheckPassWord(CString sAcc, CString sPass, CWnd* pCmdWnd) ;
	CString	GetEncPassword(CString sPswd);	//2013.13.23 KSJ 일방향암호화


	// Generated message map functions
protected:
	//{{AFX_MSG(CMapWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTabChange(NMHDR* pHdr, LRESULT* result);
	afx_msg void OnExpand();
	DECLARE_MESSAGE_MAP()

	void ShowHideBodyWnd(int nIndex);
	void SetPallete();
	int GetMajor();
	int GetMinor();
	CString Parser(CString &srcstr, CString substr = _T(""));
	void ChangeFont(int ifontsixze, LPCTSTR lszFont);
	void ChangeSelectTab(int itab);
	
	void DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap);


private:
	CWnd* m_pParent;
	CString m_strRoot;
	CFont* m_pNormalFont, *m_pDefNormalFont;
	CFont* m_pBoldFont;
	std::unique_ptr<CExpandWnd>	m_pExpandWnd;
	std::unique_ptr<CWarningWnd> m_pWarnWnd;
	std::unique_ptr<CfxStatic>	m_pGuide;

	std::unique_ptr<CfxImgButton> m_pExpand;

	std::unique_ptr<CfxTab>	m_pTab;
	HBITMAP				m_hRoundBitmap[10], m_hGuideBitmap;

	BOOL				m_bExpand, m_bInit, m_bChangeFont, m_bExpendClick;
	LPCTSTR				m_lszFontName;
	int					m_iCurFont, m_iOldFont, m_iDefaultFont, m_iDefaultSize[2], m_iOldSize[2];
	int					m_iWndWidth, m_iWndHeight, m_iExpendWidth, m_iDefExpendHeight, m_iDefExpendWidth;
	double				m_lSizeRate[2], m_lcalcRate[2];
	HINSTANCE			m_hiSha256;	//2013.12.23 KSJ 일방향암호화

};

