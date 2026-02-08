#include "stdafx.h"
#include "axisGView.h"
#include "grpView.h"
#include "fileIO.h"
#include "GroupList.h"
#include "PrintDlg.h"
#include "../Common/SavedHead.h"
#include "../../h/menudef.h"

#include "../AxMCtrl/ConfigBar_Pru.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TIMER_EXTR_TR	10001
#define	CM_SETKWANSIM	WM_USER + 9001	// 관심등록

#define TRLEN	8
#define COLOR_OUT	RGB(0, 81, 142)
#define COLOR_IN	RGB(255, 255, 255)

CGrpView::CGrpView(CWnd* pwndView, char* pcDllName)
{
	m_pApp = (CAxisGViewApp *)AfxGetApp();

	m_pwndView	= pwndView;
	strcpy(m_acDllName, pcDllName);	// fixed by schbang 2006.02.20 to skip error in debug mode

	m_hiGComLib	= NULL;
	m_hiGMainLib	= NULL;
	m_hiGDlgLib	= NULL;

	m_pCfg		= NULL;
	m_pwndInput	= NULL;
	m_pwndChart	= NULL;
	m_pwndSise	= NULL;
	m_pwndCtrl	= NULL;
	m_pwndTool	= NULL;
	m_pwndTable	= NULL;
	m_pwndKwansim	= NULL;
	m_pwndAid	= NULL;

	m_mapPanel.RemoveAll();
	m_arCode.RemoveAll();
	m_iListKind = TRCD_HISTORY;
	InitCodeList();

	m_pEnvInfo = NULL;
	ZeroMemory(m_pacRegion, sizeof(m_pacRegion));
	ZeroMemory(m_pacGraph, sizeof(m_pacGraph));
	ZeroMemory(&m_TrData, SZ_TRDATA);
	m_pcGDat = NULL;

	m_iTRIndex		= 0;
	m_pcTemp		= NULL;	
	//** m_pGroupList		= NULL;
	m_bAutotran		= false;
	m_bTransaction		= false;
	m_bTransactionS		= false;
	m_bExtrTr		= false;
	m_iDtIndex		= 0; // 2006.11.16 lsh 추가	

	m_trSender = NULL;
}

CGrpView::~CGrpView()
{
	if (m_pCfg)
		LIB_DELETE(m_pCfg);

	if (m_pwndInput)
		LIB_DELETE(m_pwndInput);

	if (m_pwndChart)
		LIB_DELETE(m_pwndChart);

	if (m_pwndSise)
		LIB_DELETE(m_pwndSise);

	if (m_pwndCtrl)
		LIB_DELETE(m_pwndCtrl);

	if (m_pwndTool)
		LIB_DELETE(m_pwndTool);

	if (m_pwndTable)
		LIB_DELETE(m_pwndTable);

	if (m_pwndAid)
		LIB_DELETE(m_pwndAid);
		

	if (m_pEnvInfo)
		BUF_DELETE(m_pEnvInfo);
	for (int ii = 0; ii < MAX_INDEX; ii++)
	{
		if (m_pacRegion[ii])
			BUF_DELETE(m_pacRegion[ii]);
		if (m_pacGraph[ii])
			BUF_DELETE(m_pacGraph[ii]);
	}
	if (m_pcTemp)
		BUF_DELETE(m_pcTemp);
	if (m_pcGDat)
		BUF_DELETE(m_pcGDat);
	
	if (m_hiGComLib)
		FreeLibrary(m_hiGComLib);
	if (m_hiGMainLib)
		FreeLibrary(m_hiGMainLib);
	if (m_hiGDlgLib)
		FreeLibrary(m_hiGDlgLib);

	/**
	if (m_pGroupList)
	{
		m_pGroupList->DestroyWindow();
		delete m_pGroupList;
		m_pGroupList = NULL;
	}
	**/

	RemoveCodeList();

	if (m_TrData.pcData[0])
		BUF_DELETE(m_TrData.pcData[0]);

	if (m_TrData.pcData[1])
		BUF_DELETE(m_TrData.pcData[1]);

	for (ii = 0; ii < m_arData.GetSize(); ii++)
	{
		char* pcData = m_arData.GetAt(ii);
		delete pcData;
	}
	m_arData.RemoveAll();
	m_arLen.RemoveAll();


	DelPanels();
}

BEGIN_MESSAGE_MAP(CGrpView, CWnd)
	//{{AFX_MSG_MAP(CGrpView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(GEV_VIEW, OnViewEvent)
	ON_MESSAGE(GEV_CHART, OnChartEvent)
	ON_MESSAGE(GEV_INPUT, OnInputEvent)
	ON_MESSAGE(GEV_SISE, OnSiseEvent)
	ON_MESSAGE(GEV_CTRL, OnCtrlEvent)
	ON_MESSAGE(GEV_TOOL, OnToolEvent)
	ON_MESSAGE(GEV_CFG, OnCfgEvent)
	ON_MESSAGE(GEV_TABLE, OnTableEvent)
	ON_MESSAGE(GEV_AID, OnAidEvent)
	ON_MESSAGE(CM_SETKWANSIM, OnSetKwansim)
	ON_MESSAGE(UM_FROM_MAIN, OnFromMain)
	//** macho add
	ON_MESSAGE(WM_VIEW_SENDTR, OnViewSendTR)
END_MESSAGE_MAP()

// test io
//#include <fstream.h>/////////xxxxx
LRESULT CGrpView::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString strTemp;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:		// Tran Data
//#ifdef _DEBUG
//WriteFile(CString((char*)lParam, HIWORD(wParam)));
//#endif
/*{
ofstream dFile ("C:\\glog.txt", ios::app);
dFile	<<endl<<"e "<<GetTickCount()<<endl;
}*/
		// debug : 20090915
		//{
		//	CString strDebug;
		//	strDebug.Format("CGrpView::OnMessage :DLL_OUB");
		//	AfxMessageBox(strDebug);
		//}
		// debug end

		return Oub(wParam, lParam);
		break;
	case DLL_ALERT:		// RealTime
		if (m_pwndChart && !m_bAutotran)
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REALTIME_DATA), lParam);
		break;
	case DLL_TRIGGER:	// Trigger
		if (m_bTransaction || m_bTransactionS)
			break;
		if (m_pwndInput && !m_bAutotran)
			m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, triggerCode), lParam);
		break;
	case DLL_DOMINO:	// Domino
		if (m_bTransaction || m_bTransactionS)
			break;
		if (m_pwndInput && !m_bAutotran)
			m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, domminoCode), lParam);
		break;
	case DLL_DROP:		// OnDrop
		OnDrop(wParam, lParam);
		break;
	case DLL_INVOKE:
		// axisAP.h
		break;
	case DLL_SETPAL:	// Change Palette
		if (m_pwndInput)	m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputChgPalette, 0), 0);
		if (m_pwndSise)	m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(siseChgPalette, 0), 0);
//		if (m_pwndChart)	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(chartChgPalette, 0), 0);
		if (m_pCfg)	m_pCfg->ChangePalette();
		RedrawWindow();
		break;
	case DLL_SETFONT:
//		if (m_pwndSise)
//			m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(siseFont, HIWORD(wParam)), lParam);
		break;
	case DLL_SETFCB:
		if (m_pwndSise)
			m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(siseFlash, HIWORD(wParam)), lParam);
		break;
	}
	return TRUE;
}

LRESULT CGrpView::Oub(WPARAM wParam, LPARAM lParam)
{
	// debug : 20090915
	//{
	//	CString strDebug;
	//	strDebug.Format("CGrpView::Oub() 1");
	//	AfxMessageBox(strDebug);
	//}
	// debug end

	// 관심종목 다이얼로그 데이터 from 서버 (서버에서 불러오기)
	if (m_pwndKwansim)
	{
		m_pwndKwansim->SendMessage(WM_USER, wParam, lParam);
		return TRUE;
	}

	if (m_trSender) 
	{
		m_trSender->SendMessage(WM_USER, wParam, lParam);
		m_trSender = NULL;

		m_bTransaction = false;
		return TRUE;
	}

	// debug : 20090915
	//{
	//	CString strDebug;
	//	strDebug.Format("CGrpView::Oub() 2");
	//	AfxMessageBox(strDebug);
	//}
	// debug end

	_envInfo* penvinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	// 데이터가 여러번에 거쳐 올경우 저장

	int recvLen = HIWORD(wParam);
	if (recvLen <= SZ_ZCOMHEAD) 
		return FALSE;

	struct  _zcom_head* pZComHead = (struct  _zcom_head*)lParam;

	int iTCnt = atoi(CString(pZComHead->tcnt, sizeof(pZComHead->tcnt)));
	int iSeqn = atoi(CString(pZComHead->seqn, sizeof(pZComHead->seqn)));

	//**Debug("T = %d, S = %d, Len = %d", iTCnt, iSeqn, HIWORD(wParam) - SZ_ZCOMHEAD);

	char* pcTotalBuf = NULL;
	int iTotalLen = 0;
	if (iTCnt > 1)
	{
		int iLen = HIWORD(wParam) - SZ_ZCOMHEAD;
		char* pcBuf = new char[iLen];
		CopyMemory(pcBuf, (char*)lParam + SZ_ZCOMHEAD, iLen);
		m_arData.Add(pcBuf);
		m_arLen.Add(iLen);
		if (iSeqn < iTCnt - 1)
			return TRUE;

		for (int ii = 0; ii < m_arData.GetSize(); ii++)
		{
			iTotalLen += m_arLen.GetAt(ii);
		}

		pcTotalBuf = new char[iTotalLen];

		ASSERT(pcTotalBuf);

		char* pcWrite = pcTotalBuf;
		for (ii = 0; ii < m_arData.GetSize(); ii++)
		{
			pcBuf = m_arData.GetAt(ii);
			iLen = m_arLen.GetAt(ii);

			CopyMemory(pcWrite, pcBuf, iLen);
			pcWrite += iLen;
			delete pcBuf;
		}

		
		m_arData.RemoveAll();
		m_arLen.RemoveAll();

		lParam = (LPARAM)pcTotalBuf;
		//wParam = MAKEWPARAM(LOWORD(wParam), iTotalLen));
	}
	else
	{
		lParam += SZ_ZCOMHEAD;
		wParam = MAKEWPARAM(LOWORD(wParam), HIWORD(wParam) - SZ_ZCOMHEAD);
	}

	if (m_bExtrTr)
	{	
		m_TrData.iLen[0] = iTotalLen > 0 ? iTotalLen : HIWORD(wParam);
		m_TrData.pcData[0] = new char[m_TrData.iLen[0]];
		m_TrData.iSiseLen[0] = 0;
		CopyMemory(m_TrData.pcData[0], (char*)lParam, m_TrData.iLen[0]);

		SetTimer(TIMER_EXTR_TR, 1, NULL);
	}
	else if (m_bTransactionS)
	{
		m_TrData.iLen[0] = iTotalLen > 0 ? iTotalLen : HIWORD(wParam);
		m_TrData.pcData[0] = new char[m_TrData.iLen[0]];
		CopyMemory(m_TrData.pcData[0], (char*)lParam, m_TrData.iLen[0]);
		m_TrData.iSiseLen[0] = 0;

		if (!m_TrData.pcData[0] || m_TrData.iLen[0] == 0)
		{
			m_bTransactionS = false;
			if (pcTotalBuf)
				delete pcTotalBuf;
			return 0;
		}

		//m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(chartTRDataS, 0), (long)&m_CxTRData);
		m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_TR_DATAS, 0), (LPARAM)&m_TrData);

		delete m_TrData.pcData[0];
		ZeroMemory(&m_TrData, sizeof(m_TrData));
		m_bTransaction = false;
		m_bTransactionS = false;
	}
	else
	{
		m_TrData.iLen[m_iTRIndex] = iTotalLen > 0 ? iTotalLen : HIWORD(wParam);
		m_TrData.pcData[m_iTRIndex] = new char[m_TrData.iLen[m_iTRIndex]];
		CopyMemory(m_TrData.pcData[m_iTRIndex], (char*)lParam, m_TrData.iLen[m_iTRIndex]);

		if (m_TrData.iLen[0] == 0 || m_TrData.pcData[0] == NULL)
		{
			m_pwndSise->SendMessage(GEV_SISE, (SISE_TR_DATA, 0), 0);
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_TR_DATA, 0), NULL);
			if (m_pwndTable)
				m_pwndTable->SendMessage(GEV_TABLE, tbReset, 0);

			m_bTransaction = false;

			if (pcTotalBuf)
				delete pcTotalBuf;
			return 0;
		}

		//int iSiseLen = m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(SISE_TR_DATA, NULL), (LPARAM)&m_TrData);
		m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(SISE_TR_DATA, NULL), (LPARAM)&m_TrData);

		//Debug("recv len = %d", m_TrData.iLen[0]);
		m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_TR_DATA, 0), (long)&m_TrData);
		
		delete m_TrData.pcData[0];
		if (m_TrData.pcData[1]) {
			delete m_TrData.pcData[1];
		}

		ZeroMemory(&m_TrData, sizeof(m_TrData));
		
		m_bTransaction = false;
	}

	if (pcTotalBuf)
		delete pcTotalBuf;

	// debug : 20090915
	//{
	//	CString strDebug;
	//	strDebug.Format("CGrpView::Oub() End");
	//	AfxMessageBox(strDebug);
	//}
	// debug end

	return TRUE;
}

