// Grid_Comm.h: interface for the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../DrCommon/UGridCtrl/UGCtrl.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTbutn.h"
#include "../../../DrCommon/UGridCtrl/_Include/ugctelps.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTsarw.h"

#include "../../../DrCommon/Dr_Control/DrInfoTip.h"
#include "../../../DrCommon/UGridCtrl/FindDataGridUtil.h"	//20110829:Add By JunokLee.

#include "../DefineFiles/server_typedef.h"
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
	COLORREF m_clrHeader;
	COLORREF m_clrSelRow;
	
private:
	CFont             m_fontDefault;
	CPen	            m_penGridLine;
	
	CDrInfoTip        m_tipInfo;
	
	int		            m_nSortCol;
	int		            m_nSortMethode;
	int		            m_nSortArrow;
	CUGSortArrowType	m_sortArrow;
	long	            m_nArrowType;
	
	BOOL              m_bCallAdjustOneTime;
	
	CString m_strType;  //# "RESULT"
	
	BOOL m_bShowCol;
	int m_nHideCol;
	
	HWND m_hChildFrame;

	CUGButtonType		m_ugButtonType;
	int					m_nButtonIndex;
	
public:	 // Member Functions
	void SetInit(CString strType);
	
	CWnd* GetFrameWnd(CWnd* pWnd);
	void SetCommMenu(POINT *point);
	void SetCommMenu_Add(CMenu* pMenu);
	CString GetCommMenu_IndexToScreenNo(int nIndex_SelMenu);
	
	void ClearAll();
    
	void SetAddRow(BOOL bReal, SignalData *pstSignalData, SIGNAL_CONDITION stSIGNAL_CONDITION, int ntotCol, int nRow = -1);
	void SetHideCol(int nCol, BOOL bShow);
	
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();
	
	void GetAttributeInfo(int nRow, char chAttribute, COLORREF &clrContrastText);
	
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	void SetSelectFluctuating();
	
	void SetColResize(BOOL bState);
	
	CString GetRowToCode(int nRow);
	void SetSelectRow(long oldrow, long newrow);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	////////////////////////////////////////////////
	// 항목편집
	CList<HeadInfoType, HeadInfoType&>	*m_plistField;
	CList<CandleInfo, CandleInfo&>		m_listCnadle;
	CList<PatenInfo, PatenInfo&>		m_listPaten;

	BOOL	m_bCandleLoadFalg;
	int		m_nFontHeight; 
	int		m_nFontWidth; 
	int		m_nFixCol;
	int		m_nSortType[RESULT_FIELD_MAX];

	CString strAppPath;


	void    SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField);
	void	SetLockColumns(int nCol, BOOL bOffSet);
	void	SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
					  CString strItemName, char dcolData[960], bool bRedraw);
	CString	FindCandleIdFromName(long lCandleID);
	BOOL	LoadCandleIndexToXml();
	CString FindPatenIdFromName(long lPatenID);
	BOOL	LoadPatenIndexToXml();
	CSize	GetFontSize();
	CString GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	/*CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);*/
	inline void GetSignColor(int nSign, COLORREF &clrSignText);
	void HeaderSetup(CString strType);
	////////////////////////////////////////////////

	CString LoadMemo(CString strKey);
	void SaveMemo(CString strKey, CString strData);
	
private:
	void SetHint_Code(int nRow);
	
	
public: // Overrides
	virtual void	OnSetup();
	
	virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);
	
	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	
	virtual int		OnCanSizeTopHdg();
	virtual void	OnSysColorChange();
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual int		OnHint(int col, long row, int section, CString *string);
	//virtual int		OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual void  OnAdjustComponentSizes(RECT *grid, RECT *topHdg,
		RECT *sideHdg, RECT *cnrBtn, 
		RECT *vScroll, RECT *hScroll, 
		RECT *tabs);
	virtual void	OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);
	
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
	
	afx_msg void OnDestroy();
	afx_msg long OnChangeChildFrameWindowHandle(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
		
		//<< 07.07.10 이준옥 Drag&Drop추가. 삼성에 맞게
public:
	enum eMouseModes { MOUSE_NOTHING, MOUSE_CODE_DRAG};
	enum  GRID_NOTIFY_CONTROL_MSG { 
		GRID_TO_JMCOMBO_SETUP_NOTIFY		= 1,
			JMCOMBO_TO_GRID_CODECHANGE_NOTIFY,
			JMCOMBO_TO_GRID_CLOSE_NOTIFY,
			JMCOMBO_TO_GRID_CODE_DROP_NOTIFY,
	};
	
	HWND		m_hFrameWnd;
	CBitmap*	m_pDragBitmap;
	CImageList*	m_pDragImage;
    BOOL		m_bDragAndDrop;
	CString		m_strDragCode;
	int         m_MouseMode;
	UINT m_uGridNotifyControlMsg;
	BOOL		m_bFocus;
	long		m_lDragRow;
	int			m_nDragCol;
	
	void FnLButtonDown(CPoint point);
	void FnLButtonUp(CPoint point);
	void FnMouseMove(CPoint point);
	
	BOOL MakeDragImage(CString strText);
	void DragImageInfoClear();
	//>> 07.07.10 이준옥
	
	void	FnRButtonUp(CPoint point);
};

#endif // !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
