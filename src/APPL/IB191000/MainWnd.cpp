// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB191000.h"
#include "MainWnd.h"
#include "toolwnd.h"
#include "groupwnd.h"

#include "TreeWnd.h"
#include "BaseWnd.h"
#include "optdlg.h"
#include "RTSWnd.h"
#include "GroupWnd.h"

#include "UsefulSetupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* pWnd)
{
	m_pWnd = pWnd;	
	m_pToolWnd = NULL; 
	m_pTreeWnd = NULL;
	m_pBasketWnd = NULL;
	m_pGroupWnd = NULL;
	m_pRTSWnd = NULL;
	m_nTree = 0;
	m_nBasket = 0;		
	m_nGroup = 0;
	m_bTree = FALSE;
	m_bBasket = FALSE;
	m_bRTS = FALSE;
	m_pOptDlg = NULL;
	m_bDestroy = false;
	m_nResize = MO_NONE;
	m_bProc = false;

	m_bThreadRTM = false;
	m_pThreadRTM = NULL;
	m_hEventRTM = NULL;
	m_bRTMReady = FALSE;
	m_bWaveApply = FALSE;

	m_clrKospi	  = RGB(0, 0, 0);
	m_clrKosdaq	  = RGB(128, 64, 64);
	m_bMKClrApply = FALSE;
	m_bPopupDlg	  = FALSE;
	m_bDispCode   = FALSE;

	initRTM();
}

CMainWnd::~CMainWnd()
{

}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers
//#include "../../axis/axmsg.hxx"
//#include "../../axis/xcaption.h"
int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	AfxEnableControlContainer();

	Variant(titleCC, "관심종목");
	init();
	loadinfo();
	CreateChild();
	SetPallette();
	return 0;
}

void CMainWnd::GuideMessage(CString msg)
{
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axGUIDE, 1), (LONG)(char*)msg.operator LPCTSTR());
}


void CMainWnd::OnDestroy() 
{
	m_bDestroy = true;

	CWnd::OnDestroy();	
	
	saveinfo();
	
	stopRTM();
	DestroyChild();	

	::CloseHandle(m_hEventRTM);
}

void CMainWnd::loadinfo()
{
	CString	filepath;

	filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);
	if (GetPrivateProfileInt(SEC_MAIN, KEY_VERSION, 0, filepath) < 1)
		::DeleteFile(filepath);
	
	char	buf[1024];
	CString	sztmp, szUnit;
	CString	section, key;
	int	val = 0;
	memset(buf, 0x00, sizeof(buf));
	
	// height
	m_size.cx = m_size.cy = 0;
	m_size.cy = GetPrivateProfileInt(SEC_MAIN, KEY_HEIGHT, SIZE_HEIGHT, filepath);

	// 트리 윈도우
	m_nTree = GetPrivateProfileInt(SEC_MAIN, KEY_TREESIZE, SIZE_TREE, filepath);
	m_bTree = (BOOL)GetPrivateProfileInt(SEC_MAIN, KEY_TREEUSE, 0, filepath);

	// 서치 옵션
	m_nOption = GetPrivateProfileInt(SEC_MAIN, KEY_SOPTION, OPT_DEFAULT, filepath);
	
	if (m_bTree)
	{
		m_size.cx += m_nTree;
		m_size.cx += SIZE_DIVIDE;
	}

	if (m_bRTS)
		m_size.cx += SIZE_RTS;

	// 장바구니 윈도우
	m_nBasket = GetPrivateProfileInt(SEC_MAIN, KEY_BASKETSIZE, SIZE_BASKET, filepath);
	m_bBasket = (BOOL)GetPrivateProfileInt(SEC_MAIN, KEY_BASKETUSE, 0, filepath);
	
	if (m_bBasket)
	{
		m_size.cx += m_nBasket;
		m_size.cx += SIZE_DIVIDE;
	}
	
	// group
	m_nGroup = GetPrivateProfileInt(SEC_MAIN, KEY_GROUPSIZE, SIZE_GROUP, filepath);
	//BLOCK FOR SIZING
/*	if(m_nGroup < 511)
	{
		m_nGroup = 511;
	}*/

	m_size.cx += m_nGroup;
// 	CString sd;
// 	sd.Format("cx : %d Tree %d RTs : %d basket : %d nGroup : %d", m_size.cx, m_nTree+SIZE_DIVIDE, SIZE_RTS, m_nBasket+SIZE_DIVIDE, m_nGroup);
// 	MessageBox(sd);
//	m_size.cx += FIRSTOPEN_XSIZE;

	// Wave File
	m_bWaveApply = (BOOL)GetPrivateProfileInt(SEC_MAIN, "APPWAVE", 0, filepath);

	CString strWaveFile;
	strWaveFile.Format("%s\\%s\\%s", m_home, IMAGEDIR, "000.WAV");
	
	memset(buf, 0x00, sizeof(buf));
	if (0 == GetPrivateProfileString(SEC_MAIN, "WAVE00", "", buf, sizeof(buf), filepath))
	{
		m_strWaveFile = strWaveFile;
	}
	else
	{
		m_strWaveFile.Format("%s", buf);
	}

	m_bMKClrApply = (BOOL)GetPrivateProfileInt(SEC_MAIN, "APPMKCLR", 0, filepath);

	m_clrKospi = GetPrivateProfileInt(SEC_MAIN, "KOSPI", 0, filepath);

	m_clrKosdaq = GetPrivateProfileInt(SEC_MAIN, "KOSDAQ", 0, filepath);

	if (0 == GetPrivateProfileString(SEC_MAIN, "POPUP1", "", buf, sizeof(buf), filepath))
	{
		m_bPopupDlg = FALSE;
	}
	else
	{
		m_bPopupDlg = atoi(buf);
	}

	if (0 == GetPrivateProfileString(SEC_MAIN, "DISPCODE", "", buf, sizeof(buf), filepath))
	{
		m_bDispCode = FALSE;
	}
	else
	{
		m_bDispCode = atoi(buf);
	}
}

