// PopupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB202200.h"
#include "PopupDlg.h"

#include "../../control/fx_misc/fxImgButton.h"
#define WM_MANAGE (WM_USER + 1000)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg dialog


CPopupDlg::CPopupDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(CPopupDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_nX = 0;
	m_nY = 0;
	m_nResult = -1;
	m_nCurSel = 0;
}


void CPopupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupDlg, CDialog)
	//{{AFX_MSG_MAP(CPopupDlg)
	ON_WM_DESTROY()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(100, 250, OnCommandBtn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupDlg message handlers

BOOL CPopupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	const int nSize = m_arrMenuID.GetSize();

	CRect rcWnd;
	GetWindowRect(rcWnd);
	const int nHeight = nSize * 20 + GetSystemMetrics(SM_CYCAPTION) + GetSystemMetrics(SM_CXBORDER) * 2;

	SetWindowPos(nullptr, m_nX, m_nY, rcWnd.Width(), nHeight, SWP_NOZORDER);

	CRect rcBtn;
	rcBtn.left   = 0;
	rcBtn.right  = rcWnd.Width();
	rcBtn.top    = 0;
	rcBtn.bottom = 20;

	CFont* pFont = GetFont();

	for (int i = 0; i < nSize; i++)
	{
		rcBtn.top    = 20 * i;
		rcBtn.bottom = 20 * (1 + i);

		auto& Btn = _vBtn.emplace_back(std::make_unique<CButton>());
		Btn->Create(m_arrMenuString.GetAt(i), BS_OWNERDRAW | WS_CHILD | WS_VISIBLE, rcBtn, this, m_arrMenuID.GetAt(i));

		if (pFont)
		{
			Btn->SetFont(pFont, FALSE);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopupDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	_vBtn.clear();
}

void CPopupDlg::OnCommandBtn(UINT nID)
{
	m_nResult = nID;
	OnOK();
}

void CPopupDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);          //Get device context object
	CRect rt;
	rt = lpDrawItemStruct->rcItem;

	int i{};
	const int nSize = m_arrMenuID.GetSize();
	for (i = 0; i < nSize; i++)
	{
	   if (nIDCtl == (int)m_arrMenuID.GetAt(i))
		   break;
	}

	CString strTemp = "   ";

	if (m_arrMenuCheck.GetAt(i))
		strTemp = "▶ ";

	// 일반 상황  
	if((lpDrawItemStruct->itemAction & ODA_DRAWENTIRE)
		|| (lpDrawItemStruct->itemAction & ODA_SELECT))  // 일반 상황
	{
		dc.FillSolidRect(rt, RGB(255, 255, 255));
		dc.SetTextColor(RGB(0,0,0));
		dc.DrawText(strTemp + m_arrMenuString.GetAt(i), rt, DT_LEFT | DT_VCENTER | DT_SINGLELINE); 
	}
}

