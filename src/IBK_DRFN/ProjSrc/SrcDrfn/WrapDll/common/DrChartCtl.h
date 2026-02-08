#if !defined(AFX_DRCHARTCTL_H__12DC6B96_BDB6_48FE_B75F_9A17190897E7__INCLUDED_)
#define AFX_DRCHARTCTL_H__12DC6B96_BDB6_48FE_B75F_9A17190897E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DrChartCtl.h : header file
//

#include "../../DrCommon/DRChartOcx.h"

#include "DataComm.h"
#include "ChartMng.h"
#include "RealMng.h"
#include "WinixConverter.h"
#include	<AFXCTL.H>

//-->RunChart
#include "RunChart/MsgForChart.h"
#include "RunChart/CtrlPackBase.h"
#include "RunChart/Include_Chart/IRealReceiver.h"
//<--RunChart

#include "../../inc/IAUPartnerMng.h"

#define		ID_CHART_WRAPWND	56801
#define		ID_CHART			56802

#define NMRETURN_MAXLENGTH				1024
#define NMPARAM_MAXCOUNT				10

#define		WMU_SETNEXTKEY		WM_USER + 2054
#define		WMU_GETNEXTKEY		WM_USER + 2055

#define		STATE_NONE				0
#define		STATE_STARTRECEIVE		1
#define		STATE_ENDRECEIVE		2
#define		STATE_CHANGEONPAGECNT	3
#define		STATE_SENDTR			4
#define		STATE_DESTROYCHART		5

///////////////////////////////////////////////////////////////////////////
// CDrChartCtl window
class CDrChartCtl : public CWnd
{
// Construction
public:
	CDrChartCtl();

// Attributes
public:
	CDRChartOcx		*m_pChart;
	CWnd			*m_pExtraChart;
	BOOL			m_bUseExtraChart;

	_param			m_ctlInfo;
	
	CTrPack			m_query;
	CStringList		m_propList;
	
	HWND			m_hCtrlWnd;
	char			*m_pszLayout;
	
	int				m_nFID, m_nCnt;
	
	CWinixConverter	m_converter;

	CDataComm		m_dataComm;
	CRealMng		m_realMng;
	CChartMng		m_chartMng;

	long			m_lState;

	List_CChartBozoItem		m_bozoItemList;

	CFont			m_Font;
	
private:
	//For DrChartCtl_Util.cpp
	IDispatch			*m_pIChartDisp;
	IRealReceiver		*m_pIRealReceiver;
	CMsgForChart		m_Msg;
	CBaseCtrlPackOut	*m_pIPackOut;

	// 복수종목 맵차트 추가 - ojtaso(20090928)
	CMapStringToString	m_mapRQByItemCode;
	CString				m_strCurrentRQ;
	CStringArray		m_arrAddPacket;

	BOOL TrimLR( char *&p_szItem, int p_nItemLen);
	BOOL Chart_Method( const char *p_szMethodName, const char *p_szParams, char *p_szReturn);
	HRESULT	GetIDsOfNames( const char *p_szProperty, DISPID &p_diProperty);
	BOOL Chart_PropertyGet( const char *p_szProperty, DISPID p_diProperty, char *p_szReturn);
	BOOL Chart_PropertySet( const char *p_szProperty, DISPID p_diProperty, const char *p_szValue, const char *p_szValueType);
	CString TokenizeEx(CString strSrc/*IN*/, const char* pszTokens/*IN*/, int& iStart/*IN, OUT*/);

	//For DrChartCtl_Real.cpp
	//-->100309-New Real
//@100521-by alzioyes.  더이상 사용안함. 	long	DoRealData(WPARAM wParam, LPARAM lParam);

	long	DoRealData_NewReal(WPARAM wParam, LPARAM lParam);
	long	DoRealData_NewReal_Sub(CString& strCode, DWORD* pArrData);
	//<--100309-New Real

	//-->100309-New Real
	BOOL	SetRealDataRQ(CString strRQ, CChartProp* pProp, CMap<int, int, CString, LPCTSTR> &map, BOOL bAdv);
	BOOL	SetRealData(CChartProp* pProp, CMap<int, int, CString, LPCTSTR> &map, BOOL bAdv);

	BOOL	SetRealDataRQ_NewReal(CString strRQ, CChartProp* pProp, DWORD* pArrData, BOOL bAdv);
	BOOL	SetRealData_NewReal(CChartProp* pProp, DWORD* pArrData, BOOL bAdv);
	//<--100309-New Real

	void	SetInputData(CString strData, CFidQuery &qry);
	void	SetCodeData(CString strCode, CFidQuery &qry);

public:
	static int	RealDataParsing(char* pszBuff, CMap<int, int, CString, LPCTSTR> &map);

	//100309-New Real
	static int	RealDataParsing_NewReal(DWORD* pArrData, CMap<int, int, CString, LPCTSTR> &map);

// Operations
public:
	void		SetCtlInfo(_param* pInfo);
	
