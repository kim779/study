#if !defined(AFX_PNSISE_H__AEE8185A_1A90_4951_87D8_BC3B583FAF0F__INCLUDED_)
#define AFX_PNSISE_H__AEE8185A_1A90_4951_87D8_BC3B583FAF0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PnSise.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPnSise window
#include "PnBaseWnd.h"
#include "afxtempl.h"

class	_comdisp
{
public:
	int		dispID;
	CString		dispStr;
	UINT		fstyle;
	CRect		drawRC;
	COLORREF	fgcolor;
	bool		bFlash;
	bool		bShow;
	int		timer;
};

class CPnSise : public CPnBaseWnd
{
// Construction
public:
	CPnSise(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPnSise)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPnSise();

	// Generated message map functions
protected:
	//{{AFX_MSG(CPnSise)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT	OnSiseEvent(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	void	changeUnit(int unit);
	void	ClearSise();
	int	ReceiveData(char* pcData, int iLen);
	void	realtime(char *pData);
	void	initComs();
	void	deleteComs();
	int	GetToken(char *pData, char token[]);

	COLORREF	parseSymbol(char sign);
	COLORREF	parseSymbol(char sign, CString& symbolS, CString& signS);
	CString		editComma(double dVal, int id);

private:
	bool		m_bCode;
	int		m_unit;

	int		m_fPoint;
	CString		m_fName;
	CFont		*m_pNMFont;
	CFont		*m_pBDFont;

	bool		m_bFlash;
	int		m_FlashIndex;

	CArray <_comdisp *, _comdisp *>	m_arDisp;

	enum {	valCODE = 0, valCURR, valDIFF, valUDYL, nameVOL, valVOL,
		nameMDHG, valMDHG, nameMSHG, valMSHG, //** macho add
		nameSIGA, valSIGA, nameKOGA, valKOGA, nameJEGA, valJEGA,
		nameMYGL, valMYGL};

	CString	m_name;		// 종목명
	CString	m_code;		// 실시간 코드
	CString	m_date;		// 영업일자
	CString	m_sahg;		// 상한가
	CString	m_hahg;		// 하한가
	CString	m_jjga;		// 전일종가
	CString	m_stock;	// 주식수
	CString	m_start;	// 장시작시간

	CString	m_curr;
	CString	m_udyl;

	CString	m_diffSym;
	CString	m_diffVal;

	CString m_volume;
private:
	//** macho add
	CString m_time;
	CRect   m_rcClient;
	CRect	m_rcTable;
	void	DrawSise(CDC& dc);
	COLORREF	GetColor(int index);
	int		GetTotalWidth();
	CRect	GetCellRect(int index, BOOL head = FALSE);

	COLORREF m_clrText, m_clrHeader, m_clrWindow, m_clrUp, m_clrDown, m_clrBorder, m_clrFlash;
	COLORREF GetPalette(int index);
	void	SetPaletteColor();
	
	_comdisp* FindDispValue(int id);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PNSISE_H__AEE8185A_1A90_4951_87D8_BC3B583FAF0F__INCLUDED_)
