// base.cpp : implementation file
//

#include "stdafx.h"
#include "IB100000.h"
#include "base.h"
#include "Pane.h"
#include "Screen.h"
#include "configbar.h"
#include "usetup.h"
#include "xtab.h"

#define	BACKGROUND	RGB(201, 238, 205)
#define	GAP		0
#define	WM_LOAD		WM_USER+9631
#define	WM_DATATRANS	WM_USER+2424
#define UM_PASSWORD	WM_USER+1111
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//const double g_xPosRate[] = {0.6835, 0.5, 0.5, 0.3165};
const double g_xPosRate[] = {0.6685, 0.5, 0.5, 0.3315};
const double g_yPosRate[] = {0.37, 0.63, 0.63, 0.37};

//const double g_yPosRate[] = {0.3313, 0.6687, 0.6687, 0.3313};
#define		MAP_PROC			_T("OnFromDLLValue")
/////////////////////////////////////////////////////////////////////////////
// CBase
CBase::CBase()
{
	m_config = NULL;
	m_lyRate = 1.f;
	m_strCode = _T("");
	m_strCurrValue = _T("");
	m_strTrigger = _T("");
	m_TriggerKey = 0;
	m_temp = NULL;
}

CBase::~CBase()
{
}


BEGIN_MESSAGE_MAP(CBase, CWnd)
	//{{AFX_MSG_MAP(CBase)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUSER)
	ON_MESSAGE(WM_XMSG, OnXMSG)
	ON_MESSAGE(WM_XMSG2, OnXMSG2)
	ON_MESSAGE(WM_XMSG3, OnXMSG3)
	ON_MESSAGE(WM_LOAD, OnLoad)

	ON_MESSAGE(WM_DATATRANS, OnDataTrans)
	ON_MESSAGE(UM_PASSWORD, OnMyPassword)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBase message handlers

LONG CBase::OnUSER(WPARAM wParam, LPARAM lParam)
{
	//const int iCnt = m_sList.GetSize();
	//int i{};
	//HWND hChild{};
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_DOMINO:	
		Configuration((char *) lParam);			
		break;
	case DLL_SETPAL:	
		ChangeColor();					
		break;
	case DLL_SETFONT:	
	case DLL_SETFONTx:
		ChangeFont(HIWORD(wParam), (char *) lParam);	
		break;
	case DLL_AXIS:		
		DLLAxis(HIBYTE(LOWORD(wParam)), HIWORD(wParam), lParam);
		break;

	case DLL_TRIGGER:
		ParseTriggerValue((char *) lParam);
/*		for(i=0; i<iCnt; ++i)
		{
			Maplist *pMap = &(m_sList.GetAt(i));
			//m_parent->InvokeHelper(DI_WIZARD, DISPATCH_METHOD, VT_EMPTY, (void *)NULL, (BYTE *)(VTS_I4 VTS_I4), MAKELONG(setFDC, pMap->paneWnd->m_key), (LPARAM)(const char*)lParam);
			hChild = FindWindowEx(pMap->paneWnd->GetSafeHwnd(), NULL, "AfxWnd42d", "");
			if (hChild)
				::SendMessage(hChild, WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)(const char*)lParam);
		}
*/		break;

	default:
		break;
	}
	return 0;
}