LRESULT CGrpView::OnViewEvent(WPARAM wParam, LPARAM lParam)
{
	struct _graph	graph;
	switch (LOWORD(wParam))
	{
	case viewInterface:
		switch (HIWORD(wParam))
		{
		case getRootPath:
			return (LRESULT)(LPCTSTR)m_pApp->GetRoot();
		case getTabPath:
			return (LRESULT)(LPCTSTR)m_pApp->GetTabPath();
		case getUserName:
			return (LRESULT)(LPCTSTR)m_pApp->GetUName();
		case getUserID:
			return (LRESULT)(LPCTSTR)m_pApp->GetUID();
			break;
//		case getDatPath:
//			return (LRESULT)(LPCTSTR)m_strDatPath;
//			break;
		case requestTR:
			SendRequest();
			break;
		case requestTRS:
			SendRequestS();
			break;
		case getDllName:
			return (LRESULT)m_acDllName;
		}
		break;
	case viewDialog:
		switch (HIWORD(wParam))
		{
		case dlgSave:
			CallSaveDlg();
			break;
		case dlgEnv:		// 버튼 환경설정
			EnvProc();
			break;
		case dlgToolEnv:	// 버튼 - 도구
			ToolEnvProc();
			break;
		case dlgEnvchart: // direct call
			if (CallEnvDlg(NULL, true))
			{
				m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, CHANGE_ENV_INFO));
				_envInfo *envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
				m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(siseChgEnv, 0), envinfo->display.rgbBG);
			}
			break;
		case dlgMap:	// 화면 설정(region + graph), lParam - _interChart *
			{
				ClearBuffer();
				
				_interChart	*iChart = (_interChart*)lParam;
				int	bufSize = sz_INTERCHART + SZ_REGION*iChart->rgncnt + SZ_GRAPH*iChart->grpcnt;

				m_pcTemp = AllocBuffer(bufSize);
				CopyMemory(m_pcTemp, (char*)lParam, bufSize);

				char* pScreen = CallMapDlg(m_pcTemp);

				ClearBuffer();

				if (!pScreen)	return FALSE;

				SyncMap(pScreen);
			}
			break;
		case DlgIndc:	// 개별차트설정, lParam - _graph *
			return CallEnvDlg((char*)lParam, false);
		case dlgSIndc:	// config bar의 개별차트설정
			return CallStandardIndc(lParam);
		case dlgTool:
			return CallToolDlg((char*)lParam);
		case dlgGwan:
			{
				HINSTANCE	hInterLib;
				CString	path;
				CString	root = m_pApp->GetRoot();
				CString	user = m_pApp->GetUName();
				//path.Format("%s\\dev\\INTER3.dll", root);
				path.Format("%s\\dev\\INTER.dll", root);
				hInterLib = LoadLibrary(path);
				if (!hInterLib)
				{
					m_pApp->MessageBoxF("%s load error", path);
					return 0;
				}
				//axInterest(CWnd *wizard, CWnd *misc, char* home, char* user, char *code)
				BOOL	(APIENTRY *axInterest)(CWnd*, CWnd*, char*, char*, char*);
				axInterest = (BOOL (APIENTRY *)(CWnd*, CWnd*, char*, char*, char*))GetProcAddress(hInterLib, "axInterest");
				BOOL bRet = axInterest(m_pwndView, this, (char*)root.operator LPCTSTR(), (char*)user.operator LPCTSTR(), (char*)lParam);

				m_pwndKwansim = NULL;

				FreeLibrary(hInterLib);
			}
			break;
		case dlgJCode:	case dlgUCode:	case dlgFCode:	case dlgOCode:	
		case dlgJUCode:	case dlgFOCode:
		case dlgCommoditiesFutureCode:	// 20090907 : ADD :
			return (long)OpenCodeDlg(HIWORD(wParam), lParam);
		case dlgGubnFOSO:
			return m_pwndView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, whichKIND), lParam);
		case dlgcommodityName:			// 20090907 : ADD :
			return m_pwndView->SendMessage(WM_USER, MAKEWPARAM(nameDLL, commodityNAME), lParam);
		case dlgToolCfg:
			return CallPnToolDlg((char*)lParam);
		case DlgAidCfg:
			return CallPnAidDlg((char*)lParam);
		case DlgYAxisCfg:
			return CallYAxisConfing((char*)lParam);
		case DlgYShowCfg:
			return CallYShowConfingByMove();
		default:
			return (long)OpenCodeDlg(usCODE, lParam);
			break;
		}
		break;
	case viewMain:
		switch (HIWORD(wParam))
		{
		case getPalette:
			return m_pwndView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), lParam);
			break;
		case getHistory:
			return m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC), lParam);
			break;
		case setHistory:
			m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, codeCC), lParam);
			break;
		case setTrigger:
			m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), lParam);
			break;
		case openView:
			m_pwndView->PostMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), lParam);
			break;
		case isSiseOnly:
			{
				char	*accn = (char*)m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, accountCC), 0);
				if (strlen(accn) <= 0)	return TRUE;
				else			return FALSE;
			}
			break;
		}
		break;
	case viewChartInfo:
		switch (HIWORD(wParam))
		{
		case getMapInfo:	// 맵그래프 정보
			return (LRESULT)GetMapInfo();
		case setMapInfo:	// 맵그래프 정보
			break;
		case getSGrpInfo:	// 개별 그래프 정보(from dat file)
			return GetGrpFromDat((char*)lParam);
		case getDatInfo:	// 전체 그래프 기본정보
			return (LRESULT)GetDat();
		case getMainGrpInfo:
			return !MainChartIsVarType();
		}
		break;
	case viewCode:
		switch (HIWORD(wParam))
		{
		case hideList:
			/**
			if (m_pGroupList)
			{
				m_pGroupList->DestroyWindow();
				delete m_pGroupList;
				m_pGroupList = NULL;
			}
			**/
			break;
		case showGroupTree:
			/**
			if (m_pGroupList)
			{
				m_pGroupList->DestroyWindow();
				delete m_pGroupList;
				m_pGroupList = NULL;
			}

			m_pGroupList = new CGroupList(m_pwndView, this, m_pFont, m_pApp->GetRoot(), m_pApp->GetUName());
			m_pGroupList->Create();
			m_pGroupList->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
			m_pGroupList->ShowWindow(SW_HIDE);

			{
				CPoint	point;
				GetCursorPos(&point);
				m_pGroupList->MoveWindow(point.x, point.y, 200, 300);
				m_pGroupList->SetShow();
				m_pGroupList->SetFocus();
			}
			**/
			break;
		case getPreCode:
			return (long)GetNearCode(true, (char*)lParam);
			break;
		case getNextCode:
			return (long)GetNearCode(false, (char*)lParam);
			break;
		}
		break;
	case viewUtility:
		switch(HIWORD(wParam))
		{
		case viewImage:
			{
				CString	path;
				CString	title = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, getSaveName), 0);
				path = m_pApp->GetUserPath() + title + ".bmp";

				if (CFileIO::ImageWindow(this, path))
				{
					m_pApp->MessageBoxF("%s로 저장되었습니다.", path);
				}
			}
			break;
		case viewPrint:
			PrintProc();				
			break;
		case viewExcel:
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, excelData), 0);
			break;
		}
		break;
	case viewNotify:
		switch (HIWORD(wParam))
		{
		case dispDayChart:
			SendMessage(GEV_VIEW, MAKEWPARAM(viewInput, setDisplayEdit), lParam);
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlDisplay), lParam);
			break;
		case dispDayInput:	// edit box
			if (m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, CHART_DISPLAY), lParam))
				m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlDisplay), lParam);
			break;
		case dispDayCtrl:
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, CHART_DISPLAY), lParam);
			SendMessage(GEV_VIEW, MAKEWPARAM(viewInput, setDisplayEdit), lParam);
			break;
		case dispDayCtrl2:	// edit box
			if (m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, CHART_DISPLAY), lParam))
				m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlDisplay), lParam);
			break;
		case dispScaleChart:
			SendMessage(GEV_VIEW, MAKEWPARAM(viewInput, setDisplayEdit), lParam);
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlDisplay), lParam);
			break;
		case dispScaleCtrl:
			{
				int dispDay = m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartScale), lParam);
				if (dispDay != 0)
					SendMessage(GEV_VIEW, MAKEWPARAM(viewInput, setDisplayEdit), dispDay);
			}
			break;
		case mainChart:	// change in chart 
			m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputEnv, inputMain), lParam);
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlMain), lParam);
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, CHANGE_MAIN_GRP), lParam);
			if (m_pCfg)	m_pCfg->ReloadGraph(LOWORD(lParam));
			break;
		case aidAddChart:
			if (MainChartIsVarType()) return 0L;

			if (!CaseCheck(lParam))	return 0L;
			
			graph.wGKind = WORD(lParam);
			GetGrpFromDat((char*)&graph);
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, ADD_AID_GRP), (long)&graph);
			if (m_pCfg)
				m_pCfg->AddGraph(WORD(lParam));
			break;
		case aidAddCfg:
			if (MainChartIsVarType()) return 0L;

			if (!CaseCheck(lParam)) return 0L;

			graph.wGKind = WORD(lParam);
			GetGrpFromDat((char*)&graph);

			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, ADD_AID_GRP), (long)&graph);
			break;
		case aidDelCfg:
			if (MainChartIsVarType()) return 0L;

			graph.wGKind = WORD(lParam);
			GetGrpFromDat((char*)&graph);
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, delAidGrp), (long)&graph);
			return 1L;
		case aidDelChart:
			if (m_pCfg)
				m_pCfg->DeleteChart(WORD(lParam));
			break;
		case unitInput:
			m_pwndSise->SendMessage(GEV_SISE, siseUnit, lParam);
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartUnit), lParam);
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlUnit), lParam);
			break;
		case indexInput:
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartIndex), lParam);
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlIndex), lParam);

			if (m_pEnvInfo)
				IndexChange(lParam);
			break;
		case inputAutotran:
			m_bAutotran = (lParam > 0)? true:false;
			if (m_pwndTool)	m_pwndTool->EnableWindow(!m_bAutotran);
			if (m_pwndCtrl)	m_pwndCtrl->EnableWindow(!m_bAutotran);
			if (m_pwndChart)	m_pwndChart->EnableWindow(!m_bAutotran);
			if (m_pCfg)	m_pCfg->EnableWindow(!m_bAutotran);
			break;
		case tableChart:
			ControlShow(PN_TABLE, (lParam)? true:false);
			if (m_pwndCtrl)	m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlTable), lParam);
			break;
		case tableCtrl:
			ControlShow(PN_TABLE, (lParam)? true:false);
			if (m_pwndChart)	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartTable), lParam);
			break;
		case gridChart:
			if (m_pwndCtrl)	m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlGrid), lParam);
			break;
		case gridCtrl:
			if (m_pwndChart)	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartGrid), lParam);
			break;
		case tipChart:
			if (m_pwndCtrl)	m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlTip), lParam);
			break;
		case tipCtrl:
			if (m_pwndChart)	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartTip), lParam);
			break;		
		case cfgMainChart:
			if (m_pwndInput)
				m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputEnv, inputMain), lParam);
			if (m_pwndCtrl)
				m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlMain), lParam);
			if (m_pwndChart)
				m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, CHANGE_MAIN_GRP), lParam);
			break;
		case clearInput:
			m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(siseData, siseClear), 0);
			m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, chartClear), 0);
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlClear), 0);
			break;
		case BookMarkCfg:
			{
				if (!m_pCfg)	return FALSE;

				LPCTSTR ulist = m_pCfg->GetUserGraph();
				if (ulist)
					m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, chartBookMark), long(ulist));
			}
			break;
		case mctrlAutotran:
			m_bAutotran = (lParam > 0)? true:false;
			if (m_pwndInput)	m_pwndInput->EnableWindow(!m_bAutotran);
			if (m_pwndTool)		m_pwndTool->EnableWindow(!m_bAutotran);
			if (m_pwndCtrl)		m_pwndCtrl->EnableWindow(!m_bAutotran);
			if (m_pwndChart)	m_pwndChart->EnableWindow(!m_bAutotran);
			break;
		}
		break;
	case viewShowWindow:
		{
			//** macho delete int	ctrlID = -1;
			//** macho add
			if (lParam == 9999) {
				return GetControlShow(HIWORD(wParam));
			}
			//** 
			bool	bShow = (lParam) ? true:false;

			switch (HIWORD(wParam))
			{
			case showSise:
				ControlShow(PN_SISE, bShow);
				break;
			case showTool:
				ControlShow(PN_TOOL, bShow);
				break;
			case showTable:
				ControlShow(PN_TABLE, bShow);
				break;
			case showCtrl:
				ControlShow(PN_CTRL, bShow);
				break;
			case showCfg:
				ControlShow(PN_CFG, bShow);
				break;
			case showAid:
				ControlShow(PN_AID, bShow);
				break;
			default:
				return 0;
			}
		}
		break;
	case viewInput:
		switch (HIWORD(wParam))
		{
		case getDisplayEdit:
			m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, getDispDay), lParam);
//			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlGetEdit), lParam);
			break;
		case setDisplayEdit:
			m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputEnv, inputDisplay), lParam);
//			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, CTRL_SET_EDIT), lParam);
			break;
		}
		break;
	case viewGroupList:
		SelectGroup((char*)lParam, HIWORD(wParam));
		break;
	}

	return TRUE;
}

LRESULT CGrpView::OnChartEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndChart)
		return m_pwndChart->SendMessage(GEV_CHART, wParam, lParam);
	return FALSE;
}

LRESULT CGrpView::OnInputEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndInput) {
		return m_pwndInput->SendMessage(GEV_INPUT, wParam, lParam);
	}
	return TRUE;
}

