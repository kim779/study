#pragma once
// OptionListCtrl.h : header file
//

#include "OHeaderCtrl.h"

#define WM_SELECT_OPTION		WM_USER+12

/////////////////////////////////////////////////////////////////////////////
// COptionListCtrl window

struct	code15	{			/* 상품옵션코드 정보		*/
	char    codx[8];   		/* 단축코드			*/
	char    hnam[50];  		/* 한글명			*/
	char	enam[50];		/* 영문명			*/

	char	jcod[12];		/* 기초자산종목코드		*/
	char    jnam[40];  		/* 거래대상 종목명		*/
	char    mont[4];   		/* 만기년월			*/
	char    atmx;      		/* ATM종목 여부(1: ATM)		*/
	char    hsga[12];   		/* 행사가격			*/
	char    tjgb[2];   		/* 거래대상 종목구분		*/
	char	mchk;			/* 매매유형허용			*/
							/* 0x01:시장가허용		*/
							/* 0x02:조건부지정가허용	*/
							/* 0x04:최유리지정가허용	*/
	char	fill[20];		/* filler			*/
};

class COptionListCtrl : public CListCtrl
{
// Construction
public:
	COptionListCtrl();

// Attributes
public:

	int InsertColumn( int nCol, LPCTSTR lpszColumnHeading, int type,
		int nFormat = LVCFMT_LEFT, int nWidth = -1, int nSubItem = -1 );

// Operations
public:

	void MakeImageList( void );

	void DrawItem(LPDRAWITEMSTRUCT lp );	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptionListCtrl)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COptionListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(COptionListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void MeasureItem ( LPMEASUREITEMSTRUCT lpMeasureItemStruct );	
	afx_msg LRESULT OnSetFont(WPARAM wParam, LPARAM);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	CBitmap		m_bitmapO;
	CBitmap		m_bitmapX;
	CBitmap		m_bitmapSO;

	COHeaderCtrl m_header;
	CImageList m_imageList;

	DECLARE_MESSAGE_MAP()
};

