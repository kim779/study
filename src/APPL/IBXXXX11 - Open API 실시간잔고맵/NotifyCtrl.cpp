// NotifyCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "NotifyCtrl.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define USES_CONVERSION_NEW int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)
#define USES_CONVERSION_NEW_CONST const int _convert = 0; (_convert); const UINT _acp = ATL::_AtlGetConversionACP() /*CP_THREAD_ACP*/; (_acp); LPCWSTR _lpw = NULL; (_lpw); LPCSTR _lpa = NULL; (_lpa)


/////////////////////////////////////////////////////////////////////////////
// CNotifyCtrl

CNotifyCtrl::CNotifyCtrl(CWnd* pParent, LPCSTR home, int key)
: CAxisExt(pParent)
{
	m_key = key;
	m_home = home;
	m_hInst = NULL;
	m_pCtrl = NULL;
}

CNotifyCtrl::~CNotifyCtrl()
{
	if (m_pCtrl)
	{
		m_pCtrl->DestroyWindow();
		delete m_pCtrl;
	}
	if (m_hInst) 
	{
		FreeLibrary(m_hInst);
	}
}

void CNotifyCtrl::CreateControl()
{
	if (m_hInst) return;
	
	//2012.12.17 KSJ CX_NOTIFY --> CX_NOTIFYAPI 로 수정
	CString dllPath = m_home + CString("\\dev\\CX_NOTIFYAPI.DLL");
	
	m_hInst = LoadLibrary((LPCSTR)dllPath);
	if (!m_hInst)
	{
		ErrorBox("잔고컨트롤 생성 실패1");
		return;
	}
	
	CWnd* (APIENTRY *axCreate)(CWnd *, void *);
	axCreate = (CWnd*(APIENTRY*)(CWnd*,void*))GetProcAddress(m_hInst, "axCreate");
	if (!axCreate)
	{
		ErrorBox("잔고컨트롤 생성 실패2");
		return;
	}
	
	struct _param param;
	GetClientRect(&param.rect);
	
	//2012.12.17 KSJ CX_NOTIFY --> CX_NOTIFYAPI 로 수정
	param.name  = "cx_NotifyAPI";
	param.fonts = "굴림체";
	param.point = 9;
	param.style = 0;
	param.tRGB  = 232;
	param.pRGB  = 192;
	//edit0 주식, edit1 선물
	if(m_key == F_NOTIFY)
	{
		//"00.종목코드" , "01.종목명",	"02.구분",	"03.잔고수량",	"04.가능수량",	"05.평균가/정산가",	"06.현재가"
		//"07.평균가대비",	"08.평가금액",	"09.평가손익",	"10.수익률",	"11.매입금액",	"12.전일대비"
		param.options.Format("/edit%d|00|01|02|03|04|05|06|07|08|09|10|11|12|", m_key);
	}
	else if(m_key == S_NOTIFY)
	{
		//"00.종목코드", "01.종목명", "02.결제잔고", "03.주문잔량",	"04.매입평균가", "05.매도가능잔고",	"06.현재가", "07.평가금액",
		//"08.평가손익", "09.평가수익률", "10.매입금액", "11.대출일", "12.신용구분", "13.신용구분내용", "14.신용구분약자", "15.신용대출금액",
		//"16.잔고구분", "17.BEP단가", "18.비용", "19.등락률"
		param.options.Format("/edit%d|00|01|02|03|04|05|06|07|08|09|10|11|12|13|14|15|16|17|18|19|", m_key);
	}
	
	param.key   = m_key;
	
	m_pCtrl = axCreate(this, (LPVOID)&param);
}

void CNotifyCtrl::ErrorBox( LPCSTR msg )
{
	MessageBox(msg, "IBK투자증권", MB_OK|MB_ICONERROR);
}

LRESULT CNotifyCtrl::OnUser( WPARAM wParam, LPARAM lParam )
{
	switch(LOWORD(wParam))
	{
	case variantDLL:
		if (HIWORD(wParam) == guideCC)
			return GetParent()->SendMessage(WM_USER, MAKEWPARAM(MAKEWORD(DLL_GUIDE,0), strlen((LPCSTR)lParam)), lParam );
		break;
	case eventDLL:
		CheckData();
		break;
	}
	return m_pParent->SendMessage(WM_USER, wParam, lParam);
}

