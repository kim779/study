// CategoryPopup.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Category.h"
#include "CategoryPopup.h"
#include "ControlWnd.h"
#include "dataio.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define IDC_STATIC_ORDER	90
#define IDC_COMBO_ORDER		91
#define	IDC_COMBO_GAP		92
#define IDC_STATIC_QC		93
#define IDC_STATIC_NT		94
#define	IDC_STATIC_GAP		95

#define IDC_CHECK_PLAY		99
#define IDC_CHECK_BGJ		100
#define IDC_CHECK_SSJ		101
#define IDC_CHECK_KOJ		102
#define IDC_CHECK_WSJ		103
#define IDC_CHECK_KOD		104
#define IDC_CHECK_KLJ		105
#define IDC_CHECK_GLJ		106
#define IDC_CHECK_KJJ		107
#define IDC_CHECK_TUJ		108
#define IDC_CHECK_KOP		109

#define IDC_BTN_CONFIG		110
#define IDC_BTN_CLOSE		111
#define IDC_BTN_SEARCH		112

#define IDC_LIST_OUTPUT		120

/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup

#define PT_TREE_H		244
#define PT_TREE_V		155

#define PT_STATIC_Y		13
#define PT_TREE_Y		33
#define PT_CHECK_Y		42

#define PT_TREE_X		11


#define CL_EDGE_POP1	RGB(91,102,132)
#define CL_EDGE_POP2	RGB(229,229,231)
#define CL_EDGE_POP3	RGB(170,186,212)
#define CL_EDGE_POP4	RGB(116,116,116)
#define CL_BK		RGB(211,211,211)
#define CL_BK_GROUP	RGB(235,235,235)
#define CL_TEXT		RGB(100,100,100)
#define CL_EDGE		RGB(152,152,152)
#define CL_TREE_HT	RGB( 0,144,171)
#define CL_TREE_HC	RGB (27,178,207)
#define CL_BK_QR	RGB(106,106,106)
#define CL_BK_HD	RGB(182,182,182)

CCategoryPopup::CCategoryPopup(CControlWnd* pWnd, int Gap)
{
	m_pMainWnd = pWnd;
	m_nKind = 0;
	m_bInitState = true;

	for (int ii = 0 ; ii < CHECKER_COUNT ; ii++)
	{
		m_pCheckBtn[ii] = nullptr;
		if (ii == 0)
			m_rcCheck[ii] = CRect(PT_TREE_V + 17 , PT_CHECK_Y  , PT_TREE_V + 17 + 130 , PT_CHECK_Y + 4+ 15);
		else
			m_rcCheck[ii] = CRect(PT_TREE_V + 17 , m_rcCheck[ii-1].bottom + 4, PT_TREE_V + 17 + 125 , m_rcCheck[ii-1].bottom +4 +15);
	}
	
	for (int ii = 0 ; ii < CHECKER_COUNT ; ii++)
	{
		switch (ii)
		{
		case 2:
		case 3:
		case 5:
		case 6:
		case 7:
		case 8:	m_pMainWnd->m_bCateChecked[ii] = true;	break;
		default:m_pMainWnd->m_bCateChecked[ii] = false;	break;
		}
	}

	m_szCheckName[0] = "불성실공시종목 제외";
	m_szCheckName[1] = "신규상장종목 제외";
	m_szCheckName[2] = "권리락종목 제외";
	m_szCheckName[3] = "우선주 제외";
	m_szCheckName[4] = "코스닥종목 제외";
	m_szCheckName[5] = "관리종목 제외";
	m_szCheckName[6] = "감리종목 제외";
	m_szCheckName[7] = "거래정지종목 제외";
	m_szCheckName[8] = "투자유의종목 제외";
	m_szCheckName[9] = "코스피종목 제외";

	m_pTree = nullptr;
	m_pStatic = nullptr;
	m_pComboOrder = nullptr;

	m_pCloseBtn = nullptr;
	m_pConfigBtn = nullptr;
	m_pFont = nullptr;
	m_pComboGap = nullptr;
	m_pSearchBtn = nullptr;
	m_pQCStatic = nullptr;
	m_pList = nullptr;
	m_pNtStatic = nullptr;
	m_iitem = nullptr;

	m_brBk.CreateSolidBrush(CL_BK);//m_pMainWnd->getAxColor(64));
	m_brSt.CreateSolidBrush(RGB(255,255,255));//m_pMainWnd->getAxColor(66));
	m_brBk1.CreateSolidBrush(CL_BK_GROUP);

	m_Gap = Gap;

	m_elwbase.RemoveAll();
}

CCategoryPopup::~CCategoryPopup()
{
	if (m_bmp1.GetSafeHandle())
		m_bmp1.DeleteObject();
	if (m_bmp2.GetSafeHandle())
		m_bmp2.DeleteObject();

	for (int ii = 0 ; ii < 10 ; ii++)
	{
		if (m_pCheckBtn[ii])
			m_pCheckBtn[ii].reset();
	}
	if (m_pTree)
		m_pTree.reset();
	if (m_pStatic)
		m_pStatic.reset();
	if (m_pComboOrder)
		m_pComboOrder.reset();
	if (m_pComboGap)	
		m_pComboGap.reset();
	if (m_pQCStatic)
		m_pQCStatic.reset();
	if (m_pSearchBtn)
		m_pSearchBtn.reset();
	if (m_pConfigBtn)
		m_pConfigBtn.reset();
	if (m_pCloseBtn)
		m_pCloseBtn.reset();
	if (m_pNtStatic)
		m_pNtStatic.reset();
	if (m_pList)
		m_pList.reset();
}


BEGIN_MESSAGE_MAP(CCategoryPopup, CWnd)
	//{{AFX_MSG_MAP(CCategoryPopup)
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_WM_SIZE()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_CONFIG,OnConfigClk)
	ON_BN_CLICKED(IDC_BTN_CLOSE,OnCloseClk)
	ON_BN_CLICKED(IDC_BTN_SEARCH,OnSearchClk)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_DBLCLK, ID_CX_CategoryTREE, OnTreeDblClk)
	ON_WM_PAINT()
	ON_WM_SIZING()
	ON_WM_KEYDOWN()
	ON_WM_SYSCHAR()
	//}}AFX_MSG_MAP
	ON_CONTROL_RANGE(BN_CLICKED,IDC_CHECK_BGJ,IDC_CHECK_KOP,OnChecked)
	ON_MESSAGE(WM_USER+2, OnEnd)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCategoryPopup message handlers

HBRUSH CCategoryPopup::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	
	if (nCtlColor == CTLCOLOR_STATIC) 
	{
		if (pWnd->GetSafeHwnd() == m_pQCStatic->GetSafeHwnd())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(CL_TEXT);
			return (HBRUSH)m_brSt;
		}
		else if (m_pNtStatic && pWnd->GetSafeHwnd() == m_pNtStatic->GetSafeHwnd())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(20,20,20));
			return (HBRUSH)m_brBk;
		}
		else if (m_chkPlay && pWnd->GetSafeHwnd() == m_chkPlay->GetSafeHwnd())
		{
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(RGB(20,20,20));
			return (HBRUSH)m_brBk;
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(CL_TEXT);
		return (HBRUSH)m_brBk1;
		
	}

	return hbr;
}

BOOL CCategoryPopup::OnEraseBkgnd(CDC* pDC) 
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc,CL_BK);//m_pMainWnd->getAxColor(64));	
	return CWnd::OnEraseBkgnd(pDC);
}

int CCategoryPopup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (m_pMainWnd->m_nLoads== 0)
	{
		m_pMainWnd->LoadHjCode();
	}
	m_pMainWnd->m_nLoads++;

	m_pFont = m_pMainWnd->getAxFont("굴림",9,0);
	InitControls();
	InitListCtrl();

	HTREEITEM hItem = m_pTree->GetFirstVisibleItem();

	m_pTree->SelectItem(hItem);

	CString temp = m_pMainWnd->Variant(historyCC, "1301");

	while (!temp.IsEmpty())
	{
		
		CString temp1 = Parser(temp,"\t").Left(6);
		temp1.TrimRight();
		m_history.Add(temp1);

	}
	return 0;
}

