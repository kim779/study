// SockPpg.cpp : Implementation of the CSockPropPage property page class.

#include "stdafx.h"
//#include "Sock.h"
#include "SockPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CSockPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CSockPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CSockPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CSockPropPage, "AxisSock.SockPropPage.KTB64",
	0x8833ef0e, 0xe690, 0x470f, 0xb3, 0xdd, 0x72, 0xa7, 0xfd, 0x13, 0xa4, 0x6e)


/////////////////////////////////////////////////////////////////////////////
// CSockPropPage::CSockPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CSockPropPage

BOOL CSockPropPage::CSockPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_SOCK_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CSockPropPage::CSockPropPage - Constructor

CSockPropPage::CSockPropPage() :
	COlePropertyPage(IDD, IDS_SOCK_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CSockPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CSockPropPage::DoDataExchange - Moves data between page and properties

void CSockPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CSockPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CSockPropPage message handlers
