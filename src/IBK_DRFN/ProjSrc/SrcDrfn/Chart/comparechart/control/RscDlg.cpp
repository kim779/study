// RscDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RscDlg.h"
#include "../define/ChartMsgDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRscDlg dialog
extern HINSTANCE g_hInstance;


CRscDlg::CRscDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate , pParent)
{
	//{{AFX_DATA_INIT(CRscDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_crBk = CLR_LEFTBAR_BKGRND;
}


void CRscDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRscDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRscDlg, CDialog)
	//{{AFX_MSG_MAP(CRscDlg)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRscDlg message handlers

 

BOOL CRscDlg::Create(UINT nID,CWnd* pParentWnd)
{
	// TODO: Add your specialized code here and/or call the base class
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
	BOOL bRetValue = CDialog::Create(nID, pParentWnd);
	AfxSetResourceHandle(hInstResourceClient);
	return bRetValue;
}

int CRscDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	HINSTANCE hInstResourceClient = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);	
	BOOL bRetValue = CDialog::DoModal();
	AfxSetResourceHandle(hInstResourceClient);
	return bRetValue;

}

BOOL CRscDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_brBkColor.CreateSolidBrush(m_crBk);

	return TRUE;
}

void CRscDlg::OnDestroy() 
{	
	m_brBkColor.DeleteObject();
	CDialog::OnDestroy();
}

HBRUSH CRscDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);	
	switch(nCtlColor) 
	{	
		case CTLCOLOR_EDIT:
		case CTLCOLOR_LISTBOX:
			return hbr;
		case CTLCOLOR_DLG		:				
			return m_brBkColor;
//		case CTLCOLOR_BTN       : 				 
//			break;		
		default:			
			{
				pDC->SetBkColor(m_crBk);
				return m_brBkColor;
			}		
			break;
	}
	return hbr;
}

BOOL CRscDlg::OnEraseBkgnd(CDC* pDC) 
{
	CRect rcExtTool;
	pDC->GetClipBox(rcExtTool);
	pDC->FillSolidRect(&rcExtTool, m_crBk);
	return FALSE;
}

void CRscDlg::ChangeSkinColor(COLORREF clrBackground)
{
	m_brBkColor.DeleteObject();
	m_crBk = clrBackground;
	m_brBkColor.CreateSolidBrush(m_crBk);
	Invalidate();
}
