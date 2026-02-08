// ResultGrid.h: interface for the CGrid_Result class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRID_RESULT_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
#define AFX_GRID_RESULT_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#import "msxml.dll"
using namespace MSXML;
#include <afxtempl.h>   //<= CList

#define RED     RGB(255, 0, 0)
#define YELLOW  RGB(255, 255, 0)
#define MAGENTA RGB(255, 0, 255)
#define WHITE   RGB(255, 255, 255)
#define BLUE    RGB(0, 0, 255)
#define BLACK		RGB(0, 0, 1)

#include "../../DrCommon/UGridCtrl/ugctrl.h"
#include "../../DrCommon/UGridCtrl/_Include/UGCTbutn.h"
#include "../../DrCommon/UGridCtrl/_Include/ugctelps.h"
#include "../../DrCommon/UGridCtrl/_Include/ugctsarw.h"

#include "../../DrCommon/Dr_Control/DrInfoTip.h"

#include "../../DrCommon/UGridCtrl/FindDataGridUtil.h"	//20110829:Add By JunokLee.


class CPrint4Grid;

class CGrid_Result : public CUGCtrl
{
public:
	CGrid_Result();
	virtual ~CGrid_Result();
	
private:
	CFont m_fontDefault, m_fontBold;
	CPen					m_penGridLine;
	
	CDrInfoTip    m_tipInfo;
	
	BOOL          m_bIndustry;      //# Out Set!!
	BOOL			m_bIsMultiSelect;
	CString			m_strType;

	//20070703 이문수 >>
public:
	short			m_nStyle4Print;
	CPen *			m_pBorderPen4Print;
	CPrint4Grid *	m_pPrint;
	CFont		*	m_pPrintTailFont;

	COLORREF	m_clrHeader;
	COLORREF	m_clrSelRow;
	//20070703 이문수 <<

public:
	int						m_nFixCol;
	
	int						m_nSortCol;
	int						m_nSortMethode;
	int						m_nSortArrow;
	
	int						m_nPrevRow;
	int						m_nLastSelRow;
	int						m_nLastCol;
	BOOL					m_bScreenMove;
	
	int						m_nFontHeight; 
	int						m_nFontWidth; 
	int						m_nNumFixField;
	
	long					m_nArrowType;
	CUGSortArrowType	m_sortArrow;
	
	VARIANT_BOOL m_bMoveSysLoadedXMLFile;
	VARIANT_BOOL m_bMoveUserLoadedXMLFile;
	VARIANT_BOOL m_bFixSysLoadedXMLFile;
	
	MSXML::IXMLDOMDocumentPtr m_pMoveSysDOM;   //# Field ID : 5XXX
	MSXML::IXMLDOMDocumentPtr m_pMoveUserDOM;  //# Field ID : 5XXX
	MSXML::IXMLDOMDocumentPtr m_pFixSysDOM;    //# Field ID : 9XXX
	
	CList <HeadFixInfo, HeadFixInfo&>			*m_plistFixHead;
	CList <HeadMoveInfo, HeadMoveInfo&>		*m_plistMoveHead;
	CList <HeadInfoType, HeadInfoType&>		*m_plistField;
	
	//## 캔들 지표 등록 ##
	BOOL m_bCandleLoadFalg;
	BOOL m_bPatenLoadFalg;
	
	int	 m_nSortType[RESULT_FIELD_MAX];
	
	CList<CandleInfo, CandleInfo&> m_listCnadle;
	CList<PatenInfo, PatenInfo&> m_listPaten;
	
	HWND m_hChildFrame;
	CUGButtonType m_ugButtonType;
	int m_nButtonIndex;
	
public:	 // Member Functions
	void ChangeGridColor(ST_GRIDCOLOR stGridColor);
	void SetInit();
	
	CWnd* GetFrameWnd(CWnd* pWnd);
	void SetCommMenu(POINT *point);
	void SetCommMenu_Add(BOOL bIndustry, CMenu* pMenu);
	CString GetCommMenu_IndexToScreenNo(int nIndex_SelMenu);
	
	void SetHeader(CList <HeadInfoType, HeadInfoType&> *plistField);
	void SetHeader_Init(CList <HeadFixInfo, HeadFixInfo&>	*plistFixHeadUser);
	
	void ClearAll();
	void SetAddRow(int nRow, char chSign, int ntotCol, char chMarkType, CString strItemCode, 
						CString strItemName, char dcolData[][16], int nPrice, bool bRedraw = false);
	
