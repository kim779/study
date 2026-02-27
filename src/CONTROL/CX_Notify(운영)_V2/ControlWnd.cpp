// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Notify.h"
#include "ControlWnd.h"
#include "ShMemory.h"

#include "../../h/ledger.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define WM_REMAIN		WM_USER + 2
#define WM_ALLACCNREMAIN	WM_USER + 3
#define WM_REMAIN_CLOSE		WM_USER + 4
#define WM_LAWCHEG		WM_USER + 5

#define	ENCTEST

#define	SC_TEST				// CCriticalSection test
#define	CREDIT				// 신용잔고 test

#define	CREDIT_DATE		11	// 신용잔고날짜 index
#define	CREDIT_SYGB		12	// 신용구분 index
#define CREDIT_JGGB		16	// 잔고구분 index

#define LEN_JGGB		2	// 잔고구분
#define	LEN_ACODE		12	// 주식 전체코드 길이
#define	LEN_SYGB		2	// 신용구분 길이
#define	LEN_DATE		8	// 날짜

#define TM_BALANCE 9898
#define VS_NOMAL "1"
#define VS_SKIP "2"
#define VS_TIMER "3"

CString GetField(const CString& src, int index)
{
	CString tmp = src;
	CString token;
	int pos = 0;
	int i = 0;

	while (!(token = tmp.Tokenize(_T("\t"), pos)).IsEmpty())
	{
		if (i == index)
			return token;
		i++;
	}
	return "";
}

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_ShMemory = nullptr;
	m_arField.RemoveAll();
	m_bAllAccn = false;
	m_bFuture = false;
	m_bLaw = false;
	m_bFilter = false;
	m_iJanType = 0;

}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_REMAIN, OnRemainMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "SetAccount", m_setAccount, OnSetAccountChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataList", m_dataList, OnDataListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Flag", m_flag, OnFlagChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataMsg", m_dataMsg, OnDataMsgChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "SetMcgb", m_setMcgb, OnSetMcgbChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Version", m_Version, OnVersionChanged, VT_BSTR)
	//DISP_PROPERTY_NOTIFY_ID(CControlWnd, "Version", dispidVersion, m_Version, OnVersionChanged, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Send", Send, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendEx", SendEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendMarketEx", SendMarketEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "AllAccnSend", AllAccnSend, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "AddReminData", AddReminData, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendX", SendX, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendRaw", SendRaw, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetFilterAcc", SetFilterAcc, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "RequestBalance", RequestBalance, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetSkipTime", SetSkipTime, VT_EMPTY, VTS_I2)
	//}}AFX_DISPATCH_MAP
	//DISP_FUNCTION_ID(CControlWnd, "RequestBalance", dispidRequestBalance, RequestBalance, VT_EMPTY, VTS_BSTR)
	
END_DISPATCH_MAP()

// {7E5363E5-D4A1-4866-B9FF-A97FC342672D}
static const IID IID_IControlWnd =
{ 0x7e5363e5, 0xd4a1, 0x4866, {0xb9, 0xff, 0xa9, 0x7f, 0xc3, 0x42, 0x67, 0x2d}};

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

#define DF_SHARED
//=================================================================================================
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#ifdef	SC_TEST
	m_cs.Lock();
#endif
	CString path = Variant(homeCC, "");
	CString userPath;
	userPath.Format("%s\\tab\\%s", path, "AXISAI.INI");
	m_Version.Format("%d", GetPrivateProfileInt("Balance", "ver", 1, userPath));
	m_Version.TrimRight();

	m_interval = GetPrivateProfileInt("Balance", "time", 500, userPath);
	m_diffSec = GetPrivateProfileInt("Balance", "diff", 1, userPath);

	if (m_Version == VS_TIMER)
		SetTimer(TM_BALANCE, m_interval, nullptr);

	m_slog.Format("[체결][cx_notify][%s]<%d> ------  m_Version ------[%s] ", __FUNCTION__, __LINE__, m_Version);
	Output_DebugString(m_slog);

	path.Replace("\\", "_");

	m_ShMemory = std::make_unique<CShMemory>();

