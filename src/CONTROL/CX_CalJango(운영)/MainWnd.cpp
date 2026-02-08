// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_CalJango.h"
#include "MainWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	LEN_ACODE	12		// 주식 전체코드 길이
#define	LEN_SYGB	2		// 신용구분 길이
#define	LEN_DATE	8		// 날짜
#define	LEN_SYMT	15		// 신용대출금액 길이
#define LEN_JGGB	2
/////////////////////////////////////////////////////////////////////////////
// CMainWnd

CMainWnd::CMainWnd(CWnd* parent)
{
	EnableAutomation();

	m_pParent = parent;
	m_nMaip = 0;
	m_nPprc = 0;
	m_nSuik = 0;
	m_dRate = 0;

	m_CodeMap.RemoveAll();
}

CMainWnd::~CMainWnd()
{
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
	DISP_PROPERTY_NOTIFY(CMainWnd, "DataList", m_dataList, OnDataListChanged, VT_BSTR)
	DISP_FUNCTION(CMainWnd, "SetJango", SetJango, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SetUpdate", SetUpdate, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SetJango2", SetJango2, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CMainWnd, "SetUpdate2", SetUpdate2, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {917CD0FD-8049-4155-B45F-0A71F7E867C8}
static const IID IID_IMainWnd =
{ 0x917cd0fd, 0x8049, 0x4155, { 0xb4, 0x5f, 0xa, 0x71, 0xf7, 0xe8, 0x67, 0xc8 } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

long CMainWnd::OnMessage(WPARAM wParam,LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		break;
	case DLL_OUBx:
	case DLL_OUB:
		break;
	case DLL_ALERTx:
	case DLL_ALERT:
		break;
	case DLL_TRIGGER:	// Remain Data Send to Map!
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		break;
	case DLL_NOTICE:
		break;
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers

int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CMainWnd::SetJango(LPCTSTR sdata) 
{
	LPCSTR	data = (LPCSTR)sdata;
	int	row_cnt = 0;
	int	dlen = 0;
	vector<StringProxy> rows, cols;
	vector<StringProxy>::iterator st, ed;
	CString sCode, sAllCode, keyS, sygbS, dateS, jggbS;
	CString sKey, tmps;
	std::shared_ptr<Jango> jango = nullptr;

	for (POSITION pos = m_CodeMap.GetStartPosition(); pos; )
	{
		m_CodeMap.GetNextAssoc(pos, sKey, jango);
		jango.reset();
	}
	m_CodeMap.RemoveAll();


	dlen = strlen(data);
	LPCSTR pos = find(data, data+dlen, '\t');
	row_cnt = str2int(data, pos-data);						// 처음 4자리는 Row 갯수
	parseString(pos+1, data+dlen, '\n', rows);					// 4자리 이후로 \n \t 구분자로 2중 Array
	
	if (row_cnt != rows.size())
	{
		TRACE("Unknown NotifyControl? (%d != %d)", row_cnt, rows.size());
		return;
	}

	//		0:code		1:name		2:결제잔고	3:주문잔량
	//		4:매입평균가	5:매도가능잔고	6:현재가	7:평가금액
	//		8:평가손익	9:평가수익률	10:매입금액     
	// 신용추가	11:대출일	12:신용구분	13:신용구분내용	14:신용구분약자	15:신용대출금액
	//		16:잔고구분	17.BEP단가	18.수수료
	// JANGO FILED [00 01 16 02 05 04 17 06 19 10 11 15 07 08 09 12 11 13 15 ]

	for (st=rows.begin(), ed=rows.end(); st!=ed; ++st)
	{
		parseString(*st, '\t', cols);
		if (cols.size() != 19)
			continue;
		
		jango = std::make_shared<Jango>();
		jango->flag = 'I';
		CopyMemory(jango->acno, m_acno, m_acno.GetLength());
		tmps = cols[0].ToCStr();
		CopyMemory(jango->cod2, tmps, tmps.GetLength());
		tmps = cols[1].ToCStr();
		CopyMemory(jango->hnam, tmps, tmps.GetLength());
		tmps = cols[2].ToCStr();
		CopyMemory(jango->jggb, tmps, tmps.GetLength());
		jango->jqty = str2int(cols[3]);
		jango->xqty = str2int(cols[4]);
		jango->pamt = str2double(cols[5]);	// 가격은 정수로 관리(*100)
		jango->mamt = str2double(cols[9]);
		jango->camt = str2double(cols[12]);
		jango->tamt = str2double(cols[13]);
		jango->srat = str2double(cols[14]);
		tmps = cols[15].ToCStr();
		CopyMemory(jango->sycd, tmps, tmps.GetLength());
		tmps = cols[10].ToCStr();
		CopyMemory(jango->sydt, tmps, tmps.GetLength());
		
		sCode = CString(jango->cod2, sizeof(jango->cod2));	// code
		sCode.TrimRight();
		dateS = CString(jango->sydt, sizeof(jango->sydt));	// 대출일
		dateS.TrimRight();
		sygbS = CString(jango->sycd, sizeof(jango->sycd));	// 신용구분
		sygbS.TrimRight();
		jggbS = CString(jango->jggb, sizeof(jango->jggb));
		jggbS.TrimRight();

		sKey = getKey(false, sCode, dateS, sygbS, jggbS, 10);
		m_CodeMap.SetAt(sKey, jango);
	}

	Calculate();
}

void CMainWnd::SetUpdate(LPCTSTR flag,LPCTSTR data) 
{
	int	dlen = strlen(data);

	if (flag[0] == 'U' || flag[0] == 'I')
	{
		data = data+24;
		dlen = strlen(data);

		CString sKey, tmps;
		CString sCode, sAllCode, keyS, sygbS, dateS, jggbS;
		vector<StringProxy> cols;
		std::shared_ptr<Jango> jango = std::make_shared<Jango>();

		parseString(StringProxy(data, data+dlen) , '\t', cols);		// 종목코드가 있다. 8byte!
		
		if (cols.size() != 19)
			return;
		
		jango->flag = flag[0];
		CopyMemory(jango->acno, m_acno, m_acno.GetLength());
		tmps = cols[0].ToCStr();
		CopyMemory(jango->cod2, tmps, tmps.GetLength());
		tmps = cols[1].ToCStr();
		CopyMemory(jango->hnam, tmps, tmps.GetLength());
		tmps = cols[2].ToCStr();
		CopyMemory(jango->jggb, tmps, tmps.GetLength());
		jango->jqty = str2int(cols[3]);
		jango->xqty = str2int(cols[4]);
		jango->pamt = str2double(cols[5]);	// 가격은 정수로 관리(*100)
		jango->mamt = str2double(cols[9]);
		jango->camt = str2double(cols[12]);
		jango->tamt = str2double(cols[13]);
		jango->srat = str2double(cols[14]);
		tmps = cols[15].ToCStr();
		CopyMemory(jango->sycd, tmps, tmps.GetLength());
		tmps = cols[10].ToCStr();
		CopyMemory(jango->sydt, tmps, tmps.GetLength());
		
		sCode = CString(jango->cod2, sizeof(jango->cod2));	// code
		sCode.TrimRight();
		dateS = CString(jango->sydt, sizeof(jango->sydt));	// 대출일
		dateS.TrimRight();
		sygbS = CString(jango->sycd, sizeof(jango->sycd));	// 신용구분
		sygbS.TrimRight();
		jggbS = CString(jango->jggb, sizeof(jango->jggb));
		jggbS.TrimRight();

		sKey = getKey(false, sCode, dateS, sygbS, jggbS, 10);

		std::shared_ptr<Jango> tj = nullptr;
		if (m_CodeMap.Lookup(sKey, tj))
		{
			tj.reset();
			m_CodeMap.RemoveKey(sKey);
		}
		m_CodeMap.SetAt(sKey, jango);
// 		
// 		WriteLog("[%c] 종목[%s] 구분[%s], 잔고[%d] 청산[%d], 평단[%d], 평가금액[%.f] 평가손익[%.f] 수익률[%.2f], 매입금액[%.f]",
// 			*flag, j.code, j.gubn, j.jqty, j.xqty, j.pprc, j.camt, j.tamt, j.srat, j.mamt);
	}
	else if (flag[0] == 'D')
	{
// 		jGubn = Trim(Mid(skey, 1, 2))		'잔고구분
// 			sCode = Trim(Mid(sKey, 3, 12))	'코드
// 			sDate = Trim(Mid(sKey, 15, 8))	'대출일
// 		sGubn = Trim(Mid(sKey, 23, 2))	'신용구분
// 		vector<StringProxy> cols;
// 		parseString(StringProxy(data, data+dlen) , '\t', cols);		// 종목코드가 있다. 8byte!
// 		
// 		if (cols.size()!=19) return;
// 
		CString strData;
		strData.Format("%s",data);

		CString sCode, sAllCode, keyS, sygbS, dateS, jggbS;
		CString sKey;

		sCode = strData.Mid(2,12);	// code
		sCode.TrimRight();
		dateS = strData.Mid(14,8);	// 대출일
		dateS.TrimRight();
		sygbS = strData.Mid(22,2);	// 신용구분
		sygbS.TrimRight();
		jggbS = strData.Mid(0,2);
		jggbS.TrimRight();

		sKey = getKey(false, sCode, dateS, sygbS, jggbS, 10);
		
		std::shared_ptr<Jango> tj = nullptr;
		if (m_CodeMap.Lookup(sKey, tj))
		{
			tj.reset();
			m_CodeMap.RemoveKey(sKey);
		}
	}
	Calculate();
}

void CMainWnd::parsingAlertx(LPARAM lParam)
{
// 	struct _alertR* alertR = (struct _alertR*)lParam;
// 	CString sCode = alertR->code;
// 	DWORD*	data = nullptr;
// 	
// 	for (int ii = alertR->size-1; ii >= 0; ii--)
// 	{
// 		data = (DWORD*)alertR->ptr[ii];
// 	}
}

void CMainWnd::Calculate()
{
	CString keyS;
	CString keyCodeS;
	std::shared_ptr<Jango>	jango = nullptr;
	double	nMaip = 0;
	double	nPprc = 0;
	double	npsuik = 0.0;

	for (POSITION codePos = m_CodeMap.GetStartPosition(); codePos != NULL;)
	{
		m_CodeMap.GetNextAssoc(codePos, keyS, jango);
		keyCodeS = keyS.Mid(2, LEN_ACODE);
		keyCodeS.TrimRight();

		nMaip  += jango->mamt;   //유통대주
		npsuik += jango->tamt;  //평가손익
		nPprc  += jango->camt;   //평가금액
	}

	if (nMaip < 1)
	{
		m_nMaip = 0;
		m_nPprc = 0;
		m_nSuik = 0;
		m_dRate = 0.0;
	}
	else
	{
		m_nMaip = nMaip;
		m_nPprc = nPprc;
		m_nSuik = npsuik;   //유통대주
		m_dRate = (m_nSuik / m_nMaip) * 100;
	}

	m_dataList.Format("%.f\t%.f\t%.f\t%.2f",m_nMaip,m_nPprc,m_nSuik,m_dRate);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
				(LPARAM)m_Param.name.GetString());
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick/*DblClick*/)),
				(LPARAM)m_Param.name.GetString());
}

