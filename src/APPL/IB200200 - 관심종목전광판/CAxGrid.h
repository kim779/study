#pragma once

#include "axheaderCtrl.h"
#include "excel9.h"
// CAxGrid
#define	ARR_UP		"↑"
#define	ARR_DN		"↓"
#define	TRI_UP		"▲"
#define	TRI_DN		"▼"
constexpr int TIMER_DRAW    = 11111;
constexpr COLORREF fontblue = RGB(0, 0, 255);
constexpr COLORREF fontred  = RGB(255, 0, 0);
constexpr int headerinfo_name       = 0;
constexpr int headerinfo_width      = 1;
constexpr int headerinfo_format     = 2;
constexpr int headerinfo_itemformat = 3;
constexpr int headerinfo_itemtype   = 4;
constexpr int headerinfo_sort       = 5;
constexpr int headerinfo_symbol     = 6;
constexpr int item_gridtime   	    = 1;
constexpr int item_gridnum          = 2;
constexpr int item_gridmark         = 3;
constexpr int gridmessage = WM_USER  + 10;
constexpr int grid_onedraw = 1;


class AxGrid : public CListCtrl
{
	DECLARE_DYNAMIC(AxGrid)

public:
	AxGrid();
	virtual ~AxGrid();
public:
	void setWizard(CWnd* pWizard) { _Wizard = pWizard; }
	void runExcelThread();
	bool ResetSymbolKey();
	void clear();
	void drawitem(int row, std::vector<int>& vCol);
	BOOL creategrid(CRect rect, CWnd* pWnd, int rowheight = 20, int time = 300, UINT nID = 100);
	void drawMark(CDC* pDC, CRect rc, const int& type);
	void SetUserFont(CFont* font, CFont* boldfont){ _itemfont = font; _boldfont = boldfont; }
	void SetBKSelColor(COLORREF clr);
	void SetHeaderColor(COLORREF txtclr, COLORREF bgclr); 
	void SetItemColor(COLORREF txtclr, COLORREF bgclr)
	{
		_clrNormal = txtclr;
		_clrbg = bgclr;
		_brBack = getAxBrush(_clrbg);
	}
	void SetStepColor(int step, COLORREF step1, COLORREF step2)
	{
		_step   = step;
		_clr1st = step1;
		_clr2st = step2;

		_br1st = getAxBrush(_clr1st); 
		_br2st = getAxBrush(_clr2st);
		RedrawWindow();
	}
	void SetConditionColor(COLORREF up, COLORREF down)
	{
		_clrUp = up;
		_clrDown = down;
	}

	std::deque<std::pair<std::vector<CString>, std::map<CString,CString>>>* getDataArray() {
		return &_vData;
	}

	std::vector<std::tuple<CString, UINT, UINT, UINT, UINT, bool, CString>>* getHeaderArr() {
		return &_vHeader;
	}

	std::multimap<CString, int>*  getCodemap(){
		return &_keymap;
	}     

	CBrush*  getAxBrush(COLORREF clr);
	CPen*    getAxPen(COLORREF clr, int width = 1, int style = PS_SOLID);
	CBitmap* getAxBitmap(CString path);
	CFont*   getAxFont(CString fName, int point = 90, int style = FW_NORMAL);

protected:
	
	void DrawSubItem(CDC* pDC, LPDRAWITEMSTRUCT lp, const int& nItem, const int& nSubItem);
	std::tuple<CString, COLORREF, int> Formating(CString& str, const int& nSubitem);
	CString& makeComma(CString& str);

	afx_msg void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);	
	DECLARE_MESSAGE_MAP()

private:
	void drawContrast(CDC* pDC, CRect cellRc, CString sText, COLORREF clr);
	void gdiclear();
	bool toExcel(bool bVisible = false);
	bool runExcel();


	CWnd*  _Wizard;
	CFont* _itemfont{};
	CFont* _boldfont{};
	CFont*  _font{};

	int   _Height = 25;
	int   _sort   = 0;
	int   _step   =  0;

	CBrush* _brFrame{};
	CBrush* _brColumn{};
	CBrush* _brBack{};
	CBrush* _brSelected{};
	CBrush* _brColumnSelected{};
	CBrush* _br1st{};
	CBrush* _br2st{};


	// font color
	COLORREF _clrSelect;
	COLORREF _clrNormal;
	COLORREF _clrbg = RGB(255, 255, 255);
	COLORREF _clr1st;
	COLORREF _clr2st;
	COLORREF _clrUp;
	COLORREF _clrDown;


	
	CAxHeaderCtrl _AxHeader;

	std::map<int,  std::vector<int> > _mDraw;
	std::deque< std::pair<std::vector<CString>,          /*data*/
	                      std::map<CString, CString>>    /*hidden*/     > _vData;   
	std::multimap<CString	/* 종목코드   */, 
	              int	/* 배열인덱스 */>      		       _keymap;
	
	std::vector<std::tuple<CString  /* 타이틀      */, 
		               UINT	/* 넓이	       */, 
		               UINT	/* 헤더	  포멧 */, 
			       UINT	/* 아이템 포멧 */, 
		               UINT	/* 타입	       */,
			       bool     /* sort        */,
			       CString  /* 심볼        */>>            _vHeader;

	CString _excelS;
	bool    _excelF = true;
	int     _sortSel = -1;
	bool    _drawing = false;
	virtual void PreSubclassWindow();

public:
	afx_msg LRESULT OnDrawGrid(WPARAM wParam, LPARAM lParam);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnNMClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLvnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
};
