// SetSignalGrid.h: interface for the CSetSignalGrid class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SETSIGNALGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_SETSIGNALGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../DrCommon/UGridCtrl/UGCtrl.h"
//#include "_Include/UGCTbutn.h"
#include "../../DrCommon/UGridCtrl/_Include/UGCTbutn.h"
//#include "_Include/ugctsarw.h"
#include "../../DrCommon/UGridCtrl/_Include/ugctsarw.h"

class CSetSignalGrid : public CUGCtrl
{
public:
	CSetSignalGrid();
	virtual ~CSetSignalGrid();

private:
	int						m_nSortCol;
	int						m_nSortMethode;
	int						m_nSortArrow;
	CUGSortArrowType	m_sortArrow;
	long					m_nArrowType;
	
	CFont m_fontDefault, m_fontBold;
	CPen		m_penGridLine;
	BOOL		m_bCallAdjustOneTime;
	CUGButtonType		  m_button;
	//CUGSpinButtonType	m_spin;
	
	int m_nIndex_SetSignal;
	int m_nTickerVal, m_nOrderVal, m_nStateVal;
	
	int			m_nLastSelRow;

	COLORREF	m_clrHeader;
	COLORREF	m_clrSelRow;

public:
	void SetInit();
	
	void ClearAll();
	void HeaderSetup();
	
	void SetAdd_Row(SetSignalData	*pstSetSignalData, BOOL bRedraw = FALSE);
	void SetDelete_Row(int nRow);
	
	CString GetRowToTitle(int nRow);
	int GetTitleToRow(CString strTitle);
	CString GetRowToKey(int nRow);
	BOOL GetHaveState(CString strTitle);
	
	void GetData_Row(int nRow, SetSignalData	&stSetSignalData);
	
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void GetPointToColor(COLORREF clrDefult, int nPoint, COLORREF &clrPointBk, COLORREF &clrPointText);
	
	int GetSelectRow();
	void SetEvenOddColor();
	
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

public:
	virtual void	OnSetup();
	
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
  virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);
	
	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);

	virtual void	OnSysColorChange();
	virtual int		OnHint(int col, long row, int section, CString *string);
	virtual void  OnAdjustComponentSizes(RECT *grid, RECT *topHdg,
                                        RECT *sideHdg, RECT *cnrBtn, 
                                        RECT *vScroll, RECT *hScroll, 
                                        RECT *tabs);

	virtual void	OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);


	virtual int		OnCellTypeNotify(long ID,int col,long row,long msg,long param);

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

#endif // !defined(AFX_CODEGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