#define		P_TAB				0x09
void CBase::ParseTriggerValue(CString sData)
{
	
	int iFind = sData.Find(P_TAB);
	
	if(iFind < 0) return;
	CString sKey = sData.Left(iFind);
	CString sParse = sData.Mid(iFind+1);

	if(sParse.Find("FOCUS") > -1 && sParse != "FOCUS")
	{
		return;
	}

CString slog;
slog.Format("[IB100000] 시작 sData = [%s] sKey = [%s] sParse = [%s]\n", sData, sKey, sParse);
//OutputDebugString(slog);

	if( sKey == "BUF_1000")
	{
		sData = sData.Right(sData.GetLength() - iFind - 1);  
		//MapKey tab Key tab val
		CStringArray astrResult;
		astrResult.RemoveAll();
		iFind = 0;

		bool bfindPWSD = false;
		while(1)
		{
			iFind = sData.Find(P_TAB); 
			if(iFind < 0 ) 
			{
				if(!sData.IsEmpty() )astrResult.Add(sData);
				break;
			}
		
			astrResult.Add( sData.Left(iFind) ); 

			if(sData.Left(iFind) == "PSWD")
			  bfindPWSD= true;

			sData = sData.Right(sData.GetLength() - iFind - 1 );  

			if(bfindPWSD == true)
			{
				iFind = sData.Find(P_TAB);
				m_strAcc = sData.Left(iFind);
				bfindPWSD = false;
			}

			if(sData == "")break;
		}

		if(astrResult.GetSize() > 1)
		{
			m_iCurMap = atoi( astrResult[0] );
			astrResult.RemoveAt(0); 

			bool bpass = false, bsend=false;
			if (astrResult[0].Find("SEND")  > -1)
			{
				bsend = true;
				astrResult.RemoveAt(0); 
			}

			
			for(int i=0; i<astrResult.GetSize(); i+=2)
			{
				if( i== 0)
					m_strCurKey = astrResult[i];

				if( astrResult[i] == "PSWD") bpass = true;
				m_astrSaveList.SetAt(astrResult[i], astrResult[i+1]);
			}

			if(bpass)
				SetTimer(TIMER_WAIT, 20, NULL);
			else
			{
				if(bsend)
				{
					
					SetTimer(TIMER_WAIT1, 20, NULL);
				}
			}
		}
	}
	else if(sKey == "1301")
	{
/*		m_strCode = sData.Right(sData.GetLength() - iFind - 1);
		CString string;
		string.Format("1301\t%s", m_strCode);

		Variant(triggerCC, string);
*/
		CString s;
		s.Format("1301 DATA [%s]\n",sData);
//		OutputDebugString(s);
		if (m_strTrigger != sData)			 //같은 트리거 연속처리 방지(종합화면특성상 4번 발생)
		{
			m_strTrigger = sData;			 //데이터 저장

// 			CString code,string;
// 			code = sData.Right(sData.GetLength() - iFind - 1);
// 			
// 			switch (GetCodeType(code))
// 			{
// 			case kospiCODE:
// 				string.Format("%s\t%s", HCOD, code);
// 				break;
// 			case futureCODE:
// 				string.Format("%s\t%s", FCOD, code);
// 				break;
// 			case optionCODE:
// 				string.Format("%s\t%s", OCOD, code);
// 				break;
// 			case foptionCODE:
// 				string.Format("%s\t%s", PCOD, code);
// 				break;
// 			case indexCODE:
// 				string.Format("%s\t%s", UCOD, code);
// 				break;
// 			case sinjuCODE:
// 				string.Format("%s\t%s", SINJUCOD, code);
// 				break;
// 			case elwCODE:
// 				string.Format("%s\t%s", HCOD, code);
// 				Variant(triggerCC, string);
// 				//					m_grid->SetFocus();
// 				string.Format("%s\t%s", ELWCOD, code);
// 				break;
// 			case thirdCODE:
// 				string.Format("%s\t%s", THIRDCOD, code);					
// 				break;
// 			}
// 
// 			Variant(triggerCC, string);
// 			Variant(codeCC,code);
			
			m_parent->PostMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)(LPCSTR)m_strTrigger);
			//SetTimer(9999, 100, NULL);		//트리거 처리중 트리거 발생이 안되어 시간차를 두어 트리거를 발생시킴

			if(m_TriggerKey > 0)
			{
				CString send;
				const int major = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
				send.Format("%s%c%s", "sendTrigger", P_TAB, sParse); 
				
				m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_TriggerKey) ), (LPARAM)(LPCTSTR)send);		
			}
		}
	}
	else if(sKey == JPRC)
	{
		CString string, mapNumber;
		string.Format("ed_jprc\t%s", sData);
		mapNumber = "17412";
		m_parent->SendMessage(WM_USER, MAKEWPARAM(formDLL, -1), (LPARAM)sData.operator LPCTSTR());

		Variant(triggerCC, string);
	}
	//test
	/*
	else if(sKey == "1000handle")	//2013.02.25 KSJ 2022T0의 맵핸들을 트리거로 받음
	{
		m_strMapHandle = sData.Mid(sData.Find(P_TAB) +1 , sData.GetLength() - sData.Find(P_TAB));
		
		CString send, temp;
		int major = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
		send.Format("%s%c%s", "Set1000Handle", P_TAB, m_strMapHandle); 
		// 					OutputDebugString("[KSJ]SETTRIGGER[" + send + "]");
		m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_TriggerKey) ), (LPARAM)(LPCTSTR)send);	
		
		temp.Format("[KSJ] 1000handle m_strMapHandle[%s], m_TriggerKey[%d]", m_strMapHandle, m_TriggerKey);
		OutputDebugString(temp);
	}
	*/
	else if(sKey == "$PHONEPAD" || sKey == "pswd")
	{
		CString sdata;
		sdata = sParse;
		sdata.TrimRight();
		sdata.TrimLeft();

		SendToMap(m_iCurMap, "PSWD", m_strAcc + sdata, true);
	}
	else
	{
		sKey.MakeUpper(); 
		if (sKey == "SETTRIGGER")
		{
			//2013.02.19 KSJ SetTrigger(cstring data, cstirng mapHandle) 함수의 인자값이 바뀜.
			//sParse에 종목코드,맵핸들 이렇게 받아짐.

OutputDebugString("---------------------[IB100000]");
slog.Format("[IB100000] sParse = [%s]", sParse);
OutputDebugString(slog);

			const int nIndex = sParse.Find(",");
			CString strHandle;
			sData = sParse.Left(nIndex);
			
			strHandle = sParse.Mid(nIndex + 1, sParse.GetLength() - nIndex);
			
OutputDebugString("[KSJ] sData[" + sData + "] strHandle[" + strHandle + "]\n");

slog.Format("[IB100000] sData = [%s] strHandle = [%s] sParse = [%s]\n", sData, strHandle, sParse);
OutputDebugString(slog);

slog.Format("[IB100000] m_strTrigger = [%s] sData = [%s] m_TriggerKey = [%d]\n", m_strTrigger, sData, m_TriggerKey);
OutputDebugString(slog);
			
			if (m_strTrigger != sData)			 //같은 트리거 연속처리 방지(종합화면특성상 4번 발생)
			{
				m_strTrigger = sData;			 //데이터 저장
				m_strTrigger.Format("1301\t%s", sData)	;

				m_parent->PostMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)(LPCSTR)m_strTrigger);
				//SetTimer(9999, 100, NULL);		//트리거 처리중 트리거 발생이 안되어 시간차를 두어 트리거를 발생시킴
				
				if(m_TriggerKey > 0)
				{
					CString send, temp;
					const int major = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
					send.Format("%s%c%s", "sendTrigger", P_TAB, sData); 
temp.Format("[IB100000] major[%d] send=[%s]\n", major, send);
OutputDebugString(temp);

					m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, m_TriggerKey) ), (LPARAM)(LPCTSTR)send);		
				}
			}
		}
	}
	//m_astrSaveList
}

int CBase::GetCodeType(CString code)
{
	const int codeL = code.GetLength();
	if (codeL <= 0) return 0;
	
	const int result = m_parent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (long)(LPCTSTR)code);
	
	switch (result)
	{
	case kospiType:				// kospi code
	case kosdaqType:			// kosdaq code
	case etfType:				// etf code
		return	kospiCODE;;
	case elwType:				// ELW
		return	elwCODE;
	case futureType:
		return	futureCODE;		// 선물
	case sfutureType:
		return sfCODE;		
	case callType:				// call option
	case putType:				// put  option
		return  optionCODE;
	case indexType:				// 업종
		return	indexCODE;
	case thirdType:				// 제3시장
		return	thirdCODE;
	case sinjuType:				// 신주인수권
		return	sinjuCODE;
	case mufundType:			// 뮤츄얼펀드
	case reitsType:				// 리츠
	case hyfundType:			// 하이일드펀드
		return 200;
	}
	
	return 0;
}