void CCategoryPopup::InitControls()
{
	m_pTree = std::make_unique<CTreeCtrl>();
	m_pTree->Create(WS_VISIBLE |WS_HSCROLL| WS_TABSTOP | WS_CHILD /*| WS_BORDER */| TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES | TVS_DISABLEDRAGDROP |TVS_SHOWSELALWAYS,
		CRect(PT_TREE_X, PT_TREE_Y, PT_TREE_V, PT_TREE_H), this, ID_CX_CategoryTREE);
//	m_pTree->ModifyStyleEx(NULL, WS_EX_CLIENTEDGE);
	baseCX_Category();
	m_pTree->SetFont(GetFont());
	
	m_pStatic = std::make_unique<CStatic>();
	m_pStatic->Create("조회순서", WS_VISIBLE|WS_CHILD , 
		CRect(PT_TREE_V+10, PT_STATIC_Y + 8,PT_TREE_V+10+50,PT_STATIC_Y  + 8 + 11),this, IDC_STATIC_ORDER);
	m_pStatic->SetFont(m_pFont);

	m_pComboOrder = std::make_unique<CSComboBox>();
	m_pComboOrder->Create(WS_VISIBLE|WS_CHILD|WS_TABSTOP
			|WS_VSCROLL|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBS_DROPDOWNLIST|CBS_HASSTRINGS    , 
		CRect(PT_TREE_V+10+50, PT_STATIC_Y + 4, /*PT_TREE_V+10+50 + 130*/m_rcCheck[1].right+50 , PT_STATIC_Y  + 4 + 15), this, IDC_COMBO_ORDER);
	m_pComboOrder->SetFont(m_pFont);
	
	m_pComboOrder->InsertString(0,"종목코드순");
	m_pComboOrder->InsertString(1,"종목코드순(코스피->코스닥)");
	m_pComboOrder->InsertString(2,"종목명순");
	m_pComboOrder->InsertString(3,"종목명순(코스피->코스닥)");
	m_pComboOrder->SetCurSel(0);

	CRect	wRc;
	m_pComboOrder->GetWindowRect(&wRc);
	m_pComboOrder->SetWindowPos(NULL, 0, 0, wRc.Width(), 20*4, SWP_NOMOVE|SWP_SHOWWINDOW);
		
	for (int ii = 0 ; ii < CHECKER_COUNT ; ii++)
	{
		m_pCheckBtn[ii] = std::make_unique<CButton>();
		m_pCheckBtn[ii]->Create(m_szCheckName[ii],WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX,
			m_rcCheck[ii], this, IDC_CHECK_BGJ + ii );
		
//		m_pCheckBtn[ii]->ShowWindow(SW_SHOW);
		m_pCheckBtn[ii]->SetFont(m_pFont);
		m_pCheckBtn[ii]->SetCheck(m_pMainWnd->m_bCateChecked[ii]);
	}

	CString str2n,str2d,str2u,
		str4n,str4d,str4u,
		str11n,str11d,str11u;
	
	str2n = m_pMainWnd->m_sRoot + "\\image\\2btn.bmp";	//"2CBTN.BMP";
	str2d = m_pMainWnd->m_sRoot + "\\image\\2btn_dn.bmp";	//"2CBTN_DN.BMP";
	str2u = m_pMainWnd->m_sRoot + "\\image\\2btn_en.bmp";	//"2CBTN_EN.BMP";

	str4n = m_pMainWnd->m_sRoot + "\\image\\4btn.bmp";	//"4CBTN.BMP";
	str4d = m_pMainWnd->m_sRoot + "\\image\\4btn_dn.bmp";	//"4CBTN_DN.BMP";
	str4u = m_pMainWnd->m_sRoot + "\\image\\4btn_en.bmp";	//"4CBTN_EN.BMP";

	str11n = m_pMainWnd->m_sRoot + "\\image\\8btn.bmp";	//"11CBTN.BMP";
	str11d = m_pMainWnd->m_sRoot + "\\image\\8btn_dn.bmp";	//"11CBTN_DN.BMP";
	str11u = m_pMainWnd->m_sRoot + "\\image\\8btn_en.bmp";	//"11CBTN_EN.BMP";

	CBitmap Bitmap;
	int	cx2 = 0, cx4 = 0, cx11 = 0, cy = 0;

	HBITMAP	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str2n,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{		
		Bitmap.Attach(hBitmap);

		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		cx2 = bm.bmWidth;
		cy = bm.bmHeight;
		Bitmap.Detach();
	}

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str4n,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{		
		Bitmap.Attach(hBitmap);

		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		cx4 = bm.bmWidth;
		cy = bm.bmHeight;
		Bitmap.Detach();
	}

	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str11n,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{		
		Bitmap.Attach(hBitmap);

		BITMAP	bm;
		Bitmap.GetBitmap(&bm);
		cx11 = bm.bmWidth;
		cy = bm.bmHeight;
		Bitmap.Detach();
	}

	m_pSearchBtn = std::make_unique<CxSkinButton>();
	m_pSearchBtn->Create("조회",  WS_VISIBLE | WS_CHILD , 
		CRect(PT_CX-15-cx2, m_rcCheck[9].bottom-18, PT_CX-15, m_rcCheck[9].bottom+2), this, IDC_BTN_SEARCH);
	m_pSearchBtn->SetFont(m_pFont);
	m_pSearchBtn->SetSkin(str2n, str2d, str2u);

	m_pQCStatic = std::make_unique<CSStatic>();
	m_pQCStatic->Create("",WS_VISIBLE | WS_CHILD |SS_CENTER  , 
		CRect(PT_TREE_X, PT_TREE_H  + 26 , PT_TREE_V , PT_TREE_H + 10 + 70), this, IDC_STATIC_QC);
	m_pQCStatic->SetFont(m_pFont);
	
	m_pList = std::make_unique<CExListCtrl>();
	m_pList->Create(WS_CHILD|WS_VISIBLE/*|WS_BORDER*/|LVS_REPORT|WS_HSCROLL|WS_VSCROLL, 
		CRect(PT_TREE_V, PT_TREE_H  + 10 , PT_CX-10/*m_rcCheck[1].right+50*/ , PT_TREE_H + 10 + 70),this, IDC_LIST_OUTPUT);
	m_pList->SetFont(m_pFont);
	
	if (m_pMainWnd->m_bShowPlay)
	{		
		//CString str = "돌려보기를 위해서는 종목코드 옆의 상하버튼을 클릭하시거나, 키보드의 PageUp/PageDown키를 입력하시면 됩니다.";
		CString str = "돌려보기를 위해서는 팝업메뉴의 시작을 선택하시거나, 우측의 상/하버튼을 누르시면 돕니다.";

		m_pNtStatic = std::make_unique<CStatic>();
		m_pNtStatic->Create(str,WS_VISIBLE|WS_CHILD|SS_LEFT,
			CRect(PT_TREE_X + 18, PT_TREE_H + 90, PT_CX-30, PT_TREE_H + 115), this, IDC_STATIC_NT);
		m_pNtStatic->SetFont(m_pFont);

		m_chkPlay = std::make_unique<CButton>();
		m_chkPlay->Create("돌려보기간격", WS_VISIBLE|WS_CHILD|BS_AUTOCHECKBOX|SS_LEFT  , 
			CRect(PT_TREE_X ,PT_TREE_H + 129 ,PT_TREE_X + 90 ,PT_TREE_H + 129+ 15),this, IDC_STATIC_GAP);
		m_chkPlay->SetFont(m_pFont);
		m_chkPlay->SetCheck(0);

		m_pComboGap = std::make_unique<CComboBox>();
		m_pComboGap->Create(WS_VISIBLE|WS_CHILD|WS_TABSTOP
				|WS_VSCROLL|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|CBS_DROPDOWN|CBS_HASSTRINGS    , 
			CRect(PT_TREE_X + 90 ,PT_TREE_H + 125 ,PT_TREE_X + 90 + 40 ,PT_TREE_H + 125+ 100), this, IDC_COMBO_GAP);
		m_pComboGap->SetFont(m_pFont);

		m_pComboGap->AddString("2");
		m_pComboGap->AddString("3");
		m_pComboGap->AddString("4");
		m_pComboGap->AddString("5");
		m_pComboGap->AddString("10");
		str.Format("%d", m_Gap);
		m_pComboGap->SetWindowText(str);

		m_pConfigBtn = std::make_unique<CxSkinButton>();
		m_pConfigBtn->Create("확인", WS_VISIBLE | WS_CHILD ,
			CRect(PT_CX-10-cx2*2 - 5,PT_TREE_H + 125,PT_CX - cx2 - 5 -10 ,PT_TREE_H+ 125+cy),
			this, IDC_BTN_CONFIG);
		m_pConfigBtn->SetFont(m_pFont);
		m_pConfigBtn->SetSkin(str2n,str2d,str2u);
	}

	m_pCloseBtn = std::make_unique<CxSkinButton>();
	if (m_pMainWnd->m_bShowPlay)
	{
		m_pCloseBtn->Create("취소", WS_VISIBLE | WS_CHILD, 
			CRect(PT_CX-10-cx2,PT_TREE_H + 125,PT_CX-10 ,PT_TREE_H+ 125+cy) , this, IDC_BTN_CLOSE);
	}
	else
	{
		m_pCloseBtn->Create("취소", WS_VISIBLE | WS_CHILD,
			CRect(PT_CX-10-cx2,PT_TREE_H + 90,PT_CX-10 ,PT_TREE_H+90+cy), this, IDC_BTN_CLOSE);
	}

	m_pCloseBtn->SetFont(m_pFont);
	m_pCloseBtn->SetSkin(str2n,str2d,str2u);

	CString str = m_pMainWnd->m_sRoot + "\\image\\분류아이콘.bmp";
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)
		m_bmp1.Attach(hBitmap);
	
	str = m_pMainWnd->m_sRoot + "\\image\\경고아이콘.bmp";
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), str,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	
	if (hBitmap)
		m_bmp2.Attach(hBitmap);
}
void CCategoryPopup::InitListCtrl()
{
	CString hData[] = 
	{
		"코드",
		"종목명",
	};
	const	int	col = 2;
	int	width[col]{};
	CRect	rect;

	initBitmap();

	m_pList->GetClientRect(&rect);
	const	int	tot = (int)(rect.Width() / col);
	width[0] = tot - 10;// - GetSystemMetrics(SM_CXHSCROLL);
	width[1] = tot + 10;

	m_pList->InitHeaderData(hData, width, col, LVCFMT_LEFT);
	m_pList->SetFullRowSelect();
}

