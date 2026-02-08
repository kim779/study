// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Notify.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetDlg dialog


CSetDlg::CSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetDlg)
	m_gubnRdo = 1;
	//}}AFX_DATA_INIT
	m_sData = "";
}


void CSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetDlg)
	DDX_Control(pDX, IDC_SET_LIST, m_SetList);
	DDX_Control(pDX, IDC_ORG_LIST, m_OrgList);
	DDX_Radio(pDX, IDC_FUTURE_RDO, m_gubnRdo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetDlg, CDialog)
	//{{AFX_MSG_MAP(CSetDlg)
	ON_BN_CLICKED(IDC_ADD_BTN, OnAddBtn)
	ON_NOTIFY(NM_DBLCLK, IDC_ORG_LIST, OnDblclkOrgList)
	ON_NOTIFY(NM_DBLCLK, IDC_SET_LIST, OnDblclkSetList)
	ON_BN_CLICKED(IDC_FUTURE_RDO, OnRdoClick)
	ON_BN_CLICKED(IDC_STOCK_RDO, OnRdoClick)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDelBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetDlg message handlers

BOOL CSetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_OrgList.InsertColumn(1, "필드명", LVCFMT_LEFT, 90);
	m_SetList.InsertColumn(1, "필드명", LVCFMT_LEFT, 90);
	OnRdoClick();
	return TRUE; 
}

void CSetDlg::OnOK() 
{
	CString szTemp;
	
	UpdateData();
	m_sData.Format("%d%s", m_gubnRdo, "|");
	for (int ii = 0; ii < m_SetList.GetItemCount(); ii++)
	{
		szTemp = m_SetList.GetItemText(ii, 0);
		m_sData +=  szTemp.Left(2) + "|";
	}
	
	CDialog::OnOK();
}

void CSetDlg::OnDblclkOrgList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnAddBtn();	
	*pResult = 0;
}


void CSetDlg::OnDblclkSetList(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnDelBtn();
	*pResult = 0;
}


void CSetDlg::OnAddBtn() 
{
	const	int	item = m_OrgList.GetSelectionMark();
	m_SetList.InsertItem(m_SetList.GetItemCount(), m_OrgList.GetItemText(item, 0));
}


void CSetDlg::OnDelBtn() 
{
	const	int	item = m_SetList.GetSelectionMark();
	m_SetList.DeleteItem(item);
}

void CSetDlg::OnRdoClick() 
{
	UpdateData();
	m_OrgList.DeleteAllItems();
	m_SetList.DeleteAllItems();
	if (m_gubnRdo)
	{
		m_OrgList.InsertItem(0, "00.종목코드");
		m_OrgList.InsertItem(1, "01.종목명");
		m_OrgList.InsertItem(2, "02.구분");
		m_OrgList.InsertItem(3, "03.잔고수량");
		m_OrgList.InsertItem(4, "04.매도가능수량");
		m_OrgList.InsertItem(5, "05.평균가/정산가");
		m_OrgList.InsertItem(6, "06.현재가");
		m_OrgList.InsertItem(7, "07.평균가대비");
		m_OrgList.InsertItem(8, "08.평가금액");
		m_OrgList.InsertItem(9, "09.평가손익");
		m_OrgList.InsertItem(10, "10.수익률");
		m_OrgList.InsertItem(11, "11.매입금액");
		m_OrgList.InsertItem(12, "12.전일대비");
	}
	else
	{
		m_OrgList.InsertItem(0, "00.종목코드");
		m_OrgList.InsertItem(1, "01.종목명");
		m_OrgList.InsertItem(2, "02.결제잔고");
		m_OrgList.InsertItem(3, "03.주문잔량");
		m_OrgList.InsertItem(4, "04.매입평균가");
		m_OrgList.InsertItem(5, "05.매도가능잔고");
		m_OrgList.InsertItem(6, "06.현재가");
		m_OrgList.InsertItem(7, "07.평가금액");
		m_OrgList.InsertItem(8, "08.평가손익");
		m_OrgList.InsertItem(9, "09.평가수익률");
		m_OrgList.InsertItem(10, "10.매입금액");
		m_OrgList.InsertItem(11, "11.대출일");
		m_OrgList.InsertItem(12, "12.신용구분");
		m_OrgList.InsertItem(13, "13.신용구분내용");
		m_OrgList.InsertItem(14, "14.신용구분약자");
		m_OrgList.InsertItem(15, "15.신용대출금액");
		m_OrgList.InsertItem(16, "16.잔고구분");
		m_OrgList.InsertItem(17, "17.BEP단가");
		m_OrgList.InsertItem(18, "18.비용");
		m_OrgList.InsertItem(19, "19.등락률");
	}
}