BOOL CBase::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
// 	NM_GVITEM* nmgv = (NM_GVITEM*) lParam;
// 	if (wParam != IDC_GRID)
// 		return CDialog::OnNotify(wParam, lParam, pResult);	
// 	
// 	switch (nmgv->hdr.code)
// 	{
// 	case GVNM_SELCHANGED:
// 		CString string;
// 		sData = sData.Right(sData.GetLength() - iFind - 1);  
// 		string.Format("1301\t%s", sData);
// 		Variant(triggerCC, string);
// 		break;
// 	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

void CBase::SendDataToMap(int iMapKey, CString sKey, CString sData)
{
	const int major = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	CString send, spass;
	send.Format("%s%c%s, %s", MAP_PROC, P_TAB, sKey, sData); 

/*
CString slog;
slog.Format("[IB100000] SendDataToMap send = [%100s]", send);
OutputDebugString(slog);
*/
	m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, iMapKey)), (LPARAM)(LPCTSTR)send);

	m_iCurMap = -1;

	SetTimer(TIMER_WAIT, 10, NULL);
	
}

void CBase::SendToMap(int iMapKey, CString sKey, CString sVal, bool bPhonepad)
{
	CPane* pane{};
	Maplist pList{};
	const int major = m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC));
	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pList = m_sList.GetAt(ii);
		pane = pList.paneWnd;
		if( iMapKey != pane->m_key || bPhonepad)
		{
			CString send;
			send.Format("%s%c%s, %s", MAP_PROC, P_TAB, sKey, sVal); 
/*
CString slog;
slog.Format("[IB100000] SendToMap iMapKey = [%d] sKey = [%s] sVal = [%s]", iMapKey, sKey, sVal);
OutputDebugString(slog);
*/
			m_parent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, pane->m_key) ), (LPARAM)(LPCTSTR)send);
			m_strCurrValue.Empty();
		}
	}
}




LONG CBase::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	return 0L;
/**
	int	stat = LOWORD(lParam);
	int	action = HIWORD(lParam);
	CPane*	pane = (CPane *) wParam;
	
	if (!pane)	
		return 0;

	CRect	cRc, sRc, wRc;
	
	GetClientRect(cRc);

	CSize	size = pane->GetMapInfo();

	switch (action)
	{
	case EX_VERT:
		switch (stat)
		{
		case STAT_NONE:
			sRc = pane->GetRect();
			pane->SetWindowPos(&wndTop, sRc.left, sRc.top, 
				sRc.Width(), sRc.Height(), SWP_SHOWWINDOW);
			break;
		case STAT_PUSH:
			sRc.CopyRect(&cRc);
			sRc.DeflateRect(GAP*2, GAP);
			wRc = pane->GetRect();
			sRc.left = wRc.left;
			sRc.right = wRc.right;
			pane->SetWindowPos(&wndTop, sRc.left, sRc.top, 
				sRc.Width(), sRc.Height(), SWP_SHOWWINDOW);
			break;
		default:break;
		}
		break;
	case EX_HORI:
		switch (stat)
		{
		case STAT_NONE:
			sRc = pane->GetRect();
			pane->SetWindowPos(&wndTop, sRc.left, sRc.top, 
				sRc.Width(), sRc.Height(), SWP_SHOWWINDOW);
			break;
		case STAT_PUSH:
			sRc.CopyRect(&cRc);
			sRc.DeflateRect(GAP, GAP);
			wRc = pane->GetRect();
			sRc.top = wRc.top;
			sRc.bottom = wRc.bottom;
			pane->SetWindowPos(&wndTop, sRc.left, sRc.top, 
				sRc.Width(), sRc.Height(), SWP_SHOWWINDOW);
			break;
		default:break;
		}
		break;
	default:break;
	}

	if (!m_config)	return 0;

	m_config->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
	return 0;
**/
}

LONG CBase::OnXMSG2(WPARAM wParam, LPARAM lParam)
{
	const int mode = LOWORD(lParam);
	const int width = HIWORD(lParam);

	if (!m_config)	return 0;

	CRect	cRc;
	GetClientRect(cRc);
	if (mode == MODE_DEFAULT)
	{
		m_config->SetWindowPos(&wndTop, 0/*GAP*/, cRc.bottom - (CONFIG_HEIGHT/*+GAP*/),
			cRc.right/* - GAP*2*/, CONFIG_HEIGHT, SWP_SHOWWINDOW);
	}
	else
	{
		m_config->SetWindowPos(&wndTop, 0/*GAP*/, cRc.bottom - (CONFIG_HEIGHT/*+GAP*/),
			width, CONFIG_HEIGHT, SWP_SHOWWINDOW);
	}
	return 0;
}

LONG CBase::OnXMSG3(WPARAM wParam, LPARAM lParam)
{
	if (!LoadInfo())
		Variant(guideCC, "화면구성정보오류입니다.");
	else
	{
		m_config->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	}

	return 0;
}

LONG CBase::OnLoad(WPARAM wParam, LPARAM lParam)
{
	if (m_domino.IsEmpty())
		LoadInfo();
	else	
		LoadInfo(m_domino);

	return 0;
}

static BOOL CALLBACK MyEnumProc(HWND hwnd, LPARAM lParam)
{
	TCHAR buf[16]{};
	GetClassName(hwnd, buf, sizeof(buf) / sizeof(TCHAR));
	AfxMessageBox(buf);
	if (_tcscmp(buf, _T("SCREEN")) == 0)  // special classname for menus
	{
		*((HWND*)lParam) = hwnd;	 // found it
		//AfxMessageBox("Find it");
		CString tmps; 
		tmps.Format("%d", (long)hwnd);
		AfxMessageBox(tmps);
		return FALSE;
	}
	return TRUE;
}

