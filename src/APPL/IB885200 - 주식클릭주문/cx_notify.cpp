// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "cx_notify.h"
#include "../../h/AxisVar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define LEN_JGGB	2
#define	LEN_ACODE	12		// 주식 전체코드 길이
#define	LEN_SYGB	2		// 신용구분 길이
#define	LEN_DATE	8		// 날짜
#define	LEN_TOTAL	24		// key Length

#define DISPID_SETACCOUNT	0x00000001
#define DISPID_DATALIST		0x00000002
#define DISPID_FLAG			0x00000003
#define DISPID_SEND			0x00000006

#define USES_CONVERSION_CONST int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)

/*
/////////////////////////////////////////////////////////////////////////////
// IControlWnd properties

CString IControlWnd::GetSetAccount()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetSetAccount(LPCTSTR propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}

CString IControlWnd::GetDataList()
{
	CString result;
	GetProperty(0x2, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetDataList(LPCTSTR propVal)
{
	SetProperty(0x2, VT_BSTR, propVal);
}

CString IControlWnd::GetFlag()
{
	CString result;
	GetProperty(0x3, VT_BSTR, (void*)&result);
	return result;
}

void IControlWnd::SetFlag(LPCTSTR propVal)
{
	SetProperty(0x3, VT_BSTR, propVal);
}

/////////////////////////////////////////////////////////////////////////////
// IControlWnd operations

CString IControlWnd::GetProperties()
{
	CString result;
	InvokeHelper(0x4, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IControlWnd::SetProperties(LPCTSTR sProperties)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x5, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sProperties);
}

void IControlWnd::Send(LPCTSTR sAccn, LPCTSTR sPswd)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 sAccn, sPswd);
}
*/

/////////////////////////////////////////////////////////////////////////////
// CNotify

CNotify::CNotify(CWnd* sendW, int sendK)
{
	m_sendW = sendW;
	m_sendK = sendK;

	m_saveK = -1;

	m_hInst = NULL;
	m_Wcontrol = NULL;

//	m_Icontrol = NULL;
}

CNotify::~CNotify()
{
}


BEGIN_MESSAGE_MAP(CNotify, CWnd)
	//{{AFX_MSG_MAP(CNotify)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,    OnSend)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CNotify message handlers
BOOL CNotify::CreateNotify(CWnd* parent, CRect rect, CString root)
{
	m_parent = parent;
	m_root   = root;

	return CWnd::Create(NULL, "Notify_window", WS_CHILD|WS_VISIBLE, rect, parent, 5001);
}

int CNotify::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	AfxEnableControlContainer();

	struct	_param	param;
	CString	path; path.Format("%s/%s/%s", m_root, DEVDIR, "cx_Notify.DLL");

//edit + Flag(주식:0, 선물:1) + "|" + fieldNum + "|" +   fieldNum....

//"00.종목코드");
//"01.종목명");
//"02.결제잔고");
//"03.주문잔량");
//"04.매입평균가");
//"05.매도가능잔고");
//"06.현재가");
//"07.평가금액");
//"08.평가손익");
//"09.평가수익률");
//"10.매입금액");

	param.name	= "cx_Notify";
	param.fonts	= "굴림체";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 232;
	param.pRGB	= 192;
	param.options = "/edit0|00|02|04|08";	
	GetClientRect(&param.rect);

	m_hInst = LoadLibrary(path);
	if (!m_hInst)
	{
		TRACE("cx_Notify control load error....\n");
		return -1;
	}

	axCreate = (CWnd* (APIENTRY *)(CWnd*, void*)) GetProcAddress(m_hInst, "axCreate");
	if (axCreate == NULL)
	{
		TRACE("cx_Notify axCreate proc error...\n");
		return -1;
	}

	m_Wcontrol = axCreate(this, (void *) &param);

//	m_Icontrol = new IControlWnd(m_Wcontrol->GetIDispatch(TRUE));
	
	return 0;
}

void CNotify::OnDestroy() 
{
	CWnd::OnDestroy();
	
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow();  delete m_Wcontrol;
//		m_Icontrol->DetachDispatch(); delete m_Icontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
}

void CNotify::OnClose() 
{
	if (m_Wcontrol != NULL)
	{
		m_Wcontrol->DestroyWindow(); 
		delete m_Wcontrol;
//		delete m_Icontrol;
	}
	if (m_hInst != NULL) FreeLibrary(m_hInst);
	
	DestroyWindow(); CWnd::OnClose();
}

