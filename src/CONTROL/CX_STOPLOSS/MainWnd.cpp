// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_STOPLOSS.h"
#include "MainWnd.h"
#include "../../h/ledger.h"
#include "../../h/Grid.h"
#include "calc_com.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
// 상한가       1	302
// 매수우선호가 2	71
// 현재가       3	23
// 매도우선호가 4	51
// 하한가       5	303
// 시가         6	29
// 고가         7	30
// 저가         8	31
// 직접입력     9

CMainWnd::CMainWnd(CWnd* parent,_param* param)
{
	EnableAutomation();

	SetParam(param);

	m_pParent = parent;
	m_bStart = FALSE;
	m_arLog.RemoveAll();

	m_bNewUnit = FALSE;	// 2023 호가단위개선
}

CMainWnd::~CMainWnd()
{
	Reset();
}

void CMainWnd::SetParam(_param *pParam)
{
	m_Param.key  = pParam->key;
	m_Param.name = pParam->name;
	m_Param.rect = pParam->rect;
	m_Param.fonts   = pParam->fonts;
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.options = pParam->options;
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_FUNCTION(CMainWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Start", Start, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CMainWnd, "SetCondition", SetCondition, VT_BOOL, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetPriceSetupValue", GetPriceSetupValue, VT_I4, VTS_BSTR VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CMainWnd, "DelCondition", DelCondition, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "Run", Run, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "GetTickPrice", GetTickPrice, VT_I4, VTS_I4 VTS_I2 VTS_BSTR VTS_I2)
	DISP_FUNCTION(CMainWnd, "SetMapHandle", SetMapHandle, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "GetMapHandle", GetMapHandle, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "Reset", Reset, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CMainWnd, "WriteLog", WriteLog, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SendTestData", SendTestData, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {E9C65AD1-95DC-474A-92F0-A6284BF9091F}
static const IID IID_IMainWnd =
{ 0xe9c65ad1, 0x95dc, 0x474a, { 0x92, 0xf0, 0xa6, 0x28, 0x4b, 0xf9, 0x9, 0x1f } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString	path;

	path.Format("%s/%s/axis.ini", Variant(homeCC, ""), TABDIR);
	m_bNewUnit = GetPrivateProfileInt("K2023SyStem", "Newhgunit", 0, path);

	return 0;
}

void CMainWnd::SetProperties(LPCTSTR sParam) 
{
	// TODO: Add your dispatch handler code here
	m_Param.options = sParam;
}

BSTR CMainWnd::GetProperties() 
{
	CString strResult = m_Param.options;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

long CMainWnd::OnMessage(WPARAM wParam,LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		break; 
	case DLL_OUBx:
		break; 
 	case DLL_ALERTx:
		if (m_bStart)
		{
			struct _alertR* alertR;
			alertR = (struct _alertR*)lParam;
			ParseAlert(alertR);
		}
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETPAL:
		break;	
	case DLL_SETFONT:
		break;
	}
	return 0;
}

void CMainWnd::SendTR(CString name, CString data, BYTE type, int key)
{
	struct _userTH udat {};
	const	int	datl = data.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datl + L_userTH + 123);
	int	idx = m_Param.name.GetLength();

	// control name
	CopyMemory(pWb.get(), m_Param.name, idx);
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, name, name.GetLength());
	udat.key = key;
	udat.stat = type;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], data, datl);
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)pWb.get());
}

CString CMainWnd::Variant(int comm, CString data)
{
	CString retvalue;
	
//  	if (comm == guideCC)
//  	{
//   		MessageBox("["+data+"]", "IBK투자증권");
// 		return retvalue;
//  	}
	
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	
	if ((long)dta > 1)
		retvalue = dta;
	
	return retvalue;
}

void CMainWnd::ParseAlert(struct _alertR* alert)
{
	CString	sVal;
	DWORD*	data = nullptr;
	
	for (int ii = alert->size-1; ii >= 0; ii--)
	{	
		data = (DWORD*)alert->ptr[ii];

		if (data[23])
		{
			sVal = (char*)data[23];
			sVal.Remove('-');
			sVal.Remove('+');
			
			CheckRealData(alert->code, sVal);
		}
	}
}