#ifdef DF_SHARED
	DWORD processID = GetCurrentProcessId();
	CString sSHMENAME;

	if (!m_bFuture)
	{
		sSHMENAME.Format("%s%s%d", path, "_remainCtrl_", processID);
		m_ShMemory->m_remainSHMEMNAME = sSHMENAME;
	}
	else
	{
		sSHMENAME.Format("%s%s%d", path, "_remainCtrl_Future", processID);
		m_ShMemory->m_remainSHMEMNAME = sSHMENAME;
	}
#else
	if (!m_bFuture)
		m_ShMemory->m_remainSHMEMNAME = path + "_remainCtrl_";
	else
		m_ShMemory->m_remainSHMEMNAME = path + "_remainCtrl_Future";
#endif

	m_ShMemory->InitSharedMemory(this->m_hWnd);
	m_ShMemory->AddHandle(this->m_hWnd);

#ifdef	SC_TEST
	m_cs.Unlock();
#endif
	return 0;
}

void CControlWnd::OnDestroy() 
{
	if (m_ShMemory == nullptr)
		return;

	HWND	MasterHwnd = m_ShMemory->GetHandle(0);
	
	//CString	Data;
	//Data.Format("%d\t%s\t%s\t%d\t", (m_bFuture?1:0), m_setAccount, "close", 1);	// 1:신용사용

	m_ShMemory->RemoveHandle(this->m_hWnd);
	m_ShMemory.reset();

	::SendMessage(MasterHwnd, WM_REMAIN_CLOSE, (WPARAM)this->m_hWnd, (LPARAM)m_setAccount.GetString());

	CWnd::OnDestroy();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		m_sCtrlData.Empty();
		return (long)m_sCtrlData.GetString();
	case DLL_OUB:
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:	// Remain Data Send to Map!
		if (!m_bLaw)
		{
			// IBXXXX01 sheared 에서 보내주는 데이타(실시간잔고)
			CString sData = (char*)lParam, sAccn, sTemp;
			if (m_bAllAccn)
			{
				sAccn = Parser(sData, "\t");
				if (m_AccnMap.Lookup(sAccn, sTemp))
					SendToMap(sData, false, sAccn);	
			}
			else if (m_setAccount == Parser(sData, "\t"))
				SendToMap(sData, false);
		}
		else
		{
				
			CString sData = (char*)lParam;
			if (m_bFilter)
			{
				CString filler;
				if (!m_AccnFilter.Lookup(sData.Mid(0,11), filler))
					return 0;
			}

			const	int pos = sData.Find("#");

			m_dataList = sData.Mid(0, pos);
			m_dataMsg = sData.Mid(pos+1, sData.GetLength()-pos);

			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)), (LPARAM)m_Param.name.GetString());  //법인용 안씀
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick/*DblClick*/)),
						(LPARAM)m_Param.name.GetString());
		}
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		{
			m_Param.point = HIWORD(wParam);
			CString sFont = CString((char*)lParam);
			if (!sFont.IsEmpty())
				m_Param.fonts = sFont;
		}
		break;
	case DLL_NOTICE:
		break;
	}
	return 0;
}

// IBXXXX01 sheared 에서 보내주는 데이타 (잔고 데이타 및 에러메세지)
long CControlWnd::OnRemainMessage(WPARAM wParam, LPARAM lParam)
{
	CString sData = (char*)lParam;

	if (Parser(sData, "\t") == "ERR")	// guide 메세지(오류 or 정상), guide + data
	{
		sData = "guide\t" + sData;

		m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)sData.GetString());

		if (sData.GetAt(0) != _T('0'))	// 오류
		{
			m_flag = _T("E");
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),  //err
						(LPARAM)m_Param.name.GetString());
		}
	}
	else
	{
		SendToMap((char*)lParam, true);	// 전체 data
	}
	return 0;
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this);
	CRect	rc;

	GetClientRect(rc);
	dc.FillSolidRect(rc, RGB(0,0,0));
}


