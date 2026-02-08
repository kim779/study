// ConditionView.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ConditionView.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern HINSTANCE g_hInstance;

/////////////////////////////////////////////////////////////////////////////
// CConditionView

IMPLEMENT_DYNCREATE(CConditionView, CView)

CConditionView::CConditionView()
{
	m_bInitFlag = FALSE;
	
	m_bTopExp = FALSE;
	
	m_pwndConditionResult = NULL;
	m_pwndConditionMake = NULL;

	m_bExtendCommonLogicEdit = FALSE;
	m_nTapSelect = 0;
}

CConditionView::~CConditionView()
{
	if (m_pwndConditionResult != NULL)
	{
		delete m_pwndConditionResult;
		m_pwndConditionResult = NULL;
	}
	
	if (m_pwndConditionMake != NULL)
	{
		delete m_pwndConditionMake;
		m_pwndConditionMake = NULL;
	}
}

BEGIN_MESSAGE_MAP(CConditionView, CView)
//{{AFX_MSG_MAP(CConditionView)
ON_WM_CREATE()
ON_WM_DESTROY()
ON_WM_SIZE()
ON_WM_PAINT()
//}}AFX_MSG_MAP
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConditionView drawing

void CConditionView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CConditionView diagnostics

#ifdef _DEBUG
void CConditionView::AssertValid() const
{
	CView::AssertValid();
}

void CConditionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CConditionView message handlers

int CConditionView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	HINSTANCE hInstSave = AfxGetResourceHandle();
	AfxSetResourceHandle(g_hInstance);
		
	m_pwndConditionResult = new CConditionResultWnd;
	m_pwndConditionResult->Create(_T("STATIC"),
		_T(""), WS_CHILD | WS_VISIBLE,
		CRect(0, 0, 0, 0), this,
		IDC_CONDITIONRESULT_WND);
	
	m_pwndConditionMake = new CConditionMakeWnd;
	m_pwndConditionMake->Create(_T("STATIC"), 
								_T(""), WS_CHILD | WS_VISIBLE,
								CRect(0, 0, 0, 0), this,
								IDC_CONDITIONMAKE_WND);
	
	AfxSetResourceHandle(hInstSave);
	
	return 0;
}

void CConditionView::OnDestroy() 
{
	if (m_pwndConditionResult != NULL)
	{
		if(m_pwndConditionResult->m_hWnd)
			m_pwndConditionResult->DestroyWindow();
		
		delete m_pwndConditionResult;
		m_pwndConditionResult = NULL;
	}
	
	if (m_pwndConditionMake != NULL)
	{
		if(m_pwndConditionMake->m_hWnd)
			m_pwndConditionMake->DestroyWindow();
		
		delete m_pwndConditionMake;
		m_pwndConditionMake = NULL;
	}
	
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

void CConditionView::SetInit()
{
	if (m_pwndConditionMake != NULL)
		m_pwndConditionMake->SetInit();
	
	if (m_pwndConditionResult != NULL)
		m_pwndConditionResult->SetInit();
	
	m_bInitFlag = TRUE;
}

void CConditionView::SetTopExpend(int nState)
{
	/*m_bTopExp = nState;*/
	m_nTopExp = nState;

	m_pwndConditionMake->m_ctrlMapLoad.SendMessage(CHANGE_EXP, 0, (LPARAM)m_nTopExp);
	OnSize();
}

void CConditionView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	OnSize();
}

void CConditionView::OnSize()
{
	if (m_bInitFlag == FALSE)
		return;
	
	CRect reClient;
	GetClientRect(reClient);
	
	/*reClient.top += 3;*/
	/*reClient.bottom -= 3;*/
	/*reClient.left += 3;*/
	/*reClient.right -= 3;*/
	reClient.left += 1;
	
	CRect reLeft, reTemp;
	reLeft = reClient;
	
	CWnd* pMainFrmWnd = GetParent()->GetParent();
	/*if (m_bTopExp == TRUE)*/
	// 2번 화면
	if(m_nTopExp == 1)
	{
		if (m_pwndConditionResult->GetSafeHwnd() != NULL)
			m_pwndConditionResult->MoveWindow(&reLeft);
		
		if (m_pwndConditionMake->GetSafeHwnd() != NULL)
			m_pwndConditionMake->ShowWindow(SW_HIDE);
		
		//유진 컨트롤 관련
		if (m_pwndConditionMake->GetSafeHwnd() != NULL)
		{

		}
	}
	else
	{
		int nConditionResult_Height;
		
		if(m_pwndConditionMake->m_nSeparateForm)
		{
			nConditionResult_Height = ((reClient.bottom - reClient.top) / 3) * 2 + 60;
		}
		else
		{
			nConditionResult_Height = (reClient.bottom - reClient.top) / 3;
		}
		
		if(m_nTopExp == 0)  // 3번 화면
		{
			nConditionResult_Height -= 30;
		}
		else // 1번 화면
		{
			nConditionResult_Height += 30;
		}
		
		reLeft.top = reLeft.bottom - nConditionResult_Height;	
		
		if (m_pwndConditionResult->GetSafeHwnd() != NULL)
		{
			if(m_bExtendCommonLogicEdit && m_nTapSelect == 0)
				reLeft.top += 18;
			
			m_pwndConditionResult->MoveWindow(&reLeft);
		}
		
		reLeft.bottom = reLeft.top - 2;
		reLeft.top = reClient.top;
		if (m_pwndConditionMake->GetSafeHwnd() != NULL)
		{
// 			if(m_bExtendCommonLogicEdit && m_nTapSelect == 0)
// 				reLeft.bottom += 18;
			
			m_pwndConditionMake->MoveWindow(&reLeft);
			m_pwndConditionMake->ShowWindow(SW_SHOW);
		}
		
		//유진 컨트롤 관련
		if (m_pwndConditionMake->GetSafeHwnd() != NULL)
		{	
			if(m_pwndConditionMake->m_ctrlMapLoad.GetBaseBtnWnd())
			{		
				
			}
			else
			{
				
			}			
		}
	}
	
	Invalidate();
}

void CConditionView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
}

void CConditionView::ChangeSkinColor(COLORREF clrSkin, ST_TABCOLOR stTabColor)
{
	if(clrSkin)
		m_clrMainSkin = clrSkin;

	if(m_pwndConditionMake)
		m_pwndConditionMake->ChangeSkinColor(m_clrMainSkin, stTabColor);
	if(m_pwndConditionResult)
		m_pwndConditionResult->ChangeSkinColor(m_clrMainSkin);

	Invalidate();
}

void CConditionView::OnExtendLigcEdit(BOOL bExtendCommonLogicEdit, int nTapSelect)
{
	m_bExtendCommonLogicEdit = bExtendCommonLogicEdit;
	m_nTapSelect = nTapSelect;
	OnSize();
}

//20100827 이문수 >>
BOOL CConditionView::OnEraseBkgnd(CDC* pDC)
{
	CRect reClient;
	GetClientRect(reClient);	
	pDC->FillSolidRect(reClient, m_clrMainSkin);	
	return FALSE;
}
//20100827 이문수 <<