LRESULT CGrpView::OnSiseEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndSise)	return m_pwndSise->SendMessage(GEV_SISE, wParam, lParam);
	return FALSE;
}

LRESULT CGrpView::OnCtrlEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndCtrl)	return m_pwndCtrl->SendMessage(GEV_CTRL, wParam, lParam);
	return FALSE;
}

LRESULT CGrpView::OnToolEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndTool)
		return m_pwndTool->SendMessage(GEV_TOOL, wParam, lParam);
	return FALSE;
}

LRESULT CGrpView::OnCfgEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pCfg)	m_pCfg->SendMessage(GEV_CFG, wParam, lParam);
	return FALSE;
}

LRESULT CGrpView::OnTableEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndTable)	return m_pwndTable->SendMessage(GEV_TABLE, wParam, lParam);
	return FALSE;
}
/*
LRESULT CGrpView::OnInfoEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndInfo)	return m_pwndInfo->SendMessage(GEV_INFO, wParam, lParam);
	return FALSE;
}
*/
LRESULT CGrpView::OnAidEvent(WPARAM wParam, LPARAM lParam)
{
	if (m_pwndAid)
		return m_pwndAid->SendMessage(GEV_AID, wParam, lParam);
	return FALSE;
}

LRESULT CGrpView::OnSetKwansim(WPARAM wParam, LPARAM lParam)
{
	m_pwndKwansim = (CWnd*)wParam;
	return TRUE;
}

LRESULT CGrpView::OnFromMain(WPARAM wParam, LPARAM lParam)
{
//	CString strPrefix = CString();

	CString strEnvPath;
	strEnvPath.Format("%s%s\\%s%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, (char*)lParam, ENV);

	CString strGrpPath;
	strGrpPath.Format("%s%s\\%s%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, (char*)lParam, GRP);
	CString strDatPath;
	strDatPath.Format("%s%s\\%s%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, (char*)lParam, DAT);

	if (wParam == 0)	// load
	{
		if (ReadEnv(strEnvPath, true))	// 개별 저장정보
		{
			DelPanels();
			InitPnInfo(m_pEnvInfo->panel);
		}
		else	// 일잔 저장정보
		{
			strEnvPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, ENV);
			if (ReadEnv(strEnvPath, true))
			{
				DelPanels();
				InitPnInfo(m_pEnvInfo->panel);
			}
			else
				return -1;
		}
			
		if (!ReadDat(strDatPath))
		{
			strDatPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, DAT);
			if (!ReadDat(strDatPath))
				return -1;
		}
			
		if (!ReadGrp(strGrpPath))
		{
			strGrpPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, GRP);
			if (!ReadGrp(strGrpPath))
				return 2;
		}
		

		m_iDtIndex = m_pEnvInfo->datainfo.btIndex;
		int sIndex = m_iDtIndex - GI_DAY;
		int iRgnCnt = m_pEnvInfo->grpinfo[sIndex].wRgnCnt;
		int iGrpCnt = m_pEnvInfo->grpinfo[sIndex].wGrpCnt;
		int iBufSize = SZ_ENVINFO + iRgnCnt*SZ_REGION + iGrpCnt*SZ_GRAPH;
		char* pcBuf = new char[iBufSize];
		CopyMemory(pcBuf, m_pEnvInfo, SZ_ENVINFO);
		CopyMemory(&pcBuf[SZ_ENVINFO], m_pacRegion[sIndex], iRgnCnt*SZ_REGION);
		CopyMemory(&pcBuf[SZ_ENVINFO + iRgnCnt*SZ_REGION], m_pacGraph[sIndex], iGrpCnt*SZ_GRAPH);

		m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, chartReload), long(pcBuf));
		if (m_pCfg)
		{
			m_pCfg->ReloadGraph(m_pEnvInfo->datainfo.wMainGrpKind);

			_barChart	barChart;
			barChart.mkind = m_pEnvInfo->datainfo.wMainGrpKind;
			barChart.rgncnt = iRgnCnt;
			barChart.grpcnt = iGrpCnt;
			char	*pInfo = GetMapInfo();
			barChart.minfo = &pInfo[sz_INTERCHART];
			barChart.ulist = m_pEnvInfo->ulist;
			m_pCfg->ReloadGraph((char*)&barChart);
		}	
		m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputEnv, inputReload), long(m_pEnvInfo));
		m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlMain), m_pEnvInfo->datainfo.wMainGrpKind);
		m_pwndSise->SendMessage(GEV_SISE, siseUnit, m_pEnvInfo->datainfo.btUnit);
		m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, CHANGE_MAIN_GRP), m_pEnvInfo->datainfo.wMainGrpKind);

		CRect wRC;
		GetClientRect(wRC);
		ResizeWnd(wRC);

		delete pcBuf;

	}
	else if (wParam == 1)	// save
	{
		WriteEnv(strEnvPath, true);
		WriteDat(strDatPath);
		WriteGrp(strGrpPath);
		
	}

	return 0;
}

int CGrpView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//return 0;
	return Init();
}

int CGrpView::Init()
{
//	m_pMyTool = new CMyTool(m_pwndView);

	CString strEnvPath;
	strEnvPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, ENV);
	CString strGrpPath;
	strGrpPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, GRP);
	CString strDatPath;
	strDatPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, DAT);

	m_pEnvInfo = (_envInfo*)new char[SZ_ENVINFO];
	if (!ReadEnv(strEnvPath)) 
		return -1;
	if (!ReadGrp(strGrpPath))
		return -1;
	if (!ReadDat(strDatPath))
		return -1;

	_fontR rFont;
	rFont.name = "굴림";
	rFont.point = 9;
	rFont.italic = 0;
	rFont.bold = 0;
	m_pFont = (CFont *)m_pwndView->SendMessage(WM_USER, getFONT, long(&rFont));

	m_hiGComLib = LoadLibrary("axisGCom.dll");
	if (!m_hiGComLib)
	{
		m_pApp->MessageBox("axisGCom.dll LoadLibrary error", COMPANYNAME);
		return -1;
	}
	m_hiGMainLib = LoadLibrary("axisGMain.dll");
	if (!m_hiGMainLib)
	{
		m_pApp->MessageBox("axisGMain.dll LoadLibrary error", COMPANYNAME);
		return -1;
	}

	InitPnInfo(m_pEnvInfo->panel);
	CreatePn();

	if (m_pEnvInfo)
	{
		m_iOS_mKind = m_pEnvInfo->datainfo.wMainGrpKind;
		m_iOS_dUnit = m_pEnvInfo->datainfo.btUnit;
		m_iOS_dIndex = m_pEnvInfo->datainfo.btIndex;
	}

	m_pwndView->SendMessage(WM_USER, MAKEWPARAM(dropDLL, 0), NULL);

	CRect	crc;
	m_pwndView->GetClientRect(&crc);
	ResizeWnd(CRect(0, 0, crc.Width(), crc.Height()));

	if (m_pEnvInfo->display.dwDspOption & DO_STARTMAX)
		GetParent()->GetParent()->PostMessage(WM_SYSCOMMAND, SC_MAXIMIZE, 0);

	return 0;
}

void CGrpView::OnDestroy() 
{
	CString strEnvPath;
	strEnvPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, ENV);
	CString strGrpPath;
	strGrpPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, GRP);
	CString strDatPath;
	strDatPath.Format("%s%s\\%s%s", m_pApp->GetGexPath(), m_pApp->GetUName(), m_acDllName, DAT);

	WriteEnv(strEnvPath);
	WriteDat(strDatPath);
	WriteGrp(strGrpPath);

	/**
	if (m_pGroupList)
	{
		m_pGroupList->DestroyWindow();
		delete m_pGroupList;
		m_pGroupList = NULL;
	}
	**/

	if (m_pCfg)
	{
		m_pCfg->DestroyWindow();
		delete m_pCfg;
		m_pCfg = NULL;
	}

//	m_pwndView->SendMessage(WM_USER, MAKEWPARAM(revokeDLL, 0), NULL);

	CWnd::OnDestroy();
}

void CGrpView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	if (cx == 0 && cy == 0)
		return;

	ResizeWnd(CRect(0, 0, cx, cy));
}

void CGrpView::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_pwndChart)
		m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(chartViewFocus, 0), 0);
	if (m_pwndInput)
		m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputViewFocus, 0), 1);

	CWnd::OnSetFocus(pOldWnd);
}

void CGrpView::OnKillFocus(CWnd* pNewWnd) 
{
	if (m_pwndInput)
		m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputViewFocus, 0), 0);

	CWnd::OnKillFocus(pNewWnd);
}

BOOL CGrpView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

void CGrpView::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == TIMER_EXTR_TR)
	{
		KillTimer(TIMER_EXTR_TR);
		SendRequest2();
	}

	CWnd::OnTimer(nIDEvent);
}

void CGrpView::ResizeWnd(CRect wRC, bool bPrint)
{
	int cx = wRC.Width();
	int cy = wRC.Height();
	
	MoveWindow(0, 0, cx, cy);

	int iGabCfg = GetPnSize(PN_CFG);
	int iGabInput = GetPnSize(PN_INPUT);
	int iGabCtrl = GetPnSize(PN_CTRL);
	int iGabTool = GetPnSize(PN_TOOL);
	int iGabSise = GetPnSize(PN_SISE);
	int iGabTable = GetPnSize(PN_TABLE);
	int iGabAid = GetPnSize(PN_AID);

	/**
	if (m_pCfg)
		m_pCfg->MoveWindow(0, HEIGHT_INPUT, iGabCfg, cy - (HEIGHT_INPUT));
	if (m_pwndInput)
		m_pwndInput->MoveWindow(0, 0, cx, HEIGHT_INPUT);
	**/

	int	iLeft = iGabCfg;
	if (m_pCfg)
	{
		if (iGabCfg)
		{
			m_pCfg->MoveWindow(5, iGabInput + 5, iGabCfg, cy - iGabInput - 10);
			iLeft  = iGabCfg + 10;
		}
		else
		{
			m_pCfg->MoveWindow(0, iGabInput + 5, iGabCfg, cy - iGabInput);
		}
	}

	if (m_pwndInput)
		m_pwndInput->MoveWindow(0, 0, cx, HEIGHT_INPUT);
	

	if (m_pwndCtrl)
		m_pwndCtrl->MoveWindow(iLeft+5, cy - iGabCtrl - iGabTable-5, cx - iLeft - iGabTool - iGabAid-10, iGabCtrl);
	if (m_pwndTool)
		m_pwndTool->MoveWindow(cx - iGabTool - 5, iGabInput + iGabSise+3, iGabTool, cy - iGabInput - iGabSise - 8);
	if (m_pwndAid)
		m_pwndAid->MoveWindow(cx - iGabTool - iGabAid - 5, iGabInput + iGabSise+3, iGabAid, cy - iGabInput - iGabSise - 8);
	if (m_pwndSise)
		m_pwndSise->MoveWindow(iLeft+5, iGabInput+3, cx - iLeft-10, iGabSise);
	if (m_pwndChart)
	{
		CRect iRC;
		iRC.left = iLeft + 5;
		iRC.top = iGabInput + iGabSise+3;
		iRC.right = iRC.left + (cx - iLeft - iGabTool - iGabAid) - 10;
		iRC.bottom = cy - iGabCtrl - iGabTable-5;
		if (iGabSise+iGabInput && iRC.top < iGabSise + iGabInput)
			iRC.top = iGabSise + iGabInput;

		m_pwndChart->MoveWindow(iRC);
	}
	if (m_pwndTable)
	{
		CRect iRC;
		iRC.left = iLeft + 5;
		iRC.top = cy - iGabTable - 5;
		iRC.right = iRC.left + (cx - iLeft - iGabTool - iGabAid) - 10;
		iRC.bottom = iRC.top + iGabTable;
		if (HEIGHT_SISE+iGabInput && iRC.top < iGabSise + iGabInput)
			iRC.top = iGabSise + iGabInput;
		
		m_pwndTable->MoveWindow(iRC);
	}
	RedrawWindow();
}

int CGrpView::GetPnSize(int ctrlID)
{
	int size = 0;
	struct _pninfo *pninfo = NULL;
	if (!m_mapPanel.Lookup(ctrlID, pninfo))
		return size;

	if (pninfo->info & PO_USE)
	{
		if (pninfo->info & PO_SHOW)
			size = pninfo->whsize;
		else 
			size = 0;
	}
	return size;
}

void CGrpView::InitPnInfo(struct _panel *panel)
{
	struct _pninfo	*pninfo = NULL;
	int iPnID = 0;
	int iSize = 0;
	WORD info = 0;
	for (int ii = 0; ii < PANEL_CNT; ii++)
	{
		if ((iPnID = panel[ii].btPnID) == 0)
			continue;

		info = panel[ii].wPnOption;
		pninfo = new struct _pninfo();
		memset(pninfo, 0x00, sizeof(struct _pninfo));
		if (info & PO_USE)
		{
			switch (iPnID)
			{
			case PN_INPUT:
				iSize = HEIGHT_INPUT;
				break;
			case PN_SISE:
				iSize = HEIGHT_SISE;
				break;
			case PN_CTRL:
				iSize = HEIGHT_CTRL;
				break;
			case PN_TOOL:
				iSize = WIDTH_TOOL;
				break;
			case PN_CFG:
				iSize = WIDTH_CFG;
				break;
			case PN_TABLE:	iSize = HEIGHT_TABLE;	
				info &= ~PO_SHOW;
				panel[ii].wPnOption &= ~PO_SHOW;				
				break;
			case PN_AID:
				iSize = WIDTH_AID;
				break;
			}
		}
		else if (iPnID == PN_AID)	// conversion 정보에서 빠져서 PO_USE를 추가한것임
		{
			info |= PO_USE;
			iSize = WIDTH_AID;
		}

		pninfo->whsize = iSize;
		pninfo->info = info;
		panel[ii].wPnOption = info;
		m_mapPanel.SetAt(iPnID, pninfo);
	}
}

