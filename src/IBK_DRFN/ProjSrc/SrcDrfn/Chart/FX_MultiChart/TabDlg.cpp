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
	m_pMultiChartWnd = NULL;
	m_pChartMng = NULL;
//	m_crBk = CRLEFTBARBACK;

	//m_pEzFinderLib = NULL;	

	m_pIL_W20H20 = NULL; //폭 20에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W34H20 = NULL; //폭 34에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W46H20 = NULL; //폭 46에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W50H20 = NULL; //폭 50에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W55H19 = NULL; //폭 55에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W60H19 = NULL; //폭 60에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W88H19 = NULL; //폭 60에 높이 19인 버튼 이미지리스트
	
	m_pIL_W151H21 = NULL; //폭 151에 높이 21인 버튼 이미지리스트
//@유진삭제 m_pIL_W26H23 = NULL; //폭 26에 높이 23인 버튼 이미지리스트
//@유진삭제 m_pIL_W58H20 = NULL; //폭 58에 높이 20인 버튼 이미지리스트
//@유진삭제	m_pIL_W82H20 = NULL; //폭 82에 높이 20인 버튼 이미지리스트
	m_pIL_W43H20 = NULL; //폭 43에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W43H19 = NULL; //폭 43에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W46H19 = NULL; //폭 46에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W58H19 = NULL; //폭 58에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W72H19 = NULL; //폭 72에 높이 19인 버튼 이미지리스트
	m_pIL_W58H20 = NULL; //폭 58에 높이 20인 버튼 이미지리스트
	m_pIL_W72H20 = NULL; //폭 72에 높이 20인 버튼 이미지리스트
//@유진삭제 m_pIL_W52H19 = NULL; //폭 52에 높이 19인 버튼 이미지리스트
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
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
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
HBRUSH CTabDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CRscDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	switch(nCtlColor) {
	case CTLCOLOR_BTN:
	case CTLCOLOR_DLG:
		return m_brBkColor;
		break;
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CTabDlg::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	if( m_pParentDlg == NULL ) return TRUE;

	// 스킨 적용
	m_crBk = (COLORREF)m_pMultiChartWnd->SendMessage(RMSG_GET_SKINCOLOR, (WPARAM)0);
//	m_crBk = ((CStdDialog*)m_pMultiChartWnd)->GetSkinColorManager()->GetColor(1);
	
	m_brBkColor.CreateSolidBrush(m_crBk);
	//m_pDBMgr = m_pParentDlg->GetpDBMgr();
	if(m_pMultiChartWnd)
	{
		CStdDialog* pMainDlg = (CStdDialog*)m_pMultiChartWnd;

		//m_pCommonBaseInfo = pMainDlg->GetpMainBaseInfo();
		//m_pCommonCodeMgr  =	pMainDlg->GetpMainCodeManager();

		m_pIL_W20H20 = &(pMainDlg->m_ImgList_W20H20); //폭 20에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W34H20 = &(pMainDlg->m_ImgList_W34H20); //폭 34에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W46H20 = &(pMainDlg->m_ImgList_W46H20); //폭 46에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W50H20 = &(pMainDlg->m_ImgList_W50H20); //폭 50에 높이 20인 버튼 이미지리스트
//@유진삭제	m_pIL_W55H19 = &(pMainDlg->m_ImgList_W55H19); //폭 55에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W60H19 = &(pMainDlg->m_ImgList_W60H19); //폭 60에 높이 19인 버튼 이미지리스트
//@유진삭제 m_pIL_W88H19 = &(pMainDlg->m_ImgList_W88H19); //폭 88에 높이 19인 버튼 이미지리스트
		m_pIL_W151H21 = &(pMainDlg->m_ImgList_W151H21); //폭 151에 높이 21인 버튼 이미지리스트
//@유진삭제 m_pIL_W26H23 = &(pMainDlg->m_ImgList_W26H23); //폭 26에 높이 23인 버튼 이미지리스트
//@유진삭제 m_pIL_W58H20 = &(pMainDlg->m_ImgList_W58H20); //폭 58에 높이 20인 버튼 이미지리스트
//@유진삭제	m_pIL_W82H20 = &(pMainDlg->m_ImgList_W82H20); //폭 82에 높이 20인 버튼 이미지리스트
		m_pIL_W43H20 = &(pMainDlg->m_ImgList_W43H20); //폭 43에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W43H19 = &(pMainDlg->m_ImgList_W43H19); //폭 43에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W46H19 = &(pMainDlg->m_ImgList_W46H19); //폭 46에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W58H19 = &(pMainDlg->m_ImgList_W58H19); //폭 58에 높이 19인 버튼 이미지리스트
//@유진삭제	m_pIL_W72H19 = &(pMainDlg->m_ImgList_W72H19); //폭 72에 높이 19인 버튼 이미지리스트
		m_pIL_W58H20 = &(pMainDlg->m_ImgList_W58H20); //폭 58에 높이 20인 버튼 이미지리스트
		m_pIL_W72H20 = &(pMainDlg->m_ImgList_W72H20); //폭 72에 높이 20인 버튼 이미지리스트
//@유진삭제 	m_pIL_W52H19 = &(pMainDlg->m_ImgList_W52H19); //폭 52에 높이 19인 버튼 이미지리스트
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTabDlg::OnDestroy() 
{
	m_brBkColor.DeleteObject();
	CRscDlg::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CTabDlg::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

void CTabDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(rcClient, m_crBk);
	dc.BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(), &memDC, 0,0, SRCCOPY);
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
}
