// KTBChartPpg.cpp : Implementation of the CKTBChartPropPage property page class.

#include "stdafx.h"
#include "KTBChart.h"
#include "KTBChartPpg.h"

#include "OcxIDs.h"				// for _OCX_PP_IMPLEMENT_OLECREATE_EX()

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CKTBChartPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CKTBChartPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CKTBChartPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

_OCX_PP_IMPLEMENT_OLECREATE_EX()

/////////////////////////////////////////////////////////////////////////////
// CKTBChartPropPage::CKTBChartPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CKTBChartPropPage

BOOL CKTBChartPropPage::CKTBChartPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_KTBCHART_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CKTBChartPropPage::CKTBChartPropPage - Constructor

CKTBChartPropPage::CKTBChartPropPage() :
	COlePropertyPage(IDD, IDS_KTBCHART_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CKTBChartPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CKTBChartPropPage::DoDataExchange - Moves data between page and properties

void CKTBChartPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CKTBChartPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CKTBChartPropPage message handlers
