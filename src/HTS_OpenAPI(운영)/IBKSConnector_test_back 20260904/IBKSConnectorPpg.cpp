// IBKSConnectorPpg.cpp : Implementation of the CIBKSConnectorPropPage property page class.

#include "stdafx.h"
#include "IBKSConnector.h"
#include "IBKSConnectorPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CIBKSConnectorPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CIBKSConnectorPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CIBKSConnectorPropPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CIBKSConnectorPropPage, "IBKSCONNECTOR.IBKSConnectorPropPage.1",
	0x5241d43a, 0x77d7, 0x4bd1, 0x82, 0x46, 0x37, 0x37, 0xed, 0xb2, 0x63, 0xbd)


/////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorPropPage::CIBKSConnectorPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CIBKSConnectorPropPage

BOOL CIBKSConnectorPropPage::CIBKSConnectorPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_IBKSCONNECTOR_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorPropPage::CIBKSConnectorPropPage - Constructor

CIBKSConnectorPropPage::CIBKSConnectorPropPage() :
	COlePropertyPage(IDD, IDS_IBKSCONNECTOR_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CIBKSConnectorPropPage)
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorPropPage::DoDataExchange - Moves data between page and properties

void CIBKSConnectorPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CIBKSConnectorPropPage)
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CIBKSConnectorPropPage message handlers