void CMainWnd::saveinfo()
{
	CString	filepath;
	CString	sztmp = _T("");

	filepath.Format("%s\\%s\\%s\\%s", m_home, USRDIR, m_user, FILE_CONFIG);
	WritePrivateProfileString(SEC_MAIN, KEY_VERSION, "1", filepath);

	// 트리윈도우
	sztmp.Format("%d", m_nTree);
	WritePrivateProfileString(SEC_MAIN, KEY_TREESIZE, sztmp, filepath);
	WritePrivateProfileString(SEC_MAIN, KEY_TREEUSE, (m_bTree) ? "1" : "0", filepath);

	sztmp.Format("%d", m_nOption);
	WritePrivateProfileString(SEC_MAIN, KEY_SOPTION, sztmp, filepath);
	
	// 장바구니 윈도우
	sztmp.Format("%d", m_nBasket);
	WritePrivateProfileString(SEC_MAIN, KEY_BASKETSIZE, sztmp, filepath);
	WritePrivateProfileString(SEC_MAIN, KEY_BASKETUSE, (m_bBasket) ? "1" : "0", filepath);

	// group
	sztmp.Format("%d", m_size.cx - ((m_bTree) ? m_nTree+SIZE_DIVIDE : 0));//BUFFET ADD FOR SIZE 
	
// 	CString dd;
// 	dd.Format("cx : %d Tree %d RTs : %d basket : %d nGroup : %d", m_size.cx, m_nTree+SIZE_DIVIDE, SIZE_RTS, m_nBasket+SIZE_DIVIDE, m_nGroup);
// 	MessageBox(dd);
	//sztmp.Format("%d", m_nGroup);
	WritePrivateProfileString(SEC_MAIN, KEY_GROUPSIZE, sztmp, filepath);
	
	sztmp.Format("%d", m_size.cy);
	WritePrivateProfileString(SEC_MAIN, KEY_HEIGHT, sztmp, filepath);	

	sztmp.Format("%d", m_bWaveApply);
	WritePrivateProfileString(SEC_MAIN, "APPWAVE", sztmp, filepath);

	WritePrivateProfileString(SEC_MAIN, "WAVE00", m_strWaveFile.GetBuffer(0), filepath);

	sztmp.Format("%d", m_bMKClrApply);
	WritePrivateProfileString(SEC_MAIN, "APPMKCLR", sztmp, filepath);

	sztmp.Format("%d", m_clrKospi);
	WritePrivateProfileString(SEC_MAIN, "KOSPI", sztmp, filepath);

	sztmp.Format("%d", m_clrKosdaq);
	WritePrivateProfileString(SEC_MAIN, "KOSDAQ", sztmp, filepath);

	sztmp.Format("%d", m_bPopupDlg);
	WritePrivateProfileString(SEC_MAIN, "POPUP1", sztmp, filepath);

	sztmp.Format("%d", m_bDispCode);
	WritePrivateProfileString(SEC_MAIN, "DISPCODE", sztmp, filepath);
}

HBITMAP CMainWnd::getBitmap(CString path)
{
	CBitmap*	pBitmap = (CBitmap*)m_pWnd->SendMessage(WM_USER, getBITMAP, (LPARAM)path.operator LPCTSTR());
	return pBitmap->operator HBITMAP();
}

CFont* CMainWnd::GetAxFont(CString fName, int point, bool bBold, bool bItalic)
{
	struct _fontR fontR;

	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = bItalic; 
	fontR.bold = bBold ? FW_BOLD : FW_NORMAL;
	return (CFont*)m_pWnd->SendMessage(WM_USER, getFONT, (long)&fontR);
}

void CMainWnd::CreateChild()
{
	COLORREF	color = getAxColor(64);
	// 각 차일드 윈도우가 생성될 때 초기화과정중 윈도우들 끼리 연동이 생길수 있으므로 차일드 생성순서에 따라 죽을 수 있다. 
	// 즉 m_pGroupWnd 생성중 m_pBasketWnd로 부터 데이터를 요구하거나 하면 죽을 수 있으므로 이점 유의 하여 생성하여야 한다.
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_home, IMAGEDIR);
	
	HBITMAP	hBITMAP, hBITMAP_DN, hBITMAP_HV;
	hBITMAP = getBitmap(fileIMG + "TROPT.bmp");
	hBITMAP_DN = getBitmap(fileIMG + "TROPT_dn.bmp");
	hBITMAP_HV = getBitmap(fileIMG + "TROPT_en.bmp");
	m_pFont = GetAxFont();

	m_btOption.Create("검색옵션\n제외종목선택", CRect(0, 0, 0, SIZE_BUTTONS), this, IDC_BT_CONFIG, TRUE, TRUE);
	m_btOption.SetFont(m_pFont, false);  
	m_btOption.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	m_btOption.SetCheck(false);
	m_nOptHeight = 0;
	
	if (m_pOptDlg == NULL)
	{
		m_pOptDlg = new COptDlg(this);
		m_pOptDlg->Create(COptDlg::IDD, this);
		CRect	rect;
		SearchOption(MO_SET, m_nOption);
		m_pOptDlg->GetWindowRect(rect);
		m_nOptHeight = rect.Height();
	}

	if (m_pTreeWnd == NULL)
	{
		m_pTreeWnd = new CTreeWnd(this);
		m_pTreeWnd->Create(WS_CHILD|TVS_HASLINES|TVS_HASBUTTONS|TVS_LINESATROOT|TVS_SHOWSELALWAYS, CRect(0, 0, 0, 0), this, IDC_TREE);			
	}	

	if (m_pRTSWnd == NULL)
	{
		m_pRTSWnd = new CRTSWnd(this);
		m_pRTSWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_TOOL);
		m_pRTSWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}

	if (m_pToolWnd == NULL)
	{
		m_pToolWnd = new CToolWnd(this);
		m_pToolWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_TOOL);
		m_pToolWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}
	
	if (m_pGroupWnd == NULL)
	{
		m_pGroupWnd = new CGroupWnd(this);
		((CGroupWnd *)m_pGroupWnd)->m_strWaveFile = m_strWaveFile;
		((CGroupWnd *)m_pGroupWnd)->m_bWaveApply  = m_bWaveApply;
		((CGroupWnd *)m_pGroupWnd)->m_bMKClrApply = m_bMKClrApply;
		((CGroupWnd *)m_pGroupWnd)->m_clrKospi	  = m_clrKospi;
		((CGroupWnd *)m_pGroupWnd)->m_clrKosdaq   = m_clrKosdaq;
		((CGroupWnd *)m_pGroupWnd)->m_bPopupDlg	  = m_bPopupDlg;
		((CGroupWnd *)m_pGroupWnd)->m_bDispCode	  = m_bDispCode;
		m_pGroupWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_GROUP);	
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);
	}
	if (m_pBasketWnd == NULL)
	{
		m_pBasketWnd = new CBasketWnd(this);
		m_pBasketWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, IDC_BASKET);
		m_pBasketWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)getAxColor(65));	
	}
	// 
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKEWPARAM(m_size.cx, m_size.cy));	
	MoveWindow(0, 0, m_size.cx, m_size.cy);
}

void CMainWnd::DestroyChild()
{
	if (LIB_IsWndAlive(m_pToolWnd))
		LIB_DeleteWnd(m_pToolWnd);
	if (LIB_IsWndAlive(m_pTreeWnd))
		LIB_DeleteWnd(m_pTreeWnd);
	if (LIB_IsWndAlive(m_pGroupWnd))
		LIB_DeleteWnd(m_pGroupWnd);
	if (LIB_IsWndAlive(m_pBasketWnd))
		LIB_DeleteWnd(m_pBasketWnd);
	if (LIB_IsWndAlive(m_pOptDlg))
		LIB_DeleteWnd(m_pOptDlg);
	if (LIB_IsWndAlive(m_pRTSWnd))
		LIB_DeleteWnd(m_pRTSWnd);

	m_btOption.DestroyWindow();
}

