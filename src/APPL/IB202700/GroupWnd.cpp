// GroupWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "GroupWnd.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupWnd

CGroupWnd::CGroupWnd(CWnd* pMainWnd, int nIndex)
 :CAxisExt(pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_nIndex= nIndex;
	m_bSelect = false;
	m_nGridIndex = -1;
	m_pGridWnd = nullptr;

	m_root = Variant(homeCC, "");
}

CGroupWnd::~CGroupWnd()
{
}


BEGIN_MESSAGE_MAP(CGroupWnd, CWnd)
	//{{AFX_MSG_MAP(CGroupWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGroupWnd message handlers

BOOL CGroupWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	return CWnd::OnNotify(wParam, lParam, pResult);
}


//////////////////////////////////////////////////////////////////////////
void CGroupWnd::InitGrid()
{	
	const CRect rect;
	CRect rc, rcTemp;
	GetWindowRect(rc);
	GetClientRect(rc);
	m_rcRect = rc;

	//button
	HBITMAP	hBITMAP{}, hBITMAP_DN{}, hBITMAP_HV{};
	
	CString	fileIMG;
	fileIMG.Format("%s\\%s\\", m_root, IMAGEDIR);
	
	rcTemp.left = rect.right - 50;
	rcTemp.top = SIZE_GAPS;
	rcTemp.right = rcTemp.left + SIZE_BUTTONS;
	rcTemp.bottom = rcTemp.top + SIZE_BUTTONS;
	
	hBITMAP = GetAxHBitmap(fileIMG + "SAVEINT.bmp");
	hBITMAP_DN = GetAxHBitmap(fileIMG + "SAVEINT_dn.bmp");
	hBITMAP_HV = GetAxHBitmap(fileIMG + "SAVEINT_en.bmp");
	
	m_btSAVE.Create("\n관심종목저장", rcTemp, this, IDC_BT_SAVE_GRID);
	m_btSAVE.SetFont(CAxisExt::m_pFont, false);  
	m_btSAVE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	
	rcTemp.left = rcTemp.right + SIZE_GAPS;
	rcTemp.right = rcTemp.left + SIZE_BUTTONS;
	
	hBITMAP = GetAxHBitmap(fileIMG + "닫기1.bmp");
	hBITMAP_DN = GetAxHBitmap(fileIMG + "닫기1_dn.bmp");
	hBITMAP_HV = GetAxHBitmap(fileIMG + "닫기1_en.bmp");
	
	m_btCLOSE.Create("\n닫기", rcTemp, this, IDC_BT_ERASE_GRID);
	m_btCLOSE.SetFont(CAxisExt::m_pFont, false);  
	m_btCLOSE.SetImgBitmap(hBITMAP, hBITMAP_DN, hBITMAP_HV);
	
	m_rcTitle.left = rc.left;
	m_rcTitle.top = 2;
	m_rcTitle.right = m_rcRect.right;
	m_rcTitle.bottom = rcTemp.top + 25;
	
	m_clrFOCUS[0] = RGB(255, 255, 128);
	m_clrFOCUS[1] = RGB(255, 255, 255);
	m_clrTEXT[0] = RGB(255, 0, 0);
	m_clrTEXT[1] = RGB(100, 100, 100);

	m_rcRect.top += 26;
	m_rcRect.left += 1;
	m_rcRect.right -= 1;
	m_rcRect.bottom -= 1;

	m_pGridWnd = std::make_unique<CGridWnd>(m_pMainWnd, m_nIndex);
	m_pGridWnd->Create(m_rcRect, this, IDC_GRID);
}

bool CGroupWnd::clear()
{
	m_pGridWnd->RemoveAll();
	return true;
}

void CGroupWnd::queryCodes(CString strGroup, CString strGroupName)
{
	if (strGroupName.IsEmpty() && strGroup.Compare("-1"))
	{
		m_szTitle = ((CMainWnd*)m_pMainWnd)->GetGroupName(atoi(strGroup));
	}
	else
	{
		m_szTitle = strGroupName;
	}

	// 	CString strTemp;
	// 	strTemp.Format("[KSJ] queryCodes Index[%s] Name[%s]", strGroup, m_szTitle);
	// 	OutputDebugString(strTemp);

		//2016.04.19 KSJ 해당타이틀을 가져왔는데 없으면 당연히 빈그룹으로 생각한다.
	if (m_szTitle.IsEmpty())
	{
		m_pGridWnd->RemoveAll();
		m_pGridWnd->Invalidate();
		((CMainWnd*)m_pMainWnd)->SetCurGroup(-1);	//2016.05.03 KSJ 빈그룹으로 생각하므로 콤보박스도 선택안해줌.
		return;
	}

	// 	CString strTemp;
	// 	strTemp.Format("[KSJ] strGroup[%s] strGroupName[%s] m_szTitle[%s]", strGroup, strGroupName, m_szTitle);
	// 	OutputDebugString(strTemp);

	m_pGridWnd->queryCodes(strGroup, strGroupName);
}


