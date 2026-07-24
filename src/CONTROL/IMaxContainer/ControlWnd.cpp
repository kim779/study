// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IMAXContainer.h"
#include "ControlWnd.h"

#include "../AXIS/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Event from IMAXApi
//
#define WUM_IMAXCODECHANGE		WM_USER + 3002
#define WUM_IMAXORDERINFO		WM_USER + 3025
#define WUM_IMAXOPEN			WM_USER + 3026
#define WUM_IMAXFETCH			WM_USER + 3999

#define IMAX_INFO_CODE			0
#define IMAX_INFO_ORDERDATA		1
#define IMAX_INFO_ORDERPRICE		2
#define IMAX_INFO_BUTTONEVENT		4


//
//	Get/Set IMAXApi
//
#define WUM_IMAXDATAGET			WM_USER + 201			// ���� ���� ��������
#define WUM_IMAXDATASET			WM_USER + 202			// ���η� �ɼ� ���� �� ���
#define WUM_RECEIVECODECHANGE		WM_USER + 3003			// �����ڵ� ����� IMAXAPI�� ����

#define IMXID_SETBALANCEVALUEFIRST	20015				// First Jango Grid
#define IMXID_SETBALANCEVALUENEXT	20080				// Next Jango Grid
#define IMXID_SETOPTION			20016				// [�ڵ��޺�] �޺� Open �ʱ�ȭ �ɼ� ���� (�⺻ : 0  , 0x0800 : ETF)
#define IMXID_SETORDERMODE		20034				// ���� �ü�����
#define IMXID_SETFOCUS			20031				// [�ڵ��޺�] SetFocus

#define IMXID_SETPRICETEXT		20022

#define IMAX_BALANCEVALUEFIRST		0				// First Jango Grid
#define IMAX_BALANCEVALUENEXT		1				// Next Jango Grid
#define IMAX_SETORDERMODE		2				// �ֹ� ��� ����
#define IMAX_SETOPTION			3				// [�ڵ��޺�] �޺� Open �ʱ�ȭ �ɼ� ���� (�⺻ : 0  , 0x0800 : ETF)
#define IMAX_FOCUS			9
#define IMAX_HKCHANGECODE		12				// ȫ��
#define IMAX_SHCHANGECODE		16				// ����
#define IMAX_SZCHANGECODE		17				// ��õ
#define	IMAX_NYCHANGECODE		21				// ����
#define IMAX_NASCHANGECODE		22				// ������
#define	IMAX_AMSCHANGECODE		23				// �Ƹ߽�
#define	IMAX_OTCCHANGECODE		24				// OTC

#define IMAX_PRCSETTING			25

//
// trigger symbol name
//
#define IMAX_EXID			"IMAXEXID"
#define IMAX_CODE			"IMAXCODE"
#define IMAX_CODENAME			"IMAXCODENAME"
#define IMAX_PRICE			"IMAXPRICE"
#define IMAX_ORQTY			"IMAXORQTY"
#define IMAX_MAPNUM			"IMAXMAPNUM"
#define IMAX_ISIN			"IMAXISIN"
#define IMAX_ABLEMEDO			"IMAXABLEMEDO"
#define IMAX_ALLGRIDCULVAL			"IMAXGRIDCULVAL"

#define	IMXID_GETCODE			 11				// �����ڵ�
#define IMXID_GETCURPRICE		100				// ���簡
#define IMXID_GETBASEPRICE		101				// ���ذ�
#define IMXID_GETBUYPRICE1		201				// �ż�1ȣ��
#define IMXID_GETBUYPRICE2		202
#define IMXID_GETBUYPRICE3		203
#define IMXID_GETBUYPRICE4		204
#define IMXID_GETBUYPRICE5		205
#define IMXID_GETSELPRICE1		301				// �ŵ�1ȣ��
#define IMXID_GETSELPRICE2		302
#define IMXID_GETSELPRICE3		303
#define IMXID_GETSELPRICE4		304
#define IMXID_GETSELPRICE5		305
#define IMXID_GETBUYVOLUME		401				// �ż�����
#define IMXID_GETSELLVOLUME		501				// �ŵ�����
#define	IMXID_GETCURRENCY		601				// ��ȭ
#define IMXID_GETAUTHORITY		701				// �ü�����
#define IMXID_GETCODEINFO		20060				// ��������


//skin color
#define IMAX_COL_GRAY			1
#define IMAX_COL_BLUE			2
#define IMAX_COL_BROWN			3
#define IMAX_COL_NONE			-1

//TR key
#define TRKEY_CUSTOMAUTH_SEARCH 9

//timer
#define DF_TIMER_SEARCH_CUSTOMER_AUTH  1
#define DF_TIMER_IMAX_LONGIN  2
#define DF_TIMER_OPEN5964 3
#define DF_TIMER_TIME 100

#define P_TAB	0x09
/////////////////////////////////////////////////////////////////////////////
// CControlWnd

void CControlWnd::showMDLSbox(CString sMsg, CWnd* parent)
{
	if (m_pMDLSdlg && ::IsWindow(m_pMDLSdlg->GetSafeHwnd()))
	{
		m_pMDLSdlg->SetForegroundWindow();
		return;
	}
	m_pMDLSdlg = std::make_unique< CMDLSbox>(parent);
	m_pMDLSdlg.get()->m_text = sMsg;
	m_pMDLSdlg.get()->Create(IDD_DLG_MDLS, parent);
	m_pMDLSdlg->ShowWindow(SW_SHOW);
}