BOOL CMainWnd::CheckRealData(CString code, CString sCurr)
{
	CString	key, sCode;
	CString strGubn, strDate, strCode;
	CString string;
	int	compprice = 0;
	const	int	nCurr = atoi(sCurr);
	std::shared_ptr<CCodeMap> pCode = nullptr;

	if (code.GetLength() != 7)
		return TRUE;

	sCode = code.Mid(1);
	for (POSITION pos = m_mapCode.GetStartPosition(); pos;)	
	{
		m_mapCode.GetNextAssoc(pos, key, pCode);
		if (!pCode || key.Find(sCode, 10) < 0)
			continue;

		strGubn = key.Mid(0,2);
		strDate = key.Mid(2,8);
		strCode = key.Mid(10);

		strGubn = GetJumunCrtCode(strGubn);
		if (pCode->loss != 0 && pCode->loss >= nCurr)
		{
			string.Format("StopOrder\t%s,%s,%s,%s,%s,%d,%d,%d", m_sMapHandle, strGubn, strDate, strCode, sCurr, 0, 0, 0);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());

			pCode->loss = 0;
			RemoveCondition(key);
		}

		if (pCode->earn != 0 && pCode->earn <= nCurr)
		{
			CString string;
			string.Format("StopOrder\t%s,%s,%s,%s,%s,%d,%d,%d", m_sMapHandle, strGubn, strDate, strCode, sCurr, 1, 0, 0);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());

			pCode->earn = 0;
			RemoveCondition(key);
		}

		if (pCode->remain != 0)
		{
			if (pCode->maxprice >= pCode->remain)	//최고가가 이익보전값보다 이상인 경우
			{
				compprice = pCode->maxprice - pCode->dndiff;

				if (nCurr <= compprice)		//현재가가 최고가대비 설정 가격보다 이하인 경우
				{
					string.Format("StopOrder\t%s,%s,%s,%s,%s,%d,%d,%d", m_sMapHandle, strGubn, strDate, strCode, sCurr, 2, pCode->maxprice, pCode->dndiff);
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)string.GetString());

					pCode->remain = 0;
					RemoveCondition(key);
				}
			}

			if (nCurr > pCode->maxprice)
				pCode->maxprice = nCurr;
		}
	}
	return TRUE;
}

void CMainWnd::Start(BOOL bStop) 
{
	// TODO: Add your dispatch handler code here
	m_bStart = bStop;
}

CString CMainWnd::GetJumunCrtCode(CString sCrtCode)
{
	CString sCodeList[] = {"101", " 02", "103", " 22", "107", "180"};

	for (int ii = 0; ii < 6; ii++)
	{
		if (sCrtCode == sCodeList[ii].Right(2)) 
			return  sCodeList[ii];
	}

	return "000";
}

BOOL CMainWnd::SetCondition(LPCTSTR index,LPCTSTR date,LPCTSTR code, LPCTSTR curr) 
{
	CString sCode = code;
	CString sCurr = curr;
	CString sDate = date;

	sDate.Remove('/');
	if (sCode != "" && sCurr != "")
	{
		CString keys;
		CStringArray arOpt;
		std::shared_ptr<CCodeMap> pCode = std::make_shared<CCodeMap>();

		SplitString(sCurr, ":" ,arOpt);

		pCode->loss	= atoi(arOpt.GetAt(0)); 
		pCode->earn	= atoi(arOpt.GetAt(1));
		pCode->remain	= atoi(arOpt.GetAt(2));
		pCode->dndiff	= atoi(arOpt.GetAt(3));
		pCode->maxprice	= 0;
		pCode->danga	= 0;
		pCode->tick	= 0;

		keys.Format("%s%s%s",index,sDate,sCode);
		m_mapCode.SetAt(keys, pCode);
		return TRUE;
	}

	return FALSE;
}