void CGrpView::CreatePn()
{
	m_pwndChart = CreateWnd(PN_CHART, m_pwndChart);

	struct _pninfo* ppninfo = GetPnInfo(PN_INPUT);
	if (ppninfo)
		m_pwndInput = CreateWnd(PN_INPUT, m_pwndInput);

////////////////////////
/*	m_pGroupList = new CGroupList(this, m_pFont, m_pApp->GetRoot(), m_pApp->GetUName());
	m_pGroupList->Create();

	m_pGroupList->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	m_pGroupList->ShowWindow(SW_HIDE);
*/
///////////////////////

	ppninfo = GetPnInfo(PN_SISE);
	if (ppninfo)
		m_pwndSise = CreateWnd(PN_SISE, m_pwndSise);

	ppninfo = GetPnInfo(PN_CTRL);
	if (ppninfo)
		m_pwndCtrl = CreateWnd(PN_CTRL, m_pwndCtrl);

	ppninfo = GetPnInfo(PN_TOOL);
	if (ppninfo)
		m_pwndTool = CreateWnd(PN_TOOL, m_pwndTool);
	
	ppninfo = GetPnInfo(PN_TABLE);
	if (ppninfo)
		m_pwndTable = CreateWnd(PN_TABLE, m_pwndTable);

//	ppninfo = GetPnInfo(PN_INFO);
//	if (ppninfo)
//		m_pwndInfo = CreateWnd(PN_INFO, m_pwndInfo);

	ppninfo = GetPnInfo(PN_CFG);
	if (ppninfo) {
		//m_pCfg = (CConfigBar *)CreateWnd(PN_CFG, m_pCfg);
		m_pCfg = (CConfigBarPru*)CreateWnd(PN_CFG, m_pCfg);
		m_pCfg->ChangePalette();
	}

	ppninfo = GetPnInfo(PN_AID);
	if (ppninfo)
		m_pwndAid = CreateWnd(PN_AID, m_pwndAid);

}

struct _pninfo* CGrpView::GetPnInfo(int localID)
{
	struct _pninfo *pninfo = NULL;
	if (!m_mapPanel.Lookup(localID, pninfo))
		return NULL;

	return pninfo;
}

CWnd* CGrpView::CreateWnd(int ctrlID, CWnd* pWnd)
{
	switch (ctrlID)
	{
	case PN_INPUT:
	case PN_SISE:
	case PN_CTRL:	
	case PN_TOOL:
//	case PN_INFO:
	case PN_TABLE:
	case PN_AID:
		{
			if (pWnd)
				return pWnd;
			struct _pninfo* pninfo = GetPnInfo(ctrlID);
			if (!pninfo || !(pninfo->info & PO_USE))
				break;
			CWnd*	(APIENTRY *axCreateCtrl)(int, CWnd *, CWnd *, char *, CFont*);
			axCreateCtrl = (CWnd* (APIENTRY *)(int, CWnd *, CWnd *, char *, CFont*))
				GetProcAddress(m_hiGComLib, "axCreateCtrl");
			pWnd = axCreateCtrl(ctrlID, m_pwndView, this, (char *)m_pEnvInfo, m_pFont);
			pWnd->MoveWindow(0, 0, 0, 0);

			return pWnd;
		}
		break;
	case PN_CFG:
		{
//			if (pWnd)
//				return pWnd;
//			struct _pninfo* pninfo = GetPnInfo(ctrlID);
//			if (!pninfo || !(pninfo->info & PO_USE))
//				break;
//			CWnd*	(APIENTRY *axCreateCtrl)(int, CWnd *, CWnd *, char *, CFont *);
//			axCreateCtrl = (CWnd* (APIENTRY *)(int, CWnd *, CWnd *, char *, CFont *))
//				GetProcAddress(m_hiGComLib, "axCreateCtrl");

			_barChart	barChart;
			barChart.mkind = m_pEnvInfo->datainfo.wMainGrpKind;
			char	*pInfo = GetMapInfo();
			_interChart	*interChart = (_interChart*)pInfo;
			barChart.rgncnt = interChart->rgncnt;
			barChart.grpcnt = interChart->grpcnt;
			barChart.minfo = &pInfo[sz_INTERCHART];
			barChart.ulist = m_pEnvInfo->ulist;

			pWnd = new CConfigBarPru(m_pwndView, this, m_pFont, (char*)&barChart);
			BOOL bCreate = ((CConfigBarPru*)pWnd)->Create(this, CRect(0, 0, 160, 300), 50000);
			pWnd->ShowWindow(SW_SHOW);

			return pWnd;
		}
		break;
	case PN_CHART:
		{
			if (pWnd)
				return pWnd;
			CWnd*	(APIENTRY* axCreateCtrl)(int, CWnd*, CWnd*, char*, CFont*);
			axCreateCtrl = (CWnd* (APIENTRY *)(int, CWnd*, CWnd*, char*, CFont*))
				GetProcAddress(m_hiGMainLib, "axCreateCtrl");
			pWnd = axCreateCtrl(GEV_CHART, m_pwndView, this, (char*)m_pEnvInfo, m_pFont);
			pWnd->MoveWindow(0, 0, 0, 0);

			return pWnd;
		}
		break;
	}
	return NULL;
}

void CGrpView::SendRequest()
{
	// debug : 20090915
	//CString strDebug;
	//strDebug.Format("CGrpView::SendRequest()");
	//AfxMessageBox(strDebug);

	if (m_bTransaction || m_bTransactionS)
		return;

	// debug
	//strDebug.Format("CGrpView::SendRequest() after if (m_bTransaction || m_bTransactionS)");
	//AfxMessageBox(strDebug);

	m_bTransaction = true;
	m_bTransactionS = false;

	m_iTRIndex = 0;
	ZeroMemory(&m_TrData, SZ_TRDATA);

	int iDataHLen;

	if (m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_HEADER2), long(&iDataHLen)))
		m_bExtrTr = true;
	else
		m_bExtrTr = false;

	ClearBuffer();

	int iInputLen = 0;

	// 20090915 : CHANGE : 심볼값 자리수가 커져 버퍼를 넓힘
	//char acInput[128];
	char acInput[256];

	if (m_pwndInput)
		iInputLen = m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_TR_INFO), (long)acInput);

	if (iInputLen == 0)
	{
		m_bTransaction = false;
		return;
	}

	// debug
	//strDebug.Format("CGrpView::SendRequest() after (iInputLen == 0)");
	//AfxMessageBox(strDebug);

	char* pcGDataH;
	if (m_pwndChart)
	{
		pcGDataH = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_HEADER), long(&iDataHLen));
	}
	
	int iTRLen = iInputLen + iDataHLen;
	m_pcTemp = AllocBuffer(iTRLen + L_userTH);

	_userTH* puserTH = (_userTH *)m_pcTemp;

	ZeroMemory(puserTH, L_userTH);
	_envInfo* penvinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	CopyMemory(puserTH->trc, GRP_HOOP, TRLEN);

	puserTH->stat = US_OOP;
	puserTH->key = (char)200;
	
	CopyMemory(&m_pcTemp[L_userTH], acInput, iInputLen);
	CopyMemory(&m_pcTemp[L_userTH + iInputLen], pcGDataH, iDataHLen);
// test io
//ofstream dFile ("C:\\glog.txt", ios::app);
//int rc = SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA,GET_TOTAL_DAY), 0);
//dFile	<<endl<<"s "<<rc<<" "<<GetTickCount()<<endl;
	

//#ifdef _DEBUG	
//WriteFile((char*)m_pcTemp);
//#endif

//	Debug(acInput);
	// debug : 20090915
	//strDebug.Format("CGrpView::SendRequest() before invokeTRx");
	//AfxMessageBox(strDebug);

	m_pwndView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, iTRLen), (long)m_pcTemp);

	// debug : 20090915
	//strDebug.Format("CGrpView::SendRequest() after invokeTRx");
	//AfxMessageBox(strDebug);
}


// request extra data
void CGrpView::SendRequest2()
{
	m_iTRIndex++;
	m_bExtrTr = false;

	ClearBuffer();

	int	inputLen = 0;
	char	inputStr[128];

	if (m_pwndInput)
		inputLen = m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, getTRInfo2), (long)inputStr);
	if (inputLen == 0)
	{
		m_bTransaction = false;
		return;
	}

	int	dataHLen;
	char	*gDataH;
	if (m_pwndChart)
	{
		gDataH = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_HEADER2), long(&dataHLen));
	}
	
	int	TRLen = inputLen + dataHLen;
	m_pcTemp = AllocBuffer(TRLen + L_userTH);

	_userTH *userTH = (_userTH *)m_pcTemp;
	ZeroMemory(userTH, L_userTH);

	_envInfo	*envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	
	CopyMemory(userTH->trc, GRP_HOOP, TRLEN);
	userTH->stat = US_OOP;
	userTH->key = char(200);
	
	CopyMemory(&m_pcTemp[L_userTH], inputStr, inputLen);
	CopyMemory(&m_pcTemp[L_userTH + inputLen], gDataH, dataHLen);

	m_pwndView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, TRLen), (long)m_pcTemp);
}

// request single data
void CGrpView::SendRequestS()
{
	m_iTRIndex = 0;

	ClearBuffer();

	m_bTransactionS = true;

	int	inputLen = 0;
	char	inputStr[128];
	if (m_pwndInput)
		inputLen = m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, getTRInfo2), (long)inputStr);
	if (inputLen == 0)
	{
		m_bTransactionS = false;
		return;
	}

	int	dataHLen;
	char	*gDataH;
	if (m_pwndChart)
	{
		gDataH = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, requestHeaderS), long(&dataHLen));
	}
	
	int	TRLen = inputLen + dataHLen;
	m_pcTemp = AllocBuffer(TRLen + L_userTH);

	_userTH *userTH = (_userTH *)m_pcTemp;
	ZeroMemory(userTH, L_userTH);

	_envInfo	*envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	CopyMemory(userTH->trc, GRP_HOOP, TRLEN);
	userTH->stat = US_OOP;
	userTH->key = char(200);
	
	CopyMemory(&m_pcTemp[L_userTH], inputStr, inputLen);
	CopyMemory(&m_pcTemp[L_userTH + inputLen], gDataH, dataHLen);

//#ifdef _DEBUG	
//WriteFile((char*)m_pcTemp);
//#endif
	m_pwndView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, TRLen), (long)m_pcTemp);
}

void CGrpView::DelPanels()
{
	struct _pninfo *pninfo = NULL;
	int pID = 0;
	POSITION pos;
	for (pos = m_mapPanel.GetStartPosition(); pos; )
	{
		m_mapPanel.GetNextAssoc(pos, pID, pninfo);
		BUF_DELETE(pninfo);
	}
	m_mapPanel.RemoveAll();
}

void CGrpView::SavePanel(struct _panel *panel)
{
	memset(panel, 0x00, SZ_PANEL * PANEL_CNT);
	struct _pninfo *pninfo = NULL;
	int localID = 0;
	int panelID = 0;
	POSITION pos;
	int ii = 0;
	for (pos = m_mapPanel.GetStartPosition(); pos; )
	{
		m_mapPanel.GetNextAssoc(pos, localID, pninfo);
		if (!pninfo)	continue;
		panel[ii].btPnID = localID;
		panel[ii].wPnOption = pninfo->info;
		ii++;
	}
}

char* CGrpView::GetDat()
{
	return m_pcGDat;
}

char* CGrpView::GetMapInfo(int iKey)
{
	ClearBuffer();

	int	index = m_iDtIndex - GI_DAY;
	int	rcnt = m_pEnvInfo->grpinfo[index].wRgnCnt;
	int	gcnt = m_pEnvInfo->grpinfo[index].wGrpCnt;
	struct _interChart ichart;
	if (iKey == -1)	// all
	{
		m_pcTemp = AllocBuffer(sz_INTERCHART + rcnt * SZ_REGION + gcnt * SZ_GRAPH);
		
		ichart.bAll = 1;
		ichart.grpcnt = gcnt;
		ichart.rgncnt = rcnt;
		
		CopyMemory(m_pcTemp, (char *)&ichart, sz_INTERCHART);
		CopyMemory(&m_pcTemp[sz_INTERCHART], m_pacRegion[index], rcnt * SZ_REGION);
		CopyMemory(&m_pcTemp[sz_INTERCHART + rcnt * SZ_REGION], m_pacGraph[index], gcnt * SZ_GRAPH);
	}
	else		// unique key
	{
		m_pcTemp = AllocBuffer(sz_INTERCHART + SZ_GRAPH);
		ichart.bAll = 0;
		ichart.grpcnt = 1;
		ichart.rgncnt = 0;
		for (int ii = 0; ii < gcnt; ii++)
		{
			struct _graph *pGrp = (struct _graph*)&m_pacGraph[index][ii * SZ_GRAPH];
			if (pGrp->wKey == iKey)
			{
				CopyMemory(m_pcTemp, &ichart, sz_INTERCHART);
				CopyMemory(&m_pcTemp[sz_INTERCHART], pGrp, SZ_GRAPH);
				break;
			}
		}

		if (ii >= gcnt)
			return NULL;
	}

	return m_pcTemp;
}