LONG CBase::OnDataTrans(WPARAM wParam, LPARAM lParam)
{
	CString tmp = (char*) lParam;

	if(tmp=="QUEST_DATA") tmp = m_data;
	else m_data = tmp;
	CPane* pane{};
	HWND cont_hwnd{};
	CString tmps;

	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		cont_hwnd = 0;
		pane = m_sList.GetAt(ii).paneWnd;
		
		cont_hwnd = ::FindWindowEx(pane->m_screen->m_hWnd, 0, NULL, "CX_DATATRANS");
		if(((int)cont_hwnd)>32)
		{
			::SendMessage(cont_hwnd, WM_DATATRANS, 0, (long)m_data.operator LPCTSTR());
		}
	}
	
	return 0;
}

void CBase::DLLAxis(int type, int key, LPARAM lParam)
{
	CPane* pane{};
	int	ii{};
	switch (type)
	{
	case renVIEW:
		for (ii = 0; ii < m_sList.GetSize(); ii++)
		{
			pane = m_sList.GetAt(ii).paneWnd;
			if (pane->GetKey() != key)
				continue;
			pane->LoadScreen((char *) lParam);
			break;
		}
		break;

	case sizeVIEW:
		for (ii = 0; ii < m_sList.GetSize(); ii++)
		{
			pane = m_sList.GetAt(ii).paneWnd;
			if (pane->GetKey() != key)
				continue;
			const int cx = LOWORD(lParam);
			const int cy = HIWORD(lParam);
			pane->ChangeSize(cx, cy);
			break;
		}

		break;
	case printVIEW:
		break;
	default:
		break;
	}
}

void CBase::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
//	CRect	 cRc;

//	GetClientRect(cRc);
//	dc.SetBkColor(RGB(255, 0, 0)/*BACKGROUND*/);
//	dc.FillSolidRect(cRc, 0xff00ff); 
	//dc.TextOut(cRc.left, cRc.top, TRANSPARENT, &cRc, "", 0, NULL);
}

BOOL CBase::CreateMap(CWnd* parent, CRect wRc)
{
	const DWORD	dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

	 if (!CWnd::Create(NULL, "BASE", dwStyle, wRc, parent, 1))
		 return FALSE;

	m_parent = parent;
	Variant(titleCC, "[1000] 맞춤종합");
	m_home = Variant(homeCC);
	m_user = Variant(nameCC);

	m_bgColor = GetIndexColor(BACK_INDEX);
	m_contentsbgColor = GetIndexColor(CONTENTS_BACK_INDEX);
	m_contentslineColor = GetIndexColor(CONTENTS_BORDER_INDEX);

	LoadBitmap();

	return	TRUE;
}

CString CBase::Variant(int comm, CString data)
{
	CString retvalue = _T("");
	
	char* dta = (char*)m_parent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue.Format("%s", dta);
	
	return retvalue;

}

DWORD CBase::LoadInfo_C(CString sKey, char* szdata)
{
	char	buf[256];
	CString file;
	file.Format("%s\\tab\\종합config.ini", m_home);

	GetPrivateProfileString("GENERAL", "section", "STOCK", buf, sizeof(buf), file);
	CString currSection(buf);

	GetPrivateProfileString("GENERAL", "key", "00", buf, sizeof(buf), file);
	CString currkey(buf);

	GetPrivateProfileString(currSection, currkey,  "", buf, sizeof(buf), file);
	CString currSec(buf);

	DWORD	dres = 0;
	if( (dres=GetPrivateProfileString(currSec, sKey,  "", buf, sizeof(buf), file)) > 0)
		strcpy(szdata, buf);

	return dres;
}

BOOL CBase::LoadInfo()
{

	char		buf[1024]{};
	char		key[32]{}, sec[32]{};
	CString		file, keys, tmpS;

	bool	clear = false;
	file.Format("%s\\tab\\종합config.ini", m_home);
	DWORD dw = GetPrivateProfileString("GENERAL", "option", "", key, sizeof(key), file);
	if (dw > 0)
	{
		file.Format("%s\\user\\%s\\종합.ini", m_home, m_user);
		dw = GetPrivateProfileString("GENERAL", "section", "", sec, sizeof(sec), file);
		if (dw <= 0)
		{
			file.Format("%s\\tab\\종합config.ini", m_home);
			WritePrivateProfileString("GENERAL", "option", "", file);
		}
	}
	else	clear = true;
	
	dw = GetPrivateProfileString("GENERAL", "section", "", sec, sizeof(sec), file);
	if (dw <= 0)	return FALSE;
	
	dw = GetPrivateProfileString("GENERAL", "key", "", key, sizeof(key), file);
	if (dw <= 0)	return FALSE;

	dw = GetPrivateProfileString(sec, key, "", buf, sizeof(buf), file);
	if (dw <= 0)	return FALSE;
	ZeroMemory(sec, sizeof(sec));
	CopyMemory(sec, buf, dw);
	int type = GetPrivateProfileInt(sec, "type", 2, file);
	type = ToValidType(type);

	//수정--이소진.09.05.26
	CPane* pane{};
	const COLORREF color = GetSkinColor();
	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS;
	int ii = 0;

	for (ii = 0; ii < 4; ii++)
	{
		const int	shape = GetType(type, ii);
		if (shape == -1)	
			continue;

		keys.Format("%c", shape);
		dw = GetPrivateProfileString(sec, keys, "", buf, sizeof(buf), file);
		if (dw <= 0)	
		{
			dw = LoadInfo_C(keys, buf);
		//	if (dw <= 0) continue;
		}

		if (m_sList.GetSize() > ii)
		{
			pane = m_sList.GetAt(ii).paneWnd;
			pane->Release();
			pane->Initialize(shape, ii);
		}
		else
		{
			//pane = new CPane(m_parent, this, m_home, shape, ii);

			auto& npane = _vList.emplace_back(std::make_unique<CPane>(m_parent, this, m_home, shape, ii));
			pane = npane.get();

			if (!pane->Create(NULL, "PANE", dwStyle, CRect(0, 0, 0, 0), this, (int) 1500 + ii))
			{
				delete pane;
				continue;
			}
			m_sList.Add(Maplist(pane));
		}

		pane->SetSkinColor(color, false);
		pane->SetScreen(buf);
	}

	CRect	sRc = CRect(0, 0, 0, 0);
	const UINT	nFlags = SWP_SHOWWINDOW;
	Maplist pList;

	CSize size;
	int ipos = 0;
	for (ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pList = m_sList.GetAt(ii);
		pane = pList.paneWnd;

		ipos = pane->GetPos();
		size = GetPaneSize(pane->GetType(), ipos);
		sRc.right = sRc.left + size.cx ;
		sRc.bottom = sRc.top + size.cy ;

		if(size.cx == 0 && size.cy == 0) continue;
		pane->SetWindowPos(NULL, sRc.left, sRc.top, sRc.Width(), sRc.Height(), nFlags);
		pane->SetRect(sRc);
		pane->LoadScreen();
		pane->Invalidate();

		pList.rc = sRc;
		m_sList.SetAt(ii, pList);

		sRc.left = sRc.right;
		if(ii == 1)
		{
			sRc.top = sRc.bottom;
			sRc.left = 0;
		}
	}

	if ( m_temp == NULL )
	{
		//m_temp = new CWnd;
		m_temp = std::make_unique<CWnd>();
		m_temp->Create(NULL, "TEMP", WS_CHILD, CRect(0,0,0,0), this, -1);
		m_TriggerKey = m_parent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, NULL), (long)m_temp.get());
	}
	
	CString mapN = "IB1000T1";
	const BOOL ret = m_parent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, m_TriggerKey), (long) mapN.operator LPCTSTR());

	CRect	cRc;
	GetClientRect(cRc);

	if (!m_config)
	{
		m_config = new CConfigBar(this, m_home, m_user);
		dwStyle = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;

		if (!m_config->Create(NULL, "config", dwStyle, CRect(0, 0, 0, 0), this, -1))
		{
			delete m_config;
			m_config = NULL;
			return FALSE;
		}
	
		m_config->LoadInfo(clear);
		m_config->SetSkinColor(RGB(228, 235, 254), RGB(228, 235, 254), GetColor(60), false);
	
		m_config->SetWindowPos(&wndTop, 0, cRc.bottom - (CONFIG_HEIGHT),
			cRc.right, CONFIG_HEIGHT, SWP_SHOWWINDOW);
	}
	else	
		m_config->ShowWindow(SW_SHOW);

	ResetMapSize(BAR_HEIGHT, cRc.right , cRc.bottom );
	return TRUE;
}

