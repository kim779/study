// InitializeControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartDesignPropertyAddIn.h"

#include "InitializeControlDlg.h"
#include "Conversion.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInitializeControlDlg dialog

CInitializeControlDlg::CInitializeControlDlg( HWND p_hOcxWnd, CWnd* pParent /*=NULL*/)
	: CDialog(CInitializeControlDlg::IDD, pParent), m_BlockSetDlg( p_hOcxWnd), m_ColorSetDlg( p_hOcxWnd)
{
	//{{AFX_DATA_INIT(CInitializeControlDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	// (2008/1/22 - Seung-Won, Bae) for Multi-Language
	m_hOcxWnd = p_hOcxWnd;
	m_toolTip.SetOcxHwnd( m_hOcxWnd);
}

CInitializeControlDlg::~CInitializeControlDlg()
{
	m_ImageList.DeleteImageList();
}

void CInitializeControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInitializeControlDlg)
	DDX_Control(pDX, IDC_BLOCKCOLORFONT, m_BlockColorFontListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInitializeControlDlg, CDialog)
	//{{AFX_MSG_MAP(CInitializeControlDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_BLOCKCOLORFONT, OnDblclkBlockcolorfont)
	ON_NOTIFY(NM_CLICK, IDC_BLOCKCOLORFONT, OnClickBlockcolorfont)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInitializeControlDlg message handlers

BOOL CInitializeControlDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitList();	

	m_nPreBlockColorFont = 0;
	m_BlockColorFontListCtrl.SetItemState(m_nPreBlockColorFont, ~LVIS_SELECTED | LVIS_FOCUSED, LVIF_STATE); 

	CreateToolTip();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//리스트컨트롤 초기화
void CInitializeControlDlg::InitList()
{
	m_ImageList.DeleteImageList();

	ML_SET_DEFAULT_RES();
	m_ImageList.Create(IDB_BLOCKCOLOR, 32, 1, RGB(255, 255, 255));
	m_BlockColorFontListCtrl.SetImageList(&m_ImageList, LVSIL_NORMAL);

	LPTSTR lpsz[] = {_T("Block"),_T("Color"),0};

	//	Insert Chart List
	for(int i = 0; i < 2; i++)
	{
		LV_ITEM listItem;
		listItem.mask = LVIF_TEXT | LVIF_IMAGE ;
		listItem.iSubItem = 0 ;
		listItem.iImage = i;       // index of the list view item’s icon 
		listItem.iItem = i ;
		listItem.pszText = lpsz[i] ;    
		m_BlockColorFontListCtrl.InsertItem(&listItem) ;
	}	
	m_BlockColorFontListCtrl.SetIconSpacing(90, 50);
}

void CInitializeControlDlg::OnDblclkBlockcolorfont(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int   iItem = pNMListView->iItem; 

	if(m_nPreBlockColorFont != -1)	{
		m_BlockColorFontListCtrl.SetItemState(m_nPreBlockColorFont, LVIS_SELECTED, LVIF_STATE);	
		m_nPreBlockColorFont = -1;
	}

	ML_SET_LANGUAGE_RES();
	if(iItem == 0)
		m_BlockSetDlg.DoModal();
	else if(iItem == 1)
		m_ColorSetDlg.DoModal();

	*pResult = 0;
}

//모든 Dlg의 속성을 하나의 String으로 받음
void CInitializeControlDlg::SetProperty( IPropertyMap *p_pIPropertyMap)
{
	m_ColorSetDlg.SetProperty( p_pIPropertyMap);
	m_BlockSetDlg.SetProperty( p_pIPropertyMap);
}

//ocx의 값 저장
void CInitializeControlDlg::SetOcxWnd(CWnd *pWnd)
{
	m_pOcxWnd = pWnd;
}

void CInitializeControlDlg::OnOK() 
{
	m_nReturnValue = 1;	
	CDialog::OnOK();
}

//ocx에 보낼 return 값.
int CInitializeControlDlg::GetReturnValue()
{
	return m_nReturnValue;
}

void CInitializeControlDlg::OnCancel() 
{
	m_nReturnValue = 0;
	
	CDialog::OnCancel();
}

void CInitializeControlDlg::OnClickBlockcolorfont(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	int select = pNMListView->iItem;

	if(m_nPreBlockColorFont != -1)	{
		m_BlockColorFontListCtrl.SetItemState(m_nPreBlockColorFont, LVIS_SELECTED, LVIF_STATE);	
		m_nPreBlockColorFont = -1;
	}

	*pResult = 0;
}

//툴팁
void CInitializeControlDlg::CreateToolTip()
{
	ML_SET_LANGUAGE_RES();
	m_toolTip.Create( this );

	CDWordArray dwaInfo;
	dwaInfo.RemoveAll();
	dwaInfo.Add( IDS_BLOCKCOLORFONT_TOOLTIP_1);
	dwaInfo.Add( IDS_BLOCKCOLORFONT_TOOLTIP_2);
	dwaInfo.Add( IDS_BLOCKCOLORFONT_TOOLTIP_3);
	dwaInfo.Add( IDS_BLOCKCOLORFONT_TOOLTIP_4);
	m_toolTip.AddControlInfo( IDC_BLOCKCOLORFONT, dwaInfo, RGB(205, 237, 237));

}

BOOL CInitializeControlDlg::PreTranslateMessage(MSG* pMsg) 
{
	bool bRButton;
	bRButton = false;

	POINT pt = pMsg->pt;	
	ScreenToClient( &pt );

	if( pMsg->message == WM_RBUTTONDOWN )	{
		m_toolTip.ShowToolTip( (CPoint)pt );
		bRButton = true;
	}
	if(pMsg->message == WM_MOUSEMOVE){
		m_toolTip.HideToolTip( (CPoint)pt );
	}
	
	if(bRButton)
		return TRUE;
	else
		return CDialog::PreTranslateMessage(pMsg);
}

//ocx의 포인터를 return
CWnd* CInitializeControlDlg::GetOcxWnd()
{
	return m_pOcxWnd;
}
