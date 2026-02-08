// DrChartCtl.cpp : implementation file
//

#include "stdafx.h"
#include "DrChartCtl.h"
#include "MultiBar.h"

#include "../common/PropertyMap.h"
#include "../common/DrMapTrDef.h"

#ifdef __MULTI_LANG__
	#include "../../Chart/Include_Chart/Dll_Load/IMetaTable.h"	//@Solomon-MultiLang:091117
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_DPP_LAYOUT_AND_COLOR	41101
#define	g_TestOnCopyStart		10

#define IDTIMER_NEXTSELECT		1

// 복수종목 맵차트 추가 - ojtaso(20090928)
#define _CHARTMSG_EVENTMESSAGE	"CHARTMSG_EVENTMESSAGE"

const UINT RMSG_CHARTMSG_EVENTMESSAGE	= ::RegisterWindowMessage( _CHARTMSG_EVENTMESSAGE);
const UINT RMSG_GET_USERID = ::RegisterWindowMessage(_T("RMSG_GET_USERID"));	// User ID 얻기

#define UWM_SELECTEDCHART			( WM_USER + 0x2D46)
#define UWM_ZOOMVALUE				( WM_USER + 0x2D36)

//int gTestMode=1;

/////////////////////////////////////////////////////////////////////////////
// CDrChartCtl

CDrChartCtl::CDrChartCtl() : m_pIDebugMng(NULL)
{
	EnableAutomation();

	m_pChart = NULL;
	m_hCtrlWnd = NULL;

	m_pIChartDisp = NULL;
	m_pIRealReceiver = NULL;

	// 복수종목 맵차트 추가 - ojtaso(20090928)
	m_strCurrentRQ = "DEFAULT";

	m_lState = STATE_NONE;
	m_bUseValueToScale	= TRUE;		//@100121 FALSE가 기본. TRUE는 임시 -> TRUE를 기본으로.
	m_bReDrawCall		= FALSE;		//@100121 FALSE가 기본.
	m_bDisableRQSet		= TRUE;		//@100121 FALSE가 기본.TRUE는 임시 -> TRUE를 기본으로.

	m_pExtraChart = NULL;
	m_bUseExtraChart = FALSE;

	m_nViewCount = 1;
}

CDrChartCtl::~CDrChartCtl()
{
	DelAllBozoMsg();

	// 차트 삭제
	if (m_pChart)
	{
		delete m_pChart;
		m_pChart = NULL;
		m_pIChartDisp = NULL;
		
		if (m_Msg.m_pItems)
		{
			m_Msg.RemoveAll();
			delete m_Msg.m_pItems;
			m_Msg.m_pItems = NULL;
		}
	}


	m_hCtrlWnd = NULL;
}

