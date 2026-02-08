#if !defined(AFX_NEOCHART2001CTL_H__D585DB92_DE13_4572_BC53_1A626B82D052__INCLUDED_)
#define AFX_NEOCHART2001CTL_H__D585DB92_DE13_4572_BC53_1A626B82D052__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../Include_Chart/Dll_Load/XScaleManager.h"	// for CXScaleManager
#include "BaseAllGetterSetter.h"						// for IBaseAllGetterSetter
#include "IndicatorDefaultData.h"						// for CIndicatorDefaultData
#include "OutsideCommonInterfaceFunction.h"				// for COutsideCommonInterfaceFunction
#include "BackgroundDrawer.h"							// for CBackgroundDrawer
#include "SaveChartCfgList.h"							// for CSaveChartCfgList
#include "FontKey.h"									// for CFontKey
#include "GlobalSharedData.h"							// for CGlobalSharedClient
#include "OutsideCommonInterface.h"
#include "OcxIDs.h"										// for _OCX_PROG_ID

// 툴바DLL에서 사용할 이미지 버튼 파일.
#define TOOLBAR_BUTTON_IMAGEFILE _T("Chart_InnerToolBar.bmp")

// (2004.05.04, 배승원) 동양 ASP, KTBChart는 Packet Data Count를 5로 고정시키는 것으로 처리한다.
#define	PACKET_COUNT_LENGTH	5

// (2008/2/15 - Seung-Won, Bae) for TEST of Multi-Language Version in OLD AUP.
#define _ML_TEST_NOT_USED()		AfxMessageBox( "I did not prepare this operation in Multi-Language Version.\r\nPlease, call chart developer. thanks.");

/////////////////////////////////////////////////////////////////////////////
// CKTBChartCtrl : See NeoChart2001Ctl.cpp for implementation.

class CSaveUserFile
{
	public:
	enum FILETYPE
	{
		ENVIRONMENT,	// 환경설정
		INDICATORLIST,	// 지표조건
		SERIESLIST,		// 
		// 분석툴 종목별 저장 - ojtaso (20080728)
		ANALTOOL		// 분석툴 
	};

	enum FILEOPENSTYLE
	{
		WRITE,			// Write
		READ,			// Read
		CREATE			//생성
	};
};

class CNeoChart2001In;	
class CNeoChart2001Out;
class CChartCfg;
class CRealTableAdvisedInfoListHW;
class CStopLoss;
class CResourceManager;
class CAddInManager;
class CIWChartCtrl;
class CBaseDesk2;
class CLocalShareInfoList;
class IMetaTableOCX;
class CRealSettingInfoList;
class CDrdsAdvisedInfoList;
class CToolBarMgrHelper;
class CXScaleManager;
class CPacketListManager;		// 복수의 PacketList를 관리 : 복수종목 - ojtaso (20061224)
class CMainBlock;
interface ISharedDataMgr;
interface IRealTableMgr;
interface IToolBarManager;
interface IChartCtrl;
interface IPropertyMap;
interface IRealReceiver;

class CKTBChartCtrl : public COleControl
{
	DECLARE_DYNCREATE(CKTBChartCtrl)
// Constructor
public:
	CKTBChartCtrl();

// (2006/1/7 - Seung-Won, Bae) COleControl Interface Routines for IChartCtrl
friend CIWChartCtrl;
protected:
	CIWChartCtrl *	m_pIChartCtrl;
// (2006/6/7 - Seung-Won, Bae) Get IChartCtrl Interface for access protected member of Ctrl Class
public:
	IChartCtrl *	GetIChartCtrl( void);

//	NeoChart I/O Base Class
public:
	CNeoChart2001In* m_pPacketInput;	
	CNeoChart2001Out* m_pPacketOutput;
// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다. 
//		첫 PacketOut시에 Packet Count Data Size를 설정하기 위하여 Packet List Object에 접근하도록
//		CNeoChart2001Out를 Friend Class로 선언한다.
friend class CNeoChart2001Out;

//	NeoChart Base Desk Class
public:
	CBaseDesk2*	m_pBaseDesk;
	HWND GetBaseDeskHWnd();
	CString GetBaseDeskPath();
	LPUNKNOWN GetDeskUnkownID() ;
	LPUNKNOWN GetCtrlUnkownID(LPCTSTR szName) ;

	// 종찬(05/05/10) Hts의 패스값을 가지고 있는 멤버변수
	CString m_strHtsPath;
	// (2006/2/27 - Seung-Won, Bae) Manage User File Path for AddIn
	CString	m_strUserFilePath;

	//bitmap, print
	void OnDrawForPrint(CDC& memoryDC,	CBitmap& bufferBitmap,	CRect rcSource,	BOOL bScreenColor = TRUE);
	void OnDrawStockItemCode(CDC& memoryDC,	CRect& rect);
	void OnDrawUsingOptimizingcolor(CDC& memoryDC,	CBitmap& bufferBitmap,	CRect rect);
	void OnDrawUsingScreencolor(CDC& memoryDC,	CBitmap& bufferBitmap,	CRect rect);
	void DrawChart( CDC &p_memoryDC, CDC &p_dcThread, CRect rect, BOOL p_bRealDraw = FALSE);

	// cj 2003.4.25 십자선 누르기 전에 수치조회창을 눌렀었는지 확인해보기 위함
	BOOL IsUseInquiryData() { return m_bUseInquiryData; };
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKTBChartCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void OnResetState();
	virtual BOOL DestroyWindow();
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CKTBChartCtrl();