void CCategoryPopup::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	CRect	rc;
	MSG	msg{};
	bool	bMyHwnd = false;

	for (m_fDone = FALSE; !m_fDone; WaitMessage())
	{
		while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if ((msg.message == WM_KILLFOCUS && msg.hwnd == this->m_hWnd) || 
			   ((msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN) && (msg.hwnd != this->m_hWnd && msg.hwnd != m_pTree->m_hWnd)) ||
			    (msg.message == WM_USER && msg.hwnd == this->m_hWnd))
			{
				bMyHwnd = false;
				for (int ii = 0; ii < CHECKER_COUNT; ii++)
				{			
					if (msg.hwnd == m_pCheckBtn[ii]->m_hWnd)
					{
						bMyHwnd = true;
						break;
					}
				}
				GetWindowRect(&rc);
				if (msg.hwnd == m_pComboOrder->m_hWnd ||
				    msg.hwnd == m_pComboGap->m_hWnd ||
				    msg.hwnd == m_pStatic->m_hWnd) 
				{	
					bMyHwnd = true;	
				}
				else if (m_pComboGap->IsChild(GetFocus()))
				{
					bMyHwnd = true;	
				}
				else if (msg.hwnd != m_pConfigBtn->m_hWnd &&
				    msg.hwnd != m_pCloseBtn->m_hWnd)
				{
					if (rc.PtInRect(msg.pt))
					{
						bMyHwnd = true;
					}
				}
				
				if (msg.hwnd == this->m_hWnd)
				{
					if (m_bInitState)
					{
						bMyHwnd = true;
					}
				}

				if (!bMyHwnd)
				{
					if (msg.message == WM_LBUTTONDOWN || msg.message == WM_NCLBUTTONDOWN)
						::PostMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
					m_fDone = TRUE;
					break;
				}
			}

			if (!IsWindow(m_hWnd))
			{
				m_fDone = TRUE;
				return;
			}
			if (!IsDialogMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		if (m_fDone)
			break;
	}
//	PostMessage(WM_CLOSE);
//	if (IsWindow(m_hWnd))
//		ShowWindow(SW_HIDE);
}

void CCategoryPopup::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized) 
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if (nState == WA_INACTIVE)
		PostMessage(WM_USER, 0, 0);	
}

void CCategoryPopup::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
//	if (m_pTree)
//		m_pTree->SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOMOVE);
}