CString	CMainWnd::getKey(bool bFuture, CString codeS, CString dateS, CString sygbS, CString sJggb, int trkey)
{
	if (bFuture)
		return codeS;
	/*
	if (atoi(sygbS) >= 10 || atoi(sygbS) == 0)	// 신용구분 아니면 0
		sygbS = "  ";
	*/
	CString keyS;
	keyS.Format("%-*s%-*s%-*s%-*s|%d", LEN_JGGB, sJggb, LEN_ACODE, codeS, LEN_DATE, dateS, LEN_SYGB, sygbS, trkey);
	
	return keyS;
}

void CMainWnd::SetParam(struct _param *pParam)
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
}

void CMainWnd::OnDataListChanged() 
{
	// TODO: Add notification handler code

}

CString CMainWnd::Parser(CString &srcstr, CString substr)
{
	CString temp;

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
	
	temp.Trim();
	return temp;
}

void CMainWnd::SetJango2(LPCTSTR sdata) 
{
	// TODO: Add your dispatch handler code here
	LPCSTR data = (LPCSTR)sdata;
	vector<StringProxy> rows, cols;
	vector<StringProxy>::iterator st, ed;
	CString sCode, sAllCode, keyS, sygbS, dateS, jggbS;
	CString sKey, tmps;
	std::shared_ptr<Jango> jango = nullptr;

	int	row_cnt = 0;
	const	int	dlen = strlen(data);

	for (POSITION pos = m_CodeMap.GetStartPosition(); pos; )
	{
		m_CodeMap.GetNextAssoc(pos, sKey, jango);
		jango.reset();
	}
	m_CodeMap.RemoveAll();

	LPCSTR pos = find(data, data+dlen, '\t');
	row_cnt = str2int(data, pos-data);			// 처음 4자리는 Row 갯수
	parseString(pos+1, data+dlen, '\n', rows);		// 4자리 이후로 \n \t 구분자로 2중 Array
	
	if (row_cnt!=rows.size())
	{
		TRACE("Unknown NotifyControl? (%d != %d)", row_cnt, rows.size());
		return;
	}
	
	//		0:code		1:name		2:결제잔고	3:주문잔량
	//		4:매입평균가	5:매도가능잔고	6:현재가	7:평가금액
	//		8:평가손익	9:평가수익률	10:매입금액
	// 신용추가	11:대출일	12:신용구분	13:신용구분내용	14:신용구분약자	15:신용대출금액
	//		16:잔고구분	17.BEP단가	18.수수료
	// JANGO FILED [00 01 16 02 05 04 17 06 19 10 11 15 07 08 09 12 11 13 15 ]
	// JANGO FILED [00 01 16 02 05 04 17 06 10 07 08 09 12 11 15 ]
	//              00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15 16 17 18

	for (st=rows.begin(), ed=rows.end(); st!=ed; ++st)
	{
		parseString(*st, '\t', cols);
		if (cols.size() != 15)
			continue;
		
		jango = std::make_shared<Jango>();
		jango->flag = 'I';
		CopyMemory(jango->acno, m_acno, m_acno.GetLength());
		tmps = cols[0].ToCStr();
		CopyMemory(jango->cod2, tmps, tmps.GetLength());
		tmps = cols[1].ToCStr();
		CopyMemory(jango->hnam, tmps, tmps.GetLength());
		tmps = cols[2].ToCStr();
		CopyMemory(jango->jggb, tmps, tmps.GetLength());
		jango->jqty = str2int(cols[3]);
		jango->xqty = str2int(cols[4]);
		jango->pamt = str2double(cols[5]);	// 가격은 정수로 관리(*100)
		jango->mamt = str2double(cols[8]);
		jango->camt = str2double(cols[9]);
		jango->tamt = str2double(cols[10]);
		jango->srat = str2double(cols[11]);
		tmps = cols[12].ToCStr();
		CopyMemory(jango->sycd, tmps, tmps.GetLength());
		tmps = cols[13].ToCStr();
		CopyMemory(jango->sydt, tmps, tmps.GetLength());
		
		sCode = CString(jango->cod2, sizeof(jango->cod2));	// code
		sCode.TrimRight();
		dateS = CString(jango->sydt, sizeof(jango->sydt));	// 대출일
		dateS.TrimRight();
		sygbS = CString(jango->sycd, sizeof(jango->sycd));	// 신용구분
		sygbS.TrimRight();
		jggbS = CString(jango->jggb, sizeof(jango->jggb));
		jggbS.TrimRight();
		
		sKey = getKey(false, sCode, dateS, sygbS, jggbS, 10);
		
		m_CodeMap.SetAt(sKey, jango);
	}
	
	Calculate();
}

