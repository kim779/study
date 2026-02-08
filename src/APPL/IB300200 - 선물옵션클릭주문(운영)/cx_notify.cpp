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

#define	LEN_ACODE	12		// 주식 전체코드 길이
#define	LEN_SYGB	2		// 신용구분 길이
#define	LEN_DATE	8		// 날짜
#define	LEN_TOTAL	22		// key Length

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
	param.fonts	= "굴림";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 232;
	param.pRGB	= 192;
//	param.options = "/edit1|00|01|02|03|04|05|11|"; //"/edit1|00|02|04|08";	
	param.options = "/edit1|00|02|03|04|05|09|11|"; //"/edit1|00|02|04|08";	
	
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
		break;
	case eventDLL:
		CheckData();
		break;

	default:
		TRACE("OnSend = [0x%x][%d]\n", LOWORD(wParam), HIWORD(wParam));
		break;
	}

	return 0;
}

void CNotify::SendJango(CString acc, CString pswd, CString codeS)
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	CString dummyArg = "0";
	pswd.TrimLeft(); pswd.TrimRight();
	if ( pswd.GetLength()==0 ) return;
	m_codeS = codeS;
//	if (m_Icontrol == NULL)
	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);
	if (pDisp == NULL)
		return;
	
	CComDispatchDriver driver(pDisp);

//	m_Icontrol->SetSetAccount(acc);
	VARIANT var;
	VariantInit(&var);

	var.vt = VT_BSTR;
	var.bstrVal = acc.AllocSysString();
	
	driver.PutPropertyByName(L"SetAccount", &var);

	if (pswd.IsEmpty())
		return;

	_variant_t varArgs[4] = { L"0" , L"0", pswd, acc };
	
	const HRESULT hr = driver.InvokeN(L"Send", &varArgs[0], 4);
	if (FAILED(hr))
		TRACE("CNotifyCtrl.Send is failed. HR[%X]", hr);
//	m_Wcontrol->InvokeHelper(DISPID_SEND, DISPATCH_METHOD, VT_EMPTY, NULL, (BYTE*)acc.AllocSysString(), (BYTE*)pswd.AllocSysString());
//	m_Icontrol->Send(acc, pswd);
}

void CNotify::CheckData()
{
	if (!m_Wcontrol) return;
	if (!m_Wcontrol->GetSafeHwnd()) return;

	IDispatch* pDisp = m_Wcontrol->GetIDispatch(FALSE);

	if (pDisp)
	{
		USES_CONVERSION_CONST;

		CComDispatchDriver driver(pDisp);
		
		VARIANT var;
		VariantInit(&var);
		driver.GetPropertyByName(L"Flag", &var);
		if (var.vt!=VT_BSTR) return;
		CString flagS = OLE2T(var.bstrVal);
		
		VariantClear(&var);
		driver.GetPropertyByName(L"DataList", &var);
		if (var.vt!=VT_BSTR) return;
		CString strData = OLE2T(var.bstrVal);

//				CString flagS = m_Icontrol->GetFlag();				
//					count \t code \t mmgb \t jgvl \n ......
//				CString	strData = m_Icontrol->GetDataList();		
		
		CString	code, mmgb, jango, pgga, jmgr, pgsk, maip, value;
		int nrec{};
		bool bAll = false;
		bool bIsCode = false;
		if (flagS == "A")
		{
			nrec = atoi(Parser(strData, "\t"));
			bAll = true;
		}
		else if (flagS != "E")
			nrec = 1;
		else
			return;

		/*
		CString tmp;
		tmp.Format("nrec : %d", nrec);
		MessageBox((LPCTSTR)tmp);
		*/

		for (int ii = 0; ii < nrec; ii++)
		{
			value = Parser(strData, "\n");

			// code
			code = Parser(value, "\t");		
			if (code.GetLength() > 8)	code = code.Left(8);

			if (code == m_codeS)
				bIsCode = true;
			
			// 매매구분
			mmgb = Parser(value, "\t");
			mmgb = mmgb.Left(1);
			// 잔고수량
			jango = Parser(value, "\t");
			jango.TrimLeft();	jango.TrimRight();
			jango = mmgb + jango;
			// 매도가능
			jmgr = Parser(value, "\t");	jmgr.TrimLeft();	jmgr.TrimRight();
			// 평균가
			pgga = Parser(value, "\t");	pgga.TrimLeft();	pgga.TrimRight();
			// 평가손익
			pgsk = Parser(value, "\t");	pgsk.TrimLeft();	pgsk.TrimRight();
			// 매입금액
			maip = Parser(value, "\t");	maip.TrimLeft();	maip.TrimRight();
			m_pControlWnd->SetJango(code, jango, maip, pgga, jmgr, pgsk, bAll);
		}

		if (m_pControlWnd->GetWaitMode())
			m_pControlWnd->SetWaitMode(FALSE);				
	}
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