void CDrChartCtl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CDrChartCtl, CWnd)
	//{{AFX_MSG_MAP(CDrChartCtl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_COPYDATA()
	ON_WM_SIZE()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)	
	// 복수종목 맵차트 추가 - ojtaso(20090928)
	ON_REGISTERED_MESSAGE( RMSG_CHARTMSG_EVENTMESSAGE,	OnEventMessage)
	ON_REGISTERED_MESSAGE( RMSG_GET_USERID,	OnGetUserID)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CDrChartCtl, CWnd)
	//{{AFX_DISPATCH_MAP(CDrChartCtl)
	DISP_FUNCTION(CDrChartCtl, "SetSubMsg", SetSubMsg, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "GetChartProperty", GetChartProperty, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetChartProperty", SetChartProperty, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "LoadAddIn", LoadAddIn, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetChartData", SetChartData, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetChartData2", SetChartData2, VT_I2, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetChartRealData", SetChartRealData, VT_EMPTY, VTS_BSTR VTS_R8 VTS_I2)
	DISP_FUNCTION(CDrChartCtl, "ShowBlock", ShowBlock, VT_BOOL, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CDrChartCtl, "GetDRChartDispatch", GetDRChartDispatch, VT_DISPATCH, VTS_NONE)
	DISP_FUNCTION(CDrChartCtl, "ChangeMajorFID", ChangeMajorFID, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "ChangeMinorFID", ChangeMinorFID, VT_I4, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "ChangeGridSymbol", ChangeGridSymbol, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "ChangeGridSymbol2", ChangeGridSymbol2, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SendTR", SendTR, VT_I4, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CDrChartCtl, "SendTrDifferReal", SendTrDifferReal, VT_I4, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CDrChartCtl, "AddChartItem", AddChartItem, VT_BOOL, VTS_I2 VTS_BSTR VTS_BSTR VTS_BSTR VTS_BOOL VTS_BOOL)
	DISP_FUNCTION(CDrChartCtl, "ChangeChartItem", ChangeChartItem, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "AddPacketRQ", AddPacketRQ, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "DelAllPacketRQ", DelAllPacketRQ, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDrChartCtl, "DelChartItem", DelChartItem, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "GetChartState", GetChartState, VT_I4, VTS_NONE)
	DISP_FUNCTION(CDrChartCtl, "ChangeGraphTitleRQ", ChangeGraphTitleRQ, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "ChangeGraphColorRQ", ChangeGraphColorRQ, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "ChangeGraphTitle", ChangeGraphTitle, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "ChangeGraphColor", ChangeGraphColor, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetCurrentRQ", SetCurrentRQ, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "AddBozoMsg", AddBozoMsg, VT_EMPTY, VTS_I2 VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "DelAllBozoMsg", DelAllBozoMsg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CDrChartCtl, "SetDiffGridSymbol", SetDiffGridSymbol, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetDiffGridSymbol2", SetDiffGridSymbol2, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetRowMaxCount", SetRowMaxCount, VT_EMPTY, VTS_BSTR VTS_I4)
	DISP_FUNCTION(CDrChartCtl, "SetValueToScale", SetValueToScale, VT_I4, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CDrChartCtl, "SetRedrawCall", SetRedrawCall, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CDrChartCtl, "SetDefaultRQ", SetDefaultRQ, VT_EMPTY, VTS_BOOL VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetLinePropRQ", SetLinePropRQ, VT_BOOL, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "GetLinePropRQ", GetLinePropRQ, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "UseExtraChart", UseExtraChart, VT_EMPTY, VTS_BSTR VTS_BOOL)
	DISP_FUNCTION(CDrChartCtl, "SetShowYScaleData", SetShowYScaleData, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetChartDataType", SetChartDataType, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "SetIndicatorHide", SetIndicatorHide, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "GetIndicatorHide", GetIndicatorHide, VT_BSTR, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CDrChartCtl, "SetNextKey", SetNextKey, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CDrChartCtl, "GetNextKey", GetNextKey, VT_BSTR, VTS_NONE)
//	DISP_FUNCTION(CDrChartCtl, "ChangePacket", ChangePacket, VT_EMPTY, VTS_BSTR, VTS_BSTR, VTS_BSTR, VTS_BSTR, VTS_BSTR, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDrChartCtl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {75A47147-9C93-4E1D-A7A1-25C6FCAE8EE9}
static const IID IID_IDrChartCtl =
{ 0x75a47147, 0x9c93, 0x4e1d, { 0xa7, 0xa1, 0x25, 0xc6, 0xfc, 0xae, 0x8e, 0xe9 } };

static const IID IDrChartCtlEvents =
{ 0x3490d5b1, 0x840b, 0x474a, { 0xaf, 0xbc, 0xbb, 0x25, 0x78, 0xa4, 0x73, 0x6a } };


BEGIN_INTERFACE_MAP(CDrChartCtl, CWnd)
	INTERFACE_PART(CDrChartCtl, IID_IDrChartCtl, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Event map
//const AFX_EVENTMAP CDrChartCtl::eventMap = { NULL, NULL };

//BEGIN_EVENT_MAP(CDrChartCtl, COleControl)
// 	//{{AFX_EVENT_MAP(CDrChartCtl)
// 	//}}AFX_EVENT_MAP
//END_EVENT_MAP()

// 	BEGIN_EVENTSINK_MAP(CDrChartCtl, CWnd)
// 	//{{AFX_EVENTSINK_MAP(CDrChartCtl)
// 	//}}AFX_EVENTSINK_MAP
// 	END_EVENTSINK_MAP()


/////////////////////////////////////////////////////////////////////////////
// CDrChartCtl message handlers
int CDrChartCtl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

 	_DoGetPartnerManger(TRUE);

	//	Get Debug Solution manager Handle.
	m_pIDebugMng = (IDebugManager*)AfxGetPctrInterface(UUID_IDebugManager);
	_DoAdviseToMonitor(TRUE);

	CString rRootDir;
	GetDir_DrfnRoot(rRootDir);

// 	{
// 		CString szTestMode;
// 		szTestMode.Format("%s\\Hanaro\\TestMode.ini", rRootDir);
// 		gTestMode = ::GetPrivateProfileInt("Mode", "test", 1, szTestMode);
// 	}

	//	Create Run Mode Chart control,

#ifdef __MULTI_LANG__
	//@Solomon-MultiLang:091117
	g_iMetaTable.m_strDataFolderPath.Format(_T("%s%s"), rRootDir, "\\Icss\\");
	g_iMetaTable.LoadMetaTable();
#endif


	m_pChart = new CDRChartOcx;

	int nX, nY;
	nX = m_ctlInfo.rect.Width();
	nY = m_ctlInfo.rect.Height();

	if (!m_pChart)
		return -1;

	BOOL bReturn = m_pChart->Create(NULL, WS_VISIBLE|WS_CHILD, CRect(0,0,nX,nY), this, ID_CHART);
	if (!bReturn)
	{
		delete m_pChart;
		m_pChart = NULL;

        AfxMessageBox("DrChart OCX is not found!");

		return -1;
	}

	m_hCtrlWnd = m_pChart->GetSafeHwnd();

	m_converter.SetParentWnd(this);
	
	// 4. Set Loaded Property
	LoadFile();

	//@복주종목 맵차트 추가 - ojtaso(20090928)
	//@복주종목 m_pChart->SetMultiItemMode(TRUE);

	if (m_converter.GetChartType() == 1)
	{
		m_pChart->SetNChartMode(3);
	}
	//<<20100309_JS.Kim		차트 모드 (HTS : 1, FX : 2, 해외선물 : 3)
	else if (m_converter.GetChartType() == 2)
	{
		m_pChart->SetNChartMode(2);
		//{{ JS.Kim_20100902 Solomon 맵차트에서 지표 삭제 가능토록 처리
		// FX맵차트에서 매도/매수 번갈아 보이는 기능을 맵에서 Hide/Show 처리하였기 때문에 삭제가 되면
		//	문제가 발생하여 삭제 못하도록 함.
		//m_pChart->SetBSelectTitle(FALSE);	// 차트 삭제 불가
		//}}
	}
	//>>

	// OCX에 경로 설정
	m_pChart->SetHtsPath(rRootDir);
	m_pChart->SetContainerDllInfo((long)GetSafeHwnd(), (short)ID_CHART);

	// Chart OCX Dispatch for Property and Method in Script 
	IUnknown *pUnk = m_pChart->GetControlUnknown();
	if (pUnk)
	{
		HRESULT hResult = pUnk->QueryInterface(IID_IDispatch, ( void **)&m_pIChartDisp);
		pUnk->Release();
		bReturn = SUCCEEDED(hResult) && m_pIChartDisp;
	}
	if( !bReturn)
	{
		delete m_pChart;
		m_pChart = NULL;
		m_pIChartDisp = NULL;
		
        AfxMessageBox( "DRChart OCX is not Created!");
		return -1;
	}

	//	실행모드
	m_pChart->SetRunTimeMode(1);

	m_pIRealReceiver = (IRealReceiver *)(m_pChart->GetAddInCustomInterface( "ChartCommonAddIn.dll:DIRECT_REAL")); // 차트 리얼 수신 준비

	// Get PacketOut Interface for AP Data
	if (m_pChart)
		m_pIPackOut = (CBaseCtrlPackOut *)m_pChart->GetBase("PACKOUT");

//	m_converter.SetParentWnd(this);

	// 4. Set Loaded Property
//	LoadFile();
	SetAllPropertiesToOCX();
//	m_pChart->ResetChart();
	m_pChart->ShowWindow(SW_SHOW);

	m_dataComm.SetCommWnd(this);
	m_dataComm.SetKey(m_ctlInfo.key);
	m_dataComm.SetChartName(m_ctlInfo.name);

	m_realMng.SetDataComm(&m_dataComm);

	if(m_bUseExtraChart && m_pExtraChart==NULL)
	{
		UseExtraChart("MultiBar", TRUE);
	}
//	InitFont();
//	m_pChart->SetFont(&m_Font, TRUE);

	return 0;
}

void CDrChartCtl::InitFont()
{
	CClientDC dc(this);
	LOGFONT logfont;

	int PointSize				= 9;
	logfont.lfHeight			= -MulDiv(PointSize, GetDeviceCaps(dc.GetSafeHdc(), LOGPIXELSY), 72);
	logfont.lfWidth				= 0;
	logfont.lfEscapement		= 0;
	logfont.lfOrientation		= 0;
	logfont.lfWeight			= FW_THIN;
	logfont.lfItalic			= FALSE;
	logfont.lfUnderline			= FALSE;
	logfont.lfStrikeOut			= FALSE;
	logfont.lfCharSet			= DEFAULT_CHARSET;
	logfont.lfOutPrecision		= OUT_CHARACTER_PRECIS;
	logfont.lfClipPrecision		= CLIP_CHARACTER_PRECIS;
	logfont.lfQuality			= DEFAULT_QUALITY;
	logfont.lfPitchAndFamily	= DEFAULT_PITCH | FF_ROMAN;
	strcpy(logfont.lfFaceName, "굴림체");

	if (m_Font.GetSafeHandle())
		m_Font.DeleteObject();

	m_Font.CreateFontIndirect(&logfont);
}

void CDrChartCtl::SetCtlInfo(_param* pInfo)
{
	m_ctlInfo.key = pInfo->key;
	m_ctlInfo.name = pInfo->name;
	m_ctlInfo.rect = pInfo->rect;
	m_ctlInfo.fonts = pInfo->fonts;
	m_ctlInfo.point = pInfo->point;
	m_ctlInfo.style = pInfo->style;
	m_ctlInfo.tRGB = pInfo->tRGB;
	m_ctlInfo.pRGB = pInfo->pRGB;

//	CString mapName = (LPCTSTR)pParent->SendMessage(WM_USER, MAKEWPARAM(mapDLL, 0));
	m_ctlInfo.options = pInfo->options;

	if(pInfo->options.CompareNoCase("/iSI028730_DRCHAR1")==0)
		m_bUseExtraChart = TRUE;
}

void CDrChartCtl::LoadFile()
{
	CString strPath, strName;
	strPath = m_ctlInfo.options;
	int nFind = strPath.Find(_T("/i"));
	strName = strPath.Right(strPath.GetLength()-nFind-2);

	GetDir_DrfnRoot(strPath);
	strPath += "\\gex\\" + strName + _T(".map");

	//	sashia	경로 설정 요망. ---------------------
	//strPath = _T("D:\\SVN\\Solomon\\ProjSrc\\WinRel\\gex\\") + strName + _T(".map");

//	strPath = m_strMapPath + m_strFileName + _T(".map");
	CFile file;
	if (!file.Open(strPath, CFile::typeBinary|CFile::modeRead))
	{
		CString strMsg;
		strMsg.Format("%s [%d]\n%s(%s)", __FILE__, __LINE__, _T("not find file"), strPath);
		AfxMessageBox(strMsg);

		return ;
	}
	g_DRDebuger.m_szMessage.Format("***MapFile***[%s]", strPath);
//	g_DRDebuger.DRLog_Message();

	CArchive ar(&file, CArchive::load);

	m_converter.Serialize(ar);
	m_converter.SelectQuery();

	ar.Close();
	file.Close();
}


BOOL CDrChartCtl::SetAllPropertiesToOCX()
{
	if (IsWindow(m_hCtrlWnd))
	{
		IPropertyMap* pProp = m_converter.GetPropMap();
//-->	sashia	-------TEST----------
//		const char *szKey=NULL, *szData=NULL;
//		POSITION pos = pProp->GetStartPosition();
//		while (pos)
//		{
//			pProp->GetNextAssoc(pos, szKey, szData);
//			//m_pIDebugMng->Trace('L', strlen(szKey) + strlen(szData) + 100, _T("%s = %s"), szKey, szData);
//		}
//<--	sashia	-------TEST----------

		return m_pChart->SetAllProperties3((long)pProp);
	}

	return FALSE;
}

#include "../../inc/IAUPartnerMng.h"
void CDrChartCtl::_DoGetPartnerManger(BOOL _bAdvise)
{
	if(_bAdvise)
	{
		m_pPtSvrMng = (IPartnerSvrManager*)AfxGetPctrInterface(UUID_IPartnerSvrManager);
		if (m_pPtSvrMng)
		{
			m_pPtMng = m_pPtSvrMng->AdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
		}
	}
	else
	{
		if (m_pPtSvrMng)
		{
			m_pPtSvrMng->UnAdviseManager(GetParent()->GetSafeHwnd(), m_hWnd);
			m_pPtSvrMng = NULL;
			m_pPtMng = NULL;
		}
	}
}

//axisfire.h에 정의된 내용들.
char *gWinixOnUser[] = { "",
					"DLL_INB",		//0x01		// char* Read() for DLL I/O controls
					"DLL_OUB",		//0x02		// void Write(pBytes, nBytes)
					"DLL_ALERT",	//0x03		// OnAlert(CString update, int stat)
					"DLL_TRIGGER",	//0x04		// OnTrigger(CString text, bool stat)
					"DLL_DOMINO",	//0x05		// SetDomino(CString text, bool stat)
					"DLL_NOTICE",	//0x06		// OnNotice(CString text)
					"DLL_DROP",		//0x07		// OnDrop(CString text)
					"DLL_AXIS",		//0x08		// OnAxis(key, string)
					"DLL_EXCEL",	//0x09		// make excel data
					"DLL_INVOKE",	//0x0a		// int OnInvoke(int wParam, int lParam)
					"DLL_SETPAL",	//0x0b		// void SetPalette()
					"DLL_SETFONT",	//0x0c		// void SetFont(int point, CString name)
					"DLL_SETFCB",	//0x0d		// void SetBlink(bool blink, int flash)
					"DLL_SELECT",	//0x0e		// OnSelect(CString maps)
					"DLL_SETFONT",	//0x0f		// void SetFont(int point, CString name)
					"DLL_ACCOUNT",	//0x10		// update account combo items
					"DLL_GUIDE",	//0x11		// BOOL SetGuide(CString guide)
					"DLL_INIT",		//0x12		// void OnInit()
					"DLL_MOUSE",	//0x13		// BOOL IsYourMouse(CPoint pt)
					"DLL_GETDATA",	//0x14		// char* GetData(CString name)
					"DLL_CODE",		//0x15		// void SetCode(CString data)
					"DLL_CLEAR",	//0x16		// void Clear(int type)
					""};


LONG CDrChartCtl::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	BYTE bt = LOBYTE(LOWORD(wParam));

	switch (bt)
	{
	case DLL_INB:
		SendTrChartGraphDifferReal("","","", "", TRUE); break;// KHD 

		if (!m_converter.m_bDirectSend && (m_converter.m_nTRType == 0))
		{
			if (m_converter.m_arrFidQuery.GetSize() <= 0)
				break;

			CFidBlock *pBlock = m_converter.GetDefaultFIDBlock(atoi(m_converter.m_strMajorFID));
			CFidQuery qry = m_converter.m_arrFidQuery.GetAt(0);
			CString strData = m_dataComm.GetFidInboundData(qry, pBlock, 0);
			return (long)(LPCTSTR)strData;
		}
		break;

	case DLL_OUBx:
		{
#if(0)//tour2k
			char* pBytes = (char *)lParam;
			struct _extTH* extTH = (struct _extTH *)pBytes;
			pBytes += L_extTH;

			int nKey = extTH->key;
			int nLen = extTH->size;

			wParam = MAKEWPARAM(MAKEWORD(DLL_OUB, nKey), nLen);
			lParam = (LPARAM)pBytes;
#else
		_extTHx* info = (_extTHx*)lParam;

		const long nSize = info->size;
		char* strSrc = info->data;
		struct mod* Mod = (struct mod*)info->data;

		if (m_converter.m_bDirectSend)
		{
			m_lState = STATE_STARTRECEIVE;
			SendOnChange();
		}

		CString strKey = CString(Mod->usr1, sizeof(Mod->usr1));
		ReceiveData(strSrc, nSize, atoi(strKey));//m_ctlInfo.key);// pTH->key);

		if (m_converter.m_bDirectSend)
		{
			m_lState = STATE_ENDRECEIVE;
			SendOnChange();
		}
		if (m_bReDrawCall)
		{
			//@100115-ojt ReCalculateAllGraphs 매물대 다시 그리도록하기 위한 루틴.
			m_pChart->ReCalculateAllGraphs();
		}
#endif
		}

	case DLL_OUB:			// Tr 수신 및 FID 데이터 수신.
		{
#if(0)//tour2k block
			int nKind = (HIBYTE(LOWORD(wParam)));
	
			struct mod* Mod = (struct mod*)lParam;
			char* strSrc = (char*)lParam;			
			long nSize = HIWORD(wParam);
			
			if (m_converter.m_bDirectSend)
			{
				m_lState = STATE_STARTRECEIVE;
				SendOnChange();
			}

			CString strKey = CString( Mod->usr1, sizeof( Mod->usr1 ) );
			ReceiveData( (char*)(lParam), nSize, atoi(strKey) );//m_ctlInfo.key);// pTH->key);

			if (m_converter.m_bDirectSend)
			{
				m_lState = STATE_ENDRECEIVE;
				SendOnChange();				
			}
			if(m_bReDrawCall)
			{
				//@100115-ojt ReCalculateAllGraphs 매물대 다시 그리도록하기 위한 루틴.
				m_pChart->ReCalculateAllGraphs();
			}
#endif
		}
		break;
	//-->100309 리얼처리방식의 변경.
	//DLL_ALERT 대신에 DLL_ALERTx 사용하는 것으로 루틴 변경함.
// 	case DLL_ALERT:			// 리얼 수신
// 		if(gTestMode==0)
// 		{
// 			LPTSTR pData = (LPTSTR)lParam;
// 			int nSize = strlen(pData);
// 
// 			DoRealData(wParam, lParam);
// 		}
// 		break;
	case DLL_ALERTx:
//		if(gTestMode==1)
		{
			DoRealData_NewReal(wParam, lParam);
		}
		break;
	//<--100309 리얼처리방식의 변경.

	case DLL_CODE:
		break;

// 	case DLL_DOMINO:		// 화면 열리면서 데이터 공유 받을 때
// 		break;
	}

	return ret;
}

//#define SAMPLE_DATA_SET
void CDrChartCtl::ReceiveData(const char* pszData, int nSize, int nKey)
{
	CString strTestBozo;

	CChartProp info, prop;
	if (m_converter.m_bDirectSend)
	{
		if (!m_dataComm.GetRcvTr(nKey, info))
			return;

		m_converter.m_prop = info;
		m_converter.m_strCurCode = info.GetCode();
		m_converter.SelectQuery(info.GetTRCode());
		m_chartMng.SetChartProp(info.GetKey(), m_converter.m_prop);
		info = m_converter.m_prop;
	}

	if (nSize <= 0)
		return;

	m_converter.ParsingDataToArr(pszData, nSize);

	CString strBozoMsg;
	int nBozoMessageSize = m_converter.GetBozoMsgFromTr(strBozoMsg);

	if (nBozoMessageSize<0)
	{
		strBozoMsg = _T("0000");
		nBozoMessageSize = strBozoMsg.GetLength();
	}

	if (m_bozoItemList.GetCount() > 0)
	{
		if (m_converter.m_pBozoMng != NULL)
			nBozoMessageSize = m_converter.m_pBozoMng->ChangeChartBozoMsg(strBozoMsg, &m_bozoItemList);
	}

	if( strBozoMsg.GetLength() == 0 )
	{
		m_Msg.GenerateSubMsg();		
		if( m_Msg.m_strSubMsg.GetLength() > 0 )
			strBozoMsg.Format("%04d%s", m_Msg.m_strSubMsg.GetLength(), m_Msg.m_strSubMsg);	
	}	

	CString strValue;
	int i, nCnt = m_arrAddPacket.GetSize();
	for (i=0; i<nCnt; i++)
		strValue += m_arrAddPacket.GetAt(i) + _T("\r\n");

	nBozoMessageSize = ReSetPacket(strValue, strBozoMsg);
	if (nBozoMessageSize < 0)
		return;

	int nPackSize = m_converter.GetFidOutRowCnt() * m_converter.GetRowPacketSize();

	char *pszChartData = NULL;
	int nDataSize;
	nDataSize = nPackSize + nBozoMessageSize + 5;

	pszChartData = new char[nDataSize+1];
	memset(pszChartData, 0x20, nDataSize);
	pszChartData[nDataSize] = 0x00;

	CString strData;
	strData.Format(_T("%05d"), m_converter.m_nRowCount);
	memcpy(pszChartData, (LPTSTR)(LPCTSTR)strData, 5);

	memcpy(pszChartData+5, (LPTSTR)(LPCTSTR)strBozoMsg, nBozoMessageSize);

	if (m_converter.m_bDirectSend)
		m_converter.ConvReceiveData((char*)(pszChartData+nBozoMessageSize+5), &info);
	else
		m_converter.ConvReceiveData((char*)(pszChartData+nBozoMessageSize+5), NULL);

	CString strPacketName;
	if (info.GetRQ().GetLength() > 0)
		strPacketName = info.GetPackName();
	else
		strPacketName = m_converter.GetPacketNameList();

	ADT_IF(m_pIDebugMng)
		m_pIDebugMng->TraceTran('O', "DrChartB", nDataSize, (LPBYTE)pszChartData);
	ADT_IFEND()

// 	if(TRUE) // 데이타 덤프
// 	{
// 			static int nCnt = 0;
// 			nCnt++;
// 			
// 			SYSTEMTIME st;
// 			GetLocalTime(&st);
// 			
// 			CString strFileName;
// 			strFileName.Format(_T("%sDump_IBK%.2d%.2d%.2d%.3d_%d"), _T("C:\\Temp\\"), st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, nCnt);
// 			
// 			HANDLE hFile;
// 			DWORD dwWritten;
// 			//AfxMessageBox(strFileName);
// 			hFile = CreateFile( ( LPCTSTR )strFileName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
// 			SetFilePointer( hFile, 0, 0, FILE_END );
// 			WriteFile( hFile, pszChartData, nDataSize, &dwWritten, NULL );
// 			CloseHandle( hFile );
// 	}


// #ifdef SAMPLE_DATA_SET
// 	strData = "000470496UM=E@UD=1@RDATET=YYYYMMDD@TDC=0.00001:시가,|0.00001:고가,|0.00001:저가,|";
// 	strData += "0.00001:종가,|1:기본거래량,@RDC=0.00001:-1:시가,|0.00001:-1:고가,|0.00001:-1:저가,|";
// 	strData += "0.00001:-1:종가,|1:기본거래량,@REALTIMET=YYYYMMDDHHMMSS@TIMEDIFF=-6@PREVPRICE=종가:0.141030|";
// 	strData += "기본거래량:     31583@MARKETTIME=1700|1600@DATATIME=20100122013311@";
// 	strData += "CODEANDNAME=6EH10:Euro FX (2010.3)@MARKETTYPE=1@MARKETDAY=20100122@RESETUSEREAL=1@";
// 	strData += "HBPRICE=0.00010@                                                                                    ";
// 	strData += "20091116     14951     15007     14876     14976      185620091117     14969     14975     14800     14845      100320091118     14875     14981     14860     14929      106520091119     14950     14950     14834     14907      170220091120     14915     14920     14788     14843      124220091123     14889     14984     14885     14961      105020091124     14953     14978     14880     14965      211720091125     14990     15135     14990     15128      247820091126     15030     15128     14950     15128       87620091127     15000     15000     14820     14949      4347";
// 	strData += "20091130     14949     14984     14949     14984         020091201     14996     15110     14990     15088      356820091202     15070     15102     15026     15029      438220091203     15114     15137     15039     15084     1799320091207     15053     14896     14750     14815     2544920091208     14822     14860     14675     14677     5029420091209     14700     14777     14662     14711     9633520091210     14731     14755     14680     14714    14260820091211     14726     14772     14581     14614    26753620091214     14611     14682     14599     14643    194420";
// 	strData += "20091215     14648     14655     14497     14523    23977420091216     14536     14588     14501     14512    23784020091217     14528     14531     14302     14347    30096820091218     14346     14409     14258     14319    24509420091221     14309     14370     14277     14287     8007220091222     14287     14330     14215     14252    17588720091223     14251     14365     14232     14334    13914320091224     14330     14416     14327     14353     5698220091228     14372     14411     14349     14380     5912420091229     14368     14457     14330     14350    150842";
// 	strData += "20091230     14346     14360     14271     14333    13309620091231     14339     14440     14304     14334    10149920100104     14305     14455     14267     14410    22085020100105     14407     14483     14344     14367    21729020100106     14362     14433     14281     14408    26286820100107     14400     14446     14296     14325    24228020100108     14306     14438     14261     14415    34620720100111     14410     14555     14409     14518    21093120100112     14512     14544     14452     14495    22556720100113     14492     14577     14455     14511    270563";
// 	strData += "20100114     14500     14555     14443     14501    24529520100115     14497     14511     14334     14355    25105320100118     14393     14397     14332     14355     6533020100119     14389     14413     14249     14290    33866920100120     14288     14288     14078     14105    33405920100121     14107     14142     14027     14103    36596420100122     14082     14146     14064     14144     31583";
// 
// 	nDataSize = strData.GetLength();
// 
// 	delete []pszChartData;
// 
// 	pszChartData = new char[nDataSize+1];
// 	memset(pszChartData, 0x20, nDataSize);
// 	pszChartData[nDataSize] = 0x00;
// 
// 	memcpy(pszChartData, (LPTSTR)(LPCTSTR)strData, nDataSize);
// #endif

	if (m_converter.m_bDirectSend)
	{
		CString strRQ = info.GetRQ();
		if (strRQ.Compare(_T("DEFAULT")) == 0)
		{
			//m_pChart->SetPacketData((LPCTSTR)strPacketName, (long)pszChartData, nDataSize, TRUE);
			m_pChart->SetChartData((long)pszChartData, nDataSize, 1);
			if (info.m_bReal)
			{
				CString strTRName = info.GetTRCode();
				CString strCode = info.GetCode();
				CString strRealSym = info.GetDiffRealSym();
				int nKey = m_realMng.AdviceRealKey(strCode);
			}
	//		SetAdvRealData(m_converter.m_nTRType, info);
		}
		else
		{
			if(m_bDisableRQSet)
			{
				CString szTmpPacket;
				//UD 1(일),2(주),3(월),4(분),5(틱),6(초)
				//카운트(5자리)+보조메시지길이(4)+보조메시지+데이터
				szTmpPacket.Format("000000010UD=5@UT=1@");
				int nTmpSize = szTmpPacket.GetLength();
				m_pChart->SetPacketDataRQ("DEFAULT", (LPCTSTR)strPacketName, (long)(LPCSTR)szTmpPacket, nTmpSize, TRUE);
			}
			m_pChart->SetPacketDataRQ(strRQ, (LPCTSTR)strPacketName, (long)pszChartData, nDataSize, TRUE);

			if (info.m_bReal)
			{
				CString strTRName = info.GetTRCode();
				CString strCode = info.GetCode();
				CString strRealSym = info.GetDiffRealSym();
				int nIndex, nRet, nKey = m_realMng.AdviceRealKey(strCode);
				if (m_converter.m_nTRType)
				{
					CTrQuery trQry;
					nIndex = m_converter.GetTrQuery(strTRName, trQry);
					if (nIndex >= 0)
						nRet = m_realMng.SendRealData(nKey, strCode, trQry, strRealSym);
				}
				else
				{
					CFidQuery fidQry;
					nIndex = m_converter.GetFidQuery(strTRName, fidQry);
					SetCodeData(strCode, fidQry);
					if (nIndex >= 0)
						nRet = m_realMng.SendRealData(nKey, fidQry, strRealSym);
				}
			}
		}
	}
	else
	{
		if(m_pExtraChart)
		{
			CMultiBar* pExtraChart = (CMultiBar*)m_pExtraChart;
			pExtraChart->SetPacketData((LPBYTE)pszChartData, nDataSize);

		}
		else
		{
			m_pChart->SetPacketData((LPCTSTR)strPacketName, (long)pszChartData, nDataSize, TRUE);
		}
	}

	delete []pszChartData;
	pszChartData = NULL;
}

void CDrChartCtl::OnDestroy()
{
	m_lState = STATE_DESTROYCHART;
	SendOnChange();

	if (m_Font.GetSafeHandle())
		m_Font.DeleteObject();

	if (m_pChart && m_pChart->GetSafeHwnd())
		m_pChart->DestroyWindow();

	if(m_pExtraChart)
	{
		if(m_pExtraChart->GetSafeHwnd())
		{
			CMultiBar* pExtraChart = (CMultiBar*)m_pExtraChart;
			pExtraChart->DestroyWindow();
		}
		delete m_pExtraChart;
		m_pExtraChart=NULL;
	}
	
	if(m_pIRealReceiver)
	{
		m_pIRealReceiver->Release();
		m_pIRealReceiver = NULL;
	}

	_DoGetPartnerManger(FALSE);
	_DoAdviseToMonitor(FALSE);

	CWnd::OnDestroy();
}

BOOL CDrChartCtl::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
	if (!pCopyDataStruct) return FALSE;
	
	DWORD lBufLen	=	pCopyDataStruct->cbData;		// 전체 길이
	if ( lBufLen <=  0 )
		return 0L;

// 	//0287화면 테스트용.
// 	{
// 		int nOff=32;
// 		CString strPacketName = "투자자매도;매도;매수;";
// 		char* pSendData = (char*)pCopyDataStruct->lpData;
// 		pSendData = &pSendData[nOff];
// 		m_pChart->SetPacketData((LPCTSTR)strPacketName, (long)pSendData, lBufLen-nOff, TRUE);
// 	}
// 	if (pCopyDataStruct->dwData>=g_TestOnCopyStart && pCopyDataStruct->dwData<=15)
// 	{
// 		int nOff=113;
// 		CString strPacketName = m_converter.GetPacketNameList();
// 		char* pSendData = (char*)pCopyDataStruct->lpData;
// 		pSendData = &pSendData[nOff];
// 		m_pChart->SetPacketDataRQ(m_strCurrentRQ, (LPCTSTR)strPacketName, (long)pSendData, lBufLen-nOff, TRUE);
// 	}
// 	else	//Real Data
// 	{
// 		int nOff=32;
// 		char* pSendData = (char*)pCopyDataStruct->lpData;
// 		pSendData = &pSendData[nOff];
// 
// 		CString szFullText;
// 		szFullText.Format("%*.*s", lBufLen-nOff, lBufLen-nOff, pSendData);
// 		CString strCode, strTipText[2];
// 		AfxExtractSubString(strCode		 ,	szFullText, 0, ',');	//종목코드
// 		AfxExtractSubString(strTipText[0], szFullText, 1, ',');		//체결시간
// 		AfxExtractSubString(strTipText[1], szFullText, 3, ',');		//종가(현재가)
// 
// 		double dValue[2];
// 		dValue[0] = atof(strTipText[0]);
// 		dValue[1] = atof(strTipText[1]);
// 
// 		// 복수종목 맵차트 추가 - ojtaso(20090928)
// 		m_pChart->SetRealDataCode(strCode, "자료일자", dValue[0], 0);
// 		m_pChart->SetRealDataCode(strCode, "종가", dValue[1], 0);
// 
// 		if (m_pIRealReceiver)
// 			m_pIRealReceiver->SetRealPacketEndRQ(strRQ);
// 	}
	return CWnd::OnCopyData(pWnd, pCopyDataStruct);
}

//////////////////////////////////////////////////////////////////////////
// OCX에 위치 정보를 주기 위해서는
// 포인트 정보를 long의 상/하위 값으로 매핑해서 넘겨쥐야 한다.
// 그래서 MAKELONG(point.x, point.y)
void CDrChartCtl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_pChart->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELONG(point.x, point.y));
}