void CCategoryPopup::OnPaint()
{
	m_bInitState = false;
	CPaintDC dc(this); // device context for painting

	CRect rc(0,0,PT_CX,PT_CY);
	CBrush br;
	br.CreateSolidBrush(CL_EDGE_POP1);
	dc.FrameRect(rc, &br);
	
	rc.DeflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE_POP2);
	dc.FrameRect(rc, &br);
	
	rc.DeflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE_POP3);
	dc.FrameRect(rc, &br);
	
	rc.DeflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE_POP4);
	dc.FrameRect(rc, &br);

	CFont* pOld = NULL;	

	rc = CRect(PT_TREE_X,PT_STATIC_Y,PT_TREE_V,PT_TREE_H);	
	rc.InflateRect(1,1,1,1);

	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE);
	dc.FrameRect(rc, &br);
	
	rc = CRect (PT_TREE_V+6 ,PT_STATIC_Y,PT_CX-10,PT_TREE_H);
	rc.InflateRect(1,1,1,1);
	br.DeleteObject();
	br.CreateSolidBrush(CL_EDGE);
	dc.FrameRect(rc, &br);
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc, CL_BK_GROUP);
	
	rc = CRect(PT_TREE_X,PT_STATIC_Y,PT_TREE_V,PT_STATIC_Y+2);
	dc.FillSolidRect(rc, CL_TREE_HC);
	rc = CRect(PT_TREE_X,PT_STATIC_Y+2,PT_TREE_V,PT_TREE_Y);
	dc.FillSolidRect(rc, CL_TREE_HT);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(253,253,255));
	pOld = dc.SelectObject(m_pMainWnd->getAxFont("굴림",9,3));
	rc.DeflateRect(30,0,0,0);
	dc.DrawText("분류",rc, DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	dc.SelectObject(pOld);
	if (m_bmp1.GetSafeHandle())
	{		
		BITMAP	bm;
		CDC	memDC;

		m_bmp1.GetBitmap(&bm);
		const	int	bx = bm.bmWidth;
		const	int	by = bm.bmHeight;
			
		memDC.CreateCompatibleDC(&dc);
		memDC.SelectObject(&m_bmp1);
		rc.InflateRect(25,0,0,0);
			
		dc.BitBlt(rc.left, rc.top , bx,by,&memDC,0,0,SRCCOPY);
		memDC.DeleteDC();
	}

	rc = CRect(PT_TREE_X , PT_TREE_H + 10 , PT_TREE_V ,PT_TREE_H +26);
	dc.Draw3dRect(rc, RGB(156,156,156),RGB(80,80,80));
	rc.DeflateRect(1,1,1,1);
	dc.FillSolidRect(rc, CL_BK_QR);

	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor(RGB(253,253,244));
	pOld = dc.SelectObject(m_pFont);
	dc.DrawText("!조회개수(최대1000개)",rc, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	dc.SelectObject(pOld);

	rc = CRect(PT_TREE_X , PT_TREE_H + 10 , PT_CX-10 , PT_TREE_H + 80);
	rc.InflateRect(1,1,1,1);
	dc.Draw3dRect(rc, CL_BK_QR, RGB(222,222,222));

	rc = CRect(PT_TREE_X , PT_TREE_H + 85 , PT_CX-10 , PT_TREE_H + 87);
	dc.Draw3dRect(rc, CL_EDGE,CL_BK_GROUP);
	

	if (m_pMainWnd->m_bShowPlay)
	{
		if (m_bmp2.GetSafeHandle())
		{		
			BITMAP	bm;
			CDC	memDC;

			m_bmp2.GetBitmap(&bm);
			const	int	bx = bm.bmWidth;
			const	int	by = bm.bmHeight;
			
			memDC.CreateCompatibleDC(&dc);
			memDC.SelectObject(&m_bmp2);
			
			dc.BitBlt(PT_TREE_X, PT_TREE_H + 85 + 10 , bx,by,&memDC,0,0,SRCCOPY);
			memDC.DeleteDC();
		}
		
		rc = CRect(PT_TREE_X , PT_TREE_H + 120 , PT_CX-10 , PT_TREE_H + 122);
		dc.Draw3dRect(rc, CL_EDGE,CL_BK_GROUP);
	}
}

void CCategoryPopup::baseCX_Category()
{
	if (!m_pTree)
		return;

	CString dir;
	HTREEITEM hItem1 = nullptr;

	dir.Format("%s\\tab\\", m_sRoot);
	switch (m_nKind)
	{
	case 0:		// 현재가 분류
		hItem1 = m_pTree->InsertItem("History", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 4));
		
		hItem1 = m_pTree->InsertItem("전종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 5));
		//거래소 전종목
		hItem1 = m_pTree->InsertItem("거래소",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 7));
		
		//코스닥 전종목
		hItem1 = m_pTree->InsertItem("코스닥",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 8));
		
		//코스피,코스탁 세분류..
		loadUCode(dir + "upcode.dat", TVI_ROOT);
		
		//KOSPI200
		hItem1 = m_pTree->InsertItem("KOSPI200",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 9));

		//KOSDAQ50
//		hItem1 = m_pTree->InsertItem("KOSDAQ50",0,1);
//		m_pTree->SetItemData(hItem1, MAKELONG(0, 10));

//		LoadHjCode(dir + "hjcode.dat" ,TVI_ROOT , "KOSDAQ50");
		//KOSDAQ50
//		loadUCode(dir + "upcode.dat", TVI_ROOT , "KOSDAQ50");
		
		//KRX100
		hItem1 = m_pTree->InsertItem("KRX100",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 11));
		
		//프리보드
		hItem1 = m_pTree->InsertItem("K-OTC",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 12));
		
		//ETF
		hItem1 = m_pTree->InsertItem("ETF",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 13));
		
		//ELW
		hItem1 = m_pTree->InsertItem("ELW",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 14));

		//ELW기초자산
		hItem1 = m_pTree->InsertItem("ELW기초자산",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 15));

		//관심종목
		m_iitem = m_pTree->InsertItem("관심종목", 0, 1);
		m_pTree->SetItemData(m_iitem, MAKELONG(0, -1));
		m_pMainWnd->RequestGroup();
		//LoadGroup(m_sRoot + "\\user\\" + m_sUser + "\\portFolio.ini", m_iitem);
		
		//테마종목
		hItem1 = m_pTree->InsertItem("섹터종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "themcd.ini", hItem1, "TMCODE", 6);

		//그룹사별
		hItem1 = m_pTree->InsertItem("그룹사별", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "gropcd.ini", hItem1, "GRCODE", 0);

		break;
	case 1:		// 관심종목 분류
		//거래소 전종목
		hItem1 = m_pTree->InsertItem("거래소",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 7));
		
		//코스닥 전종목
		hItem1 = m_pTree->InsertItem("코스닥",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 8));
		
		//코스피,코스탁 세분류..
		loadUCode(dir + "upcode.dat", TVI_ROOT);
		
		//KOSPI200
		hItem1 = m_pTree->InsertItem("KOSPI200",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 9));

		//KOSDAQ50
//		hItem1 = m_pTree->InsertItem("KOSDAQ50",0,1);
//		m_pTree->SetItemData(hItem1, MAKELONG(0, 10));

//		LoadHjCode(dir + "hjcode.dat" ,TVI_ROOT , "KOSDAQ50");
		//KOSDAQ50
//		loadUCode(dir + "upcode.dat", TVI_ROOT , "KOSDAQ50");
		
		//KRX100
		hItem1 = m_pTree->InsertItem("KRX100",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 11));
		
		//프리보드
		hItem1 = m_pTree->InsertItem("K-OTC",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 12));
		
		//ETF
		hItem1 = m_pTree->InsertItem("ETF",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 13));
		
		//ELW
		hItem1 = m_pTree->InsertItem("ELW",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 14));

		//ELW기초자산
		hItem1 = m_pTree->InsertItem("ELW기초자산",0,1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, 15));

		//관심종목
		m_iitem = m_pTree->InsertItem("관심종목", 0, 1);
		m_pTree->SetItemData(m_iitem, MAKELONG(0, -1));
		m_pMainWnd->RequestGroup();
		//LoadGroup(m_sRoot + "\\user\\" + m_sUser + "\\portFolio.ini", m_iitem);
		
		//테마종목
		hItem1 = m_pTree->InsertItem("섹터종목", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "themcd.ini", hItem1, "TMCODE", 6);

		//그룹사별
		hItem1 = m_pTree->InsertItem("그룹사별", 0, 1);
		m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
		LoadFolder(dir + "gropcd.ini", hItem1, "GRCODE", 0);
		
		break;
	}
}

//필터링 해서 조건에 맞는 경우만 참 리턴
bool CCategoryPopup::IsFilteredHj(_mhjcode* hj)
{
	if (m_pMainWnd->m_bCateChecked[0] &&	//불성실공시종목 
		(hj->jchk & 0x01))
		return false;
	if (m_pMainWnd->m_bCateChecked[1] &&//신규상장종목
		(hj->jchk & 0x02))
		return false;
	if (m_pMainWnd->m_bCateChecked[2] &&//권리락종목
		(hj->jchk & 0x04))
		return false;
	if (m_pMainWnd->m_bCateChecked[3] &&//우선주종목
		(hj->jchk & 0x08))
		return false;
	if (m_pMainWnd->m_bCateChecked[4] &&//코스닥 종목 제외
	    (hj->kosd == jmKOSDAQ))
		return false;
	if (m_pMainWnd->m_bCateChecked[5] &&//관리종목
		(hj->jchk & 0x10))
		return false;
	if (m_pMainWnd->m_bCateChecked[6] &&//감리종목
		(hj->jchk & 0x20))
		return false;
	if (m_pMainWnd->m_bCateChecked[7] &&//거래정지종목
		(hj->jchk & 0x40))
		return false;
	if (m_pMainWnd->m_bCateChecked[8] &&//투자유의종목
		(hj->jchk & 0x80))
		return false;
	if (m_pMainWnd->m_bCateChecked[9] &&//코스닥 종목 제외
	    (hj->kosd == jmKOSPI))
		return false;
	return true;
}