BOOL CBase::LoadInfo(CString section)
{
	char		buf[1024]{};
	CString		file, keys;

	CPane* pane{};
	int ii = 0;

	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pane = m_sList.GetAt(ii).paneWnd;
		delete pane;
	}
	m_sList.RemoveAll();

	const COLORREF color = GetSkinColor();
	DWORD	 dw{};
	const DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS;

	file.Format("%s\\tab\\종합config.ini", m_home);
	int type = GetPrivateProfileInt(section, "type", 1, file);
	type = ToValidType(type);

	for (ii = 0; ii < 4; ii++)
	{
		const int	shape = GetType(type, ii);
		if (shape == -1)	continue;

		//**keys.Format("%02d", shape - 'A');
		keys.Format("%c", shape);
		dw = GetPrivateProfileString(section, keys, "", buf, sizeof(buf), file);
		if (dw <= 0)	continue;

//		pane = new CPane(m_parent, this, m_home, shape, ii);
		auto& npane = _vList.emplace_back(std::make_unique<CPane>(m_parent, this, m_home, shape, ii));
		pane = npane.get();

		if (!pane->Create(NULL, "", dwStyle, CRect(0, 0, 0, 0), this, ii))
		{
			delete pane;
			continue;
		}
		pane->SetSkinColor(color, false);
		pane->SetScreen(buf);
		m_sList.Add(Maplist(pane));
	}

	CRect	sRc = CRect(0, 0, 0, 0);
	const UINT	nFlags = SWP_SHOWWINDOW;

	Maplist pList;
	CSize size;
	for (ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pList = m_sList.GetAt(ii);
		pane = pList.paneWnd;

		size = GetPaneSize(pane->GetType(), pane->GetPos());
		sRc.right = sRc.left + size.cx ;
		sRc.bottom = sRc.top + size.cy ;

		if(size.cx == 0 && size.cy == 0) continue;

		pane->SetWindowPos(NULL, sRc.left, sRc.top, sRc.Width(), sRc.Height(), nFlags);
		pane->LoadScreen();
		pane->SetRect(sRc);
		pList.rc = sRc;

		m_sList.SetAt(ii, pList);
	}
	CRect	cRc;
	GetClientRect(cRc);


	if (m_config)	m_config->ShowWindow(SW_HIDE);
	ResetMapSize(BAR_HEIGHT, cRc.right , cRc.bottom );
	if (!m_sList.GetSize())	return FALSE;
	return TRUE;
}

BOOL CBase::LoadType(CStringArray& ary, CString dat)
{
	int	pos{};
	CString	str;

	while (!dat.IsEmpty())
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			dat.TrimLeft();
			dat.TrimRight();
			ary.Add(dat);
			break;
		}

		str = dat.Left(pos);
		str.TrimLeft();
		str.TrimRight();
		ary.Add(str);
		dat = dat.Mid(pos+1);
	}

	return TRUE;
}

CSize CBase::GetPaneSize(int shape, int ipos)
{
	CSize size = CSize(0, 0);

	CRect crc, wrc;
	GetClientRect(&crc);

	int iconfigH= (BAR_HEIGHT * 2);
	if(m_config)
	{
		m_config->GetWindowRect(&wrc);
		ScreenToClient(wrc);

		iconfigH = wrc.Height() ;
	}

	const int iw = crc.Width();
	const int ih = crc.Height() - iconfigH; 
	const int itype = shape - 'A';

	size.cx = (int)(float(iw) * g_xPosRate[itype]);
	size.cy = (int)(float(ih) * g_yPosRate[itype]);
	return size;
}

