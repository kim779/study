// DlgSettingSignal.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSettingSignal.h"
#include "StrategyConfigureWizardDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingSignal dialog
#define CRBACKGROUND	RGB(211,223,241)


CDlgSettingSignal::CDlgSettingSignal(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSettingSignal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSettingSignal)
	//}}AFX_DATA_INIT
	m_pDlgStrategyList = NULL;
	m_bUseExitshort = FALSE;
	m_bUseExitlong = FALSE;
	m_bUseBuy = FALSE;
	m_bUseSell = FALSE;
	m_crBk = CRBACKGROUND;
	m_bShowName = FALSE;
	m_bShowQty = FALSE;
}


void CDlgSettingSignal::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSettingSignal)
	DDX_Control(pDX, IDC_CHK_SHOWQTY, m_chkShowQty);
	DDX_Control(pDX, IDC_CHK_SHOWNAME, m_chkShowName);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BT_CLR_EXITSHORT, m_btnClrExitshort);
	DDX_Control(pDX, IDC_BT_CLR_EXITLONG, m_btnClrExitlong);
	DDX_Control(pDX, IDC_BT_CLR_BUY, m_btnClrBuy);
	DDX_Control(pDX, IDC_BT_CLR_SELL, m_btnClrSell);
	DDX_Control(pDX, IDC_CB_SELL, m_cbSell);
	DDX_Control(pDX, IDC_CB_EXITSHORT, m_cbExitshort);
	DDX_Control(pDX, IDC_CB_EXITLONG, m_cbExitlong);
	DDX_Control(pDX, IDC_CB_BUY, m_cbBuy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSettingSignal, CDialog)
	//{{AFX_MSG_MAP(CDlgSettingSignal)
	ON_BN_CLICKED(IDC_BT_CLR_BUY, OnBtClrBuy)
	ON_BN_CLICKED(IDC_BT_CLR_EXITLONG, OnBtClrExitlong)
	ON_BN_CLICKED(IDC_BT_CLR_EXITSHORT, OnBtClrExitshort)
	ON_BN_CLICKED(IDC_BT_CLR_SELL, OnBtClrSell)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHK_SHOWNAME, OnChkShowname)
	ON_BN_CLICKED(IDC_CHK_SHOWQTY, OnChkShowqty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSettingSignal message handlers

void CDlgSettingSignal::SetCBSignal(CBitmapItemComboBox &rCb)
{
	rCb.m_bitmap.LoadBitmap(IDB_SIGNAL);
	rCb.AddString("");
	rCb.AddString("");
	rCb.AddString("");
	rCb.AddString("");
	for (int i= -1 ;i < rCb.GetCount();i++) 
		rCb.SetItemHeight(i, 14);	
}

BOOL CDlgSettingSignal::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_brBkColor.CreateSolidBrush(m_crBk);
	SetCBSignal(m_cbBuy);
	SetCBSignal(m_cbExitlong);
	SetCBSignal(m_cbSell);
	SetCBSignal(m_cbExitshort);		
	m_btnOk.SetSkin(IDB_BITMAP_OKNOR,IDB_BITMAP_OKSEL,IDB_BITMAP_OKNOR,IDB_BITMAP_OKDIS);
	m_btnCancel.SetSkin(IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELSEL,IDB_BITMAP_CANCELNOR,IDB_BITMAP_CANCELDIS);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSettingSignal::OnBtnClr(CColorButton &rColorButton)
{
	COLORREF crColor = rColorButton.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		rColorButton.SetColor(color.GetColor());				
		rColorButton.Invalidate();
	}
}


void CDlgSettingSignal::OnOK() 
{
	// TODO: Add extra validation here
	if(m_pDlgStrategyList)
	{
		m_pDlgStrategyList->SendMessage(UWM_SETALLSIGNAL);
	}	
	CDialog::OnOK();
}

void CDlgSettingSignal::OnBtClrBuy() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrBuy);
}

void CDlgSettingSignal::OnBtClrExitlong() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrExitlong);	
}

void CDlgSettingSignal::OnBtClrExitshort() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrExitshort);	
}

void CDlgSettingSignal::OnBtClrSell() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrSell);
}

void CDlgSettingSignal::UpdateInfo()
{
	m_cbBuy.EnableWindow(m_bUseBuy);
	m_btnClrBuy.EnableWindow(m_bUseBuy);
	m_cbSell.EnableWindow(m_bUseSell);
	m_btnClrSell.EnableWindow(m_bUseSell);
	m_cbExitlong.EnableWindow(m_bUseExitlong);
	m_btnClrExitlong.EnableWindow(m_bUseExitlong);
	m_cbExitshort.EnableWindow(m_bUseExitshort);
	m_btnClrExitshort.EnableWindow(m_bUseExitshort);
	if(!m_bUseBuy)
	{
		m_cbBuy.SetCurSel(-1);
		m_btnClrBuy.SetColor(RGB(100,100,100));
	}
	if(!m_bUseSell)
	{
		m_cbSell.SetCurSel(-1);
		m_btnClrSell.SetColor(RGB(100,100,100));
	}
	if(!m_bUseExitlong)
	{
		m_cbExitlong.SetCurSel(-1);
		m_btnClrExitlong.SetColor(RGB(100,100,100));
	}
	if(!m_bUseExitshort)
	{
		m_cbExitshort.SetCurSel(-1);
		m_btnClrExitshort.SetColor(RGB(100,100,100));
	}
	m_chkShowName.SetCheck(m_bShowName);
	m_chkShowQty.SetCheck(m_bShowQty);
}

HBRUSH CDlgSettingSignal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return m_brBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgSettingSignal::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	m_brBkColor.DeleteObject();
	
}

void CDlgSettingSignal::OnChkShowname() 
{
	// TODO: Add your control notification handler code here
	m_bShowName = m_chkShowName.GetCheck();
}

void CDlgSettingSignal::OnChkShowqty() 
{
	// TODO: Add your control notification handler code here
	m_bShowQty = m_chkShowQty.GetCheck();	
}
