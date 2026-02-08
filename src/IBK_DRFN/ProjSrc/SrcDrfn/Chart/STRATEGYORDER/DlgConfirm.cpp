// DlgConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "DlgConfirm.h"
#include "../chart_common/Grid/TR_drds/D49890.h"
#include "../../inc/RealUpdate.h"
#include ".\dlgconfirm.h"

#include "../Common_ST/STControllerDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	SetTextTitle(xxStr)			SetDlgItemText(IDC_SZTITLE, xxStr)
#define	SetTextAcctNo(xxStr)		SetDlgItemText(IDC_SZACCTNO, xxStr)
#define	SetTextAcctName(xxStr)		SetDlgItemText(IDC_SZACCTNAME, xxStr)
#define	SetTextCodeName(xxStr)		SetDlgItemText(IDC_SZCODE, xxStr)
#define	SetTextGubun(xxStr)			SetDlgItemText(IDC_SZGUBUN, xxStr)
#define	SetTextAmt(xxStr)			SetDlgItemText(IDC_SZAMT, xxStr)
#define	SetTextPrice(xxStr)			SetDlgItemText(IDC_SZPRICE, xxStr)

#define  CLR_MEDO					RGB(225,241,255)
#define  CLR_MESU					RGB(255,241,240)
#define QUERY_STOCK_HOGA "10051" 

CString  GetNumber( LPCSTR pData )
{
	CString szRet=_T("");
	int nLen = strlen(pData);
	char	ch;

	for(int i=0;  i<nLen ; i++) {
		ch = pData[i];
		if(ch >= '0' && ch <= '9'||ch == '.' || ch == '-' || ch== '+') 
			szRet += ch;
	}
	return szRet;
}

CString GetComma(LPCSTR pData)
{
	double ddata = atof(pData);
	if(ddata ==0)return "0";
	CString szRet=_T("");
	CString szTmp = pData;
	CString szTmp2;
	szTmp.TrimLeft();
	szTmp.TrimRight();

	szTmp = GetNumber(szTmp);
	if(szTmp.IsEmpty())	return "" ;
	TCHAR ch=' ';
	if(szTmp[0]=='+' ||szTmp[0]=='-') 
	{
		ch = szTmp[0];
		szTmp = szTmp.Mid(1);
	}
	BOOL IsFloat;
	int Index =szTmp.Find(".",0); 
	if(Index>0)
		IsFloat = TRUE;
	else
		IsFloat = FALSE;

	int	nLen;
	if(IsFloat)//소숫점 자리이면... 이렇게한다.
	{
		szTmp2 = szTmp.Mid(Index,szTmp.GetLength()-Index);
		szTmp.Delete(Index,szTmp.GetLength()- Index);

	}
	szTmp.MakeReverse();
	nLen= szTmp.GetLength();
	szRet.Empty();
	for(int i=0;   i<nLen;   i++) 
	{
		if(i!=0 && !(i%3))	szRet += ',';
		szRet += szTmp[i];
	}
	if(ch=='-') szRet += ch;
	szRet.MakeReverse();
	if(IsFloat)
		szRet+=szTmp2;
	return szRet;

}

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog


CDlgConfirm::CDlgConfirm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfirm::IDD, pParent),
	m_pITrCommManager(NULL), m_pITrComm(NULL)
{
	//{{AFX_DATA_INIT(CDlgConfirm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nResult = 0;
	m_pInData = NULL;
}


void CDlgConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirm)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SZACCTNO, m_ES_AccNum2);
	DDX_Control(pDX, IDC_STATIC_ACC, m_ST_Acc1);
	DDX_Control(pDX, IDC_STATIC_ACCNAME, m_ES_AccName);
	DDX_Control(pDX, IDC_SZACCTNAME, m_ES_AccName1);
	DDX_Control(pDX, IDC_STATIC_JM, m_ES_Jm);
	DDX_Control(pDX, IDC_SZCODE, m_ES_Jm1);
	DDX_Control(pDX, IDC_STATIC_GB, m_ES_Gb);
	DDX_Control(pDX, IDC_SZGUBUN, m_ES_Gb1);
	DDX_Control(pDX, IDC_STATIC_NUM, m_ES_Num);
	DDX_Control(pDX, IDC_SZAMT, m_ES_Num1);
	DDX_Control(pDX, IDC_STATIC_PRICE, m_ES_Price);
	DDX_Control(pDX, IDC_SZPRICE, m_ES_Price1);
	DDX_Control(pDX, IDC_STATIC_CO, m_ES_CO);
	DDX_Control(pDX, IDC_SZTITLE, m_ES_Title);
}

#define WMU_RECEIVE_TR WM_USER + 2002 

