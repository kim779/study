// ExComboBox.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExComboBox.h"
#include "MemberImgList.h"

#include "../../inc/ISkinColorMng.h"

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
// CExComboBoxApp

BEGIN_MESSAGE_MAP(CExComboBoxApp, CWinApp)
	//{{AFX_MSG_MAP(CExComboBoxApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExComboBoxApp construction

CExComboBoxApp::CExComboBoxApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

BOOL CExComboBoxApp::InitInstance() 
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);

	m_pISkinColorManager = (ISkinColorManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 6, 0);

	m_nOffset = ::GetSystemMetrics(SM_CXHTHUMB);

	CBitmap	bitmap;

//	bitmap.LoadBitmap(IDB_KB_DROPDOWN);
//	m_imgList.Create(17, 20, ILC_COLORDDB, 1, 1);
	bitmap.LoadBitmap(IDB_DROPDOWNBTN);
	m_imgList.Create(16, 16, ILC_COLORDDB, 1, 1);
	m_imgList.Add(&bitmap, RGB(255,0, 255));
	bitmap.DeleteObject();
	
	return CWinApp::InitInstance();
}

int CExComboBoxApp::ExitInstance() 
{
	m_imgList.DeleteImageList();
	
	return CWinApp::ExitInstance();
}

long CExComboBoxApp::ActionControl(LPVOID lpAction)
{
	LPST_ACTIONCONTROL lpActionControl = (LPST_ACTIONCONTROL)lpAction;
//	CExButton* pButton = (CExButton*)m_pCurMember->m_pControl;
//	m_pManager->CallNotifyControl((LPVOID)pButton->GetParent(), 0);

	return 0L;
}

void CExComboBoxApp::ExitComboBox(LPVOID lpPointer)
{
	m_pCurMember = (CMember*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

LPVOID CExComboBoxApp::InitComboBox(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	CMember* pMember = new CMemberImgList(lpPointer, m_pISkinColorManager, bUseDefSkin);
	return pMember;
}

void CExComboBoxApp::PreSubclassWindow_ComboBox(LPVOID lpPointer) 
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	pMember->m_pComboBox->CComboBox::PreSubclassWindow();
//	pMember->m_pComboBox->ModifyStyle(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, CBS_OWNERDRAWFIXED);
	pMember->m_pComboBox->ModifyStyleEx(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 0, SWP_FRAMECHANGED);
}

void CExComboBoxApp::DrawItem_ComboBox(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	long nColor = (long)lpDrawItemStruct->itemData;
	CDC dc;
	
	dc.Attach(lpDrawItemStruct->hDC);

	CString szText;
	int nIndex = lpDrawItemStruct->itemID;
	if(nIndex >= 0)
		pMember->m_pComboBox->GetLBText(nIndex, szText);

	// Save these value to restore them when done drawing.
	COLORREF crOldBkColor = dc.GetBkColor();

	if((lpDrawItemStruct->itemAction | ODA_SELECT) &&
	   (lpDrawItemStruct->itemState & ODS_COMBOBOXEDIT) &&
	   (lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
		dc.SetTextColor(RGB(255,255,255));
	}
	else 
	{
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, crOldBkColor);
  		dc.SetTextColor(nColor);
		dc.SetBkColor(crOldBkColor);
	}
	
	// Draw the text.
	dc.DrawText(
		szText,
		szText.GetLength(),
		&lpDrawItemStruct->rcItem,
		DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	dc.Detach();
}

void CExComboBoxApp::OnPaint_ComboBox(LPVOID lpPointer) 
{
	DrawComboNormal(lpPointer);
}

void CExComboBoxApp::DrawComboNormal(LPVOID lpPointer)
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	CRect rcItem;
	CRect rcOrn, rcText, rcBtn;
	pMember->m_pComboBox->GetClientRect(&rcOrn);

//	rcItem = rcOrn;

	CDC* pDC = pMember->m_pComboBox->GetDC();

/*
	DrawCombo(lpPointer, pDC, rcItem);
		
	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );
*/
//	rcOrn.top += 2;

//	rcItem = rcOrn;
//	rcItem.left = rcItem.right - 20;
//	rcItem.right = rcItem.left + 2;
//	rcItem.bottom = rcItem.top + 18;

//	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );

	rcText = rcOrn;
	rcText.right -= 18;
	rcBtn = rcOrn;
	rcOrn.DeflateRect(1,1);
	pDC->Draw3dRect(rcOrn, RGB(255,255,255), RGB(255,255,255));
	rcBtn.left = rcBtn.right - 18;
	rcBtn.top = rcBtn.top + 2;

	rcText.DeflateRect(1,1,0,1);
	pDC->Draw3dRect(rcText, RGB(255,255,255), RGB(255,255,255));

	if(pMember->m_bLBtnDown == FALSE)
		m_imgList.Draw(pDC, 0, CPoint(rcBtn.left, rcBtn.top), ILD_NORMAL);
	else
		m_imgList.Draw(pDC, 2, CPoint(rcBtn.left, rcBtn.top), ILD_NORMAL);
	

//	pDC->Draw3dRect(rcOrn, RGB(116,117,117), RGB(116,117,117));

	pMember->m_pComboBox->ReleaseDC(pDC);
}

void CExComboBoxApp::DrawCombo(LPVOID lpPointer, CDC* pDC, CRect& rcItem)
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	// Cover up dark 3D shadow.
	pDC->Draw3dRect(rcItem, pMember->m_clrOutLine, pMember->m_clrOutLine);
	rcItem.DeflateRect(1,1);
	
	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );

	// Cover up dark 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	rcItem.left = rcItem.right-m_nOffset;
	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );
	
	// Cover up normal 3D shadow on drop arrow.
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );
}

