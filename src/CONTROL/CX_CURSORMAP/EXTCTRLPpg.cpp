// EXTCTRLPpg.cpp : Implementation of the CEXTCTRLPropPage property page class.

#include "stdafx.h"
#include "CX_CURSORMAP.h"
#include "EXTCTRLPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CEXTCTRLPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CEXTCTRLPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CEXTCTRLPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CEXTCTRLPropPage, "CXCURSORMAP.CXCURSORMAPPropPage.1",
 0x826bd9c2, 0xe8ca, 0x4dd9, 0xb3, 0xa8, 0x50, 0x40, 0x34, 0x62, 0xce, 0x91)


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLPropPage::CEXTCTRLPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CEXTCTRLPropPage

BOOL CEXTCTRLPropPage::CEXTCTRLPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_EXTCTRL_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLPropPage::CEXTCTRLPropPage - Constructor

CEXTCTRLPropPage::CEXTCTRLPropPage() :
	COlePropertyPage(IDD, IDS_EXTCTRL_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CEXTCTRLPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLPropPage::DoDataExchange - Moves data between page and properties

void CEXTCTRLPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CEXTCTRLPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CEXTCTRLPropPage message handlers