long CMainWnd::GetPriceSetupValue(LPCTSTR code, long icostval, long iunitType, long istopType, long iconfigval) 
{
	// icostval	매입단가
	// iunitType	{"틱", "%", "원", "입력"};
	// istopType	0-손실목표, 1-이익, 2-보존, 3-고점대비
	// iconfigval	틱, %, 원, 입력 등으로 설정한 실제값

	const	int	iCodeType = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)code);
	const	int	iAddType = istopType;
	
	if (istopType == 0)
		istopType = -1;
	else	istopType = 1;

	icostval = getPrice((int)icostval, iCodeType, istopType);
	int	iprice = icostval;
	
	if (iunitType == 3)		// 원
	{
		if (iAddType < 3) 
		{
			iconfigval *= istopType;
			iprice = icostval + iconfigval;
		}
		else	iprice = iconfigval;
	}
	else if (iunitType == 2)	// %
	{
		iprice = (icostval * iconfigval) / 100;
		iprice *= istopType;

		if (iAddType < 3) 
			iprice = icostval + iprice;
	}
	else if (iunitType == 1)	//TICK
	{
		if (iAddType < 3)	//iAddType 0-손실목표, 1-이익, 2-보존, 3-고점대비
			iprice = getTickPrice(icostval, iconfigval, iCodeType, istopType);
		else 
			iprice = getTickValue(icostval, iconfigval, iCodeType);
	}
	return iprice;
}

void CMainWnd::SplitString(CString &strData, CString strToken,CStringArray &ar)
{
	if (strToken == "")
		return;

	CString sResult = strData;
	int	iFind = strData.Find(strToken);
	
	if (iFind < 0)
	{
		ar.Add(sResult);
		return;
	}
	
	while (iFind > -1)
	{
		sResult = strData.Left(iFind);
		strData = strData.Right(strData.GetLength() - iFind - strToken.GetLength());  
			
		ar.Add(sResult);
			
		iFind = strData.Find(strToken);
	}
	
	sResult = strData;
	
	ar.Add(sResult);
}

void CMainWnd::DelCondition(LPCTSTR index,LPCTSTR date, LPCTSTR code) 
{
	CString key;
	CString	sDate = date;

	m_bStart = FALSE;
	sDate.Remove('/');
	key.Format("%s%s%s",index, sDate, code);

	RemoveCondition(key);
}

void CMainWnd::Run() 
{
	// TODO: Add your dispatch handler code here
	m_bStart = TRUE;
}

long CMainWnd::GetTickPrice(long price, short tick, LPCTSTR code, short iplus) 
{
	const	int	iCodeType = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)code);

	return getTickPrice(price,tick,iCodeType,iplus);
}

void CMainWnd::SetMapHandle(LPCTSTR sHandle) 
{
	// TODO: Add your dispatch handler code here
	m_sMapHandle = sHandle;
}

BSTR CMainWnd::GetMapHandle() 
{
	CString strResult;
	strResult = m_sMapHandle;

	return strResult.AllocSysString();
}

void CMainWnd::Reset() 
{
	CString	keys;
	std::shared_ptr<CCodeMap> pCode = nullptr;

	m_bStart = FALSE;
	for (POSITION pos = m_mapCode.GetStartPosition(); pos;)
	{
		m_mapCode.GetNextAssoc(pos, keys, pCode);
		pCode.reset();
	}
	m_mapCode.RemoveAll();
}

void CMainWnd::SendLog(CString sData)
{
	CString strLog;
	CString strSend;

	strLog.Format("I%s1%s","IB100500",sData);
	strSend.Format("A%04d000000%s",1,strLog);
	
	SendTR("PIDOLOGM", strSend, US_OOP, m_Param.key);
}

void CMainWnd::SendLogOnce()
{
	pidologm_mid mid{};
	CString strResult,strRead, strTmp;

	for (int ii = 0; ii < m_arLog.GetSize(); ii++)
	{
		strRead = m_arLog.GetAt(ii);
		
		FillMemory(&mid,sizeof(pidologm_mid), ' ');
		CopyMemory(mid.mapn, "IB100500", sizeof(mid.mapn));
		CopyMemory(mid.data, strRead, strRead.GetLength());
		mid.func[0] = 'I';
		mid.kind[0] = '1';
		
		strTmp = CString((char*)&mid, sizeof(pidologm_mid));
		strResult += strTmp;
	}

	CString strSend;
	strSend.Format("A%04d000000%s", m_arLog.GetSize(), strResult);
	
	SendTR("PIDOLOGM", strSend, US_OOP, m_Param.key);
}

