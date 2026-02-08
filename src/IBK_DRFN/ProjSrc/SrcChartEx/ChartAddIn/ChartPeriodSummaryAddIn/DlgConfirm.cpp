// DlgConfirm.cpp : implementation file
//

#include "stdafx.h"
#include "chartperiodsummaryaddin.h"
#include "DlgConfirm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm dialog


CDlgConfirm::CDlgConfirm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfirm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfirm)
		// NOTE: the ClassWizard will add member initialization here
	m_strName = "";
	m_nUpDown = 0;
	m_nSellBuy = 0;
	//}}AFX_DATA_INIT
}


void CDlgConfirm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirm)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfirm, CDialog)
	//{{AFX_MSG_MAP(CDlgConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirm message handlers

BOOL CDlgConfirm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString strDebug;
	strDebug.Format("#######OnInitDialog::%s###\r\n", m_strName);
	OutputDebugString(strDebug);
	// TODO: Add extra initialization here
	
	CString strRise, strFall;
	strRise.LoadString(IDS_STRING_RISE);
	strFall.LoadString(IDS_STRING_FALL);
	
	if(m_nUpDown == 1)
		// GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("상승");
		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(strRise);
	else if(m_nUpDown == 2)
		// GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("하락");
		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(strFall);
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_strName);
	
	CString strSell, strBuy;
	strSell.LoadString(IDS_STRING_SELL);
	strBuy.LoadString(IDS_STRING_BUY);
	
	if(m_nSellBuy == 1)
		// GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("매도");
		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText(strSell);
	else if(m_nSellBuy == 2)
		// GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("매수");
		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText(strBuy);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfirm::SetData(CString strName, int nUpDown, int nSellBuy)
{
	m_strName = strName;
	m_nUpDown = nUpDown;
	m_nSellBuy = nSellBuy;

	CString strDebug;
	strDebug.Format("#######SetData::%s###\r\n", m_strName);
	OutputDebugString(strDebug);
}


/////////////////////////////////////////////////////////////////////////////
// CDlgConfirmList dialog


CDlgConfirmList::CDlgConfirmList(CWnd* pParent /*=NULL*/)
: CDialog(CDlgConfirmList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgConfirmList)
	// NOTE: the ClassWizard will add member initialization here
	m_strName = "";
	m_nUpDown = 0;
	m_nSellBuy = 0;
	//}}AFX_DATA_INIT
}

CDlgConfirmList::~CDlgConfirmList()
{
	POSITION pos=m_ItemList.GetHeadPosition();
	while(pos) delete m_ItemList.GetNext(pos);
	m_ItemList.RemoveAll();
}

void CDlgConfirmList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfirmList)
	DDX_Control(pDX, IDC_LIST1, m_ctlListbox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfirmList, CDialog)
//{{AFX_MSG_MAP(CDlgConfirmList)
	ON_LBN_SELCHANGE(IDC_LIST1, OnSelchangeList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfirmList message handlers

BOOL CDlgConfirmList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CString strRise, strFall;	//m_nUpDown
	strRise.LoadString(IDS_STRING_RISE);
	strFall.LoadString(IDS_STRING_FALL);
	m_szUpDown[0] = strRise;
	m_szUpDown[1] = strFall;

	CString strSell, strBuy;	//m_nSellBuy
	strSell.LoadString(IDS_STRING_SELL);
	strBuy.LoadString(IDS_STRING_BUY);
	m_szSellBuy[0] = strSell;
	m_szSellBuy[1] = strBuy;

// 	CString strDebug;
// 	strDebug.Format("#######OnInitDialog::%s###\r\n", m_strName);
// 	OutputDebugString(strDebug);
// 	// TODO: Add extra initialization here
// 
// 	
// 	if(m_nUpDown == 1)
// 		// GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("상승");
// 		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(strRise);
// 	else if(m_nUpDown == 2)
// 		// GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("하락");
// 		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(strFall);
// 	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_strName);
// 	
// 	if(m_nSellBuy == 1)
// 		// GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("매도");
// 		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText(strSell);
// 	else if(m_nSellBuy == 2)
// 		// GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("매수");
// 		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText(strBuy);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfirmList::SetData(CString strName, int nUpDown, int nSellBuy)
{
	m_strName = strName;
	m_nUpDown = nUpDown;
	m_nSellBuy = nSellBuy;
	
	CString strDebug;
	strDebug.Format("#######SetData::%s###\r\n", m_strName);
	OutputDebugString(strDebug);
}

void CDlgConfirmList::ExSetData(CString strName, int nUpDown, int nSellBuy)
{
	CTime t = CTime::GetCurrentTime();
	int nDay = t.GetDay();
	int nHour = t.GetHour();
	int nMinute = t.GetMinute();
	int nSecond = t.GetSecond();

	CString szTime;
	szTime.Format("%02i:%02i:%02i",
		nHour,
		nMinute,
		nSecond);

	CString szData;
	szData.Format("(%s)%s  %s/%s", szTime, strName, m_szUpDown[nUpDown-1], m_szSellBuy[nSellBuy-1]);

	stCDlgConfirmList_Item* pItemData = new stCDlgConfirmList_Item;
	memcpy(pItemData->aTime, szTime, szTime.GetLength());
	pItemData->aTime[8]=0;
	pItemData->strName = strName;
	pItemData->nUpDown = nUpDown;
	pItemData->nSellBuy = nSellBuy;

	int nIndex = m_ctlListbox.AddString(szData);
	m_ctlListbox.SetItemData(nIndex, (DWORD)pItemData);

	m_ItemList.AddTail(pItemData);
	if(_DoShowIndex(nIndex)) m_ctlListbox.SetCurSel(nIndex);
}

void CDlgConfirmList::OnSelchangeList1() 
{
	int nIndex = m_ctlListbox.GetCurSel();
	_DoShowIndex(nIndex);
}

BOOL CDlgConfirmList::_DoShowIndex(int nIndex)
{
	if(nIndex<0 || m_ctlListbox.GetCount()<=nIndex) return FALSE;

	stCDlgConfirmList_Item* pItemData = (stCDlgConfirmList_Item*)m_ctlListbox.GetItemData(nIndex);
	if(pItemData)
	{
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText(pItemData->strName);
		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText(m_szUpDown[pItemData->nUpDown-1]);
		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText(m_szSellBuy[pItemData->nSellBuy-1]);
	}
	else
	{
		GetDlgItem(IDC_STATIC_NAME)->SetWindowText("");
		GetDlgItem(IDC_STATIC_TYPE)->SetWindowText("");
		GetDlgItem(IDC_STATIC_PRICE)->SetWindowText("");
	}
	return TRUE;
}
