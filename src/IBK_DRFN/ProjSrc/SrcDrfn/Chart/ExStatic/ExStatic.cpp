// ExStatic.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExStatic.h"

#include "../../inc/ExStatic.h"
#include "../../inc/ISkinColorMng.h"

#include "MemberDraw.h"

#define	GAP_BORDER		2 
#define	GAP_IMAGE		8

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CExStaticApp

BEGIN_MESSAGE_MAP(CExStaticApp, CWinApp)
	//{{AFX_MSG_MAP(CExStaticApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExStaticApp construction

CExStaticApp::CExStaticApp()
{
	m_pCurMember = NULL;
}

long CExStaticApp::ActionControl(LPVOID lpAction)
{
	LPST_ACTIONCONTROL lpActionControl = (LPST_ACTIONCONTROL)lpAction;
//	CExButton* pButton = (CExButton*)m_pCurMember->m_pControl;
//	m_pManager->CallNotifyControl((LPVOID)pButton->GetParent(), 0);

	return 0L;
}

void CExStaticApp::ExitStatic(LPVOID lpPointer)
{
	m_pCurMember = (CMember*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

BOOL CExStaticApp::InitInstance() 
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);
	
	m_pISkinColorManager = (ISkinColorManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 6, 0);
	
	return CWinApp::InitInstance();
}

LPVOID CExStaticApp::InitDrawStatic(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	CMember* pMember = new CMemberDraw(lpPointer, m_pISkinColorManager, bUseDefSkin);
	return pMember;
}

BOOL CExStaticApp::SetStaticString(LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->SetMemberIndex(m_pISkinColorManager->GetIndex(lpszOutLine)
							, m_pISkinColorManager->GetIndex(lpszBackground)
							, m_pISkinColorManager->GetIndex(lpszTextClr));

	return TRUE;
}

BOOL CExStaticApp::SetStaticIndex(UINT nOutLine, UINT nBackground, UINT nText)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->SetMemberIndex(nOutLine, nBackground, nText);

	return TRUE;
}

BOOL CExStaticApp::SetStaticColor(COLORREF clrOut, COLORREF clrBk, COLORREF clrText)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->SetMemberDraw(clrOut, clrBk, clrText);

	return TRUE;
}

void CExStaticApp::OnPaint(CDC* pDC) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CRect rcItem;
	pMember->m_pStatic->GetClientRect(rcItem);

	pDC->Draw3dRect(rcItem, pMember->m_clrOutLine, pMember->m_clrOutLine);
	rcItem.DeflateRect(1, 1);

	DrawStaticText(pDC, rcItem);
	DrawStaticIcon(pDC, rcItem);
}

void CExStaticApp::DrawStaticText(CDC* pDC, CRect& rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pDC->FillSolidRect(rcItem, pMember->m_clrBackground);

	CString strText(pMember->m_lpszText);

	if(!strText.IsEmpty())
	{
		CFont *oldFont = pDC->SelectObject(pMember->m_pStatic->GetFont());
		CSize sizeText/*(pDC->GetTextExtent(strText))*/;
		GetTextExtentPoint(pDC->m_hDC, strText, strText.GetLength(), &sizeText);
		CRect rcText(rcItem);
//		rcText.DeflateRect(2,0);
		CPoint point(rcText.TopLeft());

		DWORD dwStyle = pMember->m_pStatic->GetStyle();
		if(dwStyle & SS_CENTERIMAGE)
			OffsetPoint(point, sizeText, rcItem);
//		point.x = GAP_BORDER;
		
		if(pMember->m_nIconAlign == STC_ICON_TWOROWS)
			point.y += (pMember->m_sizeImage.cy / 2) + (GAP_IMAGE / 2);

		if((dwStyle & SS_CENTER) == SS_CENTER/* || (dwStyle & SS_CENTER) == 0*/)
		{
			if (pMember->m_nIconAlign != STC_ICON_TWOROWS)
				point.x += pMember->m_sizeImage.cx;
//			else
//				point.x += m_nBorderGap;

			point.x += (rcItem.right - point.x - sizeText.cx - GAP_BORDER) / 2;
		}
		else if(dwStyle & SS_RIGHT)
		{
			point.x = rcItem.right - GAP_BORDER - sizeText.cx;
		}
		else // left
		{
			if (pMember->m_nIconAlign != STC_ICON_TWOROWS)
				point.x += pMember->m_sizeImage.cx - GAP_BORDER;
		}

		DWORD dwFlags = DST_PREFIXTEXT|DSS_NORMAL;
		pDC->SetTextColor(pMember->m_clrText);

		strText = pMember->m_lpszText;
  
		pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextAlign(TA_LEFT);
		if(strText.Find(_T("\n")) != -1)
		{
			UINT nFlag = DT_VCENTER;
			if(dwStyle & SS_CENTER)
				nFlag |= DT_CENTER;
			else if(dwStyle & SS_RIGHT)
				nFlag |= DT_RIGHT;

			pDC->DrawText(strText, rcText, nFlag);
		}
		else
		{
//			pDC->SetTextAlign(TA_LEFT);
//			if(point.x < rcText.left)
//				point.x = rcText.left;
//
//			pDC->DrawState(point, sizeText, strText, dwFlags, FALSE, strText.GetLength(), (HBRUSH)NULL);

			UINT nFlag = DT_VCENTER;
			if(dwStyle & SS_CENTER)
				nFlag |= DT_CENTER;
			else if(dwStyle & SS_RIGHT)
				nFlag |= DT_RIGHT;
			rcText.top = point.y;

			pDC->DrawText(strText, rcText, nFlag);
		}
		pDC->SelectObject(oldFont);
	}
}

