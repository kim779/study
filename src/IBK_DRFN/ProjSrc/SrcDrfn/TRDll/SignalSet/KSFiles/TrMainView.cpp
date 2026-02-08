// TrMainView.cpp : implementation file
//

#include "stdafx.h"
#include "TrMainView.h"

#include "MainFrmWnd.h"
#include "FilterSetWnd.h"
#include "../../contents/inc/IKSLibMng.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_FORM			999
/////////////////////////////////////////////////////////////////////////////
// CTrMainView

IMPLEMENT_DYNCREATE(CTrMainView, CFormView)

CTrMainView::CTrMainView()
: CFormView(CTrMainView::IDD)
{
	//{{AFX_DATA_INIT(CTrMainView)
	m_nInitFlag     = 0;
	//}}AFX_DATA_INIT
	
	m_pwndMainFrm = NULL;
}

CTrMainView::~CTrMainView()
{
}

void CTrMainView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrMainView)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrMainView, CFormView)
//{{AFX_MSG_MAP(CTrMainView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_DESTROY()

	ON_WM_PAINT()
//}}AFX_MSG_MAP
	ON_MESSAGE(UM_CHANGE_PARENT, OnChangeChildFrameWindowHandle)
	ON_REGISTERED_MESSAGE(RMSG_GET_SKINCOLOR, OnGetSkinColor)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CTrMainView, CFormView)
//{{AFX_EVENTSINK_MAP(CMainFrmWnd)
ON_EVENT(CTrMainView, IDC_CODEINPUT,		1 ,	OnEditEnter,			VTS_NONE)
ON_EVENT(CTrMainView, IDC_CODEINPUT,		2 ,	OnEditEnter,			VTS_NONE)
ON_EVENT(CTrMainView, IDC_CODEINPUT,		4 ,	OnEditEnter,			VTS_NONE)

//	ON_EVENT(CTrMainView, IDD_STD_DIALOG,	2 /* OnInitAfter */, OnFormInitAfter, VTS_NONE)
ON_EVENT(CTrMainView, IDD_STD_DIALOG,	2 /* OnInitAfter */, OnFormInitAfter, VTS_NONE)
ON_EVENT(CTrMainView,	ID_FORM,	0xb /* OnGetPageSave */,	OnFormGetPageSave,	VTS_BSTR)
ON_EVENT(CTrMainView,	ID_FORM,	0xc /* OnSetPageSave */,	OnFormSetPageSave,	VTS_BSTR)
ON_EVENT(CTrMainView,	ID_FORM,	0x1f, OnColorChange , VTS_NONE)
//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrMainView diagnostics

#ifdef _DEBUG
void CTrMainView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTrMainView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTrMainView message handlers

void CTrMainView::OnEditEnter()
{
	if (m_pwndMainFrm != NULL)
	{
		if (m_pwndMainFrm->m_pwndFilterSet->m_ctrlJMCombo.CheckCaption() == TRUE)
			m_pwndMainFrm->m_pwndFilterSet->SetAdd_OutCode();
	}
}

int CTrMainView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	
	
	//	m_crBackColor = RGB(255, 255, 255);
	
	return 0;
}

void CTrMainView::OnCreate_MainWnd()
{
	int nWidth, nHeight;
	nWidth = 720;
	nHeight = 424;
	
	m_pwndMainFrm = new CMainFrmWnd;
	
	CString strData;
	//# 1:HTS  0:FRONT(지점)

//--> HTS구분없앰. 사용하면 추후 다시
//	strData = KSLink_GetLoginInfo(ISHTS);
//	BOOL bIsHTS = FALSE;
//	if (strData.Left(1) == "1")
//		bIsHTS = TRUE;
	
	//strData = KSLink_GetLoginInfo(SUPER_USERLEVEL);
	//@유진변경	strData = AfxGetApp()->GetProfileString("STBasic", "UserL",  "");
	//@유진변경	if(strData.GetLength()==0)
	//@유진변경		strData = KSLink_GetLoginInfo(SUPER_USERLEVEL);
//	strData = "S";
	
//	if (bIsHTS == TRUE)
	{
		//# "S", "A", "B", "C", "D"...
//		strData = strData.Left(1);
//		if ((strData == "S") || (strData == "A") || (strData == "B"))
			m_pwndMainFrm->m_bIsFnUser = TRUE;
	}
//	else if (bIsHTS == FALSE)
//	{
//		strData = strData.Left(3);
//		if ((atoi(strData) == 5) || 
//			(atoi(strData) == 6) ||
//			(atoi(strData) == 7) ||
//			(atoi(strData) == 9) ||
//			(atoi(strData) == 10) ||
//			(atoi(strData) == 11) ||
//			(atoi(strData) == 12) ||
//			(atoi(strData) == 13))
//			m_pwndMainFrm->m_bIsFnUser = TRUE;
//	}
//<--
	
	BOOL bResult = m_pwndMainFrm->Create(
		NULL,
		_T("Main Frame Window"),
		WS_CHILD | WS_CLIPSIBLINGS,
		CRect(0, 0, nWidth, nHeight),
		this,
		NULL);
	m_pwndMainFrm->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	if (bResult)
	{
		m_pwndMainFrm->ShowWindow(SW_SHOW);
		SetWindowPos(NULL, 0, 0, nWidth, nHeight, SWP_NOZORDER);
	}
	
	
}