LONG CNotify::OnSend(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case invokeTRx:
		break;
	case variantDLL:
	case getPALETTE:
	case getBITMAP:
	case getFONT:
	case getPEN:
		return m_sendW->SendMessage(WM_USER, wParam, lParam);
	case formDLL:
		{
			CString strData = (char*) lParam;
			if (strData.Find("uide\t"))
			{
				strData.Delete(0,6);
				if (strData.GetAt(0) != '0')
				{
					MessageBox(strData, "IBK투자증권", MB_OK);
				}
			}
		}
		break;
	case eventDLL:
		{
//			if (m_Icontrol)
			IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);

			if (pDisp)
			{
				USES_CONVERSION_CONST;

				VARIANT var;
				VariantInit(&var);
				CComDispatchDriver::GetProperty(pDisp, DISPID_FLAG, &var);	
				CString flagS = OLE2T(var.bstrVal);

				VariantClear(&var);
				CComDispatchDriver::GetProperty(pDisp, DISPID_DATALIST, &var);
				CString strData = OLE2T(var.bstrVal);

//				CString flagS = m_Icontrol->GetFlag();				
//					count \t code \t mmgb \t jgvl \n ......
//				CString	strData = m_Icontrol->GetDataList();

//TRACE("notify = %s\n", strData.Left(500));
		
				
				CString strJCod;
				CString strJango;
				CString strPgdg;
				CString strPgsi;

				if (flagS == "A")
				{
					CString strHead = Parser(strData, "\t");
					const int iCount = atoi(strHead);

					for (int ii = 0; ii < iCount; ii++)
					{
						CString strLine = Parser(strData, "\n");
						strJCod = Parser(strLine, "\t");
						strJango = Parser(strLine, "\t");
						strPgdg = Parser(strLine, "\t");
						strPgsi = Parser(strLine, "\t");
						m_pControlWnd->SetJango(strJCod, strJango, strPgdg, strPgsi);
					}
				}
				else if (flagS != "E")
				{
					//keyS.Format("%-*s%-*s%-*s", LEN_ACODE, codeS, LEN_DATE, dateS, LEN_SYGB, sygbS);
					CString keyS = strData.Left(LEN_TOTAL);
					strData = strData.Mid(LEN_TOTAL);
					
					if (flagS == "D")
					{
						strJCod = keyS.Mid(LEN_JGGB, LEN_ACODE);
						strJCod.TrimRight();
						m_pControlWnd->JangoClear(strJCod);
					}
					else if (flagS == "U" || flagS == "I")
					{
						strJCod = Parser(strData, "\t");
						strJango = Parser(strData, "\t");
						strPgdg = Parser(strData, "\t");
						strPgsi = Parser(strData, "\t");
						m_pControlWnd->SetJango(strJCod, strJango, strPgdg, strPgsi);
					}	
				}
				/*
				else if (flagS == "E")
				{
					MessageBox(strData, "IBK투자증권-실시간잔고", MB_OK);
				}
				*/

				if (m_pControlWnd->GetWaitMode())
					m_pControlWnd->SetWaitMode(FALSE);
			}
		}
		break;

	default:
		TRACE("OnSend = [0x%x][%d]\n", LOWORD(wParam), HIWORD(wParam));
		break;
	}

	return 0;
}


void CNotify::SendJango(CString acc, CString pswd)
{

//	if (m_Icontrol == NULL)
	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);

	if (pDisp == NULL)
		return;
	
//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = acc.AllocSysString();

	CComDispatchDriver::PutProperty(pDisp, DISPID_SETACCOUNT, &var);

	if (pswd.IsEmpty())
		return;

	DISPPARAMS dispparams;

	[[gsl::suppress(26409)]]
	dispparams.rgvarg = new VARIANTARG[2];

	dispparams.rgvarg[0].vt = VT_BSTR;
	dispparams.rgvarg[0].bstrVal = pswd.AllocSysString();
	dispparams.rgvarg[1].vt = VT_BSTR;
	dispparams.rgvarg[1].bstrVal = acc.AllocSysString();
	dispparams.cArgs = 2;
	dispparams.cNamedArgs = 0;

	pDisp->Invoke(DISPID_SEND, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, NULL, NULL, NULL);
	
	[[gsl::suppress(26401)]]
	delete[] dispparams.rgvarg;
//	m_Wcontrol->InvokeHelper(DISPID_SEND, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE*)acc.AllocSysString(), (BYTE*)pswd.AllocSysString());
//	m_Icontrol->Send(acc, pswd);
}

CString CNotify::Parser(CString &srcstr, CString substr)
{
	CString temp;
	temp.Empty();
	if (srcstr.Find(substr) == -1)
	{
		temp = srcstr;
		srcstr.Empty();
	}
	else
	{
		temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	}

	temp.TrimLeft();
	temp.TrimRight();
	return temp;
}