//=================================================================================================
void CControlWnd::OnSetAccountChanged() 
{
}

void CControlWnd::OnDataListChanged() 
{
}

void CControlWnd::OnFlagChanged() 
{
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;
	return strResult.AllocSysString();
}

void CControlWnd::SetProperties(LPCTSTR sProperties) 
{
	m_Param.options = sProperties;
}

// 잔고조회
void CControlWnd::Send(LPCTSTR sAccn, LPCTSTR sPswd,LPCTSTR dfee,LPCTSTR dmass) 
{
	SendEx(sAccn, sPswd, dfee, dmass, _T("0"));
}


void CControlWnd::SendEx( LPCTSTR sAccn, LPCTSTR sPswd, LPCTSTR dfee, LPCTSTR dmass, LPCTSTR dCalcType/*=_T("0")*/ )
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bLaw = false;
	m_CodeMap.RemoveAll();
	
	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t", m_bFuture ? 1 : 0, 
							sAccn,
							sPswd,
							1,
							dfee,
							dmass,
							dCalcType,
							m_setMcgb);	// 1:신용사용	//2015.04.22 KSJ 매체구분 추가
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}

void CControlWnd::SendMarketEx(LPCTSTR sAccn, LPCTSTR sPswd, LPCTSTR dfee, LPCTSTR dmass, LPCTSTR dCalcType/*=_T("0")*/, LPCTSTR sFiller)
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bLaw = false;
	m_CodeMap.RemoveAll();

	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\t", m_bFuture ? 1 : 0,
		sAccn,
		sPswd,
		1,
		dfee,
		dmass,
		dCalcType,
		m_setMcgb,  // 1:신용사용	//2015.04.22 KSJ 매체구분 추가
		sFiller);
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}

// 잔고조회 : 신용제외
void CControlWnd::SendX(LPCTSTR sAccn, LPCTSTR sPswd) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_CodeMap.RemoveAll();

	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t%s\t", m_bFuture ? 1 : 0,
					    sAccn,
					    sPswd,
					    0,
					    m_setMcgb);			// 신용제외	//2015.04.22 KSJ 매체구분 추가
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}


// 전체잔고조회 : 사용여부 ?
void CControlWnd::AllAccnSend(LPCTSTR sUserID, LPCTSTR sPswd, LPCTSTR sCode) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bAllAccn = true;
	m_CodeMap.RemoveAll();

	Data.Format("%d\t%s\t%s\t%s", m_bFuture ? 1 : 0,
				      sUserID,
				      sPswd,
				      sCode);
	::SendMessage(MasterHwnd, WM_ALLACCNREMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}

void CControlWnd::Clear() 
{
	m_setAccount.Empty();
	m_CodeMap.RemoveAll();
	m_AccnMap.RemoveAll();
}

void CControlWnd::AddReminData(LPCTSTR sAccn, LPCTSTR sCode, LPCTSTR sRowData) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_AccnMap.SetAt(sAccn, "1");
	Data.Format("%d\t%s\t%s\t%d\t%s\t", m_bFuture ? 1 : 0,
					    sAccn,
					    sCode,
					    1,
					    sRowData);	// 신용사용
	::SendMessage(MasterHwnd, WM_ALLACCNREMAIN, (WPARAM)this->m_hWnd, (LPARAM)Data.GetString());
}

//=================================================================================================
int FindTokenIndex(const CString& src, const CString& target, TCHAR delim)
{
	int index = 0;
	int start = 0;

	while (true)
	{
		int pos = src.Find(delim, start);

		CString token;
		if (pos == -1)
		{
			token = src.Mid(start);
		}
		else
		{
			token = src.Mid(start, pos - start);
		}

		if (token == target)
			return index;

		if (pos == -1)
			break;

		start = pos + 1;
		index++;
	}

	return -1; // 못찾음
}

