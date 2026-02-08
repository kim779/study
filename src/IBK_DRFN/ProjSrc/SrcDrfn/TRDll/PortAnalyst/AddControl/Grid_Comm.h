// Grid_Comm.h: interface for the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../../DrCommon/UGridCtrl/UGCtrl.h"

#include "../../../DrCommon/UGridCtrl/UGCTsarw.h"
#include "../definefiles/server_typedef.h"
#include "../../../DrCommon/Dr_Control/DrInfoTip.h"
class CGrid_Comm : public CUGCtrl
{
public:
	CGrid_Comm();
	virtual ~CGrid_Comm();

public:
	int m_nLastSelRow;
	BOOL m_bSelectCheckFlag;

	int m_nViewCol;
	COLORREF m_clrHeader;
	COLORREF m_clrSelRow;

private:
	CFont				m_fontDefault;
	CPen	            m_penGridLine;

	CDrInfoTip			m_tipInfo;

	int		            m_nSortCol;
	int		            m_nSortMethode;
	int		            m_nSortArrow;
	CUGSortArrowType	m_sortArrow;
	long	            m_nArrowType;

	BOOL				m_bCallAdjustOneTime;

	CString				m_strType;  //# "CODE", "RESULT", "PROFIT"

	int					m_nCol_Hide;
	BOOL				m_bCol_HideState;

	HWND				m_hChildFrame;
	BOOL				m_bNoUserSorting;

public:	 // Member Functions
	void SetInit(CString strType);
	CWnd* GetFrameWnd(CWnd* pWnd);
	void SetCommMenu(POINT *point);
	void SetCommMenu_Add(CMenu* pMenu);
	CString GetCommMenu_IndexToScreenNo(int nIndex_SelMenu);

	void ClearAll();

	void SetAddRow(BOOL bChk, BaseData_Code stBaseData, BOOL bRedraw = FALSE);
	void SetAddRow(BOOL bChk, int nRow, BaseData_Code stBaseData, BOOL bRedraw = FALSE);


	void SetAddRow(AnalystInfo stAnalystInfo, BOOL bRedraw = FALSE);
	void SetAddRow_Result(AnalystInfo stAnalystInfo);
	void SetModifyRow_CodeData(int nRow, double dData, BOOL bRedraw = FALSE);

	void SetModifyRow(int nRow, BaseData_Code stBaseData, BOOL bRedraw = FALSE);

	void GetAttributeInfo(int nRow, char chAttribute, COLORREF &clrContrastText);

	void SetSelectRow(int nRow);
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();
	inline void SetBackColorIfNotRedBlue(CUGCell *pCell, COLORREF clrSet);

	BOOL GetRowToCodeInfo(int nRow, CodeInfo &stCodeInfo);
	int GetRowToMarketType(int nRow);
	void GetMarketTypeCnt(int &nKospiCnt, int &nKosdaqCnt);


	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);

	void SetSelectJong(CStringArray *psaSelectJong);
	void GetSelectJong(CStringArray &saSelectJong);

	void SetSelectFluctuating();

	void SetHaveSelectState();

	void SetViewCol(int nCol);
	void SetHideCol(int nCol, BOOL bView);

	void SetColResize(BOOL bState);
	void NoUseSorting(BOOL bNoUse)		{ m_bNoUserSorting = bNoUse; };
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	void SetSaveGridToExcel(CString strFileName);
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType);

	void GetRowCode(int nRow, CString &strCode, int &nMarkType);

	void ReSelectRow();
    
private:
	void HeaderSetup(CString strType);
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
	virtual int		OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	virtual void	OnAdjustComponentSizes(RECT *grid, RECT *topHdg, RECT *sideHdg, RECT *cnrBtn, RECT *vScroll, RECT *hScroll, RECT *tabs);
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
};

#endif // !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