void CMainWnd::SetUpdate2(LPCTSTR flag,LPCTSTR data) 
{
	CString strData;
	CString sCode, sAllCode, keyS, sygbS, dateS, jggbS;
	CString sKey, tmps;

	if (flag[0] == 'U' || flag[0] == 'I')
	{
		vector<StringProxy> cols;
		int	dlen = strlen(data);

		data = data+24;
		dlen = strlen(data);

		parseString(StringProxy(data, data+dlen) , '\t', cols);		// 종목코드가 있다. 8byte!
		
		if (cols.size() != 15)
			return;
		
		std::shared_ptr<Jango> jango = std::make_shared<Jango>();
		jango->flag = flag[0];
		CopyMemory(jango->acno, m_acno, m_acno.GetLength());
		tmps = cols[0].ToCStr();
		CopyMemory(jango->cod2, tmps, tmps.GetLength());
		tmps = cols[1].ToCStr();
		CopyMemory(jango->hnam, tmps, tmps.GetLength());
		tmps = cols[2].ToCStr();
		CopyMemory(jango->jggb, tmps, tmps.GetLength());
		jango->jqty = str2int(cols[3]);
		jango->xqty = str2int(cols[4]);
		jango->pamt = str2double(cols[5]);	// 가격은 정수로 관리(*100)
		jango->mamt = str2double(cols[8]);
		jango->camt = str2double(cols[9]);
		jango->tamt = str2double(cols[10]);
		jango->srat = str2double(cols[11]);
		tmps = cols[12].ToCStr();
		CopyMemory(jango->sycd, tmps, tmps.GetLength());
		tmps = cols[13].ToCStr();
		CopyMemory(jango->sydt, tmps, tmps.GetLength());
		
		sCode = CString(jango->cod2, sizeof(jango->cod2));	// code
		sCode.TrimRight();
		dateS = CString(jango->sydt, sizeof(jango->sydt));	// 대출일
		dateS.TrimRight();
		sygbS = CString(jango->sycd, sizeof(jango->sycd));	// 신용구분
		sygbS.TrimRight();
		jggbS = CString(jango->jggb, sizeof(jango->jggb));
		jggbS.TrimRight();
		
		sKey = getKey(false, sCode, dateS, sygbS, jggbS, 10);
		std::shared_ptr<Jango> tj = nullptr;
		if (m_CodeMap.Lookup(sKey,tj))
		{
			tj.reset();
			m_CodeMap.RemoveKey(sKey);
		}
		m_CodeMap.SetAt(sKey, jango);
// 		
// 		WriteLog("[%c] 종목[%s] 구분[%s], 잔고[%d] 청산[%d], 평단[%d], 평가금액[%.f] 평가손익[%.f] 수익률[%.2f], 매입금액[%.f]",
// 			*flag, j.code, j.gubn, j.jqty, j.xqty, j.pprc, j.camt, j.tamt, j.srat, j.mamt);
	}
	else if (flag[0] == 'D')
	{
		strData.Format("%s",data);
		sCode = strData.Mid(2,12);	// code
		sCode.TrimRight();
		dateS = strData.Mid(14,8);	// 대출일
		dateS.TrimRight();
		sygbS = strData.Mid(22,2);	// 신용구분
		sygbS.TrimRight();
		jggbS = strData.Mid(0,2);
		jggbS.TrimRight();
		
		sKey = getKey(false, sCode, dateS, sygbS, jggbS, 10);
		
		m_CodeMap.RemoveKey(sKey);
	}
	Calculate();
}

double CMainWnd::str2double(LPCSTR data, int dlen)
{
	char	buff[32]{};

	CopyMemory(buff, data, dlen);
	return (buff[1] == '-' || buff[1] == '+' || buff[1] == ' ') ? atof(&buff[1]) : atof(&buff[0]);
}

int CMainWnd::str2int(LPCSTR data, int dlen)
{
	char	buff[32]{};

	CopyMemory(buff, data, dlen);
	return atoi(buff);
}

void CMainWnd::parseString(LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy>& retval)
{
	LPCSTR	pos = nullptr;

	retval.clear();
	while (st < ed)
	{
		pos = find(st, ed, del);
		retval.push_back(StringProxy(st, pos));
		st = pos + 1;
	}
}

void CMainWnd::parseString(const StringProxy& sp, CHAR del, vector<StringProxy>& retval)
{
	parseString(sp.first, sp.second, del, retval);
}

