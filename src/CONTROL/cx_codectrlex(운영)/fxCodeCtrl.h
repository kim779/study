#pragma once
// fxCodeCtrl.h : header file
//


#include <afxtempl.h>
#include "codelist.h"
#include "ddedit.h"

#define GU_NONE		-1
#define	GU_CODE		0	// 주식 
#define	GU_FUTURE	1	// 선물 
#define	GU_OPTION	2	// 옵션 
#define	GU_INDEX	3	// 업종 
#define	GU_FOCODE	4	// 선물  + 옵션
#define	GU_POPTION	5	// 주식옵션 
#define	GU_ELWCODE	6	// ELW 
#define GU_SCODE	7	// 신주인수권 
#define GU_BOND		8	// 채권
#define GU_SBOND	9	// 소매채권
#define GU_FCODE	10	// 주식선물 
#define GU_FOSTOCK	11	// 주식선물 + 옵션 
#define GU_GOODFO	12	// 상품선물

#define	sz_JCODE	6
#define	sz_FCODE	6
#define	sz_OPTION	8
#define	sz_INDEX	3
#define	sz_FOCODE	8
#define sz_ELW		6

#define SFCODE_HISTORY	"sfcode_history.dat"
#define FOREIGN_HISTORY	"gscode_history.dat"

/////////////////////////////////////////////////////////////////////////////
// QuickSort
template <class T, class PT> class CQArray : public CArray <T, PT>
{
public:
	void QuickSort(BOOL bAscending = TRUE);
};

template <class T, class TP> void CQArray<T,TP>::QuickSort(BOOL bAscending/* = TRUE*/)
{
	if (GetSize() > 1)
	{
		::QuickSort(GetData(), GetSize(), bAscending);
	}
}

template <class T> BOOL QuickSort(T *pArr, int iSize, BOOL bAscending = TRUE)
{
	BOOL rc = TRUE;

	if (iSize > 1) 
	{
		try 
		{
			const	int	low = 0, high = iSize - 1;

			QuickSortRecursive(pArr, low, high, bAscending);

		} 
		catch (...) 
		{
			::SetLastError(ERROR_INVALID_PARAMETER);
			rc = FALSE;
		}

	} 
	else 
	{
		::SetLastError(ERROR_INVALID_PARAMETER);
		rc = FALSE;
	}

	return rc;
}


template <class T> void QuickSortRecursive(T *pArr, int low, int high, BOOL bAscending)
{
	T	str;
	T	TTemp;
	int	ii = high;
	int	jj = low;

	str = pArr[((int) ((low+high) / 2))];
	do {
		if (bAscending) 
		{
			while (pArr[jj].Name < str.Name) jj++;
			while (pArr[ii].Name > str.Name) ii--;
		} 
		else 
		{
			while (pArr[jj].Name > str.Name) jj++;
			while (pArr[ii].Name < str.Name) ii--;
		}

		if (ii >= jj) 
		{
			if (ii != jj) 
			{
				TTemp = pArr[ii];
				pArr[ii] = pArr[jj];
				pArr[jj] = TTemp;
			}

			ii--;
			jj++;
		}
	} while (jj <= ii);

	if (low < ii) QuickSortRecursive(pArr, low, ii, bAscending);
	if (jj < high) QuickSortRecursive(pArr, jj, high, bAscending);
}

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo window

class CCodeCombo : public CWnd
{
// Construction
public:
	CCodeCombo();

	enum { DM_NONE, DM_HISTORY, DM_CODELIST};
	struct _JCode
	{
		CString Code;
		CString Name;
	};

	struct	_USCode			// 해외 SYMBOL 정보
	{
		char	symb[16];	// 종목 SYMBOL
		char	dat1;		// 대분류
					// 0: 지수      1: 종목
					// 2: 선물      3: 금리
					// 4: 국채      5: 환율
					// 7: others
		char	dat2;		// 중분류(Combo Box)
					// code09.jsgb
					// [지수]
					// 1. 미국 2.아시아 3.유럽 4. MSCI 5. 기타
					// [종목]
					// 1. 미국 2.일본 3.홍콩 4.대만 5.영국 6.프랑스 7.독일 8.ADR 9.GDR
					// [선물]
					// 1.CME지수선물 2.에너지 3.상품선물
					// [금리]
					// 1.리보금리   2.미국금리
					// [국채]
					// 1.미국 2.일본 3.영국 4.독일
					// [환율]
					// 국제환율
		char	knam[40];	// 한글 SYMBOL명
		char	enam[40];	// 영문 SYMBOL명
		char	icod[6];	// 종목 업종코드
		char	ncod[3];	// 국가코드
		char	excd[4];	// 거래소코드
	};

	bool	m_bVisible;
	int	m_Width, m_Height;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCodeCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeCombo)
	afx_msg void OnSelchange();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	bool	IsValidCode(CString sCode);
	bool	OCodeLoad(CString tabPath);
	bool	FCodeLoad(CString tabPath);
	bool	SearchCode(CString code);
	CString	GetSelectCode();
	void	Key(int key);
	void	InitCodeList();
	void	InitHistory(CString history, int unit);
	bool	IsHistoryMode();
	bool	IsCodeListMode();
	bool	JCodeLoad(CString tabPath);
	bool	ELWCodeLoad(CString tabPath);
	BOOL	loadSfCode(CString tabPath);
	BOOL	LoadUSCode(CString path);
	void	RemoveCode();
	int	GetCurSel();
	void	SetTopIndex(int index);
	void	SetCurSel(int cur);
	int	GetTopIndex();
	int	GetCount();
	void	AddString(CString str);
	void	AddSFString(CString str);
	void	ResetContent();
	bool	GetDroppedState();
	void	ShowDropDown(bool bflag, bool start = true);
	void	SetDroppedState(bool bflag);
	void	SetVisible(bool bflag){ m_bVisible = bflag; }
	void	WriteHistory(CString root);