void CDrChartCtl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_pChart->SendMessage(WM_LBUTTONUP,nFlags,MAKELONG(point.x, point.y));
}

void CDrChartCtl::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_pChart->SendMessage(WM_RBUTTONUP,nFlags,MAKELONG(point.x, point.y));
}

void CDrChartCtl::OnMouseMove(UINT nFlags, CPoint point) 
{
	m_pChart->SendMessage(WM_MOUSEMOVE,nFlags,MAKELONG(point.x, point.y));
}

void CDrChartCtl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if(m_pExtraChart)
	{
		if (m_pChart->GetSafeHwnd()) m_pChart->ShowWindow(SW_HIDE);
		if (m_pExtraChart->GetSafeHwnd()) m_pExtraChart->MoveWindow(CRect(0,0,cx-1,cy-1));
	}
	else
	{
		if (m_pChart->GetSafeHwnd()) m_pChart->MoveWindow(CRect(0,0,cx,cy));
	}
}
///////////////////////////////////////////////////////////////////////////


void CDrChartCtl::SetSubMsg(LPCTSTR strSubMsg, LPCTSTR strParam) 
{
	m_Msg.SetSubMsg(strSubMsg, strParam);
}

BSTR CDrChartCtl::GetChartProperty(LPCTSTR szPropName) 
{
	CString strResult;
	char chResult[NMRETURN_MAXLENGTH+1];
	memset(chResult, 0, NMRETURN_MAXLENGTH+1);
	
	//	strResult = _T("Test");
	// 	return strResult.AllocSysString();
	// 1. Clear Property Value
	if( !szPropName || !*szPropName) return FALSE;
	
	//return strResult.AllocSysString();
	
	// 1.5 Trim Property Info
	CString strPropertyName;
	int nPropertyLen = strlen( szPropName);
	char *szProperty = strPropertyName.GetBufferSetLength( nPropertyLen);
	strcpy( szProperty, szPropName);
	TrimLR( szProperty, nPropertyLen);
	if( !*szProperty)
	{
		strResult = _T("");
		return strResult.AllocSysString();
	}
	
	// 2. Process Chart OCX Method
	// 2.1 Get Method Name (It is a Function Name in latest Chart OCX Wrapper Class.)
	char *szParams = strchr( szProperty, '(');
	if( szParams)
	{
		// 2.1.0 Trim Right Property Name
		char *szRTrim = szParams - 1;
		while( *szRTrim&& _istspace( *szRTrim)) szRTrim--;	// Cuation! Do not use isspace(), Use _istspace() for MBCS
		szRTrim[ 1] = '\0';	// Trim Right
		
		// 2.1.1 Get Method Name
		int nFunctionNameLen = szParams - szProperty;
		if( 255 < nFunctionNameLen)
		{
			CString strMsg;
			strMsg.Format( "Too long Property Name.\n%s", szPropName);
			AfxMessageBox( strMsg);
			strResult = _T("");
			return strResult.AllocSysString();
		}
		*szParams++ = '\0';
		
		// 2.1.2 Get Parameters
		// 2.1.2.1 Remove Right Parenthesis
		char *pRParenthesis = strrchr( szParams, ')');
		if( !pRParenthesis)
		{
			CString strMsg;
			strMsg.Format( "Right Parenthesis is not found!\n%s", szPropName);
			AfxMessageBox( strMsg);
			strResult = _T("");
			return strResult.AllocSysString();
		}
		*pRParenthesis = '\0';
		
		// 2.2 Run Method Process
		if(Chart_Method( szProperty, szParams, chResult))
		{
			CString strTemp;
			strTemp.Format("%s", strResult);
			strResult = _T(chResult);
			return strResult.AllocSysString();
		}
	}
	
	// 3. Process Chart OCX Property
	DISPID 	dispid = 0L;
	HRESULT hResult = GetIDsOfNames( szProperty, dispid);
	if( SUCCEEDED( hResult))
	{
		if(Chart_PropertyGet( szProperty, dispid, chResult))
		{
			//	CString strTemp;
			//	strTemp.Format("%s", strResult);
			strResult = _T(chResult);
			return strResult.AllocSysString();
		}
	}
	// 4. Process Wrapper's Property (Default Type)
	//		strVisible = DRChart1.Visible
	//			p_szPropName : "Visible"
	//		but, There is no Property supported like this. (2005/07/29)
	CString strMsg;
	strMsg.Format( "The Property [%s] is not supported.\nPlease, Check again!", szProperty);
	AfxMessageBox( strMsg);
	
	strResult = _T("");
	return strResult.AllocSysString();
}