	void		LoadFile();
	BOOL		SetAllPropertiesToOCX();

	void		ReceiveData(const char* pszData, int nSize, int nKey);
	UINT		SendOnChange();
	void		InitFont();

	int			ReSetPacket(CString &strValue, CString &strBozoMsg, CString strPacketType="");

	void		SetAdvRealData(int nTRType, CChartProp info);
	CString		GetNormalRealSet(CString strCode, CString strRealSym = "");
	CString		GetMarketSymbol(int nMarket);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrChartCtl)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrChartCtl();

	UINT HashDataAXIS(LPCSTR key);
	// Generated message map functions
protected:
	//CPCTrMngHelper	m_PctrHelper;
	IDebugManager			*m_pIDebugMng;

	IPartnerSvrManager	*m_pPtSvrMng;
	IPartnerManager		*m_pPtMng;
	void _DoGetPartnerManger(BOOL _bAdvise=TRUE);
	void _DoAdviseToMonitor(BOOL _bAdvise);	//For Test.

	//{{AFX_MSG(CDrChartCtl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LONG OnUser(WPARAM wParam, LPARAM lParam);
	// 복수종목 맵차트 추가 - ojtaso(20090928)
	LRESULT OnEventMessage( WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CDrChartCtl)
	afx_msg void SetSubMsg(LPCTSTR strSubMsg, LPCTSTR strParam);
	afx_msg BSTR GetChartProperty(LPCTSTR szPropName);
	afx_msg BOOL SetChartProperty(LPCTSTR p_szPropName, LPCTSTR p_szValue, LPCTSTR p_szValueType);
	afx_msg BOOL LoadAddIn(LPCTSTR sz_AddInDllName);
	afx_msg short SetChartData(LPCTSTR msg);
	afx_msg short SetChartData2(LPCTSTR szMsg);
	afx_msg void SetChartRealData(LPCTSTR szPacketName, double dData, short nOption);
	afx_msg BOOL ShowBlock(LPCTSTR szPacketName, short nOption);
	afx_msg LPDISPATCH GetDRChartDispatch();
	afx_msg long ChangeMajorFID(LPCTSTR pstrMajor);
	afx_msg long ChangeMinorFID(LPCTSTR pstrMinor);
	afx_msg void ChangeGridSymbol(LPCTSTR pstrSymbol);
	afx_msg void ChangeGridSymbol2(LPCTSTR pstrSymbol, LPCTSTR pstrFIDCode);
	afx_msg long SendTR(LPCTSTR pstrTRName, LPCTSTR pstrCode, LPCTSTR pstrInData, BOOL bNext);
	afx_msg long SendTrDifferReal(LPCTSTR pstrTRName, LPCTSTR pstrInData, LPCTSTR pstrRealSym, BOOL bNext);
	afx_msg BOOL AddChartItem(short nRowIdx, LPCTSTR pstrCode, LPCTSTR pstrName, LPCTSTR pstrGraphName, BOOL bBlock, BOOL bVScale);
	afx_msg BOOL ChangeChartItem(LPCTSTR pstrCode, LPCTSTR pstrName);
	afx_msg void AddPacketRQ(LPCTSTR pstrPacket);
	afx_msg void DelAllPacketRQ();
	afx_msg void DelChartItem(LPCTSTR pstrCode);
	afx_msg long GetChartState();
	afx_msg BOOL ChangeGraphTitleRQ(LPCTSTR pstrCode, LPCTSTR pstrTitle);
	afx_msg BOOL ChangeGraphColorRQ(LPCTSTR pstrCode, LPCTSTR pstrColor);
	afx_msg BOOL ChangeGraphTitle(LPCTSTR pstrName, LPCTSTR pstrTitle);
	afx_msg BOOL ChangeGraphColor(LPCTSTR pstrName, LPCTSTR pstrColor);
	afx_msg void SetCurrentRQ(LPCTSTR pstrCode);
	afx_msg void AddBozoMsg(short nType, LPCTSTR pstrKey, LPCTSTR pstrValue);
	afx_msg void DelAllBozoMsg();
	afx_msg void SetDiffGridSymbol(LPCTSTR pstrSymbol);
	afx_msg void SetDiffGridSymbol2(LPCTSTR pstrFIDCode, LPCTSTR pstrSymbol);
	afx_msg void SetRowMaxCount(LPCTSTR pstrFIDCode, long lMaxCount);
	afx_msg long SetValueToScale(LPCTSTR szKey, BOOL dUse);
	afx_msg void SetRedrawCall(BOOL bUse);
	afx_msg void SetDefaultRQ(BOOL bSetDisable, LPCTSTR szKey, LPCTSTR szData);
	afx_msg BOOL SetLinePropRQ(LPCTSTR pstrCode, LPCTSTR pstrData);
	afx_msg BSTR GetLinePropRQ(LPCTSTR pstrCode);
	afx_msg void UseExtraChart(LPCTSTR szChartName, BOOL bUse);
	afx_msg void SetShowYScaleData(LPCTSTR szGraphName);
	afx_msg void SetChartDataType(LPCTSTR szType);
	afx_msg void SetIndicatorHide(LPCTSTR szGraphName, LPCTSTR szHideData);
	afx_msg BSTR GetIndicatorHide(LPCTSTR szGraphName, short nCount);
	afx_msg void SetNextKey(LPCTSTR szNextKey);
	afx_msg BSTR GetNextKey();	