void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmp = m_Param.options;
	CString fieldS = OptionParser(tmp, "/edit");
	m_iPsCmtIndex = FindTokenIndex(fieldS, "05",'|');
	m_iPsCmtIndex -= 1;
	fieldS.Replace("|", "\t");
	m_bFuture = atoi(Parser(fieldS, "\t"))?true:false;
	if (tmp.Find("accnAll") != -1)
		m_bAllAccn = true;

	UINT	idx = 0;
	while (!fieldS.IsEmpty())
	{
		idx = atoi(Parser(fieldS, "\t"));
		m_arField.Add(idx);
	}

	CString sJtype = OptionParser(tmp, "/q");  //0전체 1유통대주만 2유통대주제외
	if (!sJtype.IsEmpty())
		m_iJanType = atoi(sJtype);

}

// bAll -> true : 잔고조회, false : 실시간 잔고
// sAccn -> !IsEmpty() : 복수계좌에서사용

void CControlWnd::SendToMap(CString sData, bool bAll, CString sAccn/* = ""*/)
{
#ifdef	SC_TEST
	m_cs.Lock();
#endif
	m_slog.Format("[cx_notify]-------------------------------------------------------------------------------");
	Output_DebugString(m_slog);
	m_slog.Format("[cx_notify] size=[%d]     bAll=[%d] sAccn=[%s] [%s]", m_drawQueue.size(), bAll, sAccn, sData);
	Output_DebugString(m_slog);
	CString sSendData, sCodeData, sCode;
	CString keyS, dateS, sygbS, jggb;
	CStringArray sDataArr;
	int	ii = 0, idx = 0, bound = 0;
	bool quantityChanged = true;
	if (bAll)	
	{
		//==================================================
		// 잔고조회
		// flag : 'A'
		// 코드갯수\t + { {편집데이타\t} * n개 +  \n }
		//==================================================
		m_CodeMap.RemoveAll();

		while (!sData.IsEmpty())
		{
			sDataArr.RemoveAll();
						
			sCodeData = Parser(sData, "\n");		// 종목별 잔고
			while (!sCodeData.IsEmpty())
				sDataArr.Add(Parser(sCodeData, "\t"));		
			if (sDataArr.GetSize() <= 0)
				continue;

			bound = sDataArr.GetUpperBound();
			sCode = sDataArr.GetAt(0);
			if (m_bFuture)
				keyS = getKey(sCode, "", "", "");
			else
			{
				getDate(sDataArr, dateS, sygbS, jggb);
				keyS = getKey(sCode, dateS, sygbS, jggb);
			}

			if (m_iJanType == 1 && sygbS != "05")   //유통대주만
				continue;

			if (m_iJanType == 2 && sygbS == "05")	//유통대주 제외
				continue;
						
			for (ii = 0; ii < m_arField.GetSize(); ii++)
			{
				idx = m_arField.GetAt(ii);
				if (idx > bound)
					continue;
				sSendData += sDataArr.GetAt(idx) + "\t";
				m_slog.Format("[cx_notify][%s]<%d> idx =[%d] val =[%s]", __FUNCTION__, __LINE__, idx, sDataArr.GetAt(idx));
				Output_DebugString(m_slog);
			}
			m_CodeMap.SetAt(keyS, sSendData);
			sSendData += "\n";
		}
		m_flag = "A";
		m_dataList.Format("%d\t", m_CodeMap.GetCount());
		m_dataList += sSendData;

		//if (m_Version == VS_SKIP || m_Version == VS_TIMER)
		//{
			m_cs.Unlock();
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)), (LPARAM)m_Param.name.GetString());  //SendToMap
			return;
		//}
	}
	else
	{
		//==================================================
		// IBXXXX01에서 받는 데이타 : sData = "sFlag \t sData \t ..."
		// 실시간 잔고
		// flag : 'I'(insert), 'D'(delete), 'U'(update)
		// 일반 : code + {편집데이타\t} * n개 
		// 복수계좌 : 계좌번호 + code + 계좌번호\t + {편집데이타\t} * n개 
		// code + date(8)
		//==================================================
		const	int	FindIndex = sData.Find("|");
		
		if (FindIndex > -1)
			sData.Delete(FindIndex,sData.GetLength()-FindIndex);

		if (sData.IsEmpty())
		{
#ifdef	SC_TEST
			m_cs.Unlock();
#endif
			return;
		}
		
		m_flag = Parser(sData, "\t");
		
		while (!sData.IsEmpty())
			sDataArr.Add(Parser(sData, "\t"));
		if (sDataArr.GetSize() <= 0)
		{
#ifdef	SC_TEST
			m_cs.Unlock();
#endif
			return;
		}
		
		bound = sDataArr.GetUpperBound();
		sCode = sDataArr.GetAt(0);
		//AfxMessageBox("[sCode: "+sCode+"]");
		if (m_bFuture)
			keyS = getKey(sCode, "", "", "");
		else
		{
			if (m_flag != "D")
			{
				getDate(sDataArr, dateS, sygbS, jggb);
				keyS = getKey(sCode, dateS, sygbS, jggb);

				if (m_iJanType == 1 && sygbS != "05")   //유통대주만
					return;

				if (m_iJanType == 2 && sygbS == "05") //유통대주 제외
					return;
			}
			else
			{
				/*
				if (sygbS == "  ")
				{
					keyS = sCode.Left(22)+"00";
				}
				else
				*/
					keyS = sCode;
			}
		}

		if (m_flag == "D")					// Delete
			m_CodeMap.RemoveKey(keyS);
		else
		{
			if (!m_CodeMap.Lookup(keyS, sSendData))		// Insert
				m_flag = "I";				
		
			sSendData.Empty();
			if (m_bAllAccn)
				sSendData = sAccn + "\t";

			for (ii = 0; ii < m_arField.GetSize(); ii++)
			{
				idx = m_arField.GetAt(ii);
				if (idx > bound)	continue;
				sSendData += sDataArr.GetAt(idx) + "\t";
			}

			CString oldData;
			if (m_CodeMap.Lookup(keyS, oldData))
			{
				quantityChanged = IsQuantityChanged(oldData, sSendData);

m_slog.Format("[cx_notify]");
Output_DebugString(m_slog);
m_slog.Format("[cx_notify][%s]<%d> ------ [%s]  keyS=[%s]  ] oldData=[%s] ", __FUNCTION__, __LINE__, quantityChanged == 1?"가능수량변동":"변화없다",keyS, oldData);
Output_DebugString(m_slog);
			}

			m_CodeMap.SetAt(keyS, sSendData);
		}
#ifdef	CREDIT
		if (m_bFuture)
			m_dataList = sCode + sSendData;	
		else	// 현물 : code(11) + date(8) + 신용구분(2)
			m_dataList = keyS + sSendData;	
#else
		m_dataList = sCode + sSendData;	
#endif
		if (m_bAllAccn)
			m_dataList = sAccn + m_dataList;
	}

	if (m_diffSec > 0)
	{
		CSingleLock lock(&m_lock, TRUE);
		if (quantityChanged || !ShouldSkipRTSByTimeDiff(sCode))
		{
			m_drawQueue.push(m_dataList);
			m_slog.Format("[cx_notify]]  [%s]<%d>   넣어주었다 !!!! ", sCode, m_drawQueue.size());
			Output_DebugString(m_slog);
		}
		else
		{
		/*	m_slog.Format("[cx_notify]]  [%s]<%d>   안넣어주었다 !!!! ", sCode, m_drawQueue.size());
			Output_DebugString(m_slog);*/
		}

		if (m_bWaitingFromVBS)
			SendNextToVBS();
	}
	//else if (m_Version == VS_TIMER)
	//{
	//	CSingleLock lock(&m_lock, TRUE);
	//	m_drawQueue.push(m_dataList);
	//}
	else if(m_diffSec == 0)
	{
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)), (LPARAM)m_Param.name.GetString());  //SendToMap
	}