CRect CBase::GetPaneRect(int shape, int pos)
{
	if (pos >= 4)	
		return CRect(0, 0, 0, 0);

	const int borderMargin = 2;
	const int widths[] = {
		700 + borderMargin, 512 + borderMargin, 512 + borderMargin, 324 + borderMargin
	};

//	CSize size = GetPaneSize(shape);
	const int tabHeight = 24;
	const int heights[] = {
		225 + tabHeight, 400 + tabHeight, 400 + tabHeight, 225 + tabHeight
	};

	const int gap = 0;

	const int index = shape - 'A';

	CRect rc(0, 0, 0, 0);
	rc.right = widths[index];
	rc.bottom = heights[index];

	int cx=-1, cy=-1;

	switch (shape)
	{
	case 'A': 
		cx = 3;
		cy = 2;
		break;
	case 'B': case 'C':
		cx = 1;
		cy = 0;
		break;
	case 'D':
		cx = 0;
		cy = 2;
		break;
	default:
		cx = 0;
		cy = 0;
		break;
	};

	switch (pos)
	{
		case 1: rc.OffsetRect(widths[cx], 0); break;
		case 2: rc.OffsetRect(0, heights[cy]); break;
		case 3: rc.OffsetRect(widths[cx], heights[cy]); break;
	}
	return rc;
}

COLORREF CBase::GetSkinColor()
{
	int		pos{};
	char		buffer[128]{};
	COLORREF	color{};
	CString		file, tmpS, dat, skin;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	DWORD dw = GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);

	if (dw <= 0)	
		skin = _T("");
	else		
		skin = buffer;
	dw = GetPrivateProfileString(GENERALSN, "Palettes", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	
		return GetSysColor(COLOR_ACTIVECAPTION);

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	CArray	< int, int >	arRgb;
	for (int ii = 0; ii < ary.GetSize() && ii < 5; ii++)
	{
		dat = ary.GetAt(ii);
		dat.TrimLeft();	dat.TrimRight();

		if (!skin.IsEmpty() && (skin != dat))		continue;
		GetPrivateProfileString(dat, "RGB", "", buffer, sizeof(buffer), file);
		dat = buffer;
		for (; !dat.IsEmpty();)
		{
			pos = dat.Find(',');
			if (pos == -1)
			{
				arRgb.Add(atoi(dat));
				break;
			}
			tmpS = dat.Left(pos++);
			dat = dat.Mid(pos);
			arRgb.Add(atoi(tmpS));
		}
		color = RGB(arRgb.GetAt(0), arRgb.GetAt(1), arRgb.GetAt(2));
		arRgb.RemoveAll();
		break;
	}
	ary.RemoveAll();

	return color;
}

void CBase::ChangeColor()
{
	LoadBitmap();
	m_bgColor = GetIndexColor(BACK_INDEX);
	m_contentsbgColor = GetIndexColor(CONTENTS_BACK_INDEX);
	m_contentslineColor = GetIndexColor(CONTENTS_BORDER_INDEX);

	CPane* pane{};
	const COLORREF color = GetSkinColor();
	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pane = m_sList.GetAt(ii).paneWnd;

//		pane->LoadRoundBitmap();
		pane->SetSkinColor(color, true);
		pane->ChangePallete();
	}

	if (m_config)	
		m_config->SetSkinColor(RGB(228, 235, 254), RGB(228, 235, 254), GetColor(60), true);
}

void CBase::ChangeFont(int pointFont, CString fontName)
{
	if(m_iFontSize == pointFont) return;
	const int iDefWidth = MAP_WIDTH;
	const int iDefHeight = MAP_HEIGHT;

	const double lratex = 0.16;
	const double lratey = 0.07;
	const double ldif = float(pointFont - 9);

	const int iWndWidth = iDefWidth + (int)(lratex * (float)iDefWidth * ldif);
	const int iWndHeight = iDefHeight +(int) (lratey * (float)iDefHeight * ldif);
	SetWindowPos(NULL, 0, 0, iWndWidth, iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_parent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iWndWidth, iWndHeight) );


	m_iFontSize = pointFont;
	m_strFontName = fontName;
	const int idx = pointFont - 8;
	m_lyRate = 	g_lratey[idx];

	if (m_config)	
		m_config->ChangeFont(m_iFontSize, m_strFontName);

	CPane* pane{};
	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pane = m_sList.GetAt(ii).paneWnd;
		pane->ChangeFont(pointFont, fontName);
	}
}

void CBase::PosChangeBar(int cx, int cy)
{
	if(m_iWidth == cx && m_iHeight == cy) return;
	m_iWidth = cx;
	m_iHeight = cy;

	CRect	viewRc;
	GetParent()->GetClientRect(viewRc);

	const int ibarH = (int)(m_lyRate * (float)CONFIG_HEIGHT);

	if (m_config->GetMode() == MODE_DEFAULT)
	{
		m_config->SetWindowPos(&wndTop, 0/*GAP*/, viewRc.bottom - (ibarH/*+GAP*/)
			, viewRc.right/* - GAP*2*/, ibarH, SWP_SHOWWINDOW);
	}
	else
	{
		const int	width = m_config->GetWidth();
		m_config->SetWindowPos(&wndTop, 0/*GAP*/, viewRc.bottom - (ibarH/*+GAP*/),
			width, ibarH, SWP_SHOWWINDOW);
	}
	m_config->ChangeFont(m_iFontSize, m_strFontName);

////ADD 이소진.09.05.26
	ResetMapSize(ibarH, cx, cy);
}

void CBase::ResetMapSize(int ibarH, int cx, int cy)
{

	CRect	sRc = CRect(0, 0, 0, 0);
	const UINT	nFlags = SWP_SHOWWINDOW;
	Maplist pList{};
	CPane*	pane = nullptr;
//	COLORREF color = GetSkinColor();
//	DWORD dwStyle = WS_CHILD|WS_VISIBLE|WS_BORDER|WS_CLIPSIBLINGS;

	CSize size{};
	int ipos{};
	cy -= ibarH;
	
	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		pList = m_sList.GetAt(ii);
		pane = pList.paneWnd;

		ipos = pane->GetPos();//pane->GetPos();
		size = GetPaneSize(pane->GetType(), ipos);
		sRc.right = sRc.left + size.cx ;
		sRc.bottom = sRc.top + size.cy ;

		if(ipos == 1 || ipos == 3) sRc.right = cx-GAP;
		if(ipos > 1) sRc.bottom = cy-GAP;

		pane->SetWindowPos(NULL, sRc.left, sRc.top, sRc.Width(), sRc.Height(), nFlags);
		pane->SetRect(sRc);
		pane->Invalidate(FALSE); 

		pList.rc = sRc;
		m_sList.SetAt(ii, pList);
		sRc.left = sRc.right;
		if(ii == 1)
		{
			sRc.top = sRc.bottom;
			sRc.left = 0;
		}
	}
}

