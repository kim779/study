// ItemConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB100400.h"
#include "ItemConfigDlg.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemConfigDlg dialog


CItemConfigDlg::CItemConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pRemainGrid = NULL;
	m_pMain = pParent;
}


void CItemConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemConfigDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CItemConfigDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemConfigDlg message handlers
void CItemConfigDlg::CreateGrid()
{
	CRect rect;
	GetClientRect(&rect);
	rect.DeflateRect(1, 25, 0, 0); 
	rect.bottom = 130;
	rect.right = CONFIGWINDOW_W;
	m_pRemainGrid = new CfxGrid;
	m_pRemainGrid->Create(rect, this, IDC_CONFIGGRID+10, GVSC_BOTH, GVDD_NONE);
	m_pRemainGrid->Initial(1, CONFIG_JCOLCOUNT, 1, 0);
	((CMainWnd*)m_pMain)->SetGridDefaultProperty(m_pRemainGrid); 
	insertGridRow(0, true);
}

int CItemConfigDlg::insertGridRow(int row, bool bheader)
{
	if(!m_pRemainGrid) return -1;
	GVITEM	item;
	if( m_pRemainGrid->GetRowCount() <= row)
		row = m_pRemainGrid->InsertRow(_T(""), row);

	LOGFONT	lf = GetLogFont(90);
	item.row = row;
	int icolor = row % 2 ;

	for (int ii = 0; ii < CONFIG_JCOLCOUNT; ii++)
	{
		item.col    = ii;
		item.mask   = GVMK_FORMAT|GVMK_ATTR|GVMK_FONT|GVMK_FGCOLOR|GVMK_BKCOLOR;
		item.format = gridHdr1[ii].format;
		item.attr   = gridHdr1[ii].attr|gridHdr1[ii].attrx|GVAT_ONLYROW;
		if(bheader)
		{
			item.format |= GVFM_HEADER;
			item.attr   = gridHdr1[ii].attr;
			item.bkcol  = ((CMainWnd*)m_pMain)->m_gridhdrbgcolor;
			item.fgcol  = ((CMainWnd*)m_pMain)->m_gridhdrtxtcolor;
		}
		else
		{
			item.bkcol  = ((CMainWnd*)m_pMain)->m_gridRowColor[icolor];
			item.fgcol  = ((CMainWnd*)m_pMain)->m_txtcolor;
		}
		
		CopyMemory(&item.font, &lf, sizeof(LOGFONT));
		m_pRemainGrid->SetItem(&item);
		m_pRemainGrid->SetColumnWidth(ii, gridHdr1[ii].width);

		if(bheader)
			m_pRemainGrid->SetItemText(row, ii, gridHdr1[ii].szhdr);
	}

	m_pRemainGrid->SetRowHeight(row, GRIDROWHEIGHT);
	return row;
}

BOOL CItemConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CreateGrid();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemConfigDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CItemConfigDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}
