// ResultGrid.h: interface for the CConVolGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONVOLGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_CONVOLGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
#include "ugctrl.h"
#include "_Include/UGCTbutn.h"
#include "_Include/ugctelps.h"
#include "_Include/ugctsarw.h"
*/

#include "../../../DrCommon/UGridCtrl/ugctrl.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTbutn.h"
#include "../../../DrCommon/UGridCtrl/_Include/ugctelps.h"
#include "../../../DrCommon/UGridCtrl/_Include/ugctsarw.h"

//const UINT RMSG_DBCLICK = ::RegisterWindowMessage(_T("RMSG_DBCLICK"));

#define RESULT_FIELD_MAX 20 

class CConVolGrid : public CUGCtrl
{
public:
	CConVolGrid();
	virtual ~CConVolGrid();

	CFont	m_fontDefault;
	CPen	m_penGridLine;

	int						m_nSortCol;
	int						m_nSortMethode;
	int						m_nSortArrow;
	CUGSortArrowType		m_sortArrow;
	long					m_nArrowType;

	BOOL m_bCallAdjustOneTime;
	int						m_nMaxRow;
	int						m_nLastRow;
	BOOL					m_bTypeChange;
	BOOL					m_nRow; //하나씩 갱신되는 방식으로 사용할때....
	CMapStringToString		m_mapCount;

public:
	CString  m_strType;		//# "CODE", "VOL", "PRICE"
	int		 m_nLastSelRow;
	COLORREF m_clrHeader;
	COLORREF m_clrSelRow;
	
public:	 // Member Functions
	void ClearAll();
	void HeaderSetup();
	void SetAddRow(StConVolClient& stConVolClient, BOOL bInsert = TRUE, BOOL bRedraw = FALSE);
	void SetModifyRow(int nRow, CString strData1, CString strData2);

	void SetAddRow(int nData1, int nData2, BOOL bRedraw = FALSE);
	void SetModifyRow(int nRow, int nData1, int nData2);
	void SetModifyRow(int nData1, int nData2);

	void SetAddRow(StTrConVol stTrConVol, BOOL bInsert, BOOL bRedraw = FALSE);

	void SetDeleteRow(int nRow);

	void GetRowData(int nRow, CString &strData1, CString &strData2);
	void GetRowData(int nRow, int &nData1, int &nData2);

	inline void GetSignColor(int nSign, COLORREF &clrSignText);
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();
	void InsetCommar(CString& str1);
	void CurSelectRow(int nRow, BOOL bInsert);
	void SetSelectRow(long oldrow, long newrow, BOOL bAdd = FALSE);
	void PaintSelectRow(long oldrow, long newrow, BOOL bAdd = FALSE);
	void HideColumn(int nCol, BOOL bHide = TRUE);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	////////////////////////////////////////////////
	// 항목편집
	CList<HeadInfoType, HeadInfoType&>	*m_plistField;
	CList<CandleInfo, CandleInfo&>		m_listCnadle;
	CList<PatenInfo, PatenInfo&>		m_listPaten;
	
	BOOL	m_bCandleLoadFalg;
	/*BOOL	m_bPatenLoadFalg;*/
	int		m_nFontHeight; 
	int		m_nFontWidth; 
	int		m_nFixCol;
	int		m_nSortType[RESULT_FIELD_MAX];
	
	
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
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	////////////////////////////////////////////////

public: // Overrides
	virtual void	OnSetup();
	
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);

	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);

	virtual int		OnCanSizeTopHdg();
	virtual void	OnSysColorChange();
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);

	virtual int		OnHint(int col, long row, int section, CString *string);

	virtual void	OnAdjustComponentSizes(RECT *grid, RECT *topHdg,
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

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SETGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
