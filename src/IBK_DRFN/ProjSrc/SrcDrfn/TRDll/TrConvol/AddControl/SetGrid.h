// ResultGrid.h: interface for the CSetGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_SETGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

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

const UINT RMSG_DBCLICK = ::RegisterWindowMessage(_T("RMSG_DBCLICK"));

class CSetGrid : public CUGCtrl
{
public:
	CSetGrid();
	virtual ~CSetGrid();

  CFont m_fontDefault;
	CPen	m_penGridLine;

	int						m_nSortCol;
	int						m_nSortMethode;
	int						m_nSortArrow;
	CUGSortArrowType	m_sortArrow;
	long					m_nArrowType;

	BOOL m_bCallAdjustOneTime;

public:
	CString m_strType;		//# "CODE", "VOL", "PRICE"
	int m_nLastSelRow;
	
public:	 // Member Functions
	void ClearAll();
	void HeaderSetup(CString strType);
	void SetAddRow(CString strData1, CString strData2, BOOL bRedraw = FALSE);
	void SetModifyRow(int nRow, CString strData1, CString strData2);

	void SetAddRow(int nData1, int nData2, BOOL bRedraw = FALSE);
	void SetModifyRow(int nRow, int nData1, int nData2);
	void SetModifyRow(int nData1, int nData2);

	void SetDeleteRow(int nRow);

	void GetRowData(int nRow, CString &strData1, CString &strData2);
	void GetRowData(int nRow, int &nData1, int &nData2);


	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();

public: // Overrides
	virtual void	OnSetup();
	
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);

	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);

	virtual int		OnCanSizeTopHdg();
	virtual void	OnSysColorChange();
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);

	virtual int		OnHint(int col, long row, int section, CString *string);

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

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SETGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
