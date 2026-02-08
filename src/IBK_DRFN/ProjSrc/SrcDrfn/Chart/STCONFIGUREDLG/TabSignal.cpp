// TabSignal.cpp : implementation file
//

#include "stdafx.h"
#include "TabSignal.h"
#include "DlgSTOption.h"
#include "../Common_ST/STControllerDef.h"
#include "../Include_ST/ISTControllerD.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabSignal dialog
extern CString Parser(LPCTSTR lpInputData, int *pNShifter );


CTabSignal::CTabSignal(CWnd* pParent /*=NULL*/)
	: CRscDlg(CTabSignal::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabSignal)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bUseBuy = FALSE;
	m_bUseSell = FALSE;
	m_bUseExitlong = FALSE;
	m_bUseExitshort = FALSE;
	m_pDlgSTOption = pParent;
}


void CTabSignal::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabSignal)
	DDX_Control(pDX, IDC_CHK_SAVEASDEFAULT, m_chkSaveAsDefault);
	DDX_Control(pDX, IDC_CHK_SHOWQTY, m_chkShowQty);
	DDX_Control(pDX, IDC_CHK_SHOWNAME, m_chkShowName);
	DDX_Control(pDX, IDC_CB_SELL, m_cbSell);
	DDX_Control(pDX, IDC_CB_EXITSHORT, m_cbExitshort);
	DDX_Control(pDX, IDC_CB_EXITLONG, m_cbExitlong);
	DDX_Control(pDX, IDC_CB_BUY, m_cbBuy);
	DDX_Control(pDX, IDC_BT_CLR_SELL, m_btnClrSell);
	DDX_Control(pDX, IDC_BT_CLR_EXITSHORT, m_btnClrExitshort);
	DDX_Control(pDX, IDC_BT_CLR_EXITLONG, m_btnClrExitlong);
	DDX_Control(pDX, IDC_BT_CLR_BUY, m_btnClrBuy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabSignal, CRscDlg)
	//{{AFX_MSG_MAP(CTabSignal)
	ON_BN_CLICKED(IDC_BT_CLR_BUY, OnBtClrBuy)
	ON_BN_CLICKED(IDC_BT_CLR_EXITLONG, OnBtClrExitlong)
	ON_BN_CLICKED(IDC_BT_CLR_EXITSHORT, OnBtClrExitshort)
	ON_BN_CLICKED(IDC_BT_CLR_SELL, OnBtClrSell)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabSignal message handlers

BOOL CTabSignal::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	// TODO: Add extra initialization here
	CISTControllerD *pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();
	SetCBSignal(m_cbBuy);
	SetCBSignal(m_cbExitlong);
	SetCBSignal(m_cbSell);
	SetCBSignal(m_cbExitshort);		
	if(!pSTControllerCfg)
	{
		m_chkSaveAsDefault.SetCheck(1);
		m_chkSaveAsDefault.EnableWindow(FALSE);
	}
	else
	{
		CString strSTNamePath = pSTControllerCfg->GetSTNamePath();
		if(!strSTNamePath.GetLength())
		{
			m_chkSaveAsDefault.SetCheck(1);
			m_chkSaveAsDefault.EnableWindow(FALSE);
		}
	}

	LoadInfo();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabSignal::SetCBSignal(CBitmapItemComboBox &rCb)
{
	rCb.m_bitmap.LoadBitmap(IDB_SIGNAL);
	rCb.AddString("");
	rCb.AddString("");
	rCb.AddString("");
	rCb.AddString("");
	for (int i= -1 ;i < rCb.GetCount();i++) 
		rCb.SetItemHeight(i, 14);	
}

void CTabSignal::OnBtClrBuy() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrBuy);
}

void CTabSignal::OnBtClrExitlong() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrExitlong);	
}

void CTabSignal::OnBtClrExitshort() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrExitshort);	
}

void CTabSignal::OnBtClrSell() 
{
	// TODO: Add your control notification handler code here
	OnBtnClr(m_btnClrSell);
}

void CTabSignal::OnBtnClr(CColorButton &rColorButton)
{
	COLORREF crColor = rColorButton.GetColor();
	CColorDialog color(crColor, CC_FULLOPEN);
	if(color.DoModal() == IDOK) 
	{
		rColorButton.SetColor(color.GetColor());				
		rColorButton.Invalidate();
	}
}

