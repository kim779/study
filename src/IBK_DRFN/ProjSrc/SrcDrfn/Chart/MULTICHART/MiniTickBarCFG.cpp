// MiniTickBarCFG.cpp : implementation file
//

#include "stdafx.h"
#include "multichart.h"
#include "MiniTickBarCFG.h"
#include "MiniTickBarDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMiniTickBarCFG dialog


CMiniTickBarCFG::CMiniTickBarCFG(CSetTickButton* pTickButton, CWnd* pParent /*=NULL*/)
	: CDialog(CMiniTickBarCFG::IDD, pParent),
	m_pTickButton(pTickButton)
{
	//{{AFX_DATA_INIT(CMiniTickBarCFG)
	//}}AFX_DATA_INIT
	for(int i = 0; i < 9; i++)
	{
		m_strBtn[i] = _T("");
		m_nMarket[i] = 0;
		m_nDate[i] = 0;
		m_nTick[i] = 1;
		m_bAllDay[i] = 1;
	}
}


void CMiniTickBarCFG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniTickBarCFG)
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STC_BTN_1, m_strBtn[IDC_STC_BTN_1 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_2, m_strBtn[IDC_STC_BTN_2 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_3, m_strBtn[IDC_STC_BTN_3 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_4, m_strBtn[IDC_STC_BTN_4 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_5, m_strBtn[IDC_STC_BTN_5 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_6, m_strBtn[IDC_STC_BTN_6 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_7, m_strBtn[IDC_STC_BTN_7 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_8, m_strBtn[IDC_STC_BTN_8 - IDC_STC_BTN_1]);
	DDX_Text(pDX, IDC_STC_BTN_9, m_strBtn[IDC_STC_BTN_9 - IDC_STC_BTN_1]);
	
	DDX_CBIndex(pDX, IDC_CMB_MARKET_1, m_nMarket[IDC_CMB_MARKET_1 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_2, m_nMarket[IDC_CMB_MARKET_2 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_3, m_nMarket[IDC_CMB_MARKET_3 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_4, m_nMarket[IDC_CMB_MARKET_4 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_5, m_nMarket[IDC_CMB_MARKET_5 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_6, m_nMarket[IDC_CMB_MARKET_6 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_7, m_nMarket[IDC_CMB_MARKET_7 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_8, m_nMarket[IDC_CMB_MARKET_8 - IDC_CMB_MARKET_1]);
	DDX_CBIndex(pDX, IDC_CMB_MARKET_9, m_nMarket[IDC_CMB_MARKET_9 - IDC_CMB_MARKET_1]);

	DDX_CBIndex(pDX, IDC_CMB_DATE_1, m_nDate[IDC_CMB_DATE_1 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_2, m_nDate[IDC_CMB_DATE_2 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_3, m_nDate[IDC_CMB_DATE_3 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_4, m_nDate[IDC_CMB_DATE_4 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_5, m_nDate[IDC_CMB_DATE_5 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_6, m_nDate[IDC_CMB_DATE_6 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_7, m_nDate[IDC_CMB_DATE_7 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_8, m_nDate[IDC_CMB_DATE_8 - IDC_CMB_DATE_1]);
	DDX_CBIndex(pDX, IDC_CMB_DATE_9, m_nDate[IDC_CMB_DATE_9 - IDC_CMB_DATE_1]);

	DDX_Text(pDX, IDC_EDIT_TICK_1, m_nTick[IDC_EDIT_TICK_1 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_2, m_nTick[IDC_EDIT_TICK_2 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_3, m_nTick[IDC_EDIT_TICK_3 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_4, m_nTick[IDC_EDIT_TICK_4 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_5, m_nTick[IDC_EDIT_TICK_5 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_6, m_nTick[IDC_EDIT_TICK_6 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_7, m_nTick[IDC_EDIT_TICK_7 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_8, m_nTick[IDC_EDIT_TICK_8 - IDC_EDIT_TICK_1]);
	DDX_Text(pDX, IDC_EDIT_TICK_9, m_nTick[IDC_EDIT_TICK_9 - IDC_EDIT_TICK_1]);

	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_1, m_bAllDay[IDC_CMB_ALLDAY_1 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_2, m_bAllDay[IDC_CMB_ALLDAY_2 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_3, m_bAllDay[IDC_CMB_ALLDAY_3 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_4, m_bAllDay[IDC_CMB_ALLDAY_4 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_5, m_bAllDay[IDC_CMB_ALLDAY_5 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_6, m_bAllDay[IDC_CMB_ALLDAY_6 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_7, m_bAllDay[IDC_CMB_ALLDAY_7 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_8, m_bAllDay[IDC_CMB_ALLDAY_8 - IDC_CMB_ALLDAY_1]);
	DDX_CBIndex(pDX, IDC_CMB_ALLDAY_9, m_bAllDay[IDC_CMB_ALLDAY_9 - IDC_CMB_ALLDAY_1]);
}


BEGIN_MESSAGE_MAP(CMiniTickBarCFG, CDialog)
	//{{AFX_MSG_MAP(CMiniTickBarCFG)
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_CMB_MARKET_1, IDC_CMB_MARKET_9, OnSelchangeCmbMarket)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_CMB_DATE_1, IDC_CMB_DATE_9, OnSelchangeCmbDate)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT_TICK_1, IDC_EDIT_TICK_9, OnEditTick)
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_CMB_ALLDAY_1, IDC_CMB_ALLDAY_9, OnSelchangeCmbAllday)
	ON_NOTIFY_RANGE(UDN_DELTAPOS, IDC_SPIN_TICK_1, IDC_SPIN_TICK_9, OnDeltaposSpinTick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniTickBarCFG message handlers

BOOL CMiniTickBarCFG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetTickButtonConfig();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMiniTickBarCFG::SetTickButtonConfig()
{
	if(!m_pTickButton)
		return;

	for(int i = 0; i < 9; i++)
	{
		m_strBtn[i] = m_pTickButton->GetTitle(i);
		m_nMarket[i] = m_pTickButton->GetMarket(i);
		m_nDate[i] = m_pTickButton->GetDateIndex(i);
		m_nTick[i] = m_pTickButton->GetTick(i);
		m_bAllDay[i] = m_pTickButton->GetAllDay(i);

		if(m_nDate[i] > 1)
		{
			GetDlgItem(IDC_EDIT_TICK_1 + i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_CMB_ALLDAY_1 + i)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_SPIN_TICK_1 + i)->ShowWindow(SW_HIDE);
		}
	}
}


void CMiniTickBarCFG::OnSelchangeCmbMarket(UINT nID) 
{
	UpdateData();

	int nIndex = nID - IDC_CMB_MARKET_1;
	m_pTickButton->SetMarket(nIndex, m_nMarket[nIndex]);

	m_strBtn[nIndex] = m_pTickButton->MakeTickButtonTitle(nIndex);

	UpdateData(FALSE);
}

void CMiniTickBarCFG::OnSelchangeCmbDate(UINT nID) 
{
	UpdateData();

	int nIndex = nID - IDC_CMB_DATE_1;
	m_pTickButton->SetDateIndex(nIndex, m_nDate[nIndex]);

	m_strBtn[nIndex] = m_pTickButton->MakeTickButtonTitle(nIndex);

	UpdateData(FALSE);

	int nShow = m_nDate[nIndex] <= 1 ? SW_SHOW : SW_HIDE;
	GetDlgItem(IDC_EDIT_TICK_1 + nIndex)->ShowWindow(nShow);
	GetDlgItem(IDC_CMB_ALLDAY_1 + nIndex)->ShowWindow(nShow);
	GetDlgItem(IDC_SPIN_TICK_1 + nIndex)->ShowWindow(nShow);

	if(m_nDate[nIndex] > 1)
	{
		m_pTickButton->SetTick(nIndex, 1);
		m_pTickButton->SetAllDay(nIndex, TRUE);
	}
	else
	{
		m_pTickButton->SetTick(nIndex, m_nTick[nIndex]);
		m_pTickButton->SetAllDay(nIndex, m_bAllDay[nIndex]);
	}
}

void CMiniTickBarCFG::OnEditTick(UINT nID) 
{
	UpdateData();

	int nIndex = nID - IDC_EDIT_TICK_1;

	if(m_nTick[nIndex] <= 0)
		m_nTick[nIndex] = 1;
	else if(m_nTick[nIndex] > 360) // KHD : 360으로 변경
		m_nTick[nIndex] = 360;

	m_pTickButton->SetTick(nIndex, m_nTick[nIndex]);

	m_strBtn[nIndex] = m_pTickButton->MakeTickButtonTitle(nIndex);

	UpdateData(FALSE);
}

void CMiniTickBarCFG::OnSelchangeCmbAllday(UINT nID) 
{
	UpdateData();

	int nIndex = nID - IDC_CMB_ALLDAY_1;
	m_pTickButton->SetAllDay(nIndex, m_bAllDay[nIndex]);

	UpdateData(FALSE);
}

void CMiniTickBarCFG::OnDeltaposSpinTick(UINT nID, NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	UpdateData();

	int nIndex = nID - IDC_SPIN_TICK_1;
	int nDelta = pNMUpDown->iDelta * -1;

	m_nTick[nIndex] += nDelta;

	if(m_nTick[nIndex] <= 0)
		m_nTick[nIndex] = 1;
	else if(m_nTick[nIndex] > 120)
		m_nTick[nIndex] = 120;

	UpdateData(FALSE);
	
	*pResult = 0;
}