public:
	int		m_DataMode;
	CWnd*		m_pWizard;
	CWnd*		m_pParent;
	bool		m_bTracking;

protected:
	std::unique_ptr<CCodeList> m_pCodeList;
	CString		m_sCode;

	struct OCode
	{
		CString	Price;

		char	Atm;
		struct CALL
		{
			char	Exist=0;
			CString Name;
			CString	Code;
		} Call[4];
		struct PUT
		{
			char	Exist=0;
			CString	Name;
			CString	Code;
		} Put[4];
	};

	CArray  <_JCode, _JCode> m_pHCode;	// history list
	CQArray <_JCode, _JCode> m_pJCode;	// code list
	CArray  <_JCode, _JCode> m_arUSCODE;
};

/////////////////////////////////////////////////////////////////////////////
// CodeEdit window 

class CCodeEdit : public CDDEdit
{
// Construction
public:
	CCodeEdit(class CfxCodeCtrl *pParent, CWnd* pTarget);
	virtual ~CCodeEdit();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCodeEdit)
	public:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	// Generated message map functions
protected:
	//{{AFX_MSG(CCodeEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	bool	isHexNumeric(CString str);
	bool	isHexNumeric2(CString str);
	bool	isNumeric(CString str);
	void	SetUnit(int unit) { m_Unit = unit; }
	void	ListCode(CString code);
	void	ChangeHangulMode(HWND hWnd, bool bHangle = true);
public:
	bool	m_bValidCheck;

private:
	CBrush	m_NObackColor;
	CBrush	m_backColor;

	class	CfxCodeCtrl* m_pParent;
	CWnd*	m_pTarget;

	int		m_Unit;
	CString		m_preCode;
	bool		m_bKeydown;
	BOOL	m_bFocusInit;
};

/////////////////////////////////////////////////////////////////////////////
// CfxCodeCtrl window

class CfxCodeCtrl : public CCodeCombo
{
// Construction
public:
	CfxCodeCtrl(CWnd* parent, CString tabPath);
	virtual ~CfxCodeCtrl();

// Attributes
public:
	std::unique_ptr<CCodeEdit>  m_pEdit;

	CRect	m_btnListRect;
	CRect	m_btnCodeRect;

	CString	m_sHistory;
	CString	m_sPath;
	CString	m_Name;
	bool	m_bValidCheck;
	bool	m_bHistory;
	int	m_nIndex;
	bool	m_bFirst;
	int	m_Unit;

private:
	CFont*	m_pFont;
	int	m_btnDown;
	int	m_btnList;
	int	m_updnIndex;

	std::unique_ptr<class CfxImgButton>	m_pCtlUpBtn;
	std::unique_ptr<class CfxImgButton>	m_pCtlDownBtn;
	std::unique_ptr<class CfxImgButton>	m_pBtnCode;
	std::unique_ptr<class CfxImgButton>	m_pBtnList;

	CString	m_sbmpCode;
	CString	m_sbmpCodepush;
	CString	m_sbmpCodeEn;
	CString	m_sbmpList;
	CString	m_sbmpListpush;
	CString	m_sbmpListEn;
	
	CStringArray	m_arrList;

	BOOL	m_bFocusInit;
		
// Operations
public:
	BOOL	Create(CWnd* parent, CRect rect, UINT id);
	void	SetHistory(CString sHistory);
	void	SetFont(CFont* pFont, bool redraw = true);
	CString	GetEditData();
	CString GetHistCode(int idx);
	void	SetEditData(CString sData, bool bflag = false);
	void	SetUnit(int unit);
	void	HistoryMode();
	void	CodeListMode();
	void	LButtonUp();
	void	SetValidCheck(bool bValid);
	void	GetRect(CRect &rc);
	bool	IsForeign(CString code);

	void	SetEditFocus();
	void	SetComboBtnBmp(CString sCode,CString sCodedown,CString sList,CString sListdown);
	void	SetListCode(CString str);
	HBITMAP GetAxBitmap(CString bmps);
	HCURSOR GetCursor(int kind);
	HWND	GetEditSafeHwnd(){ _ASSERT(m_pEdit); return m_pEdit->GetSafeHwnd(); }
	void	SetNormal();
	void	SetAxFocus(bool bflag);
	void	FireChangeEvent();
	void	GoDown();
	void	GoUp();
	void	LoadData();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CfxCodeCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	CString GetSymbol();

	// Generated message map functions
public:
	CBrush m_backColor;
	CBrush m_NObackColor;
	void ResizeCtrl();
	void ResizeEdit(int cx, int cy);
	//{{AFX_MSG(CfxCodeCtrl)
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	afx_msg void OnBtnUp();
	afx_msg void OnBtnDown();
	afx_msg void OnCodeDown();
	afx_msg void OnListDown();
	DECLARE_MESSAGE_MAP()
};