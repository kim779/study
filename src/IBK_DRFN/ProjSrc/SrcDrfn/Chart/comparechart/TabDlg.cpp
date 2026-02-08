// TabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TabDlg.h"
#include "./define/ChartMsgDef.h"
#include "LeftBarDlg.h"

//#include "../../../DataMgr/DBMgr.h" //05.09.27
//#include "../../../DrMLib/DrMiddleLib.h"//05.09.27

#include "StdDialog.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabDlg dialog


CTabDlg::CTabDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CRscDlg(nIDTemplate , pParent)
{
	//{{AFX_DATA_INIT(CTabDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParentDlg = NULL;
	m_pCompareChartWnd = NULL;
	m_pChartMng = NULL;
//	m_crBk = CRLEFTBARBACK;
//	m_crBk = CLR_LEFTBAR_BKGRND;

	//m_pEzFinderLib = NULL;
}

CTabDlg::~CTabDlg()
{
	
}

void CTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTabDlg, CRscDlg)
	//{{AFX_MSG_MAP(CTabDlg)
//	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
//	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabDlg message handlers

void CTabDlg::RecalcLayoutCtrl(CWnd& rWnd,int nID /*=0*/ )
{
	if(rWnd.GetSafeHwnd())
	{
		if(!nID)
		{
			nID = rWnd.GetDlgCtrlID();
		}
		CRect rect = GetRectOfCtrl(nID);
		rWnd.MoveWindow(rect.left,rect.top,rect.Width(),rect.Height());
	}
}


BOOL CTabDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->wParam == VK_HOME)
		return FALSE;
	
	if(::IsWindow(m_hWnd))
		return GetParent()->PreTranslateMessage(pMsg);
	else
		return CRscDlg::PreTranslateMessage(pMsg);
}
//HBRUSH CTabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
//{
//	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
//	
//	// TODO: Change any attributes of the DC here
//	switch(nCtlColor) {
//	case CTLCOLOR_BTN:
//	case CTLCOLOR_DLG:
//		return m_brBkColor;
//		break;
//	}
//	
//	// TODO: Return a different brush if the default is not desired
//	return hbr;
//}

BOOL CTabDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	if( m_pParentDlg == NULL ) return TRUE;
//	m_crBk = ((CStdDialog*)m_pCompareChartWnd)->GetSkinColorManager()->GetColor(1);
	
//	m_brBkColor.CreateSolidBrush(m_crBk);

	CStdDialog* pMainDlg = (CStdDialog*)GetParent();
	m_pIL_W46H20 = &(pMainDlg->m_ImgList_W46H20); //폭 46 높이 20인 버튼 이미지리스트
	m_pIL_W31H20 = &(pMainDlg->m_ImgList_W31H20); //폭 31 높이 20인 버튼 이미지리스트
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabDlg::OnDestroy() 
{
	m_brBkColor.DeleteObject();
	CRscDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

//BOOL CTabDlg::OnEraseBkgnd(CDC* pDC) 
//{
//	return TRUE;
//}

//void CTabDlg::OnPaint() 
//{
//	CPaintDC dc(this); // device context for painting
//	CRect rcClient;
//	GetClientRect(&rcClient);
//
//	CDC memDC;
//	CBitmap bitmap;
//	memDC.CreateCompatibleDC(&dc);
//	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
//	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);
//
//	memDC.FillSolidRect(rcClient, m_crBk);
//	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
//	memDC.SelectObject(pOldBitmap);
//	memDC.DeleteDC();
//	bitmap.DeleteObject();
//}