void CGroupWnd::RecvOper(int kind, CRecvData* rdata)
{
	if (m_pGridWnd) 
		m_pGridWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_RECVDATA, kind), (LPARAM)rdata);
}

void CGroupWnd::FieldSetup(BOOL bSetup)
{
	if (m_pGridWnd) 
		m_pGridWnd->FieldSetup(bSetup);
}

void CGroupWnd::DrawTitle(BOOL bFocus)
{
	m_pMainWnd = GetParent();

	if(m_pMainWnd == nullptr || m_pGridWnd == nullptr) 
		return;


	CRect rect;
	GetClientRect(rect);

	CDC* pDC = GetDC();

	const int nmode = pDC->SetBkMode(TRANSPARENT);
	IH::DrawBox(pDC, CRect(rect.left, rect.top, rect.right, rect.top + 25), RGB(200, 200, 200), BOX_ALL, (bFocus) ? m_clrFOCUS[0] : m_clrFOCUS[1], TRUE);
	const COLORREF clr = pDC->SetTextColor(m_clrTEXT[0]);
	CFont*	oldfont = nullptr;
	if (bFocus)
		oldfont = pDC->SelectObject(m_pFontB);
	else
		oldfont = pDC->SelectObject(CAxisExt::m_pFont);
	
	pDC->DrawText(m_szTitle, CRect(rect.left + SIZE_GAPS, rect.top, rect.right, rect.top + 25), DT_SINGLELINE|DT_VCENTER|DT_LEFT);
	pDC->SetTextColor(clr);
	
	if (oldfont)
		pDC->SelectObject(oldfont);
	pDC->SetBkMode(nmode);
	
	ReleaseDC(pDC);

	DrawSelect(bFocus);
	OperResize(-1,-1);

	m_btSAVE.Invalidate(FALSE);
	m_btCLOSE.Invalidate(FALSE);
}

void CGroupWnd::DrawSelect(BOOL bFocus)
{
	CRect rect;
	
	GetClientRect(rect);
	
	CDC* pDC = GetDC();
	
// 	pDC->FillSolidRect(rect, GetIndexColor(68));

	const int nmode = pDC->SetBkMode(TRANSPARENT);
	
	CPen	pen(PS_SOLID, 4, bFocus ? RGB(255,0,0):RGB(0,0,0) );
	CPen*	oldpen = pDC->SelectObject(&pen);
	
	//rect.top -= 25;
	
	//rect.InflateRect(1,1,1,1);
	
	//BOX_LEFT
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.left, rect.bottom));
	}
	
	//BOX_RIGHT
	{
		pDC->MoveTo(CPoint(rect.right, rect.top));
		pDC->LineTo(CPoint(rect.right, rect.bottom));
	}
	
	//BOX_TOP
	{
		pDC->MoveTo(rect.TopLeft());
		pDC->LineTo(CPoint(rect.right, rect.top));
	}
	
	//BOX_BOTTOM
	{
		pDC->MoveTo(rect.left, rect.bottom);
		pDC->LineTo(CPoint(rect.right, rect.bottom));
	}
	
	pDC->SetBkMode(nmode);
	
	pDC->SelectObject(oldpen);
	pen.DeleteObject();
	
	ReleaseDC(pDC);

// 	if(m_pGridWnd != nullptr) m_pGridWnd->Refresh();
}