LONG CMainWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch (LOWORD(wParam))
	{
	case MK_ISTREE:
		ret = m_bTree;
		break;

	case MK_GROUPHEIGHT:
		{
			int	nHeight		= m_size.cy;
			if (!m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE))
				nHeight -= (SIZE_TOOL / 2 + 1);
			else
				nHeight -= SIZE_TOOL;
			if (m_bBasket)
				nHeight -= m_nBasket;
			ret = nHeight;
		}
		break;
	case MK_PROCDLL:
		m_bProc = true;
		break;
	case MK_VARIANT:
		{
			m_szRET = _T("");
			m_szRET.Format("%s", Variant(HIWORD(wParam), (char*)lParam));
			ret = (LONG)(char*)m_szRET.operator LPCTSTR();
		}
		break;
	case MK_SHOWTOOLTIP:
		Variant(guideCC, (char*)lParam);
		break;

	case MK_TOOLSIZE:
		ResizeOper(m_size.cx, m_size.cy);
		break;

	case MK_SYNC:
		switch (HIWORD(wParam))
		{
		case MO_LOCK:
			m_sync.Lock();
			break;
		case MO_UNLOCK:
			m_sync.Unlock();
			break;
		}
		break;
	case MK_GETID:
		ret = (LONG)(char*)m_id.operator LPCTSTR();
		break;
	case MK_GETROOT:
		ret = (LONG)(char*)m_home.operator LPCTSTR();
		break;
	case MK_GETUSER:
		ret = (LONG)(char*)m_user.operator LPCTSTR();
		break;		
	case MK_GETBITMAP:
		ret = (LONG)m_pWnd->SendMessage(WM_USER, getBITMAP, lParam);
		break;
	case MK_TREEVISIBLE:
		ret = OperTreeVisible(HIWORD(wParam));
		break;
	case MK_RTSVISIBLE:
		ret = OperRTSVisible(HIWORD(wParam));
		break;
	case MK_BASKETVISIBLE:
		ret = OperBasketVisible(HIWORD(wParam));
		break;
	case MK_SIZE:
		OperSize(HIWORD(wParam), CSize(LOWORD(lParam), HIWORD(lParam)));
		break;
	case MK_OPENSCR:
		OpenScreen(HIWORD(wParam), (char*)lParam);
		break;
	case MK_SEARCHOPT:
		ret = SearchOption(HIWORD(wParam), (DWORD)lParam);
		break;
	case MK_GETWND:
		switch (HIWORD(wParam))
		{
		case MO_VIEW: 
			ret = (LONG)m_pWnd;
			break;
		case MO_GROUP:
			ret = (LONG)m_pGroupWnd;
			break;
		case MO_TOOL:
			ret = (LONG)m_pToolWnd;
			break;
		case MO_TREE:
			ret = (LONG)m_pTreeWnd;
			break;
		case MO_BASKET:
			ret = (LONG)m_pBasketWnd;
			break;
		case MO_GRID:
			ret = m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GRID));
			break;
		}
		break;
	case MK_SELGROUP:
		if (HIWORD(wParam) == MO_SET)
			SelectGroup((int)lParam);
		break;
	// by pass message
	case MK_SENDTR:
		m_bRTMReady = FALSE;		// RTM not Ready
		SendOper((CSendData*)lParam);
		break;
	case MK_GROUP:
	case MK_GETGROUP:
	case MK_CHANGEFIELD:
	case MK_EXCEL:
	case MK_EXPECT:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_AUTOEXPECT:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_EQUALIZER:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);
		break;
	case MK_TREEDATA:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);		//4
		break;
	case MK_UPJONGDATA:
		ret = (LONG)m_pGroupWnd->SendMessage(WM_MANAGE, wParam, lParam);		//4
		break;
	case MK_ALERTWAVE:
		{
			CUsefulSetupDlg* pDlg = (CUsefulSetupDlg*)lParam;

			if (NULL != pDlg)
			{
				m_bWaveApply  = pDlg->m_bWaveApply;
				m_strWaveFile = pDlg->m_strFile;
			}
		}
		break;
	case MK_MARKETCOLOR:
		{
			CUsefulSetupDlg* pDlg = (CUsefulSetupDlg*)lParam;

			if (NULL != pDlg)
			{
				m_bMKClrApply = pDlg->m_bClrApply;
				m_clrKospi    = pDlg->m_clrKospi;
				m_clrKosdaq   = pDlg->m_clrKosdaq;
			}
		}
		break;
	case MK_CLOSE:
		{
			OnDestroy();
		}
		break;
	case MK_MARKETCODE:
		{
			m_bDispCode = (BOOL)lParam;
		}
		break;
	}

	return ret;
}

void CMainWnd::SetPallette()
{
	COLORREF	color = getAxColor(64);
	if (m_pToolWnd)
		m_pToolWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);

	if (m_pGroupWnd)
		((CGroupWnd*)m_pGroupWnd)->SetGridBkColor(color);

	if (m_pRTSWnd)
		m_pRTSWnd->SendMessage(WM_MANAGE, MK_SETPAL, (LPARAM)color);//BUFFET

	m_strPal = GetPalette();
	
	Invalidate();
}

LONG CMainWnd::OperDLLOUB(WPARAM wParam, LPARAM lParam)
{
	LONG ret = 0;
	WORD	kind = HIBYTE(LOWORD(wParam));
	CRecvData	rdata;
	CSendData	sdata;
	struct _trkey* trkey = (struct _trkey*)&kind;

	rdata.SetData(sdata.GetUserData(), wParam, lParam);
	switch (trkey->kind)
	{
	case TRKEY_GROUPACCNLIST:
	case TRKEY_ACCCUSTOM:
	case TRKEY_ACCSTAFF:
	case TRKEY_NEWS:
	case TRKEY_REMAIN:
		if (m_bDestroy)
			break;
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata); 
		break;
	case TRKEY_ELWISSUESEC:
		if (m_bDestroy)
			break;
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_ELWBASEASSET:
		if (m_bDestroy)
			break;
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_RECOMMAND:
		if (m_bDestroy)
			break;
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	case TRKEY_GRIDUPJONG:
		if (m_bDestroy)
			break;
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
		break;
	default:
		if (m_bDestroy)
			break;

		switch (trkey->kind)
		{
		case TRKEY_GRIDNEW:
		case TRKEY_GRIDNEWS:
		case TRKEY_GRIDROW:
		case TRKEY_GRIDSAVE:
			removeRTM();
			m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)&rdata);
			break;
		default:
			if (m_bRTS && m_pRTSWnd)
				m_pRTSWnd->SendMessage(WM_USER, wParam, lParam);
			break;
		}		
		break;
	}

	return ret;
}


//테스트용
void CMainWnd::testSaveFile(CString dt)
{
	CString strPath(_T("")), m_root, m_user;
	strPath.Format("c:\\test.ini");
	COleDateTime oTime;
	oTime = COleDateTime::GetCurrentTime();
	CString strCurTime;
	strCurTime.Format(_T("%dh:%dm:%ds"), oTime.GetHour(), oTime.GetMinute(), oTime.GetSecond());
	CString titletime;
	
	titletime.Format("[%s]MainWnd::OnUser", strCurTime);
	
	WritePrivateProfileString("IB202200", strCurTime, (LPCTSTR)dt, (LPCTSTR)strPath);
}