void CCategoryPopup::SearchCode()
{
	HTREEITEM hItem = m_pTree->GetSelectedItem();
	if (hItem == nullptr)
		return ;

	int	ii = 0;
	CString szCode, szName;
	// 분류별 로드
	std::shared_ptr<_mhjcode> hj = nullptr;
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> tempCodes;
	std::shared_ptr<CStringArray> pArr = nullptr;

	m_shjCodes.RemoveAll();
	tempCodes.Copy(m_pMainWnd->m_hjCodes);

	int	ntype = HIWORD(m_pTree->GetItemData(hItem));
	switch (ntype)
	{
	case 0:		//그룹사
		pArr = m_pMainWnd->m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem)));
		for (ii = 0 ; ii < pArr->GetSize(); ii++)
		{
			szName = pArr->GetAt(ii);
			szName.Trim();
			szName = "A" + szName;
			
			for (int jj = 0; jj < tempCodes.GetSize() ; jj++)
			{
				hj = tempCodes.GetAt(jj);
				if (!IsFilteredHj(hj.get()))
					continue;

				if (hj->code == szName)
				{
					m_shjCodes.Add(hj);
					break;	
				}
			}
		}
		break;

	case 6:		//테마종목
		pArr = m_pMainWnd->m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem)));
		for (ii = 0 ; ii < pArr->GetSize(); ii++)
		{
			for (int jj = 0; jj < tempCodes.GetSize() ; jj++)
			{
				hj = tempCodes.GetAt(jj);
				if (!IsFilteredHj(hj.get()))
					continue;

				szName = pArr->GetAt(ii);
				szName.Trim();
				szName = "A" + szName;
				if (hj->code == szName)
				{
					m_shjCodes.Add(hj);
					break;	
				}
			}
		}
		break;

	case 2:		//코스피 업종
		for (ii = 0; ii < tempCodes.GetSize() ; ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;

			ListUpjongSort(LOWORD(m_pTree->GetItemData(hItem)), hj);
		}
		break;
	case 3:		// 관심종목
		{
			const	int	nGroup = LOWORD(m_pTree->GetItemData(hItem));
			m_pMainWnd->RequestCode(nGroup);
			return;
			/*
			CString tmp = m_pMainWnd->LoadCodeList(nGroup);

			while (!tmp.IsEmpty())
			{
				szCode = Parser(tmp,"\t");
				szCode.Trim();
				for (int jj = 0; jj < tempCodes.GetSize() ; jj++)
				{
					hj = tempCodes.GetAt(jj);
					if (hj->code.Mid(1) == szCode)
					{
						if (!IsFilteredHj(hj.get()))
							continue;

						m_shjCodes.Add(hj);
						break;	
					}
				}
			}
			*/
		}
		break;

	case 4:		//히스토리
		for (ii = 0 ; ii < m_history.GetSize(); ii++)
		{		
			szCode = m_history.GetAt(ii);
			for (int jj = 0; jj < tempCodes.GetSize(); jj++)
			{
				hj = tempCodes.GetAt(jj);
				if (hj->code.Mid(1) == szCode)
				{
					m_shjCodes.Add(hj);
					break;	
				}
			}		
		}
		break;

	case 5:		//전종목
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			
			m_shjCodes.Add(hj);
		}
		break;

	case 7:		//코스피 전종목
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->kosd == jmKOSPI)
			{					
				m_shjCodes.Add(hj);
			}
		}
		break;

	case 8:		//코스닥 전종목
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->kosd == jmKOSDAQ)
			{						
				m_shjCodes.Add(hj);
			}
		}
		break;

	case 9:		//KOSPI200
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;

			if (hj->kosd == jmKOSPI)
			{
				if (hj->kpgb == 1 || hj->kpgb == 2 || hj->kpgb == 3) 
					m_shjCodes.Add(hj);
			}			
		}
		break;

/*	case 10:	//KOSDAQ50
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->kosd == jmKOSDAQ&& hj->kpgb == 1)
			{
				m_shjCodes.Add(hj);
			}
		}
		break;
*/
	case 11:	//KRX100
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->unio == 1)
			{		
				m_shjCodes.Add(hj);
			}
		}
		break;

	case 12:	//프리보드
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->kosd == jm3RD)
			{		
				m_shjCodes.Add(hj);
			}
		}
		break;

	case 13:	//ETF
		for (ii = 0; ii < tempCodes.GetSize() ; ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->ssgb == jmETF)
			{		
				m_shjCodes.Add(hj);
			}
		}
		break;

	case 14:	//ELW
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			if (hj->ssgb == jmELW)
			{
				m_shjCodes.Add(hj);
			}
		}
		break;
	case 15:	// ELW기초자산
		szName.Format("%s\\tab\\", m_sRoot);
		LoadELWBase(szName + "elwcod.dat");
		for (ii = 0; ii < m_elwbase.GetSize() ; ii++)
		{
			hj = m_elwbase.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;

			m_shjCodes.Add(hj);
		}
		break;
	case 16:	//코스닥 업종
		for (ii = 0; ii < tempCodes.GetSize(); ii++)
		{
			hj = tempCodes.GetAt(ii);
			if (!IsFilteredHj(hj.get()))
				continue;
			ListUpjongSort(LOWORD(m_pTree->GetItemData(hItem)), hj, 1);
		}
		break;

	default:break;
	}

	fillList();
	/*
	
	tempCodes.RemoveAll();
	tempCodes.Copy(m_shjCodes);

	// Sort
	int	ncmpcnt =0;
	std::shared_ptr<_mhjcode> hja = nullptr, hjb = nullptr;

	ntype = m_pComboOrder->GetCurSel();
	switch (ntype)
	{
	case 1:
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE-1);
		for (ii = 0; ii < ncmpcnt; ii++)
		{
			hja = tempCodes.GetAt(ii);
			if (hja->kosd != jmKOSPI)
			{				
				for (int jj = ii+1; jj < tempCodes.GetSize(); jj++)
				{				
					hjb = tempCodes.GetAt(jj);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(ii, hjb);
						tempCodes.SetAt(jj, hja);
						break;
					}
				}
			}			
		}
		break;
	case 2:
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE-1);
		for (ii = 0 ; ii < ncmpcnt ; ii++)
		{
			for (int jj = ii+1 ; jj < tempCodes.GetSize() ; jj++)
			{
				hja = tempCodes.GetAt(ii);
				hjb = tempCodes.GetAt(jj);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(ii, hjb);
					tempCodes.SetAt(jj, hja);
				}
			}
		}
		break;

	case 3:
		ncmpcnt = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE-1);
		for (ii = 0 ; ii < ncmpcnt ; ii++)
		{
			for (int jj = ii+1 ; jj < tempCodes.GetSize() ; jj++)
			{
				hja = tempCodes.GetAt(ii);
				hjb = tempCodes.GetAt(jj);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(ii, hjb);
					tempCodes.SetAt(jj, hja);
				}
			}
		}
		for (ii = 0; ii < ncmpcnt ; ii++)
		{
			hja = tempCodes.GetAt(ii);
			if (hja->kosd != jmKOSPI)
			{				
				for (int jj = ii+1 ; jj < tempCodes.GetSize() ; jj++)
				{				
					hjb = tempCodes.GetAt(jj);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(ii, hjb);
						tempCodes.SetAt(jj ,hja);
						break;
					}
				}
			}
		}
		break;
	default:break;
	}

	m_shjCodes.RemoveAll();
	m_shjCodes.Copy(tempCodes);
	if (m_shjCodes.GetSize() > MAX_LIST_SIZE)
	{	
		ncmpcnt = m_shjCodes.GetSize()- MAX_LIST_SIZE ;
		m_shjCodes.RemoveAt(MAX_LIST_SIZE , ncmpcnt);
	}
	
	m_pList->DeleteAllItems();
	m_pMainWnd->m_sCodeList = "";

	const	int	col = 2;
	int	tot = 0, swidth = 0, width[col]{};
	CRect	rect;

	if (m_shjCodes.GetSize() > 4)
	{
		m_pList->GetClientRect(&rect);
		tot = (int)(rect.Width() / col);
		swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10 - swidth;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0,width[0]);
		m_pList->SetColumnWidth(1,width[1]);
	}
	else
	{
		m_pList->GetClientRect(&rect);
		tot = (int)(rect.Width() / col);
		swidth = GetSystemMetrics(SM_CXHSCROLL);
		width[0] = tot - 10 ;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0,width[0]);
		m_pList->SetColumnWidth(1,width[1]);
	}

	for (ii = 0 ; ii < m_shjCodes.GetSize() ; ii++)
	{
		if (ii >= MAX_LIST_SIZE)
			break;

		hj = m_shjCodes.GetAt(ii);
		m_pList->InsertItem(ii,hj->code.Mid(1));
		m_pList->SetItem(ii,1,LVIF_TEXT, hj->hnam,0,0,0,0);
		m_pMainWnd->m_sCodeList += hj->code.Mid(1);
		m_pMainWnd->m_sCodeList += '\t';
	}
	*/
}

