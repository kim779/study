#pragma once
// HedgePage.h : header file
//

#include <Afxtempl.h>
#include "../../control/fx_misc/fxImgButton.h"

/////////////////////////////////////////////////////////////////////////////
// CHedgePage dialog

class CHedgePage : public CPropertyPage
{
	DECLARE_DYNCREATE(CHedgePage)

// Construction
public:
	CHedgePage(CWnd* pView = NULL, CWnd* pParent = NULL);
	~CHedgePage();

public:
	CIB410100App*		m_pApp = nullptr;
	CWnd*				m_pView = nullptr;
	CWnd*				m_pParent = nullptr;
	std::unique_ptr<class CfxGrid>		m_pGrid = nullptr;

	int		m_RadioIdx{};
	int		m_Grid_Height{};
	int		m_Grid_FSize{};
	CFont*		m_pFont = nullptr;
	COLORREF	m_FontColor{}, m_cFgColor{}, m_cBgColor{}, m_sFgColor{}, m_hFgColor{}, m_hBgColor{};
	COLORREF	m_bkColor{};
	CBrush		m_bkBr{};

	struct _HgridData {
		CString	jcod;			// 종목코드

		CString	jnam;			// 종목명
		int	jqty{};			// 수량, 매수(+), 매도(-)
		double	curr{};			// 현재가
		double	jprc{};			// 소요자금
		double	delt{};			// 델타
		double	gama{};			// 감마
		double	ceta{};			// 세타
		double	vega{};			// 베가
		double	gdan{};			// 2012.05.23 KSJ 거래승수 추가
	};
	CArray <_HgridData*, _HgridData*>		m_gridArray;
	CStringArray							m_codeArray;
	double	m_pHedgeTarget[5]{};	// 가격합, 델타, 감마, 세타, 베가
	CString	m_tData;

public:
	void	CalcHedge();
	void	CalcHedgeD();
	void	CalcHedgeP();
	void	AddRow();
	void	SetRow(int Row);
	void	OnRadio(int idx);
	void	InitProperties();
	void	InitGrid();
	void	DeleteGridArray(bool bData = true, bool bGrid = true);
	int	Round(double dVal);
	CString AddComma(CString strData);

// Dialog Data
	//{{AFX_DATA(CHedgePage)
	enum { IDD = IDD_TAB_HEDGE };
	CfxImgButton	m_btnOrder;
	CfxImgButton	m_btnCalc;
	CEdit	m_EEdit;
	CEdit	m_SEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHedgePage)
	protected:
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CHedgePage)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnHradio1();
	afx_msg void OnHradio2();
	afx_msg void OnDeltaposHspin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposHspin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHbtn1();
	afx_msg void OnHbtn2();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg long OnMessage(WPARAM wParam, LPARAM lParam);
	afx_msg long OnHedgeData(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