#ifdef	SC_TEST
	m_cs.Unlock();
#endif
}

void CControlWnd::SendNextToVBS()
{
	CString data;

	
	CSingleLock lock(&m_lock, TRUE);

	if (m_drawQueue.empty())
	{
		m_bWaitingFromVBS = TRUE;  // 다음 요청 대기
		return;
	}

	m_dataList = m_drawQueue.front();
	m_drawQueue.pop();
	m_bWaitingFromVBS = FALSE;
	
	// VBScript에 전달 (예: IDispatch 호출 / WebBrowser 호출)
	m_pParent->PostMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)m_Param.name.GetString());  //SendNextToVBS()
}

BOOL CControlWnd::SendTR(CString sTR, BYTE type, CString data, int key /*= -1*/)
{
	struct	_userTH	udat {};
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(L_userTH + data.GetLength() + 128);
	int	idx = 0;

	if (key != -1)
	{
		type |= US_KEY;
		pWb[idx++] = key;
		key = MAKELONG(key, m_Param.key);
	}
	else
	{
		type &= ~US_KEY;
		key = m_Param.key;
		idx = m_Param.name.GetLength();
		CopyMemory(pWb.get(), m_Param.name.GetString(), idx);
	}
	pWb[idx++] = '\t';

	CopyMemory(udat.trc, sTR.GetString(), min(sizeof(udat.trc), sTR.GetLength()));
	udat.key  = key;
	udat.stat = type;

	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	CopyMemory(&pWb[idx], data.GetString(), data.GetLength());

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)pWb.get());
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

