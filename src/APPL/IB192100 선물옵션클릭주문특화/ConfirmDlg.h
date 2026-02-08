#pragma once

#include "AxisExt.h"
#include "dataio.h"
#include "../../control/fx_misc/fxImgButton.h"

#define IDC_BTN_YES		(1001)
#define IDC_BTN_NO		(1002)

class CConfirmDlg : public CDialog, public CAxisExt
{
// Construction
public:
	CConfirmDlg(CWnd* pParent, CWnd* pWizard);   // standard constructor
	CString GetCdgbName();

public:
	int m_odgb=0, m_old_oprc=0, m_oprc=0, m_oqty=0, m_acno_qty=0, m_cdgb=0, m_ojno=0, m_jqty=0;
	CString m_code, m_cdnm;

protected:
	CRect m_rcClient;
	CBitmap *m_hRndBmp[4], *m_hBtn2[3];
	std::shared_ptr<CfxImgButton> m_pBtnYes{}, m_pBtnNo{};

protected:
	void CalcSize();
//	CfxImgButton* CreateImgButton(LPCSTR caption, int nID, CBitmap** bmp);
	std::shared_ptr<CfxImgButton> CreateImgButton(LPCSTR caption, int nID, CBitmap** bmp);
	void DrawRoundRectangle(CDC *pDC, CRect rc);
	void DrawBitmap(CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawBitmap(CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol=0xFF00FF);
	void DrawSellOrder(CDC *pDC, CRect *pRC);
	void DrawBuyOrder(CDC *pDC, CRect *pRC);
	void DrawModifyOrder(CDC *pDC, CRect *pRC);
	void DrawCancelOrder(CDC *pDC, CRect *pRC);
	void DrawLabel(CDC *pDC, CRect *pRC, CString msg, UINT format, COLORREF crText, COLORREF crBk, COLORREF crLine);
	void DrawLine(CDC *pDC, CRect *pRC, CString header, CString text, int iSplit, COLORREF crText, COLORREF crHeadBk, COLORREF crTextBk, COLORREF crLine);

// Dialog Data
	//{{AFX_DATA(CConfirmDlg)
	enum { IDD = IDD_CONFIRM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfirmDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfirmDlg)
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnNoClick();
	afx_msg void OnYesClick();
};