CControlWnd::CControlWnd(CWnd *pParent, struct _param *pParam)
{
	EnableAutomation();
	
	m_pMainWnd	= nullptr;
	m_pParent	= pParent;
	m_sHome		= "";
	m_sViewNo	= "";
	m_bLoinError	= FALSE;
	m_hView		= nullptr;
	m_sUsID		= "";
	m_nGroupNo	= 1;
	m_bStaf		= false;

	m_sExid		= "";
	m_sCode		= "";
	m_sCodeName	= "";
	m_sBidUnit	= "";
	m_sAskUnit	= "";
	m_sSelPrice	= "";
	m_sBidPrice	= "";
	m_sAskPrice	= "";
	m_sCurrency	= "";
	m_sOrderQty	= "";

	SetParam(pParam);
	m_iSkin = IMAX_COL_NONE;
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 999, OnMsgFormDlg)
	ON_MESSAGE(WM_USER + 201, IMAXOnMessageJano)  //201 b-> 202
	ON_MESSAGE(WUM_IMAXCODECHANGE, IMAXOnMessageCODEChange)		// WM_USER + 3002
	ON_MESSAGE(WUM_IMAXORDERINFO, IMAXOnMessageORDERINFO)		// WM_USER + 3025
	ON_MESSAGE(WUM_IMAXOPEN, IMAXOnMessageOPEN)			// WM_USER + 3026
	ON_MESSAGE(WUM_IMAXFETCH, IMAXOnMessageFETCH)			// WM_USER + 3999
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "SetIMAXGroup", _SetIMAXGroup, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CControlWnd, "SetIMAXCIFId", _SetIMAXCIFId, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetIMAXSkin", _SetIMAXSkin, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CControlWnd, "SetIMAXData", _SetIMAXData, VT_EMPTY, VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetIMAXValue", _GetIMAXValue, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "InitIMAX", _InitIMAX, VT_I2, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetVisible", _SetVisible, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "SetEnable", _SetEnable, VT_EMPTY, VTS_BOOL)
	//}}AFX_DISPATCH_MAP
	DISP_PROPERTY_EX_ID(CControlWnd, "sJango", dispidsJango, GetsJango, SetsJango, VT_BSTR)
	DISP_FUNCTION_ID(CControlWnd, "SetMapNum", dispidSetMapNum, SetMapNum, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlWnd, "GetMarketType", dispidGetMarketType, GetMarketType, VT_BSTR, VTS_NONE)
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {A59B7CBB-358C-41A3-AA9A-E22DA4D76E94}
static const IID IID_IControlWnd =
{ 0xa59b7cbb, 0x358c, 0x41a3, { 0xaa, 0x9a, 0xe2, 0x2d, 0xa4, 0xd7, 0x6e, 0x94 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

long CControlWnd::IMAXOnMessageFETCH(WPARAM wParam, LPARAM lParam)
{
	// wParam = 0 �̸� ȭ�� ����, �׿ܿ��� ����
	// lParam = IMAX ȭ���ȣ
	int nView = (int)lParam;
	int nOk = (int)wParam;

	m_slog.Format("[IMAX][FETCH][%s] nView = [%d] nOk = [%d] m_bLoinSuccess=[%d]", __FUNCTION__, nView, nOk, m_bLoinSuccess);
	OutputDebugString(m_slog);

	// 20141105 WParam 0 �̸� ȭ�� ���½���, �׿ܿ��� ����
	//if (nOk != 0 && !m_bLoinSuccess)
	if (nOk != 0)
	{
		m_bSearhAuthFinish = TRUE;
		SetTimer(DF_TIMER_IMAX_LONGIN, DF_TIMER_TIME, nullptr);  //IMAXOnMessageFETCH
	}
	return 0;
}

long CControlWnd::IMAXOnMessageOPEN(WPARAM wParam, LPARAM lParam)
{
m_tmpX.Format("IMAXOnMessageOPEN()[OPEN] viewNo[%s]msg[%s]map[%s]", m_sViewNo, (LPCTSTR)lParam, (LPCTSTR)wParam);
OutputDebugString(m_tmpX);
dprint();

	CString sIMAXMessageInfo = (LPCTSTR)lParam;
	CString	sImaxMapNum = (LPCTSTR)wParam;
	CString	stmp;
	
	if (sImaxMapNum == "3111")   //�ü���û
	{
		stmp.Format("%s /S /p5 /t0", "IB599000");
		openView(typePOPUP, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "8520")   //�������
	{
		stmp.Format("%s /S /p5 /t0", "IB567100");
		openView(typePOPUP, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "2112")   //���簡
	{
		stmp.Format("%s /t", "IB567000");
		openView(typeVIEW, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "3333")  //��Ʈ
	{
		stmp.Format("%s /S/t0", "IB586000");
		openView(typeVIEW, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "1448") //�ֹ�����
	{
		stmp.Format("%s /S/t0", "IB550000");
		openView(typeVIEW, stmp); 
		return 0;
	}
	else if (sImaxMapNum == "5964") //�̱��ü���û
	{
		m_sCustomerAuth.Empty();
		m_sCustomerNum.Empty();
		AbroadAuth( true);

		stmp.Format("%s /S/t0", "IB596400");
		openView(typeVIEW, stmp);
		m_pParent->GetParent()->PostMessage(WM_CLOSE);
		return 0;
	}
	else if (!sImaxMapNum.IsEmpty())
	{
		stmp.Format("%s\t%s", IMAX_MAPNUM, sImaxMapNum);
		IMAXToMapTrigger(stmp);
	}

	parseIMAXMessageData(IMAX_INFO_BUTTONEVENT, sIMAXMessageInfo);

	return 0;
}

//
long CControlWnd::IMAXOnMessageJano(WPARAM wParam, LPARAM lParam)
{
	
	if (m_sViewNo == "1428") //�ܰ��ΰ��4
	{
		m_slog.Format("IMAXOnMessageJano()  viewNo[%s][%s] wParam = [%d] msg[%s]", m_sViewNo, m_param.options,(int)wParam, (LPCTSTR)lParam);
		OutputDebugString(m_slog);
		m_sJango.Format("%s'", (char*)lParam);
		int icnt = atoi(Parser(m_sJango, "\t"));

		m_slog.Format("IMAXOnMessageJano()  viewNo[%s]  icnt = [%d] msg[%s]", m_sViewNo, icnt , m_sJango);
		OutputDebugString(m_slog);


		double sumBuyAmt = 0.0;		// ���Աݾ� ��
		double sumMarketAmt = 0.0;	// �򰡱ݾ�	��
		double sumProfit = 0.0;   // �򰡼��� ��
		//double sumRate = 0.0;     // ���ͷ� ��
		int count = 0;            // �ܰ� ������ ����
		CString stmp;
		CString sArr[50]{ "", };
		int idx = 0;
		//arr 7 - ���Աݾ�(��ȭ) , 15 - �򰡱ݾ� , 11 - �򰡼���, ���ͷ� = ((�򰡱ݾ� - ���Աݾ�)/���Աݾ�)*100
		while (AfxExtractSubString(stmp, m_sJango, idx, '\n'))
		{
			sArr[idx] = stmp;
			++idx; // �������� ���� ����
		}


		for (int ii = 0; ii < 50; ii++)
		{
			m_slog.Format("IMAXOnMessageJano()  viewNo[%s]  ii=[%d] sArr=[%s]",
				m_sViewNo, ii, sArr[ii]);
			if (sArr[ii].GetLength() > 1)
			{
				OutputDebugString(m_slog);
				CString colArr[20]{"", };
				stmp.Empty();
				idx = 0;
				while (idx < 20 && AfxExtractSubString(stmp, sArr[ii], idx, '\t'))
				{
					colArr[idx] = stmp;
					++idx;
				}
				
				//replace�� ó�����ϸ� �����̽����� ���� ��ȭ�鿡�� �۾��� �ѹ� ���������.
				CString sBuyAmt, sMarketAmt, sProfit, sQty, sExRate;
				sBuyAmt = colArr[7];//�ż��ݾ�(��ȭ) ��մܰ�
				sBuyAmt.Replace(",", "");
				sMarketAmt = colArr[15];//��ȭ �򰡱ݾ�
				sMarketAmt.Replace(",", "");
				sProfit = colArr[13];	//��ȭ �򰡼���
				sProfit.Replace(",", "");

				sQty = colArr[6];
				sQty.Replace(",", "");	//�ܰ�����

				sExRate = colArr[14];
				sExRate.Replace(",", "");	//����ȯ��
				double BuyAmt = _tstof(sBuyAmt); // 
				double MarketAmt =	_tstof(sMarketAmt); // CString -> double
				double profit =		_tstof(sProfit); // CString -> double
				double Qty = _tstof(sQty); // CString -> double
				double ExRate = _tstof(sExRate); // CString -> double
				//double rate = _tstof(colArr[12]);

				
				sumBuyAmt += BuyAmt* Qty * ExRate;	//��ȭ �ż��ݾ��� ���°���� ��մܰ�*�ܰ�����*ȯ���� ���
				sumMarketAmt += MarketAmt;
				sumProfit += profit;
				//sumRate += rate;
				count++;

				//m_slog.Format("IMAXOnMessageJano()  [%s] �򰡼�����=[%f] ���ͷ���=[%f]  �򰡼���=[%f]   ���ͷ�=[%f]",
				//	colArr[4], sumProfit, sumRate, profit, rate);
				//OutputDebugString(m_slog);

				
			}
			else
				break;
		}

		if (count > 0)
		{
			double avgRate = 0.0;
			if (sumBuyAmt > 0 && sumMarketAmt > 0)
				avgRate = ((sumMarketAmt - sumBuyAmt) / sumBuyAmt);// *100;

			CString result;
			CString send;
			
			result.Format("IMAXOnMessageJano  ��� �򰡼���: %.2f, ��� ���ͷ�: %.2f", sumProfit, avgRate);
			OutputDebugString(result);
			send.Format("%s%c%d|%d|%d|%.2f", "SetAvgData", P_TAB, (int)sumBuyAmt, (int)sumMarketAmt, (int)sumProfit, avgRate);	//���Աݾ�, �򰡱ݾ�, �򰡼���, ���ͷ� ��ȭ�������� ��� �Ҽ����� �ʿ���� ����� int����ȯ
			IMAXToMapTrigger(send);
		}
		else
		{
			OutputDebugString("IMAXOnMessageJano()  ������ ����");
		}

		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)m_param.name.GetString());
	}
	return 0;
}

long CControlWnd::IMAXOnMessageCODEChange(WPARAM wParam, LPARAM lParam)
{
m_tmpX.Format("IMAXOnMessageCODEChange()[CODEChange] viewNo[%s]msg[%s]", m_sViewNo, (LPCTSTR)lParam); dprint();
OutputDebugString(m_tmpX);

	CString sIMAXMessageInfo = (LPCTSTR)lParam;
	parseIMAXMessageData(IMAX_INFO_CODE, sIMAXMessageInfo);
	return 0;
}

long CControlWnd::IMAXOnMessageORDERINFO(WPARAM wParam, LPARAM lParam)
{
m_tmpX.Format("IMAXOnMessageORDERINFO()[ORDERINFO] viewNo[%s]msg[%s]", m_sViewNo, (LPCTSTR)lParam); dprint();	
OutputDebugString(m_tmpX);

	CString sIMAXMessageInfo = (LPCTSTR)lParam;

	if (sIMAXMessageInfo.Find('\t') < 0)
		parseIMAXMessageData(IMAX_INFO_ORDERPRICE, sIMAXMessageInfo);
	else
		parseIMAXMessageData(IMAX_INFO_ORDERDATA, sIMAXMessageInfo);

	return 0;
}

void CControlWnd::parseIMAXMessageData(int nKind, CString sIMAXInfo)
{
	m_slog.Format("]IMAXTRIGGER]----------------------------------------");
	OutputDebugString(m_slog);
	m_slog.Format("]IMAXTRIGGER][%d] ", nKind);
	OutputDebugString(m_slog);


	bool	bEvent = false;
	CString stmp;
	
	CString sExid, sCode, sCodeName, sSelPrice, sOrderQty, sISINCode, sablemedo;
	CString spBPrc, sgbVal, sValCnt, sRate, sevPrf, sPrfrate, sevPrc, sevWPrf, sevWPrc, scurr, sSExrate, sLoanDate, sLoanWprc, sExday;

	CString sArr[50]{"",};
	int idx = 0;
	while (AfxExtractSubString(stmp, sIMAXInfo, idx++, '\t'))
		sArr[idx - 1] = stmp;

	switch (nKind)
	{
	case IMAX_INFO_CODE:
		sExid		= sArr[0];
		sCode		= sArr[1];
		sCodeName	= sArr[2];
		m_sBidUnit	= sArr[3];
		m_sAskUnit	= sArr[4];
		sSelPrice	= sArr[5];
		m_sBidPrice	= sArr[6];
		m_sAskPrice	= sArr[7];
		m_sCurrency	= sArr[8];

		sISINCode	= sArr[14];
		sablemedo	= sArr[15];
		m_sCodeAll = sISINCode;      //20230321 �����ڵ� ��ȯ
		m_sCodeAll.TrimRight();

		if (sISINCode.IsEmpty())
		//if (!sISINCode.IsEmpty())
		{
			CString stmp;
			stmp.Format("%s\t%s", IMAX_ISIN, sISINCode);
			IMAXToMapTrigger(stmp);
			return;
		}

m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_CODE) viewNo[%s]Kind[%d]sExid[%s]sCode[%s]sCodeName[%s]m_sBidUnit[%s]m_sAskUnit[%s]m_sSelPrice[%s]m_sBidPrice[%s]m_sAskPrice[%s]m_sCurrency[%s]sASINCode[%s]", 
		m_sViewNo, nKind, sExid, sCode, sCodeName, m_sBidUnit, m_sAskUnit, m_sSelPrice, m_sBidPrice, m_sAskPrice, m_sCurrency, sISINCode); 
//dprint();
		break;

	case IMAX_INFO_ORDERDATA:
		sExid		= sArr[0];
		sCode		= sArr[1];
		sCodeName	= sArr[2];
		m_sBidUnit	= sArr[3];
		m_sAskUnit	= sArr[4];
		sSelPrice	= sArr[5];
		sOrderQty	= sArr[6];
		sOrderQty.TrimRight();
		//spBPrc ,   sgbVal,     sValCnt,     sRate,   sevPrf,      sPrfrate,     sevPrc,      sevWPrf,          sevWPrc,           scurr, sSExrate, sLoanDate, sLoanWprc;
		//��մܰ�, �ܰ�����, �ܰ�����  �����  �򰡼���  ���ͷ�       �򰡱ݾ�  ��ȭ�򰡼���  ��ȭ�򰡱ݾ�    ��ȭ ����ȯ��  ������       �����(��ȭ)
	
		spBPrc = sArr[7];  //��մܰ�
		sgbVal = sArr[8];
		sValCnt = sArr[9];
		sRate = sArr[10];
		sevPrf = sArr[11];
		sPrfrate = sArr[12];
		sevPrc = sArr[13];
		sevWPrf = sArr[14];
		sevWPrc = sArr[15];
		scurr = sArr[16];
		sSExrate = sArr[17];
		sLoanDate = sArr[18];
		sExday = sArr[19];
		sLoanWprc = sArr[20];

m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_ORDERDATA) viewNo[%s]Kind[%d]sExid[%s]sCode[%s]sCodeName[%s]m_sBidUnit[%s]m_sAskUnit[%s]sSelPrice[%s]sOrderQty[%s]", 
		m_sViewNo, nKind, sExid, sCode, sCodeName, m_sBidUnit, m_sAskUnit, sSelPrice, sOrderQty); 
//dprint();
		break;

	case IMAX_INFO_ORDERPRICE:
		sSelPrice	= sArr[0];
m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_ORDERPRICE) viewNo[%s]Kind[%d]sSelPrice[%s]", m_sViewNo, nKind, sSelPrice); 
//dprint();
		break;

	case IMAX_INFO_BUTTONEVENT:
		sExid		= sArr[0];
		sCode		= sArr[1];
		sCodeName	= sArr[2];
		m_sBidUnit	= sArr[3];
		m_sAskUnit	= sArr[4];
m_tmpX.Format("parseIMAXMessageData(IMAX_INFO_BUTTONEVENT) viewNo[%s]Kind[%d]sExid[%s]sCode[%s]sCodeName[%s]m_sBidUnit[%s]m_sAskUnit[%s]", 
		m_sViewNo, nKind, sExid, sCode, sCodeName, m_sBidUnit, m_sAskUnit); 
//dprint();
		break;

	default:break;
	}

	OutputDebugString(m_tmpX);

	if (!sablemedo.IsEmpty())
	{
		stmp.Format("%s\t%s", IMAX_ABLEMEDO, sablemedo);
		IMAXToMapTrigger(stmp);
		m_tmpX = _T("parseIMAXMessageData() ABLEMEDO_") + stmp; dprint();		
	}

	if (!sISINCode.IsEmpty())
	{
		m_sExid = sExid;
		stmp.Format("%s\t%s", IMAX_ISIN, sISINCode);
		IMAXToMapTrigger(stmp);
		m_tmpX = _T("parseIMAXMessageData() ASIN_") + stmp; dprint();		

		m_slog.Format("]IMAXTRIGGER]<%d>[%d] IMAX_ISIN = %s", __LINE__, nKind, sISINCode);
		OutputDebugString(m_slog);
	}

	if (!sExid.IsEmpty())
	{
		m_sExid = sExid;
		stmp.Format("%s\t%s", IMAX_EXID, sExid);
		if (m_sViewNo != "1427")
			IMAXToMapTrigger(stmp);
m_tmpX = _T("parseIMAXMessageData() EXID_") + stmp; dprint();		
	
	m_slog.Format("]IMAXTRIGGER]<%d>[%d] IMAX_EXID = %s", __LINE__, nKind, sExid);
	OutputDebugString(m_slog);
	}

	if (!sCode.IsEmpty())
	{
		if (sCode.CompareNoCase(m_sCode) || m_sCode.IsEmpty())
		{
			m_sCode = sCode;
			stmp.Format("%s\t%s", IMAX_CODE, sCode);
			IMAXToMapTrigger(stmp);
			bEvent = true;

			m_slog.Format("]IMAXTRIGGER]<%d>[%d] IMAX_CODE = %s", __LINE__ , nKind, sCode);
			OutputDebugString(m_slog);
		}
m_tmpX = _T("parseIMAXMessageData() CODE_") + stmp; dprint();
	}

	if (!sCodeName.IsEmpty())
	{
		if (sCodeName.CompareNoCase(m_sCodeName) || m_sCodeName.IsEmpty())
		{
			m_sCodeName = sCodeName;
			stmp.Format("%s\t%s", IMAX_CODENAME, sCodeName);
			IMAXToMapTrigger(stmp);
			bEvent = true;

			m_slog.Format("]IMAXTRIGGER]<%d>[%d] IMAX_CODENAME = %s", __LINE__, nKind, sCodeName);
			OutputDebugString(m_slog);
		}
m_tmpX = _T("parseIMAXMessageData() CODENAME_") + stmp; dprint();	
	}

	if (!sSelPrice.IsEmpty())
	{
		m_sSelPrice = sSelPrice;
		stmp.Format("%s\t%s", IMAX_PRICE, sSelPrice);
		IMAXToMapTrigger(stmp);
m_tmpX = _T("parseIMAXMessageData() PRICE_") + stmp; dprint();		
	}

	if (!sOrderQty.IsEmpty())
	{
		m_sOrderQty = sOrderQty; 
		stmp.Format("%s\t%s", IMAX_ORQTY, sOrderQty);
		IMAXToMapTrigger(stmp);
		m_tmpX = _T("parseIMAXMessageData() JQTY_") + stmp; dprint();		

		int icnt = sizeof(sArr) / sizeof(CString);
		stmp.Empty();
		CString sval;
		for (int ii = 0; ii < icnt; ii++)
		{
			sval += sArr[ii];
			sval += "|";
		}

		stmp.Format("%s\t%s", IMAX_ALLGRIDCULVAL, sval);

		OutputDebugString(stmp);

		IMAXToMapTrigger(stmp);

		stmp.Format("%s\t%s", "IMAXJQTY",  sArr[6]);
		IMAXToMapTrigger(stmp);


		m_slog.Format("]IMAXTRIGGER]<%d>[%d] IMAXJQTY code = %s ���ɼ��� =%s ", __LINE__,nKind, sCode, sArr[6]);
		OutputDebugString(m_slog);
	}

	if (bEvent)
	{
		DllEvent(nKind);
m_tmpX = _T("parseIMAXMessageData() EVENT") + stmp; dprint();
	}
}

