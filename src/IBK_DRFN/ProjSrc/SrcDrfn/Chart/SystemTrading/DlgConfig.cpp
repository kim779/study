// DlgConfig.cpp : implementation file
//

#include "stdafx.h"
#include "systemtrading.h"
#include "DlgConfig.h"

#include "DlgEditorConfig.h"
#include "../Include_ST/DeclareDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx

CTabCtrlEx::CTabCtrlEx()
{
	// TODO: add construction code here.

}

CTabCtrlEx::~CTabCtrlEx()
{
	// TODO: add destruction code here.

}

IMPLEMENT_DYNAMIC(CTabCtrlEx, CXTTabCtrl)

BEGIN_MESSAGE_MAP(CTabCtrlEx, CXTTabCtrl)
	//{{AFX_MSG_MAP(CTabCtrlEx)
	ON_COMMAND(XT_IDC_TAB_CLOSE, OnCloseTab)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlEx message handlers

void CTabCtrlEx::OnCloseTab()
{
	if ( GetItemCount( ) == 1 )
	{
//		AfxMessageBox( IDS_CANNOTDEL );
	}
	else
	{
		DeleteView( GetCurSel( ) );
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig dialog


CDlgConfig::CDlgConfig(UINT nScriptType, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConfig::IDD, pParent)
	, m_nScriptType(nScriptType)
{
	//{{AFX_DATA_INIT(CDlgConfig)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pDlgTradeStation = NULL;
	m_pDlgYesTrader = NULL;
}


void CDlgConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgConfig)
	DDX_Control(pDX, IDC_TAB_CTRL, m_tabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgConfig)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgConfig message handlers

BOOL CDlgConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_tabCtrl.SendInitialUpdate(TRUE);
	m_tabCtrl.SetPadding(CSize(6, 3));

	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_IMAGELIST);

	m_imageList.Create(16, 16, ILC_COLOR24|ILC_MASK, 4, 1);
	m_imageList.Add(&bitmap, RGB(255,0,0));

	// Create the image list used by the tab control.
/*
	if (!m_imageList.Create( IDB_IMAGELIST, 16, 1, RGB( 0x00,0xFF,0x00 )))
	{
		TRACE0("Failed to create image list.\n");
		return -1;
	}
*/

	// Set the tab controls image list.
	m_tabCtrl.SetImageList(&m_imageList);

	// Set the tab to auto condense (shrink) its tabs when sized.
//	m_tabCtrl.SetAutoCondense(m_bAutoCondense);

	// Clip children of the tab control from paint routines to reduce flicker.
	m_tabCtrl.ModifyStyle(0L,
		WS_CLIPCHILDREN|WS_CLIPSIBLINGS);

	ForceRefreshTabCtrl();

	m_pFont = new CFont;

	int x = 0, y = 15, nWeight = 400;
	m_pFont->CreateFont(y, x, 0, 0, nWeight, FALSE, FALSE, 0,
			DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
			CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
			FF_ROMAN, "±¼¸²Ã¼");

	CreateDlgEditor();

	m_tabCtrl.SetCurSel(m_nScriptType);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgConfig::CreateDlgEditor()
{
	m_pDlgTradeStation = new CDlgEditorConfig(m_pFont, _T("TRADESTATION"), ST_TRADESTATION, this);
	if(!m_pDlgTradeStation->Create(CDlgEditorConfig::IDD, this))
	{
		delete m_pDlgTradeStation;
		m_pDlgTradeStation = NULL;

		return;
	}

	m_tabCtrl.AddControl(_T("Trade Station"), m_pDlgTradeStation, 0, 0);

	m_pDlgYesTrader = new CDlgEditorConfig(m_pFont, _T("YESTRADER"), ST_YESTRADER, this);
	if(!m_pDlgYesTrader->Create(CDlgEditorConfig::IDD, this))
	{
		delete m_pDlgYesTrader;
		m_pDlgYesTrader = NULL;

		return;
	}

	m_tabCtrl.AddControl(_T("Yes Trader"), m_pDlgYesTrader, 1, 1);

	if(m_nScriptType == ST_TRADESTATION)
	{
		m_pDlgYesTrader->ShowWindow(SW_HIDE);
		m_pDlgTradeStation->ShowWindow(SW_SHOW);
		
		m_pDlgYesTrader->EnableWindow(FALSE);
		m_pDlgTradeStation->EnableWindow(TRUE);
	}
}

void CDlgConfig::ForceRefreshTabCtrl()
{
	::SetWindowPos(m_tabCtrl.m_hWnd, NULL, 0,0,0,0,
		SWP_NOMOVE|SWP_NOSIZE|SWP_FRAMECHANGED);
	m_tabCtrl.Invalidate(FALSE);

/*
	UpdateData();
	BOOL bEnable = !m_bVertical && xtAfxData.bXPMode && !m_bAutoCondense && !m_bMultiLine;

	GetDlgItem(IDC_ARROWS)->EnableWindow(bEnable);
	GetDlgItem(IDC_CLOSE)->EnableWindow(bEnable);
*/
}

void CDlgConfig::OnDestroy() 
{
	CDialog::OnDestroy();
	
	m_pFont->DeleteObject();
	delete m_pFont;

	if(m_pDlgTradeStation)
	{
		m_pDlgTradeStation->DestroyWindow();
		delete m_pDlgTradeStation;
		m_pDlgTradeStation = NULL;
	}
	
	if(m_pDlgYesTrader)
	{
		m_pDlgYesTrader->DestroyWindow();
		delete m_pDlgYesTrader;
		m_pDlgYesTrader = NULL;
	}
}

void CDlgConfig::OnOK() 
{
	if(m_pDlgTradeStation)
		m_pDlgTradeStation->SaveData();

	if(m_pDlgYesTrader)
		m_pDlgYesTrader->SaveData();

	CDialog::OnOK();
}