LONG CMainWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
//		testSaveFile("NULL");

		m_sync.Lock();
		ret = OperDLLOUB(wParam, lParam);
		this->EnableWindow(TRUE);
		m_sync.Unlock();
		SendMode(false);
		m_bRTMReady = TRUE;
		break;
	case DLL_ALERT:
		{
			if (m_bDestroy || !m_bThreadRTM)
				break;

			char *pBuf = (char*)lParam;
			int len = strlen(pBuf);

			char* pBytes = new char[len+1];
			memset(pBytes, 0x00, len+1);
			memcpy(pBytes, pBuf, len);

			m_syncRTM.Lock();
			m_arBytesRTM.Add(pBytes);
			m_syncRTM.Unlock();
			SetEvent(m_hEventRTM);
		}
		break;
	case DLL_NOTICE:
		{
			if (m_bRTS && m_pRTSWnd)
				m_pRTSWnd->SendMessage(WM_USER, wParam, lParam);
		}
		break;
	case DLL_SETPAL:
		SetPallette();
		break;
	case DLL_TRIGGER:
		if (m_bDestroy)
			break;
		parsingTrigger((char*)lParam);
		break;
	case DLL_DOMINO:
		parsingDomino((char*)lParam);	//종합화면에서 2022열면서 그룹정보 보낼 시
		break;
	case DLL_GUIDE:
		return 1;
		break;
	}

	return ret;
}

void CMainWnd::parsingDomino(CString datB)
{
	CString symbol, entry;

	while(!datB.IsEmpty())
	{
		symbol = IH::Parser(datB, "\t");
		entry = IH::Parser(datB, "\n");

		if(symbol == _T("selectGroup") && !entry.IsEmpty())
		{
			m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELECTGROUP, (LPARAM)atoi(entry)));
		}
	}
}

void CMainWnd::parsingTrigger(CString datB)
{
	CString	symbol;

	symbol = IH::Parser(datB, "\t");

	if (symbol == _T("OnPortfolio") && datB == _T("ok"))
	{
		if (!m_bProc)
		{

			int	ret = (int)m_pToolWnd->SendMessage(WM_MANAGE, MK_SETUPOK);
			m_pTreeWnd->SendMessage(WM_MANAGE, MK_SETUPOK);

			// 순서를 지켜야한다. m_pToolWnd와 m_pTreeWnd가 서로 setting된후에 그값을 기준으로 m_pGroupWnd가 변한다.
			m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SETUPOK, ret));	

			m_pToolWnd->SendMessage(WM_MANAGE, MK_SENDTREE);
		}
		m_bProc = false;
	}	
	else if (symbol == _T("OnCatchChange") && datB == _T("ok"))	// 조건변경
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SISECATCH, MO_INIT));
	}
	else if (symbol == _T("Alarm"))					// 해당종목
	{
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SISECATCH, MO_CATCH), (LPARAM)(char*)datB.operator LPCTSTR());
	}


}


COLORREF CMainWnd::getAxColor(int color)
{
	if (color & 0x02000000)
		return color;

	return m_pWnd->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}


void CMainWnd::sendTR(CString trCode, char* datB, int datL, int key)
{
	char* sendB = new char [L_userTH + datL + 1];

	struct	_userTH* uTH;
	uTH = (struct _userTH *) sendB;

	strcpy(uTH->trc, trCode);
	uTH->key = key;
	uTH->stat = US_PASS;

	CopyMemory(&sendB[L_userTH], datB, datL);
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) sendB);
	delete [] sendB;
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	char* dta = (char*)m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = CString(dta);
	
	return retvalue;
}

void CMainWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting	
//	CMemDC	mdc(&dc);
	DrawDivide(&dc);
}

void CMainWnd::init()
{
	m_home = Variant(homeCC);
	m_user = Variant(nameCC);
	m_id = Variant(userCC);
	m_pBrush = GetAxBrush(RGB(100, 100, 100));
}

void CMainWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (cx == 0 || cy == 0)
		return;
	
	ResizeOper(cx, cy);
}

CBrush* CMainWnd::GetAxBrush(COLORREF clr)
{
	return (CBrush*)m_pWnd->SendMessage(WM_USER, getBRUSH, (long)clr);
}

void CMainWnd::ResizeOper(int cx, int cy)
{
	if (m_pGroupWnd == NULL || m_pToolWnd == NULL || m_pTreeWnd == NULL || m_pBasketWnd == NULL || m_pOptDlg == NULL || m_pRTSWnd == NULL)
		return;

	int	minx = ((m_bTree) ? m_nTree : 0);
	int	miny = SIZE_TOOL + SIZE_GROUPMINY + ((m_bBasket) ? SIZE_BASKETMINY : 0);
	int	ncnt = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ALL));
	
	minx += (ncnt * SIZE_GROUPMINX);

	if (cx < minx)
		cx = minx;
	if (cy < miny)
		cy = miny;

	m_size = CSize(cx, cy);
	CRect	rect(0, GAP2, m_size.cx, m_size.cy);
//	CRect	rcTree(0, 0, 0, m_size.cy), rcBasket(0, 0, m_size.cx, m_size.cy);

	CRect	rcTree(GAP1, SIZE_DIVIDE*GAP2, 0, m_size.cy-2*SIZE_DIVIDE);//BUFFET
	CRect	rcBasket(0, SIZE_DIVIDE, m_size.cx, m_size.cy-2*SIZE_DIVIDE);//BUFFET

	BOOL	bBig = m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
