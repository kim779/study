// ExDialog.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExDialog.h"
#include "../../Inc/ExControls.h"
#include "../../Inc/IMainExInfoMng.h"

//#include "c:\Program Files\Microsoft Visual Studio\VC98\MFC\SRC\winhand_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CExDialog dialog

IMPLEMENT_DYNCREATE(CExDialog, CDialog)

CExDialog::CExDialog()
	: CExResize(NULL, 0),
	m_pIPCTRManager(NULL),
	m_pITrCommManager(NULL), m_pITrComm(NULL),
	m_pISkinColorManager(NULL),	m_lSkinSiteKey(0),
	m_pIAUDrdsManager(NULL), m_pIDrdsLib(NULL),
	m_pIMasterDataManager(NULL), m_pGDIManager(NULL)
{
	m_hBrBackground = NULL;
	m_pPcTrHelper = NULL;
}

CExDialog::CExDialog(const UINT nID, CWnd* pParent, const UINT nFlags) : 
	CDialog(nID, pParent),
	CExResize(this, nFlags),
	m_pIPCTRManager(NULL),
	m_pITrCommManager(NULL), m_pITrComm(NULL),
	m_pISkinColorManager(NULL), m_lSkinSiteKey(0),
	m_pIAUDrdsManager(NULL), m_pIDrdsLib(NULL),
	m_pIMasterDataManager(NULL), m_pGDIManager(NULL)
{
	m_nDialogID = nID;
	m_hBrBackground = NULL;
	m_pPcTrHelper = NULL;
}

BEGIN_MESSAGE_MAP(CExDialog, CDialog)
	//{{AFX_MSG_MAP(CExDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExDialog message handlers
BOOL CExDialog::OnInitDialog() 
{
	// Only show the resize grip if the frame is a resizing frame
	if ( !m_pPcTrHelper )
		m_pPcTrHelper = new CPCTrMngHelper;
	DWORD dwStyle = ::GetWindowLong(m_hWnd, GWL_STYLE);
	if ((dwStyle & WS_THICKFRAME) == 0) {
		SetFlag(SZ_NOSIZEICON);
	}

	CDialog::OnInitDialog();
	CExResize::Init();

	// Enable tool tips for this dialog
	EnableToolTips();

	AfxGetIGateManager(m_pIGateManager);

	::SetPermanentHandle(m_hWnd, this);

	return TRUE;
}

void CExDialog::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CExResize::GetMinMaxInfo(pMMI);
}

void CExDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CExResize::Size();
}

void CExDialog::OnDestroy() 
{
	// Save window placement if AutoLoadPlacement was used.
	if (!m_strSection.IsEmpty()) {
		SavePlacement(m_strSection);
	}

	if(m_hBrBackground)
	{
		::DeleteObject(m_hBrBackground);
		m_hBrBackground = NULL;
	}

	if(m_pITrCommManager && m_pITrComm)
		m_pITrCommManager->UnAdviseTrComm(m_pITrComm);

	if(m_pISkinColorManager && m_lSkinSiteKey)
		m_pISkinColorManager->UnadviseSite(m_lSkinSiteKey);

	if(m_pIAUDrdsManager && m_pIDrdsLib)
		m_pIAUDrdsManager->UnadviseDrdsLib(m_pIDrdsLib);

	if(m_pPcTrHelper)
	{
		m_pPcTrHelper->NotifyPCTrState(NotifyPCTrW_AppState, PCTrMng_ExitInstance);
		delete m_pPcTrHelper;
		m_pPcTrHelper = NULL;
	}
	CDialog::OnDestroy();
}

void CExDialog::SetBaseInterface(UINT nFlag/* = EXDLG_INTERFACE_ALL*/)
{
	if(!nFlag)
		return;

	int nID = 0;
	if(nFlag & EXDLG_INTERFACE_TRCOMM)
	{
		nID = 2;
		m_pITrCommManager = (IAUTrCommManager*)m_pIGateManager->GetInterface(nID);
		m_pITrComm = m_pITrCommManager->AdviseTrComm();
		m_pITrComm->ChangeCallback(&m_xMyITrCommSite);
	}

	if(nFlag & EXDLG_INTERFACE_PCTR)
	{
		nID = 1;
		m_pIPCTRManager = (IPCTRManager*)m_pIGateManager->GetInterface(nID);
	}

	if(nFlag & EXDLG_INTERFACE_SKIN || nFlag & EXDLG_INTERFACE_DEFSKIN)
	{
		nID = 6;
		m_pISkinColorManager = (ISkinColorManager*)m_pIGateManager->GetInterface(nID);

		if(nFlag & EXDLG_INTERFACE_DEFSKIN)
			SetBackgroundColor(m_pISkinColorManager->GetDefaultColor(1));
		else
		{
			m_lSkinSiteKey = m_pISkinColorManager->AdviseSite(&m_xMyISkinColorSite);
			SetBackgroundColor(m_pISkinColorManager->GetColor(1));
		}
	}

	if(nFlag & EXDLG_INTERFACE_DRDS)
	{
		nID = 19;
//		m_pIAUDrdsManager = (IAUDrdsManager*)m_pIGateManager->GetInterface(nID);
//		m_pIDrdsLib = m_pIAUDrdsManager->AdviseDrdsLib();
	}

	if(nFlag & EXDLG_INTERFACE_MASTER)
	{
		nID = 5;
		m_pIMasterDataManager = (IMasterDataManager*)m_pIGateManager->GetInterface(nID);
	}

	if(nFlag & EXDLG_INTERFACE_GDI)
	{
		nID = 4;
//		m_pGDIManager = (IAUGDIManager*)m_pIGateManager->GetInterface(nID);
	}
}

void CExDialog::NotifyToCtrlColorTableChanged()
{
	POSITION pos = m_listChangeSkinNotify.GetHeadPosition();
	while(pos)
	{
		FpChangeSkinNotify pChangeSkinNotify = m_listChangeSkinNotify.GetNext(pos);
		pChangeSkinNotify();
	}
}

POSITION CExDialog::SetChangeSkinNotifyFunction(FpChangeSkinNotify pChangeSkinNotify)
{
	POSITION posReturn = m_listChangeSkinNotify.Find(pChangeSkinNotify);
	if(posReturn)
		return posReturn;

	return m_listChangeSkinNotify.AddTail(pChangeSkinNotify);
}

void CExDialog::RemoveChangeSkinNotifyFunction(POSITION posFunction)
{
	if(!posFunction)
		return;

	m_listChangeSkinNotify.RemoveAt(posFunction);
}

HBRUSH CExDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = NULL;
	
	if(nCtlColor == CTLCOLOR_STATIC)
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CExStatic)) || pWnd->IsKindOf(RUNTIME_CLASS(CExCheckBox)))
			return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

		if(m_hBrBackground)
			pDC->SetBkColor(m_clrBackground);

		hbr = m_hBrBackground;
	}
	else
		hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

BOOL CExDialog::OnEraseBkgnd(CDC* pDC) 
{
	if(m_hBrBackground)
	{
		CRect rcClient;
		GetClientRect(rcClient);

		::FillRect(pDC->GetSafeHdc(), rcClient, m_hBrBackground);
		return TRUE;
	}
	
	return CDialog::OnEraseBkgnd(pDC);
}