void CCategoryPopup::ListUpjongSort(int nidx, std::shared_ptr<_mhjcode> hjc, int ntype)
{
	//코스피
	if (ntype == 0 && hjc->kosd == jmKOSPI)
	{
		if (hjc->ssgb == jmSINJU || hjc->ssgb == jmSINJS || hjc->ssgb == jmHYFND)	// 신주인수권 수익증권 제외
			return;

		if (nidx == 1)
		{
			m_shjCodes.Add(hjc);
		}
		else if (nidx < 5)
		{
			// 2 : 대형주, 3 : 중형주, 4 : 소형주
			if (hjc->size == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 27)
		{
			CString	ucdm = acd_KOSPI[nidx];
			if (!ucdm.CompareNoCase(hjc->acdm) || !ucdm.CompareNoCase(hjc->acds))
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 27)	// 제조업
		{
			if (hjc->jjug == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 41)
		{
			if (hjc->jsiz == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 41)	// 배당지수
		{
			if (hjc->jsiz >= 38 + nidx && hjc->jsiz <= 41 + nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 42)	// 지배구조우수
		{
			if (hjc->wsgb == 1)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 101)	// 코스피 200
		{
			if (hjc->kpgb == 1 || hjc->kpgb == 2 || hjc->kpgb == 3)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 401)	// 코스피 100
		{
			if (hjc->kpgb == 3 || hjc->kpgb == 2)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 402)	// 코스피 50
		{
			if (hjc->kpgb == 3)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 403)	// 코스피 IT
		{
			if (hjc->itgb == 1)
				m_shjCodes.Add(hjc);
		}
	}
	else if (ntype == 1 && hjc->kosd == jmKOSDAQ) //코스닥
	{
		nidx = nidx % 100;
		if (nidx == 1)
		{
			m_shjCodes.Add(hjc);
		}
		else if (nidx == 2)	// 벤처지수
		{
			if (hjc->jjug == 2)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 5)
		{
			// 3 : 기타 서비스, 4 : 코스닥 IT 종합
			if (hjc->size == nidx)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 5)	// 코스닥 IT 벤처
		{
			if (hjc->size == 4 && hjc->jjug == 2)
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 38 || nidx == 41)
		{
			CString	ucdm = acd_KOSDAQ[nidx];
			if (!ucdm.CompareNoCase(hjc->acdm) || !ucdm.CompareNoCase(hjc->acds))
				m_shjCodes.Add(hjc);
		}
		else if (nidx < 41)
		{
			// 38 : KOSDAQ 100, 39 : KOSDAQ MID 300, 40 : KOSDAQ SMALL
			if (hjc->jsiz == nidx - 37)
				m_shjCodes.Add(hjc);
		}
		else if (nidx == 42)	// KOSDAQ 우량기업부
		{
			if (hjc->kpgb == 1)
				m_shjCodes.Add(hjc);
		}
		else if (nidx > 42 && nidx <= 45)
		{
			CString ucdm = acd_KOSDAQ[nidx];
			if (!ucdm.CompareNoCase(hjc->acdl))
				m_shjCodes.Add(hjc);
		}
	}
}

BOOL CCategoryPopup::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));	
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CCategoryPopup::LoadFolder(CString path, HTREEITEM hItem, CString sSection, int nKey)
{
	char	wb[2048]{};
	CString	tmps, sCode;
	HTREEITEM hItem1 = nullptr;
	std::shared_ptr<CStringArray> pStrArr = nullptr;

	for (int ii = 0; ii < 100; ii++)
	{
		tmps.Format("%02d", ii);
		GetPrivateProfileString("FOLDER", tmps, "", wb, sizeof(wb), path);
		tmps = wb;
		if (tmps.IsEmpty())
			break;
		
		hItem1 = m_pTree->InsertItem(Parser(tmps, ";"), 0, 1, hItem);

		//tmps.Format("%02d", ii);
		GetPrivateProfileString(sSection, tmps, "", wb, sizeof(wb), path);
		tmps = wb;
		if (tmps.IsEmpty())
		{
			m_pTree->DeleteItem(hItem1);
			continue;
		}

		pStrArr = std::make_shared<CStringArray>();
		while (!tmps.IsEmpty())
		{
			sCode = Parser(tmps, ";");
			if (!sCode.IsEmpty())
				pStrArr->Add(sCode);

			if (tmps.IsEmpty())
			{
				m_pTree->SetItemData(hItem1, MAKELONG(m_pMainWnd->m_codeArr.Add(pStrArr), nKey));
				break;
			}
		}
	}
}

void CCategoryPopup::LoadELWBase(CString path)
{
	if (m_elwbase.GetSize() > 0)
		return;

	CFile	file;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary | CFile::shareDenyNone))
		return;

	struct  elwcode  ELWCode;
	const	int	codeN = (int)file.GetLength() / sizeof(struct elwcode);
	bool	bFound = false;
	CStringArray	tmpBase;

	tmpBase.RemoveAll();
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));
		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
			bcode.TrimRight();
			if (bcode.IsEmpty())
				break;

			bFound = false;
			for (int kk = 0; kk < tmpBase.GetSize(); kk++)
			{
				if (bcode.CompareNoCase(tmpBase.GetAt(kk)) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				if (bcode.CompareNoCase("KOSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("BASKET") == 0)
					break;

				tmpBase.Add(bcode);
			}
		}
	}
	file.Close();

	std::shared_ptr<_mhjcode> mhjcode = nullptr;
	for (int ii = 0; ii < tmpBase.GetSize(); ii++)
	{
		mhjcode = FindCode(tmpBase.GetAt(ii));
		if (mhjcode)
			m_elwbase.Add(mhjcode);
	}

	tmpBase.RemoveAll();
}

std::shared_ptr<_mhjcode> CCategoryPopup::FindCode(CString sCode)
{
	std::shared_ptr<_mhjcode> mhjcode = nullptr;
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> tempCodes ;

	tempCodes.Copy(m_pMainWnd->m_hjCodes);
	for (int ii = 0; ii < tempCodes.GetSize(); ii++)
	{
		mhjcode = tempCodes.GetAt(ii);
		if (mhjcode->code == sCode)
			return mhjcode;
	}

	return nullptr;
}

void CCategoryPopup::setPathInfo(CString root, CString user)
{
	m_sRoot = root;
	m_sUser = user;
}

void CCategoryPopup::setParent(CWnd *pWnd)
{
	m_pParent = pWnd;
}

