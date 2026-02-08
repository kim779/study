// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Notify.h"
#include "ControlWnd.h"
#include "ShMemory.h"

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

#define	SC_TEST			// CCriticalSection test
#define	CREDIT			// 신용잔고 test

#define	CREDIT_DATE	11	// 신용잔고날짜 index
#define	CREDIT_SYGB	12	// 신용구분 index
#define CREDIT_JGGB	16	// 잔고구분 index

#define LEN_JGGB	2		// 잔고구분
#define	LEN_ACODE	12		// 주식 전체코드 길이
#define	LEN_SYGB	2		// 신용구분 길이
#define	LEN_DATE	8		// 날짜

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;
	m_ShMemory = nullptr;

	m_arField.RemoveAll();
	m_bAllAccn = false;
	m_bLaw = false;
	m_bFilter = false;
	m_bFuture = false;
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
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "SetAccount", m_setAccount, OnSetAccountChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataList", m_dataList, OnDataListChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Flag", m_flag, OnFlagChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataMsg", m_dataMsg, OnDataMsgChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Send", Send, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "AllAccnSend", AllAccnSend, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "AddReminData", AddReminData, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendX", SendX, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendRaw", SendRaw, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetFilterAcc", SetFilterAcc, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendEx", SendEx, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {7E5363E5-D4A1-4866-B9FF-A97FC342672D}
static const IID IID_IControlWnd =
{ 0x7e5363e5, 0xd4a1, 0x4866, {0xb9, 0xff, 0xa9, 0x7f, 0xc3, 0x42, 0x67, 0x2d}};

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

//=================================================================================================
int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
#ifdef	SC_TEST
	m_cs.Lock();
#endif
	CString path = Variant(homeCC, "");
	path.Replace("\\", "_");

	m_ShMemory = std::make_unique<CShMemory>();
	m_ShMemory->remainSHMEMNAME = path + "_remainCtrl_API";
	m_ShMemory->InitSharedMemory(this->m_hWnd);
	m_ShMemory->AddHandle(this->m_hWnd);

#ifdef	SC_TEST
	m_cs.Unlock();
#endif
	return 0;
}

void CControlWnd::OnDestroy() 
{
	HWND MasterHwnd = m_ShMemory->GetHandle(0);
	
	m_ShMemory->RemoveHandle(this->m_hWnd);
	m_ShMemory.reset();
	::SendMessage(MasterHwnd, WM_REMAIN_CLOSE, (WPARAM)this->m_hWnd, (LPARAM)m_setAccount.GetString());

	CWnd::OnDestroy();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		m_sCtrlData.Empty();
		return (long)m_sCtrlData.operator LPCTSTR();
	case DLL_OUB:
		break;
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:	// Remain Data Send to Map!
		if (!m_bLaw)
		{
			// IBXXXX01 sheared 에서 보내주는 데이타(실시간잔고)
			const	int	key = HIBYTE(LOWORD(wParam));
			CString	sAccn, sTemp, sData = (char*)lParam;
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
				if (!m_AccnFilter.Lookup( sData.Mid(0,11), filler))
				{
					return 0;
				}

			}

			const	int	pos = sData.Find("#");
				
			m_dataList = sData.Mid(0, pos);
			m_dataMsg = sData.Mid(pos+1, sData.GetLength()-pos);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
							(LPARAM)m_Param.name.GetString());
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick/*DblClick*/)),
							(LPARAM)m_Param.name.GetString());
		}
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		{
			CString sFont = CString((char*)lParam);
			m_Param.point = HIWORD(wParam);
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
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
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

void CControlWnd::SendEx( LPCTSTR sAccn, LPCTSTR sPswd ,LPCTSTR dfee,LPCTSTR dmass,LPCTSTR dCalcType/*=_T("0")*/ )
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bLaw = false;
	m_CodeMap.RemoveAll();
	
	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t%s\t%s\t%s\t", (m_bFuture?1:0),m_setAccount,sPswd,1,dfee,dmass,dCalcType);	// 1:신용사용
	
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)(LPCTSTR)Data);
}

// 잔고조회 : 신용제외
void CControlWnd::SendX(LPCTSTR sAccn, LPCTSTR sPswd) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_CodeMap.RemoveAll();
	m_setAccount = sAccn;
	Data.Format("%d\t%s\t%s\t%d\t", (m_bFuture?1:0), m_setAccount, sPswd, 0);	// 신용제외
	::SendMessage(MasterHwnd, WM_REMAIN, (WPARAM)this->m_hWnd, (LPARAM)(LPCTSTR)Data);
}

// 전체잔고조회 : 사용여부 ?
void CControlWnd::AllAccnSend(LPCTSTR sUserID, LPCTSTR sPswd, LPCTSTR sCode) 
{
	CString	Data;
	HWND MasterHwnd = m_ShMemory->GetHandle(0);

	m_bAllAccn = true;
	m_CodeMap.RemoveAll();
	Data.Format("%d\t%s\t%s\t%s", (m_bFuture?1:0), sUserID, sPswd, sCode);
	::SendMessage(MasterHwnd, WM_ALLACCNREMAIN, (WPARAM)this->m_hWnd, (LPARAM)(LPCTSTR)Data);
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
	Data.Format("%d\t%s\t%s\t%d\t%s\t", (m_bFuture?1:0), sAccn, sCode, 1, sRowData);	// 신용사용
	::SendMessage(MasterHwnd, WM_ALLACCNREMAIN, (WPARAM)this->m_hWnd, (LPARAM)(LPCTSTR)Data);
}