	CString GetFormatStr(long lData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	CString GetFormatStr(double dData, int nUnitNum, BOOL bFormatFlag, CString strFormat);
	
	void GotoLastRow();
	
	CSize	GetFontSize();
	
	inline void GetSignColor(int nSign, COLORREF &clrSignText);
	inline void GetEvenOddBkColor(int nRow, COLORREF &clrBk);
	void EvenOddColorSetting();
	
	//## Head Information 관련 Function ##
	BOOL HeadFixInsert();
	void HeadFixAllDelete();
	BOOL HeadFixJongCodeFlag();
	int HeadFixViewCount();
	
	void HeadMoveInsert(HeadMoveInfo stHeadInfo);
	void HeadMoveDelete(CString strElement);
	void HeadMoveAllDelete();
	void HeadMoveChange(int FrontIndex, int AfterIndex);
	void HeadMoveUpdate();
	BOOL HeadMoveSearch(int nIndexCount, CString strElement, CString strHeadID, 
		BOOL bView, BOOL bCondition);
	BOOL HeadMoveSysSearch(CString ExtractStr, HeadInfoType &stHeadType);
	BOOL HeadMoveSetStandLine(CList<OneElementInfo, OneElementInfo> &listElement);
	
	POSITION GetElementToMovePosition(CString strElement);
	
	//## 실제 보여줄 Field Count return 
	int HeaderDataSetup(BOOL bBackTest = FALSE, int nSearchType = 0, int nStepCnt = -1);  
	//# nSearchType   : 0(기본)
	//#               : 1(순차)
	//# nSetpCnt      : -1(일괄)
	//#               : n (단계)
	
	//########################################
	
	void LoadMoveSysFiledXML();
	void LoadMoveUserFiledXML();
	void LoadFixSysFiledXML();
	
	//## 캔들 지표를 Load ##
	BOOL LoadCandleIndexToXml();
	CString FindCandleIdFromName(long lCandleID);
	
	
	BOOL LoadPatenIndexToXml();
	CString FindPatenIdFromName(long lPatenID);
	
	BOOL GetRegStockItem(CStringArray &saRegStockItem);
	BOOL GetRegStockItem(CPtrArray	&paRegStockItem);
	
	void SetSharedData(int nRow);
	
	void GetRowCode(int nRow, CString &strCode, int &nMarkType);
	BOOL IsRowCodeChecked(int nRow);	//State of Code Check.
	void GetRowData(int nRow, BOOL bBackTest, CString &strCode, CString &strNowPrice);
	
	CString GetCellData(int nRow, int nCol);
	void GetCellColor(int nRow, int nCol, COLORREF &clrText, COLORREF &clrBack);
	
	void GetCellSignText(int nRow, int nCol, CString &strSignText);
	CString GetSignText(int nSign);
	
	void SetLockColumns(int nCol, BOOL bOffSet = TRUE);
	
	void SetHint_Code(int nRow);

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
	int OnCellTypeNotify(long ID,int col,long row,long msg,long param);
	CString GetStockStatus(CString strItemCode, CString strItemName, COLORREF& clrbunText, COLORREF& clrbunBack);

	BOOL GetRowToCodeName(int nRow, CString& strCode, CString& strName, int& nMarketType);

	void SaveMemo(CString strKey, CString strData);
	CString LoadMemo(CString strKey);

	void SetHeader();
	void SetAddRow(int nCol, int nRow, CString strCode, CString strName, BOOL bRedraw);

protected:
	/// 키보드 방향키와 종목연동처리 2009 0423 이민형
	BOOL			m_bIsKeyDown;
	
public: // Overrides
	virtual void	OnKeyDown(UINT *vcKey,BOOL processed);
	virtual void	OnSetup(BOOL bIndustry, CString strType = "");
	virtual void	OnTH_LClicked(int col, long row, int updn, RECT *rect , POINT *point , BOOL processed);
	virtual void	OnRClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnLClicked(int col, long row, int updn, RECT *rect, POINT *point, BOOL processed);
	virtual void	OnDClicked(int col,long row, RECT *rect, POINT *point, BOOL processed);
	virtual int		OnSortEvaluate(CUGCell *cell1,CUGCell *cell2,int flags);
	virtual int		OnCanSizeTopHdg();
	virtual void	OnRowChange(long oldrow, long newrow);
	virtual void	OnSysColorChange();
	virtual int		OnHint(int col, long row, int section, CString *string);
	
	virtual int OnCanViewMove(int oldcol, long oldrow, int newcol, long newrow);
	
	
	
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
	afx_msg void OnSelectMenu(UINT nIndex);
	
	afx_msg long OnChangeChildFrameWindowHandle(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
		
//<< 07.07.10 이준옥 Drag&Drop추가. 삼성에 맞게
	int	 GetCodeColNumber(int nType=0);
	BOOL GetRowToCode(int nRow, CString &strCode, int &nMarketType);

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
	UINT		m_uGridNotifyControlMsg;
	BOOL		m_bFocus;
	long		m_lDragRow;
	int			m_nDragCol;
	BOOL		m_bSelectCheckFlag;	//종목코드 체크
	
	void FnLButtonDown(CPoint point);
	void FnLButtonUp(CPoint point);
	void FnMouseMove(CPoint point);

	BOOL MakeDragImage(CString strText);
	void DragImageInfoClear();
	//>> 07.07.10 이준옥
	
	void	FnRButtonUp(CPoint point);

};

#endif // !defined(AFX_GRID_RESULT_H__312CE096_5941_4D50_9D51_7018BF0EF9AF__INCLUDED_)