void CCategoryPopup::OnTreeDblClk(NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM hItem = m_pTree->GetSelectedItem();
	// 분류별 로드
	const	int	nType = HIWORD(m_pTree->GetItemData(hItem));
	if (nType < 0 || nType == 0xffff)
		return;

	CString str;

	SearchCode();	
	str.Format("%d개 조회",m_pList->GetItemCount());
	m_pQCStatic->SetWindowText(str);
	m_pQCStatic->Invalidate();

/*
	switch (HIWORD(m_pTree->GetItemData(hItem)))
	{
	case 0:	// 계열사
		m_pParent->SendMessage(WM_CATEGORY, 1, (LPARAM)m_pMainWnd->m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem))));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 6:	// 테마리스트 
		m_pParent->SendMessage(WM_CATEGORY, 2, (LPARAM)m_pMainWnd->m_codeArr.GetAt(LOWORD(m_pTree->GetItemData(hItem))));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 1:	// 등락종목
		m_pParent->SendMessage(WM_CATEGORY, 4, LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 2:	// 업종
		m_pParent->SendMessage(WM_CATEGORY, 6, LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 3:	// 관심등록
		m_pParent->SendMessage(WM_CATEGORY, 7, LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 4:	// History
		m_pParent->SendMessage(WM_CATEGORY, 8, LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 5:	// 전종목
		m_pParent->SendMessage(WM_CATEGORY, 9, LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0, 0);
		break;
	case 7: // 코스피 전종목
		m_pParent->SendMessage(WM_CATEGORY, 10 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 8: // 코스닥 전종목
		m_pParent->SendMessage(WM_CATEGORY, 11 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 9: // KOSPI200
		m_pParent->SendMessage(WM_CATEGORY, 12 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 10: // KOSDAQ50
		m_pParent->SendMessage(WM_CATEGORY, 13 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 11: // KRX100
		m_pParent->SendMessage(WM_CATEGORY, 14 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 12: // 프리보드
		m_pParent->SendMessage(WM_CATEGORY, 15 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 13: // ETF
		m_pParent->SendMessage(WM_CATEGORY, 16 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	case 14: // ELW
		m_pParent->SendMessage(WM_CATEGORY, 17 , LOWORD(m_pTree->GetItemData(hItem)));
//		PostMessage(WM_USER, 0 , 0);
		break;
	}
*/
}

CString CCategoryPopup::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CCategoryPopup::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CCategoryPopup::loadUCode(CString path, HTREEITEM	hItem)
{
	HANDLE fh = CreateFile(path.GetString(), GENERIC_READ, 0, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox("File not found.");
		CloseHandle(fh);
		return;
	}

	char	buf[256]{};
	int	res = 0;
	DWORD	dw = 0;
	int	bufsize = 0;
	CString	name, dta;
	struct	upcode* pUpCode = nullptr;
	HTREEITEM hItem1 = m_pTree->InsertItem("KOSPI", 0, 1, hItem);
	HTREEITEM hItem2 = m_pTree->InsertItem("KOSDAQ", 0, 1, hItem);

	m_pTree->SetItemData(hItem1, MAKELONG(0, -1));
	m_pTree->SetItemData(hItem2, MAKELONG(0, -1));

	bufsize = sizeof(struct upcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (bufsize != (int)dw)
			break;

		pUpCode = (struct upcode *)buf;
		name = CString(pUpCode->hnam, UNameLen);
		res = name.Find('\0');
		if (res != -1)
			name = name.Left(res);
		name.TrimRight();

		dta.Format("%d%02d", (int)pUpCode->jgub, (int)pUpCode->ucod);

		if (pUpCode->jgub == upKOSPI || pUpCode->jgub == upINTEGRATE ||
			(pUpCode->jgub == upKOSPI200 && pUpCode->ucod == 1) || 
			pUpCode->jgub == upKOSPIx)
			m_pTree->SetItemData(m_pTree->InsertItem(name, 0, 1, hItem1), MAKELONG(atoi(dta), 2));
		else if (pUpCode->jgub == upKOSDAQ || 
			pUpCode->jgub == upKOSDAQ50)
			m_pTree->SetItemData(m_pTree->InsertItem(name, 0, 1, hItem2), MAKELONG(atoi(dta), 16));
	}
	CloseHandle(fh);
}

void CCategoryPopup::LoadGroup(char* datb, int datl)
{
	if (m_iitem == nullptr)
		return;

	if (datl < 4)
		return;

	struct	_gfoldex* group = (struct _gfoldex*)datb;
	const	int	cnt = atoi(CString(group->nrec, sizeof(group->nrec)));
	if (cnt <= 0)
		return;

	CString	tmps;
	HTREEITEM hItem1 = nullptr;

	for (int ii = 0, ival=  0; ii < cnt; ii++)
	{
		ival = atoi(CString(group->glist[ii].ogrs, sizeof(group->glist[ii].ogrs)));
		tmps = CString(group->glist[ii].gnam, sizeof(group->glist[ii].gnam));
		tmps.Trim();
		if (tmps.IsEmpty())
			continue;

		hItem1 = m_pTree->InsertItem(tmps, 0, 1, m_iitem);
		m_pTree->SetItemData(hItem1, MAKELONG(ival, 3));
	}
}

void CCategoryPopup::OnSizing(UINT fwSide, LPRECT pRect) 
{
/*
	
//	if (fwSide ==)
	CRect rc;
	m_pMainWnd->GetWindowRect(&rc);
	if (m_pMainWnd->m_bShowPlay)
		rc.SetRect(rc.left  , rc.bottom  , rc.left + PT_CX, rc.bottom + PT_CY);
	else
		rc.SetRect(rc.left  , rc.bottom  , rc.left + PT_CX, rc.bottom + PT_CX-30);

	pRect->left = rc.left;
	pRect->top = rc.top;
	pRect->right = rc.right;
	pRect->bottom = rc.bottom;
		
	CWnd::OnSizing(fwSide, pRect);
*/	
}

void CCategoryPopup::loadRank(HTREEITEM hItem)
{
	const	struct _rankStruct
	{
		char	*name;
		long	value;
	} rankStruct[] = {
		{"상승률상위(거래소)",			111},
		{"상승률상위(코스닥)",			112},
		{"하락율상위(거래소)",			121},
		{"하락율상위(코스닥)",			122},
		{"보통주/우선주이격율(거래소)",		131},
		{"보통주/우선주이격율(코스닥)",		132},
		{"거래량상위(거래소)",			141},
		{"거래량상위(코스닥)",			142},
		{"거래대금상위(거래소)",		151},
		{"거래대금상위(코스닥)",		152},
		{"시가총액순위(거래소)",		161},
		{"시가총액순위(코스닥)",		162},
		{"상한가종목(거래소)",			11},
		{"상한가종목(코스닥)",			12},
		{"상승종목(거래소)",			21},
		{"상승종목(코스닥)",			22},
		{"보합종목(거래소)",			31},
		{"보합종목(코스닥)",			32},
		{"하락종목(거래소)",			51},
		{"하락종목(코스닥)",			52},
		{"하한가종목(거래소)",			41},
		{"하한가종목(코스닥)",			42},
		{"52주고가종목(거래소)",		171},
		{"52주고가종목(코스닥)",		172},
		{"52주저가종목(거래소)",		181},
		{"52주저가종목(코스닥)",		182},
		{"매수체결비율상위(거래소)",		211},
		{"매수체결비율상위(코스닥)",		212},
		{"매도체결비율상위(거래소)",		221},
		{"매도체결비율상위(코스닥)",		222},
		{"5일-20일 골든크로스종목(거래소)",	311},
		{"5일-20일 골든크로스종목(코스닥)",	312},
		{"5일-60일 골든크로스종목(거래소)",	321},
		{"5일-60일 골든크로스종목(코스닥)",	322},
		{"20일-60일 골든크로스종목(거래소)",	331},
		{"20일-60일 골든크로스종목(코스닥)",	332},
		{"5일-20일 데드크로스종목(거래소)",	351},
		{"5일-20일 데드크로스종목(코스닥)",	352},
		{"5일-60일 데드크로스종목(거래소)",	361},
		{"5일-60일 데드크로스종목(코스닥)",	362},
		{"20일-60일 데드크로스종목(거래소)",	371},
		{"20일-60일 데드크로스종목(코스닥)",	372}
	};
	
 
	for (int ii = 0; ii < (sizeof(rankStruct) / sizeof(_rankStruct)); ii++)
	{
		m_pTree->SetItemData(
			m_pTree->InsertItem(LoadStringTb(IDS_RISERATETOP_KP + ii), 0, 1, hItem), 
			MAKELONG(rankStruct[ii].value, 1));
	}
}