//=================================================================================================
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

	UINT	idx = 0;
	CString tmp = m_Param.options;
	CString fieldS = OptionParser(tmp, "/edit");

	fieldS.Replace("|", "\t");
	m_bFuture = atoi(Parser(fieldS, "\t"))?true:false;
	if (tmp.Find("accnAll") != -1)
		m_bAllAccn = true;

	while (!fieldS.IsEmpty())
	{
		idx = atoi(Parser(fieldS, "\t"));
		m_arField.Add(idx);
	}
}

// bAll -> true : 잔고조회, false : 실시간 잔고
// sAccn -> !IsEmpty() : 복수계좌에서사용

void CControlWnd::SendToMap(CString sData, bool bAll, CString sAccn/* = ""*/)
{
#ifdef	SC_TEST
	m_cs.Lock();
#endif
	CString sSendData, sCodeData, sCode;
	CString keyS, dateS, sygbS, jggb;
	CStringArray sDataArr;
	int	ii = 0, idx = 0, bound = 0;
	
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
			bound = sDataArr.GetUpperBound();
			if (sDataArr.GetSize() <= 0)
				continue;
			
			sCode = sDataArr.GetAt(0);
			
			if (m_bFuture)
				keyS = getKey(sCode, "", "", "");
			else
			{
				getDate(sDataArr, dateS, sygbS, jggb);
				keyS = getKey(sCode, dateS, sygbS, jggb);
			}
						
			for (ii = 0; ii < m_arField.GetSize(); ii++)
			{
				idx = m_arField.GetAt(ii);
				if (idx > bound)
					continue;
				sSendData += sDataArr.GetAt(idx) + "\t";
			}
			m_CodeMap.SetAt(keyS, sSendData);
			sSendData += "\n";
		}
		m_flag = "A";
		m_dataList.Format("%d\t", m_CodeMap.GetCount());
		m_dataList += sSendData;
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
		idx = sData.Find("|");
		if (idx > -1)
		{
			sData.Delete(idx, sData.GetLength()-idx);
		}

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

		bound = sDataArr.GetUpperBound();
		if (sDataArr.GetSize() <= 0)
		{
#ifdef	SC_TEST
			m_cs.Unlock();
#endif
			return;
		}
		
		sCode = sDataArr.GetAt(0); 
		if (m_bFuture)
			keyS = getKey(sCode, "", "", "");
		else
		{
			if (m_flag != "D")
			{
				getDate(sDataArr, dateS, sygbS, jggb);
				keyS = getKey(sCode, dateS, sygbS, jggb);
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
				if (idx > bound)
					continue;
				sSendData += sDataArr.GetAt(idx) + "\t";
			}
			m_CodeMap.SetAt(keyS, sSendData);
		}
#ifdef	CREDIT
		if (m_bFuture)
			m_dataList = sCode + sSendData;	
		else	// 현물 : code(11) + date(8) + 신용구분(2)
		{
			m_dataList = keyS + sSendData;	
		}
#else
		m_dataList = sCode + sSendData;	
#endif
		if (m_bAllAccn)
			m_dataList = sAccn + m_dataList;
	}

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
				(LPARAM)m_Param.name.GetString());
#ifdef	SC_TEST
	m_cs.Unlock();
#endif
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char*	dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
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
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
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
	jggb = arData.GetAt(CREDIT_JGGB);
}

void CControlWnd::SendRaw(LPCTSTR sName) 
{
	// TODO: Add your dispatch handler code here
	//AfxMessageBox(sName);
	m_bLaw = true;
	m_CodeMap.RemoveAll();

	HWND MasterHwnd = m_ShMemory->GetHandle(0);
	CString	Data;
	Data.Format("%s", sName);	// 1:신용사용
	::SendMessage(MasterHwnd, WM_LAWCHEG, (WPARAM)this->m_hWnd, (LPARAM)(LPCTSTR)Data);
}

void CControlWnd::SetFilterAcc(LPCTSTR sAccList) 
{
	// TODO: Add your dispatch handler code here
	m_AccnFilter.RemoveAll();
	CString list = sAccList;
	CString acc;
	while (!list.IsEmpty())
	{
		acc = Parser(list, "\t");
		acc.TrimRight();
		m_AccnFilter.SetAt(acc,"account");
	}
	
	if (m_AccnFilter.GetCount()>0)
	{
		m_bFilter = true;
	}
	else
	{
		m_bFilter = false;
	}
	//acc.Format("%d",m_AccnFilter.GetCount());
	//AfxMessageBox(acc);	
	//m_AccnFilter.SetAt();
}

void CControlWnd::OnDataMsgChanged() 
{
	// TODO: Add notification handler code

}