CString CNotifyCtrl::GetCtrlProperty( _bstr_t prop_name )
{
	USES_CONVERSION_NEW_CONST;
	if (!m_pCtrl) return "";
	if (!m_pCtrl->GetSafeHwnd()) return "";
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return "";
	
	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(prop_name, &var);
	
	return (LPCSTR)(_bstr_t)var;
}

void CNotifyCtrl::ChangeFont( LPCSTR font, int point )
{
	m_pCtrl->SendMessage(WM_USER, MAKEWPARAM(DLL_SETFONT, point), (LPARAM)font);
	
	if (m_pCtrl)
	{
		CRect rc;
		GetClientRect(rc);
		m_pCtrl->MoveWindow(rc, TRUE);
	}
}

BEGIN_MESSAGE_MAP(CNotifyCtrl, CWnd)
	//{{AFX_MSG_MAP(CNotifyCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNotifyCtrl message handlers

int CNotifyCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CreateControl();

	return 0;
}

void CNotifyCtrl::Send( _bstr_t acno, _bstr_t pswd, _bstr_t code )
{
	if (!m_pCtrl) return;
	if (!m_pCtrl->GetSafeHwnd()) return;
	if (strlen(pswd)==0) 
	{
		TRACE("\nCNotifyCtrl.Send -> password is null!");
		return;
	}
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return;

	CComDispatchDriver driver(pDisp);
	_variant_t varArgs[4] = { _bstr_t(), _bstr_t(), pswd, acno };

	driver.PutPropertyByName(L"SetAccount", &_variant_t(acno));
	const HRESULT hr = driver.InvokeN(L"Send", &varArgs[0], 4);
	if (FAILED(hr))
		TRACE("CNotifyCtrl.Send is failed. HR[%X]", hr);

	m_acno = (LPCSTR)acno;
}

void CNotifyCtrl::SetCtrlProperty( _bstr_t prop_name, _variant_t &var )
{
	if (!m_pCtrl) return;
	if (!m_pCtrl->GetSafeHwnd()) return;
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return;
	
	CComDispatchDriver driver(pDisp);
	driver.PutPropertyByName(prop_name, &var);
}