BOOL CDrChartCtl::SetChartProperty(LPCTSTR p_szPropName, LPCTSTR p_szValue, LPCTSTR p_szValueType) 
{
	// 1. Clear Property Value
	if( !p_szPropName || !*p_szPropName || !p_szValue || !*p_szValue || !p_szValueType || !*p_szValueType)
		return FALSE;
	
//	if (strName == "ChangeChartItem")
//		m_pChart->AddChartItem(p_szValue, p_szValueType, FALSE);

	// 2. Trim Property Info
	// 2.1 Trim Property Name
	CString strPropertyName;
	int nPropertyNameLen = strlen( p_szPropName);
	char *szPropertyName = strPropertyName.GetBufferSetLength( nPropertyNameLen);
	strcpy( szPropertyName, p_szPropName);
	TrimLR( szPropertyName, nPropertyNameLen);
	if( !*szPropertyName) return FALSE;
	// 2.2 Trim Property Value
	CString strPropertyValue;
	int nPropertyValueLen = strlen( p_szValue);
	char *szPropertyValue = strPropertyValue.GetBufferSetLength( nPropertyValueLen);
	strcpy( szPropertyValue, p_szValue);
	TrimLR( szPropertyValue, nPropertyValueLen);
	if( !*szPropertyValue) return FALSE;
	// 2.3 Trim Property Value Type
	CString strPropertyValueType;
	int nPropertyValueTypeLen = strlen( p_szValueType);
	char *szPropertyValueType = strPropertyValueType.GetBufferSetLength( nPropertyValueTypeLen);
	strcpy( szPropertyValueType, p_szValueType);
	TrimLR( szPropertyValueType, nPropertyValueTypeLen);
	if( !*szPropertyValueType) return FALSE;
	
	// 3. Process Chart OCX Property
	DISPID 	dispid = 0L;
	HRESULT hResult = GetIDsOfNames( szPropertyName, dispid);
	if( SUCCEEDED( hResult)) return Chart_PropertySet( szPropertyName, dispid, szPropertyValue, szPropertyValueType);
	
	// 4. Process Wrapper's Property (Default Type)
	//		strVisible = DRChart1.Visible
	//			p_szPropName : "Visible"
	//		but, There is no Property supported like this. (2005/07/29)
	CString strMsg;
	strMsg.Format( "The Property [%s] is not supported.\nPlease, Check again!", szPropertyName);
	AfxMessageBox( strMsg);
	
	return FALSE;
}

BOOL CDrChartCtl::LoadAddIn(LPCTSTR sz_AddInDllName) 
{
	if(m_pChart->LoadAddIn(sz_AddInDllName))
	{
		// 		if(m_pChart->InvokeAddIn(sz_AddInDllName, 0, NULL))
		// 			return  TRUE;
		// 		AfxMessageBox("Invoke addin falied");
		return FALSE;
	}
	CString strMsg = sz_AddInDllName;
	AfxMessageBox("Loading failed " + strMsg);
	return FALSE;
}

short CDrChartCtl::SetChartData(LPCTSTR msg) 
{
	// msg에 보조메시지에 UM=E@ 만  있을 경우 SetSubMsg로 셋팅한 보조메시지가 셋팅안되는 문제 해결
	CString strHelpMsg, strMsg = msg;
	int nPos = strMsg.Find("UM=E@");
	if(nPos == -1 || strMsg.GetLength() < 14) return 0;
	
	m_Msg.GenerateSubMsg();		// 보조메시지 생성
	
	strHelpMsg.Format("%04d%s", m_Msg.m_strSubMsg.GetLength(), m_Msg.m_strSubMsg);
	
	CString strSendMsg = strMsg.Left(5) + strHelpMsg + strMsg.Mid(14);
	
	int nLength = strSendMsg.GetLength();
	
	return m_pIPackOut->SetData2(strSendMsg.GetBuffer(nLength), nLength);
}

short CDrChartCtl::SetChartData2(LPCTSTR szMsg) 
{
	CString strMsg = szMsg;
	int nDataCount = 0;
	
	m_Msg.m_strData = _T("");
	
	m_Msg.GenerateSubMsg();		// 보조메시지 생성
	
	if(strMsg.Right(1) != ",") strMsg += ",";
	
	int iStart = 0;
	
	CString strTmp, strFormat, strTempBuf;
	
	while (iStart >= 0)
	{
		POSITION pos = m_Msg.m_pItems->GetHeadPosition();
		while (pos)
		{
			strTempBuf = TokenizeEx(strMsg, ",", iStart);
			if(iStart == -1)	// No data
				break;
			strFormat.Format("%%%ds", ((CItemInChartEu*)m_Msg.m_pItems->GetNext(pos))->m_lLength);
			strTmp.Format(strFormat, strTempBuf);
			m_Msg.AddData(strTmp);			
		}		
		nDataCount++;
	}
	
	m_Msg.SetCount(--nDataCount);
	m_Msg.GenerateMsg();	// ocx에 전달할 메시지 생성
	
	long lLength;
	lLength = m_Msg.m_strMsg.GetLength();
	return m_pIPackOut->SetData2( m_Msg.m_strMsg.GetBuffer(lLength), lLength);
}