//	CRect rcRTS(0, (bBig) ? SIZE_TOOL : SIZE_TOOL/2 + 1, 0, m_size.cy);
//	CRect	rcTool(0, 0, m_size.cx, (bBig) ? SIZE_TOOL : SIZE_TOOL/2 + 1), rcTemp(0, 0, 0, 0);
	CRect rcRTS(0, (bBig) ? SIZE_TOOL + SIZE_DIVIDE*3 : SIZE_TOOL/2 + SIZE_DIVIDE*4, 0, m_size.cy-2*SIZE_DIVIDE);
	CRect	rcTool((m_bTree) ? SIZE_DIVIDE+GAP1:GAP1, GAP2, m_size.cx-GAP1, (bBig) ? SIZE_TOOL : SIZE_TOOL/2 + 3), rcTemp(0, 0, 0, 0);

	m_rcDivide[0] = CRect(0, 0, 0, 0);
	m_rcDivide[1] = CRect(0, 0, 0, 0);

	m_rcPanel[0] = CRect(0, 0, 0, 0);
	m_rcPanel[1] = CRect(0, 0, 0, 0);
	m_rcPanel[2] = CRect(0, 0, 0, 0);

	if (m_bTree)
	{
		m_rcDivide[0].left = rcTree.right = m_nTree;
		m_rcDivide[0].right = m_rcDivide[0].left + SIZE_DIVIDE;
		m_rcDivide[0].top = 0;
		m_rcDivide[0].bottom = m_size.cy;
		
		//for rounding
		m_rcPanel[0].left = GAP1;
		m_rcPanel[0].right = rcTree.right + SIZE_DIVIDE;
		m_rcPanel[0].top = GAP2;
		m_rcPanel[0].bottom = m_size.cy - 1;
		//for rounding 

		//rcBasket.left = rcTool.left = m_rcDivide[0].right + GAP1;	
		rcBasket.left = m_rcDivide[0].right + GAP1;		
		rcTool.left = m_rcDivide[0].right + SIZE_DIVIDE + GAP1;	
				
		rcTemp = rect;
		rcTemp.left = rcTemp.left + SIZE_DIVIDE;//BUFFET
		rcTemp.right = rcTree.right - SIZE_DIVIDE; 
		rcTemp.bottom = m_size.cy - SIZE_DIVIDE;
					
		if (m_btOption.IsChecked())
		{
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - m_nOptHeight;
			m_pOptDlg->MoveWindow(rcTemp, TRUE);
			m_pOptDlg->ShowWindow(SW_SHOW);
			rcTemp.bottom = rcTemp.top;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
			
		}
		else
		{
			m_pOptDlg->ShowWindow(SW_HIDE);
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
		}

		m_btOption.MoveWindow(rcTemp, TRUE);
		rcTree.bottom = rcTemp.top;
		m_btOption.ShowWindow(SW_SHOW);
		m_pTreeWnd->MoveWindow(rcTree, TRUE);
		m_pTreeWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pTreeWnd->ShowWindow(SW_HIDE);		
		m_btOption.ShowWindow(SW_HIDE);
		m_pOptDlg->ShowWindow(SW_HIDE);
	}

	if (m_bRTS)
	{
		rcRTS.left = rcTool.left + SIZE_DIVIDE;
		rcRTS.right = rcRTS.left + SIZE_RTS;
		m_rcDivide[1].left = rcRTS.right - 1;
		m_rcDivide[1].right = m_rcDivide[1].left + SIZE_DIVIDE;
		m_rcDivide[1].top = rcTool.bottom;
		m_rcDivide[1].bottom = m_size.cy;
		
		// END MODIFY
		
		m_pRTSWnd->MoveWindow(rcRTS, TRUE);
		m_pRTSWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pRTSWnd->ShowWindow(SW_HIDE);
	}

	m_pToolWnd->MoveWindow(rcTool);
	m_pToolWnd->ShowWindow(SW_SHOW);

	//GRID BACKGROUND OUTBOX
	m_rcPanel[2].right = rcTool.right+GAP1;
	m_rcPanel[2].bottom = m_size.cy;
	
	//GRID OUTBOX
	m_rcPanel[1].left = m_bTree ? rcTree.right + SIZE_DIVIDE*2+ GAP1: GAP1;
	m_rcPanel[1].top = rcTool.bottom +SIZE_DIVIDE*3/2 ;//back
	m_rcPanel[1].right = rcTool.right;
	m_rcPanel[1].bottom = m_size.cy - GAP1;

	rcTool.top = rcTool.bottom + SIZE_DIVIDE*2;//BUFFET ADD SIZE_DIVIDE

	rcTool.left = m_bTree ? rcTree.right + SIZE_DIVIDE*8/3 : SIZE_DIVIDE*2/3;
	rcTool.left += m_bRTS ? rcRTS.Width() + SIZE_DIVIDE : 0;
	
	if (m_bBasket)
	{
		m_rcDivide[1].bottom = rcBasket.top = rcBasket.bottom - m_nBasket;
		m_rcDivide[1].bottom -= 1;
		m_rcDivide[1].top = m_rcDivide[1].bottom - SIZE_DIVIDE;
		m_rcDivide[1].left = rcTool.left - ((m_bTree) ? 2 : 0);
		m_rcDivide[1].right = rcTool.right;
		rcTool.bottom = m_rcDivide[1].top;
		
		m_pBasketWnd->MoveWindow(rcBasket, TRUE);
		m_pBasketWnd->ShowWindow(SW_SHOW);			
	}
	else
	{
		m_pBasketWnd->ShowWindow(SW_HIDE);
		rcTool.bottom = m_size.cy - SIZE_DIVIDE;//buffet
	}

	{
		rcTool.right = rcTool.right - SIZE_DIVIDE*2/3;//BUFFET
		m_pGroupWnd->MoveWindow(rcTool);
		m_nGroup = rcTool.Width();
		m_pGroupWnd->ShowWindow(SW_SHOW);
	}

	//InvalidateDivide();	
	Invalidate();//BUFFET 	
}
/*
void CMainWnd::ResizeOper(int cx, int cy)
{
	if (m_pGroupWnd == NULL || m_pToolWnd == NULL || m_pTreeWnd == NULL || m_pBasketWnd == NULL || m_pOptDlg == NULL || m_pRTSWnd == NULL)
		return;

	CRect rcTreeTemp(0, 0, 0, 0);
	if (m_bTree && m_pTreeWnd)
		m_pTreeWnd->GetClientRect(&rcTreeTemp);

	int	minx = ((m_bTree) ? rcTreeTemp.Width() : 0);
	// MODIFY PSH 20070918
	//int	miny = SIZE_TOOL + SIZE_GROUPMINY + ((m_bBasket) ? SIZE_BASKETMINY : 0);
	int	miny = SIZE_TOOL + (DEF_ROWH * 2) + ((m_bBasket) ? SIZE_BASKETMINY : 0);
	// END MODIFY
	int	ncnt = (int)m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETGROUP, MO_ALL));
	minx += (ncnt * SIZE_GROUPMINX);	
	if (cx < minx)
		cx = minx;
	if (cy < miny)
		cy = miny;
	int temp_size = REGURAR_XSIZE;

	if (cx%REGURAR_XSIZE != 0)
	{
		cx = temp_size * ncnt;
	}

	m_size = CSize(cx, cy);
	CRect	rect(0, 0, m_size.cx, m_size.cy);
	// MODIFY PSH 20070907
	//CRect	rcTree(0, 0, 0, m_size.cy), rcBasket(0, 0, m_size.cx, m_size.cy);
	BOOL	bBig = m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
	CRect	rcTree(0, (bBig) ? SIZE_TOOL : SIZE_TOOL/2 + 1, 0, m_size.cy - SIZE_TOOL), rcBasket(0, 0, m_size.cx, m_size.cy);
	CRect rcRTS(0, (bBig) ? SIZE_TOOL : SIZE_TOOL/2 + 1, 0, m_size.cy);
	//BOOL	bBig = m_pToolWnd->SendMessage(WM_MANAGE, MK_TOOLSIZE);
	// END MODIFY
	CRect	rcTool(0, 0, m_size.cx, (bBig) ? SIZE_TOOL : SIZE_TOOL/2 + 1), rcTemp(0, 0, 0, 0);
	m_rcDivide[0] = CRect(0, 0, 0, 0);
	m_rcDivide[1] = CRect(0, 0, 0, 0);
	m_rcDivide[2] = CRect(0, 0, 0, 0);
	if (m_bTree)
	{
		m_rcDivide[0].left = rcTree.right = m_nTree;
		m_rcDivide[0].right = m_rcDivide[0].left + SIZE_DIVIDE;
		//m_rcDivide[0].top = 0;
		m_rcDivide[0].top = rcTool.bottom;
		m_rcDivide[0].bottom = m_size.cy;
		
		// MODIFY PSH 20070907
		//rcBasket.left = rcTool.left = m_rcDivide[0].right + 1;		
		rcBasket.left = m_rcDivide[0].right + 1;		
		// END MODIFY
		
		rcTemp = rect;
		rcTemp.right = rcTree.right;
					
		if (m_btOption.IsChecked())
		{
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - m_nOptHeight;
			m_pOptDlg->MoveWindow(rcTemp, TRUE);
			m_pOptDlg->ShowWindow(SW_SHOW);
			rcTemp.bottom = rcTemp.top;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
			
		}
		else
		{
			m_pOptDlg->ShowWindow(SW_HIDE);
			rcTemp.right = rcTree.right;
			rcTemp.top = rcTemp.bottom - SIZE_BUTTONS;
		}

		m_btOption.MoveWindow(rcTemp, TRUE);
		rcTree.bottom = rcTemp.top;
		m_btOption.ShowWindow(SW_SHOW);
		m_pTreeWnd->MoveWindow(rcTree, TRUE);
		m_pTreeWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pTreeWnd->ShowWindow(SW_HIDE);		
		m_btOption.ShowWindow(SW_HIDE);
		m_pOptDlg->ShowWindow(SW_HIDE);
	}

	if (m_bRTS)
	{
		rcRTS.left = m_rcDivide[0].right;
		rcRTS.right = rcRTS.left + SIZE_RTS;
		m_rcDivide[1].left = rcRTS.right - 1;
		m_rcDivide[1].right = m_rcDivide[1].left + SIZE_DIVIDE;
		m_rcDivide[1].top = rcTool.bottom;
		m_rcDivide[1].bottom = m_size.cy;
		
		// END MODIFY
		
		m_pRTSWnd->MoveWindow(rcRTS, TRUE);
		m_pRTSWnd->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pRTSWnd->ShowWindow(SW_HIDE);
	}

	m_pToolWnd->MoveWindow(rcTool);
	m_pToolWnd->ShowWindow(SW_SHOW);
	rcTool.top = rcTool.bottom;

	rcTool.left = m_bTree ? rcTree.Width() + SIZE_DIVIDE : 0;
	rcTool.left += m_bRTS ? rcRTS.Width() + SIZE_DIVIDE : 0;

	// ADD PSH 20070907
//	if (m_bTree)
//	{
//		rcTool.left = rcTree.right + 1 + SIZE_DIVIDE;
//	}
	// END ADD
	
	if (m_bBasket)
	{
		m_rcDivide[2].bottom = rcBasket.top = rcBasket.bottom - m_nBasket;
		m_rcDivide[2].bottom -= 1;
		m_rcDivide[2].top = m_rcDivide[2].bottom - SIZE_DIVIDE;
		m_rcDivide[2].left = rcTool.left - ((m_bTree) ? 2 : 0);
		m_rcDivide[2].right = rcTool.right;
		rcTool.bottom = m_rcDivide[2].top;
		
		m_pBasketWnd->MoveWindow(rcBasket, TRUE);
		m_pBasketWnd->ShowWindow(SW_SHOW);			
	}
	else
	{
		m_pBasketWnd->ShowWindow(SW_HIDE);
		rcTool.bottom = m_size.cy;
	}

	{
		m_pGroupWnd->MoveWindow(rcTool);
		m_nGroup = rcTool.Width();
		m_pGroupWnd->ShowWindow(SW_SHOW);
	}

	InvalidateDivide();	
}
*/