void CTrMainView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	if (m_pwndMainFrm != NULL)
	{
		CRect reClient;
		GetClientRect(&reClient);
		m_pwndMainFrm->MoveWindow(&reClient);
	}
}

void CTrMainView::OnInitialUpdate() 
{	
	CFormView::OnInitialUpdate();	
	
	if (m_nInitFlag == 0)
		SetInit_MainWnd();
	
	m_nInitFlag++;
}

void CTrMainView::OnFormInitAfter()
{
	
}

void CTrMainView::SetInit_MainWnd()
{
	if (m_pFormDisp)
		m_lRunForm.AttachDispatch(m_pFormDisp, FALSE);
		
	OnCreate_MainWnd();
	
	IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
	if(pKSMng)
		m_crBackColor = pKSMng->Lib_KSLink_GetColor(1);
	
	if (m_pwndMainFrm != NULL)
	{
		m_pwndMainFrm->SetInit(m_MapControlName);
		//	m_pwndMainFrm->ChangeSkinColor();
	}

	// DoDllSizeEx();
}

void CTrMainView::OnDestroy() 
{
	CFormView::OnDestroy();
	
}

void CTrMainView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CBrush BackBrush(m_crBackColor);

	CRect rect;
	dc.GetClipBox(&rect);
	dc.FillRect(rect, &BackBrush);	

	
	BackBrush.DeleteObject();
	// Do not call CFormView::OnPaint() for painting messages
}

void CTrMainView::SetCloseScreen()
{
	m_lRunForm.CloseScreen();
}

// 메인의 저장화면관련.
void CTrMainView::OnFormGetPageSave(LPCTSTR strFileName)
{
}

// 메인의 저장화면관련.
void CTrMainView::OnFormSetPageSave(LPCTSTR strFileName)
{
}

//메인의 스킨변경시 불린다.
void CTrMainView::OnColorChange()
{
	IKSLibManager*	pKSMng = (IKSLibManager*)AfxGetPctrInterface(UUID_IKSLibManager);
	if(pKSMng)
		m_crBackColor = pKSMng->Lib_KSLink_GetColor(1);
	
	m_pwndMainFrm->ChangeSkinColor();
}

COLORREF CTrMainView::OnGetSkinColor(WPARAM wParam, LPARAM lParam)
{
	int nColorType = (int)wParam;
	if(nColorType == 0)		// 배경색
		return m_crBackColor;
	
	return RGB(255,255,255);
}

//--> DLL화면 사이징처리관련추가
CWnd* CTrMainView::GetFrameWnd(CWnd* pWnd)
{
	CWnd* pParentWnd = pWnd->GetParent();  // start with one parent up
	while (pParentWnd != NULL)
	{
		DWORD dwExStyle = GetWindowLong(pParentWnd->GetSafeHwnd(), GWL_STYLE);
		if ( dwExStyle & 0x8000)
		{
			return pParentWnd;
		}
		pParentWnd = pParentWnd->GetParent();
	}
	return NULL;
}

void CTrMainView::DoDllSizeEx(HWND _hChildFrame)
{
	HWND hChildFrame = _hChildFrame;

	if(hChildFrame==NULL)
	{
		CWnd* pChildFrame = GetFrameWnd(this);
		hChildFrame = pChildFrame->GetSafeHwnd();
	}

	if(hChildFrame)
		::SendMessage(hChildFrame, UM_SET_FORMMAXSIZE, 34, MAKELPARAM(0, 0));
}
//<--

long CTrMainView::OnChangeChildFrameWindowHandle( WPARAM wParam, LPARAM lParam)
{
	//m_hChildFrame = (HWND)lParam;
	SendMessageToDescendants(UM_CHANGE_PARENT, wParam, lParam, TRUE);
	return 0L;
}