void CExComboBoxApp::DrawComboPressed(LPVOID lpPointer)
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	CRect rcItem;
	CRect rcOrn;
	pMember->m_pComboBox->GetClientRect(&rcOrn);

	rcItem = rcOrn;

	CDC* pDC = pMember->m_pComboBox->GetDC();

	DrawCombo(lpPointer, pDC, rcItem);
		
	rcItem.top -= 1;
	rcItem.bottom += 1;
	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );
	rcItem.left -= 1;
	pDC->Draw3dRect( rcItem, pMember->m_clrBackground, pMember->m_clrBackground );

	m_imgList.Draw(pDC, 1, CPoint(rcOrn.Width()-18, rcItem.top), ILD_NORMAL);
}

void CExComboBoxApp::OnMouseMove_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point) 
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	pMember->m_pComboBox->SetTimer(1,10,NULL);
	pMember->m_pComboBox->OnTimer(1);
	pMember->m_pComboBox->CComboBox::OnMouseMove(nFlags, point);
}

void CExComboBoxApp::OnLButtonDown_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point) 
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	pMember->m_bLBtnDown = TRUE;
	pMember->m_pComboBox->CComboBox::OnLButtonDown(nFlags, point);
}

void CExComboBoxApp::OnLButtonUp_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point) 
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	pMember->m_bLBtnDown = FALSE;
	pMember->m_pComboBox->Invalidate();
	pMember->m_pComboBox->CComboBox::OnLButtonUp(nFlags, point);
}

void CExComboBoxApp::OnTimer_ComboBox(LPVOID lpPointer, UINT nIDEvent) 
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;

	POINT pt;
	GetCursorPos(&pt);
	CRect rcItem;
	pMember->m_pComboBox->GetWindowRect(&rcItem);

	// OnLButtonDown, show pressed.
	if(pMember->m_bLBtnDown) 
	{
		pMember->m_pComboBox->KillTimer (1);
		DrawComboPressed(lpPointer);
		return;
	}
	else
	{
		CRect rcCombo, rcBtn;
		pMember->m_pComboBox->GetClientRect(&rcCombo);
		rcBtn = rcCombo;
		CDC* pDC = pMember->m_pComboBox->GetDC();
		if(rcItem.PtInRect(pt))
		{
			pDC->Draw3dRect(rcCombo, RGB(63,124,204), RGB(63,124,204));
			rcBtn.left = rcBtn.right - 18;
			rcBtn.top = rcBtn.top + 2;
			
			m_imgList.Draw(pDC, 1, rcBtn.TopLeft(), ILD_NORMAL);
		}
		else
		{
			pDC->Draw3dRect(rcCombo, RGB(102,102,102), RGB(209,209,209));
			rcBtn.left = rcBtn.right - 18;
			rcBtn.top = rcBtn.top + 2;
			
			m_imgList.Draw(pDC, 0, rcBtn.TopLeft(), ILD_NORMAL);
		}
		pMember->m_pComboBox->ReleaseDC(pDC);
	}
	
	pMember->m_pComboBox->CComboBox::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExComboBoxApp object

CExComboBoxApp theApp;

extern "C" __declspec(dllexport) long Ex_ActionControl(LPVOID lpPointer, LPVOID lpAction)
{
	theApp.m_pCurMember = (CMember*)lpPointer;

	long lResult = theApp.ActionControl(lpAction);

	theApp.m_pCurMember = NULL;

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_ExitComboBox(LPVOID lpPointer)
{
	theApp.ExitComboBox(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitComboBox(LPVOID lpPointer)
{
	return theApp.InitComboBox(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitComboBox2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitComboBox(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) void Ex_PreSubclassWindow_ComboBox(LPVOID lpPointer)
{
	theApp.PreSubclassWindow_ComboBox(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_DrawItem_ComboBox(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
//	theApp.DrawItem_ComboBox(lpPointer, lpDrawItemStruct);
}

extern "C" __declspec(dllexport) void Ex_OnPaint_ComboBox(LPVOID lpPointer)
{
	theApp.OnPaint_ComboBox(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_OnMouseMove_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.OnMouseMove_ComboBox(lpPointer, nFlags, point);
}

extern "C" __declspec(dllexport) void Ex_OnLButtonDown_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.OnLButtonDown_ComboBox(lpPointer, nFlags, point);
}

extern "C" __declspec(dllexport) void Ex_OnLButtonUp_ComboBox(LPVOID lpPointer, UINT nFlags, CPoint point)
{
	theApp.OnLButtonUp_ComboBox(lpPointer, nFlags, point);
}

extern "C" __declspec(dllexport) void Ex_OnTimer_ComboBox(LPVOID lpPointer, UINT nIDEvent)
{
	theApp.OnTimer_ComboBox(lpPointer, nIDEvent);
}