void CControlWnd::SetParam(struct _param *pParam)
{
	CString	keys, text, tmps;

	m_param.key	= pParam->key;
	m_param.name	= CString(pParam->name, pParam->name.GetLength());
	m_param.rect	= CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts	= CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point	= pParam->point;
	m_param.style	= pParam->style;
	m_param.tRGB	= pParam->tRGB;
	m_param.pRGB	= pParam->pRGB;
	m_param.options	= CString(pParam->options, pParam->options.GetLength());
	
	tmps = _T("/ ");
	keys = _T("tk");
	
	for (int ii = 0, idx = 0, pos = 0; ii < keys.GetLength(); ii++)
	{
		tmps.SetAt(1, keys[ii]);
		idx = m_param.options.Find(tmps);
		if (idx < 0)
			continue;
		
		idx += 2;
		pos = m_param.options.Find('/', idx);
		
		text = (pos < 0) ? m_param.options.Mid(idx) : m_param.options.Mid(idx, pos-idx);
		text.TrimLeft();
		
		switch (keys[ii])
		{
		case 't':
			m_sViewNo = text;
			break;
		case 'k':
			m_iType = atoi(text);
			break;
		}
	}
m_tmpX.Format("SetParam() viewNo[%s]cx[%d]cy[%d]", m_sViewNo, m_param.rect.Width(), m_param.rect.Height()); dprint();
}

