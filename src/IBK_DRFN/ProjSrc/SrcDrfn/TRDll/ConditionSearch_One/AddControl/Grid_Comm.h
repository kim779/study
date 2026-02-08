// ResultGrid.h: interface for the CGrid_Comm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "msxml.dll"
using namespace MSXML;
#include <afxtempl.h>   //<= CList

#include "../resource.h"

#include "../../../DrCommon/UGridCtrl/UGCtrl.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTbutn.h"
#include "../../../DrCommon/UGridCtrl/_Include/ugctelps.h"
#include "../../../DrCommon/UGridCtrl/_Include/UGCTsarw.h"

#include "../../../DrCommon/Dr_Control/DrInfoTip.h"

#include "../FieldEditDlg.h"

#include "../../../DrCommon/UGridCtrl/FindDataGridUtil.h"

class CGrid_Comm : public CUGCtrl
{
public:
	CGrid_Comm();
	virtual ~CGrid_Comm();
	
private:
	CFont				m_fontDefault;
	CPen				m_penGridLine;

	CDrInfoTip			m_tipInfo;
	
	int					m_nSortCol;
	int					m_nSortMethode;
	int					m_nSortArrow;
	CUGSortArrowType	m_sortArrow;
	long				m_nArrowType;
	
	BOOL				m_bCallAdjustOneTime;
	CUGButtonType		m_button;
	
	CUGButtonType		m_ugButtonType;
	int					m_nButtonIndex;
//	CString		m_strType;		//# "RESULT", "LIST", "GROUP", "REAL", "INDUSTRY"
	BOOL				m_bAdjustColSizes;
	
	CImageList			m_imgList;
	
	HWND				m_hChildFrame;

	CFieldEditDlg*		m_pFieldEditDlg;
	
public:
	CString				m_strType; //유진 수정 private->public
	int					m_nIndustry;
	int					m_nLastSelRow;
	int					m_nViewCol;
	int 				m_nLastCol;
	int 				m_nPrevRow;
	
	int					m_nOffset_CodeToRow;
	CMapStringToString  m_mapCodeToRow;
	CStringArray		m_saCode;
	BOOL				m_bChkDelOut;
	COLORREF			m_clrHeader;
	COLORREF			m_clrSelRow;

	CStringArray		m_saFieldList;

	//20100330 SJ_KIM 항목편집
	CList <HeadInfoType, HeadInfoType&> *m_plistField;
	int	m_nFontHeight; 
	int	m_nFontWidth; 
	int m_nFixCol;
	int	 m_nSortType[RESULT_FIELD_MAX];

	void SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
		CString strItemName, char dcolData[960], int nPrice, bool bRedraw);
	void SetAddRow_Alert_DynamicField(int nRow, int ntotCol, char dcolData[960], CString strItemName, 
		char chMarkType, char chSign, CString szItemCode, BOOL bRedraw, int nPrice);
	void SetAddRow_Real_DynamicField(SIGNAL_CONDITION stSIGNAL_CONDITION, BOOL bJobChk, BOOL bRedraw, int ntotCol);
	CString FindCandleIdFromName(long lCandleID);
	BOOL LoadCandleIndexToXml();
	BOOL LoadPatenIndexToXml();
	CString FindPatenIdFromName(long lPatenID);

	CList<CandleInfo, CandleInfo&> m_listCnadle;
	CList<PatenInfo, PatenInfo&> m_listPaten;
	BOOL m_bCandleLoadFalg;
	BOOL m_bPatenLoadFalg;
	/////////////////////////////////	

	int  m_nCol_Hide;
	BOOL m_bCol_HideState;
	BOOL m_bCol_ForecastPrice_HideState;