bool CGrpView::GetGrpFromDat(char *info)
{
	struct _graph	*graph = (struct _graph*)info;

	char	*pDat = GetDat();
	if (pDat == NULL)
		return false;

	struct _cntInfo *cntInfo = (struct _cntInfo *)pDat;
	int	count = cntInfo->wMainCnt + cntInfo->wAidCnt;
	for (int ii = 0; ii < count; ii++)
	{
		struct _graph *pGrp = (struct _graph*)&pDat[SZ_CNTINFO + SZ_GRAPH * ii];
		if (graph->wGKind == pGrp->wGKind)
		{
			CopyMemory(graph, pGrp, SZ_GRAPH);
			return true;
		}
	}
	return false;
}

bool CGrpView::SetGrpAtDat(char *info)
{
	struct _graph	*graph = (struct _graph*)info;

	char	*pDat = GetDat();
	if (pDat == NULL)
		return false;

	switch (graph->wGKind)
	{
	case GK_JPN:	case GK_BAR:	case GK_LIN:	case GK_POLE:
	case GK_EQV:	case GK_CDV:
	case GK_CLOCK:	case GK_PV:	case GK_THREE:	case GK_PNF:	case GK_SWING:
	case GK_KAGI:	case GK_RENKO:
	case GK_BALANCE:	case GK_AVOL:
		break;
	default:
		return false;
	}

	struct _cntInfo *cntInfo = (struct _cntInfo *)pDat;
	int	count = cntInfo->wMainCnt + cntInfo->wAidCnt;
	for (int ii = 0; ii < count; ii++)
	{
		struct _graph *pGrp = (struct _graph*)&pDat[SZ_CNTINFO + SZ_GRAPH*ii];
		if (graph->wGKind == pGrp->wGKind)
		{
			CopyMemory(pGrp, graph, SZ_GRAPH);
			return true;
		}
	}

	return false;
}

bool CGrpView::ReadEnv(CString strEnvPath, bool bLoadJCOD)
{
	memset(m_pEnvInfo, 0x00, SZ_ENVINFO);

	CSavedHead sh;
	DWORD dwRead;
	char* pcDat = sh.Read(strEnvPath, dwRead);
	
	if (!pcDat || dwRead < 1) {
		return false;
	}

	if (dwRead != SZ_SAVEDHEAD + SZ_ENVINFO)
		return false;

	if (sh.m_sh.dwVersion != VERSION_ENV)
		return false;

	if (*((struct _envInfo*)pcDat)->datainfo.acCode == NULL && bLoadJCOD)
		return false;

	CopyMemory(m_pEnvInfo, pcDat, SZ_ENVINFO);

	m_iDtIndex = m_pEnvInfo->datainfo.btIndex;
	for (int ii = 0; ii < MAX_INDEX; ii++)
	{
		m_aiOS_GrpCnt[ii] = m_pEnvInfo->grpinfo[ii].wGrpCnt;
		m_aiOS_RgnCnt[ii] = m_pEnvInfo->grpinfo[ii].wRgnCnt;
	}

	if (bLoadJCOD)
	{
		CString strJCOD = CString(m_pEnvInfo->datainfo.acCode, sizeof(m_pEnvInfo->datainfo.acCode));
		strJCOD.TrimRight();
		m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, ONLY_SET_CODE), (LPARAM)strJCOD.operator LPCTSTR());
	}

	return true;
}

bool CGrpView::ReadDat(CString strDatPath)
{
	CSavedHead sh;
	DWORD dwReadLen;

	char* pcDat = sh.Read(strDatPath, dwReadLen);
	if (!pcDat || dwReadLen < 1)
		return false;

	if (sh.m_sh.dwVersion != VERSION_DAT)
		return false;

	if (m_pcGDat)
		delete m_pcGDat;

	m_pcGDat = new char[sh.m_sh.dwDataLength];
	CopyMemory(m_pcGDat, pcDat, sh.m_sh.dwDataLength);

	// 20090928 : ADD : RSI 지표 추가
	CheckNewList();

	return true;
}

bool CGrpView::ReadGrp(CString strGrpPath)
{
	DWORD dwReadLen;
	CSavedHead sh;
	char* pcGrp = sh.Read(strGrpPath, dwReadLen);
	if (!pcGrp || dwReadLen < 1)
		return false;

	if (sh.m_sh.dwVersion != VERSION_GRP)
	{
		return false;
	}

	int iLen = 0;
	for (int ii = 0; ii < MAX_INDEX; ii++)
	{
		int iRCnt = m_pEnvInfo->grpinfo[ii].wRgnCnt;
		int iGCnt = m_pEnvInfo->grpinfo[ii].wGrpCnt;

		if (m_pacRegion[ii])
			BUF_DELETE(m_pacRegion[ii]);
		if (m_pacGraph[ii])
			BUF_DELETE(m_pacGraph[ii]);

		m_pacRegion[ii] = new char[iRCnt * SZ_REGION];
		m_pacGraph[ii] = new char[iGCnt * SZ_GRAPH];
		CopyMemory(m_pacRegion[ii], &pcGrp[iLen], iRCnt * SZ_REGION);
		iLen += iRCnt * SZ_REGION;
		CopyMemory(m_pacGraph[ii], &pcGrp[iLen], iGCnt * SZ_GRAPH);
		iLen += iGCnt * SZ_GRAPH;
	}

	return true;
}


bool CGrpView::WriteEnv(CString strEnvPath, bool bSaveJCOD)
{
	struct _envInfo* pEnvInfo = NULL;
	if (m_pwndChart)
		pEnvInfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	if (!pEnvInfo)
		return false;

	if (m_pwndInput)
		m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, getCurGapList), long(pEnvInfo->datainfo.acIGap));
	if (m_pCfg)
		CopyMemory(pEnvInfo->ulist, m_pCfg->GetUserGraph(), sizeof(pEnvInfo->ulist));

	CSavedHead sh;
	char* pcDat= sh.Make(VERSION_ENV, 1, SZ_ENVINFO);

	if (bSaveJCOD)
	{
		CString strJCOD = CString((char*)m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, GET_VALID_CODE), 0));
		if (!strJCOD.IsEmpty())
		{
			ZeroMemory(pEnvInfo->datainfo.acCode, sizeof(pEnvInfo->datainfo.acCode));
			CopyMemory(pEnvInfo->datainfo.acCode, strJCOD.operator LPCTSTR(), min(sizeof(pEnvInfo->datainfo.acCode), strJCOD.GetLength()));
		}
	}	

	if (pEnvInfo->wEnvOption & EO_SAVE)
	{
		CopyMemory(m_pEnvInfo, pEnvInfo, SZ_ENVINFO);
		if (!bSaveJCOD)
			pEnvInfo->datainfo.btUnit = m_iOS_dUnit;
		SavePanel(pEnvInfo->panel);
		CopyMemory(pcDat, pEnvInfo, SZ_ENVINFO);
		sh.Write(strEnvPath);
	}
	else if (pEnvInfo->wEnvOption & EO_SAVEALL)
	{
		CopyMemory(m_pEnvInfo, pEnvInfo, SZ_ENVINFO);
		SavePanel(pEnvInfo->panel);

		CopyMemory(pcDat, pEnvInfo, SZ_ENVINFO);
		sh.Write(strEnvPath);
	}
	else
	{
		struct _envInfo *pInfo = new _envInfo;

		CopyMemory(pInfo, pEnvInfo, SZ_ENVINFO);
		for (int ii = 0; ii < MAX_INDEX; ii++)
		{
			pInfo->grpinfo[ii].wGrpCnt = m_aiOS_GrpCnt[ii];
			pInfo->grpinfo[ii].wRgnCnt = m_aiOS_RgnCnt[ii];
		}
		pInfo->datainfo.wMainGrpKind = m_iOS_mKind;
		pInfo->datainfo.btUnit = m_iOS_dUnit;
		pInfo->datainfo.btIndex = m_iOS_dIndex;
		SavePanel(pInfo->panel);

		CopyMemory(pcDat, pInfo, SZ_ENVINFO);

		sh.Write(strEnvPath);
		delete pInfo;
		return false;
	}


	return true;
}

void CGrpView::WriteDat(CString strDatPath)
{
	if (!m_pcGDat)
		return;

	struct _cntInfo* cntInfo = (_cntInfo *)m_pcGDat;

	int iMainCnt = cntInfo->wMainCnt;
	int iAidCnt = cntInfo->wAidCnt;
	int iUserCnt = cntInfo->wUserCnt;
	int iSize = SZ_CNTINFO + (iMainCnt + iAidCnt + iUserCnt) * SZ_GRAPH;

	CSavedHead sh;
	char* pcDat = sh.Make(VERSION_DAT, 1, iSize);
	CopyMemory(pcDat, m_pcGDat, iSize);

	sh.Write(strDatPath);
}

void CGrpView::WriteGrp(CString strGrpPath)
{
	if (!m_pwndChart)
		return;

	int iTotalSize = 0;
	int iIndex = m_iDtIndex - GI_DAY;
	int iRgnCnt = m_pEnvInfo->grpinfo[iIndex].wRgnCnt;
	int iGrpCnt = m_pEnvInfo->grpinfo[iIndex].wGrpCnt;

	for (int ii = 0; ii < MAX_INDEX; ii++)
		iTotalSize += m_pEnvInfo->grpinfo[ii].wRgnCnt * SZ_REGION + m_pEnvInfo->grpinfo[ii].wGrpCnt * SZ_GRAPH;

	char* pcBuf = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_MAP), 0);
	if (!pcBuf)
		return;
		
	if (m_pacRegion[iIndex])
		BUF_DELETE(m_pacRegion[iIndex]);
	if (m_pacGraph[iIndex])
		BUF_DELETE(m_pacGraph[iIndex]);

	m_pacRegion[iIndex] = new char[iRgnCnt * SZ_REGION];
	m_pacGraph[iIndex] = new char[iGrpCnt * SZ_GRAPH];
	CopyMemory(m_pacRegion[iIndex], pcBuf, iRgnCnt * SZ_REGION);
	CopyMemory(m_pacGraph[iIndex], &pcBuf[iRgnCnt * SZ_REGION], iGrpCnt * SZ_GRAPH);

	CSavedHead sh;
	char* pcDat = sh.Make(VERSION_GRP, 1, iTotalSize);
	int iLen = 0;
	for (ii = 0; ii < MAX_INDEX; ii++)
	{
		iRgnCnt = m_pEnvInfo->grpinfo[ii].wRgnCnt;
		iGrpCnt = m_pEnvInfo->grpinfo[ii].wGrpCnt;
		CopyMemory(&pcDat[iLen], m_pacRegion[ii], iRgnCnt * SZ_REGION);

		iLen += iRgnCnt * SZ_REGION;
		CopyMemory(&pcDat[iLen], m_pacGraph[ii], iGrpCnt * SZ_GRAPH);

		iLen += iGrpCnt * SZ_GRAPH;
	}

	sh.Write(strGrpPath);
}


// text : point.x \t point.y \n SYM \t DATA (\t ...)
// LOBYTE(HIWORD(WPARAM)) : source window ID
// HIBYTE(HIWORD(WPARAM)) : destination window ID
void CGrpView::OnDrop(WPARAM wParam, LPARAM lParam)
{
	CPoint	point;
	int	index;
	CString dropS, str;

	dropS = (char *)lParam;
	index = dropS.Find("\t");
	if (index == -1)
		return;
	str = dropS.Left(index);
	point.x = atoi(str);

	dropS = dropS.Mid(++index);
	index = dropS.Find("\n");
	if (index == -1)
		return;
	str = dropS.Left(index);
	point.y = atoi(str);

	dropS = dropS.Mid(++index);
	index = dropS.Find("\t");
	if (index == -1)
		return;

	CString	strSymbol = dropS.Left(index);
	CString	strCode = dropS.Mid(index+1);
	if (strSymbol == _T("301"))
		strSymbol = _T("1301");
	dropS.Format("%s\t%s", strSymbol, strCode);

	if (m_pwndInput && !m_bAutotran)
	{
		if (m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, triggerCode), long(dropS.operator LPCTSTR())))
		{
			m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), long(dropS.operator LPCTSTR()));
			m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, codeCC), long(dropS.operator LPCTSTR()));
		}
	}
}

char* CGrpView::OpenCodeDlg(int kind, UINT lParam)
{
	ClearBuffer();

	int dKind;
	switch (kind)
	{
	case dlgJCode:
		dKind = kospiCODE;
		break;
	case dlgUCode:
		dKind = indexCODE;
		dKind = MAKEWORD(indexCODE, 99);
		break;
	case dlgFCode:
		dKind = futureCODE;
		break;
	case dlgOCode:
		dKind = optionCODE;
		break;
	case dlgJUCode:
		dKind = allCODE;
		break;
	case dlgFOCode:
		dKind = foptionCODE;
		break;
	// 20090907 : ADD
	case dlgCommoditiesFutureCode:
	//	dKind = commodityCODE;   //파생상품 코드개편 
		break;
	default:
		dKind = usCODE;
		break;
	}
/*
Kind : LOBYTE(HIWORD(wParam))
Type : HIBYTE(HIWORD(wParam))
Point.x : LOWORD(lParam)
Point.y : HIWORD(lParam)
*/
	return (char*) m_pwndView->SendMessage(WM_USER, MAKEWPARAM(codeDLL, dKind), lParam);
}

bool CGrpView::CallEnvDlg(char *pInfo, bool bTotal)
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	bool bRet;
	if (bTotal)
	{
		char*	(APIENTRY *axTotalConfig)(CWnd *, DWORD, char *, char *, char *);
		axTotalConfig = (char* (APIENTRY *)(CWnd *, DWORD, char *, char *, char *)) GetProcAddress(m_hiGDlgLib, "axTotalConfig");
		if (!axTotalConfig)
			return false;

		_envInfo *envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
		SavePanel(envinfo->panel);

////////////////////////////// get screen info
		ClearBuffer();

		m_pcTemp = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, requestCurInter), 0);
		if (m_pcTemp == NULL)
			return false;
