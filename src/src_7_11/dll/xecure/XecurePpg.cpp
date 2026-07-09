// XecurePpg.cpp : Implementation of the CXecurePropPage property page class.

#include "stdafx.h"
#include "Xecure.h"
#include "XecurePpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CXecurePropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CXecurePropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CXecurePropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CXecurePropPage, "AxisZecure.XecurePropPage.1",
	0x4e9849a9, 0x6eca, 0x4a6a, 0xa7, 0x23, 0x70, 0xb0, 0xb, 0xe0, 0xf9, 0x70)


/////////////////////////////////////////////////////////////////////////////
// CXecurePropPage::CXecurePropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CXecurePropPage

BOOL CXecurePropPage::CXecurePropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_XECURE_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CXecurePropPage::CXecurePropPage - Constructor

CXecurePropPage::CXecurePropPage() :
	COlePropertyPage(IDD, IDS_XECURE_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CXecurePropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CXecurePropPage::DoDataExchange - Moves data between page and properties

void CXecurePropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CXecurePropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CXecurePropPage message handlers