BEGIN_MESSAGE_MAP(CDlgConfirm, CDialog)
	//{{AFX_MSG_MAP(CDlgConfirm)
	ON_MESSAGE(WMU_RECEIVE_TR,     OnReceiveTR)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm message handlers
LRESULT CDlgConfirm::OnReceiveTR(WPARAM wp, LPARAM lp)
{
	//CTRResult *pResult = m_pTAupTrAgent->GetTRResult();
	//if(pResult == NULL)
	//{
	//	return 0L;
	//} 

	//CQueryInterface *pQA =  pResult->GetQueryAgent();
	//if ( pQA->GetQAIndex() == IDX_ACCTQA)
	//{
	//	m_pCbAcct->UpdateControl(NULL);
	//}


	//delete pResult;

	return 1L;
}

BOOL CDlgConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	COLORREF BkColor;
	if(m_pInData)
	{
		CString szTmp;
		if(m_pInData->nOrderType == VALSELL)
		{
			m_ES_Title.SetText(STORDER_TITLE_4);
			m_brBkColor.CreateSolidBrush(CLR_MEDO);
			BkColor = CLR_MEDO;
		}
		else if(m_pInData->nOrderType == VALBUY)
		{
			m_ES_Title.SetText(STORDER_TITLE_5);
			m_brBkColor.CreateSolidBrush(CLR_MESU);
			BkColor = CLR_MESU;
		}	
		else if(m_pInData->nOrderType == VALENTRYBUY)//매수진입
		{		
			m_ES_Title.SetText(STORDER_TITLE_0);
			m_brBkColor.CreateSolidBrush(CLR_MESU);
			BkColor = CLR_MESU;
		}
		else if(m_pInData->nOrderType == VALEXITLONG)
		{		
			m_ES_Title.SetText(STORDER_TITLE_1);
			m_brBkColor.CreateSolidBrush(CLR_MEDO);
			BkColor = CLR_MEDO;
		}
		else if(m_pInData->nOrderType == VALEXITSHORT)
		{		
			m_ES_Title.SetText(STORDER_TITLE_2);
			m_brBkColor.CreateSolidBrush(CLR_MESU);
			BkColor = CLR_MESU;
		}
		else if(m_pInData->nOrderType == VALENTRYSELL)
		{		
			m_ES_Title.SetText(STORDER_TITLE_3);
			m_brBkColor.CreateSolidBrush(CLR_MEDO);
			BkColor = CLR_MEDO;
		}
		szTmp.Format(_T("%.3s-%.2s-%s"), m_pInData->aAcctNo, m_pInData->aAcctNo + 3, m_pInData->aAcctNo + 5);
		m_ES_AccNum2.SetText(szTmp);
		m_ES_AccName1.SetText(m_pInData->aAcctName);
		m_ES_Jm1.SetText(m_pInData->aCodeName);
		//szTmp.Format("%d", m_pInData->nOrderGubun);
		if(!m_pInData->nOrderGubun)
		{
			szTmp = _T("지정가");
			m_ES_Gb1.SetText(szTmp);
			if(m_pInData->nMarket == 0)
				szTmp.Format(_T("%d"), (int)m_fOrderPrice);
			else
				szTmp.Format(_T("%.2f"), m_fOrderPrice);
			m_ES_Price1.SetText(GetComma(szTmp));
		}
		else
		{
			szTmp = _T("시장가");
			SetTextGubun(szTmp);
			m_ES_Gb1.SetText(szTmp);
		}

		szTmp = m_pInData->aOrderAmt;
		m_ES_Num1.SetText(GetComma(szTmp));
	}

	//KHD 
	m_ES_AccNum2.SetStaticColor(RGB(172,186,186),BkColor,RGB(0,0,0));
	m_ST_Acc1.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_AccName.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_AccName1.SetStaticColor(RGB(172,186,186),BkColor,RGB(0,0,0));
	m_ES_Jm.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_Jm1.SetStaticColor(RGB(172,186,186),BkColor,RGB(0,0,0));
	m_ES_Gb.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_Gb1.SetStaticColor(RGB(172,186,186),BkColor,RGB(0,0,0));
	m_ES_Num.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_Num1.SetStaticColor(RGB(172,186,186),BkColor,RGB(0,0,0));
	m_ES_Price.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_Price1.SetStaticColor(RGB(172,186,186),BkColor,RGB(0,0,0));

	m_ES_CO.SetStaticColor(BkColor,BkColor,RGB(0,0,0));
	m_ES_Title.SetStaticColor(RGB(0,0,0),BkColor,RGB(0,0,0));
	//END

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfirm::ApplyHoga()
{
	if(m_pInData)
	{
		SetTextPrice(GetPrice(m_pInData->nOrderPriceTypeEnt));
	}
}