void CExStaticApp::DrawStaticIcon(CDC* pDC, CRect& rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	HICON hIcon = pMember->m_hIcon;
	if(hIcon)
	{
        CPoint point = CalculateImagePosition(pDC, rcItem);

		DWORD dwFlags = DST_ICON | DSS_NORMAL;
		pDC->DrawState(point, pMember->m_sizeImage, hIcon, dwFlags, (HBRUSH)NULL);
	}
}

CPoint CExStaticApp::CalculateImagePosition(CDC* pDC, CRect& rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CString strText(pMember->m_lpszText);
	CSize   sizeText(pDC->GetTextExtent(strText));
	CPoint  point(rcItem.TopLeft());

	DWORD dwStyle = pMember->m_pStatic->GetStyle();
	
	if(dwStyle & SS_CENTERIMAGE)
		OffsetPoint(point, pMember->m_sizeImage, rcItem);

    point.x = GAP_BORDER;

    if(pMember->m_nIconAlign == STC_ICON_TWOROWS) 
    {
        if(!strText.IsEmpty())
			point.y -= ((sizeText.cy/2)+(GAP_IMAGE/2));
		
        if((dwStyle & SS_CENTER) == SS_CENTER/* || (dwStyle & SS_CENTER) == 0*/)
            point.x = rcItem.left + __max(0, ((rcItem.Width() - pMember->m_sizeImage.cx) / 2));
        else if(dwStyle & SS_RIGHT)
			point.x = rcItem.right - GAP_BORDER - pMember->m_sizeImage.cx;
        else // left
        {
            // already left aligned
        }
	}
	else
	{
		if(strText.IsEmpty())
		{
			if((dwStyle & SS_CENTER) == SS_CENTER)
	        {
				int cx = rcItem.Width();
				if(cx >= pMember->m_sizeImage.cx)
					point.x = (cx - pMember->m_sizeImage.cx) / 2;
			}
			if((dwStyle & SS_CENTERIMAGE) == SS_CENTERIMAGE)
			{
				int cy = rcItem.Height();
				if(cy >= pMember->m_sizeImage.cy)
					point.y = (cy - pMember->m_sizeImage.cy) / 2;
			}
		}
	}

    return point;
}

void CExStaticApp::OffsetPoint(CPoint& point, CSize size, CRect rcClient)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	if (pMember->m_nIconAlign == BTN_ICON_TWOROWS)
	{
        point.x = __max(0, ((rcClient.Width()-size.cx) / 2));
		point.y = __max(0, ((rcClient.Height()-size.cy) / 2));
	}
	else 
	{
		point.x = GAP_BORDER;
//		point.y = __max(0, (rcClient.Height() / 2) - (size.cy / 3));
//		point.y = __max(0, ((rcClient.Height()-size.cy) / 2));
		point.y = int(rcClient.top + double(rcClient.bottom - rcClient.top - size.cy)/2.0);
	}
}

BOOL CExStaticApp::SetIcon(CSize size, HICON hIcon, UINT nIconAlign)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pMember->m_hIcon = hIcon;
	pMember->m_sizeImage = size;
	pMember->m_nIconAlign = nIconAlign;

	return TRUE;
}

void CExStaticApp::SetText(LPCTSTR lpszText)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
//	pMember->m_pStatic->ShowWindow(SW_HIDE);

	pMember->SetText(lpszText);