void CNotifyCtrl::CheckData()
{
	USES_CONVERSION_NEW;

	if (!m_pCtrl) return;
	if (!m_pCtrl->GetSafeHwnd()) return;
	
	IDispatch *pDisp = m_pCtrl->GetIDispatch(FALSE);
	if (!pDisp) return;

	CComDispatchDriver driver(pDisp);

	_variant_t var;
	int dlen{};
	LPCSTR flag{}, data{};

	driver.GetPropertyByName(L"Flag", &var);
	if (var.vt!=VT_BSTR) return;
	flag = OLE2A(var.bstrVal); var.Clear();

	driver.GetPropertyByName(L"DataList", &var);
	if (var.vt!=VT_BSTR) return;
	data = OLE2A(var.bstrVal); var.Clear();
	if (data != NULL && strlen(data) > 0)
		dlen = strlen(data);
	else
		dlen = 0;

	if(*flag=='A' || *flag=='U' || *flag=='I' || *flag=='D')
	{
		CString strData, strTemp;
		strData.Format("%s\t%s", flag, data);

// 		strTemp.Format("[%s]\t[%s]", flag, data);
// 		strTemp.Replace("\n", "[][]");
// 		strTemp.Replace("\t", "??");
// 		OutputDebugString("[KSJ][IBXXX11] (" + strTemp + ")");
		
		if(m_key == F_NOTIFY)
			GetParent()->SendMessage(WM_APP_SIG, WP_F_JNGO_CHANGE, (LPARAM)strData.GetBuffer(strData.GetLength()));
		else if(m_key == S_NOTIFY)
			GetParent()->SendMessage(WM_APP_SIG, WP_S_JNGO_CHANGE, (LPARAM)strData.GetBuffer(strData.GetLength()));

		strData.ReleaseBuffer();
	}

	/*
	if (*flag=='A')
	{
		int row_cnt;

		vector<StringProxy> rows, cols;
		vector<StringProxy>::iterator st, ed;
		
		LPCSTR pos = find(data, data+dlen, '\t');
		row_cnt = Str2Int(data, pos-data);							// 처음 4자리는 Row 갯수
		ParseString(pos+1, data+dlen, '\n', rows);					// 4자리 이후로 \n \t 구분자로 2중 Array
		
		if (row_cnt!=rows.size())
		{
			TRACE("Unknown NotifyControl? (%d != %d)", row_cnt, rows.size());
			return;
		}
		for( st=rows.begin(), ed=rows.end(); st!=ed; ++st)
		{
			ParseString(*st, '\t', cols);
			if (cols.size()!=NOTIFY_COLS) continue;

			Jango j;;
			j.flag = 'I';
			sprintf(j.acno, (LPCSTR)m_acno);
			n= 0; sprintf(j.code, "%s", cols[n].ToCStr() );
			n= 1; sprintf(j.name, "%s", cols[n].ToCStr() );
			n= 2; sprintf(j.gubn, "%s", cols[n].ToCStr() );
			n= 3; j.jqty = Str2Int( cols[n] );
			n= 4; j.xqty = Str2Int( cols[n] );
			n= 5; j.pprc = Str2Double( cols[n] )*100 + DOUBLE_PREFIX;	// 가격은 정수로 관리(*100)
			n= 8; j.camt = Str2Double( cols[n] );
			n= 9; j.tamt = Str2Double( cols[n] );
			n=10; j.srat = Str2Double( cols[n] );
			n=11; j.mamt = Str2Double( cols[n] );

			GetParent()->SendMessage(WM_APP_SIG, WP_JNGO_CHANGE, (LPARAM)&j);
			/*
			TRACE("\n[%c] 종목[%s] - 구분[%s], 잔고[%d] 청산[%d], 평단[%d], 평가금액[%.f] 평가손익[%.f] 수익률[%.2f], 매입금액[%.f]",
				*flag, j.code, j.gubn, j.jqty, j.xqty, j.pprc, j.camt, j.tamt, j.srat, j.mamt);
			
		}
	}
	else if (*flag=='U' || *flag=='I')
	{
		vector<StringProxy> cols;
		ParseString(StringProxy(data+8, data+dlen) , '\t', cols);		// 종목코드가 있다. 8byte!
		
		if (cols.size()!=NOTIFY_COLS) return;
		
		Jango j;
		j.flag = *flag;
		sprintf(j.acno, (LPCSTR)m_acno);
		n= 0; sprintf(j.code, "%s", cols[n].ToCStr() );
		n= 1; sprintf(j.name, "%s", cols[n].ToCStr() );
		n= 2; sprintf(j.gubn, "%s", cols[n].ToCStr() );
		n= 3; j.jqty = Str2Int( cols[n] );
		n= 4; j.xqty = Str2Int( cols[n] );
		n= 5; j.pprc = Str2Double( cols[n] )*100.0;	// 가격은 정수로 관리(*100)
		n= 8; j.camt = Str2Double( cols[n] );
		n= 9; j.tamt = Str2Double( cols[n] );
		n=10; j.srat = Str2Double( cols[n] );
		n=11; j.mamt = Str2Double( cols[n] );

		GetParent()->SendMessage(WM_APP_SIG, WP_JNGO_CHANGE, (LPARAM)&j);
		/*
		TRACE("\n[%c] 종목[%s] - 구분[%s], 잔고[%d] 청산[%d], 평단[%d], 평가금액[%.f] 평가손익[%.f] 수익률[%.2f], 매입금액[%.f]",
				*flag, j.code, j.gubn, j.jqty, j.xqty, j.pprc, j.camt, j.tamt, j.srat, j.mamt);
		
	}
	else if (*flag=='D')
	{
		Jango j;
		j.flag = 'D';
		sprintf(j.acno, (LPCSTR)m_acno);
		sprintf(j.code, "%.8s", data);

		GetParent()->SendMessage(WM_APP_SIG, WP_JNGO_CHANGE, (LPARAM)&j);
	}
*/
}