void CMainWnd::WriteLog(LPCTSTR sType, LPCTSTR sLog, LPCTSTR sCode, LPCTSTR sName) 
{
	CString strDate, strTime;
	CString sSendLog;
	SYSTEMTIME	tm;

	GetLocalTime(&tm);
	strDate.Format("%04d%02d%02d", tm.wYear, tm.wMonth, tm.wDay);    
	strTime.Format("%02d%02d%02d", tm.wHour, tm.wMinute, tm.wSecond);    
	
	sSendLog.Format("%s;%s;%s;%s;%s;%s", strDate, strTime, sType, sCode, sName, sLog);
	
	KillTimer(2000);
	m_arLog.Add(sSendLog);

	SetTimer(2000,2000,NULL);
}

void CMainWnd::OnTimer(UINT nIDEvent) 
{
	CString sSendLog;

// 	for (int i=0;i<m_arLog.GetSize();i++)
// 	{
// 		sSendLog = m_arLog.GetAt(i);
// 		SendLog(sSendLog);
// 	}
	SendLogOnce();

	m_arLog.RemoveAll();

	KillTimer(2000);

	CWnd::OnTimer(nIDEvent);
}

void CMainWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CMainWnd::OnClose() 
{
	CWnd::OnClose();
}

void CMainWnd::SendTestData(LPCTSTR code, LPCTSTR curr) 
{
	CString sCode, sCurr;

	sCode.Format("%s", code);
	sCurr.Format("%s", curr);

	CheckRealData("A"+sCode, sCurr);
	/*
	CString string;
	string.Format("StopOrder\t%s,%s,%s,%s,%s,%d,%d",m_sMapHandle,"111","222","000660","99999", 2, "");
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, 0), (LPARAM)(LPCTSTR) string);
	*/
}

BOOL CMainWnd::RemoveCondition(CString keys)
{
	std::shared_ptr<CCodeMap> pCode = nullptr;
	if (m_mapCode.Lookup(keys, pCode))
	{
		pCode.reset();
		m_mapCode.RemoveKey(keys);
		return TRUE;
	}

	return FALSE;
}

int CMainWnd::checkDelta(int spVal, int icodetype)
{
	int	nRet = 0;

	if (m_bNewUnit)		// 2023 호가단위개선
	{
		if (icodetype == thirdType)
		{
			if (spVal < 1000)
				nRet = 1;
			else if (spVal >= 1000 && spVal < 5000)
				nRet = 5;
			else if (spVal >= 5000 && spVal < 10000)
				nRet = 10;
			else if (spVal >= 10000 && spVal < 50000)
				nRet = 50;
			else if (spVal >= 50000 && spVal < 100000)
				nRet = 100;
			else if (spVal >= 100000 && spVal < 500000)
				nRet = 500;
			else
				nRet = 1000;
		}
		else if (icodetype == etfType || icodetype == elwType)
		{
			nRet = 5;
		}
		else
		{
			if (spVal < 2000)
				nRet = 1;
			else if (spVal < 5000)
				nRet = 5;
			else if (spVal < 20000)
				nRet = 10;
			else if (spVal < 50000)
				nRet = 50;
			else if (spVal < 200000)
				nRet = 100;
			else if (spVal < 500000)
				nRet = 500;
			else
				nRet = 1000;
		}
	}
	else	// asis
	{
		if (icodetype == kospiType)
		{
			if (spVal < 1000)
				nRet = 1;
			else if (spVal >= 1000 && spVal < 5000)
				nRet = 5;
			else if (spVal >= 5000 && spVal < 10000)
				nRet = 10;
			else if (spVal >= 10000 && spVal < 50000)
				nRet = 50;
			else if (spVal >= 50000 && spVal < 100000)
				nRet = 100;
			else if (spVal >= 100000 && spVal < 500000)
				nRet = 500;
			else
				nRet = 1000;
		}
		else if (icodetype == kosdaqType)
		{
			if (spVal < 1000)
				nRet = 1;
			else if (spVal >= 1000 && spVal < 5000)
				nRet = 5;
			else if (spVal >= 5000 && spVal < 10000)
				nRet = 10;
			else if (spVal >= 10000 && spVal < 50000)
				nRet = 50;
			else
				nRet = 100;
		}
		else if (icodetype == thirdType)
		{
			if (spVal < 1000)
				nRet = 1;
			else if (spVal >= 1000 && spVal < 5000)
				nRet = 5;
			else if (spVal >= 5000 && spVal < 10000)
				nRet = 10;
			else if (spVal >= 10000 && spVal < 50000)
				nRet = 50;
			else if (spVal >= 50000 && spVal < 100000)
				nRet = 100;
			else if (spVal >= 100000 && spVal < 500000)
				nRet = 500;
			else
				nRet = 1000;
		}
		else if (icodetype == etfType || icodetype == elwType)
		{
			nRet = 5;
		}
		else
		{
			if (spVal < 1000)
				nRet = 1;
			else if (spVal >= 1000 && spVal < 5000)
				nRet = 5;
			else if (spVal >= 5000 && spVal < 10000)
				nRet = 10;
			else if (spVal >= 10000 && spVal < 50000)
				nRet = 50;
			else if (spVal >= 50000 && spVal < 100000)
				nRet = 100;
			else if (spVal >= 100000 && spVal < 500000)
				nRet = 500;
			else
				nRet = 1000;
		}
	}

	return nRet;
}