LPCSTR CDlgConfirm::GetPrice(int nPriceType)
{
	if(nPriceType==0)	return (LPCSTR)m_Current; // 현재가
	if(nPriceType>=-5 && nPriceType<=-1)	return (LPCSTR)m_SellHoga[abs(nPriceType)];
	if(nPriceType>=1  && nPriceType<=5)		return (LPCSTR)m_BuyHoga[nPriceType];
	if(nPriceType==99) 			return (LPCSTR)m_UHoga;
	if(nPriceType==-99) 		return (LPCSTR)m_LHoga;

	return "0";
}

BOOL CDlgConfirm::InitTrComm()
{
	if(m_pITrCommManager && m_pITrComm)
		m_pITrCommManager->UnAdviseTrComm(m_pITrComm);

	m_pITrCommManager = (IAUTrCommManager*)AfxGetPctrInterface(UUID_IAUTrCommManager);
	m_pITrComm = m_pITrCommManager->AdviseTrComm();
	m_pITrComm->ChangeCallback(&m_xMyITrCommSite);

	return TRUE;
}

HRESULT CDlgConfirm::ReceiveData(long dwKey, LPCSTR szTR, LPCSTR szMsgCode, LPCSTR szMessage, LPVOID	aTRData, long dwTRDateLen)
{
	ConvertHogaData(szTR, aTRData, dwTRDateLen);
	ApplyHoga();

	return S_OK;
}

HRESULT CDlgConfirm::ReceiveRealData(WORD wFID, LPCSTR szMainKey, void* pRealData)
{
	return S_OK;
}

void CDlgConfirm::QueryPrice(CString strCode, int nOrderPriceType)
{
	WORD wFID[] = {15001, 14531, 14532, 14533, 14501, 14502, 14503, 16117, 16118};

	m_pITrComm->SetTR(QUERY_STOCK_HOGA);
	m_pITrComm->AddMainKey(DI_CODE, strCode, C_DT_STRING, 6);

	m_pITrComm->AddDataKey(wFID[nOrderPriceType - 1]);
	m_pITrComm->Send2Server("", 0, FALSE);
}

int CDlgConfirm::Send2Server(LPCTSTR lpData,int nSize,BOOL bWait)
{
	D49890_I* p49890 = (D49890_I*)lpData;
	CString sCode;
	sCode.Format("%6.6s", p49890->shcode);

	m_pITrComm->SetTR(QUERY_STOCK_HOGA);
	m_pITrComm->AddMainKey(16013, sCode, C_DT_STRING, 6);
	m_pITrComm->SetRealUpdateType(2);			//  RT_UPDATE	2	// 업데이트 타입
	AddDataKey_Hoga();
	m_pITrComm->Send2Server(lpData, nSize, bWait);	

	return 0;
}

//003 [[ 호가TR_AddDataKey	
void CDlgConfirm::AddDataKey_Hoga()
{
	m_pITrComm->AddDataKey(15007);	//기준가
	m_pITrComm->AddDataKey(15176);	//예상체결가격
	m_pITrComm->AddDataKey(15309);	//예상체결수량	//-> 예상거래량으로 임시받음

	m_pITrComm->AddDataKey(DI_NAME);	//종목명
	m_pITrComm->AddDataKey(DI_PRICE);	//현재가
	m_pITrComm->AddDataKey(DI_SIGN);	//등락부호
	m_pITrComm->AddDataKey(15004);	//등락률
	m_pITrComm->AddDataKey(15472);	//대비율
	m_pITrComm->AddDataKey(DI_VOLUME);	//누적거래량	//-> 거래량으로 임시받음
	m_pITrComm->AddDataKey(DI_AMOUNT);	//누적거래대금	//-> 거래대금으로 임시받음
	//m_pITrComm->AddDataKey(14659);	//호가잔량기준시간

	// 매도 8 + 40 + 
	int nIdx;
	int nDataKey;

	nDataKey = DI_OFFER_HO1;		// 매도호가1
	for(nIdx=0; nIdx < 5; nIdx++)
	{
		m_pITrComm->AddDataKey(nDataKey+nIdx);
	}

	nDataKey = DI_OFFER_REM1;		// 매도호가잔량1
	for(nIdx=0; nIdx < 5; nIdx++)
	{
		m_pITrComm->AddDataKey(nDataKey+nIdx);		
	}

	nDataKey = DI_BID_HO1;		// 매수호가1
	for(nIdx=0; nIdx < 5; nIdx++)
	{
		m_pITrComm->AddDataKey(nDataKey+nIdx);		
	}

	nDataKey = DI_BID_REM1;		// 매수호가잔량1
	for(nIdx=0; nIdx < 5; nIdx++)
	{
		m_pITrComm->AddDataKey(nDataKey+nIdx);		
	}

	m_pITrComm->AddDataKey(DI_OFFER_TOTREM);	//총매도잔량
	m_pITrComm->AddDataKey(DI_BID_TOTREM);	//총매수잔량
	m_pITrComm->AddDataKey(DI_OFFER_OUT_TOT);	//시간외매도잔량
	m_pITrComm->AddDataKey(DI_BID_OUT_TOT);	//시간외 매수잔량

	m_pITrComm->AddDataKey(16117);	//상한가
	m_pITrComm->AddDataKey(16118);	//하한가
	m_pITrComm->AddDataKey(15009);	//시가
	m_pITrComm->AddDataKey(15010);	//고가
	m_pITrComm->AddDataKey(15011);	//저가

	m_pITrComm->AddDataKey(3007);		//전일거래량  //-> 전일거래량_스프레드제외 데이터로 임시사용
	m_pITrComm->AddDataKey(16143);	//상장kospi발행kosdaq 주식수
}
//003 ]]