/////////////////////////////
		DWORD tkColor = m_pwndView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), 4);
		bool	bSiseShow = GetPnSize(PN_SISE)? true:false;

		char *pScreen = (char *)axTotalConfig(this, tkColor, (char*)envinfo, m_pcGDat, m_pcTemp);

		ClearBuffer();

		if (pScreen)
		{
			struct _pninfo	*pninfo = NULL;
			WORD info = 0;
			for (int ii = 0; ii < PANEL_CNT; ii++)
			{
				if (envinfo->panel[ii].btPnID == 0)
					continue;

				info = envinfo->panel[ii].wPnOption;
				if (info & PO_USE && envinfo->panel[ii].btPnID == PN_SISE)
				{
					if ((info & PO_SHOW && !bSiseShow) || (!(info & PO_SHOW) && bSiseShow))
						ControlShow(PN_SISE);

					break;
				}
			}

			SyncMap(pScreen);
			return true;
		}
		else 
			return false;
	}
	else
	{
		bool	(APIENTRY *axIndcConfig)(CWnd *, DWORD, char *, char *, char *);
		axIndcConfig = (bool (APIENTRY *)(CWnd *, DWORD, char *, char *, char *)) GetProcAddress(m_hiGDlgLib, "axIndcConfig");
		if (!axIndcConfig)
			return false;

		_envInfo *envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
		DWORD tkColor = m_pwndView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), 4);
		bRet = axIndcConfig(this, tkColor, (char*)envinfo, m_pcGDat, pInfo);

		if (bRet)
			SetGrpAtDat(pInfo);
	}

	return bRet;
}

char* CGrpView::CallMapDlg(char *pInfo)
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	char*	(APIENTRY *axScreenConfig)(CWnd*, DWORD, char*, char *, char*);
	axScreenConfig = (char* (APIENTRY *)(CWnd*, DWORD, char*, char *, char*)) GetProcAddress(m_hiGDlgLib, "axScreenConfig");
	if (!axScreenConfig)
		return NULL;

	_envInfo *envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	DWORD tkColor = m_pwndView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), 4);
	return axScreenConfig(this, tkColor, (char*)envinfo, m_pcGDat, pInfo);
}

bool CGrpView::CallStandardIndc(int kind)
{
	struct _cntInfo *cntInfo = (struct _cntInfo *)m_pcGDat;
	int	count = cntInfo->wMainCnt + cntInfo->wAidCnt;
	for (int jj = 0; jj < count; jj++)
	{
		struct _graph *pGrp = (struct _graph*)&m_pcGDat[SZ_CNTINFO + SZ_GRAPH*jj];
		if (kind == pGrp->wGKind)
		{
			CallEnvDlg((char *)pGrp, false);

			return true;
		}
	}

	return false;
}

bool CGrpView::CallToolDlg(char *pInfo)
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	bool	(APIENTRY *axToolConfig)(CWnd *, char *);
	axToolConfig = (bool (APIENTRY *)(CWnd *, char *)) GetProcAddress(m_hiGDlgLib, "axToolConfig");
	if (!axToolConfig)
		return false;

	return axToolConfig(this, pInfo);
}

void CGrpView::CallSaveDlg()
{
	CString	listDir = m_pApp->GetGexPath() + m_pApp->GetUName();
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(listDir, &findFileData);
	if (hFind == INVALID_HANDLE_VALUE)
		CreateDirectory(listDir, NULL);

	//CString	dllName = SAVE_TOTAL_NAME; // m_acDllName;
	CString	dllName = m_acDllName;
	CString	listPath = listDir + "\\*" + USV;
	CStringArray	saveList;
	CFileIO::GetFilenameInPath(listPath, dllName, saveList);

	CString	fList = _T("");
	if (saveList.GetSize() > 0)
	{
		CString	name;
		int	index;
		for (int ii = 0; ii < saveList.GetSize(); ii++)
		{
			name = saveList.GetAt(ii);
			//name.MakeLower();
			index = name.Find(USV);
			if (index != -1)
				name = name.Left(index);
			fList += name + "\t";
		}
	}

	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return;
		}
	}

	bool	(APIENTRY *axSaveFrame)(CWnd*, char *, char *, char *);
	axSaveFrame = (bool (APIENTRY *)(CWnd*, char *, char *, char *)) GetProcAddress(m_hiGDlgLib, "axSaveFrame");
	if (!axSaveFrame)
		return;

	char * pName = new char[65];
	sprintf(pName, "%s_%s", dllName, USV);
	CString	Path = m_pApp->GetGexPath() + m_pApp->GetUName() + "\\";
	int result = axSaveFrame(this, pName, (char*)fList.operator LPCTSTR(), (char*)Path.operator LPCTSTR());

	if (strlen(pName) <= 0)
	{
		delete[] pName;
		return;
	}
	
	switch (result)
	{
	case 1:	// save
		SaveGraph(Path + dllName + "_" + pName + USV);
		break;
	case 2:	// load
		LoadGraph(Path + dllName + "_" + pName + USV);
		if (m_pwndCtrl)
			m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlReload), (long)m_pEnvInfo);
		break;
	default:
		break;
	}

	delete[] pName;
}

bool CGrpView::SaveGraph(CString strPath)
{
	char* pcGraphInfo = GetDat();
	_envInfo* pEnvInfo = (_envInfo *)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	if (m_pwndInput)
		m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_DATA, getCurGapList), long(pEnvInfo->datainfo.acIGap));
	if (m_pCfg)
		CopyMemory(pEnvInfo->ulist, m_pCfg->GetUserGraph(), sizeof(pEnvInfo->ulist));

	SavePanel(pEnvInfo->panel);
	char* pcGrp = (char *)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_MAP), 0);

	_cntInfo* pCntInfo = (_cntInfo*)pcGraphInfo;
	int iInfoSize = SZ_CNTINFO + (pCntInfo->wMainCnt + pCntInfo->wAidCnt) * SZ_GRAPH;
	int iBufSize = SZ_SAVEINFO + SZ_ENVINFO + iInfoSize;
	for (int ii = 0; ii < MAX_INDEX; ii++)
	{
		iBufSize += pEnvInfo->grpinfo[ii].wRgnCnt * SZ_REGION + pEnvInfo->grpinfo[ii].wGrpCnt * SZ_GRAPH;
	}

	CSavedHead sh;
	char* pcBuf = sh.Make(VERSION_USV , 1, iBufSize);
	
	_saveinfo* pSaveinfo = (_saveinfo *)pcBuf;

	// header copy
	CopyMemory(&pSaveinfo->cntinfo, pCntInfo, SZ_CNTINFO);
	for (ii = 0; ii < MAX_INDEX; ii++)
	{
		pSaveinfo->agrpinfo[ii].wRgnCnt = pEnvInfo->grpinfo[ii].wRgnCnt;
		pSaveinfo->agrpinfo[ii].wGrpCnt = pEnvInfo->grpinfo[ii].wGrpCnt;
	}	

	CopyMemory(&pcBuf[SZ_SAVEINFO], pEnvInfo, SZ_ENVINFO);	// pEnvInfo copy
	int iLen = SZ_SAVEINFO + SZ_ENVINFO;
	int iSIndex = m_iDtIndex - GI_DAY;
	for (ii = 0; ii < MAX_INDEX; ii++)			// map config copy
	{
		if (iSIndex == ii)
		{
			CopyMemory(&pcBuf[iLen], pcGrp, pEnvInfo->grpinfo[ii].wRgnCnt * SZ_REGION + pEnvInfo->grpinfo[ii].wGrpCnt * SZ_GRAPH);
			iLen += pEnvInfo->grpinfo[ii].wRgnCnt * SZ_REGION + pEnvInfo->grpinfo[ii].wGrpCnt * SZ_GRAPH;
		}
		else
		{
			CopyMemory(&pcBuf[iLen], m_pacRegion[ii], pEnvInfo->grpinfo[ii].wRgnCnt * SZ_REGION);
			iLen += pEnvInfo->grpinfo[ii].wRgnCnt * SZ_REGION;
			CopyMemory(&pcBuf[iLen], m_pacGraph[ii], pEnvInfo->grpinfo[ii].wGrpCnt * SZ_GRAPH);
			iLen += pEnvInfo->grpinfo[ii].wGrpCnt * SZ_GRAPH;
		}
	}
	CopyMemory(&pcBuf[iLen], pcGraphInfo, iInfoSize);		// dat copy

	if (!sh.Write(strPath))
	{
		TRACE("SaveGraph() Fail\n");
		return false;
	}

	return true;
}

bool CGrpView::LoadGraph(CString strPath)
{
	CSavedHead sh;
	DWORD dwReadLen;
	char* pcBuf = sh.Read(strPath, dwReadLen);
	if (!pcBuf || dwReadLen < 1)
	{
		TRACE("LoadGraph() Fail\n");
		return false;
	}

	_saveinfo saveinfo;
	CopyMemory(&saveinfo, pcBuf, SZ_SAVEINFO);

	int iMapSize = SZ_ENVINFO;
	for (int ii = 0; ii < MAX_INDEX; ii++)
		iMapSize += saveinfo.agrpinfo[ii].wRgnCnt * SZ_REGION + saveinfo.agrpinfo[ii].wGrpCnt * SZ_GRAPH;
	int iDatSize = SZ_CNTINFO + (saveinfo.cntinfo.wMainCnt + saveinfo.cntinfo.wAidCnt) * SZ_GRAPH;
	int iBufSize = SZ_SAVEINFO + iMapSize + iDatSize;

	if (iBufSize != (int)sh.m_sh.dwDataLength)
	{
		TRACE("LoadGraph() Fail\n");
		return false;
	}

	for (ii = 0; ii < MAX_INDEX; ii++)
	{
		if (m_pacRegion[ii])
			BUF_DELETE(m_pacRegion[ii]);
		if (m_pacGraph[ii])
			BUF_DELETE(m_pacGraph[ii]);
	}

	ClearBuffer();


	if (m_pcGDat)
		BUF_DELETE(m_pcGDat);
	m_pcGDat = new char[iDatSize];
	CopyMemory(m_pcGDat, &pcBuf[SZ_SAVEINFO + iMapSize], iDatSize);

	// 20090928 : ADD : RSI 지표 추가
	CheckNewList();

	_envInfo* pEnvInfo = (_envInfo*)&pcBuf[SZ_SAVEINFO];
	CopyMemory(m_pEnvInfo, pEnvInfo, SZ_ENVINFO);
	DelPanels();
	InitPnInfo(pEnvInfo->panel);

	int iRCnt;
	int iGCnt;
	int iLen = SZ_SAVEINFO + SZ_ENVINFO;
	for (ii = 0; ii < MAX_INDEX; ii++)
	{
		iRCnt = m_pEnvInfo->grpinfo[ii].wRgnCnt;
		iGCnt = m_pEnvInfo->grpinfo[ii].wGrpCnt;
		m_pacRegion[ii] = new char[iRCnt * SZ_REGION];
		m_pacGraph[ii] = new char[iGCnt * SZ_GRAPH];

		CopyMemory(m_pacRegion[ii], &pcBuf[iLen], iRCnt * SZ_REGION);
		iLen += iRCnt * SZ_REGION;
		CopyMemory(m_pacGraph[ii], &pcBuf[iLen], iGCnt * SZ_GRAPH);
		iLen += iGCnt * SZ_GRAPH;
	}

	m_iDtIndex = m_pEnvInfo->datainfo.btIndex;
	int iSIndex = m_iDtIndex - GI_DAY;
	iRCnt = m_pEnvInfo->grpinfo[iSIndex].wRgnCnt;
	iGCnt = m_pEnvInfo->grpinfo[iSIndex].wGrpCnt;
	iBufSize = SZ_ENVINFO + iRCnt * SZ_REGION + iGCnt * SZ_GRAPH;
	pcBuf = new char[iBufSize];
	CopyMemory(pcBuf, m_pEnvInfo, SZ_ENVINFO);
	CopyMemory(&pcBuf[SZ_ENVINFO], m_pacRegion[iSIndex], iRCnt * SZ_REGION);
	CopyMemory(&pcBuf[SZ_ENVINFO + iRCnt * SZ_REGION], m_pacGraph[iSIndex], iGCnt * SZ_GRAPH);

	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, chartReload), long(pcBuf));
	if (m_pCfg)
	{
		m_pCfg->ReloadGraph(m_pEnvInfo->datainfo.wMainGrpKind);

		_barChart barChart;
		barChart.mkind = m_pEnvInfo->datainfo.wMainGrpKind;
		barChart.rgncnt = iRCnt;
		barChart.grpcnt = iGCnt;
		char* pInfo = GetMapInfo();
		barChart.minfo = &pInfo[sz_INTERCHART];
		barChart.ulist = m_pEnvInfo->ulist;
		m_pCfg->ReloadGraph((char*)&barChart);
	}

	m_pwndInput->SendMessage(GEV_INPUT, MAKEWPARAM(inputEnv, inputReload), long(m_pEnvInfo));
	m_pwndCtrl->SendMessage(GEV_CTRL, MAKEWPARAM(CTRL_ENV, ctrlMain), m_pEnvInfo->datainfo.wMainGrpKind);
	m_pwndSise->SendMessage(GEV_SISE, siseUnit, m_pEnvInfo->datainfo.btUnit);
	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, CHANGE_MAIN_GRP), m_pEnvInfo->datainfo.wMainGrpKind);

	CRect wRC;
	GetClientRect(wRC);
	ResizeWnd(wRC);

	delete pcBuf;
	return true;
}