int CMainWnd::checkDelta(int spVal, int iCodeType, bool bPlus)
{
	int	nRet = 0;

	if (m_bNewUnit)		// 2023 호가단위개선
	{
		if (iCodeType == thirdType)
		{
			if (bPlus)
			{
				if (spVal < 1000)		nRet = 1;
				else if (spVal < 5000)		nRet = 5;
				else if (spVal < 10000)		nRet = 10;
				else if (spVal < 50000)		nRet = 50;
				else if (spVal < 100000)	nRet = 100;
				else if (spVal < 500000)	nRet = 500;
				else				nRet = 1000;
			}
			else
			{
				if (spVal <= 1000)		nRet = 1;
				else if (spVal <= 5000)		nRet = 5;
				else if (spVal <= 10000)	nRet = 10;
				else if (spVal <= 50000)	nRet = 50;
				else if (spVal <= 100000)	nRet = 100;
				else if (spVal <= 500000)	nRet = 500;
				else				nRet = 1000;
			}
		}
		else if (iCodeType == etfType || iCodeType == elwType)
		{
			nRet = 5;
		}
		else
		{
			if (bPlus)
			{
				if (spVal < 2000)		nRet = 1;
				else if (spVal < 5000)		nRet = 5;
				else if (spVal < 20000)		nRet = 10;
				else if (spVal < 50000)		nRet = 50;
				else if (spVal < 200000)	nRet = 100;
				else if (spVal < 500000)	nRet = 500;
				else				nRet = 1000;
			}
			else
			{
				if (spVal <= 1)			nRet = 0;
				else if (spVal <= 2000)		nRet = 1;
				else if (spVal <= 5000)		nRet = 5;
				else if (spVal <= 20000)	nRet = 10;
				else if (spVal <= 50000)	nRet = 50;
				else if (spVal <= 200000)	nRet = 100;
				else if (spVal <= 500000)	nRet = 500;
				else				nRet = 1000;
			}
		}
	}
	else	// asis
	{
		if (iCodeType == kospiType)
		{
			if (bPlus)
			{
				if (spVal < 1000)		nRet = 1;
				else if (spVal < 5000)		nRet = 5;
				else if (spVal < 10000)		nRet = 10;
				else if (spVal < 50000)		nRet = 50;
				else if (spVal < 100000)	nRet = 100;
				else if (spVal < 500000)	nRet = 500;
				else				nRet = 1000;
			}
			else
			{
				if (spVal <= 1000)		nRet = 1;
				else if (spVal <= 5000)		nRet = 5;
				else if (spVal <= 10000)	nRet = 10;
				else if (spVal <= 50000)	nRet = 50;
				else if (spVal <= 100000)	nRet = 100;
				else if (spVal <= 500000)	nRet = 500;
				else				nRet = 1000;
			}
		}
		else if (iCodeType == kosdaqType)
		{
			if (bPlus)
			{
				if (spVal < 1000)		nRet = 1;
				else if (spVal < 5000)		nRet = 5;
				else if (spVal < 10000)		nRet = 10;
				else if (spVal < 50000)		nRet = 50;
				else				nRet = 100;
			}
			else
			{
				if (spVal <= 1000)		nRet = 1;
				else if (spVal <= 5000)		nRet = 5;
				else if (spVal <= 10000)	nRet = 10;
				else if (spVal <= 50000)	nRet = 50;
				else				nRet = 100;
			}
		}
		else if (iCodeType == thirdType)
		{
			if (bPlus)
			{
				if (spVal < 1000)		nRet = 1;
				else if (spVal < 5000)		nRet = 5;
				else if (spVal < 10000)		nRet = 10;
				else if (spVal < 50000)		nRet = 50;
				else if (spVal < 100000)	nRet = 100;
				else if (spVal < 500000)	nRet = 500;
				else				nRet = 1000;
			}
			else
			{
				if (spVal <= 1000)		nRet = 1;
				else if (spVal <= 5000)		nRet = 5;
				else if (spVal <= 10000)	nRet = 10;
				else if (spVal <= 50000)	nRet = 50;
				else if (spVal <= 100000)	nRet = 100;
				else if (spVal <= 500000)	nRet = 500;
				else				nRet = 1000;
			}
		}
		else if (iCodeType == etfType || iCodeType == elwType)
		{
			nRet = 5;
		}
		else
		{
			if (bPlus)
			{
				if (spVal < 1000)		nRet = 1;
				else if (spVal < 5000)		nRet = 5;
				else if (spVal < 10000)		nRet = 10;
				else if (spVal < 50000)		nRet = 50;
				else				nRet = 100;
			}
			else
			{
				if (spVal <= 1000)		nRet = 1;
				else if (spVal <= 5000)		nRet = 5;
				else if (spVal <= 10000)	nRet = 10;
				else if (spVal <= 50000)	nRet = 50;
				else				nRet = 100;
			}
		}
	}

	return nRet;
}

