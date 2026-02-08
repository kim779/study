// NewsViewer.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "NewsViewer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsViewer dialog


CNewsViewer::CNewsViewer(CWnd* pParent /*=NULL*/)
	: CDialog(CNewsViewer::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsViewer)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CNewsViewer::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsViewer)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewsViewer, CDialog)
	//{{AFX_MSG_MAP(CNewsViewer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsViewer message handlers