void CDrChartCtl::SetChartRealData(LPCTSTR szPacketName, double dData, short nOption=0) 
{
	m_pChart->SetRealData(szPacketName, dData, nOption);
}

#define UMSG_ADDCHARTBLOCK				WM_USER+17	// 해당그래프를 추가한다.
#define UMSG_HASGRAPH					WM_USER+16	// 해당그래프가 있는지 체크한다.
#define UMSG_DEL_CHART_BLOCK			WM_USER+197 // 차트 블럭 삭제.

BOOL CDrChartCtl::ShowBlock(LPCTSTR szPacketName, short nOption)
{
	UINT uMsg;
	if(nOption==0)		uMsg = UMSG_DEL_CHART_BLOCK;
	else if(nOption==2)	uMsg = UMSG_HASGRAPH;
	else				uMsg = UMSG_ADDCHARTBLOCK;
	
	BOOL bExistance = m_pChart->SendMessage(uMsg,(WPARAM)szPacketName);
	return bExistance;
}


LPDISPATCH CDrChartCtl::GetDRChartDispatch() 
{
	return m_pIChartDisp;
}

long CDrChartCtl::ChangeMajorFID(LPCTSTR pstrMajor)
{
	m_converter.m_strMajorFID = pstrMajor;

	CString strSymbol;
	strSymbol = m_converter.m_strMajorFID + m_converter.m_strMinorFID;
	m_converter.SetGridSymbol2(strSymbol, _T("FID Query"));

//	m_converter.GetFIDLayout();

	return 0;
}

long CDrChartCtl::ChangeMinorFID(LPCTSTR pstrMinor)
{
	m_converter.m_strMinorFID = pstrMinor;

	CString strSymbol;
	strSymbol = m_converter.m_strMajorFID + m_converter.m_strMinorFID;
	m_converter.SetGridSymbol2(strSymbol, _T("FID Query"));

//	m_converter.GetFIDLayout();

	return 0;
}

void CDrChartCtl::SetDiffGridSymbol(LPCTSTR pstrSymbol) 
{
	CString strSymbol;
	strSymbol = pstrSymbol;
	m_converter.SetGridSymbol(strSymbol);
}

void CDrChartCtl::SetDiffGridSymbol2(LPCTSTR pstrFIDCode, LPCTSTR pstrSymbol)
{
	CString strSymbol, strCode;
	strSymbol = pstrSymbol;
	strCode = pstrFIDCode;

	m_converter.SetDiffGridSymbol(strCode, strSymbol);
}

void CDrChartCtl::SetRowMaxCount(LPCTSTR pstrFIDCode, long lMaxCount)
{
	CString strCode(pstrFIDCode);

	m_converter.SetRowMaxCount(strCode, lMaxCount);
}



void CDrChartCtl::ChangeGridSymbol(LPCTSTR pstrSymbol) 
{
	CString strSymbol;
	strSymbol = pstrSymbol;
	m_converter.SetGridSymbol2(strSymbol, _T("FID Query"));

//	m_converter.GetFIDLayout();
}

void CDrChartCtl::ChangeGridSymbol2(LPCTSTR pstrSymbol, LPCTSTR pstrFIDCode)
{
	CString strSymbol, strCode;
	strSymbol = pstrSymbol;
	strCode = pstrFIDCode;

	m_converter.SetGridSymbol2(strSymbol, strCode);
}

void CDrChartCtl::DelChartItem(LPCTSTR pstrCode)
{
	CString strKey(pstrCode), strRQ, strCode;

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		strCode = info.GetCode();

		CChartPropArr arr;
		m_chartMng.GetChartPropArr(strCode, arr);
		if (arr.GetSize() <= 1)
			m_realMng.UnAdviceRealKey(strCode);

		m_chartMng.RemoveRQ(info.GetRQ());
		
		m_pChart->RemoveGraphRQ(info.GetRQ(), info.GetTitle());

		m_strCurrentRQ = "DEFAULT";
	}
}

long CDrChartCtl::GetChartState()
{
	return m_lState;
}
/*
void CDrChartCtl::ChangePacket(LPCTSTR pstrPreName, LPCTSTR pstrName, LPCTSTR pstrFidName, LPCTSTR pstrType, LPCTSTR pstrSize, LPCTSTR pstrReal)
{
	m_pChart->RemovePacket(pstrPreName);
	m_pChart->RemovePacket(pstrName);

	CString strPacket;
	CString strReal;
	if (pstrReal == "1")	strReal = "유;\r\n";
	else					strReal = "무;\r\n";

	int nSize = atoi(pstrSize);
	strPacket.Format(_T("1;%s;%s;%d;%s;%s"), pstrName, pstrFidName, nSize, pstrType, strReal);

	CChartProp info;
	info = m_converter.m_prop;
	info.SetRQ(m_strCurrentRQ);
	info.SetTitle(pstrName);

	ST_CHART_PACKET stPack;
	stPack.bReal = (pstrReal == "1")?TRUE:FALSE;
	stPack.nPacketSize = nSize;
	stPack.szFID = pstrFidName;
	stPack.szPacketName = pstrName;
	stPack.szRealFID = "";
	stPack.szTextType = pstrType;

	info.AddPacket(stPack);
	m_converter.m_prop = info;

	m_pChart->AddPacket(strPacket);
}
*/
BOOL CDrChartCtl::ChangeGraphTitleRQ(LPCTSTR pstrCode, LPCTSTR pstrTitle)
{
	CString strKey(pstrCode), strTitle(pstrTitle), strOldTitle, strCode;

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		CString strRQ = info.GetRQ();
		if (!strRQ.IsEmpty())
		{
			strOldTitle = info.GetTitle();
			if (strOldTitle.GetLength() <= 0)
				m_pChart->GetGraphPropertyAtOnce(strRQ, "");

			m_pChart->ChangeGraphPropertyAtOnce(strRQ, info.GetTitle(), strTitle, -1, -1, -1, NULL, -1, -1, -1);
			m_pChart->Invalidate();
			info.SetTitle(strTitle);
			m_chartMng.SetChartProp(strKey, info);
		}
	}

	return TRUE;
}

BOOL CDrChartCtl::ChangeGraphColorRQ(LPCTSTR pstrCode, LPCTSTR pstrColor)
{
	CString strKey(pstrCode), strCode, strColor(pstrColor);

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		CString strRQ = info.GetRQ();
		if (!strRQ.IsEmpty())
		{
			CString strInColor;
			strInColor = strColor + ";" + strColor + ";" + strColor + ";" + strColor + ";" + strColor + ";" + strColor + ";";
			strInColor += "0;1;";
			strInColor += strColor + ";" + strColor + ";" + strColor + ";" + strColor + ";";
			strInColor += "0;1";

			m_pChart->ChangeGraphPropertyAtOnce(strRQ, info.GetTitle(), info.GetTitle(), -1, -1, -1, strInColor, -1, -1, -1);
			m_pChart->Invalidate();
		}
	}

	return TRUE;
}

BOOL CDrChartCtl::ChangeGraphTitle(LPCTSTR pstrName, LPCTSTR pstrTitle)
{
	CString strName(pstrName), strTitle(pstrTitle);
	m_pChart->ChangeGraphTitle2(strName, strTitle);
	m_pChart->Invalidate();

	return TRUE;
}

BOOL CDrChartCtl::ChangeGraphColor(LPCTSTR pstrName, LPCTSTR pstrColor)
{
	CString strName(pstrName), strColor(pstrColor);
	m_pChart->ChangeGraphColor(strName, strColor);
	m_pChart->Invalidate();

	return TRUE;
}

void CDrChartCtl::SetCurrentRQ(LPCTSTR pstrCode)
{
	CString strKey(pstrCode);

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		CString strRQ = info.GetRQ();
		if (!strRQ.IsEmpty())
		{
			m_pChart->SetCurrentRQ(strRQ);
			m_strCurrentRQ = strRQ;
		}
	}
}

void CDrChartCtl::AddBozoMsg(short nType, LPCTSTR pstrKey, LPCTSTR pstrValue)
{
	CString strKey(pstrKey), strValue(pstrValue);

//>>[로그확인루틴 지우지마세요.
// 	{
// 		CString szMessage;
// 		szMessage.Format("CDrChartCtl::AddBozoMsg nType(%d) pstrKey(%s) pstrValue(%s)", nType, pstrKey, pstrValue);
// 		g_DRDebuger.DRLog_Message(szMessage);
// 	}
//<<

	int nBozoType = CChartBozoItem::cbi_add;

	if (nType == 0)			nBozoType = CChartBozoItem::cbi_add;
	else if (nType == 1)	nBozoType = CChartBozoItem::cbi_chanage;
	else if (nType == 2)	nBozoType = CChartBozoItem::cbi_delete;

	POSITION pos = FindBozoMsgKey(pstrKey);
	if(pos)
	{
		CChartBozoItem* pItem = m_bozoItemList.GetAt(pos);

		if(nBozoType == CChartBozoItem::cbi_delete)
		{
			delete pItem;
			m_bozoItemList.RemoveAt(pos);
		}
		else
		{
			pItem->m_nOption = nBozoType;
			//pItem->m_szKey = pstrKey;
			pItem->m_szValue = pstrValue;
		}
	}
	else
	{	
		CChartBozoItem* pItem = new CChartBozoItem(nBozoType, strKey, strValue);
		m_bozoItemList.AddTail(pItem);
	}
}

POSITION CDrChartCtl::FindBozoMsgKey(LPCTSTR pstrKey)
{
	CChartBozoItem* pItem = NULL;
	POSITION pos = m_bozoItemList.GetHeadPosition();
	POSITION tpos;
	while (pos)
	{
		tpos = pos;
		pItem = m_bozoItemList.GetNext(pos);
		if (pItem)
		{
			if(pItem->m_szKey.CompareNoCase(pstrKey)==0)
				return tpos;
		}
	}
	
	return NULL;
}

void CDrChartCtl::DelAllBozoMsg()
{
	CChartBozoItem* pItem = NULL;
	POSITION pos = m_bozoItemList.GetHeadPosition();
	while (pos)
	{
		pItem = m_bozoItemList.GetNext(pos);
		if (pItem)
		{
			delete pItem;
			pItem = NULL;
		}
	}

	m_bozoItemList.RemoveAll();
}

/*
void CDrChartCtl::ChangeRealSymbol(LPCTSTR pstrSymbol)
{
	CString strSymbol;
	strSymbol = pstrSymbol;
//	m_converter.SetGridSymbol2(strSymbol, strCode);
}
*/
void CDrChartCtl::DelAllPacketRQ()
{
	m_arrAddPacket.RemoveAll();
}

void CDrChartCtl::AddPacketRQ(LPCTSTR pstrPacket)
{
	CString strPacket(pstrPacket);
	m_arrAddPacket.Add(strPacket);
}