public:
	void SetInit(CString strType = "RESULT", int nImgID = -1);
	
	CWnd* GetFrameWnd(CWnd* pWnd);
	void SetCommMenu(POINT *point);
	void SetCommMenu_Add(int nIndustry, CMenu* pMenu);
	CString GetCommMenu_IndexToScreenNo(int nIndex_SelMenu);

	virtual void	OnSetup(CString strType, UINT nImgID = -1);
	
	void ClearAll();
	void HeaderSetup(CString strType, UINT nImgID = -1);
	void SetColResize(BOOL bState);
	
	void SetAddRow_Group(CString strName, int nID, BOOL bRedraw = FALSE);
	void SetAddRow_List(ConditionInfo	*pstConditionInfo, BOOL bRedraw = FALSE);
	void SetAddRow_Result(int nRow, TR_RESULT_LIST stTR_RESULT_LIST, BOOL bRedraw = FALSE);
	
	void SetAddRow_Alert(int nRow, TR_RESULT_LIST stTR_RESULT_LIST, BOOL bRedraw = FALSE);
	void SetAddRow_Real(SIGNAL_CONDITION stSIGNAL_CONDITION, BOOL bJobChk, BOOL bRedraw = FALSE);
	
	void SetSendData(int nRow);
	void SetSelectRow(int nRow);
	
	int  GetCodeColNumber(int nType=0);
	BOOL GetRowToCode(int nRow, CString &strCode, int &nMarketType);
	BOOL GetRowToCodeName(int nRow, CString& strCode, CString& strName, int& nMarketType);
	BOOL IsRowCodeChecked(int nRow);
	
	void SetSaveGridToExcel(CString strFileName);
	void SetSaveGridToText(CString strFileName);

	void DeleteOutItem();	
	int GetFluctuating(CUIntArray *piaFluctuating, double &dAvgRate);
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);

	//20100330 SJ_KIM 항목편집
	void SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField, CString szType);
	void SetLockColumns(int nCol, BOOL bOffSet = FALSE);
	CSize GetFontSize();
	/////////////////////////
	
	void SetHideCol(int nCol, BOOL bView);
	void SetVisibleForecastPriceCol(BOOL bView);

	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);

	void SetSignalAtGrid(CString strCode, CString strSignal, COLORREF clrBK, COLORREF clrText);

	BOOL GetMarketInfo(CString strType, CStringArray *psaCode, CStringArray *psaName);

	void GetRowCode(int nRow, CString &strCode, int &nMarkType);
	CString GetCellData(int nRow, int nCol);

	CString LoadMemo(CString strKey);
	void SaveMemo(CString strKey, CString strData);

private:
	BOOL SetDrew_SelectRow(int nRow);
	inline void GetSignColor(int nSign, COLORREF &clrSignText);
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();
	
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	CString GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	
	void SetSelectJong(CStringArray *psaSelectJong);
	void GetSelectJong(CStringArray &saSelectJong);
	
	void SetSelectFluctuating();
	void SetHaveSelectState();
	
	int SetMessageBox(LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);
	
	void SetHint_Code(int nRow);

	void GetFieldList();
	void SetFieldEdit();
	void GetCellColor(int nRow, int nCol, COLORREF &clrText, COLORREF &clrBack);
	CString GetStockStatus(CString strItemCode, CString strItemName, COLORREF& clrbunText, COLORREF& clrbunBack);
	
	BOOL m_bSendTR;

private: // Overrides
	//virtual void	OnSetup(CString strType);
	
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnRowChange(long oldrow, long newrow);
	
	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	virtual int		OnCanSizeTopHdg();
	virtual void	OnSysColorChange();
	virtual int		OnHint(int col, long row, int section, CString *string);

	virtual int  OnCanMove(int oldcol,long oldrow,int newcol,long newrow);

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
	//}}AFX_MSG
	
	afx_msg void OnDestroy();
	afx_msg void OnSelectMenu(UINT nIndex);
	afx_msg void OnTimer(UINT nIDEvent);

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
	BOOL		m_bSelectCheckFlag;
	
	void FnLButtonDown(CPoint point);
	void FnLButtonUp(CPoint point);
	void FnMouseMove(CPoint point);

	BOOL MakeDragImage(CString strText);
	void DragImageInfoClear();
	//>> 07.07.10 이준옥

	void	FnRButtonUp(CPoint point);
	void	OnBtnFieldEdit();
};


#endif // !defined(AFX_GRID_COMM_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
