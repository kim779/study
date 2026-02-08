// Grid_Comm.h: interface for the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../DrCommon/UGridCtrl/ugctrl.h"
#include "../../../DrCommon/UGridCtrl/ugctsarw.h"
#include "../../../DrCommon/UGridCtrl/UGCTbutn.h"
#include "../../../DrCommon/Dr_Control/DrInfoTip.h"

#include "../../../DrCommon/UGridCtrl/FindDataGridUtil.h"	//20110829:Add By JunokLee.

//#include "DrInfoTip.h"
class CPrint4Grid;
class CGrid_Comm : public CUGCtrl
{
public:
	CGrid_Comm();
	virtual ~CGrid_Comm();

public:
	int m_nLastSelRow;
	int m_nLastCol;
	int m_nPrevRow;
	BOOL m_bSelectCheckFlag;
	CString m_strDate;

	//20070703 이문수 >>
public:
	short			m_nStyle4Print;
	CPen *			m_pBorderPen4Print;
	CPrint4Grid *	m_pPrint;
	CFont		*	m_pPrintTailFont;

	//20070703 이문수 <<	
private:
	CFont				m_fontDefault;
	CPen				m_penGridLine;

	int		            m_nSortCol;
	int		            m_nSortMethode;
	int		            m_nSortArrow;
	CUGSortArrowType	m_sortArrow;
	long	            m_nArrowType;

	CImageList			m_imglistSignal;

	BOOL				m_bCallAdjustOneTime;

	CString m_strType;  //# "RESULT"

	BOOL m_bShowCol, m_bShow_Order;
	int m_nHideCol;

	int m_nFontWidth, m_nFontHeight;

	CUGButtonType m_ugButtonType;
	int m_nButtonIndex;

	CMapStringToString m_mapUpjong;

	COLORREF			m_clrHeader;
	COLORREF			m_clrSelRow;

public:	 // Member Functions
	void SetInit(CString strType);

	void SetCommMenu_Add(CMenu* pMenu);
	CString GetCommMenu_IndexToScreenNo(int nIndex_SelMenu);

	void ClearAll();


	void SetHideCol(int nCol, BOOL bShow);
	void SetHide_Order(BOOL bShow);

	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();

	int GetCodeColNumber(int nType=0);
	CString GetRowToCode(int nRow);
	BOOL IsRowCodeChecked(int nRow);	//State of Code Check.
	void SetSelectRow(long oldrow, long newrow);

	void SetSelectRow(int nRow);

	void SetAddRow(int nRow, int lOutFieldCnt, CANDLE_OUT_LIST stCANDLE_OUT_LIST, int arCandledata[5]);

	CSize GetFontSize();

	int  m_nFixCol;
	int	 m_nSortType[RESULT_FIELD_MAX];
	CList <HeadInfoType, HeadInfoType&> *m_plistField;

	void SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField);
	void SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType,
		CString strItemCode, CString strItemName, double* dcolData, bool bRedraw);

	CString GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	void GetSignInfo(int nRow, int nSign, COLORREF &clrSignText);


	void SetLockColumns(int nCol, BOOL bOffSet = FALSE);

	int OnGetSelectRow();

	BOOL GetRowToCodeName(int nRow, CString &strCode, CString& strName, int &nMarketType);

	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

private:
	void HeaderSetup(CString strType);
	void SetHint_Code(int nRow);
	CString GetStockStatus(CString strItemCode, CString strItemName, COLORREF& clrbunText, COLORREF& clrbunBack);
	CString LoadMemo(CString strKey);
	void SaveMemo(CString strKey, CString strData);

public: // Overrides
	virtual void	OnSetup();

	virtual int OnCellTypeNotify(long ID, int col, long row, long msg, long param);

	virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);

	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);

	//virtual int		OnCanSizeTopHdg();
	virtual void	OnSysColorChange();
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual int		OnHint(int col, long row, int section, CString *string);
	//virtual int		OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual void  OnAdjustComponentSizes(RECT *grid, RECT *topHdg,
		RECT *sideHdg, RECT *cnrBtn, 
		RECT *vScroll, RECT *hScroll, 
		RECT *tabs);
	virtual void	OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);
	virtual void OnKeyDown(UINT *vcKey,BOOL processed);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(ShowGrid)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	afx_msg long OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

	//--> Drag&Drop : 소리마치용.
	BOOL GetRowToCode(int nRow, CString &strCode, int &nMarketType);

public:
	enum eMouseModes { MOUSE_NOTHING, MOUSE_CODE_DRAG};
	enum  GRID_NOTIFY_CONTROL_MSG { 
		GRID_TO_JMCOMBO_SETUP_NOTIFY		= 1,
		JMCOMBO_TO_GRID_CODECHANGE_NOTIFY,
		JMCOMBO_TO_GRID_CLOSE_NOTIFY,
		JMCOMBO_TO_GRID_CODE_DROP_NOTIFY,
	};

	CBitmap*	m_pDragBitmap;
	CImageList*	m_pDragImage;
	BOOL		m_bDragAndDrop;
	CString		m_strDragCode;
	int         m_MouseMode;
	UINT		m_uGridNotifyControlMsg;
	BOOL		m_bFocus;
	long		m_lDragRow;
	int			m_nDragCol;

	void FnLButtonDown(CPoint point);
	void FnLButtonUp(CPoint point);
	void FnMouseMove(CPoint point);

	BOOL MakeDragImage(CString strText);
	void DragImageInfoClear();
	//<-- Drag&Drop

	HWND m_hChildFrame;
	void FnRButtonUp(CPoint point);
	void SetCommMenu(POINT *point);
	HWND GetFrameWnd(CWnd* pWnd);

	void GetRowCode(int nRow, CString &strCode, int &nMarkType);
	void GetRowData(int nRow, BOOL bBackTest, CString &strCode, CString &strNowPrice);
	CString GetCellData(int nRow, int nCol);
	void GetCellColor(int nRow, int nCol, COLORREF &clrText, COLORREF &clrBack);
	void GetCellSignText(int nRow, int nCol, CString &strSignText);
	CString GetSignText(int nSign);

	//20070703 이문수 >>
	int PrintingInit(CDC * pDC, CPrintDialog* pPD, int startCol,long startRow,
		int endCol,long endRow);
	int PrintingPage(CDC * pDC, int pageNum);
	int PrintingSetMargin(int whichMargin,int size);
	int PrintingSetScale(double scale);
	int PrintingSetOption(int option,long param);
	int PrintingGetOption(int option,long *param);
	void PrintingTail(CDC * pDC, CPrintInfo * pInfo);

	int GetTHRowHeight(int row);
	void SetCellBorderColor4Print(COLORREF clrBorder);
	void SetBorderStyle4Print(short nStyle);
	short GetBorderStyle4Print();

	void SetPrintTailFont(CString strFontName, int nFontSize);
	CFont * GetPrintTailFont();
	//20070703 이문수 <<
};

#endif // !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