int CControlWnd::OpenView(int type, CString data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.GetString());
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;

	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}
 
// 주식 : code(12) + date(8) + sygb(2)
CString CControlWnd::getKey(CString codeS, CString dateS, CString sygbS, CString jggb)
{
	if (m_bFuture)
		return codeS;

	/*
				
	if (atoi(sygbS) >= 10 || atoi(sygbS) == 0)	// 신용구분 아니면 0
		sygbS = "  ";
	*/
	CString keyS;

	keyS.Format("%-*s%-*s%-*s%-*s", LEN_JGGB, jggb, LEN_ACODE, codeS, LEN_DATE, dateS, LEN_SYGB, sygbS);
	return keyS;
}

void CControlWnd::getDate(const CStringArray& arData, CString& dateS, CString& sygbS, CString& jggb)
{
	dateS.Empty();
	sygbS.Empty();

	if (m_bFuture || arData.GetUpperBound() < CREDIT_SYGB)
		return;

	dateS = arData.GetAt(CREDIT_DATE);
	sygbS = arData.GetAt(CREDIT_SYGB);
	jggb  = arData.GetAt(CREDIT_JGGB);
}

void CControlWnd::SendRaw(LPCTSTR sName) 
{
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bLaw = true;
	m_CodeMap.RemoveAll();

	::SendMessage(MasterHwnd, WM_LAWCHEG, (WPARAM)this->m_hWnd, (LPARAM)sName);
}

void CControlWnd::SetFilterAcc(LPCTSTR sAccList) 
{
	CString list = sAccList;
	CString acc;

	m_AccnFilter.RemoveAll();
	while (!list.IsEmpty())
	{
		acc = Parser(list, "\t");
		acc.TrimRight();
		m_AccnFilter.SetAt(acc, "account");
	}
	
	m_bFilter = (m_AccnFilter.GetCount() > 0) ? true : false;
}

void CControlWnd::OnDataMsgChanged() 
{
	// TODO: Add notification handler code

}

void CControlWnd::OnSetMcgbChanged() 
{
	// TODO: Add notification handler code

}


void CControlWnd::RequestBalance(BSTR sVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//m_slog.Format("[cx_notify][%s]<%d> ------  m_Version ------[%s] size=[%d]", __FUNCTION__, __LINE__, m_Version, m_drawQueue.size());
	//Output_DebugString(m_slog);

	if (m_Version == VS_SKIP)
	{
		m_bWaitingFromVBS = TRUE;
		SendNextToVBS();
	}
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}