void CGrpView::EnvProc()
{
	if (CallEnvDlg(NULL, true))
	{
		m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, CHANGE_ENV_INFO));

		_envInfo *envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
		m_pwndSise->SendMessage(GEV_SISE, MAKEWPARAM(siseChgEnv, 0), envinfo->display.rgbBG);
	}
}

void CGrpView::ToolEnvProc()
{
	CMenu cMenu;
	if (m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_ENV, chartIsValid), 0))
	{
		cMenu.CreatePopupMenu();
		if (GetControlShow(showSise))
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, 996, "시세바 보이기");
		else
			cMenu.AppendMenu(MFT_STRING, 996, "시세바 보이기");
		if (GetControlShow(showTool))
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, 995, "도구바 보이기");
		else
			cMenu.AppendMenu(MFT_STRING, 995, "도구바 보이기");
		if (GetControlShow(showAid))
			cMenu.AppendMenu(MFT_STRING|MF_CHECKED, 994, "지표바 보이기");
		else
			cMenu.AppendMenu(MFT_STRING, 994, "지표바 보이기");

		cMenu.AppendMenu(MFT_STRING, 993, "도구바 설정");
		cMenu.AppendMenu(MFT_STRING, 992, "지표바 설정");
	}
	else
	{
		return;
	}

	CPoint	point;
	GetCursorPos(&point);
	int menuID = cMenu.TrackPopupMenu(TPM_RIGHTBUTTON|TPM_RETURNCMD, point.x, point.y, this, NULL);
	int	aKind = 0;
	switch (menuID)
	{
	case 996:
		SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showSise), !GetControlShow(showSise));
		break;
	case 995:
		SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showTool), !GetControlShow(showTool));
		break;
	case 994:
		SendMessage(GEV_VIEW, MAKEWPARAM(viewShowWindow, showAid), !GetControlShow(showAid));
		break;
	case 993:
		if (m_pwndTool)	m_pwndTool->PostMessage(GEV_TOOL, MAKEWPARAM(TOOL_CONFIG, 0), NULL);
		break;
	case 992:
		if (m_pwndAid)	m_pwndAid->PostMessage(GEV_AID, MAKEWPARAM(AID_CONFIG, 0), NULL);
		break;
	}
}

void CGrpView::PrintProc()
{
	CPrintDlg	dlg;
	if (dlg.DoModal() == IDOK)
	{
		CAxisGViewApp	*pApp = (CAxisGViewApp*)AfxGetApp();
		if (dlg.m_select == PRINT_CHART)
		{
			CString	title = (char*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, getPrintTitle), 0);
			CFileIO::PrintWindow(pApp->GetRoot(), this, m_pwndChart, title, m_pFont, true);
		}
		else
		{
			CFileIO::PrintWindow(pApp->GetRoot(), this->GetParent()->GetParent(), true);
		}
	}
}

bool CGrpView::GetControlShow(int iID)
{
	int id;
	switch (iID) {
		case showSise: id = PN_SISE; break;
		case showTool: id = PN_TOOL; break;
		case showTable:id = PN_TABLE;break;
		case showCtrl: id = PN_CTRL; break;
		case showCfg:  id = PN_CFG;  break;
		case showAid:  id = PN_AID;  break;
		default:
			return false;
	}

	struct _pninfo *pninfo;

	if (!m_mapPanel.Lookup(id, pninfo))
		return false;

	return (bool)(0 != (pninfo->info & PO_SHOW));
}


bool CGrpView::ControlShow(int iID, bool bShow)
{
	struct _pninfo *pninfo;

	if (!m_mapPanel.Lookup(iID, pninfo))
		return false;

//** macho modify --;
//**
	switch (iID)
	{
	case PN_SISE:
	case PN_TOOL:
	case PN_CFG:
		if (pninfo->info & PO_SHOW)
			pninfo->info &= ~PO_SHOW;
		else
			pninfo->info |= PO_SHOW;
		break;
	case PN_TABLE:
	case PN_AID:
		if (bShow)
			pninfo->info |= PO_SHOW;
		else
			pninfo->info &= ~PO_SHOW;
		break;
	default:
		return false;
	}
//**

	CRect	wRC;
	GetWindowRect(&wRC);
	ResizeWnd(wRC);

	return true;
}

void CGrpView::SyncMainChart(char *pInfo)
{
	_interChart	*iChart = (_interChart*)pInfo;
	for (int ii = 0; ii < iChart->grpcnt; ii++)
	{
		_graph *graph = (_graph *)&pInfo[sz_INTERCHART + SZ_REGION*iChart->rgncnt + SZ_GRAPH*ii];
		switch (graph->wGKind)
		{
		case GK_JPN:
		case GK_LIN:
		case GK_BAR:
		case GK_POLE:
		case GK_BALANCE:
		case GK_AVOL:
		case GK_EQV:
		case GK_CDV:
		case GK_PV:
		case GK_CLOCK:
		case GK_THREE:
		case GK_PNF:
		case GK_SWING:
		case GK_KAGI:
		case GK_RENKO:
			{
				struct _cntInfo *cntInfo = (struct _cntInfo *)m_pcGDat;
				int	count = cntInfo->wMainCnt + cntInfo->wAidCnt;
				for (int jj = 0; jj < count; jj++)
				{
					struct _graph *pGrp = (struct _graph*)&m_pcGDat[SZ_CNTINFO + SZ_GRAPH*jj];
					if (graph->wGKind == pGrp->wGKind)
					{
						CopyMemory(pGrp, graph, SZ_GRAPH);
						return;
					}
				}

			}
			break;
		default:
			break;
		}
	}
}

void CGrpView::SyncMap(char *pScreen)
{
	_interChart	*iChart = (_interChart*)pScreen;
	int	bufSize = sz_INTERCHART + SZ_REGION*iChart->rgncnt + SZ_GRAPH*iChart->grpcnt;
	m_pcTemp = AllocBuffer(bufSize);
	CopyMemory(m_pcTemp, pScreen, bufSize);

	SyncMainChart(m_pcTemp);
	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, changeMapInfo), long(m_pcTemp));

	ClearBuffer();

	_envInfo *envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	switch (envinfo->datainfo.wMainGrpKind)
	{
	case GK_JPN:
	case GK_BAR:
	case GK_POLE:
	case GK_LIN:
	case GK_AVOL:
	case GK_BALANCE:
		{
			int	sIndex = m_iDtIndex - GI_DAY;
			m_pEnvInfo->grpinfo[sIndex].wRgnCnt = iChart->rgncnt;
			m_pEnvInfo->grpinfo[sIndex].wGrpCnt = iChart->grpcnt;
			if (m_pacRegion[sIndex])
				delete m_pacRegion[sIndex];
			if (m_pacGraph[sIndex])
				delete m_pacGraph[sIndex];
			m_pacRegion[sIndex] = new char[iChart->rgncnt*SZ_REGION];
			m_pacGraph[sIndex] = new char[iChart->grpcnt*SZ_GRAPH];
			CopyMemory(m_pacRegion[sIndex], &pScreen[sz_INTERCHART], SZ_REGION*iChart->rgncnt);
			CopyMemory(m_pacGraph[sIndex], &pScreen[sz_INTERCHART+SZ_REGION*iChart->rgncnt], SZ_GRAPH*iChart->grpcnt);

			_barChart	barChart;
			barChart.mkind = m_pEnvInfo->datainfo.wMainGrpKind;
			barChart.rgncnt = BYTE(m_pEnvInfo->grpinfo[sIndex].wRgnCnt);
			barChart.grpcnt = BYTE(m_pEnvInfo->grpinfo[sIndex].wGrpCnt);
			char	*pInfo = GetMapInfo();
			barChart.minfo = &pInfo[sz_INTERCHART];
			CopyMemory(m_pEnvInfo->ulist, m_pCfg->GetUserGraph(), sizeof(m_pEnvInfo->ulist));
			barChart.ulist = m_pEnvInfo->ulist;
			m_pCfg->ReloadGraph((char*)&barChart);			
		}
		break;
	default:
		break;
	}

	delete pScreen;
}

void CGrpView::IndexChange(LPARAM lParam)
{
	int index = LOWORD(lParam);
	int gap = HIWORD(lParam);

	//Debug("Index Change = %d, %d", index, gap);

	if (m_iDtIndex == index && gap > 0) 
	{
		//Debug("Index Change Return");
		return;
	}
///////////// old info save

	struct _envInfo	*envinfo = NULL;
	if (m_pwndChart)
		envinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	char *buf = (char *)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_MAP), 0);
	if (!buf)
		return;
		
	CopyMemory(m_pEnvInfo, envinfo, SZ_ENVINFO);

	int	sIndex = m_iDtIndex - GI_DAY;
	int	rcnt = m_pEnvInfo->grpinfo[sIndex].wRgnCnt;
	int	gcnt = m_pEnvInfo->grpinfo[sIndex].wGrpCnt;

	if (m_pacRegion[sIndex])
		BUF_DELETE(m_pacRegion[sIndex]);
	if (m_pacGraph[sIndex])
		BUF_DELETE(m_pacGraph[sIndex]);

	m_pacRegion[sIndex] = new char[rcnt*SZ_REGION];
	m_pacGraph[sIndex] = new char[gcnt*SZ_GRAPH];
	CopyMemory(m_pacRegion[sIndex], buf, rcnt*SZ_REGION);
	CopyMemory(m_pacGraph[sIndex], &buf[rcnt*SZ_REGION], gcnt*SZ_GRAPH);

///////////// new index info set
	m_iDtIndex = index;
	sIndex = m_iDtIndex - GI_DAY;
	rcnt = m_pEnvInfo->grpinfo[sIndex].wRgnCnt;
	gcnt = m_pEnvInfo->grpinfo[sIndex].wGrpCnt;

	int	totalsize = rcnt*SZ_REGION + gcnt*SZ_GRAPH + SZ_ENVINFO;
	char	*newBuf = new char[totalsize];

	CopyMemory(newBuf, m_pEnvInfo, SZ_ENVINFO);
	CopyMemory(&newBuf[SZ_ENVINFO], m_pacRegion[sIndex], rcnt*SZ_REGION);
	CopyMemory(&newBuf[SZ_ENVINFO + rcnt*SZ_REGION], m_pacGraph[sIndex], gcnt*SZ_GRAPH);

	m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_SET_GRP, chartIReload), long(newBuf));
	delete [] newBuf;

	if (m_pCfg)
	{
		_barChart	barChart;
		barChart.mkind = m_pEnvInfo->datainfo.wMainGrpKind;
		barChart.rgncnt = rcnt;
		barChart.grpcnt = gcnt;
		char	*pInfo = GetMapInfo();
		barChart.minfo = &pInfo[sz_INTERCHART];
		CopyMemory(m_pEnvInfo->ulist, m_pCfg->GetUserGraph(), sizeof(m_pEnvInfo->ulist));
		barChart.ulist = m_pEnvInfo->ulist;
		m_pCfg->ReloadGraph((char*)&barChart);
	}	
}

void CGrpView::RemoveCodeList()
{
	for (int ii = 0; ii < m_arCode.GetSize(); ii++)
	{
		struct _codeinfo *codeinfo = m_arCode.GetAt(ii);
		delete codeinfo;
		codeinfo = NULL;
	}
	m_arCode.RemoveAll();
}

void CGrpView::InitCodeList()
{
	CString	symbol = _T("1301");
	CString	history = (char *)m_pwndView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC),
				(long)symbol.operator LPCTSTR());

	CString	Str, Code, Name;
	int	index;
	for (;;)
	{
		index = history.Find("\t");
		if (index <= 0)
			break;

		Str = history.Left(index);
		history = history.Mid(index + 1);

		index = Str.Find(" ");
		if (index > 0)
		{
			_codeinfo *pCode = new struct _codeinfo;
			memset(pCode, 0x00, sz_Code);

			Code = Str.Left(index);
			//Name = Str.Mid(index + 1);
			memcpy(pCode->code, Code, Code.GetLength());
			//memcpy(pCode->name, Name, Name.GetLength());
			m_arCode.Add(pCode);
		}
	}
	m_iSelect = -1;
}

void CGrpView::SelectGroup(CString codelist, int kind)
{
	RemoveCodeList();
	m_iListKind = kind;

	CString	Str;
	int	index;
	for (;;)
	{
		index = codelist.Find("\t");
		if (index <= 0)
			break;

		Str = codelist.Left(index);
		codelist = codelist.Mid(index + 1);

		_codeinfo *pCode = new struct _codeinfo;
		memset(pCode, 0x00, sz_Code);
		memcpy(pCode->code, Str, Str.GetLength());
		m_arCode.Add(pCode);
	}
	m_iSelect = -1;
}