BOOL CDrChartCtl::AddChartItem(short nRowIdx, LPCTSTR pstrCode, LPCTSTR pstrName, LPCTSTR pstrGraphName, BOOL bBlock, BOOL bVScale)
{
	CString strKey(pstrCode), strCode, strName(pstrName), strGraphName(pstrGraphName);
	CString strRQ, strTemp;
	CString strValue, strPacket;

	strName.TrimRight();
	strKey.TrimLeft();	strKey.TrimRight();

// 	if(m_bDisableRQSet)
// 	{
// 		CString szTmpPacket;
// 		//UD 1(일),2(주),3(월),4(분),5(틱),6(초)
// 		//카운트(5자리)+보조메시지길이(4)+보조메시지+데이터
// 		szTmpPacket.Format("000000010UD=5@UT=1@");
// 		int nTmpSize = szTmpPacket.GetLength();
// 		CString strPacketName("_DateTime_;_Close_;");
// 		m_pChart->SetPacketDataRQ("DEFAULT", (LPCTSTR)strPacketName, (long)(LPCSTR)szTmpPacket, nTmpSize, TRUE);
// 	}

#ifdef __MULTI_LANG__
	if (strGraphName.Compare(_T("선차트")) == 0)
		strGraphName = _T("단일선차트");

	strGraphName = g_iMetaTable.GetMetaTextDirectly(IMetaTable::INDICATOR_NAME, IMetaTable::KOREAN, strGraphName);

// 	if (strGraphName.Compare(_T("선차트")) == 0)    strGraphName = _T("_Single Line Chart_");
// 	else if (strGraphName.Compare(_T("가격차트")) == 0)   strGraphName = _T("_PriceChart_");
// 	else if (strGraphName.Compare(_T("가격 이동평균")) == 0) strGraphName = _T("");
// 	else if (strGraphName.Compare(_T("거래량차트")) == 0)  strGraphName = _T("_VolumeChart_");
#endif
	int nFind = strKey.Find(_T("\t"));
	if (nFind >= 0)
		strCode = strKey.Left(nFind);

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		AfxMessageBox(_T("이미 등록된 종목입니다."));
		return FALSE;
	}

	strRQ = m_strCurrentRQ = m_pChart->AddChartItem(strKey, _T(""), TRUE);
	info.SetRQ(strRQ);		info.SetKey(strKey);
	info.SetCode(strCode);	info.SetTitle(strName);

	info.RemoveAllPacket();
	if (m_arrAddPacket.GetSize() > 0)
	{
		int nSize = m_arrAddPacket.GetSize();
		for (int i=0; i<nSize; i++)
		{
			strPacket = m_arrAddPacket.GetAt(i);
			info.AddPacket(strPacket);
			m_pChart->AddPacketRQ(m_strCurrentRQ, strPacket);
//			m_pChart->ChangePacketType(strPacket, strPacket);
			strValue += strPacket + _T("\r\n");
		}
	}
	else
	{
		CPropertyMap *pPropMap = (CPropertyMap *)m_converter.GetPropMap();
		if (!pPropMap)
			return FALSE;

		const char* szValue=NULL;
		pPropMap->Lookup(_T("PO"), szValue);
		strValue.Format(_T("%s"), szValue);

		CString strData = strValue;
		int nFind = strData.Find(_T("\n"));
		while (nFind>=0)
		{
			strPacket = strData.Left(nFind+1);
			info.AddPacket(strPacket);
			m_pChart->AddPacketRQ(m_strCurrentRQ, strPacket);

			strPacket = strData.Right(strData.GetLength()-nFind-1);
			strData = strPacket;
			nFind = strData.Find(_T("\n"));
		}
	}

	info.ResetPacketName();

	if (bBlock || (nRowIdx < 0))
		nRowIdx = (int)m_pChart->GetRowBlockCount();

	info.SetRowIdx(nRowIdx);

	m_pChart->AddGraph(nRowIdx, 0, 0, strGraphName, bBlock, bVScale, TRUE);
#ifdef __MULTI_LANG__
	m_pChart->ChangeGraphPropertyAtOnce(info.GetRQ(), _T("_Price_"), strName, -1, -1, -1, NULL, -1, -1, -1);

	//@100115-ojt
	//@100121-alzioyes. 마지막 파리미터 0: 등락률표시, 1:등락률표시 안함.
	if(m_bUseValueToScale)
		m_pChart->ChangeGraphDrawingData(strGraphName, FALSE, FALSE, FALSE, FALSE, 1);

#else
	m_pChart->ChangeGraphPropertyAtOnce(info.GetRQ(), _T("가격"), strName, -1, -1, -1, NULL, -1, -1, -1);
#endif

	m_chartMng.SetChartProp(strKey, info);

//	m_realMng.AdviceRealKey(strCode);

	return TRUE;
}

BOOL CDrChartCtl::ChangeChartItem(LPCTSTR pstrCode, LPCTSTR pstrName)
{
	CString strRQ;
	CString strKey(pstrCode), strCode, strName(pstrName);

	int nFind = strKey.Find(_T("\t"));
	if (nFind >= 0)
		strCode = strKey.Left(nFind);
	else
		strCode = strKey;

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		strRQ = info.GetRQ();
		if (m_strCurrentRQ.Compare(strRQ))
		{
			AfxMessageBox(_T("이미 등록된 종목입니다."));
			return FALSE;
		}
	}

	if (m_chartMng.GetChartRQ(m_strCurrentRQ, info) < 0)
		return FALSE;

	CString strOldCode = info.GetCode();

	strRQ = m_pChart->AddChartItem(strKey, _T(""), FALSE);
	info.SetRQ(strRQ);
	m_pChart->ChangeGraphPropertyAtOnce(strRQ, info.GetTitle(), strName, -1, -1, -1, NULL, -1, -1, -1);

	info.SetTitle(strName);
	info.SetCode(strCode);
	info.SetKey(strKey);
	m_chartMng.SetChartProp(strKey, info);
//	m_realMng.AdviceRealKey(strCode);

	CChartPropArr arr;
	m_chartMng.GetChartPropArr(strOldCode, arr);
	if (arr.GetSize() <= 1)
		m_realMng.UnAdviceRealKey(strOldCode);

	return TRUE;
}

long CDrChartCtl::SendTR(LPCTSTR pstrTRName, LPCTSTR pstrCode, LPCTSTR pstrInData, BOOL bNext)
{
	return SendTrChartGraphDifferReal( pstrTRName, pstrCode, pstrInData, _T(""), bNext);	
}

long CDrChartCtl::SendTrDifferReal(LPCTSTR pstrTRName, LPCTSTR pstrInData, LPCTSTR pstrRealSym, BOOL bNext)
{
	CString strTRName(pstrTRName), strInputData(pstrInData), strRealSym(pstrRealSym), strCode, strTemp;
	//TEST
	strInputData ="000660\t";
	//
	int nIndex = 0, nRet;
	if (m_converter.m_bDirectSend == FALSE)		//	직접조회하는 설정만 조회할 수 있음.
		return 1;

	BOOL bRealABS = FALSE;

	CFidBlock *pBlock = NULL;
	CTrQuery trQry;
	CFidQuery fidQry;
	if (m_converter.m_nTRType)
	{
		nIndex = m_converter.GetTrQuery(strTRName, trQry);
		if (nIndex < 0)
			return 1;

		int nFind = strInputData.Find(_T("\t"));
		strCode = strInputData.Left(nFind);
		if (strCode.IsEmpty())
			return 1;

		bRealABS = trQry.m_OutPack.GetRealABS();
	}
/* KHD : FID 들어냄
	else
	{
		nIndex = m_converter.GetFidQuery(strTRName, fidQry);
		if (nIndex < 0)
			return 1;

		int nLeft = atoi(fidQry.m_OutPack.GetDiffGridSymbol())/1000;
		pBlock = m_converter.GetDefaultFIDBlock(nLeft);

		SetInputData(strInputData, fidQry);

//		strCode = fidQry.m_InPack.GetDataAt(CFidQuery::GetCodeFID(fidQry.m_nMarket));
		CString strFid;
		strCode = fidQry.m_InPack.GetCodeData(strFid);
		if (strCode.IsEmpty())
			return 1;

		bRealABS = fidQry.m_OutPack.GetRealABS();
	}
*/
	strCode.TrimRight();	strTRName.TrimRight();
	CString strKey = strCode + "\t" + strTRName;
	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) < 0)
	{
		info = m_converter.m_prop;
		info.SetRQ(_T("DEFAULT"));	info.SetTRCode(strTRName);
		info.SetKey(strKey);		info.SetCode(strCode);
	}
	else
	{
		CString strRQ = info.GetRQ();
		if (strRQ.Compare(_T("DEFAULT")) == 0)
		{
			CString strDefCode = info.GetCode();
			CChartPropArr arr;
			m_chartMng.GetChartPropArr(strDefCode, arr);
			if (arr.GetSize() <= 0)
				m_realMng.UnAdviceRealKey(strDefCode);
		}
	}
	info.SetABS(bRealABS);
	info.SetDiffRealSym(strRealSym);

	m_converter.m_prop = info;
	m_chartMng.SetChartProp(strKey, info);

	if (m_converter.m_nTRType)
	{
		info.SetTRCode(trQry.m_strID);
		nRet = m_dataComm.SendTRData(info, &trQry, strInputData);
	}
/* FID 들어냄 
	else
	{
		info.SetTRCode(fidQry.m_strID);
		nRet = m_dataComm.SendFIDData(info, fidQry, pBlock, strInputData, bNext);
	}
*/
/*
	if (info.m_bReal)
	{
		int nKey = m_realMng.AdviceRealKey(strCode);
		if (m_converter.m_nTRType)
			nRet = m_realMng.SendRealData(nKey, strCode, trQry, strRealSym);
		else
			nRet = m_realMng.SendRealData(nKey, fidQry, strRealSym);
	}
*/

	if (!m_converter.m_nTRType)
		m_converter.SetFidQuery(nIndex, fidQry);

	m_lState = STATE_SENDTR;
	SendOnChange();

	return 0;
}

