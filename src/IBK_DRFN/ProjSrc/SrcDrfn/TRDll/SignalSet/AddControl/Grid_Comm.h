// Grid_Comm.h: interface for the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../DrCommon/UGridCtrl/UGCtrl.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTsarw.h"

#include "../../../DrCommon/Dr_Control/DrInfoTip.h"
class CGrid_Comm : public CUGCtrl
{
public:
  CGrid_Comm();
	virtual ~CGrid_Comm();

public:
	int m_nLastSelRow;
	BOOL m_bSelectCheckFlag;
 
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
	
	CString m_strType;  //# "KEYWORD", "CODE", "VOL", "PRICE"
	
	BOOL m_bShowCol;
	int m_nHideCol;

	COLORREF	m_clrHeader;
	COLORREF	m_clrSelRow;

public:	 // Member Functions
	void SetInit(CString strType);
	void HeaderSetup(CString strType);
	void ClearAll();
	
	//# KEYWORD
	int SetAdd_Row(KeywordData stKeywordData, BOOL bRedraw = FALSE);
	void SetDelete_Row(int nRow);
	
	void GetFind_Keyword(CString strWord, CUIntArray &iaRow);
	void SetGoto(int nRow, BOOL bSelete = FALSE);
	int GetSelectRow();
	void GetData_Row(int nRow, KeywordData	&stKeywordData);
	
	//# SUBSET
	void GetData_Row(int nRow, CString &strData1, CString &strData2);
	void GetData_Row(int nRow, int &nData1, int &nData2);
	
	void SetAdd_Row(CString strData1, CString strData2, BOOL bRedraw = FALSE);
	void SetAdd_Row(int nData1, int nData2, BOOL bRedraw = FALSE);
	
	void SetModify_Row(int nRow, CString strData1, CString strData2);
	void SetModify_Row(int nRow, int nData1, int nData2);
	void SetModify_Row(int nData1, int nData2);
	
	
	
	
	void SetHideCol(int nCol, BOOL bShow);
	
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();
	
	void GetAttributeInfo(int nRow, char chAttribute, COLORREF &clrContrastText);
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);

	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
  
private:
  
	void SetHint_Code(int nRow);


public: // Overrides
	virtual void	OnSetup();
	
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

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