char* CGrpView::GetNearCode(bool bPre, CString searchCode)
{
	switch (m_iListKind)
	{
	case TRCD_HISTORY:
	case TRCD_COMPANY:
	case TRCD_INTEREST:
	case TRCD_THEME:
		searchCode = _T("");
	}

	CString			code = _T("");
	int			nSelect = -1;
	struct _codeinfo*	pCode = NULL;

	int	nSize = m_arCode.GetSize();
	if (nSize > 0)
	{
		if (bPre)
		{
			if (!searchCode.IsEmpty())
			{
				for ( int ii = 0 ; ii < nSize ; ii++ )
				{
					pCode = m_arCode.GetAt(ii);
					code.Format("%s", pCode->code);

					if (searchCode.CompareNoCase(code) == 0)
					{
						m_iSelect = ii;
						break;
					}
				}
			}

			if (m_iSelect < 0)
			{
				m_iSelect = 0;
			}
			else
			{
				m_iSelect--;
				
				if (m_iSelect < 0)
					m_iSelect = nSize - 1;				
			}

			pCode = m_arCode.GetAt(m_iSelect);
			code.Format("%s", pCode->code);
		}
		else
		{
			if (!searchCode.IsEmpty())
			{
				for ( int ii = 0 ; ii < nSize ; ii++ )
				{
					pCode = m_arCode.GetAt(ii);
					code.Format("%s", pCode->code);

					if (searchCode.CompareNoCase(code) == 0)
					{
						m_iSelect = ii;
						break;
					}
				}
			}

			if (m_iSelect < 0)
			{
				m_iSelect = 0;
			}
			else
			{
				m_iSelect++;
				m_iSelect = m_iSelect % nSize;
			}

			pCode = m_arCode.GetAt(m_iSelect);
			code.Format("%s", pCode->code);
		}		
	}

	ClearBuffer();

	m_pcTemp = AllocBuffer(64);
	sprintf(m_pcTemp, "%s", code);

	return m_pcTemp;
}

BOOL CGrpView::MainChartIsVarType()
{
	struct _envInfo* env = (struct _envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	int mainGKind = env->datainfo.wMainGrpKind;

	if (mainGKind >= GK_EQV && mainGKind <= GK_CDVV)
		return TRUE;
	if (mainGKind >= GK_PNF && mainGKind <= GK_GUKKI)
		return TRUE;
	else
		return FALSE;
}

//**
BOOL CGrpView::CaseCheck(int wGKind)
{
	CString title;
	struct _envInfo* env = (struct _envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);

	int mainGKind = env->datainfo.wMainGrpKind;
	int	index = env->datainfo.btIndex;

	if (mainGKind >= GK_EQV && mainGKind <= GK_CDVV)
		return FALSE;
	if (mainGKind >= GK_PNF && mainGKind <= GK_GUKKI)
		return FALSE;

	int unit = env->datainfo.btUnit;
	switch (wGKind) 
	{
		case GK_RRATE:	title = "회전율"; break;
		case GK_AMT:	title = "거래대금"; break;
		case GK_AMA:	title = "거래대금 이평"; break;
		case GK_ADR:	title = "ADR"; break;
		case GK_FRGNHAVE: title = "외국인보유(수량)"; break;
		case GK_FRGNRATE: title = "외국인보유(비율)"; break;
		case GK_PSNLPBUY: title = "개인순매수(수량)"; break;
		case GK_FRGNPBUY: title = "외국인순매수(수량)"; break;
		case GK_ORGNPBUY: title = "기관순매수(수량)"; break;
		case GK_PSNLTBUY: title = "개인누적순매수(수량)"; break;
		case GK_FRGNTBUY: title = "외국인누적순매수(수량)"; break;
		case GK_ORGNTBUY: title = "기관누적순매수(수량)"; break;
		default: 
			return TRUE;
	}

	switch (wGKind)
	{
	case GK_RRATE:		// 회전율(주 - 일주월)
		if ((unit == GU_CODE) && (index == GI_DAY || index == GI_WEEK || index == GI_MONTH))
			break;
		
		m_pApp->MessageBoxF("%s 지표는 주식 일,주,월차트에만 추가할 수 있습니다.", title);
		return FALSE;
	case GK_AMT:		// 거래대금(주,업 - 일주월)
	case GK_AMA:		// 대금이평(주,업 - 일주월)
		if ((unit == GU_CODE || unit == GU_INDEX) && (index == GI_DAY || index == GI_WEEK || index == GI_MONTH))
			break;

		m_pApp->MessageBoxF("%s 지표는 주식,업종 일,주,월차트에만 추가할 수 있습니다.", title);
		return FALSE;
	case GK_ADR:		// ADR
		if (unit == GU_INDEX && index == GI_DAY)
			break;

		m_pApp->MessageBoxF("%s 지표는 업종 일차트에만 추가할 수 있습니다.", title);
		return FALSE;
	case GK_FRGNHAVE:	// 외국인보유현황(수량)
	case GK_FRGNRATE:	// 외국인보유비율
		if (unit == GU_CODE && index == GI_DAY)
			break;

		m_pApp->MessageBoxF("%s 지표는 주식 일차트에만 추가할 수 있습니다.", title);
		return FALSE;
	case GK_PSNLPBUY:	// 개인순매수량
	case GK_FRGNPBUY:	// 외국인순매수량
	case GK_ORGNPBUY:	// 기관순매수량
	case GK_PSNLTBUY:	// 개인누적순매수량
	case GK_FRGNTBUY:	// 외국인누적순매수량
	case GK_ORGNTBUY:	// 기관누적순매수량
		if ((unit == GU_CODE || unit == GU_INDEX) && (index == GI_DAY))
			break;

		m_pApp->MessageBoxF("%s 지표는 주식,업종 일차트에만 추가할 수 있습니다.", title);
		return FALSE;
	}

	return TRUE;
}

bool CGrpView::CallPnToolDlg(char* pcInfo)
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	bool (APIENTRY *axPnToolConfig)(CWnd*, CWnd*, void*);
	axPnToolConfig = (bool (APIENTRY *)(CWnd*, CWnd*, void*)) GetProcAddress(m_hiGDlgLib, "axPnToolConfig");
	if (!axPnToolConfig)
		return false;

	return axPnToolConfig(this, m_pwndView, pcInfo);
}

bool CGrpView::CallPnAidDlg(char* pcInfo)
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	bool (APIENTRY *axPnAidConfig)(CWnd*, CWnd*, void*);
	axPnAidConfig = (bool (APIENTRY *)(CWnd*, CWnd*, void*)) GetProcAddress(m_hiGDlgLib, "axPnAidConfig");
	if (!axPnAidConfig)
		return false;

	return axPnAidConfig(this, m_pwndView, pcInfo);
}

bool CGrpView::CallYAxisConfing(char* pcInfo)
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	bool (APIENTRY *axYAxisConfig)(CWnd*, CWnd*, void*, void*, void*);
	axYAxisConfig = (bool (APIENTRY *)(CWnd*, CWnd*, void*, void*, void*)) GetProcAddress(m_hiGDlgLib, "axYAxisConfig");
	if (!axYAxisConfig)
		return false;

	_envInfo* penvinfo = (_envInfo*)m_pwndChart->SendMessage(GEV_CHART, MAKEWPARAM(CHART_DATA, REQUEST_CUR_ENV), 0);
	return axYAxisConfig(this, m_pwndView, pcInfo, m_pcGDat, penvinfo);
}

int CGrpView::CallYShowConfingByMove()
{
	if (!m_hiGDlgLib)
	{
		m_hiGDlgLib = LoadLibrary("axisGDlg.dll");
		if (!m_hiGDlgLib)
		{
			m_pApp->MessageBox("axisGDlg.dll LoadLibrary error", COMPANYNAME);
			return false;
		}
	}

	int (APIENTRY *axYShowConfigByMove)();
	axYShowConfigByMove = (int (APIENTRY *)()) GetProcAddress(m_hiGDlgLib, "axYShowConfigByMove");
	if (!axYShowConfigByMove)
		return false;

	return axYShowConfigByMove();
}

#ifdef _DEBUG
void CGrpView::WriteFile(CString strData)
{
	CFile		fileTrace;
	DWORD		dwPos;
	CString		strFileName, strTime;
	SYSTEMTIME	st;
	
	GetLocalTime(&st);
	strTime.Format("[%x] [%d년 %d월 %d 일% d시 %d분 %d초]\n" 
			, this, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	strFileName.Format("%s\\%s\\%s\\chart_debug%d%02d%02d.trc", m_pApp->GetRoot(), "USER", m_pApp->GetUName(), st.wYear, st.wMonth, st.wDay);
	BOOL bExist = fileTrace.Open(strFileName, CFile::modeReadWrite);//CFile::modeCreate|CFile::modeReadWrite);
	if (!bExist)
	{
		fileTrace.Open(strFileName, CFile::modeCreate|CFile::modeReadWrite);
		fileTrace.Write((char *) strTime.operator LPCTSTR(), strTime.GetLength());
		fileTrace.Write(strData.operator LPCTSTR(), strData.GetLength());
	}
	else
	{
		dwPos = fileTrace.SeekToEnd();
		fileTrace.Seek((long) dwPos, CFile::begin);
		fileTrace.Write((char *) strTime.operator LPCTSTR(), strTime.GetLength());
		fileTrace.Write(strData.operator LPCTSTR(), strData.GetLength());
	}

	char newline[] = "\r\n\r\n";
	fileTrace.Write(newline, strlen(newline));
	fileTrace.Flush();
	fileTrace.Close();
}
#endif


void CGrpView::SendTR(CWnd* sender, CString strCode, LPCTSTR strData, int nSize, char stat)
{
	if (m_trSender) return;

	m_trSender = sender;

	char*	cDataBuffer = new char [L_userTH + nSize]; //10240];
	struct	_userTH* userTH;
	userTH = (struct _userTH *) cDataBuffer;
	
	strcpy(userTH->trc, strCode);
	userTH->key = 120;
	userTH->stat = stat;
	
	CopyMemory(&cDataBuffer[L_userTH], strData, nSize);

	m_pwndView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nSize), (LPARAM)cDataBuffer);
	delete [] cDataBuffer;	

	m_bTransaction = true;
}

LRESULT CGrpView::OnViewSendTR(WPARAM wParam, LPARAM lParam)
{
	LPTRINFO tr = (LPTRINFO)lParam;

	SendTR(tr->sender, tr->trCode, tr->data, tr->size, tr->stat);

	return 0L;
}


LPSTR CGrpView::AllocBuffer(int size)
{
	ClearBuffer();

	//Debug("AllocBuffer(%d)", size);
	m_pcTemp = new char[size];
	return m_pcTemp;
}

void CGrpView::ClearBuffer()
{
	if (m_pcTemp) {
		//Debug("ClearBuffer");
		BUF_DELETE(m_pcTemp);
	}
}





void CGrpView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect crc, rc;
	GetClientRect(crc);

	DWORD bkColor = m_pwndView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), 64);
	dc.FillSolidRect(crc, bkColor);

	rc = crc;
	rc.top += HEIGHT_INPUT;
	int iGabCfg = GetPnSize(PN_CFG);

	CPen pn(PS_NULL, 1, m_pwndView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), 181));
	CPen	*sPen = dc.SelectObject(&pn);

	if (iGabCfg != 0)
	{
		rc.left += iGabCfg + 10 + 2;
		rc.InflateRect(-1,-2, 0, -2);
		dc.RoundRect(rc,CPoint(5,5));

		rc.SetRect(crc.left, HEIGHT_INPUT, iGabCfg + 8, crc.bottom);
		rc.InflateRect(-1,-2, 1, -2);
		dc.RoundRect(rc,CPoint(5,5));
	}
	else
	{
		rc.InflateRect(-1,-2, 0, -2);
		dc.RoundRect(rc,CPoint(5,5));
	}

	dc.SelectObject(sPen);
}

//
// 20090928 : ADD : RSI 지표 추가
//
#include "../../h/axisgbinfo.h"
void CGrpView::CheckNewList()
{
	if (!m_pcGDat)
		return;

	/*
	_basicInfo	addInfo[] = 
	{
		{ GT_LINE,	GK_CRBLCNUM,	0,	0, {0, {0, 0, 0, 0,}, {{DEFCR8,DFL,1},}}},
	};
	*/
	_basicInfo	addInfo[] = 
	{
		{ GT_LINE,	GK_RSIWEIGHT,	GC_MAEXP,	GO_FILLOVER,
		{0, {14, 9, 0, 0,}, {{DEFCR8,DFL,1},{DEFCR10,DFL,1},}}, 
		{0x03, {30,70,}, DEFCR2, DEFCR9, {2, 1, }, {{DEFCR13 ,DFL,1},{DEFCR12 ,DFL,1},}}}
	};

	int	nCount = sizeof(addInfo) / sizeof(struct _basicInfo);

	struct _cntInfo *cntInfo = (struct _cntInfo *)m_pcGDat;
	int mcnt = cntInfo->wMainCnt;
	int acnt = cntInfo->wAidCnt;
	int ucnt = cntInfo->wUserCnt;

	struct _graph	*pGraph;
	for (int jj = 0; jj < nCount; jj++)
	{
		for (int ii = 69; ii < mcnt+acnt+ucnt; ii++)
		{
			pGraph = (_graph *)&m_pcGDat[SZ_CNTINFO + ii*SZ_GRAPH];
			if (pGraph->wGKind <= 0 || pGraph->wGKind == addInfo[jj].wGrpKind)
				break;
		}

		if (pGraph->wGKind == addInfo[jj].wGrpKind)
			continue;

		ZeroMemory(pGraph, SZ_GRAPH);
		pGraph->wKey = 1;
		pGraph->btDataKey = 1;
		pGraph->btGType = addInfo[jj].btType;
		pGraph->wGKind = addInfo[jj].wGrpKind;
		pGraph->wSKind = 1;
		pGraph->wCalOption = addInfo[jj].wCOption;
		pGraph->dwDraOption = addInfo[jj].dwDOption;
		pGraph->rgbLabel = LINCR;
		CopyMemory(&pGraph->cInfo, &addInfo[jj].chartInfo, SZ_CHARTINFO);
		CopyMemory(&pGraph->tInfo, &addInfo[jj].tickInfo, SZ_TICKINFO);
	}
}