//	pMember->m_pStatic->SetWindowText(lpszText);
	
	CRect rcStatic;
	pMember->m_pStatic->GetWindowRect(rcStatic);
	pMember->m_pStatic->GetParent()->ScreenToClient(rcStatic);

	rcStatic.right = rcStatic.left;
	rcStatic.left -= GAP_BORDER;
	
	pMember->m_pStatic->GetParent()->InvalidateRect(rcStatic);
	pMember->m_pStatic->Invalidate(FALSE);
}

FpChangeSkinNotify CExStaticApp::GetSkinNotifyFunction()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->GetNotifySkinTableFunction();
}

void CExStaticApp::PreSubclassWindow_Statc()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->m_pStatic->CStatic::PreSubclassWindow();

	pMember->m_pStatic->ModifyStyle(0, SS_NOTIFY);
//	pMember->m_pStatic->ModifyStyleEx(0, WS_EX_TRANSPARENT);

	CString strText;
	pMember->m_pStatic->GetWindowText(strText);
	pMember->SetText(strText);
}

LRESULT CExStaticApp::WindowProc_Static(UINT message, WPARAM wParam, LPARAM lParam)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	if(message == WM_LBUTTONDBLCLK)
	{
		pMember->m_pStatic->GetParent()->SendMessage(RMSG_LBTNDBLCLICK, 0, (LPARAM)pMember->m_pStatic->GetSafeHwnd());
	}
	else if(message == WM_LBUTTONDOWN)
	{
		pMember->m_pStatic->GetParent()->SendMessage(RMSG_LBTNDOWN, 0, (LPARAM)pMember->m_pStatic->GetSafeHwnd());
	}

	return pMember->m_pStatic->CStatic::WindowProc(message, wParam, lParam);
}

BOOL CExStaticApp::OnEraseBkgnd(CDC* pDC) 
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

/*
	CRect rcStatic;
	pMember->m_pStatic->GetWindowRect(rcStatic);
	pMember->m_pStatic->GetParent()->ScreenToClient(rcStatic);

	pDC->FillSolidRect(rcStatic, pMember->m_clrBackground);
*/
	
//	UNUSED_ALWAYS(pDC);
	return pMember->m_pStatic->CStatic::OnEraseBkgnd(pDC);
}

void CExStaticApp::GetWindowText(CString& rString)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	rString = pMember->m_lpszText;
}
/////////////////////////////////////////////////////////////////////////////
// The one and only CExStaticApp object

CExStaticApp theApp;

extern "C" __declspec(dllexport) long Ex_ActionControl(LPVOID lpPointer, LPVOID lpAction)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	long lResult = theApp.ActionControl(lpAction);

	theApp.m_pCurMember = NULL;

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_ExitStatic(LPVOID lpPointer)
{
	theApp.ExitStatic(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitStatic(LPVOID lpPointer)
{
	return theApp.InitDrawStatic(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitStatic2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitDrawStatic(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) BOOL Ex_SetStaticColor(LPVOID lpPointer, COLORREF clrOut, COLORREF clrBk, COLORREF clrText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetStaticColor(clrOut, clrBk, clrText);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_SetStaticString(LPVOID lpPointer, LPCTSTR lpszOutLine, LPCTSTR lpszBackground, LPCTSTR lpszTextClr)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetStaticString(lpszOutLine, lpszBackground, lpszTextClr);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) BOOL Ex_SetStaticIndex(LPVOID lpPointer, UINT nOutLine, UINT nBackground, UINT nText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetStaticIndex(nOutLine, nBackground, nText);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_OnPaint(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnPaint(pDC);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) BOOL Ex_SetIcon(LPVOID lpPointer, CSize size, HICON hIcon, UINT nIconAlign)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetIcon(size, hIcon, nIconAlign);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_SetText(LPVOID lpPointer, LPCTSTR lpszText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetText(lpszText);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) FpChangeSkinNotify Ex_GetSkinNotifyFunction(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	FpChangeSkinNotify pChangeSkinNotify = theApp.GetSkinNotifyFunction();
	theApp.m_pCurMember = NULL;

	return pChangeSkinNotify;
}

extern "C" __declspec(dllexport) void Ex_PreSubclassWindow_Statc(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.PreSubclassWindow_Statc();
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) LRESULT Ex_WindowProc_Static(LPVOID lpPointer, UINT message, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	return theApp.WindowProc_Static(message, wParam, lParam);
}

extern "C" __declspec(dllexport) BOOL Ex_OnEraseBkgnd_Static(LPVOID lpPointer, CDC* pDC)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.OnEraseBkgnd(pDC);
//	theApp.m_pCurMember = NULL;

	return bResult;
}

void WINAPI Ex_GetWindowText_Static(LPVOID lpPointer, CString& rString)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.GetWindowText(rString);
	theApp.m_pCurMember = NULL;
}