void CControlWnd::Init()
{
	m_sHome	= Variant(homeCC);
	m_bStaf = m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, prohibitCC), 0) ? true : false;
	m_sName = Variant(nameCC);


m_tmpX.Format("Init() viewNo[%s],Home[%s],User[%s]\n", m_sViewNo, m_sHome, m_sUsID); dprint();

m_sName = Variant(nameCC);

m_pMainWnd = AfxGetMainWnd();

	if (!m_pMainWnd)
	{
		int readL = 0;
		char readB[64];
		CString userD;
		userD.Format("%s\\%s\\%s\\%s.ini", m_sHome, "user", m_sName, m_sName);
		readL = GetPrivateProfileString("MODE", "main", "", readB, sizeof(readB), userD);

		if (readL <= 0)
			return;

		CString sWnd;
		sWnd.Format("%s", readB);
		int ddata = atoi(sWnd);
		HWND hWnd = (HWND)ddata;
		m_pMainWnd = CWnd::FromHandle(hWnd);
	}

	AbroadAuth(false);
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString ret;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());
	
	if ((long)dta > 1)
		ret = dta;
	
	return ret;
}

void CControlWnd::LoginIMAX()
{
	m_tmpX.Format("LoginIMAX() call"); dprint();

	const	int	nIntetNetKind = 0;

	CString sID;
	sID.Format("%s\t%s\t%s", m_sUsID, m_sCustomerAuth, m_sCustomerNum);

	LPTSTR	szLogin = sID.GetBuffer(sID.GetLength() + 1);
	HWND	hFrameWnd = this->GetSafeHwnd();

	m_bLoinError = FALSE;

//MMSG_SHARED_GUIDEMESSAGE
	m_slog.Format("[IMAX][%s]<%d> !!!!!!!!!!!!!!!!!!!!!!   sID = [%s]  m_bSearhAuthFinish=[%d]",
		__FUNCTION__, __LINE__, sID, m_bSearhAuthFinish);
	OutputDebugString(m_slog);

	//m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(MMSG_SHARED_GUIDEMESSAGE, 0),(LPARAM)(LPSTR)(LPCTSTR)m_slog);

	if (!m_IMAXApi.IMAXInit(hFrameWnd, szLogin, nIntetNetKind))
		m_bLoinError = TRUE;
	m_tmpX.Format("LoginIMAX() viewNo[%s]usid[%s]frameWnd[%d]error[%d]\n", m_sViewNo, m_sUsID, (int)hFrameWnd, m_bLoinError); dprint();
	OutputDebugString(m_tmpX);
	m_sUsID.ReleaseBuffer();
}