int CBase::GetType(int type, int pos)
{
	int	shape{};

	switch (type)
	{
	case TYPE_2:
		switch (pos)
		{
		case 0: shape = 'B'; break;
		case 1: shape = 'C'; break;
		case 2: shape = 'D'; break;
		case 3: shape = 'A'; break;
		}
		break;
	case TYPE_3:
		switch (pos)
		{
		case 0:	shape = 'B'; break;
		case 1:	shape = 'C'; break;
		case 2:	shape = 'A'; break;
		case 3:	shape = 'D'; break;
		}
		break;
	case TYPE_4:
		switch (pos)
		{
		case 0:	shape = 'A'; break;
		case 1:	shape = 'D'; break;
		case 2:	shape = 'B'; break;
		case 3:	shape = 'C'; break;
		}
		break;
	case TYPE_5:
		switch (pos)
		{
		case 0:	shape = 'D'; break;
		case 1:	shape = 'A'; break;
		case 2:	shape = 'B'; break;
		case 3:	shape = 'C'; break;
		}
		break;
	default:	
		shape = -1;	
		break;
	}

	return shape;
}

void CBase::Configuration(CString domino)
{
	CString	section, caption, tmpS;
	int	pos = domino.Find("SECTION\t");
	if (pos == -1)	return;
	pos += 8;
	section = domino.Mid(pos, domino.Find('\n', pos) - pos);

	if (section.IsEmpty())
	{
		//LoadInfo();
		m_domino.Empty();
		PostMessage(WM_LOAD);
		pos = domino.Find("CAPTIONX\t");
		if (pos == -1)	return;
		else
		{
			pos += 9;
			caption = domino.Mid(pos, domino.Find('\n', pos) - pos);
		}
	}
	else
	{
//		LoadInfo(section);
		m_domino = section;
		PostMessage(WM_LOAD);
		pos = domino.Find("CAPTIONX\t");
		if (pos == -1)
			caption = section;
		else
		{
			pos += 9;
			caption = domino.Mid(pos, domino.Find('\n', pos) - pos);
		}
	}

	Variant(titleCC, caption);
}


COLORREF CBase::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;

	return m_parent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

COLORREF CBase::GetColor(int index)
{
	int		pos{};
	char		buffer[128]{}, kindex[8]{};
	COLORREF	color = RGB(212, 212, 212);
	CString		file, tmpS, dat, skin;
	CStringArray	ary;

	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);
	DWORD dw = GetPrivateProfileString(GENERALSN, "Palette", "", buffer, sizeof(buffer), file);
	if (dw <= 0)	return RGB(215, 215, 215);
	else		skin = buffer;
	sprintf(kindex, "RGB%03d", index);
	dw = GetPrivateProfileString(skin, kindex, "", buffer, sizeof(buffer), file);
	if (dw <= 0)	return RGB(215, 215, 215);

	dat = buffer;
	for (; !dat.IsEmpty();)
	{
		pos = dat.Find(',');
		if (pos == -1)
		{
			ary.Add(dat);
			break;
		}
		tmpS = dat.Left(pos++);
		dat = dat.Mid(pos);
		ary.Add(tmpS);
	}

	if (ary.GetSize() >= 3)
		color = RGB(atoi(ary.GetAt(0)), atoi(ary.GetAt(1)), atoi(ary.GetAt(2)));
	ary.RemoveAll();

	return color;
}

int CBase::ToValidType(int type)
{
	if (type < 2)
		return 2;
	else if (type > 5)
		return 5;
	else
		return type;
}

void CBase::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (m_config)	
		PosChangeBar( cx, cy);
	else
	{
		m_iWidth = cx;
		m_iHeight = cy;
	}

}

CBitmap* CBase::GetBitmap(const char* bmpName)
{
	CString fileName;
	fileName.Format("%s\\IMAGE\\%s.BMP", m_home, bmpName);

	CBitmap* bmp = (CBitmap*)
		m_parent->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, fileName.GetLength()), (LPARAM)(LPCTSTR)fileName);

	return bmp;
}

COLORREF CBase::GetSkinColor(int index)
{
	return (COLORREF)m_parent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), index);
}

CString CBase::GetSkinName()
{
	CString file;
	file.Format("%s\\%s\\%s", m_home, TABDIR, PALETTE);

	char buffer[MAX_PATH];
	GetPrivateProfileString(GENERALSN, "Palette", "Gray", buffer, sizeof(buffer), file);

	return CString(buffer);
}

const char* tabImage[] = {
	"l_%stab", "%stab", "r_%stab", "l_s%stab", "s%stab", "r_s%stab", NULL
};

void CBase::LoadBitmap()
{

	CString bmpName;
	CString skinName(GetSkinName());

	m_bmpReady = TRUE;

	for (int i = BM_ACTIVE; i <= BM_INACTIVE; i++)
	{
		for (int j = BM_LEFT; j <= BM_RIGHT; j++)
		{
			bmpName.Format(tabImage[i * (BM_RIGHT + 1) + j], skinName);
			m_bmp[i][j] = GetBitmap(bmpName);

			if (!m_bmp[i][j])
				m_bmpReady = FALSE;
		}
	}

	m_bmpExpand = GetBitmap("AXTAB_SCR");
	if (!m_bmpReady)
		m_bmpReady = FALSE;
}