void CGroupWnd::OperResize(int cx, int cy)
{
	CRect	rect, rcButton;
	
	GetClientRect(rect);

	rcButton = CRect(rect.left, rect.top, rect.right, rect.top + 25);
	
	rcButton.right -= 1;
	rcButton.top += 3;
	rcButton.left = rcButton.right - SIZE_BUTTONS - 1;
	rcButton.bottom = rcButton.top + SIZE_BUTTONS;
	
	if (m_btCLOSE.GetSafeHwnd() && m_btSAVE.GetSafeHwnd())
	{
		m_btCLOSE.MoveWindow(rcButton, TRUE);

		rcButton.OffsetRect(-SIZE_BUTTONS - 2, 0);

		m_btSAVE.MoveWindow(rcButton, TRUE);

		m_btSAVE.ShowWindow(SW_SHOW);
		m_btCLOSE.ShowWindow(SW_SHOW);
	}
}
//////////////////////////////////////////////////////////////////////////
void CGroupWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here

	CRect rect;
	GetClientRect(&rect);

	rect.top += 26;
	rect.left += 1;
	rect.right -= 1;
	rect.bottom -= 1;

	if(m_pGridWnd != nullptr) 
		m_pGridWnd->MoveWindow(rect);

	OperResize(cx, cy);
}

BOOL CGroupWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	//m_pGridWnd->SendMessage(WM_COMMAND, wParam, lParam);

	m_pMainWnd = GetParent();

	switch (LOWORD(wParam))
	{
	case IDC_BT_SAVE_GRID:
		(CMainWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_nIndex);
		m_pGridWnd->saveInterestX();
		break;
	case IDC_BT_ERASE_GRID:
		(CMainWnd*)m_pMainWnd->PostMessage(WM_MANAGE, MAKEWPARAM(MK_CLOSEGRID, 0), (LPARAM)m_nIndex);
		break;
	}

	return CWnd::OnCommand(wParam, lParam);
}

void CGroupWnd::OnDestroy() 
{
	CWnd::OnDestroy();
	
	m_btSAVE.DestroyWindow();
	m_btCLOSE.DestroyWindow();

	if(m_pGridWnd)	
	{
		m_pGridWnd->DestroyWindow();
	}

	// TODO: Add your message handler code here
	
}

void CGroupWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	m_bLBtnDown = true;
	CWnd::OnLButtonDown(nFlags, point);
}

void CGroupWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(m_bLBtnDown)
	{
		m_bSelect = true;
		DrawTitle(m_bSelect);
		
		m_pMainWnd = GetParent();

		((CMainWnd*)m_pMainWnd)->SetCurGroup(m_pGridWnd->GetGroupIndex());
		((CMainWnd*)m_pMainWnd)->SetSelectGroup(m_nIndex+1);
		((CMainWnd*)m_pMainWnd)->SendMessage(WM_LBUTTONUP, nFlags, MAKELPARAM(point.x, point.y));

		m_nGridIndex = m_nIndex;
		m_nGroupIndex = m_pGridWnd->GetGroupIndex();
	}
	else
	{
		m_bSelect = false;
		InvalidateRect(m_rcRect, FALSE);
	}

	m_bLBtnDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}

void CGroupWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	//m_pGridWnd->InvalidateRect(m_rcRect);
	
	if(((CMainWnd*)GetParent())->GetSelectGroup() -1 == m_nGridIndex)	m_bSelect = true;
	else									m_bSelect = false;

	DrawTitle(m_bSelect);

	//Draw();
	// Do not call CWnd::OnPaint() for painting messages
}

LRESULT CGroupWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:	break;
	case DLL_OUB:
	case DLL_ALERTx:
	case DLL_TRIGGER:	
	case DLL_DOMINO:	
	case DLL_NOTICE:	
	case DLL_DROP:	
	case DLL_AXIS:		
	case DLL_EXCEL:	
	case DLL_INVOKE:	
	case DLL_SETPAL:	
	case DLL_SETFONT:	
	case DLL_SETFCB:	
	case DLL_SETFONTx:	
	case DLL_SELECT:	
	case DLL_ACCOUNT:	
	case DLL_GUIDE:	
		{
			if(m_pGridWnd != nullptr)
				m_pGridWnd->SendMessage(WM_USER, wParam, lParam);
			break;
		}
	}
	return TRUE;
}

LONG CGroupWnd::OnManage(WPARAM wParam, LPARAM lParam)
{	
	CString strTemp;
	
	switch (LOWORD(wParam))
	{
	case MK_RECVDATA:
		RecvOper(HIWORD(wParam), (CRecvData*)lParam);	
		break;

	default:
		//ret = ::OnManage(wParam, lParam);
		break;
		
	}
	return 0;
}