long CControlWnd::OnMsgFormDlg(WPARAM wParam, LPARAM lParam)
{
	int ival = (int)wParam;
	m_slog.Format("%d", ival);
	switch (ival)
	{
		case 0:
		{
			m_pMDLSdlg.reset();
			m_slog.Format("[AUTH]  MDSL return false");
			OutputDebugString(m_slog);
		}
		break;
		case 1:
		{
			m_pMDLSdlg.reset();
			m_slog.Format("[AUTH]   MDSL return false true");
			OutputDebugString(m_slog);
			SetTimer(DF_TIMER_OPEN5964, 500, nullptr);
		}
		break;
	}
	return 0;
}


long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	int key{};
	switch (LOBYTE(LOWORD(wParam)))
	{
		case DLL_OUBx:
		{
			struct	_extTHx* exth;
			exth = (struct _extTHx*)lParam;

			if (exth->key == TRKEY_CUSTOMAUTH_SEARCH)
			{
				

				char* ptest = (char*)exth->data;
				char* pBuf = (char*)exth->data;
				CString strAccount, strAccName, strecode, stremsg, strItems, stmp;

				stremsg = CString(((struct _ledgerH*)exth->data)->emsg, 99).Trim();
				strecode = CString(((struct _ledgerH*)exth->data)->ecod, 4);

				pBuf = pBuf + L_ledgerH;
				_stSACMT518* isacmt518 = (_stSACMT518*)pBuf;

				m_slog.Format("[IMAX][%s]<%d>@@@@@@   strecode = [%s] m_bSearhAuthFinish : [%d] m_sCustomerAuth : [%s] m_sCustomerNum : [%s]",
					__FUNCTION__, __LINE__, strecode,  m_bSearhAuthFinish, m_sCustomerAuth, m_sCustomerNum);
				OutputDebugString(m_slog);

			/*	if (strecode.Left(1) != "0")
				{
					m_sCustomerAuth.Empty();
					m_sCustomerNum.Empty();
					AbroadAuth(true);
					m_bSearhAuthFinish = TRUE;
				}*/
				if (strecode.Left(1) != "0")
				{ //��ȸ ����� ������ ������ �׳� ����ü�
					m_sCustomerAuth = "00000000000000000000000000000000011";
					m_sCustomerNum.Empty();
					AbroadAuth(true);
					m_bSearhAuthFinish = TRUE;
				}
				else
				{
					pBuf += L_ISACMT518;
					const	_stSACMT518_out* osacmt518 = (_stSACMT518_out*)pBuf;

					m_sCustomerNum.Format("%.20s", osacmt518->CIntNo);
					m_sCustomerNum.TrimRight();

					m_slog.Format("[IMAX][%s] ------ dll_oub CustomNo [%s] ", __FUNCTION__, m_sCustomerNum);
					OutputDebugString(m_slog);

					stmp.Format("%.1s", osacmt518->TrxStat);
					m_slog.Format("[IMAX][%s] ------ dll_oub stmp  [%s] ", __FUNCTION__, stmp);
					OutputDebugString(m_slog);

					if (stmp == "Y")
						m_sCustomerAuth = "00000000000000000000000000000000011";
					else
						m_sCustomerAuth = "00000000000000000000000000000000022";

					m_slog.Format("%s\t%s", m_sCustomerAuth, m_sCustomerNum);
					if (m_pMainWnd)
					{ 
						AbroadAuth( true);
					}
					m_bSearhAuthFinish = TRUE;
				}
			

				m_slog.Format("[IMAX][%s]<%d>!!!!!!!!!!  m_bSearhAuthFinish : [%d] m_sCustomerAuth : [%s] m_sCustomerNum : [%s]",
					__FUNCTION__, __LINE__, m_bSearhAuthFinish, m_sCustomerAuth, m_sCustomerNum);
				OutputDebugString(m_slog);
			}
		}
		break;
	}
	return 0;
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_bLoinError)
		return;

	CRect rc;
	
	GetClientRect(&rc);
	m_IMAXApi.Resize(m_hView, rc.Width(), rc.Height());
}

void CControlWnd::IMAXToMapTrigger(CString sValue)
{
m_tmpX.Format("IMAXToMapTrigger() : viewNo[%s]Trigger[%s]", m_sViewNo, sValue); dprint();

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, -1), (LPARAM)sValue.GetString());
	Variant(triggerCC, sValue);
}

void CControlWnd::DllEvent(int nKind)
{
	CString sName = m_param.name;

m_tmpX.Format("DllEvent() viewNo[%s]nKind[%d]", m_sViewNo, nKind); dprint();

	switch (nKind)
	{
	case IMAX_INFO_CODE:
	case IMAX_INFO_BUTTONEVENT:
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnChange)), (LPARAM)sName.GetString());
		break;
	case IMAX_INFO_ORDERDATA:
	case IMAX_INFO_ORDERPRICE:
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnClick)), (LPARAM)sName.GetString());
		break;
	}
}

void CControlWnd::_SetIMAXCIFId(LPCTSTR CIFId) 
{
	//m_IMAXApi.LoadImaxGate();


	m_sUsID = CString(CIFId);	

	m_slog.Format("[IMAX][%s]<%d> m_sUsID = [%s] m_bSearhAuthFinish : [%d]", __FUNCTION__, __LINE__, m_sUsID, m_bSearhAuthFinish);
	OutputDebugString(m_slog);

	SearchCustomAuth();
}