void CMainWnd::DrawDivide(CDC* pDC)
{
/*	if (m_bTree)
	{
		IH::DrawDivideH(pDC, m_rcDivide[0]);		
	}

	if (m_bRTS)
	{
		IH::DrawDivideH(pDC, m_rcDivide[1]);
	}

	if (m_bBasket)
	{
		IH::DrawDivideV(pDC, m_rcDivide[2]);
	}*/

	pDC->FillSolidRect(&m_rcPanel[2], getAxColor(64));

	pDC->FillSolidRect(&m_rcPanel[0], getAxColor(192));
	if (m_bTree)
	DrawRoundBitmap(pDC, m_rcPanel[0], ROUND_PANEL);

	pDC->FillSolidRect(&m_rcPanel[1], getAxColor(192));
	DrawRoundBitmap(pDC, m_rcPanel[1], ROUND_PANEL);

}

LONG CMainWnd::OperTreeVisible(int opt)
{
	LONG	ret = 0;

	if (opt)	// set visible
	{
		switch (opt)
		{
		case IDC_BT_PREV:
			m_size.cx += (m_nTree + SIZE_DIVIDE);
			m_bTree = TRUE;
			break;
		case IDC_BT_NEXT:
			m_size.cx -= (m_nTree + SIZE_DIVIDE);			
			m_bTree = FALSE;
			break;
		}

		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_size.cx, m_size.cy));
		//SendMinMaxInfo();//BUFFET BLOCK
	}
	else			// get visible state
	{
		ret = m_bTree;
	}

	return ret;
}

LONG CMainWnd::OperRTSVisible(int opt)
{
	LONG ret = 0;

	if (opt)	// set visible
	{
		switch (opt)
		{
		case MO_SHOW:
			m_size.cx += (SIZE_RTS + SIZE_DIVIDE);
			m_bRTS = TRUE;
//			if (m_pRTSWnd)
//				((CRTSWnd*)m_pRTSWnd)->SendTR();
			break;
		case MO_HIDE:
			m_size.cx -= (SIZE_RTS + SIZE_DIVIDE);			
			m_bRTS = FALSE;
			if (m_pRTSWnd)
				((CRTSWnd*)m_pRTSWnd)->SendTR(TRUE);
			break;
		}
		
		
		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_size.cx, m_size.cy));

		SendMinMaxInfo();
	}
	else			// get visible state
	{
		ret = m_bRTS;
	}

	return ret;
}

LONG CMainWnd::OperBasketVisible(int opt)
{
	LONG	ret = 0;

	if (opt)	// set visible
	{
		switch (opt)
		{
		case MO_SHOW:
			m_bBasket = TRUE;
			break;
		case MO_HIDE:
			m_bBasket = FALSE;
			break;
		}	
		ResizeOper(m_size.cx, m_size.cy);
	}
	else			// get visible state
	{
		ret = m_bBasket;		
	}

	return ret;
}

