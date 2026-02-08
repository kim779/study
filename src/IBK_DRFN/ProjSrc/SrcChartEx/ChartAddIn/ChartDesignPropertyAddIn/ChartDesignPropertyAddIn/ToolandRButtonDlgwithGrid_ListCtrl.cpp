// ToolandRButtonDlgwithGrid_ListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"
#include "ToolandRButtonDlgwithGrid_ListCtrl.h"

#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonDlgwithGrid_ListCtrl dialog


CToolandRButtonDlgwithGrid_ListCtrl::CToolandRButtonDlgwithGrid_ListCtrl(const int nID, CWnd* pParent /*=NULL*/)
	: CDialog(CToolandRButtonDlgwithGrid_ListCtrl::IDD, pParent)
{
	// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
	m_hOcxWnd = NULL;

	m_nID = nID;
	//{{AFX_DATA_INIT(CToolandRButtonDlgwithGrid_ListCtrl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CToolandRButtonDlgwithGrid_ListCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CToolandRButtonDlgwithGrid_ListCtrl)
	DDX_Control(pDX, IDC_LIST_FRAME, m_Frame);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CToolandRButtonDlgwithGrid_ListCtrl, CDialog)
	//{{AFX_MSG_MAP(CToolandRButtonDlgwithGrid_ListCtrl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CToolandRButtonDlgwithGrid_ListCtrl message handlers

BOOL CToolandRButtonDlgwithGrid_ListCtrl::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect rc;
	m_Frame.GetWindowRect(rc);
	ScreenToClient(rc);

	// (2006/3/3 - Seung-Won, Bae) for Outer ToolBar
	rc.bottom -= 35;

	m_Frame.ShowWindow(SW_HIDE);
	if(m_GridList.GetSafeHwnd())
			m_GridList.MoveWindow(rc);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CToolandRButtonDlgwithGrid_ListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_GridList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0,0,0,0), this, m_nID);	
	m_GridList.InitializeGrid();
	return 0;
}

void CToolandRButtonDlgwithGrid_ListCtrl::InitDropTarget()
{
	m_GridList.InitDropTarget();
}

CString CToolandRButtonDlgwithGrid_ListCtrl::GetListItemInfo()
{
	return m_GridList.GetListItemInfo();
}

// MODIFY: 종찬(04/03/17) 수치조회관련. 내부툴바 탭페이지와 수치조회탭페이지의 옵션값 연동을 위해서
void CToolandRButtonDlgwithGrid_ListCtrl::SetListItemInfoMgrObject(CToolandRButtonItemMgr *pItemInfoMgr, LPCTSTR pcszTabName, CNumericalInquiryOptionDlg* pNumericalInquiryOptionDlg)
{
	m_GridList.SetListItemInfoMgrObject(pItemInfoMgr, pcszTabName, pNumericalInquiryOptionDlg); 
}

void CToolandRButtonDlgwithGrid_ListCtrl::SetListItemInfoFromParent(const CString &strItemInfo)
{
	if(strItemInfo.IsEmpty())
		return;

	CString _szData = strItemInfo;

	if(_szData.Right(2) != "\r\n")
		_szData += "\r\n";

	int nCount = CDataConversion::GetStringDataCount(_szData, "\r\n");
	for(int i =0 ; i< nCount; i++) {
		CString szItemData = CDataConversion::GetStringData(_szData, "\r\n");
		CString strItemName = CDataConversion::GetStringData(szItemData, ";");
		CString strLimit1 = CDataConversion::GetStringData(szItemData, ";");
		//데이터 보기에서에 엑셀지원 옵션을 없앰으로 인한 예외처리
		//2003.08.06 nykim
		if(strLimit1.Find( _MTEXT( C4_EXPORT_EXCEL_DATA)) >= 0)
			strLimit1 = "";
		CString strLimit2 = CDataConversion::GetStringData(szItemData, ";");
		m_GridList.InsertSavedItemInfo(strItemName, strLimit1, strLimit2);
	}

	SetRedraw(1);
	InvalidateRect(NULL);
	UpdateWindow();
}


// (2008/1/14 - Seung-Won, Bae) for MultiLanguage.
void CToolandRButtonDlgwithGrid_ListCtrl::SetOcxHwnd( HWND p_hOcxWnd)
{
	m_hOcxWnd = p_hOcxWnd;
	m_GridList.SetOcxHwnd( p_hOcxWnd);
}
