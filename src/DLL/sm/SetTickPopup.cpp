// SetTickPopup.cpp : implementation file
//

#include "stdafx.h"
#include "SetTickPopup.h"
#include "smdefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTickPopup dialog


CSetTickPopup::CSetTickPopup(CWnd* pParent /*=NULL*/)
	: CDialog(CSetTickPopup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetTickPopup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
}


void CSetTickPopup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTickPopup)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTickPopup, CDialog)
	//{{AFX_MSG_MAP(CSetTickPopup)
	ON_WM_KILLFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_ALL_CHECK, OnAllCheck)
	ON_BN_CLICKED(IDC_SETUP_BTN, OnSetupBtn)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTickPopup message handlers

void CSetTickPopup::OnKillFocus(CWnd* pNewWnd) 
{
	CDialog::OnKillFocus(pNewWnd);
	
		
}

void CSetTickPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
	if (nState == WA_INACTIVE)
	{
		PostNcDestroy();
	}
}

void CSetTickPopup::PostNcDestroy() 
{	
	CDialog::PostNcDestroy();
	delete this;
}

void CSetTickPopup::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// lines
	if (GetDlgItem(IDC_KS_NEXT) && GetDlgItem(IDC_KOSPI_INV) && GetDlgItem(IDC_FUTURE_INV) && GetDlgItem(IDC_DOWJONES))
	{
		CRect itemRc;
		CRect clientRc;
		GetClientRect(&clientRc);
		
		clientRc.DeflateRect(10, 0, 10, 0);
		
		//DWORD items[] = {IDC_KS_NEXT, IDC_KOSPI_INV, IDC_FUTURE_INV, IDC_DOWJONES};
		const DWORD items[] = {IDC_KS_NEXT, IDC_KOSPI_INV};
		for (int ii = 0; ii < sizeof(items)/ sizeof(DWORD); ii++)
		{
			GetDlgItem(items[ii])->GetWindowRect(&itemRc);
			ScreenToClient(&itemRc);
			clientRc.top = itemRc.bottom + 3;
			clientRc.bottom = clientRc.top + 1;
			dc.Draw3dRect(clientRc, GetSysColor(COLOR_3DHILIGHT), GetSysColor(COLOR_3DSHADOW));
		}
	}
}

// DWORD ctrlIDs[] = 
// {
// 	IDC_KRX100, IDC_KOSPI, IDC_KOSPI200, IDC_KOSTAR, 
// 	IDC_KOSDAQ, IDC_KOSDAQ_VEN, IDC_KP_NEAR, IDC_KP_NEXT, 
// 
// 	IDC_KOSPI_PRG, IDC_KOSDAQ_PRG, 
// 	IDC_KOSPI_INV, IDC_KOSDAQ_INV, 
// 	IDC_FUTURE_INV, IDC_CALL_INV, IDC_PUT_INV, 
// 	
// 	IDC_NASDAQ, IDC_NASDAQ100_FUTURE, IDC_MININ100, 
// 	IDC_SP500, IDC_SP500_FUTURE, IDC_MINISP500, IDC_DOWJONES, 
// 	
// 	IDC_NIKKEI, IDC_WEIGHT, IDC_ASHARE, IDC_HANGSE, IDC_HSCE, IDC_SRI, IDC_KP_NEAR_BASIS,
// 	IDC_SANGHAI_B, IDC_SIM_B, IDC_BOMBAY, IDC_TAIPEI, IDC_HCMC, IDC_MICRO, IDC_INMEX,
// 	IDC_BOVESPA, IDC_FTSE100, IDC_XETRA, IDC_CAC40, IDC_RTSI, IDC_SANGHAI
// };
//티커 지수 수정시 고쳐야되는 부분
DWORD ctrlIDs[] = 
{
	IDC_KRX100, IDC_KOSPI, IDC_KOSPI200, IDC_KOSDAQ150,/*IDC_KOSTAR,*/ 
		IDC_KOSDAQ, IDC_KOSDAQ_VEN, IDC_KP_NEAR, IDC_KP_NEXT, 
		
		IDC_KOSPI_PRG, IDC_KOSDAQ_PRG, 
		IDC_KOSPI_INV, IDC_KOSDAQ_INV, 
		IDC_FUTURE_INV, IDC_CALL_INV, IDC_PUT_INV, 
		
		IDC_SRI, IDC_KP_NEAR_BASIS
};