void CBase::DrawButton(CDC* dc, CRect rc, BOOL active, const char* text)
{
	if (rc.Height() == 0)
		return;
	CDC mdc;
	mdc.CreateCompatibleDC(dc);

	CBitmap* oldBmp;
	oldBmp = mdc.SelectObject(m_bmp[active][BM_LEFT]);

	BITMAP bm{};

	rc.right++;
	//** body
	mdc.SelectObject(m_bmp[active][BM_BODY]);
	m_bmp[active][BM_BODY]->GetBitmap(&bm);
	dc->StretchBlt(rc.left, rc.top, rc.Width(), rc.Height()+1, &mdc, 0, 0, bm.bmWidth, bm.bmHeight, SRCCOPY);

	//** left
	mdc.SelectObject(m_bmp[active][BM_LEFT]);
	m_bmp[active][BM_LEFT]->GetBitmap(&bm);
	dc->BitBlt(rc.left, rc.top, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

	//** right
	mdc.SelectObject(m_bmp[active][BM_RIGHT]);
	m_bmp[active][BM_RIGHT]->GetBitmap(&bm);
	dc->BitBlt(rc.right - bm.bmWidth, rc.top, bm.bmWidth, bm.bmHeight, &mdc, 0, 0, SRCCOPY);

	const COLORREF textColor = GetSkinColor(active ? 71: 70);
	const COLORREF oldTextColor = dc->SetTextColor(textColor);
	dc->SetBkMode(TRANSPARENT);

	rc.top += 2;
	dc->DrawText(CString(text), rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);


	mdc.SetTextColor(oldTextColor);
	mdc.SelectObject(oldBmp);
}

int CBase::DrawExpand(CDC* dc, CRect rc, UINT stat)
{
	if (rc.Height() == 0)
		return 0;

	if (!m_bmpExpand)
		return 0;

	CDC mdc;
	mdc.CreateCompatibleDC(dc);

	CBitmap* oldBmp;
	oldBmp = mdc.SelectObject(m_bmpExpand);

	BITMAP bm;
	m_bmpExpand->GetBitmap(&bm);

	const int iresult = bm.bmWidth;
	const int buttonWidth = bm.bmWidth / 6;

	rc.top--;
	rc.left = rc.right - buttonWidth * 2;

	int sx1 = 0, sx2 = buttonWidth * 3;

	switch (stat)
	{
	case EXSTAT_LEFT_DOWN:
		sx1 = buttonWidth * 2;
		break;
	case EXSTAT_LEFT_HOVER:
		sx1 = buttonWidth * 1;
		break;
	case EXSTAT_RIGHT_DOWN:
		sx2 = buttonWidth * 5;
		break;
	case EXSTAT_RIGHT_HOVER:
		sx2 = buttonWidth * 4;
		break;
	}

	const int ih = rc.Height(); 
//	dc->BitBlt(rc.left, rc.top, buttonWidth, bm.bmHeight, &mdc, sx1, 0, SRCCOPY);
//	dc->BitBlt(rc.left + buttonWidth, rc.top, buttonWidth, bm.bmHeight, &mdc, sx2, 0, SRCCOPY);

	dc->StretchBlt(rc.left, rc.top, buttonWidth, ih, &mdc, 0, 0, 
				buttonWidth, bm.bmHeight, SRCCOPY);
	dc->StretchBlt(rc.left + buttonWidth, rc.top, buttonWidth, ih, &mdc, sx2, 0, 
				buttonWidth, bm.bmHeight, SRCCOPY);

	mdc.SelectObject(oldBmp);

	return buttonWidth*2;
}

int CBase::GetExpandWidth()
{
	if (!m_bmpExpand)
		return 0;

	BITMAP bm;
	m_bmpExpand->GetBitmap(&bm);

	return bm.bmWidth / 6;
}

void CBase::OnDestroy() 
{
	CWnd::OnDestroy();

//	CPane*	pane = NULL;
//	for (int ii = 0; ii < m_sList.GetSize(); ii++)
//	{
////수정--이소진.09.05.26
//		pane = m_sList.GetAt(ii).paneWnd;
//		pane->DestroyWindow();
//		delete pane;
//	}
	
	_vList.clear();

	if ( m_temp != NULL )
	{
		m_temp->DestroyWindow();
	}

	m_sList.RemoveAll();
	if (m_config)	delete m_config;
}

LRESULT CBase::OnMyPassword(WPARAM wParam, LPARAM lParam)
{
	//m_pPass->SetWindowText((char*)lParam);
	CString tmp = (char*) lParam;

	//MessageBox("OnMyPassword: "+tmp);
	//if(tmp=="QUEST_DATA") tmp = m_data;
	tmp = "AUTOPWD|"+tmp;
	CPane* pane{};
	HWND cont_hwnd{};
	CString tmps;


	for (int ii = 0; ii < m_sList.GetSize(); ii++)
	{
		cont_hwnd = 0;
		pane = m_sList.GetAt(ii).paneWnd;
		
		cont_hwnd = ::FindWindowEx(pane->m_screen->m_hWnd, 0, NULL, "CX_DATATRANS");

		if(((int)cont_hwnd)>32)
		{
			::SendMessage(cont_hwnd, WM_DATATRANS, 0, (long)tmp.operator LPCTSTR());
		}
		
	}
	
	return 1;
}

void CBase::OnTimer(UINT nIDEvent) 
{
	if( nIDEvent == TIMER_WAIT)
	{
		KillTimer(nIDEvent);
		CString spass;
		if( m_astrSaveList.Lookup("PSWD", spass) )
			SendToMap(m_iCurMap, "PSWD", spass);
	}

	if( nIDEvent == TIMER_WAIT1)
	{
		KillTimer(nIDEvent);
		CString sval;
		if( m_astrSaveList.Lookup(m_strCurKey, sval) )
			SendToMap(m_iCurMap, m_strCurKey, sval);
	}
	
	if(nIDEvent == TIMER_WAIT2)
	{
		KillTimer(nIDEvent);
		if(!m_strCurrValue.IsEmpty())
			SendToMap(m_iCurMap, JPRC, m_strCurrValue);
	}
/*
	if( nIDEvent == 9999)
	{
		KillTimer(nIDEvent);
		Variant(triggerCC, m_strTrigger);
	}
*/
	CWnd::OnTimer(nIDEvent);
}

void CBase::OnSysCommand(UINT nID, LPARAM lParam) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnSysCommand(nID, lParam);
}