int CMainWnd::getTickPrice(int price, int tick, int codetype, int iplus)
{
#if 0
	// price에 따른 호가 단위 계산하여 tick만큼 loop
	int nPrice = price;
	int profit1 = checkDelta(nPrice, codetype);

	for (int i = 0; i < tick; i++)
	{
		if (iplus > 0)	nPrice += profit1;
		else			nPrice -= profit1;
	}
	nPrice -= nPrice % profit1;
	return nPrice;
#else
	int	delta = 0, tickval = 0;

	if (iplus > 0)
	{
		for (int n = 0; n < tick; n++)
		{
			delta = checkDelta(price, codetype, true);
			tickval += delta;
			price += delta;
		}
	}
	else
	{
		for (int n = 0; n < tick; n++)
		{
			delta = checkDelta(price, codetype, false);
			tickval -= delta;
			price -= delta;
		}
	}
	price -= (price % checkDelta(price, codetype, (iplus > 0) ? true : false));
	return price;
#endif
}

int CMainWnd::getTickValue(int price, int tick, int codetype)
{
#if 0
	int profit1 = checkDelta(price, codetype);
	return profit1 * tick;
#else
	int	delta = 0, tickval = 0;
	if (tick > 0)
	{
		for (int n = 0; n < tick; n++)
		{
			delta = checkDelta(price, codetype, true);
			tickval += delta;
			price += delta;
		}
	}
	else
	{
		for (int n = tick; n < 0; n++)
		{
			delta = checkDelta(price, codetype, false);
			tickval -= delta;
			price -= delta;
		}
	}
	return tickval;
#endif
}

int CMainWnd::getPrice(int price, int codetype, int iroundup/*올림*/)
{
	const	int	iprofit = checkDelta(price, codetype);
	const	int	idiv = price % iprofit;

	if (idiv > 0)
	{
		price -= idiv;
		if (iroundup > 0)
			price += iprofit;

	}
	return price;
}