void SetByteOrderRevDouble(LPSTR pDouble)
{
	int nCount, nLength = 4;

	for (nCount = 0; nCount < nLength; nCount++)
	{
		unsigned char chTemp;
		chTemp = *(pDouble + nCount);
		*(pDouble + nCount) = *(pDouble + sizeof(double) - 1 - nCount);
		*(pDouble + sizeof(double) - 1 - nCount) = chTemp;
	}
}

void CDlgConfirm::ConvertHogaData(LPCSTR szTR, LPVOID	aTRData, long dwTRDateLen)
{
	int nIdx = 11 + 2*41;

	CString strCode, strOut, strData;

	//메인키
	strCode = m_pITrComm->ApplyByteOrder(C_DT_STRING, (char *)aTRData+nIdx, 6, &nIdx);
	nIdx += 6;

	//기준가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//예상체결가격
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//예상체결수량 -> 예상거래량으로 임시받음
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 12, &nIdx);
	strOut += strData;

	//종목명
	strData = m_pITrComm->ApplyByteOrder(C_DT_STRING, (char *)aTRData+nIdx, 20, &nIdx);
	strOut += strData;
	nIdx += 32;

	//현재가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;
	m_Current = strData;

	//등락부호
	strData = m_pITrComm->ApplyByteOrder(C_DT_BYTE, (char *)aTRData+nIdx, 1, &nIdx);
	strOut += strData;

	//등락폭
	strData = m_pITrComm->ApplyByteOrder(C_DT_LONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//대비율
	strData = m_pITrComm->ApplyByteOrder(C_DT_LONG, (char *)aTRData+nIdx, 10, &nIdx);
	strOut += strData;

	//누적거래량		//->임시데이터를 받아서처리
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 10, &nIdx);
	strOut += strData;

	//누적거래대금		//->임시데이터를 받아서처리
	strData = m_pITrComm->ApplyByteOrder(C_DT_LONGLONG, (char *)aTRData+nIdx, 12, &nIdx);
	strOut += strData;

	//호가잔량기준시간
	//strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 6, &nIdx);
	//strOut += strData;


	// 1-10 매도호가/잔량, 1-10 매수호가/잔량, 총매도/매수잔량, 시간외매도/매수잔량, 상/하한가
	for(int i=0; i< 5; i++)
	{
		// 매도호가
		strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
		strOut += strData;
		m_SellHoga[i] = strData;
	}
	for(int i=0; i< 5; i++)
	{
		//매도잔량
		strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
		strOut += strData;
	}
	for(int i=0; i< 5; i++)
	{
		//매수호가
		strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
		strOut += strData;
		m_BuyHoga[i] = strData;
	}
	for(int i=0; i< 5; i++)
	{
		//매수잔량
		strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
		strOut += strData;
	}

	//총매도잔량
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//총매수잔량
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//시간외매도잔량
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//시간외 매수잔량
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//상한가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;
	m_UHoga = strData;

	//하한가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;
	m_LHoga = strData;

	//시가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;
	//고가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;
	//저가
	strData = m_pITrComm->ApplyByteOrder(C_DT_ULONG, (char *)aTRData+nIdx, 9, &nIdx);
	strOut += strData;

	//전일거래량
	strData = m_pITrComm->ApplyByteOrder(C_DT_LONG, (char *)aTRData+nIdx, 10, &nIdx);
	strOut += strData;

	//상장kospi발행kosdaq 주식수
	strData = m_pITrComm->ApplyByteOrder(C_DT_LONGLONG, (char *)aTRData+nIdx, 10, &nIdx);
	strOut += strData;

	//m_strOut = strCode +strOut;
}

// END

HBRUSH CDlgConfirm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.

	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return m_brBkColor;
}