	DECLARE_OLECREATE_EX(CKTBChartCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CKTBChartCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CKTBChartCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CKTBChartCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CKTBChartCtrl)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDeleteBlock();
	afx_msg void OnReturnOrg();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnReturnOrgDisplay();
	afx_msg void OnPrint();
	afx_msg void OnToExcel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCaptureChanged(CWnd *pWnd);
	afx_msg void OnUpdateReverseYscale(CCmdUI* pCmdUI);
	afx_msg void OnReverseYscale();
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnPricechartMinmax();
	afx_msg void OnUpdatePricechartMinmax(CCmdUI* pCmdUI);
	afx_msg void OnScreenMinmax();
	afx_msg void OnUpdateScreenMinmax(CCmdUI* pCmdUI);
	afx_msg void OnMenuCrossline();
	afx_msg void OnUpdateMenuCrossline(CCmdUI* pCmdUI);
	afx_msg void OnMenuDatawindow();
	afx_msg void OnUpdateMenuDatawindow(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnReturnToGeneralChart();
	afx_msg void OnUpdateDeleteBlock(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReturnOrgDisplay(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReturnOrg(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToExcel(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePrintScreen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLoadMapGwan(CCmdUI* pCmdUI);
	afx_msg void OnUpdateReturnToGeneralChart(CCmdUI* pCmdUI);
	afx_msg void OnGlancebalanceCursor();
	afx_msg void OnUpdateGlancebalanceCursor(CCmdUI* pCmdUI);
	afx_msg void OnHideGraphTitle();
	afx_msg void OnUpdateHideGraphTitle(CCmdUI* pCmdUI);
	afx_msg void OnShowMinmaxText();
	afx_msg void OnUpdateShowMinmaxText(CCmdUI* pCmdUI);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
		LRESULT OnCursorExchange(WPARAM wParam, LPARAM lParam);
		LRESULT OnSetEtc(WPARAM wParam, LPARAM lParam);	
		LRESULT OnGraphSelectedIndex(WPARAM wParam, LPARAM lParam);
		//nami 11.26
		LRESULT OnGetUserMargin(WPARAM wParam, LPARAM lParam);
		LRESULT OnToolFunction(WPARAM wParam, LPARAM lParam);
		// (2006/10/18 - Seung-Won, Bae) Add AddIn Message Proc.
		LRESULT OnAddInMsg(WPARAM wParam, LPARAM lParam);
		// (2007/2/21 - Seung-Won, Bae) Draw in MemDC for Print or Save
		LRESULT OnDrawForPrint( WPARAM wParam, LPARAM lParam);	// wParam - 0, lParam - HDC
		// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI (Zoom)
		LRESULT OnReCalcDRI( WPARAM wParam, LPARAM lParam);
		// (2008/10/21 - JS, Kim) User Changes Time Difference
		LRESULT OnChangeTimeDiff( WPARAM wParam, LPARAM lParam);// wParam - Time Index, lParam - 0
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	//{{AFX_DISPATCH(CKTBChartCtrl)
	short m_nChartMode;
	afx_msg void OnNChartModeChanged();
	BOOL m_bNormalizedMinuteTimeRule;
	afx_msg void OnbNormalizedMinuteTimeRule();
	long m_nBlockType;
	afx_msg void OnnBlockTypeChanged();
	long m_nBlockCompart;
	afx_msg void OnnBlockCompartChanged();
	long m_nBlockRowCount;
	afx_msg void OnnBlockRowCountChanged();
	long m_nBlockColumnCount;
	afx_msg void OnnBlockColumnCountChanged();
	long m_nBlockGap;
	afx_msg void OnnBlockGapChanged();
	long m_nBlockBorderThickness;
	afx_msg void OnnBlockBorderThicknessChanged();
	CString m_strAddInLoadList;
	afx_msg void OnStrAddInLoadListChanged();
	long m_nBorderType;
	afx_msg void OnnBorderTypeChanged();
	long m_nNotUsedProperty;
	afx_msg void OnNotUsedPropertyChanged();
	BOOL m_bWholeView;
	afx_msg void OnbWholeViewChanged();
	long m_nOnePageDataCount;
	afx_msg void OnnOnePageDataCountChanged();
	long m_nUserBlockAddCount;
	afx_msg void OnnUserBlockAddCountChanged();
	BOOL m_bGraphDragDrop;
	afx_msg void OnbGraphDragDropChanged();
	long m_nLeftMargin;
	afx_msg void OnnLeftMarginChanged();
	long m_nRightMargin;
	afx_msg void OnnRightMarginChanged();
	long m_nTopMargin;
	afx_msg void OnnTopMarginChanged();
	long m_nBottomMargin;
	afx_msg void OnnBottomMarginChanged();
	BOOL m_bAddGraphMenuShow;
	afx_msg void OnbAddGraphMenuShowChanged();
	CString m_strUserEnvironmentFileName;
	afx_msg void OnstrUserEnvironmentFileNameChanged();
	CString m_strUserIndicatorListFileName;
	afx_msg void OnstrUserIndicatorListFileNameChanged();
	CString m_strNotUsedProperty;
	BOOL m_bHide;
	afx_msg void OnbHideChanged();
	BOOL m_bIsInput;
	afx_msg void OnbIsInputChanged();
	CString m_strToolbarOption;
	afx_msg void OnstrToolbarOptionChanged();
	CString m_strPacketOption;
	afx_msg void OnstrPacketOptionChanged();
	CString m_strLocalSharedList;
	afx_msg void OnstrLocalSharedListChanged();
	BOOL m_bUseRealData;
	afx_msg void OnbUseRealDataChanged();
	CString m_strRealList;
	afx_msg void OnstrRealListChanged();
	short m_nMarginBorderType;
	afx_msg void OnNMarginBorderTypeChanged();
	long m_nRowDoubleSizeBlock;
	afx_msg void OnNRowDoubleSizeBlockChanged();
	BOOL m_bUseInquiryDlgWhenUseCrossLine;
	afx_msg void OnBUseInquiryDlgWhenUseCrossLineChanged();
	long m_nColumnDoubleSizeBlock;
	afx_msg void OnNColumnDoubleSizeBlockChanged();
	BOOL m_bUseShortKey;
	afx_msg void OnBUseShortKeyChanged();
	BOOL m_bUseConditiontoTitle;
	afx_msg void OnBUseConditiontoTitleChanged();
	BOOL m_bUseDatatoTitle;
	afx_msg void OnBUseDatatoTitleChanged();
	short m_nHorzGap;
	afx_msg void OnNHorzGapChanged();
	short m_nVertGap;
	afx_msg void OnNVertGapChanged();
	short m_nRButtonMenuType;
	afx_msg void OnNRButtonMenuTypeChanged();
	BOOL m_bShowWarningMessageBox;
	afx_msg void OnBShowWarningMessageBoxChanged();
	BOOL m_bControlBlockSize;
	afx_msg void OnBControlBlockSizeChanged();
	BOOL m_bBlockMoveDelete;
	afx_msg void OnBBlockMoveDeleteChanged();
	BOOL m_bSelectTitle;
	afx_msg void OnBSelectTitleChanged();
	BOOL m_bSelectGraph;
	afx_msg void OnBSelectGraphChanged();
	BOOL m_bUseScaleRButtonMenu;
	afx_msg void OnBUseScaleRButtonMenuChanged();
	short m_nGraphRButtonType;
	afx_msg void OnNGraphRButtonTypeChanged();
	short m_nHoldingPacketSize;
	afx_msg void OnNHoldingPacketSizeChanged();
	CString m_szBasePacketName;
	afx_msg void OnSzBasePacketNameChanged();
	short m_nMinDataCountOnOnePage;
	afx_msg void OnNMinDataCountOnOnePageChanged();
	BOOL m_bFireRealEvent;
	afx_msg void OnBFireRealEventChanged();
	short m_nInquiryType;
	afx_msg void OnNInquiryTypeChanged();
	BOOL m_bOcxAreaNumericalInquiry;
	afx_msg void OnBOcxAreaNumericalInquiryChanged();
	long m_nNumericalInquiryOption;
	afx_msg void OnNNumericalInquiryOptionChanged();
	CString m_strGlobalSharedList;
	afx_msg void OnStrGlobalSharedListChanged();
	short m_nUseInquiryWithCrossline;
	afx_msg void OnNUseInquiryWithCrosslineChanged();
	BOOL m_bShowPriceRateInquiry;
	afx_msg void OnBShowPriceRateInquiryChanged();
	BOOL m_bDoesUseSkinColor;
	afx_msg void OnBDoesUseSkinColorChanged();
	CString m_strMapName;
	afx_msg void OnStrMapNameChanged();
	short m_nBongCountMargin;
	afx_msg void OnNBongCountMarginChanged();
	BOOL m_bOnePageDataCountInTRCount;
	afx_msg void OnBOnePageDataCountInTRCountChanged();
	BOOL m_bNotUsedProperty;
	short m_nPacketCountDataSize;
	afx_msg void OnNPacketCountDataSizeChanged();
	short m_nLanguageID;
	afx_msg void OnNLanguageIDChanged();
	BOOL m_bFreezedPageState;
	afx_msg void OnBFreezedPageStateChanged();
	CString m_strPagePathWithPrefix;
	afx_msg void OnStrPagePathWithPrefixChanged();
	short m_nRealTimeTruncationType;
	afx_msg void OnNRealTimeTruncationTypeChanged();
	BOOL m_bUseGapRevision;
	afx_msg void OnBUseGapRevisionChanged();
	afx_msg BOOL GetIsShowInneerToolBar();
	afx_msg BSTR GetStrChartList();
	afx_msg void SetStrChartList(LPCTSTR lpszNewValue);
	afx_msg OLE_COLOR GetClrBackground();
	afx_msg void SetClrBackground(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrBackBorder();
	afx_msg void SetClrBackBorder(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrBlock();
	afx_msg void SetClrBlock(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrBlockBorder();
	afx_msg void SetClrBlockBorder(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrScaleLine();
	afx_msg void SetClrScaleLine(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrScaleText();
	afx_msg void SetClrScaleText(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrGraphBase1();
	afx_msg void SetClrGraphBase1(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrGraphBase2();
	afx_msg void SetClrGraphBase2(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrGraphBase3();
	afx_msg void SetClrGraphBase3(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrGraphBase4();
	afx_msg void SetClrGraphBase4(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrGraphBase5();
	afx_msg void SetClrGraphBase5(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrGraphBase6();
	afx_msg void SetClrGraphBase6(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrAnalysisTool();
	afx_msg void SetClrAnalysisTool(OLE_COLOR nNewValue);
	afx_msg BSTR GetStrBlockRegion();
	afx_msg void SetStrBlockRegion(LPCTSTR lpszNewValue);
	afx_msg OLE_COLOR GetClrScaleLine2();
	afx_msg void SetClrScaleLine2(OLE_COLOR nNewValue);
	afx_msg OLE_COLOR GetClrInquiryWindow();
	afx_msg void SetClrInquiryWindow(OLE_COLOR nNewValue);
	afx_msg BSTR GetCustomFont();
	afx_msg void SetCustomFont(LPCTSTR lpszNewValue);
	afx_msg OLE_COLOR GetClrLattice();
	afx_msg void SetClrLattice(OLE_COLOR nNewValue);
	afx_msg BSTR GetStartDateOfData();
	afx_msg BOOL OnRunningOneChart();
	afx_msg void SetEnableHorizontalLineInNIDLG(BOOL bNewValue);
	afx_msg BOOL GetBShowPriceMinMaxText();
	afx_msg void SetBShowPriceMinMaxText(BOOL bNewValue);
	afx_msg BOOL GetBHideAllTitle();
	afx_msg void SetBHideAllTitle(BOOL bNewValue);
	afx_msg BSTR GetStrMapIndicatorInfo();
	afx_msg BOOL GetBEnableIndicatorCalculation();
	afx_msg void SetBEnableIndicatorCalculation(BOOL bNewValue);
	afx_msg long GetNCurrentDrawCount();
	afx_msg long GetNFullDataCount();
	afx_msg BOOL	SetAllProperties2(long dwData);
	afx_msg void	SetBaseDesk(long lKey);
	afx_msg long	GetBase(LPCTSTR szKey);
	afx_msg short	SetChartData(long pChartData, short nCount, short nType);
	afx_msg short	SetChartDataStr(LPCTSTR szChartData, short nCount, short nType);
	afx_msg short	testGetPacketdatasize(LPCTSTR pIn);
	afx_msg void	testLoadAnalysisTools(short nToolOption, short nOption);
	afx_msg void	MakeSaveShortKey(LPCTSTR szFileName, short nFlag);
	afx_msg short	NotUsedMethod5(short nBlockRowPosition, short nBlockColumnPosition, LPCTSTR strGraphTitle, LPCTSTR strGraphName, LPCTSTR strPacketName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, short nLineWidth, OLE_COLOR clrLineColor);
	afx_msg short	ClearChartData(short nOption);
	afx_msg short	SetGraphHide(short nRow, short nColumn, LPCTSTR strTitle, BOOL bHide);
	afx_msg BOOL	AddChartBlock(LPCTSTR strChartName);
	afx_msg void	SetOnePageDataCount(long nOnePageDataCount);
	afx_msg long	GetOnePageDataCount();
	afx_msg BOOL	SaveandOpenFromFileWithASave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	afx_msg BOOL	JoinandDevisionforCompareChart(long nRowCount, long nColumnCount, BOOL bJoin);
	afx_msg BOOL	ChangeGraphTitle(short nRowIndex, short nColumnIndex, short nGraphIndex, LPCTSTR strNewTitle);
	afx_msg short	SetChartDataFromFile(LPCTSTR szFileName, short nType);
	afx_msg BOOL	RemoveChart(LPCTSTR szIndicatorName);
	afx_msg BOOL	ChangeGraphType(LPCTSTR strGraphName, LPCTSTR strGraphType, LPCTSTR strGraphStyle, LPCTSTR strPacketNameList, BOOL bRunatOnce);
	afx_msg void	SetChartAllInfo();
	afx_msg BSTR	GetData(LPCTSTR strPacketName, short nDataIndex);
	afx_msg BOOL	SetDrdsAdvise(BOOL bOnAdvise);
	afx_msg short	GetDataCount();
	afx_msg BOOL	ChangeChartItem(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce);
	afx_msg BOOL	NotUsedMethod7(BOOL bIsStopLoss, double dStopLossPrice, BOOL bIsProfit, double dProfitPrice);
	afx_msg BOOL	RestoreMinMax(LPCTSTR strPacketName);
	afx_msg BOOL	RestoreMin(LPCTSTR strPacketName);
	afx_msg BOOL	RestorMax(LPCTSTR strPacketName);
	afx_msg BOOL	SetMin(LPCTSTR strPacketName, double dMin);
	afx_msg BOOL	SetMax(LPCTSTR strPacketName, double dMax);
	afx_msg BOOL	SetMinMax(LPCTSTR strPacketName, double dMin, double dMax);
	afx_msg BOOL	SetHorzScaleData(short nRowIndex, short nColumnIndex, LPCTSTR strScaleData);
	afx_msg double	GetMin(LPCTSTR strPacketName);
	afx_msg double	GetMax(LPCTSTR strPacketName);
	afx_msg BOOL	SetGraphHide2(short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, short nSubGraphIndex, BOOL bHide);
	afx_msg BOOL	NotUsedMethod1(short nRowIndex, short nColumnIndex, short nVerticalIndex, short nGraphIndex, LPCTSTR strNewGraphName, LPCTSTR strNewTitle, LPCTSTR strNewPacketName, LPCTSTR strNewGraphType, LPCTSTR strNewGraphStyle, short nNewUpLineWidth, short nNewDownLineWidth, short nNewEqualLineWidth, BOOL bRunatOnce);
	afx_msg void	SaveandApplyGraphStyle(short nSaveDataType, short nChangeDataType);
	afx_msg void	SetDataRegionInBlock(short nDataStartIndex);
	afx_msg void	SetItemInfo(LPCTSTR lpstrCode, LPCTSTR lpstrName);
	afx_msg void	AddandDeleteIndicator(LPCTSTR strGraphName);
	afx_msg long	GetAllProperties2(long lpData);
	afx_msg long	GetBasePropIf();
	afx_msg BOOL	SetVertScaleData(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, LPCTSTR strVertScaleData);
	afx_msg BOOL	NotUsedMethod4(short nBlockType, short nRowCount, short nColumnCount);
	afx_msg BOOL	ChangeHorzScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex);
	afx_msg BOOL	ResetChart();
	afx_msg void	SetRealData(LPCTSTR strPacketName, double dData, short nOption);
	afx_msg BOOL	SetGraphTitle(LPCTSTR strGraphName, short nSubGraphIndex, LPCTSTR strNewGraphTitle, BOOL bGraphShowing);
	afx_msg BSTR	GetIndicatorNameList(LPCTSTR p_strGroup);
	afx_msg void	AddGraph(short nRow, short nColumn, short nVertScaleGroup, LPCTSTR szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);
	afx_msg void	AddPacket(LPCTSTR szPacketOption);
	afx_msg BOOL	RemovePacket(LPCTSTR szPacketName);
	afx_msg void	SetContainerDllInfo(long p_lDllWnd, short p_nID);
	afx_msg BOOL	SetPacketData(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset);
	afx_msg void	SetBackgroundColors(OLE_COLOR p_rgbBlockArea, OLE_COLOR p_rgbScaleArea);
	afx_msg BOOL	AddRealItem(LPCTSTR szRealInfo);
	afx_msg void	ShowIndicatorConfigDlg();
	afx_msg void	ResetChartCfgWith(short p_nIndex);
	afx_msg BOOL	EmptyChart(BOOL bIsDeleteAllPacket, BOOL bIsDefault);
	afx_msg BOOL	HasGraph(LPCTSTR p_szGraphName);
	afx_msg void	RemoveRealItem(LPCTSTR szRealInfo);
	afx_msg void	ShowInnerToolBar(BOOL p_bShow);
	afx_msg BOOL	ChangeVertScalePosition(short nScalePosition, short nRowIndex, short nColumnIndex, short nVSGroupIndex);
	afx_msg short	GetRowBlockCount();
	afx_msg short	GetColumnBlockCount();
	afx_msg BOOL	SetStartPeriodOfPacket(LPCTSTR p_szPacketName, long p_lStartPeriod, BOOL p_bAddSpace);
	afx_msg long	GetStartPeriodOfPacket(LPCTSTR p_szPacketName);
	afx_msg BOOL	GetStartEndDateOnView(double FAR* p_pStartDate, double FAR* p_pEndDate);
	afx_msg BOOL	SetStartEndDateOfView(double p_dStartDate, double p_dEndDate);
	afx_msg BOOL	ScrollToDataOfEnd(double p_dData);
	afx_msg short	ZoomToWithRightFix(short p_nZoomValue, BOOL p_bWithRightFix);
	afx_msg void	ClearTimeMark();
	afx_msg void	ShowEnvironmentDlg();
	afx_msg void	SetTimeMark(LPCTSTR p_szMarkTime, BOOL p_bWithScroll);
	afx_msg short	GetVertScalePosition(short p_nRowIndex, short p_nColumnIndex, short p_nVSGroupIndex);
	afx_msg void	EnableBoundMark(BOOL p_bEnable);
	afx_msg void	EnableReceivingTimeShare(BOOL p_bEnable);
	afx_msg BOOL	ChangePacketType(LPCTSTR p_szPacketName, LPCTSTR p_szPacketType);
	afx_msg BOOL	ChangeGraphDrawingData(LPCTSTR p_szIndicatorName, BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData);
	afx_msg BOOL	ChangePriceGraphDrawingData(BOOL p_bLowHigh, BOOL p_bLowCur, BOOL p_bHighCur, BOOL p_bPrevCur, short p_nCurData, short p_nDefaultType);
	afx_msg void	ShowAnalysisToolText(BOOL p_bTimeLine, BOOL p_bPriceLineLeft, BOOL p_bPriceLineRight, BOOL p_bTrendLine, BOOL p_bTrisectionLine, BOOL p_bQuadrisectionLine, BOOL p_bFibonacciRetraceLine);
	afx_msg BOOL	HasGraphInSpecialBack(LPCTSTR p_szGraphName);
	afx_msg BOOL	LoadAddIn(LPCTSTR p_szAddInDllName);
	afx_msg BOOL	InvokeAddIn(LPCTSTR p_szAddInDllName, short p_nCommandType, long p_lData);
	afx_msg void	SetDragModeType(short p_nDragModeType);
	afx_msg BOOL	ResetRealListString(LPCTSTR p_szRealList, BOOL p_bAdviseNow);
	afx_msg BOOL	AddOrRemoveChart(LPCTSTR strGraphName);
	afx_msg void	SetHtsPath(LPCTSTR pcszHtsPath);
	afx_msg BSTR	GetRealSettingInfo();
	afx_msg void	SetAllLogScale(BOOL bLogScale);
	afx_msg void	GetListViewerData(BSTR FAR* bstrTitle, BSTR FAR* bstrHorzScale, BSTR FAR* bstrPacketNamesPerBlock, BSTR FAR* bstrPacketNames, BSTR FAR* bstrSaveFilePath, BOOL FAR* bViewPacketSequence, long FAR* lpPacketList);
	afx_msg void	SetDataIndexByDate(LPCTSTR pcszStartDate, LPCTSTR pcszEndDate);
	afx_msg void	SetRunTimeMode(BOOL bRunTimeMode);
	afx_msg BSTR	GetPacketOption();
	afx_msg void	SetPacketOption(LPCTSTR strPacketOption);
	afx_msg void	SetOutsideCommonInterface(long lpOutsideCommonInterface);
	afx_msg BOOL	RemoveGraph_Title(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle, BOOL bIsOnlyNoTRData, BOOL bIsAutoHorzScalePosition);
	afx_msg BSTR	GetGraphPacketName(short nRowIndex, short nColumnIndex, LPCTSTR szGraphTitle);
	afx_msg BOOL	ChangeGraphTitle2(LPCTSTR szTargetTitle, LPCTSTR szSourceTitle);
	afx_msg BOOL	PartitionAllGraph();
	afx_msg BOOL	JoinAllGraph(BOOL bIsOneVertScaleGroup);
	afx_msg BOOL	NotUsedMethod3(LPCTSTR szTargetGraphName, LPCTSTR szSourceGraphName, BOOL bKeepOldGraphColor);
	afx_msg BOOL	ChangeIndicatorPacketName(LPCTSTR strIndicatorName, short nSubGraphIndex, LPCTSTR strNewPacketName);
	afx_msg void	RunEnvironmentDlg(short nOption);
	afx_msg BOOL	SetIndicatorCondition(LPCTSTR strGraphName, short nConditionType, short nConditionIndex, double dConditionValue, BOOL bReCalculate);
	afx_msg BOOL	SetPacketData2(LPCTSTR strPacketName, LPCTSTR strPacketType, LPCTSTR strPacketData, short nDataCount, BOOL bReCalculate);
	afx_msg BOOL	SetLogScale(LPCTSTR strGraphName, BOOL bLogScale);
	afx_msg BOOL	GetLogScale(LPCTSTR strGraphName);
	afx_msg BOOL	SetPacketHelpMessage(LPCTSTR strHelpMsg, BOOL bIsAllRemove);
	afx_msg BOOL	InvokeAddInStr(LPCTSTR p_szAddInDllName, LPCTSTR p_szCommandName, LPCTSTR p_szData);
	afx_msg BOOL	SetHelpMessageRealDataRate(LPCTSTR strHelpMessageData, BOOL bIsRemove);
	afx_msg void	SetBongCountMargin(short nBongCountMargin);
	afx_msg BOOL	SetAllPropertiesFromFile(LPCTSTR strFilePath, LPCTSTR strFileName);
	afx_msg BOOL	SetGraphHide3(LPCTSTR strTitle, BOOL bIsHide);
	afx_msg BOOL	GetBoundMarkFlag();
	afx_msg BOOL	UnloadAddIn(LPCTSTR p_szAddInDllName);
	afx_msg void	SetOnePageDataCount2(long nOnePageDataCount, BOOL bIncludeBongCountMargin);
	afx_msg BOOL	ClearPacketData(LPCTSTR strPacketName, BOOL bIsAbsoluteClear);
	afx_msg void	SetAllGraphHide(short nRowIndex, short nColumnIndex, short bHide);
	afx_msg void	InitStaticIndiCond(LPCTSTR p_szBasePath);
	afx_msg void	SetAllGraphsHide(short nRowIndex, short nColumnIndex, BOOL bHide);
	afx_msg BSTR	GetPacketData2(LPCTSTR strPacketName, short nDataIndex, BOOL bIsFullDataSize);
	afx_msg BSTR	NotUsedMethod6(short nPoint_x, short nPoint_y);
	afx_msg short	GetPacketIndex(LPCTSTR szPacketName, double dKeyValue);
	afx_msg short	GetCurrentPriceChartType();
	afx_msg BOOL	RemoveBlock(short nPoint_x, short nPoint_y);
	afx_msg BOOL	RemoveGraph(short nRowIndex, short nColumnIndex, LPCTSTR strGraphTitle);
	afx_msg BOOL	SetGraphColor(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, short nGraphIndex, short nSubGraphIndex, LPCTSTR strNewColor);
	afx_msg BOOL	SetPacketDataInIndex(LPCTSTR strPacketName, short nIndex, LPCTSTR strData, BOOL bIsRealData);
	afx_msg BOOL	SetVertScaleLogType(short nRowIndex, short nColumnIndex, BOOL bIsLogType);
	afx_msg BOOL	OpenFileOfEnvAndIndi(LPCTSTR strFileName_Env, LPCTSTR strFileName_Indi, BOOL bIsCurrentSave);
	afx_msg BOOL	SetPacketDataInAdditionPacket(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset);
	afx_msg void	SetOnePageDataCountInTRCount(BOOL bIsTRCount, BOOL bIsNowChange);
	afx_msg void	PrintChatImage();
	afx_msg void	ExportToExcel();
	afx_msg BOOL	_RunToolFunction( short nToolCommand, short nOption);
	afx_msg BOOL SetSubIndicatorCondition(LPCTSTR strGraphName, short nConditionType, short nSubGraphIndex, short nConditionIndex, double dConditionValue, BOOL bReCalculate);
	afx_msg BOOL SetGraphHide4(LPCTSTR strGraphName, short nSubGraphIndex, BOOL bHide);
	afx_msg BOOL SetMapDefaultInCurrent(BOOL bIsNewMapDefault);
	afx_msg BOOL NotUsedMethod2(LPCTSTR p_szTargetFName, LPCTSTR p_szNewFName, LPCTSTR p_szNewSGName);
	afx_msg BOOL GetHeadAndTailData(LPCTSTR strPacketName, double FAR* pHeadData, double FAR* pTailData);
	afx_msg LPUNKNOWN GetAddInCustomInterface(LPCTSTR p_szAddInName);
	afx_msg BOOL LoadAddInWithOverExclusive(LPCTSTR p_szAddInName);
	afx_msg BOOL OpenFromFile(LPCTSTR p_szFileName, BOOL p_bAutoPrevSave);
	afx_msg BOOL SaveandOpenFromFileWithMSave(LPCTSTR strOpenFileName, LPCTSTR strSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	afx_msg BOOL AddRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar);
	afx_msg void RemoveRealItemWithSeparator(LPCTSTR szRealInfo, LPCTSTR szSeparatorChar);
	afx_msg long GetAllProperties3(long p_lIPropertyMap);
	afx_msg long SetAllProperties3(long p_lIPropertyMap);
	afx_msg BOOL ShowIndicatorConfigDlgWithName(LPCTSTR p_szIndicatorName);
	afx_msg BOOL RemoveBlockWithIndex(short p_nRowIndex, short p_nColumnIndex);
	afx_msg BOOL SetBinTrHelpMsg(LPCTSTR p_szHelpMessage);
	afx_msg BOOL SetBinTrDoubleData(LPCTSTR p_szPacketName, long p_nCount, double FAR* p_pdData);
	afx_msg BOOL SetBinTrStringData(LPCTSTR p_szPacketName, long p_nCount, long p_lData);
	afx_msg BOOL SetBinTrEnd();
	afx_msg BOOL EmptyChart2(LPCTSTR p_szChartList, BOOL p_bIsDeleteAllPacket, BOOL p_bIsDefault);
	afx_msg short GetHorzScalePosition(short p_nRowIndex, short p_nColumnIndex);
	afx_msg BOOL HasIndicator(LPCTSTR p_szIndicatorName);
	afx_msg BOOL HasIndicatorInSpecialBack( LPCSTR p_szIndicatorName);
	afx_msg BOOL LockWindowUpdate();
	afx_msg void UnlockWindowUpdate();
	afx_msg BSTR AddChartItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, BOOL bBuiltChart);
	afx_msg void SetRealDataCode(LPCTSTR strCode, LPCTSTR strPacketName, double dData, short nOption);
	afx_msg void ShowIndicator(LPCTSTR p_szIndicatorName, BOOL p_bShow, BOOL p_bRecalculate);
	afx_msg void SetPacketAutoScaleType(LPCTSTR p_szPacketName, BOOL p_bAutoScaleType);
	afx_msg BOOL SetPacketTypeWithAutoScale(LPCTSTR p_szPacketName, LPCTSTR p_szPacketType, BOOL p_bAutoScale);
	afx_msg void ShowBaseText(BOOL p_bShow);
	afx_msg BOOL ChangeGraphColor(LPCTSTR p_szGraphTitle, LPCTSTR p_szColors);
	afx_msg void AddPacketRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketOption);
	afx_msg BOOL SetPacketDataRQ(LPCTSTR lpszRQ, LPCTSTR lpszPacketName, long nData, long nLength, BOOL bReset);
	afx_msg BOOL AddChartItemRQ(LPCTSTR lpszRQ, LPCTSTR lpszItemCode, LPCTSTR lpszItemName);
	afx_msg void SetCurrentRQ(LPCTSTR lpszRQ);
	afx_msg void NotUsedMethod8(BOOL bReset);
	afx_msg void SetFutureScaleVisible(long nScaleVisible);
	afx_msg void SetMultiItemMode(BOOL bMultiItem);
	afx_msg void SetVertScaleType(short nBlockRow, short nBlockCol, short nScaleGroup, short nScaleType, double dTypeValue);
	afx_msg void OpenFromPage(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex);
	afx_msg void SaveToPage(LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex);
	afx_msg BOOL ZoomTo(long p_nZoomValue, BOOL p_bRelative);
	afx_msg BOOL ShiftPacket(LPCTSTR p_szPacketNames, long p_szShiftCount);
	afx_msg BOOL SaveandOpenFromAnalToolFile(LPCTSTR lpszOpenFileName, LPCTSTR lpszSaveFileName, BOOL bOpenOption, BOOL bSaveOption);
	afx_msg void SetHorizontalScaleHeight(short nBlockRow, short nBlockCol, long nHeight);
	afx_msg void ChangeGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle, LPCTSTR lpszNewTitle, short nGraphType, short nGraphStyle, short nDrawStyle, LPCTSTR lpszColors, short nThickness1, short nThickness2, short nThickness3);
	afx_msg BOOL EmptyChart3(LPCTSTR lpszChartList, BOOL bIsDeleteAllPacket, BOOL bIsDefault, short nBlockCount);
	afx_msg BOOL AddChartAllBlock(LPCTSTR strChartName);
	afx_msg BOOL RemoveGraphRQ(LPCTSTR lpszRQ, LPCTSTR lpszTitle);
	afx_msg BSTR AddPairItem(LPCTSTR lpszItemCode, LPCTSTR lpszItemName, BOOL bBuiltChart);
	afx_msg BOOL HasIndicatorWithRQ(LPCTSTR p_szIndicatorName2, LPCTSTR p_szRQ);
	afx_msg void RemoveAllGraphOfRQ(LPCTSTR p_szRQ);
	afx_msg short GetRowOfGraphWithRQ(LPCTSTR p_szIndicatorName2, LPCTSTR p_szRQ);
	afx_msg BOOL SetGraphColorWithRQ(LPCTSTR p_szRQ, LPCTSTR p_szIndicatorName2, LPCTSTR p_szColor);
	afx_msg void SetUseSTExtData(LPCTSTR lpszExtDataName);
	afx_msg BOOL ShowGraphDataDrawingData(LPCTSTR lpszRQ, LPCTSTR lpszIndicatorName, BOOL bShow, short nShowType);
	afx_msg void ShowSelEnvironmentDlg(short nPage);
	afx_msg long GetVertScaleGroupCount(short nRow, short nColumn);
	afx_msg void SetSyncPacketData(LPCTSTR strSyncPacketNames);
	afx_msg void SetEmptyChart();
	afx_msg void SetDrawVerticalBox(double dHorzValue, LPCTSTR strColor, BOOL bFill);
	afx_msg void UseDummyPacketData(BOOL bUse);
	afx_msg void SetDoubleSizeBlock(short nRowIndex, short nColIndex);
	afx_msg long AddHorizontalLine(LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	afx_msg void DeleteHorizontalLine(long nIndex);
	afx_msg void DeleteAllHorizontalLine();
	afx_msg void UpdateHorizontalLine(long nIndex, LPCTSTR lpszTitle, OLE_COLOR clrLine, short nLine, double dPrice);
	afx_msg BSTR GetPacketTypeFromKrxCmeCode(LPCTSTR p_szKrxCmeCode);
	afx_msg void SetEncUserID(LPCTSTR strUserID);
	afx_msg void ReCalculateAllGraphs();
	afx_msg BSTR GetGraphPropertyAtOnce(LPCTSTR lpszRQ, LPCTSTR lpszTargetTitle);
	afx_msg void ChangeVertScaleMinMaxType(short nRowIndex, short nColumnIndex, short nVertScaleGroupIndex, short nScaleType, double dMinValue, double dMaxValue);
	afx_msg void SetAvgUnitPrice(double dAvgPrice);
	afx_msg long GetPacketCount(LPCTSTR strPacketName);
	afx_msg BOOL FindRQ(LPCTSTR lpszRQ);
	afx_msg short GetPacketNearIndex(LPCTSTR szPacketName, double dKeyValue);
	afx_msg BOOL ChangePacketTypeRQ(LPCTSTR lpszRQ, LPCTSTR p_szPacketName, LPCTSTR p_szPacketType);
	afx_msg void SetDrawBaseLine(BOOL bDraw);
	afx_msg BOOL GetGraphHide4(LPCTSTR strGraphName2, short nSubGraphIndex);//20100518_솔로몬  해당 지표의 SubGraphIndex의 Hide 속성을 Get
	afx_msg void SetItemCode(LPCTSTR p_szRQ, LPCTSTR p_szItemCode, LPCTSTR p_szItemName);
	afx_msg void MoveChart( short nRow, short nCol, short nGroupIdx, short nGraphIdx, short nTargetRow, short nTargetCol, short nTargetGroupIdx, BOOL bIsAddTail );

	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

// Event maps
	//{{AFX_EVENT(CKTBChartCtrl)
	void FireDataUpandDown(short nOption, LPCTSTR strStartDate, LPCTSTR strEndDate)
		{FireEvent(eventidDataUpandDown,EVENT_PARAM(VTS_I2  VTS_BSTR  VTS_BSTR), nOption, strStartDate, strEndDate);}
	void FireRequestBongData(short nOption, short nTemp)
		{FireEvent(eventidRequestBongData,EVENT_PARAM(VTS_I2  VTS_I2), nOption, nTemp);}
	void FireChangeOnePageDataCount(long nOnePageDataCount)
		{FireEvent(eventidChangeOnePageDataCount,EVENT_PARAM(VTS_I4), nOnePageDataCount);}
	void FireRestore()
		{FireEvent(eventidRestore,EVENT_PARAM(VTS_NONE));}
	void FireRequestMapLoad(short nOption, LPCTSTR strCode, LPCTSTR strDate)
		{FireEvent(eventidRequestMapLoad,EVENT_PARAM(VTS_I2  VTS_BSTR  VTS_BSTR), nOption, strCode, strDate);}
	void FireRequestNewPacket(short nOption, LPCTSTR strName, LPCTSTR strCode, LPCTSTR strDate)
		{FireEvent(eventidRequestNewPacket,EVENT_PARAM(VTS_I2  VTS_BSTR  VTS_BSTR  VTS_BSTR), nOption, strName, strCode, strDate);}
	void FireSaveSettedChartInfoToServer()
		{FireEvent(eventidSaveSettedChartInfoToServer,EVENT_PARAM(VTS_NONE));}
	void FireGetSettedChartInfoFromServer()
		{FireEvent(eventidGetSettedChartInfoFromServer,EVENT_PARAM(VTS_NONE));}
	void FireRequestPacketData(LPCTSTR strDateType, LPCTSTR strStartDate, LPCTSTR strPacketNames)
		{FireEvent(eventidRequestPacketData,EVENT_PARAM(VTS_BSTR  VTS_BSTR  VTS_BSTR), strDateType, strStartDate, strPacketNames);}
	void FireAddInEvent(LPCTSTR p_szAddInName, short p_nEventID, LPCTSTR p_szData, double p_dData, long p_lData)
		{FireEvent(eventidAddInEvent,EVENT_PARAM(VTS_BSTR  VTS_I2  VTS_BSTR  VTS_R8  VTS_I4), p_szAddInName, p_nEventID, p_szData, p_dData, p_lData);}
	void FireChangeDateIndex(short nDateIndex)
		{FireEvent(eventidChangeDateIndex,EVENT_PARAM(VTS_I2), nDateIndex);}
	void FireRequestCodeAndName()
		{FireEvent(eventidRequestCodeAndName,EVENT_PARAM(VTS_NONE));}
	void FireSendDataIndexInLButtonUp(short nRowIndex, short nColumnIndex, short nDataIndex, double dValue)
		{FireEvent(eventidSendDataIndexInLButtonUp,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2  VTS_R8), nRowIndex, nColumnIndex, nDataIndex, dValue);}
	void FireReturnOrgChart(long nOnePageDataCount)
		{FireEvent(eventidReturnOrgChart,EVENT_PARAM(VTS_I4), nOnePageDataCount);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	//nami 9.25
	void SetInquiryValue( const BOOL nIs);
	//nami end
	void DeleteZoomInOutInquiryDlg();
	int SetChartData2(long pChartData, int nCount, int nType);
	int GetOnePacketSize();
	
	//yousun 10.15
	CRect GetMainBlockRegion();

	enum {
	//{{AFX_DISP_ID(CKTBChartCtrl)
	dispidNChartMode = 1L,
	dispidBNormalizedMinuteTimeRule = 2L,
	dispidNBlockType = 3L,
	dispidNBlockCompart = 4L,
	dispidNBlockRowCount = 5L,
	dispidNBlockColumnCount = 6L,
	dispidNBlockGap = 7L,
	dispidNBlockBorderThickness = 8L,
	dispidStrAddInLoadList = 9L,
	dispidNBorderType = 10L,
	dispidNScaleLineType = 11L,
	dispidBWholeView = 12L,
	dispidNOnePageDataCount = 13L,
	dispidNUserBlockAddCount = 14L,
	dispidBGraphDragDrop = 15L,
	dispidNLeftMargin = 16L,
	dispidNRightMargin = 17L,
	dispidNTopMargin = 18L,
	dispidNBottomMargin = 19L,
	dispidBAddGraphMenuShow = 20L,
	dispidStrUserEnvironmentFileName = 21L,
	dispidStrUserIndicatorListFileName = 22L,
	dispidStrUserSeriesListFileName = 23L,
	dispidBHide = 24L,
	dispidBIsInput = 25L,
	dispidStrToolbarOption = 26L,
	dispidStrPacketOption = 27L,
	dispidStrLocalSharedList = 28L,
	dispidBUseRealData = 29L,
	dispidStrRealList = 30L,
	dispidNMarginBorderType = 31L,
	dispidNRowDoubleSizeBlock = 32L,
	dispidBUseInquiryDlgWhenUseCrossLine = 33L,
	dispidNColumnDoubleSizeBlock = 34L,
	dispidBUseShortKey = 35L,
	dispidBUseConditiontoTitle = 36L,
	dispidBUseDatatoTitle = 37L,
	dispidNHorzGap = 38L,
	dispidNVertGap = 39L,
	dispidNRButtonMenuType = 40L,
	dispidBShowWarningMessageBox = 41L,
	dispidBControlBlockSize = 42L,
	dispidBBlockMoveDelete = 43L,
	dispidBSelectTitle = 44L,
	dispidBSelectGraph = 45L,
	dispidBUseScaleRButtonMenu = 46L,
	dispidNGraphRButtonType = 47L,
	dispidNHoldingPacketSize = 48L,
	dispidSzBasePacketName = 49L,
	dispidBRedrawAfterUpdatedAllPackets = 50L,
	dispidNMinDataCountOnOnePage = 51L,
	dispidBFireRealEvent = 52L,
	dispidNInquiryType = 53L,
	dispidBOcxAreaNumericalInquiry = 54L,
	dispidNNumericalInquiryOption = 55L,
	dispidStrGlobalSharedList = 56L,
	dispidNUseInquiryWithCrossline = 57L,
	dispidBShowPriceRateInquiry = 58L,
	dispidBDoesUseSkinColor = 59L,
	dispidStrMapName = 60L,
	dispidNBongCountMargin = 61L,
	dispidBOnePageDataCountInTRCount = 62L,
	dispidBIsMapDefault = 63L,
	dispidNPacketCountDataSize = 64L,
	dispidNLanguageID = 65L,
	dispidBFreezedPageState = 66L,
	dispidIsShowInneerToolBar = 70L,
	dispidStrChartList = 71L,
	dispidClrBackground = 72L,
	dispidClrBackBorder = 73L,
	dispidClrBlock = 74L,
	dispidClrBlockBorder = 75L,
	dispidClrScaleLine = 76L,
	dispidClrScaleText = 77L,
	dispidClrGraphBase1 = 78L,
	dispidClrGraphBase2 = 79L,
	dispidClrGraphBase3 = 80L,
	dispidClrGraphBase4 = 81L,
	dispidClrGraphBase5 = 82L,
	dispidClrGraphBase6 = 83L,
	dispidClrAnalysisTool = 84L,
	dispidStrBlockRegion = 85L,
	dispidClrScaleLine2 = 86L,
	dispidClrInquiryWindow = 87L,
	dispidCustomFont = 88L,
	dispidClrLattice = 89L,
	dispidStrStartDateOfData = 90L,
	dispidOnRunningOneChart = 91L,
	dispidEnableHorizontalLineInNIDLG = 92L,
	dispidBShowPriceMinMaxText = 93L,
	dispidBHideAllTitle = 94L,
	dispidStrMapIndicatorInfo = 95L,
	dispidBEnableIndicatorCalculation = 96L,
	dispidStrPagePathWithPrefix = 67L,
	dispidNCurrentDrawCount = 97L,
	dispidNRealTimeTruncationType = 68L,
	dispidBUseGapRevision = 69L,
	dispidNFullDataCount = 98L,
	dispidSetAllProperties2 = 99L,
	dispidSetBaseDesk = 100L,
	dispidGetBase = 101L,
	dispidSetChartData = 102L,
	dispidSetChartDataStr = 103L,
	dispidTestGetPacketdatasize = 104L,
	dispidTestLoadAnalysisTools = 105L,
	dispidMakeSaveShortKey = 106L,
	dispidChangeChart = 107L,
	dispidClearChartData = 108L,
	dispidSetGraphHide = 109L,
	dispidAddChartBlock = 110L,
	dispidSetOnePageDataCount = 111L,
	dispidGetOnePageDataCount = 112L,
	dispidSaveandOpenFromFile = 113L,
	dispidJoinandDevisionforCompareChart = 114L,
	dispidChangeGraphTitle = 115L,
	dispidSetChartDataFromFile = 116L,
	dispidRemoveChart = 117L,
	dispidChangeGraphType = 118L,
	dispidSetChartAllInfo = 119L,
	dispidGetData = 120L,
	dispidSetDrdsAdvise = 121L,
	dispidGetDataCount = 122L,
	dispidChangeChartItem = 123L,
	dispidSetStopLossPrice = 124L,
	dispidRestoreMinMax = 125L,
	dispidRestoreMin = 126L,
	dispidRestorMax = 127L,
	dispidSetMin = 128L,
	dispidSetMax = 129L,
	dispidSetMinMax = 130L,
	dispidSetHorzScaleData = 131L,
	dispidGetMin = 132L,
	dispidGetMax = 133L,
	dispidSetGraphHide2 = 134L,
	dispidChangeGraphDatas = 135L,
	dispidSaveandApplyGraphStyle = 136L,
	dispidSetDataRegionInBlock = 137L,
	dispidSetItemInfo = 138L,
	dispidAddandDeleteIndicator = 139L,
	dispidGetAllProperties2 = 140L,
	dispidGetBasePropIf = 141L,
	dispidSetVertScaleData = 142L,
	dispidChangeBlockType = 143L,
	dispidChangeHorzScalePosition = 144L,
	dispidResetChart = 145L,
	dispidSetRealData = 146L,
	dispidSetGraphTitle = 147L,
	dispidGetIndicatorNameList = 148L,
	dispidAddGraph = 149L,
	dispidAddPacket = 150L,
	dispidRemovePacket = 151L,
	dispidSetContainerDllInfo = 152L,
	dispidSetPacketData = 153L,
	dispidSetBackgroundColors = 154L,
	dispidAddRealItem = 155L,
	dispidShowIndicatorConfigDlg = 156L,
	dispidResetChartCfgWith = 157L,
	dispidEmptyChart = 158L,
	dispidHasGraph = 159L,
	dispidRemoveRealItem = 160L,
	dispidShowInnerToolBar = 161L,
	dispidChangeVertScalePosition = 162L,
	dispidGetRowBlockCount = 163L,
	dispidGetColumnBlockCount = 164L,
	dispidSetStartPeriodOfPacket = 165L,
	dispidGetStartPeriodOfPacket = 166L,
	dispidGetStartEndDateOnView = 167L,
	dispidSetStartEndDateOfView = 168L,
	dispidScrollToDataOfEnd = 169L,
	dispidZoomToWithRightFix = 170L,
	dispidClearTimeMark = 171L,
	dispidShowEnvironmentDlg = 172L,
	dispidSetTimeMark = 173L,
	dispidGetVertScalePosition = 174L,
	dispidEnableBoundMark = 175L,
	dispidEnableReceivingTimeShare = 176L,
	dispidChangePacketType = 177L,
	dispidChangeGraphDrawingData = 178L,
	dispidChangePriceGraphDrawingData = 179L,
	dispidShowAnalysisToolText = 180L,
	dispidHasGraphInSpecialBack = 181L,
	dispidLoadAddIn = 182L,
	dispidInvokeAddIn = 183L,
	dispidSetDragModeType = 184L,
	dispidResetRealListString = 185L,
	dispidAddOrRemoveChart = 186L,
	dispidSetHtsPath = 187L,
	dispidGetRealSettingInfo = 188L,
	dispidSetAllLogScale = 189L,
	dispidGetListViewerData = 190L,
	dispidSetDataIndexByDate = 191L,
	dispidSetRunTimeMode = 192L,
	dispidGetPacketOption = 193L,
	dispidSetPacketOption = 194L,
	dispidSetOutsideCommonInterface = 195L,
	dispidRemoveGraph_Title = 196L,
	dispidGetGraphPacketName = 197L,
	dispidChangeGraphTitle2 = 198L,
	dispidPartitionAllGraph = 199L,
	dispidJoinAllGraph = 200L,
	dispidChangeAllFunctionName = 201L,
	dispidChangeIndicatorPacketName = 202L,
	dispidRunEnvironmentDlg = 203L,
	dispidSetIndicatorCondition = 204L,
	dispidSetPacketData2 = 205L,
	dispidSetLogScale = 206L,
	dispidGetLogScale = 207L,
	dispidSetPacketHelpMessage = 208L,
	dispidInvokeAddInStr = 209L,
	dispidSetHelpMessageRealDataRate = 210L,
	dispidSetBongCountMargin = 211L,
	dispidSetAllPropertiesFromFile = 212L,
	dispidSetGraphHide3 = 213L,
	dispidGetBoundMarkFlag = 214L,
	dispidUnloadAddIn = 215L,
	dispidSetOnePageDataCount2 = 216L,
	dispidClearPacketData = 217L,
	dispidSetAllGraphHide = 218L,
	dispidInitStaticIndiCond = 219L,
	dispidSetAllGraphsHide = 220L,
	dispidGetPacketData2 = 221L,
	dispidGetSelectedSubGraphData = 222L,
	dispidGetPacketIndex = 223L,
	dispidGetCurrentPriceChartType = 224L,
	dispidRemoveBlock = 225L,
	dispidRemoveGraph = 226L,
	dispidSetGraphColor = 227L,
	dispidSetPacketDataInIndex = 228L,
	dispidSetVertScaleLogType = 229L,
	dispidOpenFileOfEnvAndIndi = 230L,
	dispidSetPacketDataInAdditionPacket = 231L,
	dispidSetOnePageDataCountInTRCount = 232L,
	dispidPrintChatImage = 233L,
	dispidExportToExcel = 234L,
	dispidRunToolFunction = 235L,
	dispidSetSubIndicatorCondition = 236L,
	dispidSetGraphHide4 = 237L,
	dispidSetMapDefaultInCurrent = 238L,
	dispidChangeFunctionName = 239L,
	dispidGetHeadAndTailData = 240L,
	dispidGetAddInCustomInterface = 241L,
	dispidLoadAddInWithOverExclusive = 242L,
	dispidOpenFromFile = 243L,
	dispidSaveandOpenFromFileWithMSave = 244L,
	dispidAddRealItemWithSeparator = 245L,
	dispidRemoveRealItemWithSeparator = 246L,
	dispidGetAllProperties3 = 247L,
	dispidSetAllProperties3 = 248L,
	dispidShowIndicatorConfigDlgWithName = 249L,
	dispidRemoveBlockWithIndex = 250L,
	dispidSetBinTrHelpMsg = 251L,
	dispidSetBinTrDoubleData = 252L,
	dispidSetBinTrStringData = 253L,
	dispidSetBinTrEnd = 254L,
	dispidEmptyChart2 = 255L,
	dispidGetHorzScalePosition = 256L,
	dispidHasIndicator = 257L,
	dispidHasIndicatorInSpecialBack = 258L,
	dispidLockWindowUpdate = 259L,
	dispidUnlockWindowUpdate = 260L,
	dispidAddChartItem = 261L,
	dispidSetRealDataCode = 262L,
	dispidShowIndicator = 263L,
	dispidSetPacketAutoScaleType = 264L,
	dispidSetPacketTypeWithAutoScale = 265L,
	dispidShowBaseText = 266L,
	dispidChangeGraphColor = 267L,
	dispidAddPacketRQ = 268L,
	dispidSetPacketDataRQ = 269L,
	dispidAddChartItemRQ = 270L,
	dispidSetCurrentRQ = 271L,
	dispidSetResetTotalCountWhileDraw = 272L,
	dispidSetFutureScaleVisible = 273L,
	dispidSetMultiItemMode = 274L,
	dispidSetVertScaleType = 275L,
	dispidOpenFromPage = 276L,
	dispidSaveToPage = 277L,
	dispidZoomTo = 278L,
	dispidShiftPacket = 279L,
	dispidSaveandOpenFromAnalToolFile = 280L,
	dispidSetHorizontalScaleHeight = 281L,
	dispidChangeGraphPropertyAtOnce = 282L,
	dispidEmptyChart3 = 283L,
	dispidAddChartAllBlock = 284L,
	dispidRemoveGraphRQ = 285L,
	dispidAddPairItem = 286L,
	dispidHasIndicatorWithRQ = 287L,
	dispidRemoveAllGraphOfRQ = 288L,
	dispidGetRowOfGraphWithRQ = 289L,
	dispidSetGraphColorWithRQ = 290L,
	dispidSetUseSTExtData = 291L,
	dispidShowGraphDataDrawingData = 292L,
	dispidShowSelEnvironmentDlg = 293L,
	dispidGetVertScaleGroupCount = 294L,
	dispidSetSyncPacketData = 295L,
	dispidSetEmptyChart = 296L,
	dispidSetDrawVerticalBox = 297L,
	dispidUseDummyPacketData = 298L,
	dispidSetDoubleSizeBlock = 299L,
	dispidAddHorizontalLine = 300L,
	dispidDeleteHorizontalLine = 301L,
	dispidDeleteAllHorizontalLine = 302L,
	dispidUpdateHorizontalLine = 303L,
	dispidGetPacketTypeFromKrxCmeCode = 304L,
	dispidSetEncUserID = 305L,
	dispidReCalculateAllGraphs = 306L,
	dispidGetGraphPropertyAtOnce = 307L,
	dispidChangeVertScaleMinMaxType = 308L,
	dispidSetAvgUnitPrice = 309L,
	dispidGetPacketCount = 310L,
	dispidFindRQ = 311L,
	dispidGetPacketNearIndex = 312L,
	dispidChangePacketTypeRQ = 313L,
	dispidSetDrawBaseLine = 314L,
	dispidGetGraphHide4 = 315L,
	dispidSetItemCode = 316L,
	dispidMoveChart = 317L,
	eventidDataUpandDown = 1L,
	eventidRequestBongData = 2L,
	eventidChangeOnePageDataCount = 3L,
	eventidRestore = 4L,
	eventidRequestMapLoad = 5L,
	eventidRequestNewPacket = 6L,
	eventidSaveSettedChartInfoToServer = 7L,
	eventidGetSettedChartInfoFromServer = 8L,
	eventidRequestPacketData = 9L,
	eventidAddInEvent = 10L,
	eventidChangeDateIndex = 11L,
	eventidRequestCodeAndName = 12L,
	eventidSendDataIndexInLButtonUp = 13L,
	eventidReturnOrgChart = 14L,
	//}}AFX_DISP_ID
	};

private:
	const CString DATA_FILE_FOLDER;
	const CString USER_FILE_FOLDER;

public:

private:
	void GetIndicatorDefaultData(CIndicatorDefaultData& indiDefaultData) const;

	BOOL m_bShowMinMaxbyPrice;
	CString m_strUserID;
	BOOL IsThisPacketConnected(const int nIndex, const CString strpacketname);
	// 05.02.2002  이동평균선 이름 변경
	void ReplaceMATitle(CString &szpricema, int nOption = 0);
	// 04.17.2002  초기화및 이벤트
	void OnReturnOrg(const bool bIsMessageBox);
	bool OnReturnOrg2(const BOOL bIsMessageBox = TRUE);
	void OnFullMinMax(int nOption);

	bool ResetSaveChartCfg(int nOption, bool bRegionRetain = false);
	bool ResetSaveChartCfg(CChartCfg* pChart, const bool bRegionRetain);
	CChartCfg* GetChartCfg(const int nOption);
	bool SetOcxDataFromChartCfg(CChartCfg* pChart, bool bRegionRetain);
	bool SaveChartCfg( BOOL p_bDontUseMainBlock = FALSE);	// (2008/10/13 - Seung-Won, Bae) Do not use MainBlock. It is not Initialized. Use Cfg member variables.
	bool ChangeChartType(CString strGraphName, CString strType, CString strStyle, CString strPacketNameList);
	BOOL	ChangeOneChart( const CString& strChartName, CString &p_strGraphName);
	BOOL	GetBlockDataForMaking( const CString& strChartName, CString &p_strBlockData, CString &p_strGraphName) const;
	bool RollbackLastNoOneChart();
	// file에 환경 저장하기
	//BOOL GetWritableOrReadFile(const CString& strFileName, CFile * file, CSaveUserFile::FILEOPENSTYLE nFileOpenType);
	void WriteUserFile(const CString& strFileName);
	void WriteUserFile(CSaveUserFile::FILETYPE nType);
	// 분석툴 종목별 저장 - ojtaso (20080728)
	void WriteUserFileOnlyAnalTool(const CString &strFileName);
	// file에서 저장내용 읽기
	BOOL ReadUserFile(const CString& strFileName);
	BOOL ReadUserFile(CSaveUserFile::FILETYPE nType);
	// 분석툴 종목별 저장 - ojtaso (20080728)
	BOOL ReadUserFileOnlyAnalTool(const CString& strFileName);
	// 디렉토리 생성
	bool CreateUserDirectory();
	// MainBlock에서 내용 가져오기
	CString GetDataFromMainBlock();
	CChartCfg* GetLastChartCfg_NoOneChart();
	void SetDataToMainBlock(const CString& strData);
	// MainBlock setting
	void SetMainBlock(BOOL bUserMode, BOOL p_bWithIndicatorInfoUpdate = TRUE, BOOL bWithScroll = TRUE) ;
	void SetMainBlock_MainBlock(const bool bUserMode, BOOL p_bWithIndicatorInfoUpdate = TRUE);
	void SetMainBlock_ToolBar(const bool bUserMode);
	BOOL ResetPropertiesfromMainBlock();

	BOOL PacketDataMaker();
	CString GetUserDefaultFilePath(const CString& strBaseDeskPath = "");
	CString GetUserFilePathAndName(const CString& strFileName);
	CString GetUserDefaultFilePathAndName(const CString& strFileName);
	CString GetDefaultFilePathAndName(const CString& strFileName);

	int GetMarginInIndicatorInfo( const CString &p_strIndicatorName) const;

	BOOL	SetUserEnvironmentAndIndicatorListFileName(const CString& strFileName);
	void UserFileVersionChecker();

	int GetMaxDataCount() const;

	CString	m_strCode;	// 종목코드

	// MainBlock (ChartObject dll)
	// (2007/6/25 - Seung-Won, Bae) Support Dynamic Loading of ChartObject.dll
	HMODULE			m_hMainBlock;
	CMainBlock * m_pMainBlock;
	// X-Scale Manager
	CXScaleManager* m_pxScaleManager;
	// PacketData dll
	CPacketList * m_pPacketList;
	// Indicator dll
	CIndicatorList * m_pIndicatorList;
	// Draw Back Ground (ViewDraw dll)
	CBackgroundDrawer m_pBackgroundDraw;

	// ocx region
	CRect m_rcMain;
	BOOL m_bFirstDraw;

	///////////////////////////////////////////////////////
	// version 1.0.0.2
	// User Margin
	//int m_nUserMargin;

	//	Color Properties
	COLORREF	m_clrBackground;
	COLORREF	m_clrBackBorder;
	COLORREF	m_clrBlock;
	COLORREF	m_clrBlockBorder;
	COLORREF	m_clrScaleLine;
	COLORREF	m_clrScaleLine2;
	COLORREF	m_clrScaleText;
	COLORREF	m_clrGraphBase1;
	COLORREF	m_clrGraphBase2;
	COLORREF	m_clrGraphBase3;
	COLORREF	m_clrGraphBase4;
	COLORREF	m_clrGraphBase5;
	COLORREF	m_clrGraphBase6;
	COLORREF	m_clrAnalysisTool;
	COLORREF	m_clrInquiryWindow;
	COLORREF	m_clrLattice;

	CString		m_strBlockRegion;

	// backup용 Properties
	BOOL	m_bOrgWholeView;
	int		m_nOrgBlockRowCount;
	int		m_nOrgBlockColumnCount;
	CString m_strOrgBlockRegion;
	int		m_nOrgRowDoubleSizeBlock;
	int		m_nOrgOnePageDataCount;
	COLORREF	m_clrOrgBackground;
	COLORREF	m_clrOrgBackBorder;
	COLORREF	m_clrOrgBlock;
	COLORREF	m_clrOrgBlockBorder;
	COLORREF	m_clrOrgScaleLine;
	COLORREF	m_clrOrgScaleText;
	COLORREF	m_clrOrgAnalysisTool;
	COLORREF	m_clrOrgScaleLine2;
	COLORREF    m_clrOrgLattice;
	//nami 11.22
	CString m_strOrgBlocksPosition;

	// event wait
	BOOL m_bWaitRequest;
	// save chart list & cfg
	CSaveChartCfgList m_SaveChartcfg;

	//7.9
	BOOL m_bOnAdvise; //advise 되어 있는지. KeyName이 다를때마다
	BOOL m_bConnect; //connect이 되어 있는지. packetName이 다를때마다

	//nami 9.10
	BOOL m_bToolTipwithZoomIn;

	//yousun 10.15
	BOOL m_bRadar;

	//sy 2002.11.23.
	BOOL	m_bMainOSBFilled;	// (2009/1/13 - Seung-Won, Bae) Main Image Cache.
	CBitmap m_bmpMainOSB;
	void CreateBufferBitmap(CDC* pdc, const CRect& rcBounds);

	CString m_strItemName;
	CString m_strItemCode;

	BOOL m_bShowTrendLineText;
	/// cj 1월 22일
	BOOL m_bShowVerticalText;

	// 종찬(04/10/14) 가격Text를 왼쪽, 오른쪽 구분해서 보이기.
	BOOL m_bShowHorizonTextLeft;
	BOOL m_bShowHorizonTextRight;
	
	BOOL m_bShowThreeText;
	BOOL m_bShowFourText;
	BOOL m_bShowFiboText;

	long m_lFlagEvent;


// 패킷 데이타가 없는 경우 처리 - ojtaso (20080618)
public:
	BOOL m_bSyncPacket;
	CString m_strSyncPacketNames;

// (2004.02.11, 배승원) 수치조회창족에서 OCX의 영역을 실시간으로 조회할 수 있도록 Public으로 선언한다.
public:
	CRect GetVirtualRegion();

// (2004.02.11, 배승원) 수치조회창이 1개 Block 내에서만 이동하지 않고,
//		Script에서 조정한 Property에 따라 OCX 전반에 걸쳐 이동될 수 있도록 개선한다.
//		(Property를 조회하는 Interface를 제공한다.)
public:
	BOOL	IsOcxAreaNumericalInquiry( void) const	{	return m_bOcxAreaNumericalInquiry;	}

// (2003.11.12, 배승원) Map에서 설정한 '한 화면 최소 Data 수'를 SlideBar에 적용한다.
public:
	int		GetSliderMin( void)		{	return m_nMinDataCountOnOnePage;	};

// (2003.12.23, 배승원) Resource Manager 적용을 개선한다.
protected:
	LOGFONT				m_logFont;				// Resource Manager가 없는 경우 주로 이용하게되는 Font Meta Data이다.
	CString				m_strCSFont;			// Component에 설정된 Font의 Meta String이다.
	CFontKey			m_FontKey;				// Resource Manager에게 Adivse하기 위한 Font Access Key Data이다.
	CResourceManager *	m_pResourceManager;
	CFont *				m_pRscFont;
protected:
	class CAllPropGetterSetter : public IBaseAllGetterSetter
	{
	public:
		virtual void getAllCtrlProperty(CStringList* plist);
		virtual BOOL setAllCtrlProperty(CStringList* plist);
		virtual void setBaseDesk(long lKey);
		virtual void setRscMng(long lKey);
	} m_xAllGetterSetter;
	friend class CAllPropGetterSetter;
protected:
	void	FV_vInitFont();						// m_strCSFont의 Font Meta를 이용하여 LOG Font Data를 생성한다.
	void	FP_vSetInitFont();						// 현재 설정된 Font Meta를 이용하여 적절한 Font Object를 생성한다.
												//	(Resource Manager가 있는 경우 이용한다.)
	void	SetRscMng( long lKey);

// (2004.01.09, 배승원) 특수한 경우의 Map에서는 Packet Count가 5자리로 확장될 수 있도록 조정한다.
//		이 값은 Property nPacketCountDataSize의 값을 초기 Script에서 설정하는 것으로 이용한다.
//		따라서 Property Page와 연동하거나 AnyBuilder의 Property 관리, XFM File등과 연계되지 않는다.
public:
	short	GetPacketCountDataSize( void) const		{	return m_nPacketCountDataSize;	}

// (2004.02.16, 배승원) 일목균형표에 의한 우측여백을 별도로 관리한다.
//		별도로 관리하지 않을 경우, 일목균형표 추가/삭제시 확장 및 축소할 우측여백의 일관성이 없고,
//		또한 이용자가 설정한 우측여백에 의해서 그 값이 손상될 우려가 있기 때문이다.
protected:
	int		m_nGlanceBalanceMargin;

// (2004.02.17, 배승원) Radar Chart에서 최소 ZoomIn 제한을 확인할 수 있도록
//	한화면 최소 Data수를 조회하는 Interface를 제공한다.
public:
	int		GetMinDataCountOnOnePage( void) const	{	return m_nMinDataCountOnOnePage;	}

// (2004.03.08, 배승원) HTS Main Window를 AfxGetMainWnd()가 아니라, Desk를 통하여 정확하게
//		구하도록 지원한다. (Dialog Based Main의 경우 AfxGetMainWnd()가 정상동작하지 않음)
protected:
	CWnd *	GetMainWnd( void);

// (2004.05.06, 배승원) DLL Container의 Window Handle과 Chart의 내부 ID를 관리하여 Event Notify를 전달하도록 한다.
protected:
	HWND	m_hContainerDllWnd;
	short	m_nID;
public:
	LRESULT	SendEventMsgToDLL( UINT message, WPARAM wParam = NULL, LPARAM lParam = NULL);

// (2004.05.20, 배승원) DLL Container에서 지정한 Packet 초기화 상태까지만 RollBack 시키는 기능을 추가한다.
//		0로 초기화를 시도하는 경우, DLL Container에서 지정한 Packet 초기화 상태까지만 RollBack시킨다.
protected:
	CChartCfg	*GetChartCfgWithPacketReset( int nOption);

//특수차트로 저장이 되어 있었다면
//화면을 다시 띄울때 외부툴바에 선택이 되어 있어야 한다.
protected:
	CToolOptionInfo::TOOLOPTION GetSavedChartType();

// (2004.05.25, 배승원) 내부 ToolBar를 보이고, 숨기는 Interface를 제공한다.
//		별도의 Flag을 두어, ToolBar를 OCX의 영역보다 아래쪽에 위치되도록 한다.
protected:
	BOOL	m_bShowInnerToolBar;

// (2004.06.04, 배승원) Scroll이 변경될 때마다 DLL Container에게 끝일시를 전달한다.
//		View의 End Data Index를 전달하여, 일시를 구해 전달하도록 한다.
// (2005/2/17 - Seung-Won, Bae) Add Message Parameter for UWM_DRAGSTART/UWM_DRAGEND
protected:
	void	SendTimeOfIndexToDLL( UINT p_uMessage, int p_nIndex);
	LRESULT	RmsgFormStateChange( WPARAM wParam, LPARAM lParam);	//sy 2005.04.26.

// (2004.09.06, 배승원) OCX Counter를 두어, 마지막 OCX의 종료시에 User Default 정보를 저장한다.
protected:
	static int	m_nChartOcxCnt;

// (2004.09.15, 배승원) Public으로 변경한다.
public:
	// 단독차트 실행중인지 리턴
	virtual BOOL	IsRunningOneChart( void);
	// (2009/11/6 - Seung-Won, Bae) Do not disable zoom and scroll on OneChart
	BOOL	IsRunningNoZoomChart( void);

// (2004.10.06, 배승원) AddIn Manager를 관리한다.
protected:
	CAddInManager *		m_pAddInManager;

// (2004.10.13, 배승원) 수치조회창에 대비율이 보일지를 설정하는 Property를 추가한다.
public:
	BOOL	IsShowPriceRateInquiry( void) const		{	return m_bShowPriceRateInquiry;	}

// (2004.10.18, 배승원) User File Path를 구하는 Interface를 제공한다.
public:
	CString GetUserFilePath(const CString& strBaseDeskPath = "");

// (2004.10.25, 배승원) Chart 전용 Font를 제공하는 Interface를 제공한다.
public:
	CFont *GetChartFont( void)	{	return m_pRscFont;	}

// [04/10/27] Data 좌표와 Screen 좌표를 변환하는 Interface를 제공한다.
public:
	int		ConvertDataToYPosition( double p_dData, double p_dDataMax, double p_dDataMin,
						int p_nYPositionTop, int p_nYPositionBottom, BOOL p_bLog, BOOL p_bReverse);
// (2004.11.02, 배승원) 현 Mouse 위치의 Chart Block을 뒤집기 처리하는 Popup Menu를 제공한다.
//		OnUpdateReverseYscale()에서 확인한 Block 정보를 관리하여, OnReverseYscale()에 이용토록 한다.
protected:
	bool			m_bReverseYScale_ReverseChart;
	CBlockIndex		m_biReverseYScale_BlockIndex;

//////////////////////////////////////////////////////////////////////
// [KTBChartScroll.cpp] (2004.11.09, 배승원, 분석) Scroll & Zoom 관련 Interface이다.
//////////////////////////////////////////////////////////////////////
public:	// (2004.11.09, 배승원, 분석) Print를 위한 Scrolling을 처리하는 Routine이다.
			//		그러나 ToolBar가 있는 경우로 제한된 Old Version이어서 
			//		ToolBar가 없는 화면의 경우 출력에 오류가 있을 수 있다.
	int		GetCurrentScrollPos();
	BOOL	SetScrollPosToNextPage( BOOL p_bBackward = FALSE);
	void	ResetScroll( BOOL p_bWithCalcDRI = FALSE);
	int		GetOnePageDataCountForScroll() const;
public:	// (2004.11.09, 배승원, 분석) Index 기준 Scroll Info 조회 Interface이다.
	// (2004.07.02, 배승원) MaxRange도 조회하는 Interface를 제공한다. (마지막 Index의 값이된다.)
	BOOL	GetScrollRange( int &p_nRange);
	// (2004.06.15, 배승원) Scrolling을 처리하는 Interface를 제공한다.
	// 1. Data Index로 Scrolling을 지정하는 Routine을 제공한다. (1 Based Index로 처리한다.)
	BOOL	GetDataStartEnd( int &p_nStartIndex, int &p_nEndIndex);
public:	// (2004.11.09, 배승원, 분석) Scroll 및 Zoom을 간단히 처리하는 Interface를 제공한다.
	BOOL	ScrollTo( int p_nScrollValue, BOOL p_bRelative);
	// BOOL	ZoomTo( int p_nZoomValue, BOOL p_bRelative); - Method
// (2006/2/6 - Seung-Won, Bae) DragZoomIn for DragZoom AddIn
protected:

// 종찬(05/06/01) AddIn에서 필요로하는 그래프 추가,삭제, 변경에 관련된 Interface이다.
public:
	void	AddPacketAdapter(const char* pcszPacketOption);	// AddPacket()의 Public Adapter.
	void	AddGraphAdapter(short nRow, short nColumn, short nVertScaleGroup, const char* szGraphOption, BOOL bCreateBlock, BOOL bCreateVertScaleGroup, BOOL bUseAutoScale);	// 그래프 추가 Interface.
	bool	ChangeSubGraphData(const char* pcszGraphName, const char* pcszSubGraphName, COLORREF clrLineColor, int nThickness, int nDrawStyle);	// 그래프의 색깔, 굵기, 타입등을 변경하는 Interface.
	bool	SetStartPeriodOfPacketAdapter(const char* szPacketName, long lStartPeriod, BOOL bAddSpace);	// 패킷의 시작인덱스를 세팅하는 Interface.
	bool	SetStartIndexOfPacket(const char* pcszRQ, const char* pcszPacketName, int nStartDataIndex, BOOL bAddSpace);	// 패킷의 시작인덱스를 세팅하는 Interface.
	bool	HasGraphAdapter(const char* szGraphName);	// 해당 그래프가 있는지를 확인하는 Interface.

// 종찬(04/11/16) 스크롤바의 사용유무를 리턴한다.
//		툴바가 있어도 스크롤바가 없는 경우엔 SetScrollData_NoneToolBar()함수가 호출되어야
//		하나 툴바가 없을 경우에만(!) SetScro.._None..()가 호출되는 오류를 수정하기 위한 함수.
private:
	bool IsUseToolScrollBar();

// 종찬(04/11/16) MaxRange를 리턴한다.
public:
	int GetMaxRange();
private:
	int m_nMaxRange;
	
// 종찬(04/11/24) RButton메뉴의 지표타이틀 감추기 옵션 변수.
private:
	bool m_bAllGraphTitleHiding;

private:
	bool	AddOrDeleteChart(const CString& strChartName);
	bool	AddOrDeleteChart_Special(const CString& strChartName);
	// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
	bool	AddOrDeleteChart_Genneral(const CString& strChartName, const BOOL bSelect = TRUE);
	BOOL	AddChart_GenneralChart(const CString& strChartName, CString &p_strGraphName, BOOL p_bWithPair = TRUE);
	bool	DeleteChart_Genneral(const CString& strChartName);
	// 모든 RQ의 그래프의 차트 변경 - ojtaso (20080516)
	BOOL	ChangeChart_PriceChartStyle( const CString &p_strChartName, CString &p_strGraphName, const BOOL bSelect = TRUE);

// (2004.11.05, 배승원) 수치조회창 관련 Logic 정리
//	[Logic]
//		1. 수치조회창 관련 설정
//			- XFM, ToolBar의 수치조회창 설정
//				- 설정 없음
//				- 설정 있음 ('팝업 사용' 및 '수치Dialog 사용' 옵션 관련 기능 없음, 무시)
//			- XFM, 수치조회창 사용여부 설정
//				- 수치Dialog 형태(Drag할때) 이용 설정	On / Off
//				- 십자선 보이기	설정					On / Off
//				- ToolTip								On / Off
//			- 환경 설정
//				- 십자선 사용시 연동 설정				숨김 / 보기
//				- 자동이동 여부 설정					자동이동 / 수동이동
//		2. 수치조회창 관련 Object
//			- 수치조회창	(with ToolBar or Not)
//			- 십자선		(with 수치조회창 or Not)
//			- ToolTip
//		3. 수치조회창 Using Case
//			- 수치조회창
//				- 수치조회창의 이용은 기본적으로 ToolBar내의 Button을 이용하여 생성한 수치조회창을 의미한다.
//					십자선과 연동하는 것은 십자선 이용시의 Option 기능으로 분류하여 수치조회창 기본기능으로 감안하지 않는다.
//					따라서, 수치조회창의 이용은 ToolBar의 Button을 통하거나, Script를 통하여 처리토록 한다.
//					특히 Script를 이용한 처리시 ToolBar가 없어도 처리될 수 있어야 한다.
//				- Code Info 1 (Case)
//					- By ToolBar
//					- By Script (만약 ToolBar에 해당 기능의 Button이 있다면 눌린 것으로 처리하여, ToolBar에 의해 해제가 가능하도록 한다.)
//				- Code Info 2 (Case)
//					- Auto Move
//					- Manual Move
//			- 십자선
//				- 십자선은 Mouse를 위치시켜 Scale의 위치를 알아 볼 수 있게 지원하는 Cursor Mode이다.
//					특히 수치조회창을 연동하여 함께 값을 조회할 수 있다.
//					XFM의 수치조회창 사용여부 설정에 의해서 지정된다.
//				- Code Info 1 (Case) ~ 특히 십자선은 기본적으로 LDown에서 LUp까지 나타나며, 다음과같이 여러 경우로 나타난다.
//					- 십자선 with 수치조회창 (ToolBar나 Script에 의해 이미 생성된 경우에는 십자선 종료시 닫지 않는다.)
//					- 십자선 without 수치조회창
//				- Code Info 2 (Case) ~ 이 옵션은 기본 십자선의 옵션 설정을 그대로 이용한다.
//					- Auto Move
//					- Manual Move
//			- ToolTip
//				- ToolTip은 Mouse를 위치시켜, 단간히 수치조회를 확인하는 Mouse Move 기능이다.
//					XFM의 수치조회창 사용여부 설정에 의해서 지정된다.
//				- Code Info 1 (Case)
//					- ToolTip
protected:
//	long	m_nNumericalInquiryOption;	// Script Property Memember [nNumericalInquiryOption]
										//	Design Property Data XFM Code Name NIO, Property Index 42
										//	수치조회창 1, 십자선 2, ToolTip 4 값의 조합으로 구성됨
										//	기본값 0
//	short	m_nUseInquiryWithCrossline;	// Script Property Memember [nUseInquiryWithCrossline]
										//	환경설정의 수치조회창, 십자선과 같이 보기 옵션 Flag
	BOOL	m_bUseInquiryData;			// 현재 수치조회창을 띄웠는지를 나타내는 Flag
										//	ToolBar 선택에 의해서 생성되거나, 십자선 연동에 의해 생성

// (2004.10.22, 배승원) Mouse Cursor의 등록 및 관리기능을 개선한다. (현 Cursor의 Handle만 관리한다.)
protected:
	HCURSOR		m_hCursor;
protected:
//	BOOL		OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);	// Message Map
//	LRESULT		OnCursorExchange(WPARAM wParam, LPARAM lParam);			// Message Map
public:
	HCURSOR		SetCursorHandle( HCURSOR p_hNewCursor);
	HCURSOR		GetCursorHandle( void)	{	return m_hCursor;	}

// (2004.11.02, 배승원) Mouse Capturing이 중첩적으로 처리되도록, 중첩 Count와 별도의 Capturing Interface를 제공한다.
protected:
	int		m_nMouseCaptureCount;		// 0 : Released Capturing State, n : Capture Count
public:
	CWnd *	AddSetCapture( void);
	BOOL	RemoveSetCapture( void);
// (2006/2/6 - Seung-Won, Bae) Manage Mouse DownUp Count for AddIn's Mouse Action ID
protected:
	DWORD	m_dwMouseActionID;
// (2006/2/6 - Seung-Won, Bae) for Mouse Action ID and Capture Count Handling
protected:
	void	OnLButtonDownProc(		UINT nFlags, CPoint point);
	void	OnLButtonUpProc(		UINT nFlags, CPoint point);
	void	OnLButtonDblClkProc(	UINT nFlags, CPoint point);
	void	OnRButtonDownProc(		UINT nFlags, CPoint point);
	BOOL	OnRButtonUpProc(		UINT nFlags, CPoint point);	// (2006/10/30 - Seung-Won, Bae) to deny Context Menu.
	void	OnRButtonDblClkProc(	UINT nFlags, CPoint point);

public:
	void	SetWholeView( const BOOL bWholeView);

// (2005/2/17 - Seung-Won, Bae) Point Data for Dragging Notify to DLL Continer
//		Flag Value :	LONG_MIN ~ No Drag
//						LONG_MAX ~ In Dragging
protected:
	CPoint	m_ptLDownPosition;

//sy 2005.04.26.
private:
	CString CHART_SKIN_NAME;
	bool ApplySkin();
	int GetSkinNumber() const;

//sy 2005.06.16.
private:
	int GetFullDataCount(const bool bIsHorzScaleDataCheck) const;
	bool IsScrollInHorzScaleData() const;

public:
	virtual CString GetDefaultPath();
	virtual CString	GetUserPath();
	virtual CString GetImagePath( void);	// (2009/9/2 - Seung-Won, Bae) for Image Path

	virtual void InvalidateOCX(const bool bErase = true);	// OCX Invalidate
	bool RestoreChartCfg(const int nIndex);	// 인덱스에 해당하는 차트화면으로 복원한다.
	virtual bool RemoveChartCfgFromIndexToEnd(const int nTailIndex);	// head ~ index까지만 유지한다.
	virtual bool AddRemoveGraph(LPCTSTR pcszGraphName);	// 지표Graph 추가/삭제를 한다.

// 종찬(05/07/12) 환경설정용. 함수들...
public:
	virtual void	SetNumericalInquiryOption(const long nInquiryOption, const bool bOutside = true);	// 수치조회창 표시여부 0:없음, 1:Window
	virtual void	SetShowTrendLineValue(const int nPosition, const bool bOutside = true);			// 추세선값 표시 (0: , 1: , 2: , 3: )
	virtual void	SetAutoSave(const bool bAutoSave, const bool bOutside = true);					// 차트종료시 자동 저장 유무
	virtual void	SetShowMinMaxPrice(const bool bShow, const bool bOutside = true);				// 최고가/최저가표시.
public:
	virtual bool	GetImaginaryCandle();						// 허수봉설정.
	virtual long	GetNumericalInquiryOption( void) const	{	return m_nNumericalInquiryOption;	}	// 수치조회창 표시여부 0:없음, 1:Window
	virtual bool	GetShowIndicatorValue();					// 지표정보 - 지표값 표시
	virtual int		GetShowTrendLineValue();					// 추세선값 표시 (0: , 1: , 2: , 3: )
	virtual bool	GetAutoSave();								// 차트종료시 자동 저장 유무
	virtual bool	GetShowMinMaxPrice();						// 최고가/최저가 표시
	virtual bool	SaveChartCfgInCurrentOCX();

	virtual CIndicatorList* GetIndicatorList(void);// CIndicatorList를 리턴한다.
	virtual CMainBlock *	GetMainBlock(void);
	virtual CPacketList*	GetPacketList(void);
	virtual CXScaleManager*	GetXScaleManager(void);
	// PacketListManager : 복수종목 - ojtaso (20070110)
	virtual CPacketListManager*	GetPacketListManager(void);
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual CScaleBaseData::HORZSCALEDRAWERTYPE GetMasterValueType(const int nRow, const int nCol);
	// PacketRQ의 상태를 알리도록 인터페이스 제공 : 복수종목 - ojtaso (20070531)
	void NotifyRQStatusToAddin( CCommonInterfaceEnum::PACKET_RQ_STATUS eStatus, LPCTSTR lpszRQ);

// 종찬(05/05/11) 툴바DLL 추가
private:
	// (2006/2/8 - Seung-Won, Bae) Create from Regular DLL
	CToolBarMgrHelper *	m_pToolBarMgrHelper;
	IToolBarManager *	m_pToolBarManager;	// 툴바 인터페이스.

public:
	void CreateToolBar();						// 툴바 생성.
	virtual bool	IsWholeView() const;				// 전체보기인지 리턴.
	virtual void	InvalidateOleControl(const bool bIsResetBlockRegion = false);		// COleControl의 InvalidateControl()을 호출한다.
	virtual BOOL	SetStartEndIndex(const int nStartIndex, const int nEndIndex);	// 스크롤바에 의해 새로 계산한 Start/End Index를 세팅한다.
	virtual int		GetPageCountNextStep(const int nSliderPos_Prev, const int nSliderPos_New, const UINT nSBCode, int nMin, int nMax);
//	virtual BOOL	GetReverse();
	virtual bool	RunToolFunction( const CToolOptionInfo::TOOLOPTION p_eToolOption, const int p_nParam);	// 툴기능실행/해제
	virtual bool	RunToolFuncGraphStyle(const int nParam);		// 그래프 형태를 설정한다.
	virtual bool	RunToolFuncLineThickness(const int nParam);		// 선 굵기를 설정한다.
	virtual bool	RunToolFuncGraphColor(const COLORREF clrGraphColor);	// 그래프 색을 설정한다.
	virtual void	SetStateWholeView(const int nParam = 0);		// 전체보기를 설정한다.
	virtual CWnd*	GetChartWindowPointer() const;	// OCX의 Window Pointer를 넘겨준다.
	virtual int		GetMinCountPerPage() const;	// 한페이지의 최소 데이타갯수를 넘긴다.
	virtual int		GetDataCountForToolBar() const;
	virtual CRect	GetToolBarRegion() const;
	virtual CFont*	GetRscFont() const;	// 리소스폰트를 가져온다.	
	virtual double	ConvertYPositionToData(const int nYPosition, const int nYPositionTop, const int nYPositionBottom,
						const double& dDataMax, const double& dDataMin, const bool bLog, const bool bReverse);	// 수평대를 구해오기위한 함수.
	virtual COLORREF GetToolTipBackColor( void)		{	return RGB( 255, 255, 255);	}
	virtual CRect	GetOcxRegion() const;							// OCX영역을 가져온다. -> (2006/3/6 - Seung-Won, Bae) Not use any more in ToolBar.
	virtual BOOL	IsOnCaptionFlagOfNIDLG( void) const		{	return FALSE;	}	// (2006/3/29 - Seung-Won, Bae) Dynamic Caption Setting for each Site.
	virtual int		GetBongCountMargin();							// 오른쪽 여백
// 툴바DLL 추가 끝!

private:
	BOOL IsRunTimeMode() const;	// 현재 DesignTime인지 RunTime인지를 리턴한다. TRUE: RunTimeMode.
	BOOL m_bRunTimeMode;	// 차트가 현재 DesignTime인지 RunTime인지 설정하는 변수.

private:
	COutsideCommonInterfaceFunction m_OutsideCommonInterfaceFunction;
	
// ChartObject용 CommonInterface.
public:
	virtual void BehaviorBlock(const long pBlockBehaviorData);
	virtual void SetGraphSelectedDataIndex(const int nDataIndex, LPCTSTR pcszGraphName);
	virtual void SetGraphThickness(const int nGraphThickness1);
	virtual void SetGraphTypeStyle(const int nGraphType, const int nDrawStyle);
//	virtual void SetGraphMargin(LPCTSTR pcszGraphName, const bool bIsAddMargin);
	virtual void SetCursorExchange(const CCursorShape::CHARTOBJECT enumChartObject);
	virtual int  GetUserMargin(void) const;
	virtual HWND GetWndHandle(void) const;
	virtual void InvalidateRect(const CRect& region, const bool  bErase);
	virtual int	 ShowIndicatorConfigDlg( CIndicatorInfo *p_pIndicatorInfo);
	// 그래프/타이틀 선택 때 RQ알려줌 : 복수종목 - ojtaso (20070106)
	virtual void SelectedGraph(LPCTSTR lpszRQ, BOOL bSendEvent = TRUE);
	// RQ별 Real등록/해제 : 복수종목 - ojtaso (20070119)
	virtual void SetDrdsAdviseRQ(LPCTSTR lpszRQ, BOOL bOnAdvise);
	// RQ 삭제 : 복수종목 - ojtaso (20070305)
	virtual void DeletedGraph(LPCTSTR lpszRQ, BOOL bRemovablePacketRQ);
	// VertScaleGroup의 가장 처음 Graph의 타입 : 복수종목 - ojtaso (20070504)
	virtual LPCTSTR GetMasterValueRQ(const int nRow, const int nCol);

public:
	bool	CalculateIndicatorInTRData(CIndicatorInfo* pIndicatorInfo, const CList<CPacket*, CPacket*>& packetList);

protected:
	void	 NeoFireDataUpandDown(int nOption);
	void	 NeoRequestBongData(int nOption, int nTemp);
	void	 NeoChangeOnePageDataCount( int nOnePageDataCount);
	void	 NeoRestore();
	void	 NeoRequestMapLoad(int nOption, CString strCode, CString strDate);
	void	 NeoRequestNewPacket(int nOption, CString strName, CString strCode, CString strDate);
	void	 NeoFireSaveSettedChartInfoToServer();
	void	 NeoFireGetSettedChartInfoFromServer();
	void	 NeoRequestPacketData(const CString& strDateType, const CString& strStartDate, const CString& strPacketNames);
	void	 NeoAddInEvent( const char *p_szAddInName, short p_nEventID, const char *p_szData, double p_dData, long p_lData);
	void	 NeoChangeDateIndex(int nDateIndex);
	void	 NeoRequestCodeAndName();
	void	 NeoSendDataIndexInLButtonUp(int nRowIndex, int nColumnIndex, int nDataIndex, double dValue);
	void	 NeoReturnOrgChart(const int nOnePageDataCount);

	//sy 2005.08.30.
	void OnPacketTRData();
	void OnPacketTRData(const CString& strTRPacketNames, const bool bIsReset);

	//sy 2005.09.08.
	BOOL SetAllProperties_NEOCONVERT( IPropertyMap *p_pIPropertyMap);
	void SetAllProperties_Chart();
	
	//sy test
	CString GetFilePathAndName(const CString& strFilePath, const CString& strFileName);

//sy 2005.11.09.
private:
	//TR data가 들어와서 setting 되는 부분.
	bool SetPacketDataInPacketList(LPCTSTR szPacketName, long szData, long lLength, BOOL bReset);

	bool m_bIncludeBongCountMargin; // 봉여백갯수를 포함하여 적용할것인가

//sy 2006.03.02
private:
	bool GetGraphDataForMaking(const CString& strGraphOption, CString& strGraphDataForMaking) const;
	bool IsBlockEmpty() const;
	void SetEmptyOcxData( const char *p_szChartList);
	void SetOneChart();

public:
	// 종찬(05/08/08) 해당패킷명으로 TR데이타를 요청한다.
	virtual void RequestTRPacket(const CString& strGraphName, const CString& strPacketNames);
	virtual void RequestTRPacket(const CString& strGraphName, const CString& strPacketNames, const int nCount, const bool bIsNew);
	bool GetRequestTRPacketName(const CString& strPacketNames, CString& strPacketNamesForTR) const;
	// scale setting 에 관련해서 Dll에 값을 알린다.
	virtual void OnScaleSettingMessage(const CCommonInterfaceEnum::ETCMESSAGETYPE eEtcMessageType, const int nData);

// (2004.06.08, 배승원) Global 공유 기능을 추가한다.
public:
	//////////////////////////////////////////////////////
	// Global 공유데이터
	class CGSharedCli : CGlobalSharedClient
	{
	public:
		CGSharedCli() : m_pGSharedSvr(NULL),m_bGetGData(FALSE),m_szGSharedData("") {}
		CDeskSharedServer*	m_pGSharedSvr;
		CList	<ST_GSHAREDFORMAT, ST_GSHAREDFORMAT &> m_ListShare;
		BOOL GetData(LPCSTR _szName, CString& szValue);
		LPCSTR GetData2(LPCSTR _szName);
		BOOL SetData(LPCSTR _szName, LPCSTR _szValue);
		BOOL Advise(LPCSTR _szName, CGlobalSharedClient* _pClient);
		BOOL UnAdvise(LPCSTR _szName, CGlobalSharedClient* _pClient);
		void MakeList(CString szList);
		POSITION FindShareData(LPCSTR _szName);
		// 데이터가 변경되었을 대 공유를 발생하도록 요청한다.
		void CallCheckSharedData(LPCSTR _szData);
		int  AddSharedNameToServer();
		int  RemoveSharedNameToServer();
		void IniStartGShareData();
		BOOL m_bGetGData;
		CString m_szGSharedData;
		// Virtual
		virtual BOOL ChangedData(LPCSTR _szName, LPCSTR _szValue) ;
		// (2004.06.11, 배승원) 공유의 Get/Set 상관없이 공유 설정 정보를 조회하는 Interface를 추가한다.
		ST_GSHAREDFORMAT FindShareInfo( LPCSTR _szName);
	};
protected:
	CGSharedCli		m_xGSharedCli;
	friend class	CGSharedCli;
protected:
	void	OnGSharedDataChange( const char *p_szGShareKey, const char *p_szGShareValue, BOOL p_bStartShare = FALSE);
	LRESULT	RmsgFormInializeComplete( WPARAM wParam, LPARAM lParam);
// (2004.06.11, 배승원) 다른 Class에서 Global 공유를 발생시킬 수 있도록 Interface를 제공한다.
// (2004.11.04, 배승원) CGSharedCli를 직접 Open하지 않는다.
public:
	BOOL				FireGSharedData( const char *p_szGSharedName, const char *p_szGSharedData);
	ST_GSHAREDFORMAT	FindGSharedDataInfo( const char *p_szGSharedName);
	BOOL				CheckGSharedSetting( const char *p_szGSharedName, BOOL p_bStart, BOOL p_bGet, BOOL p_bSet);
//////////////////////////////////////////////////////////////////////
// [KTBChartLShare.cpp] (2004.11.29, 배승원, 분석) Local 공유의 Interface를 별도로 관리한다.
//		차트에서의 Local 공유는 기본적으로 Real 요청을 위한 Key Code로 이용한다.
//////////////////////////////////////////////////////////////////////
// Local Share Info
protected: 
//	CString					m_strLocalSharedList;
	CLocalShareInfoList *	m_plLocalShareInfoList;
// (2004.11.29, 배승원, 분석) SetAllProperties2()에서 호출되어, 설정된 공유 설정 String인
//		m_strLocalSharedList를 m_ListLShare로 구성하는 작업을 처리한다.
//		특히 이때, 초기 공유값 확인과 공유 변경 Notify를 수신을 요청토록 한다.
protected:
	BOOL					InitLSharedData();
// Local Shared Data Receiving Routine
protected:
	afx_msg LRESULT Rmsg_LSharedChaned(WPARAM wParam, LPARAM lParam);
//////////////////////////////////////////////////////////////////////
// (2004.11.29, 배승원) 변경된 공유 전달 방식이 Message 방식을 이용하면서,
//		이전의 Interface 통보 방식은 이용되지 않고 있다.
//	ILSharedData *	m_pILSharedData;
//	DECLARE_INTERFACE_MAP()
//	BEGIN_INTERFACE_PART( LAdviseSink, ILAdviseSink)
//		STDMETHOD( OnDataChange) ( LPCSTR szSharedName, LPCSTR szData);
//	END_INTERFACE_PART( LAdviseSink)
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// [KTBChartReal.cpp] (2004.11.17, 배승원, 분석) Real 관련 Interface이다.
//		Real의 처리를 위해서는 기본적으로 3가지 자료구조가 이용된다.
//		이것은 3가지 개념의 상호 연동에 의한 것이기 때문이며,
//			경우에 따라 이 3가지 자료구조가 불일치하고, 
//			이를 적절하게 운용해 나가는 절차가 필요하기 때문이다.
//	1. Local Share Info
//		- Real을 요청하기 위한, Key Code를 관리하는 자료구조이다.
//		- XFM의 설정에서와 같이 Local Share Name과 그 속성을 관리한다.
//		- Local 공유값이 변경되어도 Real을 직접 조정하지 않고,
//			OnDrdsAdvise() 처리시에만 Real에 반영한다. (즉시 처리시 머리 아파진다.)
//		-  Data :
//				Local Share Name
//				Local Shared Data
//				Start	Share Flag
//				Set		Share Flag
//				Get		Share Flag
//		- Local Share Name이 Key이며, 이를 기준으로 Real 요청시 참조된다.
//	2. 운용법
//		2.1 SetAllProperties2()에서 XFM의 Real 설정인 m_strRealList를 분석하여 Real Setting Info를 구성한다.
//			(이렇게 구성된 Real Setting Info는 Destructor에서나 제거된다.)
//		2.2 이어, InitLSharedData()를 통해 Real을 요청하기위해 필요한 Real Key Code의 Local Share Info를 구성한다.
//		2.3 Map Loading이 되고, 초기 Local 공유가 전달되어 설정된다. (Start Flag이 아니어도 Start 효과가 날듯)
//		2.4 TR이 수신되어 Real이 Off/On되면서 구성된 Real Setting Info와 Local Share Info를 바탕으로 Real을 요청하게 된다.
//		2.5 OnDrdsData()와 OnDrdsPacketEnd()를 통해서 Real Data를 처리하게 된다.
//				특히 OnDrdsPacketEnd()의 처리에서는 m_bRedrawAfterUpdatedAllPackets의 값에 따라 요청한 Real Packet이
//					모두 1회 이상 수신된 경우에만 Drawing과 기타 Real 처리 마무리 작업을 진행하게 된다.
//		2.6 이렇게 Real이 수신되는 와중에 공유가 변경되거나, Real 설정이 변경되면,
//				Local Share Info나 Real Setting Info만 변경된다. (즉시적으로 Real 요청에 반영되지는 않는다.)
//		2.7 Off/OnDrdsAdvise()를 통하여 조정된 공유나 Real 설정에 의한 Real을 다시 요청한다.
//				(이때 Real은 일괄 재요청되며, DRDS Advised Info도 함께 갱신된다.)
//		2.8 종료시 Window가 제거되면서(OnDestroyWindow) OffDrdsAdvise()가 호출되 Real 수신을 끝낸다.
//		2.* 주의! Real Setting Info와 Local Shared Data가 변경되어도 즉시 Real Advise에는 적용하지 않는다.
//				항상 OnDrdsAdvise()를 통하여만 적용되도록 한다.
//				특히 XFM에 설정되지 않고, 즉시적으로 Real의 설정을 지정하는 경우에도
//				OnDrdsAdvise()를 통해서만 적용되도록 한다. 안그럼 너무 머리 아프다.
//		2.* 주의! Method에 의한 Real Setting Info와 Local Share Info의 설정 및 제거 String은
//				Map의 설정 String과 Parsing Routine을 공유하도록 Format을 통일한다. (구분자를 ','에서 '.'으로 변경한다.)
//////////////////////////////////////////////////////////////////////
// (2006/7/25 - Seung-Won, Bae) Use Real Module with AddIn
protected:
	IRealReceiver *	m_pIRealReceiver;
// Real Data Process Routine
protected:
	// Notify Routine of 1 Real Field
	//		int			wParam	~ Size of DrdsData, Fixed Value
	//		DrdsData *	lParam	~ Data Structure
	LONG	OnDrdsData(UINT wParam, LPARAM lParam);
	// Notify Routine of 1 Real Packet Process End
	//		const char *wParam	~ Packet Name
	//		const char *lParam	~ Key Code
	LONG	OnDrdsPacketEnd(UINT wParam, LPARAM lParam);

// bds test -------------------------------------------------------------------------
// 종찬(05/02/15) KTBChart포팅작업에서 메소드를 맞추는데 필요한 Field, Function선언.
private:
	const CString FILETYPE;

	int OpenFileAndSave(CString& strOldFileName, const CString& strNewFileName, const bool bIsCurrentSave, const CSaveUserFile::FILETYPE eFileType);

// (2005.01.20, 배승원) ToolBar에 다음 Item이 있는지 확인하는 Interface를 제공한다.
public:
	BOOL	HasToolItem( CToolOptionInfo::TOOLOPTION p_eToolItem);

// (2006/1/23 - Seung-Won, Bae) Manage Log Flag. It Managed by IChartCtrl in ChartLogAddIn
protected:
	BOOL	m_bInLogging;

// (2006/1/28 - Seung-Won, Bae) Reset Tool UI
protected:
	void	ReleaseOtherToolUI( const CToolOptionInfo::TOOLOPTION p_eToolOption);

// (2006/7/5 - Seung-Won, Bae) Manage the flag of Full GetAllProperties3()
protected:
	BOOL	m_bFullGetAllProperties3;

// (2006/7/26 - Seung-Won, Bae) Manage Removed Packet Data Count for Reset Scroll in OnDrdsPacketEnd().
protected:
	int		m_nRemovePacketDataCount;

// (2006/7/31 - Seung-Won, Bae) Manage AddIn Reload Flag of SetAllProperties3 for AddIn
protected:
	BOOL	m_bSetAllProperties3WithReloadAddIn;

// (2006/8/7 - Seung-Won, Bae) for checking AddIn Site Number
protected:
	int		GetAddInSiteID( void)	{	return 13;	}

// (2006/8/23 - Seung-Won, Bae) Manage AddIn Load String
//		Format	: DDDDDD:AAAA1,AAAA2,...AAAAN:BBBB1,BBBB2,...BBBBN
//					AAAA1~BBBBN can be none.
//					so "DDDDDD::" is minimum format.
//protected:
//	CString		m_strAddInLoadList;	// Map Property
protected:
	friend class CKTBChartApp;
	static CMapStringToString	m_mapAddInLoadString;
// (2006/8/23 - Seung-Won, Bae) Do not support the step for each Version Up.
//			Because the Base String for each version is not managed.
//		p_szOldVersion	: "DDDDDD:"
//		p_szRemove		: "AAAA1,..." ~ ""	(Caustion! It is not need to be sorted by OLD order)
//		p_szInsert		: "AAAA1,..." ~ ""	(Caustion! Must be sorted by NEW order)
//							if "AAAA1" is in p_szRemove and p_szInsert, then "AAAA1" is always inserted in the New position.
//		p_bReposition	: if TRUE and "AAAA1" is in p_szRemove and p_szInsert, then "AAAA1" is inserted in the new position when It is in the Target Data.
//			Caution. If "AAAA1" is in p_szInsert and in the Target Data. then "AAAA1" is inserted twice.
//						If you want to do ONLY reposition, then "AAAA1" is in p_szRemove and p_bReposition is TRUE.
protected:
	BOOL		AddInNameListVersionUpFor( const char *p_szOldVersion, const char *p_szRemove, const char *p_szInsert, BOOL p_bReposition = FALSE);
	BOOL		AddInNameListParse( char *&szItem, char *&szItemNext, int *pnItemLength = NULL, int *pnItemOver = NULL);

// (2006/10/23 - Seung-Won, Bae) Manage the Flag of Showing the Price Min/Max Data Text.
protected:
	BOOL	m_bShowPriceMinMaxText;

// (2006/10/30 - Seung-Won, Bae) Support Map Indicator Info String for Multiple-Step of Set/GetAllProperties()
protected:
	CString	m_strChartList;
	CString	m_strMapIndicatorInfo;

// (2006/11/12 - Seung-Won, Bae) Temp Variable for Vertical Scale Min/Max of View Menu
protected:
	BOOL	m_bVScaleMinMaxOfView;

// (2006/11/15 - Seung-Won, Bae) Notify Tool State Changing to MChart an ToolBar
public:
	BOOL	OnToolStateChanged( const CToolOptionInfo::TOOLOPTION p_eToolID, const BOOL p_bTurnOn);

// (2006/11/17 - Seung-Won, Bae) Graph Add/Remove State Event Process
public:
	virtual BOOL	OnGraphAdded( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName);
	virtual BOOL	OnGraphDeleted( const char *p_szRQName, const char *p_szGraphName, const char *p_szIndicatorName);

// (2006/12/20 - Seung-Won, Bae) Support Binary TR Data Receiving
protected:
	CScaleBaseData::HORZSCALEDRAWERTYPE		m_eBinTrPrevUnitType;
	int										m_nBinTrPrevTimeInterval;

// (2007/1/20 - Seung-Won, Bae) Support Multiple Same-Name Indicator.
public:
	BOOL	m_bMultipleSameNameIndicator;

// (2007/1/20 - Seung-Won, Bae) Remove Graph with Graph Name.
public:
	BOOL	RemoveGraphWithGraphName( const char *p_szGraphName);

// (2007/1/22 - Seung-Won, Bae) Add Indicator and Get Graph Name.
public:
	BOOL	AddChartBlockWithGetGraphName( const char *p_szIndicatorName, CString &p_strGraphName);

private:
	CPacketListManager*		m_pPacketListManager;	// 복수의 PacketList를 관리 : 복수종목 - ojtaso (20061224)
	CString					m_strRQ;				// 현재 RQ : 복수종목 - ojtaso (20061224)
	BOOL					m_bResetChart;			// 차트 초기화때는 종목변경 하지 않음 : 복수종목 - ojtaso (20070621)
	BOOL					m_bUseMultiItemMode;	// 차트DLL에서 복수종목을 사용할지 : 복수종목 - ojtaso (20070904)

// (2007/2/22 - Seung-Won, Bae) User Zoom Changed.
public:
	void	OnUserZoomChanged( const int p_nDataCountInView);

// (2007/3/4 - Seung-Won, Bae) Make Thread for the 'CLUTCH' of Real Update.
protected:
	long	m_lDrawingLoadTime;
	BOOL	m_bLockWindowUpdate;
public:
	void	InvalidateControl( LPCRECT lpRect = NULL, BOOL bErase = TRUE);
	void	InvalidateRgn( CRgn* pRgn, BOOL bErase = TRUE );

// (2007/7/2 - Seung-Won, Bae) Do not Support Main Grid Type.
//	for Old Version Map and User File.
protected:
	int	m_nScaleLineType;
	int m_nVertScaleType;
	int m_nHorzScaleType;
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	CString	m_strSTExtPacketName;
	
public:
	// 조건값 사용자 지정 가능 - ojtaso (20070913)
	virtual void ReCalculateIndicator();
	// ST에서 사용하는 기본 패킷 이외의 사용 가능한 데이터 추가 - ojtaso (20080609)
	virtual CString	GetUseSTDataName();

// (2008/1/2 - Seung-Won, Bae) Support MultiLanguage Version.
protected:
	HWND		m_hOcxWnd;
	CString		m_strLocalLanguageTable;

// (2008/3/7 - Seung-Won, Bae) Backup R-Button Menu popup Position for ON_UPDATE_COMMAND_UI and ON_COMMAND
protected:
	CPoint		m_ptRButtonMenuPopupPosition;	

// (2008/4/29 - Seung-Won, Bae) to use the ProgID in ChartMetaTable.dat
protected:
	void		GetProgID( CString &p_strProgID)	{	p_strProgID = _OCX_PROG_ID;	}

// (2008/5/13 - Seung-Won, Bae) Chart TEST ENV for Runtime.
protected:
	static const int			m_nTestEnvBufferSize;
	static BOOL					m_bTestEnvInitialized;
	static CMapStringToString	m_mapTestEnvSetting;
public:
	BOOL	GetTestEnvValue( const char *p_szEnvKey, CString &p_strEnvValue);
protected:
    SYSTEMTIME					m_stLogStart;
	long						m_lDataCount;
	BOOL						m_bEnableLogAddIn;
	BOOL						m_bEnableLogSaveAndOpenFromFile;
	BOOL						m_bEnableLogChartLoading;
	BOOL						m_bEnableLogSetGetAll;

// (2008/5/22 - Seung-Won, Bae) Use page info for User File Path Redirection.
protected:
	BOOL		WriteUserFiles( void);
	BOOL		ReadUserFiles( void);
	void		SetPageInfo( LPCTSTR p_szPagePath, BOOL p_bIsVirtualPage, short p_nVirtualPageIndex, short p_nDuplicateIndex);
	// (2008/6/9 - Seung-Won, Bae) Check saved file. if not found, Use not-paged file.
	void		CheckPagedFile( const char *p_szPagePath, const char *p_szFileName);

// (2008/12/21 - Seung-Won, Bae) Manage On Real Processing
protected:
	BOOL		m_bOnRealProcessing;
protected:
	void		SetOnRealProcessing( BOOL p_bOn)	{	m_bOnRealProcessing = p_bOn;	}

// (2009/1/5 - Seung-Won, Bae) Update ScrollBar and Zoom SliderBar from XScaleManager by Real.
public:
	virtual void	UpdateZoomAndScrollInfo( int p_nFullRange, int p_nStartPos, int p_nViewCount);

// (2009/1/11 - Seung-Won, Bae) for X Scale Manager's Calc_DRI (Zoom)
protected:
	BOOL		m_bHideDrawForCalc;

// (2009/1/14 - Seung-Won, Bae) Support threaded drawing.
protected:
	//friend UINT CKTBChartCtrl_PrepareOnDraw( LPVOID pParam);
	CDC				m_dcThread;
	//BOOL			m_bOnPrepareOnDraw;

/////////////////////////////////////////////////////////////////////////////
// (2009/1/18 - Seung-Won, Bae) Manage Zoom and Scroll Info.

// Zoom and Scroll Setting value infos.
protected:	
//	int		m_nOnePageDataCount;			// It is supported as property.
//	int		m_nMinDataCountOnOnePage;		// It is supported as property.
//	BOOL	m_bOnePageDataCountInTRCount;	// It is supported as property.
//	BOOL	m_bWholeView;					// It is supported as property.

// Zoom Method.
protected:	
	void			DragZoomIn( const CPoint &p_ptStart, const CPoint &p_ptEnd);	// Mouse Drag Zoom In !
	virtual void	GetClientRect( CRect *p_rctClient) const		{	COleControl::GetClientRect( p_rctClient);	}

/////////////////////////////////////////////////////////////////////////////

// (2009/5/19 - Seung-Won, Bae) for Auto Resize of Print and Image Capture
protected:
	BOOL						m_bLockScrollUIUpdate;
	BOOL						m_bWholeViewForLock;
	long						m_nOnePageDataCountForLock;
	int							m_nStartDrawIdxCurForLock;
	BOOL						m_bIsFixedIntervalForLock;
	CXScaleManager::RSA_TYPE	m_eCandleTypeOnResizeForLock;
public:
	void						LockScrollUIUpdateWithScrollInfo( BOOL p_bLockScrollUIUpdate);
	virtual CRect				GetAllBlockRegion( void);

// (2009/5/13 - Seung-Won, Bae) for Right Margin
protected:
	int		m_nRightMarginWidth;
public:
	void	SetRightMarginWidth( int nRightMarginWidth);
	int		GetRightMarginWidth( void)		{	return m_nRightMarginWidth;	}

// (2009/5/17 - Seung-Won, Bae) for Future Time
protected:
	int		m_nFutureTimeCount;
public:
	void	SetFutureTimeCount( int nFutureTimeCount);
	int		GetFutureTimeCount( void)		{	return m_nFutureTimeCount;	}

// (2009/4/13 - Seung-Won, Bae) ChartList for AddIn Interface.
protected:
	const char *	GetChartList( void);
	void			SetChartList( LPCTSTR lpszNewValue);

// (2009/6/5 - Seung-Won, Bae) Check mode for moving and deleting the graph.
public:
	BOOL	CanGraphMoveAndDelete( void)	{	return m_bSelectTitle;	}

// (2009/6/5 - Seung-Won, Bae) for CmdUI of AddIn
public:
	void	OnCmdMsg( HMENU p_hMenu, UINT p_nItemID, int p_nItemPos, int p_nItemCount, int p_nCmdCode);

// (2009/9/1 - Seung-Won, Bae) Define Chart Mode. (Load setting.)
protected:
	static CChartInfo::CHART_MODE		m_nChartModeSetting;

// (2009/9/2 - Seung-Won, Bae) Support Chart Mode. (for HTS/FX/WorldOn)
protected:
	virtual CChartInfo::CHART_MODE		GetChartMode( void) const		{	return ( CChartInfo::CHART_MODE)m_nChartMode;	}

// (2009/9/2 - Seung-Won, Bae) Check Market Index with search.
protected:
	static int		GetSubMenuIndex( CMenu *p_pMainMenu, const char *p_szMenuString);
	static CMenu *	GetSubMenu( CMenu *p_pMainMenu, const char *p_szMenuString);

// (2007/6/19 - Seung-Won, Bae) Support Only Draw Condition of P&F for User Setting.
//		Clear Draw Condition for Reset User Setting.
protected:
	void		ResetUserCndForAutoCnd( void);

	// FX차트 주문 연동 - onlyojt (20091204)
	BOOL		OnOrderInfomationChanged(const double dPirce, const int Qty, const int nOrderType, LPCTSTR szOrgOrderID, LPCTSTR strCustItem1, LPCTSTR strCustItem2, LPCTSTR strCustItem3);

protected:
	//(2012/01/19 - alzioyes) save to file with chart's properties
	BOOL SaveChartPropertiesToChartMapFile(LPCSTR szSaveFile);

	//(2012/01/19 - alzioyes) Load form file and set properties to chart
	BOOL LoadChartPropertiesToChartMapFile(LPCSTR szLoadFile);

// (2009/12/11 - Seung-Won, Bae) Append lost NMTR data.
public:
	void	SetFirstNMTRClock( void);

	void	ResetBound(LPCSTR lpszBound, double& dMinValue, double& dMaxValue);	

protected:
	int		m_nMouseWheelIndex;
	int		m_nMinMaxRatio;

public:
	void	SetMouseWheelIndex( int nMouseWheelIndex) { m_nMouseWheelIndex = nMouseWheelIndex; }
	int		GetMouseWheelIndex( void)		{	return m_nMouseWheelIndex;	}

	void	SetPriceMinMaxShowType( int nMinMaxRatio);
	int		GetPriceMinMaxShowType( void)		{	return m_nMinMaxRatio;	}
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEOCHART2001CTL_H__D585DB92_DE13_4572_BC53_1A626B82D052__INCLUDED)