void CControlWnd::SearchCustomAuth()
{
	m_bSearhAuthFinish = FALSE;

	m_slog.Format("[IMAX][%s]<%d> m_bSearhAuthFinish : [%d] m_sCustomerAuth : [%s] m_sCustomerNum : [%s]",
		__FUNCTION__, __LINE__, m_bSearhAuthFinish, m_sCustomerAuth, m_sCustomerNum);
	OutputDebugString(m_slog);

	if (!m_sCustomerAuth.IsEmpty())
	{
		m_bSearhAuthFinish = TRUE;
		return;
	}

	//�������� ��ȸ
	CString stmp{};
	stmp = m_sUsID;
	stmp.Replace(".", "#");
	CString sUserID{};
	int ilen = stmp.GetLength();

	for (int ii = ilen - 1; ii >= 0; ii--)
		sUserID.AppendChar(stmp[ii]);

	char	datb[L_userTH + L_ledgerH + sizeof(struct _stSACMT518) + 128]{};
	memset(datb, 0x00, L_userTH + L_ledgerH + sizeof(struct _stSACMT518));
	struct	_userTH udat {};
	struct	_ledgerH ledger {};
	int	index = 0;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CopyMemory(ledger.svcd, _T("SACMT518"), sizeof(ledger.svcd));
	CopyMemory(ledger.usid, (LPCTSTR)sUserID, sUserID.GetLength());
	CopyMemory(ledger.brno, "000", 3);
	CopyMemory(ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '3';
	ledger.odrf[0] = '2';

	ISACMT518 isacamq518{};

	FillMemory(&isacamq518, L_ISACMT518, ' ');

	memcpy(isacamq518.in, "00001", 5);
	memcpy(isacamq518.TrxTp, "0", 1);
	memcpy(isacamq518.AcntNo, GetAuthAccount(), 11);

	memcpy(isacamq518.CIntTp, "1", 1);
	memcpy(isacamq518.EmpYn, "1", 1);
	memcpy(isacamq518.MktTp, "1", 1);
	memcpy(isacamq518.IndtpPtnCode, "01", 2);


	m_slog.Format("[IMAX][%s][%s] ", __FUNCTION__, m_sViewNo);
	OutputDebugString(m_slog);

	CopyMemory(udat.trc, "pibopbxq", sizeof(udat.trc));
	udat.stat = US_ENC | US_KEY;
	udat.key = m_param.key;
	datb[index++] = (BYTE)TRKEY_CUSTOMAUTH_SEARCH;
	CopyMemory(&datb[index], (char*)m_param.name.GetString(), m_param.name.GetLength());
	index += m_param.name.GetLength();
	datb[index++] = '\t';

	CopyMemory(&datb[index], (char*)&udat, L_userTH);	index += L_userTH;
	CopyMemory(&datb[index], (char*)&ledger, L_ledgerH);	index += L_ledgerH;
	CopyMemory(&datb[index], (char*)&isacamq518, L_ISACMT518);

	int iret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_ISACMT518 + L_ledgerH), (LPARAM)datb);

	m_slog.Format("[IMAX][%s] ret = [%d] ", __FUNCTION__, iret);
	OutputDebugString(m_slog);

	//����ID ��ȸ end
}

short CControlWnd::_InitIMAX() 
{
	m_tmpX.Format("InitMAX() call"); dprint();
	SetTimer(DF_TIMER_IMAX_LONGIN, DF_TIMER_TIME, nullptr);  //_InitIMAX()
	return 0;
}

bool CControlWnd::LoadIMAXView(CString ViewName, CRect rcCli)
{
	if (m_bLoinError || ViewName.IsEmpty())
		return false;
	
	if (m_iSkin > 0)
	{
		m_IMAXApi.ChangeSkin(m_iSkin);
	}
	else
	{
		if (m_sViewNo == "3111" || m_sViewNo == "8520")
		{ 
			char readB[64];
			CString userD;
			userD.Format("%s\\%s\\PALETTE.INI", m_sHome, "tab");
			int readL = GetPrivateProfileString("General", "Palette", "", readB, sizeof(readB), userD);

			if (readL <= 0)
				return false;

			CString sSkinName;
			sSkinName.Format("%s", readB);
			sSkinName.TrimRight();

			if (sSkinName == "Gray")  //gray ȸ����
				m_iSkin = 1;
			else if (sSkinName == "Blue")
				m_iSkin = 2;
			else if (sSkinName == "Brown")
				m_iSkin = 4;
			m_IMAXApi.ChangeSkin(m_iSkin);
		}
		else
			m_IMAXApi.ChangeSkin(2);
	}
	
	LPTSTR szImaxNo = ViewName.GetBuffer(ViewName.GetLength() +1);
	LPTSTR szCodeInfo = m_sCode.GetBuffer(m_sCode.GetLength() +1); 

m_tmpX.Format("LoadIMAXView() API.Opne(hwnd[%p]viewNo[%s]code[%s]width[%d]height[%d])", this->GetSafeHwnd(), szImaxNo, szCodeInfo, rcCli.Width(), rcCli.Height()); dprint();
	m_hView = m_IMAXApi.Open(this->GetSafeHwnd(), szImaxNo, szCodeInfo, rcCli.Width(), rcCli.Height()); dprint();
	if (m_hView == nullptr)
	{
m_tmpX.Format("LoadIMAXView() API.Open call error : m_hView is nullptr"); dprint();
		ViewName.ReleaseBuffer(); m_sCode.ReleaseBuffer();
		return false;
	}
m_tmpX.Format("LoadIMAXView() API.Open call OK"); dprint();
	
	m_IMAXApi.SetGroup(m_hView, m_nGroupNo);
	ViewName.ReleaseBuffer(); m_sCode.ReleaseBuffer();

	return true;
}

void CControlWnd::_SetIMAXGroup(short nGroup) 
{
m_tmpX.Format("SetIMAXGroup() viewNo[%s]group[%d]", m_sViewNo, nGroup); dprint();

	if (m_bLoinError)
		return;

	m_nGroupNo = nGroup;
	if (!m_hView)
		return;

	m_IMAXApi.SetGroup(m_hView, nGroup);
}

void CControlWnd::_SetIMAXSkin(short nSkin) 
{
	//return; 
m_tmpX.Format("SetIMAXSkin() viewNo[%s]skin[%d]", m_sViewNo, nSkin); dprint();
	
	if (m_bLoinError)
		return;

	m_iSkin = nSkin;
	m_IMAXApi.ChangeSkin(nSkin);
}