//	afx_msg void ChangePacket(LPCTSTR pstrPreName, LPCTSTR pstrName, LPCTSTR pstrFidName, LPCTSTR pstrType, LPCTSTR pstrSize, LPCTSTR pstrReal);
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	LRESULT OnGetUserID( WPARAM wParam, LPARAM lParam);

	// Event maps
	//{{AFX_EVENT(CDrChartCtl)
	//}}AFX_EVENT
	//DECLARE_EVENT_MAP(
	// Events
	// Connection point container

	virtual LPCONNECTIONPOINT GetConnectionHook(REFIID iid);
	virtual BOOL GetExtraConnectionPoints(CPtrArray* pConnPoints);

	BEGIN_CONNECTION_PART(CDrChartCtl, EventConnPt)
	virtual void OnAdvise(BOOL bAdvise);
	virtual REFIID GetIID();
	virtual LPUNKNOWN QuerySinkInterface(LPUNKNOWN pUnkSink);
	END_CONNECTION_PART(EventConnPt)

	static const AFX_DATA AFX_EVENTMAP_ENTRY _eventEntries[];
	virtual const AFX_EVENTMAP* GetEventMap() const;
	static const AFX_DATA AFX_EVENTMAP eventMap;
	const AFX_EVENTMAP_ENTRY* GetEventMapEntry(LPCTSTR pszName,
		DISPID* pDispid) const;
	void FireEventV(DISPID dispid, BYTE* pbParams, va_list argList);
	void AFX_CDECL FireEvent(DISPID dispid, BYTE* pbParams, ...);

	const IID* m_piidEvents;            // IID for control events
	const IID* m_piidPrimary;			// IID for primary idispatch
	//DECLARE_EVENTSINK_MAP()

	enum {
		//{{AFX_DISP_ID(CDrChartCtl)
	dispidSetSubMsg = 1L,
	dispidGetChartProperty = 2L,
	dispidSetChartProperty = 3L,
	dispidLoadAddIn = 4L,
	dispidSetChartData = 5L,
	dispidSetChartData2 = 6L,
	dispidSetChartRealData = 7L,
	dispidShowBlock = 8L,
	dispidGetDRChartDispatch = 9L,
	dispidChangeMajorFID = 10L,
	dispidChangeMinorFID = 11L,
	dispidChangeGridSymbol = 12L,
	dispidChangeGridSymbol2 = 13L,
	dispidSendTR = 14L,
	dispidSendTrDifferReal = 15L,
	dispidAddChartItem = 16L,
	dispidChangeChartItem = 17L,
	dispidAddPacketRQ = 18L,
	dispidDelAllPacketRQ = 19L,
	dispidDelChartItem = 20L,
	dispidGetChartState = 21L,
	dispidChangeGraphTitleRQ = 22L,
	dispidChangeGraphColorRQ = 23L,
	dispidChangeGraphTitle = 24L,
	dispidChangeGraphColor = 25L,
	dispidSetCurrentRQ = 26L,
	dispidAddBozoMsg = 27L,
	dispidDelAllBozoMsg = 28L,
	dispidSetDiffGridSymbol = 29L,
	dispidSetDiffGridSymbol2 = 30L,
	dispidSetRowMaxCount = 31L,
	dispidSetValueToScale = 32L,
	dispidSetRedrawCall = 33L,
	dispidSetDefaultRQ = 34L,
	dispidSetLinePropRQ = 35L,
	dispidGetLinePropRQ = 36L,
	dispidUseExtraChart = 37L,
	dispidSetShowYScaleData = 38L,
	dispidSetChartDataType = 39L,
	dispidSetIndicatorHide = 40L,
	dispidGetIndicatorHide = 41L,
	dispidSetNextKey = 42L,
	dispidGetNextKey = 43L,
//	dispidChangePacket = 44L,
	//}}AFX_DISP_ID
	};

	BOOL m_bUseValueToScale,
		 m_bReDrawCall,
		 m_bDisableRQSet;	// "DEFAULT" RQ를 사용되지 않게 처리.

	int	 m_nViewCount;

	POSITION FindBozoMsgKey(LPCTSTR pstrKey);	//BozoMsg Key Search.

	long SendTrChartGraphDifferReal(LPCTSTR pstrTRName, LPCTSTR pstrCode, LPCTSTR pstrInData, LPCTSTR pstrRealSym, BOOL bNext);
	CString ConvertToValidData( CString strData );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRCHARTCTL_H__12DC6B96_BDB6_48FE_B75F_9A17190897E7__INCLUDED_)
