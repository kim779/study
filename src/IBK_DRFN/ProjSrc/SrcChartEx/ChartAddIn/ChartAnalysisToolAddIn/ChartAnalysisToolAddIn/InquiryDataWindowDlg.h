#if !defined(AFX_INQUIRYDATAWINDOWDLG_H__F277BA79_4A57_4A50_8AEE_BCB343492CB6__INCLUDED_)
#define AFX_INQUIRYDATAWINDOWDLG_H__F277BA79_4A57_4A50_8AEE_BCB343492CB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InquiryDataWindowDlg.h : header file
// 기본적인 수치조회창.

#include "NumericalInquiryDlg.h"
#include "Resource.h"
#include "InquiryDataListCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CInquiryDataWindowDlg dialog

class CInquiryDataWindowDlg : public CNumericalInquiryDlg
{
	// (2006/2/15 - Seung-Won, Bae) for checking Runtime Class ~ Not used yet in 2006/2/15
	DECLARE_DYNAMIC( CInquiryDataWindowDlg)

// Construction
public:
	CInquiryDataWindowDlg( IChartOCX *p_pIChartOCX, CWnd* pParent = NULL);   // standard constructor
	virtual ~CInquiryDataWindowDlg();

// Dialog Data
	//{{AFX_DATA(CInquiryDataWindowDlg)
	enum { IDD = IDD_INQUIRYDATAWINDOW };
	CInquiryDataListCtrl	m_listctrlInquiryData;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInquiryDataWindowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInquiryDataWindowDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnGetdispinfoListInquirydata(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual void SetInquiryData(CDC* pDC, CInquiryData *pInquiryData,CInquiryData *pInquiryPrevData,int nMaxDecimal = 0);
	virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE, CDC* pDC = NULL);
	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	virtual void SetDlgBrush(COLORREF clrDlgBrush);
	virtual void OnMouseMove(CPoint point);	

private:
	int SetColumnWidth(int nColIndex, CDC* pDC);	// 칼럼 너비 계산하여 조정.
	CString ReplaceTitle(const CString& strTitle);	// 타이틀명을 고친다.

private:
	CInquiryItem *	m_listData;
	bool			m_bCalculateSize;
// (2006/7/2 - Seung-Won, Bae) Using Virtual Draw. 0:Title, 1:Data
protected:
	int				m_nListDataSize;
	CStringArray	m_saListData[ 2];

// (2006/6/27 - Seung-Won, Bae) Manage Interfaces
protected:
	IChartOCX *		m_pIChartOCX;
	IChartManager *	m_pIChartManager;

// (2006/6/24 - Seung-Won, Bae) Manage the Numerical Inquiry Data Queryer with HorizontalLine Option.
protected:
	CNumericalInquiryDataQueryer	m_niDataQueryer;
	HWND	m_hOcxWnd;
public:
	void	EnableHorizontalLineInNIDLG( BOOL p_bEnable)	{	m_niDataQueryer.EnableHorizontalLineInNIDLG( p_bEnable);	}

protected:
	bool m_bUseNumericalInquiryDlg;		// 수치조회창 사용여부.
	bool m_bInitDialogMove;				// 수치조회창 이동 초기화여부.('고정'일경우 단 한번만 제위치 찾아주기 위함)
	bool m_bNumericalInquiryDlgPosLeft;	// 수치조회창의 위치(true: 왼쪽, false: 오른쪽)
	bool m_bCalcSizeInquiryDlg;			// 수치조회창 사이즈 계산여부.
	short m_nMoveNumericalInquiryDlg;	// 수치조회창 이동/고정/마우스따라다니기 여부.
	COLORREF m_clrInquiryDlg;			// 수치조회창 색상.
public:
	void	ShowNumericalInquiryDlg( const BOOL p_bShow);
	void	ShowNumericalInquiryDlg( CDC *p_pDC, const CPoint &p_ptPoint);
	void	OnMouseMove(CDC* pDC, CPoint point);
	bool	DestroyNumericalInquiryDlg( void);
	bool	CreateNumericalInquiryDlg( void);
	void	SetNumericalInquiryDlgBrush(COLORREF clrDlg);			// 조치창 색상.
	bool	IsUsingNumericalInquiryDlg();							// 사용여부.
	void	MoveNumericalInquiryDlg(CDC* pDC, CRect rectRegion, CPoint point);			// 이동.
	void	ResizeNumericalInquiryDlg(CDC* pDC, CRect* rectDlg);	// 데이타길이에 맞게 수치조회창 싸이즈 조절하기.
	void	OnNumericalInquiryOptionChanged( const long p_lNumericalInquiryOption);	// (2006/3/4 - Seung-Won, Bae) Notify Cursor Mode Change for Drag Cross Line & Numercial Inquiry DLG
	void	Refresh( void);

// (2008/10/30 - Seung-Won, Bae) for Inquiry Data Export.
protected:
	HWND	m_hExportWnd;
public:
	void	SetExportMode( HWND p_hExportWnd)	{	m_hExportWnd = p_hExportWnd;	}
	BOOL	IsExportMode( void)					{	return ( BOOL)m_hExportWnd;	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INQUIRYDATAWINDOWDLG_H__F277BA79_4A57_4A50_8AEE_BCB343492CB6__INCLUDED_)