//KHD : IBK 맵차트 통신 재구현 
long CDrChartCtl::SendTrChartGraphDifferReal(LPCTSTR pstrTRName, LPCTSTR pstrCode, LPCTSTR pstrInData, LPCTSTR pstrRealSym, BOOL bNext)
{
	CString strRealSym(pstrRealSym), strCode(pstrCode), strTemp;
	CString strSendData, trData, usr1;

	CString strTRCode;

	int nLength;
	
	int nIndex = 0, nRet, nTrKey;
	if (m_converter.m_bDirectSend == FALSE)		//	직접조회하는 설정만 조회할 수 있음.
		return 1;
	
	BOOL bRealABS = FALSE;
	
	CFidBlock *pBlock = NULL;
	CTrQuery trQry;
	CFidQuery fidQry;
	if (m_converter.m_nTRType)
	{
		nIndex = m_converter.GetTrQuery(pstrTRName, trQry);
		if (nIndex < 0)
			return 1;	
	
		strCode.TrimLeft();
		strCode.TrimRight();
		if ( strCode.IsEmpty() )
			return 1;
		
		strTRCode = trQry.m_strTRCode;
		bRealABS = trQry.m_OutPack.GetRealABS();
		//m_converter.m_nRealAbs = bRealABS;
	}
	else
		strTRCode = pstrTRName;

	CString strKey = strCode + "\t" + strTRCode;
	CChartProp info, infoPrev;
	CString strPrevCode, strPrevKey;
	
	infoPrev = m_converter.m_prop;
	strPrevKey = infoPrev.GetKey();
	strPrevCode = infoPrev.GetCode();
	if (m_chartMng.GetChartProp(strPrevKey, infoPrev) >= 0)
		m_realMng.UnAdviceRealKey(strPrevCode);


	if (m_chartMng.GetChartProp(strKey, info) < 0)
	{
		info = m_converter.m_prop;
		info.SetRQ(_T("DEFAULT"));	info.SetTRCode(strTRCode);
		info.SetKey(strKey);		info.SetCode(strCode);		
	}
	else
	{
		CString strRQ = info.GetRQ();
		if (strRQ.Compare(_T("DEFAULT")) == 0)
		{
			CString strDefCode = info.GetCode();
			CChartPropArr arr;
			m_chartMng.GetChartPropArr(strDefCode, arr);
			if (arr.GetSize() <= 0)
				m_realMng.UnAdviceRealKey(strDefCode);
		}
	}
	info.SetABS(bRealABS);
	info.SetDiffRealSym(strRealSym);
	
	m_converter.m_prop = info;
	m_chartMng.SetChartProp(strKey, info);

	if (m_converter.m_nTRType)
	{
		info.SetTRCode(trQry.m_strID);
		nTrKey = nRet = m_dataComm.SendTRData(info, &trQry, &pstrCode[0]);
		usr1.Format("%d", nRet);//키값저장 
	}

	//PIBF0001 데이터// 헤더세팅
	struct mid trMid;
	memset((void*)&trMid, 0x20, sizeof(trMid));	
	CopyMemory(trMid.trxc, strTRCode, 8);	/* 서비스명                 */

	memcpy(trMid.usr1, usr1, strlen(usr1));		/* 공통 입력1               */
	memcpy(trMid.cod2, pstrInData, strlen(pstrInData));

	nLength = SZ_HEADSIZE_MID + strlen(pstrInData);
	strSendData = CString((char *)&trMid, nLength);
	//PIBF0001 데이터 end

	//_userTH 데이터
	struct _userTH udat;
	memset((void*)&udat, 0, sizeof(udat));	
	CopyMemory(udat.trc, strTRCode, strlen(strTRCode));
	udat.stat = US_PASS;
	udat.key = m_ctlInfo.key;//*/TRKEY_GROUP;
	//_userTH 데이터 end
	trData += m_ctlInfo.name + "\t";	//심볼명
	trData += CString((char *)&udat, L_userTH);	//_userTH 데이터
	trData += strSendData;	//PIBF0001 데이터

	OutputDebugString(strSendData);
	OutputDebugString("\n");

//	nRet = m_dataComm.SendTR(strTRCode,trData ,30, US_PASS);
	nRet = m_dataComm.SendTR(strTRCode, trData, nTrKey, US_PASS);
	m_lState = STATE_SENDTR;
	SendOnChange();
	return 0;
}
void CDrChartCtl::SetInputData(CString strData, CFidQuery &qry)
{
	CStringArray arr;
	CDataComm::ParsingInputData(strData, arr);

	int nCnt = arr.GetSize();
	CFidItem *pItem = NULL;
	int nSize = qry.m_InPack.GetItemCount();
	for (int i=0; i<nSize; i++)
	{
		pItem = qry.m_InPack.GetItemAt(i);
		if (pItem)
		{
			if (i >= nCnt)
				break;

			pItem->SetValue(arr.GetAt(i));
		}
	}
}

void CDrChartCtl::SetCodeData(CString strCode, CFidQuery &qry)
{
	CString strID;
	CFidItem *pItem = NULL;
	int nID, nSize = qry.m_InPack.GetItemCount();
	for (int i=0; i<nSize; i++)
	{
		pItem = qry.m_InPack.GetItemAt(i);
		if (pItem)
		{
			strID = pItem->GetItemID();
			nID = atoi(strID)%1000;
			if (nID == 301)
			{
				pItem->SetValue(strCode);
				break;
			}
		}
	}
}

void CDrChartCtl::_DoAdviseToMonitor(BOOL _bAdvise)
{
	//return ;
// 		if(!m_pIDebugMng) return;
// 
// 		WORD dwOnCopyDataKey=g_TestOnCopyStart;
// 
// 		char *gTranName[] = {"DrChartB", NULL};
// 	//	char *gTranName[] = {"p0602","p0603", "p0604", "p0605","p0606","p0607",
// 	//					"KXS3", "IXIJ", "FFC0", "HWFJ","EWs3", NULL};
// 		int nSize = (sizeof(gTranName)/sizeof(char*)) -1;
// 
// 		BYTE cAU = 'A';
// 		BYTE cIO = 'O';
// 		if(!_bAdvise)  cAU = 'U';
// 
// 		for(int i=0; i<nSize; i++)
// 		{
// 			m_pIDebugMng->TraceSR(cAU, cIO, gTranName[i], m_hWnd, dwOnCopyDataKey+i);
// 		}
}

// 복수종목 맵차트 추가 - ojtaso(20090928)
LRESULT CDrChartCtl::OnEventMessage( WPARAM wParam, LPARAM lParam)
{	
	MSG *pMsgFromChart = (MSG *)lParam;

	switch(pMsgFromChart->message)
	{
	case UWM_SELECTEDCHART:
		if (pMsgFromChart->lParam)
		{
			LPCTSTR pSelRQ = (LPCTSTR)pMsgFromChart->lParam;
			CString strRQ(pSelRQ);
			CString strItemCode;
			CChartProp info;

			if(m_bDisableRQSet && strcmp(pSelRQ,"DEFAULT")==0)
			{
				SetTimer(IDTIMER_NEXTSELECT, 1, NULL);
			}
			if (m_chartMng.GetChartRQ(strRQ, info) < 0)
				break;

			if (!pMsgFromChart->wParam)	// 차트 삭제
			{
				CString strDefCode = info.GetCode();
				CChartPropArr arr;
				m_chartMng.GetChartPropArr(strDefCode, arr);
				if (arr.GetSize() <= 1)
					m_realMng.UnAdviceRealKey(strDefCode);

				m_chartMng.RemoveRQ(info.GetRQ());
				
				m_strCurrentRQ = "DEFAULT";
			}
			else						// 선택됨
				m_strCurrentRQ = strRQ;
		}

		break;
	case UWM_ZOOMVALUE:
		//ViewCount : pMsgFromChart->lParam
		{
			if(m_nViewCount!=pMsgFromChart->lParam)
			{
// 				CString szDebug;
// 				szDebug.Format("Map ZoomValue = %d\n", pMsgFromChart->lParam);
// 				OutputDebugString(szDebug);
//				long nCount = m_pChart->GetNOnePageDataCount();
				m_lState = STATE_CHANGEONPAGECNT;
				SendOnChange();
				m_nViewCount=pMsgFromChart->lParam;
			}
		}
// 			if (pMsgFromChart->wParam == 0) //20090922 SJ_KIM ViewCount
// 			{
// 	//			m_lState = STATE_CHANGEONPAGECNT;
// 	//			SendOnChange();
// 			}
		break;
	}
	return 0;
}

LRESULT CDrChartCtl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch (message)
	{
	case WMU_SETNEXTKEY:
		{
			CString strKey;
			strKey = (LPCTSTR)wParam;

			CChartProp info;
			if (m_chartMng.GetChartProp(strKey, info) >= 0)
			{
				GRID_O grid_o;
				memcpy(&grid_o, (char*)lParam, sizeof(GRID_O));
				info.SetGrid_O((GRID_O)grid_o);
				m_chartMng.SetChartProp(strKey, info);
			}
		}
		break;
	case WMU_GETNEXTKEY:
		{
			CString strKey;
			strKey = (LPCTSTR)wParam;

			CChartProp info;
			GRID_I *pInGrid;
			pInGrid = (GRID_I *)lParam;
			if (m_chartMng.GetChartProp(strKey, info) >= 0)
			{
				GRID_O grid_o;
				info.GetGrid_O(grid_o);

				memcpy(pInGrid->save, grid_o.save, sizeof(pInGrid->save));
				memcpy(pInGrid->page, grid_o.page, sizeof(pInGrid->page));
			}
		}
		break;
	default :
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

UINT CDrChartCtl::SendOnChange()
{
	WORD wHigh = MAKEWORD(m_ctlInfo.key, evOnChange);
	WPARAM wParam = MAKELONG(eventDLL, wHigh);

	long lState = m_lState;
	GetParent()->SendMessage(WM_USER, wParam, (LPARAM)(LPTSTR)(LPCTSTR)m_ctlInfo.name);

	if (m_lState == lState)
		m_lState = STATE_NONE;

	return 0;
}

#include <io.h>
#include <DIRECT.H>
LRESULT CDrChartCtl::OnGetUserID( WPARAM wParam, LPARAM lParam)
{
	LPCSTR szUserID;
	static CString szID;
	szUserID = (char*)GetParent()->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);
	// 20170110 전중현 - 0015021: [2000] window10 에서 차트에 지표추가등 설정 변경후 화면 재오픈시 유지되지 않는 현상
	
	//szID.Format("%08u", HashKey(szUserID));
	szID.Format("%08u", HashDataAXIS(szUserID));
	
	szID += "\\ChartMap";

	{
		CString szUserDir;
		GetDir_DrfnRoot(szUserDir);
		szUserDir += "\\user\\" + szID;
		if (_access(szUserDir, 0) == -1)
			mkdir(szUserDir);
	}
	return (long)(LPCSTR)szID;
}


UINT CDrChartCtl::HashDataAXIS(LPCSTR key)
{
	UINT nHash = 0;
	while (*key)
		nHash = (nHash << 5) + nHash + *key++;

	return nHash;
}

long CDrChartCtl::SetValueToScale(LPCTSTR szKey, BOOL bUse) 
{
	m_bUseValueToScale = bUse;

	return 0;
}

void CDrChartCtl::SetRedrawCall(BOOL bUse) 
{
	m_bReDrawCall = bUse;
}

void CDrChartCtl::SetDefaultRQ(BOOL bSetDisable, LPCTSTR szKey, LPCTSTR szData) 
{
	m_bDisableRQSet = bSetDisable;
}

void CDrChartCtl::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	{
		KillTimer(nIDEvent);

		CString rString;
		if(m_chartMng.GetRQChartAt(rString, 0)==true)
		{
			m_pChart->SetCurrentRQ(rString);
			m_strCurrentRQ = rString;
			//m_pChart->SetGraphHide3(rString, FALSE);
			//m_pChart->SetGraphHide3(rString, TRUE);
		}
	}
	CWnd::OnTimer(nIDEvent);
}