//+, - 버튼 클릭시
/*void CMainWnd::OperSize(int opt, CSize size)
{
	switch (opt)
	{
	case MO_MINUS:
		m_size = m_size - size;
		m_size.cx -= 2;
		break;
	case MO_PLUS:
		m_size = m_size + size;
		break;
	}

	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_size.cx, m_size.cy));
}*/

void CMainWnd::OperSize(int opt, CSize size)
{
	CSize szTemp = m_size;
	
	switch (opt)
	{
	case MO_MINUS:
		szTemp = szTemp - size;
		szTemp.cx -= 2;
		break;
	case MO_PLUS:
		szTemp = szTemp + size;
		break;
	}
	
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(szTemp.cx, szTemp.cy));
	
	if (m_size.cx != szTemp.cx || m_size.cy != szTemp.cy)
		m_pWnd->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_size.cx, m_size.cy));
	
	MoveWindow(0, 0, m_size.cx, m_size.cy);
}

void CMainWnd::OpenScreen(int type, CString data)
{
	m_pWnd->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.operator LPCTSTR());
}

LRESULT CMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_ERASEBKGND)
		return 1;
	else if ( message == WM_DESTROY )
		m_pGroupWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_INTSAVE, 0));

	return CWnd::WindowProc(message, wParam, lParam);
}

void CMainWnd::SelectGroup(int kind)
{
	if (m_pTreeWnd)
		m_pTreeWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);

	if (m_pToolWnd)
		m_pToolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);	
}

void CMainWnd::SendOper(class CSendData* sdata)
{
	SendMode(true);
	sendTR(sdata->GetTRCODE(), sdata->GetData(), sdata->GetDataLen(), sdata->GetDataKey());
}

BOOL CMainWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if (LOWORD(wParam) == IDC_BT_CONFIG)
	{
		ResizeOper(m_size.cx, m_size.cy);
	}
	return CWnd::OnCommand(wParam, lParam);
}

DWORD CMainWnd::SearchOption(WORD kind, DWORD option)
{
	DWORD	ret = 0;
	switch (kind)
	{
	case MO_SET:
		m_pOptDlg->SetOption((int)option);
		break;
	case MO_GET:
		ret = m_pOptDlg->GetOption();
		break;
	case MO_ADD:
		m_pOptDlg->AddOption((int)option);
		break;
	case MO_DEL:
		m_pOptDlg->DelOption((int)option);
		break;
	}
	return ret;
}

void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE("CMainWnd::OnLButtonDown\n");
	if (m_rcDivide[0].PtInRect(point))
	{
		SetCapture();
		m_nResize = MO_LEFTRIGHT;	
		m_xPosMove = m_xPos = point;
		CRect	rect = m_rcDIVIDE = m_rcDivide[0];
		rect.left = SIZE_TREEMINX;
		rect.right = rect.left + 200;
		ClientToScreen(rect);
		ClipCursor(rect);
		DrawMove(m_rcDIVIDE);
	}
	else if (m_rcDivide[2].PtInRect(point))
	{
		SetCapture();
		m_nResize = MO_TOPBOTTOM;
		m_xPosMove = m_xPos = point;
		CRect	rect = m_rcDIVIDE = m_rcDivide[2];
		rect.bottom = m_size.cy - SIZE_BASKETMINY;
		rect.top = rect.bottom - 300;
		ClientToScreen(rect);
		ClipCursor(rect);
		DrawMove(m_rcDIVIDE);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CMainWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	ClipCursor(NULL);
	
	if ((m_nResize == MO_LEFTRIGHT && point.x != m_xPos.x) || (m_nResize == MO_TOPBOTTOM && point.y != m_xPos.y))
	{
		switch (m_nResize)
		{
		case MO_LEFTRIGHT:
			m_nTree += (point.x - m_xPos.x);
			break;
		case MO_TOPBOTTOM:
			m_nBasket -= (point.y - m_xPos.y);
			break;
		}
		
		ResizeOper(m_size.cx, m_size.cy);
	}

	DrawMove(m_rcDIVIDE);
	m_nResize = MO_NONE;
	m_rcDIVIDE.SetRectEmpty();
	m_xPosMove = m_xPos = CPoint(-1, -1);
	//InvalidateDivide();
	Invalidate();//BUFFET 
	CWnd::OnLButtonUp(nFlags, point);
}

void CMainWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_nResize != MO_NONE)
	{
		DrawMove(m_rcDIVIDE);

		switch (m_nResize)
		{
		case MO_LEFTRIGHT:
			m_rcDIVIDE.OffsetRect(point.x - m_xPosMove.x, 0);
			m_xPosMove.x = point.x;
			break;
		case MO_TOPBOTTOM:
			m_rcDIVIDE.OffsetRect(0, point.y - m_xPosMove.y);
			m_xPosMove.y = point.y;			
			break;
		}
		
		DrawMove(m_rcDIVIDE);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CMainWnd::DrawMove(CRect rect)
{
	CWindowDC	dc(this);
	int	nMode = dc.SetROP2(R2_XORPEN);
	CBrush	*oldbrush = dc.SelectObject(m_pBrush);
	dc.Rectangle(rect);
	dc.SelectObject(oldbrush);
	dc.SetROP2(nMode);
}

BOOL CMainWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint	pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if (m_rcDivide[0].PtInRect(pt))
	{
		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_LR));
		return TRUE;
	}
	else if (m_rcDivide[2].PtInRect(pt))
	{
//		MessageBox("IDC_CURSOR_TB");
//		::SetCursor(AfxGetApp()->LoadCursor(IDC_CURSOR_TB));
		return TRUE;	
	}
		
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CMainWnd::InvalidateDivide()
{
	CRect	rect = m_rcDivide[0] + CRect(0, 0, 1, 0);
	InvalidateRect(rect, FALSE);
	InvalidateRect(m_rcDivide[2], FALSE);
}

void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_rcDivide[0].PtInRect(point))
	{
		m_nTree = SIZE_TREE;
		ResizeOper(m_size.cx, m_size.cy);
	}
	else if (m_rcDivide[2].PtInRect(point))
	{
		m_nBasket = SIZE_BASKET;
		ResizeOper(m_size.cx, m_size.cy);
	}
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CMainWnd::SendMode(bool bSend /* = true */)
{
	if (bSend)
	{
		Enable(MO_ALLWND, !bSend);
		KillTimer(1000);
		SetTimer(1000, 10 * 1000, NULL);
	}
	else
	{
		Enable(MO_ALLWND, !bSend);
	}
}

void CMainWnd::Enable(int kind, bool bEnable /* = true */)
{
	if (kind & MO_MAIN)
		EnableWindow(bEnable);
	if (kind & MO_TOOL)
		if (m_pToolWnd)
			m_pToolWnd->EnableWindow(bEnable);
	if (kind & MO_GROUP)
		if (m_pGroupWnd)
			m_pGroupWnd->EnableWindow(bEnable);
	if (kind & MO_BASKET)
		if (m_pBasketWnd)
			m_pBasketWnd->EnableWindow(bEnable);
	if (kind & MO_TREE)
	{
		if (m_pTreeWnd)
			m_pTreeWnd->EnableWindow(bEnable);
			
		if (m_pOptDlg)
			m_pOptDlg->EnableWindow(bEnable);
		
		if (m_btOption.m_hWnd)
			m_btOption.EnableWindow(bEnable);
		
	}
	if (kind & MO_RTS)
		if (m_pRTSWnd)
			m_pRTSWnd->EnableWindow(bEnable);
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 1000)
	{
		KillTimer(1000);
		SendMode(false);		
	}
	CWnd::OnTimer(nIDEvent);
}

