// CConditionGrid.h: interface for the CConditionGrid class.
//
//////////////////////////////////////////////////////////////////////

/*##################################################################
##																																##
##								Condition Make Grid Control											##
##																																## 
##																																##
##									작성일 : 2003. 7 ~														##
##									작성자 : 우동우 (TEXEN SYSTEM)								##
##																																##  
###################################################################*/

//# 2003. 7 : UpDate

#if !defined(AFX_CONDITIONGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_CONDITIONGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "../UGridCtrl/UGCtrl.h"
#include "../UGridCtrl/_Include/UGCTbutn.h"
#include "../UGridCtrl/_Include/ugctelps.h"
#include "../UGridCtrl/_Include/ugctsarw.h"

class CConditionGrid : public CUGCtrl
{
public:
	CConditionGrid();
	virtual ~CConditionGrid();

private:
	CFont m_fontDefault, m_fontBold;

	//## UpDate : 07 #################
public :
	CWnd *m_pParentWnd;

	//## System Val : 외부 Set Val ##########################################
	BOOL m_bGridType;		//# TRUE : Order Grid, FALSE : Common Grid

	BOOL m_bUserDragDrop;

	BOOL m_bSearchMode;  //# TRUE : 기본검색 
	//# FALSE : 순차검색

	BOOL	m_bViewState;		//#<= Grid Display [Min / Max]
	int		m_nDefRowHeight;
	int		m_nDefRowHeadHeight;

	BOOL m_bDetailView;
	//#######################################################################

public:	// Attributes
	int								m_nSortCol;
	int								m_nSortMethode;

	int								m_nSortArrow;
	CUGSortArrowType				m_sortArrow;
	CUGButtonType					m_button;
	CUGButtonType					m_buttonUp;
	CUGButtonType					m_buttonDown;
	CUGButtonType					m_buttonMaxUp;
	CUGButtonType					m_buttonMinDown;

	long							m_nArrowType;
	bool							m_bProcessSelectChange;
	int								m_nLastSelRow;

	//## DragDrop 관련 ##
	BOOL m_bSetFocus;
	BOOL m_bLeftBtnState;  //<= Mouse Left Button State
	BOOL m_bDuringDrag;
	long m_loldRow, m_lnewRow;

	BOOL m_bViewAllField;

	//## Size Values ##
public:
	BOOL m_allowColSizing;
	BOOL m_allowRowSizing;
	BOOL m_bCallAdjustOneTime;

	int m_minWidth, m_maxWidth;
	COLORREF	m_clrHeader;
	COLORREF	m_clrSelRow;

public:	 // Member Functions
	void ClearAll();
	void GotoLastRow();
	CSize	GetFontSize();

	void SetRow(int nRow,
		BOOL		bState,					//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기 

	void SetAddRow(BOOL		bState,						//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetInsertRow(int nRow,
		BOOL		bState,						//# Condition View
		int			nElementIndex,	//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetModifyRow(int nRow,
		BOOL		bState,					//# Condition View
		CString	strElement,			//# Element Index
		BOOL		bNot,						//# Not State 
		int			nTargetType,		//# Target Type
		int			nTargetIndex,		//# Target Index
		BOOL		bTargetEnable,	//# Target Enable
		CString strItem,				//# 완성 구문 
		BOOL		bFieldState,		//# 값보기 상태 (TRUE : 값보기 O, FALSE : 값보기 X)
		BOOL		bFieldVal);			//# 값보기

	void SetModifyRow(int nRow,
		BOOL bNot, 
		CString strItem, 
		BOOL bFieldVal,
		BOOL bRedraw = TRUE);

	BOOL SetDeleteRow(int nRow);

	void SetModifyAllTargetCol(int nTargetIndex, int nOutType = -1, BOOL bEnable = FALSE);


	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void SetEvenOddColor();
	void SetStepColor(BOOL bStepFlag = FALSE, int nSearchType = 0, int nStepCnt = 0);
	inline void SetBackColorIfNotBtnStep(CUGCell *pCell, COLORREF clrSet);
	inline void SetBackColorBtn(CUGCell *pCell, COLORREF clrSet);

	void SetFieldAllCheck(BOOL nView);
	int SetAlignElement(int nRowIndex);

	CString GetCellDate(int col, int row);
	void SetCellDate(int col, int row, CString strData);

	int GetListBoxType(int nTagetType);
	CString GetRowToElement(int nRow);

	BOOL OnDragDropRowChange(long nOldRow, long nNewRow);

	void SetPopUpMenu();
	void SetHeadColor(BOOL bFocus);

	BOOL SetMakeRowVisible(BOOL bViewState);


	void SetSelectRow(int nRow);
	void SetSelectRow(int nOldRow, int nNewRow);
	void SetUnSelectRow(int nRow = -1, BOOL bDel = FALSE);

	void SetDetailViewState(BOOL bDetail);

	//## Element (Logic Edit) ##
	void GetInsertElement(CStringArray &saInsetedElement);

	int GetElementIndex(CString strElement);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	BOOL ConditionRowChange(long nOldRow, long nNewRow);

public: // Overrides
	virtual void	OnSetup();

	virtual void	OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);
	virtual void	OnTH_RClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);

	virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnMouseMove(int col,long row,POINT *point,UINT nFlags,BOOL processed);

	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	virtual int		OnCanSizeTopHdg();
	virtual void	OnRowChange(long oldrow, long newrow);
	virtual void	OnSysColorChange();

	virtual void	OnKillFocus(int section);
	virtual void	OnSetFocus(int section);

	virtual int		OnHint(int col, long row, int section, CString *string);

	virtual int		OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	//## Size Function ##
	virtual int		OnCanSizeCol(int col);
	virtual void	OnColSizing(int col,int *width);
	virtual int		OnCanSizeRow(long row);

	virtual void  OnAdjustComponentSizes(RECT *grid, RECT *topHdg,
		RECT *sideHdg, RECT *cnrBtn, 
		RECT *vScroll, RECT *hScroll, 
		RECT *tabs);

	//# Menu ##################
	virtual void	OnMenuCommand(int col,long row,int section,int item);
	virtual int		OnMenuStart(int col,long row,int section);

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(SetGrid)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CONDITIONGRID_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