void CControlWnd::SetSkipTime(SHORT sec)
{
	m_diffSec = sec;

	m_slog.Format("[cx_notify][%s]<%d> ------  스킵시간지정  m_diffSec=[%d] sec=[%d]", __FUNCTION__, __LINE__, m_diffSec, sec);
	Output_DebugString(m_slog);
}

void CControlWnd::OnVersionChanged()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (m_Version == VS_TIMER)
	{
		KillTimer(TM_BALANCE);
		SetTimer(TM_BALANCE, m_interval, nullptr);
	}
	// TODO: 여기에 속성 처리기 코드를 추가합니다.
}


void CControlWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_BALANCE:
		{
			if (m_drawQueue.empty())
				return;

			m_slog.Format("[체결][cx_notify][%s]<%d> ------  m_Version ------[%s] size = [%d]", __FUNCTION__, __LINE__, m_Version, m_drawQueue.size());
			Output_DebugString(m_slog);

			m_bWaitingFromVBS = TRUE;
			SendNextToVBS();
		}
		break;

	}
	CWnd::OnTimer(nIDEvent);
}

bool CControlWnd::ShouldSkipRTSByTimeDiff(CString sCode)
{ //false 실시간 처리   ,    true 스킵
	if (m_flag != "U")
		return false;

	std::string code = CStringA(sCode);
	auto it = m_lastRTSTimeMap.find(code);
	const CTime now = CTime::GetCurrentTime();
	// 첫 수신은 무조건 통과
	if (it == m_lastRTSTimeMap.end())
	{

		m_lastRTSTimeMap.emplace(sCode, now);
m_slog.Format("[cx_notify][%s]<%d> ------  첫수신  return  sCode=[%s]", __FUNCTION__, __LINE__, sCode);
//Output_DebugString(m_slog);
		return false;
	}
	
	const CTime& prevServerTime = it->second;
	int diffSec = (int)(now - prevServerTime).GetTotalSeconds();

	if (diffSec <= m_diffSec)
	{
		m_slog.Format("[cx_notify][%s]<%d> ------  스킵  return  sCode=[%s]  diffSec=[%d] m_flag=[%s] m_diffSec=[%d] ", __FUNCTION__, __LINE__, sCode, diffSec, m_flag, m_diffSec);
		Output_DebugString(m_slog);
		return true;
	}

	m_slog.Format("[cx_notify][%s]<%d> ------  스킵안함  return  sCode=[%s] diffSec=[%d] m_flag=[%s] m_diffSec=[%d]", __FUNCTION__, __LINE__, sCode, diffSec, m_flag, m_diffSec);
	Output_DebugString(m_slog);
	it->second = now;
	return false;
}

bool CControlWnd::IsQuantityChanged(const CString& oldD, const CString& newD)
{
	if (oldD.IsEmpty())
		return true;   // 이전 없으면 변화로 간주
	if (GetField(oldD, 0) != GetField(newD, 0))
	{
		m_slog.Format("************* [cx_notify][% s]< % d>  return  종목코드 = [% s] 종목코드 = [% s]  서로다르다", __FUNCTION__, __LINE__, GetField(oldD, 1), GetField(newD, 1));
		Output_DebugString(m_slog);
		return false;
	}


	CString oldQty = GetField(oldD, m_iPsCmtIndex); // 4번째
	CString newQty = GetField(newD, m_iPsCmtIndex);

	m_slog.Format("			[cx_notify][%s]<%d>  return  oldQty=[%s] newQty=[%s] ", __FUNCTION__, __LINE__, oldQty, newQty);
	Output_DebugString(m_slog);

	m_slog.Format("			[cx_notify][%s]<%d oldD=[%s]  ", __FUNCTION__, __LINE__, oldD);
	Output_DebugString(m_slog);
	m_slog.Format("			[cx_notify][%s]<%d>newD=[%s] ", __FUNCTION__, __LINE__, newD);
	Output_DebugString(m_slog);


	return oldQty != newQty;
}