CString CCategoryPopup::LoadStringTb(UINT nIdx)
{
	CString str;

	str.LoadString(nIdx);
	return str;
}

void CCategoryPopup::OnChecked(UINT nID)
{
	if (nID >= IDC_CHECK_BGJ && nID <= IDC_CHECK_KOP)
	{
		const	int	nCheck = ((CButton*)GetDlgItem(nID))->GetCheck();
		m_pMainWnd->m_bCateChecked[nID-IDC_CHECK_BGJ] = (bool)nCheck;
	}
}

long CCategoryPopup::OnEnd(WPARAM wParam, LPARAM lParam)
{
	m_fDone = true;
	return 0;
}

void CCategoryPopup::OnConfigClk()
{
	CString	str;
	int	nType = 4;	//디폴트는 history;
	HTREEITEM hItem = m_pTree->GetSelectedItem();
	
	if (hItem != NULL)
		nType = HIWORD(m_pTree->GetItemData(hItem));

	m_pComboGap->GetWindowText(str);
	m_Gap = atoi(str);
	
	const	int	nTmp = m_chkPlay->GetCheck();
	m_pMainWnd->m_nChkPlay = nTmp;
	m_pMainWnd->SendOK(m_shjCodes, (m_Gap >= 2) ? m_Gap : 2);

	if (IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);

	PostMessage(WM_USER+2, 0, 0);
}

void CCategoryPopup::OnCloseClk()
{
	if (IsWindow(m_hWnd))
		ShowWindow(SW_HIDE);

	PostMessage(WM_USER+2, 0, 0);
}

void CCategoryPopup::OnSearchClk()
{
	CString str;

	SearchCode();
	str.Format("%d개 조회",m_pList->GetItemCount());
	m_pQCStatic->SetWindowText(str);
	m_pQCStatic->Invalidate();	
}


void CCategoryPopup::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
	{
		PostMessage(WM_USER+2, 0, 0);
	}
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCategoryPopup::OnSysChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if (nChar == VK_ESCAPE)
	{
		PostMessage(WM_USER+2, 0, 0);
	}
	CWnd::OnSysChar(nChar, nRepCnt, nFlags);
}

BOOL CCategoryPopup::PreTranslateMessage(MSG* pMsg) 
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CCategoryPopup::initBitmap()
{
	CImageList	imgList;

	imgList.Create(1, 7, ILC_COLORDDB, 1, 0);
	m_pList->SetImageList(&imgList, LVSIL_SMALL);
}

void CCategoryPopup::LoadInterestCode(const CStringArray& codes)
{
	CString	tmps;
	std::shared_ptr<_mhjcode> hj = nullptr;

	m_shjCodes.RemoveAll();
	for (int ii = 0; ii < codes.GetSize(); ii++)
	{
		tmps = codes.GetAt(ii);
		for (int jj = 0; jj < m_pMainWnd->m_hjCodes.GetSize(); jj++)
		{
			hj = m_pMainWnd->m_hjCodes.GetAt(jj);
			if (hj->code.Mid(1) == tmps)
			{
				if (!IsFilteredHj(hj.get()))
					continue;

				m_shjCodes.Add(hj);
				break;
			}
		}
	}

	fillList();
}

void CCategoryPopup::fillList()
{
	CArray <std::shared_ptr<_mhjcode>, std::shared_ptr<_mhjcode>> tempCodes;
	int	ii = 0, count = 0;
	const	int	ntype = m_pComboOrder->GetCurSel();
	std::shared_ptr<_mhjcode> hja = nullptr, hjb = nullptr;
	CString	tmps;

	tempCodes.Copy(m_shjCodes);
	switch (ntype)
	{
	case 1:
		count = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (ii = 0; ii < count; ii++)
		{
			hja = tempCodes.GetAt(ii);
			if (hja->kosd != jmKOSPI)
			{
				for (int jj = ii + 1; jj < tempCodes.GetSize(); jj++)
				{
					hjb = tempCodes.GetAt(jj);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(ii, hjb);
						tempCodes.SetAt(jj, hja);
						break;
					}
				}
			}
		}
		break;
	case 2:
		count = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (ii = 0; ii < count; ii++)
		{
			for (int jj = ii + 1; jj < tempCodes.GetSize(); jj++)
			{
				hja = tempCodes.GetAt(ii);
				hjb = tempCodes.GetAt(jj);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(ii, hjb);
					tempCodes.SetAt(jj, hja);
				}
			}
		}
		break;

	case 3:
		count = min(tempCodes.GetSize() - 1, MAX_LIST_SIZE - 1);
		for (ii = 0; ii < count; ii++)
		{
			for (int jj = ii + 1; jj < tempCodes.GetSize(); jj++)
			{
				hja = tempCodes.GetAt(ii);
				hjb = tempCodes.GetAt(jj);
				if (hjb->hnam.CompareNoCase(hja->hnam) < 0)
				{
					tempCodes.SetAt(ii, hjb);
					tempCodes.SetAt(jj, hja);
				}
			}
		}
		for (ii = 0; ii < count; ii++)
		{
			hja = tempCodes.GetAt(ii);
			if (hja->kosd != jmKOSPI)
			{
				for (int jj = ii + 1; jj < tempCodes.GetSize(); jj++)
				{
					hjb = tempCodes.GetAt(jj);
					if (hjb->kosd == jmKOSPI)
					{
						tempCodes.SetAt(ii, hjb);
						tempCodes.SetAt(jj, hja);
						break;
					}
				}
			}
		}
		break;
	default:break;
	}

	m_shjCodes.RemoveAll();
	m_shjCodes.Copy(tempCodes);
	if (m_shjCodes.GetSize() > MAX_LIST_SIZE)
	{
		count = m_shjCodes.GetSize() - MAX_LIST_SIZE;
		m_shjCodes.RemoveAt(MAX_LIST_SIZE, count);
	}

	m_pList->DeleteAllItems();
	m_pMainWnd->m_sCodeList = "";

	const	int	col = 2;
	int	tot = 0, width[col]{};
	const	int	swidth = GetSystemMetrics(SM_CXHSCROLL);
	CRect	rect;

	if (m_shjCodes.GetSize() > 3)
	{
		m_pList->GetClientRect(&rect);
		tot = (int)(rect.Width() / col);
		width[0] = tot - 10 - swidth;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0, width[0]);
		m_pList->SetColumnWidth(1, width[1]);
	}
	else
	{
		m_pList->GetClientRect(&rect);
		tot = (int)(rect.Width() / col);
		width[0] = tot - 10;
		width[1] = tot + 10;
		m_pList->SetColumnWidth(0, width[0]);
		m_pList->SetColumnWidth(1, width[1]);
	}

	for (ii = 0; ii < m_shjCodes.GetSize(); ii++)
	{
		if (ii >= MAX_LIST_SIZE)
			break;

		hja = m_shjCodes.GetAt(ii);
		m_pList->InsertItem(ii, hja->code.Mid(1));
		m_pList->SetItem(ii, 1, LVIF_TEXT, hja->hnam, 0, 0, 0, 0);
		m_pMainWnd->m_sCodeList += hja->code.Mid(1);
		m_pMainWnd->m_sCodeList += '\t';
	}

	tmps.Format("%d개 조회", m_pList->GetItemCount());
	m_pQCStatic->SetWindowText(tmps);
	m_pQCStatic->Invalidate();
}