LRESULT CTabSignal::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case UMSG_ST_SAVE_ENV:
		{
			return SaveInfo();
		}		
		break;
	case UMSG_ST_REMOVE_ENV:
		{
			RemoveInfo();
		}
		break;
	}	
	return CRscDlg::WindowProc(message, wParam, lParam);
}

void CTabSignal::LoadInfo()
{
	CISTControllerD* pSTControllerCfg = ((CDlgSTOption*)m_pDlgSTOption)->GetISTControllerD();
	CString strCfgPath = ((CDlgSTOption*)m_pDlgSTOption)->GetSfgPath();

	char szBuffer[256];
	long lShapeSignal = 0;
	COLORREF clrSignal = RGB(0,0,0);
	DWORD dSizeRead = 0;

	m_bUseBuy = FALSE;
	m_bUseSell = FALSE;
	m_bUseExitlong = FALSE;
	m_bUseExitshort = FALSE;

	if(pSTControllerCfg)
	{
		CString strComment = pSTControllerCfg->GetDeclaredInnerFuncInfo();
		CString strItem;
		int nShifter = 0;
		while(1)
		{
			strItem = Parser(strComment,&nShifter);
			if(strItem.GetLength())
			{
				if(strItem==USEBUY)			m_bUseBuy = TRUE;
				if(strItem==USESELL)		m_bUseSell = TRUE;
				if(strItem==USEEXITLONG)	m_bUseExitlong = TRUE;
				if(strItem==USEEXITSHORT)	m_bUseExitshort= TRUE;
			}
			else
			{
				break;
			}			
		}
	}
	else
	{
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,USEBUY,"0",szBuffer,sizeof(szBuffer),2);
		m_bUseBuy = atol(szBuffer);
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,USEEXITLONG,"0",szBuffer,sizeof(szBuffer),2);
		m_bUseExitlong = atol(szBuffer);
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,USESELL,"0",szBuffer,sizeof(szBuffer),2);
		m_bUseSell = atol(szBuffer);
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,USEEXITSHORT,"0",szBuffer,sizeof(szBuffer),2);
		m_bUseExitshort = atol(szBuffer);
	}
	if(m_bUseBuy)
	{
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEBUY,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
			if(lShapeSignal<0)	lShapeSignal = 0;
		}
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,COLORBUY,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrSignal = SIGNALBUYDEFAULT;	
		}
		else
		{
			clrSignal = atol(szBuffer);
		}
		m_cbBuy.SetCurSel(lShapeSignal);
		m_btnClrBuy.SetColor(clrSignal);
	}
	else
	{
		m_btnClrBuy.SetColor(DISABLEDEFAULT);
		m_cbBuy.EnableWindow(FALSE);
		m_btnClrBuy.EnableWindow(FALSE);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_bUseExitlong)
	{
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEEXITLONG,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
			if(lShapeSignal<0)	lShapeSignal = 0;
		}
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,COLOREXITLONG,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrSignal = SIGNALEXITLONGDEFAULT;	
		}
		else
		{
			clrSignal = atol(szBuffer);
		}	
		m_cbExitlong.SetCurSel(lShapeSignal);
		m_btnClrExitlong.SetColor(clrSignal);
	}
	else
	{
		m_btnClrExitlong.SetColor(DISABLEDEFAULT);
		m_cbExitlong.EnableWindow(FALSE);
		m_btnClrExitlong.EnableWindow(FALSE);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_bUseSell)
	{
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHAPESELL,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
			if(lShapeSignal<0)	lShapeSignal = 0;
		}
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,COLORSELL,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrSignal = SIGNALSELLDEFAULT;	
		}
		else
		{
			clrSignal = atol(szBuffer);
		}	
		m_cbSell.SetCurSel(lShapeSignal);
		m_btnClrSell.SetColor(clrSignal);
	}
	else
	{
		m_btnClrSell.SetColor(DISABLEDEFAULT);
		m_cbSell.EnableWindow(FALSE);
		m_btnClrSell.EnableWindow(FALSE);
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(m_bUseExitshort)
	{
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEEXITSHORT,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			lShapeSignal = 0;
		}
		else
		{
			lShapeSignal = atol(szBuffer);
			if(lShapeSignal<0)	lShapeSignal = 0;
		}
		dSizeRead = ((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,COLOREXITSHORT,"",szBuffer,sizeof(szBuffer),2);
		if(!dSizeRead)
		{
			clrSignal = SIGNALEXITSHORTDEFAULT;	
		}
		else
		{
			clrSignal = atol(szBuffer);
		}	
		m_cbExitshort.SetCurSel(lShapeSignal);
		m_btnClrExitshort.SetColor(clrSignal);
	}
	else
	{
		m_btnClrExitshort.SetColor(DISABLEDEFAULT);
		m_cbExitshort.EnableWindow(FALSE);
		m_btnClrExitshort.EnableWindow(FALSE);
	}


	if(m_bUseBuy||m_bUseSell||m_bUseExitlong||m_bUseExitshort)	
	{
		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALNAME,"0",szBuffer,sizeof(szBuffer),2);
		m_chkShowName.SetCheck(atol(szBuffer));

		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALQTY,"0",szBuffer,sizeof(szBuffer),2);
		m_chkShowQty.SetCheck(atol(szBuffer));