UINT ParsingRTM(LPVOID lpvoid)
{
	CMainWnd* pMain = (CMainWnd *)lpvoid;
	if (pMain == NULL)
		return FALSE;

	char* pBytes = NULL;
	while (pMain->m_bThreadRTM)
	{
		if (pMain->m_arBytesRTM.GetUpperBound() < 0)
		{
			pMain->WaitRTM();
			continue;
		}
		pMain->m_syncRTM.Lock();
		pBytes = pMain->m_arBytesRTM.GetAt(0);
		pMain->m_arBytesRTM.RemoveAt(0);
		pMain->m_syncRTM.Unlock();

		pMain->doRTM(pBytes);
		delete [] pBytes;
		pBytes = NULL;
		
	}
	return 0;
}

bool CMainWnd::WaitRTM()
{
	if (!m_pThreadRTM)
		return false;
	DWORD res = WaitForSingleObject(m_hEventRTM, 3000);
	switch (res)
	{
	case WAIT_OBJECT_0:
		return true;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	case WAIT_TIMEOUT:
		return false;
	}
	return true;
}

void CMainWnd::doRTM(char *pBytes)
{
	if (m_bRTMReady)	// RTM Ready 판별(20070817_장희)
	{
		CRecvData	rdata;
		rdata.SetData("", 0, LPARAM(pBytes));
		m_pGroupWnd->SendMessage(WM_MANAGE, MK_RTSDATA, (LPARAM)&rdata);
	}
}

void CMainWnd::initRTM()
{
	if (m_bThreadRTM)
		return;

	m_bThreadRTM = true;
	m_pThreadRTM = AfxBeginThread((AFX_THREADPROC) ParsingRTM, (LPVOID) this, THREAD_PRIORITY_NORMAL);
	m_hEventRTM = ::CreateEvent(NULL, FALSE, FALSE, NULL);	
}

void CMainWnd::stopRTM()
{
	if (!m_bThreadRTM)
		return;
	
	m_bThreadRTM = false;
	SetEvent(m_hEventRTM);
	m_pThreadRTM->PostThreadMessage(WM_QUIT, 0, 0);
	switch (WaitForSingleObject(m_pThreadRTM->m_hThread, 1000))
	{
	case WAIT_OBJECT_0:
	case WAIT_FAILED:
	case WAIT_ABANDONED:
		break;
	case WAIT_TIMEOUT:
		TerminateThread(m_pThreadRTM->m_hThread, 0);
		break;
	}
	removeRTM();
}

void CMainWnd::removeRTM()
{
	m_syncRTM.Lock();
	char* pBytes = NULL;
	for (int ii = m_arBytesRTM.GetUpperBound(); ii >= 0; ii--)
	{
		pBytes = m_arBytesRTM.GetAt(ii);
		delete [] pBytes;	pBytes = NULL;
	}
	m_arBytesRTM.RemoveAll();
	m_syncRTM.Unlock();
}

CString CMainWnd::GetDataTitle(int nKind)
{
	CString strTitle = (char*)m_pTreeWnd->SendMessage(WM_MANAGE, MK_GETDATATITLE, nKind);
	return strTitle;
}

void CMainWnd::SetRTSTreeData(CGridWnd* pGrid)
{
	if (m_pRTSWnd && m_bRTS && pGrid)
		((CRTSWnd*)m_pRTSWnd)->SetSelectItem(pGrid->GetTitle(), pGrid->GetKind());
}

BOOL CMainWnd::IsValidCode(CString strCode)
{
	if (m_pGroupWnd)
		return ((CGroupWnd*)m_pGroupWnd)->IsValidCode(strCode);

	return FALSE;
}

void CMainWnd::SendMinMaxInfo()
{
/*	int nWidth = 0;
	int nGroupCount;
	CRect rcClient;
	int nKey = m_pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), NULL);

	if (nKey == 0)
		return;

	if (m_pGroupWnd)
	{
		nGroupCount = ((CGroupWnd*)m_pGroupWnd)->GetGroupCount();
		nWidth = SIZE_GROUPMINX * nGroupCount;
		nWidth = nWidth + SIZE_DIVIDE * (nGroupCount - 1);
	}

	if (m_bTree)
	{
		((CGroupWnd*)m_pTreeWnd)->GetClientRect(&rcClient);
		nWidth += rcClient.Width();
		nWidth += SIZE_DIVIDE;
	}

	if (m_bRTS)
	{
		nWidth += SIZE_RTS;
		nWidth += SIZE_DIVIDE;
	}

	m_MMI.ptReserved.x = 4;
	m_MMI.ptReserved.y = 0;
	m_MMI.ptMinTrackSize.x = max(nWidth, SIZE_MINWIDTH);

	if(nGroupCount == 1)
	{
			m_MMI.ptMinTrackSize.x = 511;
			ResizeOper(m_MMI.ptMinTrackSize.x, 340);
	}
	AfxGetMainWnd()->SendMessage(WM_AXIS, MAKEWPARAM(axMinMaxInfo, nKey), (LPARAM)&m_MMI);
*/
}

void CMainWnd::DrawRoundBitmap(CDC *pDC, CRect rc, int nKind)
{
	CRect bitRc;
	
	switch(nKind)
	{
	case ROUND_PANEL:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "0_rb.bmp");
		break;
	case ROUND_CONTENTS:
		//left + top
		bitRc.SetRect(rc.left, rc.top, rc.left + 5, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lt.bmp");
		
		//right + top
		bitRc.SetRect(rc.right - 5, rc.top, rc.right, rc.top + 5);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rt.bmp");

		//left + bottom
		bitRc.SetRect(rc.left, rc.bottom - 5, rc.left + 5, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_lb.bmp");

		//right + bottom
		bitRc.SetRect(rc.right - 5, rc.bottom - 5, rc.right, rc.bottom);
		DrawBitmap(pDC, bitRc, "axpanel" + m_strPal + "1_rb.bmp");
		break;
	}
}

void CMainWnd::DrawBitmap(CDC *pDC, CRect rc, CString strImage)
{
	CString strPath = Variant(homeCC, "");
	CString sImageDir = strPath + "\\image\\" + strImage;

	HBITMAP BitMap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), sImageDir, IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	CDC		memDC;

	memDC.CreateCompatibleDC(pDC);

	if (memDC.m_hDC != NULL) 
		memDC.SelectObject(BitMap);

	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	DeleteObject(BitMap);
	memDC.DeleteDC();
}

CString CMainWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_home, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}