void CControlWnd::_SetIMAXData(short nKind, LPCTSTR pData) 
{
	if (m_bLoinError)
		return;

	CString	sValue = CString(pData);
	CString	sCodeFormat;
	WPARAM wParam;
	LPARAM lParam;
	
	HWND	hWnd = m_IMAXApi.GetHwnd(m_hView);

	if (!IsWindow(hWnd))
		return;

	::SendMessage(hWnd, WUM_IMAXDATASET, 0, 0);

	switch ((int)nKind)
	{
	case IMAX_BALANCEVALUEFIRST:
m_tmpX.Format("SetIMAXData(IMAX_BALANCEVALUE) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();
	::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETBALANCEVALUEFIRST, (LPARAM)sValue.GetString());
	wParam = _ttoi(sValue.SpanExcluding(_T("/")));
	lParam = (LPARAM)sValue.GetString();
	IMAXOnMessageJano(wParam, lParam);

		break;

	case IMAX_BALANCEVALUENEXT:
m_tmpX.Format("SetIMAXData(IMAX_BALANCEVALUENEXT) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();	
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETBALANCEVALUENEXT, (LPARAM)sValue.GetString());
		break;

	case IMAX_SETORDERMODE:
m_tmpX.Format("SetIMAXData(IMAX_SETORDERMODE) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, ""); dprint();		
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETORDERMODE, 0);
		break;

	case IMAX_SETOPTION:
m_tmpX.Format("SetIMAXData(IMAX_SETOPTION) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETOPTION, atoi(sValue));
		break;

	case IMAX_FOCUS:
m_tmpX.Format("SetIMAXData(IMAX_FOCUS) viewNo[%s]kind[%d]value[%s]", m_sViewNo, nKind, sValue); dprint();
		::SendMessage(hWnd, WUM_IMAXDATASET, (WPARAM)IMXID_SETFOCUS, (LPARAM)(LPCSTR)0);
		break;

	case IMAX_HKCHANGECODE:	case IMAX_SHCHANGECODE:	 case IMAX_SZCHANGECODE:
	case IMAX_NYCHANGECODE:	case IMAX_NASCHANGECODE: case IMAX_AMSCHANGECODE:
	case IMAX_OTCCHANGECODE:  //ctrl_code.SetIMAXData GetIMAXMk(smkgubn), GetIMaxMkcode(smkgubn) & vbTab & scode
m_tmpX.Format("SetIMAXData(IMAX_CHANGECODE) viewNo[%s]kind[%d]value[%s]code[%s]", m_sViewNo, nKind, sValue, m_sCode); dprint();	
		if (m_sCode.CompareNoCase(sValue) || m_sCode.IsEmpty())
		{
	//		sCodeFormat.Format("%04d\t%s", (int)nKind, sValue);
	//		::SendMessage(hWnd, WUM_RECEIVECODECHANGE, 770, (LPARAM)sCodeFormat.GetString());
			sCodeFormat.Format("%s", sValue);
			m_IMAXApi.ChangeCodeWindow(hWnd, (char *)sCodeFormat.GetString());
			m_sCode = sValue;
		}
	case IMAX_PRCSETTING:  //ȣ�����ɿ���Ʈ ������
		{
			::SendMessage(hWnd, IMXID_SETPRICETEXT, (WPARAM)0, (LPARAM)sValue.GetString());
		}
		break;
	}
}

BSTR CControlWnd::_GetIMAXValue(LPCTSTR sValueText) 
{
	CString strResult;

	if (m_bLoinError)
		return strResult.AllocSysString();

	int	nKey = 0;
	CString sText, sValue = "";
	char	szBuf[256]{};
	HWND	hWnd = m_IMAXApi.GetHwnd(m_hView);

	if (!IsWindow(hWnd))
		return strResult.AllocSysString();

	sText = CString(sValueText); sText.Trim();
	if (sText.IsEmpty()) return strResult.AllocSysString();

	if (sText == "code")		nKey = IMXID_GETCODE;
	else if (sText == "curr")	nKey = IMXID_GETCURPRICE;
	else if (sText == "base")	nKey = IMXID_GETBASEPRICE;
	else if (sText == "buy1")	nKey = IMXID_GETBUYPRICE1;
	else if (sText == "buy2")	nKey = IMXID_GETBUYPRICE2;
	else if (sText == "buy3")	nKey = IMXID_GETBUYPRICE3;
	else if (sText == "buy4")	nKey = IMXID_GETBUYPRICE4;
	else if (sText == "buy5")	nKey = IMXID_GETBUYPRICE5;
	else if (sText == "sel1")	nKey = IMXID_GETSELPRICE1;
	else if (sText == "sel2")	nKey = IMXID_GETSELPRICE2;
	else if (sText == "sel3")	nKey = IMXID_GETSELPRICE3;
	else if (sText == "sel4")	nKey = IMXID_GETSELPRICE4;
	else if (sText == "sel5")	nKey = IMXID_GETSELPRICE5;
	else if (sText == "buyq")	nKey = IMXID_GETBUYVOLUME;
	else if (sText == "selq")	nKey = IMXID_GETSELLVOLUME;
	else if (sText == "currency")	nKey = IMXID_GETCURRENCY;
	else if (sText == "auth")	nKey = IMXID_GETAUTHORITY;
	else if (sText == "codeinfo") nKey = IMXID_GETCODEINFO;
	else if (sText == "jangoinfo") nKey = 20905;

	if (nKey == IMXID_GETCODE)  //20230321 �����ڵ� ��ȯ
	{
		strResult = m_sCodeAll;
		return strResult.AllocSysString();
	}

	if (nKey == IMXID_GETCODEINFO)  //�ڵ�����
	{
		::SendMessage(hWnd, WUM_IMAXDATAGET + 1, (WPARAM)nKey, (LPARAM)szBuf);
		strResult = szBuf;
		m_slog = strResult;
		//m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(MMSG_SHARED_GUIDEMESSAGE, 0), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
		return strResult.AllocSysString();
	}

	if (nKey == 20905)  //20230321 �ܰ� ����
	{
		::SendMessage(hWnd, WUM_IMAXDATAGET, (WPARAM)nKey, (LPARAM)szBuf);
		strResult = szBuf;

		return strResult.AllocSysString();
	}

	if (nKey == IMXID_GETCURPRICE)  //20230321 �ܰ� ����
	{
		::SendMessage(hWnd, WUM_IMAXDATAGET, (WPARAM)nKey, (LPARAM)szBuf);
		strResult = szBuf;

		return strResult.AllocSysString();
	}



	::SendMessage(hWnd, WUM_IMAXDATAGET, (WPARAM)nKey, (LPARAM)szBuf);
	sValue = szBuf;

	if (!sValue.IsEmpty())
	{
		sValue.Trim();
	}

	if (sValue.IsEmpty())
	{
		if (sText == "buyq")
			sValue = m_sBidUnit;
		else if (sText == "edprc")
			sValue = m_sSelPrice;

		m_slog.Format("[IMAX][%s][%s] m_bSearhAuthFinish = [%d] sText=[%s] m_sBidUnit=[%s]",
			__FUNCTION__, m_sViewNo, m_bSearhAuthFinish, sText, m_sBidUnit);
		OutputDebugString(m_slog);

m_tmpX.Format("parseIMAXMessageData(IMAX__) sValue empty"); dprint();
dprint();
	}

	strResult = sValue;

m_tmpX.Format("parseIMAXMessageData(IMAX__) GetIMAXValue() videNo[%s]sText[%s]nKey[%d]strResult[%s]", m_sViewNo, sText, nKey, strResult); dprint();
dprint();	
	return strResult.AllocSysString();
}

void CControlWnd::OnDestroy() 
{
m_tmpX.Format("Close() viewNo[%s]hView[%p]", m_sViewNo, m_hView); dprint();

	m_IMAXApi.Close(m_hView);

	CWnd::OnDestroy();	
}

void CControlWnd::_SetVisible(BOOL bVisible) 
{
	if (bVisible)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

void CControlWnd::_SetEnable(BOOL bEnable) 
{
	EnableWindow(bEnable);
}

void CControlWnd::dprint()
{
	if (m_tmpX.IsEmpty())
		return;

	//if (m_iType == 1)
	//	m_tmpX.Empty();

	m_tmpX.Insert(0, _T("[IMAX:CON] : "));

// �ʿ�� ����Ұ�. ��ҿ� �ּ�ó��
//	OutputDebugString(m_tmpX);

	m_tmpX.Empty();
}

int CControlWnd::openView(int type, CString data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)data.GetString());
}

void CControlWnd::SendTR_Control(CString name, char* datb, int datl, char stat)
{
	char	szBuf[1024]{};
	struct _userTH udat {};
	int	idx = m_param.name.GetLength();

	CopyMemory(udat.trc, (char*)name.GetString(), name.GetLength());
	udat.key = m_param.key;
	udat.stat = stat | US_KEY;

	szBuf[0] = (int)TRKEY_CUSTOMAUTH_SEARCH;
	idx++;
	CopyMemory(&szBuf[1], (char*)m_param.name.GetString(), m_param.name.GetLength());

	szBuf[idx++] = '\t';

	CopyMemory(&szBuf[idx], &udat, L_userTH);
	idx += L_userTH;

	CopyMemory(&szBuf[idx], datb, datl);

	int iret{};
	if (m_pParent->GetSafeHwnd())
		iret = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)szBuf);
}

void CControlWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch (nIDEvent)
	{
		case DF_TIMER_OPEN5964:
		{
			KillTimer(DF_TIMER_OPEN5964);
			m_sCustomerAuth.Empty();
			m_sCustomerNum.Empty();
			AbroadAuth( true);

			CString stmp;
			stmp.Format("%s /S/t0", "IB596400");
			openView(typeVIEW, stmp);
			m_pParent->GetParent()->PostMessage(WM_CLOSE);
		}
		break;
		case DF_TIMER_IMAX_LONGIN:
		{
			m_slog.Format("[IMAX][%s][%s] m_bSearhAuthFinish = [%d]",
				__FUNCTION__, m_sViewNo, m_bSearhAuthFinish);
			OutputDebugString(m_slog);

			m_slog.Format("[IMAX][%s]<%d> m_bSearhAuthFinish : [%d]", __FUNCTION__, __LINE__, m_bSearhAuthFinish);
			OutputDebugString(m_slog);

			if (m_bSearhAuthFinish  == TRUE)
			{
				m_bSearhAuthFinish = FALSE;
				KillTimer(DF_TIMER_IMAX_LONGIN);
				m_IMAXApi.LoadImaxGate();

				LoginIMAX();

				if (m_bLoinError)
					return;

				bool bret = LoadIMAXView(m_sViewNo, m_param.rect);

				m_bLoinSuccess = TRUE;
				m_slog.Format("[IMAX][%s][%s] LoadIMAXView ret = [%d]",
					__FUNCTION__, m_sViewNo, bret);
				OutputDebugString(m_slog);

				
				
			/*	if(GetTickCount() % 2 == 0)
					m_sCustomerAuth = "00000000000000000000000000000000022";
				else
					m_sCustomerAuth = "00000000000000000000000000000000011";*/
				

				m_IMAXApi.SetAuth((LPSTR)(LPCTSTR)m_sCustomerAuth);
				m_slog.Format("[IMAX][%s][%s] SetAuth m_sCustomerAuth  [%s] m_bLoinSuccess [%d]",
					__FUNCTION__ , m_sViewNo, m_sCustomerAuth, m_bLoinSuccess);
				OutputDebugString(m_slog);


				if (bret)  //5500 ���� ���������� ������ ����ȭ���� ����
					m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_param.key, evOnDblClk)), (LPARAM)m_param.name.GetString());			
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}

CString CControlWnd::GetAuthAccount()
{
	if ((long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)
		return  "           ";

	CString strData = Variant(accountCC);
	CString strName, strAccInfo, stemp, sAccNum;
	bool bDelegation{}, bWrap{};

	int	aflg = 0;

	while (!strData.IsEmpty())
	{
		strAccInfo = Parser(strData, _T("\n"));


		sAccNum = Parser(strAccInfo, _T("\t"));
		stemp = Parser(strAccInfo, _T("\t"));
		strName = Parser(strAccInfo, _T("\t"));
		stemp = Parser(strName, _T("|"));
		stemp = Parser(strName, _T("|"));

		aflg = atoi((LPCTSTR)Parser(strName, _T("|")));
		bDelegation = (aflg & 0x01) ? TRUE : FALSE;
		bWrap = (aflg & 0x02) ? TRUE : FALSE;

		if (sAccNum.Mid(3, 2) == "00")
			continue;

		if (!(aflg & 0x01))
			continue;

		return sAccNum;

	}
	return "";
}

CString CControlWnd::Parser(CString& strSrc, CString strSub)
{
	CString strTemp(_T(""));

	if (strSrc.Find(strSub) == -1)
	{
		strTemp = strSrc;
		strSrc.Empty();
		return strTemp;
	}
	strTemp = strSrc.Left(strSrc.Find(strSub));
	strSrc = strSrc.Mid(strSrc.Find(strSub) + strSub.GetLength());
	return strTemp;
}

void CControlWnd::AbroadAuth(bool bSet, bool bfile)
{
	if (m_pMainWnd)
	{
		int readL = 0;
		char readB[64]{};
		CString sfile, stmp;
		sfile.Format("%s\\tab\\AXISENC.ini", m_sHome);
	
		m_slog.Format("[IMAX][%s] ------  m_sCustomerAuth [%s]  m_sCustomerNum [%s] m_bSearhAuthFinish [%d]", 
			__FUNCTION__, m_sCustomerAuth, m_sCustomerNum, m_bSearhAuthFinish);
		OutputDebugString(m_slog);

		if (bSet)
		{
			m_slog.Format("%s\t%s", m_sCustomerAuth, m_sCustomerNum);
	
			if (bfile)
			{
				WritePrivateProfileString("ABROAD", "auth", m_slog, sfile);
			}
			else
				m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(MMSG_SET_USER_ABORAD_AUTH, 0), (LPARAM)(LPSTR)(LPCTSTR)m_slog);
		}
		else
		{
			if (bfile)
			{
				GetPrivateProfileString("ABROAD", "auth", "", readB, sizeof(readB), sfile);
				stmp.Format("%s", readB);
				stmp.TrimRight();
			}
			else
				stmp = CString((LPCTSTR)m_pMainWnd->SendMessage(WM_USER, MMSG_GET_USER_ABORAD_AUTH));

			m_sCustomerAuth = Parser(stmp, _T("\t"));
			m_sCustomerAuth.TrimRight();
			m_sCustomerNum = stmp;

			if ((long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L) == 0)  //0 ����
			{
				m_sCustomerAuth = "00000000000000000000000000000000022";
				m_sCustomerNum = "";
			}
		}

		m_slog.Format("[AUTH]  bSet=[%d] bfile=[%d]    m_sCustomerAuth  [%s]  m_sCustomerNum [%s] ", bSet, bfile, m_sCustomerAuth, m_sCustomerNum);
		OutputDebugString(m_slog);
	}
}

//#include <chrono>
void CControlWnd::DoWork(int itype, int iSec)
{
	switch (itype)
	{
		case TYPE_WAIT:
		{
			auto fut = std::async(std::launch::async, [iSec]() {
				auto start = std::chrono::steady_clock::now();

				while (true)
				{
					std::this_thread::yield();  //cpu�纸
					auto now = std::chrono::steady_clock::now();
					if (std::chrono::duration_cast<std::chrono::seconds>(now - start).count() >= iSec)
						break;
				}
			
				});

			fut.get();
			return;
		}
		break;
	}
}

BSTR CControlWnd::GetsJango()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: ���⿡ ����ġ ó���� �ڵ带 �߰��մϴ�.
	strResult.Format("%s", m_sJango);
	return strResult.AllocSysString();
}


void CControlWnd::SetsJango(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: ���⿡ �Ӽ� ó���� �ڵ带 �߰��մϴ�.
}

#include "CMarketTimeManager.h"
void CControlWnd::SetMapNum(BSTR sMapNum)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CMarketTimeManager mgr;
	CTime serverKst = mgr.GetServerKoreanTime();

	CMarketTimeManager::US_MARKET_SESSION session =
		mgr.GetUSMarketSession(serverKst);

	m_slog.Format("[IMAXContainer][MARKETTIME][%s]<%d> session=[%d]", __FUNCTION__, __LINE__, (int)session);
	OutputDebugString(m_slog);
	m_slog.Format("[IMAXContainer][MARKETTIME][%s]<%d> session=[%s]", __FUNCTION__, __LINE__, mgr.GetUSMarketSessionName(session));
	OutputDebugString(m_slog);

	m_sViewNo.Format("%s", sMapNum);
	m_sViewNo.TrimRight();
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}


BSTR CControlWnd::GetMarketType()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.

	return strResult.AllocSysString();
}