//		((CDlgSTOption*)m_pDlgSTOption)->GetPrivateProfileStringInST(SIGNAL,POSOFSIGNAL,"0",szBuffer,sizeof(szBuffer),2);
//		int nPosOfSignal = atol(szBuffer);
//		if(nPosOfSignal==0)
//		{
//			m_ctrlREndOfCandle.SetCheck(1);
//			m_ctrlRStartOfCandle.SetCheck(0);
//		}
//		else
//		{
//			m_ctrlREndOfCandle.SetCheck(0);
//			m_ctrlRStartOfCandle.SetCheck(1);
//		}
	}
	else
	{
		m_chkShowName.EnableWindow(FALSE);
		m_chkShowQty.EnableWindow(FALSE);
//		m_ctrlREndOfCandle.EnableWindow(FALSE);
//		m_ctrlRStartOfCandle.EnableWindow(FALSE);
	}
}

BOOL CTabSignal::SaveInfo()
{
	int nUseType = m_chkSaveAsDefault.GetCheck()==TRUE? 3 : 2;
	CString strKeyData;

	if(m_bUseBuy)
	{
		strKeyData.Format("%d",m_cbBuy.GetCurSel());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEBUY,strKeyData,nUseType);
		strKeyData.Format("%d",m_btnClrBuy.GetColor());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLORBUY,strKeyData,nUseType);
	}

	if(m_bUseExitlong)
	{
		strKeyData.Format("%d",m_cbExitlong.GetCurSel());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEEXITLONG,strKeyData,nUseType);
		strKeyData.Format("%d",m_btnClrExitlong.GetColor());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLOREXITLONG,strKeyData,nUseType);
	}

	if(m_bUseSell)
	{
		strKeyData.Format("%d",m_cbSell.GetCurSel());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPESELL,strKeyData,nUseType);
		strKeyData.Format("%d",m_btnClrSell.GetColor());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLORSELL,strKeyData,nUseType);
	}

	if(m_bUseExitshort)
	{
		strKeyData.Format("%d",m_cbExitshort.GetCurSel());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEEXITSHORT,strKeyData,nUseType);
		strKeyData.Format("%d",m_btnClrExitshort.GetColor());
		((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLOREXITSHORT,strKeyData,nUseType);
	}

	strKeyData.Format("%d",m_chkShowName.GetCheck());
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALNAME,strKeyData,nUseType);
	strKeyData.Format("%d",m_chkShowQty.GetCheck());
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALQTY,strKeyData,nUseType);

	return TRUE;
}

void CTabSignal::RemoveInfo()
{
	int nUseType = 2; // Remove In STFile
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEBUY,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLORBUY,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEEXITLONG,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLOREXITLONG,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPESELL,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLORSELL,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHAPEEXITSHORT,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,COLOREXITSHORT,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALNAME,"",nUseType);
	((CDlgSTOption*)m_pDlgSTOption)->WritePrivateProfileStringInST(STCONFIG_SIGNAL,SHOWSIGNALQTY,"",nUseType);
}

//void CTabSignal::OnRendofcandle() 
//{
//	// TODO: Add your control notification handler code here
//	m_ctrlREndOfCandle.SetCheck(1);
//	m_ctrlRStartOfCandle.SetCheck(0);	
//}
//
//void CTabSignal::OnRstartofcandle() 
//{
//	// TODO: Add your control notification handler code here
//	m_ctrlREndOfCandle.SetCheck(0);
//	m_ctrlRStartOfCandle.SetCheck(1);	
//}

HBRUSH CTabSignal::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) 
	{
	case CTLCOLOR_EDIT:
	case CTLCOLOR_LISTBOX:
		return hbr;
		break;
	default:
		pDC->SetBkColor(m_crBk);
		return *m_pBrBkColor;		
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