int CDrChartCtl::ReSetPacket(CString &strValue, CString &strBozoMsg, CString strPacketType/*=""*/)
{
	int nBozo = strBozoMsg.GetLength();
	CStringArray arrName, arrSize, arrType;

	m_converter.GetDataPropArr(strValue, 1, arrName);
	m_converter.GetDataPropArr(strValue, 3, arrSize);
	m_converter.GetDataPropArr(strValue, 4, arrType);

	int nSize, i;

	strValue = _T("");
	if (m_converter.GetChartType() == 1)
	{
		int nPcnt = 0;
		double dRate=1;
		CString strType, strRate, strRes;
		
		CFidBlock* pBlock = m_converter.GetDefaultFIDBlock(atoi(m_converter.m_strMajorFID));
		if (pBlock)
		{
			CFidItem *pItem = pBlock->GetItemNameAt(_T("가격표시구분"));
			if (pItem)
			{
				m_converter.GetDecimalRate(pItem->GetValue(), nPcnt, dRate, strRes);
				strRate.Format(_T("%.*f"), nPcnt, dRate);

				nSize = arrType.GetSize();
				CString strName, strType;
				for (i=0; i<nSize; i++)
				{
					strType = arrType.GetAt(i);
					if (strType.GetLength() > 2)
						strType = strType.Mid(2);
					else
						continue;
					
					if (atof(strType) == 0.000001)
						strValue += arrName.GetAt(i) + _T(":") + arrSize.GetAt(i) + _T(":") + strRes + _T("|");
					else
					{
						strName = arrName.GetAt(i);
						strType = arrType.GetAt(i);
						if (!strPacketType.IsEmpty() && strName.CompareNoCase(_T("_DateTime_"))==0)
							strType = strPacketType;

						strValue += strName + _T(":") + arrSize.GetAt(i) + _T(":") + strType + _T("|");
					}
				}
			}
		}
	}
	else
	{
		nSize = arrName.GetSize();
		//<<20100310_JS.Kim FX
		//for (i=0; i<nSize; i++)
		//	strValue += arrName.GetAt(i) + _T(":") + arrSize.GetAt(i) + _T(":") + arrType.GetAt(i) + _T("|");
		if( strPacketType.IsEmpty() )
		{
			for (i=0; i<nSize; i++)
				strValue += arrName.GetAt(i) + _T(":") + arrSize.GetAt(i) + _T(":") + arrType.GetAt(i) + _T("|");
		}
		else
		{
			CString strName;
			for (i=0; i<nSize; i++)
			{
				strName = arrName.GetAt(i);
				if( strName == "_Close_" || strName == "_Open_" || strName == "_High_" || strName == "_Low_"
					|| strName == "_BuyClose_" || strName == "_BuyOpen_" || strName == "_BuyHigh_" || strName == "_BuyLow_"
					|| strName == "_SellClose_" || strName == "_SellOpen_" || strName == "_SellHigh_" || strName == "_SellLow_" )
					strValue += arrName.GetAt(i) + _T(":") + arrSize.GetAt(i) + _T(":") + strPacketType + _T("|");
			}
		}
		//>>
	}

	if (strValue.GetLength() > 0)
	{
		List_CChartBozoItem bozoItemList;
		CChartBozoItem* pItem = new CChartBozoItem(CChartBozoItem::cbi_add, "RESETPACKET", strValue);
		bozoItemList.AddTail(pItem);
		nBozo = m_converter.m_pBozoMng->ChangeChartBozoMsg(strBozoMsg, &bozoItemList);
		delete pItem;	pItem = NULL;
		bozoItemList.RemoveAll();
	}

	return nBozo;
}

BOOL CDrChartCtl::SetLinePropRQ(LPCTSTR pstrCode, LPCTSTR pstrData)
{
	CString strKey(pstrCode), strData(pstrData);

	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		CString strRQ = info.GetRQ();
		if (!strRQ.IsEmpty())
		{
			CString strValue, strInColor;
			int nGraphType, nGraphStyle, nDrawStyle, nThickness1, nThickness2, nThickness3;
			nGraphType = nGraphStyle = nDrawStyle = nThickness1 = nThickness2 = nThickness3 = -1;

			int nIndex=0, nFind = strData.Find("\t");
			while (nFind >= 0)
			{
				strValue = strData.Left(nFind);
				if (strValue.GetLength() > 0)
				{
					switch (nIndex)
					{
					case 0:	nGraphType = atoi(strValue);	break;
					case 1:	nGraphStyle = atoi(strValue);	break;
					case 2:	nDrawStyle = atoi(strValue);	break;
					case 4:	nThickness1 = atoi(strValue);	break;
					case 5:	nThickness2 = atoi(strValue);	break;
					case 6:	nThickness3 = atoi(strValue);	break;
					case 3:
						if (strValue.GetLength() == 9)
						{
							strInColor = strValue.Left(3) + "," + strValue.Mid(3, 3) + "," + strValue.Right(3);
							strValue = strInColor;

							strInColor = strValue + ";" + strValue + ";" + strValue + ";" + strValue + ";" + strValue + ";" + strValue + ";";
							strInColor += "0;1;";
							strInColor += strValue + ";" + strValue + ";" + strValue + ";" + strValue + ";";
							strInColor += "0;1";
						}
						break;
					}
				}

				strData = strData.Mid(nFind+1);
				nFind = strData.Find("\t");

				nIndex++;
			}

			m_pChart->ChangeGraphPropertyAtOnce(strRQ, info.GetTitle(), info.GetTitle(),
						nGraphType, nGraphStyle, nDrawStyle, strInColor, nThickness1, nThickness2, nThickness3);

			m_pChart->Invalidate();
		}
	}

	return TRUE;
}

BSTR CDrChartCtl::GetLinePropRQ(LPCTSTR pstrCode)
{
	CString strKey(pstrCode), strResult;

	strResult = _T("");
	CChartProp info;
	if (m_chartMng.GetChartProp(strKey, info) >= 0)
	{
		CString strRQ = info.GetRQ();
		if (!strRQ.IsEmpty())
			strResult.Format(_T("%s"), m_pChart->GetGraphPropertyAtOnce(strRQ, info.GetTitle()));
	}

	return strResult.AllocSysString();
}

void CDrChartCtl::UseExtraChart(LPCTSTR _szChartName, BOOL bUse) 
{
	CString szChartName(_szChartName);
	if(szChartName.CompareNoCase("MultiBar")==0)
	{
		if(bUse)
		{
			CMultiBar* pExtraChart = new CMultiBar;
			m_pExtraChart = (CWnd*)pExtraChart;

			BOOL bCreate = pExtraChart->Create("", WS_CHILD|WS_VISIBLE, CRect(0,0,0,0), this, ID_CHART+1);
		}
	}
}

void CDrChartCtl::SetShowYScaleData(LPCTSTR _szGraphName) 
{
	if(m_pChart)
	{
		CString strGraphName = g_iMetaTable.GetMetaTextDirectly(IMetaTable::INDICATOR_NAME, IMetaTable::KOREAN, _szGraphName);
		m_pChart->ChangeGraphDrawingData(strGraphName, FALSE, FALSE, FALSE, FALSE, 1);
	}
}

//<<20100310_JS.Kim FX에서 매도/매수 여부.
void CDrChartCtl::SetChartDataType(LPCTSTR szType)
{
	if( strlen(szType) == 0 )		return;

	if( strcmp(szType, "B") == 0 )
		m_converter.m_nFXChartType = 1;		// 0:매도, 1:매수
	else
		m_converter.m_nFXChartType = 0;		// 0:매도, 1:매수
}
//>>

//<<20100518_JS.Kim FX	지표 차트 Hide/Show
void CDrChartCtl::SetIndicatorHide(LPCTSTR szGraphName, LPCTSTR szHideData)
{
	if(!m_pChart)		return;

	short	nSubGraphIndex = 0;
	BOOL	bHide;
	int		pos;
	CString strData = szHideData;
	while( !strData.IsEmpty() && (pos = strData.Find(',')) != -1 )
	{
		bHide = (BOOL)atoi(strData.Left(pos));
		strData = strData.Mid(pos+1);
		m_pChart->SetGraphHide4(szGraphName, nSubGraphIndex, bHide);
		nSubGraphIndex ++;
	}
	strData.TrimLeft();		strData.TrimRight();
	if( !strData.IsEmpty() )
	{
		bHide = (BOOL)atoi(strData);
		m_pChart->SetGraphHide4(szGraphName, nSubGraphIndex, bHide);
	}
}


BSTR CDrChartCtl::GetIndicatorHide(LPCTSTR szGraphName, short nCount)
{
	CString strResult = "";

	if(!m_pChart)		return strResult.AllocSysString();

	for( int i=0; i<nCount; i++ )
	{
		if( m_pChart->GetGraphHide4(szGraphName, i) )
			strResult += "1,";
		else
			strResult += "0,";
	}
	return strResult.AllocSysString();
}

//차트 다음 조회를 위한 NextKey
void CDrChartCtl::SetNextKey(LPCTSTR szNextKey)
{
	m_converter.m_strNextKey = szNextKey;
}


BSTR CDrChartCtl::GetNextKey()
{
	CString strResult = m_converter.m_strNextKey;
	return strResult.AllocSysString();
}

//>>
void CDrChartCtl::SetAdvRealData(int nTRType, CChartProp info)
{
	if(nTRType == 1) //TR Type
	{
		CString strCode = info.GetCode();
		CString strTRName = info.GetTRCode();
		CString strRealSym = info.GetDiffRealSym();	
		CString strData = GetNormalRealSet(strCode, strRealSym);	
	
		OutputDebugString(strData); OutputDebugString("\n");

		CString strField = "KXS3";
		char* DataName = (LPSTR)(LPCTSTR)strField;
		char* KeyName  = (LPSTR)(LPCTSTR)strCode;
	//	m_pPtMng->RequestMChartReal('A', DataName, KeyName);
		m_dataComm.SendTR("GOOPHOOP", (LPSTR)(LPCTSTR)strData, 102, US_OOP);
	}
	else //FID Type
	{

	}
}

CString CDrChartCtl::GetNormalRealSet(CString strCode, CString strRealSym)
{
	int nMarket = m_converter.GetMarketTypeFromCode(strCode);
	CString strMarketSymbol;
	CString strData;
	if(strRealSym.IsEmpty())
	{
		strMarketSymbol = GetMarketSymbol(nMarket);

		strData += strMarketSymbol + _T("301");
		strData += sDS;
		strData += strCode;
		strData += sCELL;
		strData += strMarketSymbol + _T("021");
		strData += sCELL;
		strData += strMarketSymbol + _T("023");
		strData += sCELL;	
	}
	return strData;
}

CString CDrChartCtl::GetMarketSymbol(int nMarket)
{
	CString strMarketSymbol = _T("");

	if(nMarket == CMasterTypeInfo::STOCK_CHART)  //주식
		strMarketSymbol = _T("1");
	else if(nMarket == CMasterTypeInfo::UPJONG_CHART) //업종
		strMarketSymbol = _T("21");
	else if(nMarket == CMasterTypeInfo::FUTOPT_CHART) //선물 (선물/옵션은 따로 구분 작업 해야함.)
		strMarketSymbol = _T("31");
	else if(nMarket == CMasterTypeInfo::FUTOPT_CHART) //옵션
		strMarketSymbol = _T("41");
	else if(nMarket == CMasterTypeInfo::STOCKFUTOPT_CHART) //주식선물
		strMarketSymbol = _T("61");
	else if(nMarket == CMasterTypeInfo::FOREIGN_CHART) //해외
		strMarketSymbol = _T("51");
	else if(nMarket == CMasterTypeInfo::COMMODITY_CHART) //상품선물
		strMarketSymbol = _T("101");
	else if(nMarket == CMasterTypeInfo::FUOPT_JIPYO_CHART) // FO 지수,
		strMarketSymbol = _T("");
	else if(nMarket == CMasterTypeInfo::FO_FOREIGN_CHART) // 해외선물, IBK 는 없음.
		strMarketSymbol = _T("");
	else if(nMarket == CMasterTypeInfo::FX_CHART)	    	// FX 지수, IBK 는 없음.
		strMarketSymbol = _T("");
	else if(nMarket == CMasterTypeInfo::STOCKFUTOPT_CHART) // 주식옵션, IBK 는 없음.
		strMarketSymbol = _T("");
	else
		strMarketSymbol = _T("");

	return strMarketSymbol;
}



	