void CSetTickPopup::OnAllCheck() 
{
	if (GetDlgItem(IDC_ALL_CHECK))
	{
		const BOOL bCheck = ((CButton*)GetDlgItem(IDC_ALL_CHECK))->GetCheck();
		{
			for (int ii = 0; ii < sizeof(ctrlIDs) / sizeof(DWORD); ii++)
			{
				/*
				if (ii == 28) //환율이라면 호출 안하도록 수정
					((CButton*)GetDlgItem(ctrlIDs[ii]))->SetCheck(false);
				else
				*/
				((CButton*)GetDlgItem(ctrlIDs[ii]))->SetCheck(bCheck);
			}
		}
	}
}

//기존 해외 지수
// const int indexKey[] = {
// 	1, 2, 3, 4, 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 20,
// 		21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33
// };
//새로운 해외 지수
const int indexKey[] = {
	1, 2, 3, 4,/*4,*/ 5, 6, 9, 10, 13, 14, 15, 16, 17, 18, 19, 32, 33
};
  
const int indexKeyCount = ArraySize(indexKey);

DWORD CSetTickPopup::GetIDByIdx(int nIdx)
{
	for (int i = 0; i < indexKeyCount; i++)
	{
		if (indexKey[i] == nIdx)
		{
			return ctrlIDs[i];
		}
	}

	return 0;

	/**
	int	nID = 0;
	switch (nIdx)
	{
	default:
	case 1:		nID = IDC_KRX100;	break;
	case 2:		nID = IDC_KOSPI;	break;
	case 3:		nID = IDC_KOSPI200;	break;
	case 4:		nID = IDC_KOSTAR;	break;
	case 5:		nID = IDC_KOSDAQ;	break;
	case 6:		nID = IDC_KOSDAQ_VEN;	break;
	case 9:		nID = IDC_KP_NEAR;	break;
	case 10:	nID = IDC_KP_NEXT;	break;
	case 11:	nID = IDC_KS_NEAR;	break;   
	case 12:	nID = IDC_KS_NEXT;	break;
	case 13:	nID = IDC_KOSPI_PRG;	break;
	case 14:	nID = IDC_KOSDAQ_PRG;	break;
	case 15:	nID = IDC_KOSPI_INV;	break;
	case 16:	nID = IDC_KOSDAQ_INV;	break;
	case 17:	nID = IDC_FUTURE_INV;	break;
	case 18:	nID = IDC_CALL_INV;	break;
	case 19:	nID = IDC_PUT_INV;	break;
	case 20:	nID = IDC_NASDAQ;	break;
	case 21:	nID = IDC_NASDAQ100_FUTURE;	break;
	case 22:	nID = IDC_MININ100;	break;
	case 23:	nID = IDC_SP500;	break;
	case 24:	nID = IDC_SP500_FUTURE;	break;
	case 25:	nID = IDC_MINISP500;	break;
	case 26:	nID = IDC_DOWJONES;	break;
	case 27:	nID = IDC_NIKKEI;	break;
	case 28:	nID = IDC_WEIGHT;	break;
	case 29:	nID = IDC_ASHARE;	break;
	case 30:	nID = IDC_HANGSE;	break;
	case 31:	nID = IDC_HSCE;		break;
	case 35:	nID = IDC_EXCHANGE;	break;
	}
	return nID;
	**/
}

void CSetTickPopup::SetCheck(int nIdx, BOOL bCheck)
{
	const int nID = GetIDByIdx(nIdx);
	if (nID)
	{
		if (nID == 35) //환율이라면 무조건 호출 안하도록 수정
			((CButton*)GetDlgItem(nID))->SetCheck(false);
		else
			((CButton*)GetDlgItem(nID))->SetCheck(bCheck);
	}
}

BOOL CSetTickPopup::GetCheck(int nIdx)
{
	const int nID = GetIDByIdx(nIdx);

	if (nID)
	{
		if (nID == 35) //환율이라면 무조건 호출 안하도록 수정
			return false;
		else
			return ((CButton*)GetDlgItem(nID))->GetCheck();
	}
	return FALSE;
}

void CSetTickPopup::OnOK() 
{
	m_pParent->SendMessage(WM_XMSG, MAKEWPARAM(CTRL_SETTICKPOPUP, 0), (LPARAM)this);
	CDialog::OnOK();
}

BOOL CSetTickPopup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//if (m_point.x != -1 && m_point.y != -1)
	{
		CRect rc;
		GetWindowRect(&rc);

		const int x = m_point.x;
		int y = m_point.y - rc.Height();
		if (y < 0)
			y = 0;
		SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetTickPopup::OnSetupBtn() 
{
	m_pParent->PostMessage(WM_XMSG, MAKEWPARAM(CTRL_SETTICKPOPUP, 1), 0);
	CDialog::OnCancel();
}
	
BOOL CSetTickPopup::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CSetTickPopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

