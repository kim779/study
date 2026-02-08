// ExButton.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "ExButton.h"

#include "../../inc/ExButton.h"
#include "../../inc/ExCheckGroupBox.h"
#include "../../inc/ISkinColorMng.h"

#include "MemberDraw.h"
#include "MemberBitmap.h"
#include "MemberImgList.h"
#include "MemberChkGrpBox.h"
#include "MemberCheckBox.h"

#define	GAP_BORDER		4 
#define	GAP_IMAGE		8

#define ID_CHECKBOX		0xFFFE
#define	LEFT_OFFSET		12
#define	CHECKBOX_SIZE	16

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DEFLATE 0
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

void NotifyControl(UINT nFlag)
{
}

/////////////////////////////////////////////////////////////////////////////
// CExButtonApp

BEGIN_MESSAGE_MAP(CExButtonApp, CWinApp)
	//{{AFX_MSG_MAP(CExButtonApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExButtonApp construction

CExButtonApp::CExButtonApp()
{
	m_pCurMember = NULL;
	m_bToggleMode = FALSE;
	m_bPushed = FALSE;
}

long CExButtonApp::ActionControl(LPVOID lpAction)
{
	LPST_ACTIONCONTROL lpActionControl = (LPST_ACTIONCONTROL)lpAction;
//	CExButton* pButton = (CExButton*)m_pCurMember->m_pControl;
//	m_pManager->CallNotifyControl((LPVOID)pButton->GetParent(), 0);

	return 0L;
}

BOOL CExButtonApp::InitInstance() 
{
	HWND hMainWnd;
	AfxGetHanaroMainWnd(hMainWnd);

	m_pISkinColorManager = (ISkinColorManager*)::SendMessage(hMainWnd, RMSG_INTERFACE, 6, 0);
	
	return CWinApp::InitInstance();
}

LPVOID CExButtonApp::InitDrawButton(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	CMember* pMember = new CMemberDraw(lpPointer, m_pISkinColorManager, bUseDefSkin);
	return pMember;
}

LPVOID CExButtonApp::InitBitmapButton(LPVOID lpPointer)
{
	CMember* pMember = new CMemberBitmap(lpPointer);
	return pMember;
}

LPVOID CExButtonApp::InitImageListButton(LPVOID lpPointer)
{
	CMember* pMember = new CMemberImgList(lpPointer);
	return pMember;
}

LPVOID CExButtonApp::InitCheckBox(LPVOID lpPointer, BOOL bUseDefSkin/* = FALSE*/)
{
	CMember* pMember = new CMemberCheckBox(lpPointer, m_pISkinColorManager, bUseDefSkin);
	return pMember;
}

void CExButtonApp::ExitButton(LPVOID lpPointer)
{
	m_pCurMember = (CMember*)lpPointer;
	delete m_pCurMember;
	m_pCurMember = NULL;
}

void CExButtonApp::DrawItemButton(LPDRAWITEMSTRUCT lpDrawItemStruct)
{ 
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	CRect rcBtn(lpDrawItemStruct->rcItem);
	DrawButtonLine(pDC, lpDrawItemStruct->itemState, rcBtn);
	DrawButtonText(pDC, lpDrawItemStruct->itemState, rcBtn);
	DrawButtonIcon(pDC, lpDrawItemStruct->itemState, rcBtn);
}

void CExButtonApp::DrawButtonText(CDC* pDC, UINT nState, CRect& rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	if(nState & ODS_SELECTED)
		pDC->FillSolidRect(rcItem, pMember->m_bPushed ? pMember->m_clrBackground : pMember->m_clrBackgroundDown);
	else if(nState & ODS_DISABLED)
		pDC->FillSolidRect(rcItem, pMember->m_clrBackgroundDis);
	else
		pDC->FillSolidRect(rcItem, pMember->m_bPushed ? pMember->m_clrBackgroundDown : pMember->m_clrBackground);

	CString strText(pMember->m_lpszText);

	if(!strText.IsEmpty())
	{
		CFont *oldFont = pDC->SelectObject(pMember->m_pButton->GetFont());
		CSize sizeText(pDC->GetTextExtent(strText));
		CRect rcText(rcItem);
//		rcText.DeflateRect(2,0);
		CPoint point(rcText.TopLeft());

		OffsetPoint(point, sizeText, rcItem);
		
		DWORD dwStyle = pMember->m_pButton->GetStyle();

		if((dwStyle & BS_CENTER) == BS_CENTER || (dwStyle & BS_CENTER) == 0)
		{
			if (pMember->m_nIconAlign != BTN_ICON_TWOROWS)
				point.x += pMember->m_sizeImage.cx;
//			else
//				point.x += m_nBorderGap;

			point.x += (rcItem.right - point.x - sizeText.cx - GAP_BORDER) / 2;
			point.x += 1;
		}
		else if(dwStyle & BS_LEFT)
		{
			if (pMember->m_nIconAlign != BTN_ICON_TWOROWS)
				point.x += pMember->m_sizeImage.cx + GAP_IMAGE;
		}
		else // right
		{
			point.x = rcItem.right - GAP_BORDER - sizeText.cx;
		}

		if(nState & ODS_SELECTED)
			point.Offset(1, 1);

		DWORD dwFlags = DST_PREFIXTEXT;

		if(nState & ODS_DISABLED)
			pDC->SetTextColor(pMember->m_clrTextDis);
		else if(nState & ODS_FOCUS)
		{
			pDC->SetTextColor(pMember->m_clrText);// solomon
			int nDeflate = min(DEFLATE, min(rcItem.Width(), rcItem.Height()) >> 2);
			rcItem.DeflateRect(nDeflate, nDeflate);
			pDC->DrawFocusRect(&rcItem);
		}
		else
		{
			dwFlags |= DSS_NORMAL;
			pDC->SetTextColor(pMember->m_clrText);
		}

		strText = pMember->m_lpszText;
  
		pDC->SetBkMode(TRANSPARENT);

		if(strText.Find(_T("\n")) != -1)
		{
			UINT nFlag = DT_VCENTER;
			if(dwStyle & BS_CENTER)
				nFlag |= DT_CENTER;
			else if(dwStyle & BS_RIGHT)
				nFlag |= DT_RIGHT;

			pDC->DrawText(strText, rcText, nFlag);
		}
		else
		{
			pDC->SetTextAlign(TA_LEFT);
			pDC->DrawState(point, sizeText, strText, dwFlags, TRUE, strText.GetLength(), (HBRUSH)NULL);
		}

		pDC->SelectObject(oldFont);
	}
}

void CExButtonApp::DrawButtonText_ImgList(CDC* pDC, UINT nState, CRect& rcItem)
{
	CMemberImgList* pMember = (CMemberImgList*)m_pCurMember;

	CString strText(pMember->m_lpszText);

	if(!strText.IsEmpty())
	{
		CFont *oldFont = pDC->SelectObject(pMember->m_pButton->GetFont());
		CSize sizeText(pDC->GetTextExtent(strText));
		CRect rcText(rcItem);
		rcText.DeflateRect(2,0);
		CPoint point(rcText.TopLeft());

		OffsetPoint(point, sizeText, rcItem);
//		point.x = GAP_BORDER;
		
		DWORD dwStyle = pMember->m_pButton->GetStyle();
		if((dwStyle & BS_CENTER) == BS_CENTER || (dwStyle & BS_CENTER) == 0)
		{
			point.x += (rcItem.right - point.x - sizeText.cx - GAP_BORDER) / 2;
		}
		else if(dwStyle & BS_LEFT)
		{
		}
		else // right
		{
			point.x = rcItem.right - GAP_BORDER - sizeText.cx;
		}

		if(nState & ODS_SELECTED)
			point.Offset(1, 1);

		DWORD dwFlags = DST_PREFIXTEXT;
		if(nState & ODS_DISABLED)
			pDC->SetTextColor(pMember->m_clrTextDis);
		else
		{
			dwFlags |= DSS_NORMAL;
			pDC->SetTextColor(pMember->m_clrText);
		}

		strText = pMember->m_lpszText;
  
		pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextAlign(TA_LEFT);
		pDC->DrawState(point, sizeText, strText, dwFlags, TRUE, strText.GetLength(), (HBRUSH)NULL);
		pDC->SelectObject(oldFont);
	}
}

void CExButtonApp::DrawButtonLine(CDC* pDC, UINT nState, CRect& rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	if(nState & ODS_SELECTED)
	{
		if(pMember->m_bPushed)
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLine, pMember->m_clrOutLine);
		else
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLineDown, pMember->m_clrOutLineDown);
		rcItem.DeflateRect(1, 1);
		if(pMember->m_bPushed)
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLineTopLeft, pMember->m_clrInLineTopLeft);
		else
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLineTopLeftDown, pMember->m_clrInLineTopLeftDown);
		rcItem.DeflateRect(1, 1);
		if(pMember->m_bPushed)
			pDC->Draw3dRect(rcItem, pMember->m_clrInLineBottomRight, pMember->m_clrOutLineBottomRight);
		else
			pDC->Draw3dRect(rcItem, pMember->m_clrInLineBottomRightDown, pMember->m_clrOutLineBottomRightDown);
	}
	else if(nState & ODS_DISABLED)
	{
		pDC->Draw3dRect(rcItem, pMember->m_clrOutLineDis, pMember->m_clrOutLineDis);
		rcItem.DeflateRect(1, 1);
		pDC->Draw3dRect(rcItem, pMember->m_clrOutLineTopLeftDis, pMember->m_clrInLineTopLeftDis);
		rcItem.DeflateRect(1, 1);
		pDC->Draw3dRect(rcItem, pMember->m_clrInLineBottomRightDis, pMember->m_clrOutLineBottomRightDis);
	}
	else
	{
		if(pMember->m_bPushed)
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLineDown, pMember->m_clrOutLineDown);
		else
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLine, pMember->m_clrOutLine);
		rcItem.DeflateRect(1, 1);
		if(pMember->m_bPushed)
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLineTopLeftDown, pMember->m_clrInLineTopLeftDown);
		else
			pDC->Draw3dRect(rcItem, pMember->m_clrOutLineTopLeft, pMember->m_clrInLineTopLeft);
		rcItem.DeflateRect(1, 1);
		if(pMember->m_bPushed)
			pDC->Draw3dRect(rcItem, pMember->m_clrInLineBottomRightDown, pMember->m_clrOutLineBottomRightDown);
		else
			pDC->Draw3dRect(rcItem, pMember->m_clrInLineBottomRight, pMember->m_clrOutLineBottomRight);
	}

	rcItem.DeflateRect(1, 1);
}

void CExButtonApp::DrawItemBitmap(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CMemberBitmap* pMember = (CMemberBitmap*)m_pCurMember;
	
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	
	if(pMember->m_hDIB && pMember->m_hDIBDown) 
	{
		if((lpDrawItemStruct->itemState & ODS_SELECTED))
		{
			if(m_bPushed)
				DrawDIB(pDC, pMember->m_hDIB);
			else
				DrawDIB(pDC, pMember->m_hDIBDown);

			if(m_bToggleMode)
				m_bPushed = !m_bPushed;
		}
		else
		{
			if((lpDrawItemStruct->itemState & ODS_DISABLED) && pMember->m_hDIBDis)
				DrawDIB(pDC, pMember->m_hDIBDis);
			else
			{
				if(m_bPushed)
					DrawDIB(pDC, pMember->m_hDIBDown);
				else
					DrawDIB(pDC, pMember->m_hDIB);
			}
		}
		
		if(lpDrawItemStruct->itemState & ODS_FOCUS)
		{
			CRect rcBtn(lpDrawItemStruct->rcItem);
			int nDeflate = min(DEFLATE, min(rcBtn.Width(), rcBtn.Height()) >> 2);
			rcBtn.DeflateRect(nDeflate, nDeflate);
			pDC->DrawFocusRect(&rcBtn);
		}
	}
	else 
	{
		CRect rect(lpDrawItemStruct->rcItem);
		pDC->DrawText("NO_IMG", rect, DT_CENTER|DT_VCENTER);
	}
}

void CExButtonApp::DrawDIB(CDC* pDC, HGLOBAL hDIB)
{
	if(!hDIB)
		return;

	LPVOID lpDIBBits = NULL;		// Pointer to DIB bits

	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB;
	int nColors = bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	if(bmInfo.bmiHeader.biBitCount > 8)
		lpDIBBits = (LPVOID)((LPDWORD)(bmInfo.bmiColors +
			bmInfo.bmiHeader.biClrUsed) +
			((bmInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bmInfo.bmiColors + nColors);
	
	::SetDIBitsToDevice(pDC->m_hDC, 			// hDC
				   0,							// DestX
				   0,							// DestY
				   bmInfo.bmiHeader.biWidth,	// nDestWidth
				   bmInfo.bmiHeader.biHeight,	// nDestHeight
				   0,							// SrcX
				   0,							// SrcY
				   0,							// nStartScan
				   bmInfo.bmiHeader.biHeight,	// nNumScans
				   lpDIBBits,					// lpBits
				   (LPBITMAPINFO)hDIB,			// lpBitsInfo
				   DIB_RGB_COLORS); 			// wUsage
}

BOOL CExButtonApp::SetBitmapFile(LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis)
{
	CMemberBitmap* pMember = (CMemberBitmap*)m_pCurMember;

	LoadBMP(lpszUp, &(pMember->m_hDIB));
	LoadBMP(lpszDown, &(pMember->m_hDIBDown));
	LoadBMP(lpszDis, &(pMember->m_hDIBDis));

	return TRUE;
}

BOOL CExButtonApp::LoadBMP(LPCTSTR lpszBMPFile, HGLOBAL* phDIB)
{
	if(!lpszBMPFile)
		return FALSE;

	CFile file;
	if( !file.Open( lpszBMPFile, CFile::modeRead) )
		return FALSE;

	CMemberBitmap* pMember = (CMemberBitmap*)m_pCurMember;

	BITMAPFILEHEADER bmfHeader;
	long nFileLen = file.GetLength();

	// Read file header
	if (file.Read((LPSTR)&bmfHeader, sizeof(bmfHeader)) != sizeof(bmfHeader))
		return FALSE;

	// File type should be 'BM'
	if (bmfHeader.bfType != ((WORD) ('M' << 8) | 'B'))
		return FALSE;

	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nFileLen);
	if (hDIB == 0)
		return FALSE;

	// Read the remainder of the bitmap file.
	if (file.Read((LPSTR)hDIB, nFileLen - sizeof(BITMAPFILEHEADER)) !=
		nFileLen - sizeof(BITMAPFILEHEADER) )
	{
		::GlobalFree(hDIB);
		return FALSE;
	}
	
	BITMAPINFO &bmInfo = *(LPBITMAPINFO)hDIB ;
	pMember->m_nWidth = bmInfo.bmiHeader.biWidth;
	pMember->m_nHeight = bmInfo.bmiHeader.biHeight;

	bmInfo.bmiHeader.biClrUsed ? bmInfo.bmiHeader.biClrUsed : 
						1 << bmInfo.bmiHeader.biBitCount;

	*phDIB = hDIB;
	
	return TRUE;
}

BOOL CExButtonApp::SetDrawButton(UINT nButton)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	switch(nButton)
	{
	case SKINBTN_DEFAULT:
		pMember->SetDefaultColor();
		break;
	case SKINBTN_ACCENT:
		pMember->SetAccentColor();
		break;
	case SKINBTN_TAB:
		pMember->SetTabColor();
		break;
	case SKINBTN_BUY:
		pMember->SetBuyColor();
		break;
	case SKINBTN_SELL:
		pMember->SetSellColor();
		break;
	case SKINBTN_MODIFY:
		pMember->SetModifyColor();
		break;
	case SKINBTN_CANCEL:
		pMember->SetCancelColor();
		break;
	}

	return TRUE;
}

BOOL CExButtonApp::SetImageListButton(CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText, COLORREF clrText, COLORREF clrTextDis)
{
	CMemberImgList* pMember = (CMemberImgList*)m_pCurMember;
	pMember->SetMemeberImgList(pImageList, nNormal, nDown, nDisable, nOver, lpszText, clrText, clrTextDis);

	return TRUE;
}

void CExButtonApp::DrawItemImageList(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CMemberImgList* pMember = (CMemberImgList*)m_pCurMember;
	if(pMember->m_pImageList == NULL)
		return;

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);	// DC 를 얻어온다.
	CRect rc(lpDrawItemStruct->rcItem);
	
	if (lpDrawItemStruct->itemState & ODS_DISABLED)		// 비활성 상태
		pMember->m_pImageList->Draw(pDC, pMember->m_nDisable, CPoint(0, 0), ILD_TRANSPARENT);
	else
	{
		if (lpDrawItemStruct->itemState & ODS_SELECTED)	// 버튼다운상태
			pMember->m_pImageList->Draw(pDC, pMember->m_nDown, CPoint(0, 0), ILD_TRANSPARENT);
		else																// 보통상태
			pMember->m_pImageList->Draw(pDC, pMember->m_nNormal, CPoint(0, 0), ILD_TRANSPARENT);
	}

	if(lpDrawItemStruct->itemState & ODS_FOCUS)
	{
		int nDeflate = min(DEFLATE + 2, min(rc.Width(), rc.Height()) >> 2);
		rc.DeflateRect(nDeflate, nDeflate);
		pDC->DrawFocusRect(&rc);
	}
	
	DrawButtonText_ImgList(pDC, lpDrawItemStruct->itemState, rc);
}

BOOL CExButtonApp::SetIcon(CSize size, HICON hIcon, HICON hIconPushed, UINT nIconAlign)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	pMember->m_hIcon = hIcon;
	pMember->m_hIconPushed = hIconPushed;
	pMember->m_sizeImage = size;
	pMember->m_nIconAlign = nIconAlign;

	return TRUE;
}

void CExButtonApp::DrawButtonIcon(CDC* pDC, UINT nState, CRect& rcItem)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	HICON hIcon = pMember->m_hIcon;
	if(hIcon)
	{
        CPoint point = CalculateImagePosition(pDC, nState, rcItem, (pMember->m_hIconPushed != NULL));

		if(pMember->m_hIconPushed && (nState & ODS_SELECTED)) 
			hIcon = pMember->m_hIconPushed;

		DWORD dwFlags = DST_ICON;
		if(nState & ODS_DISABLED)
			dwFlags |= DSS_DISABLED;
		else
			dwFlags |= DSS_NORMAL;

		pDC->DrawState(point, pMember->m_sizeImage, hIcon, dwFlags, (HBRUSH)NULL);
	}
}

CPoint CExButtonApp::CalculateImagePosition(CDC* pDC, UINT nState, CRect& rcItem, bool bHasPushedImage)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;

	CString strText(pMember->m_lpszText);
	CSize   sizeText(pDC->GetTextExtent(strText));
	CPoint  point(rcItem.TopLeft());

	DWORD dwStyle = pMember->m_pButton->GetStyle();
	
	OffsetPoint(point, pMember->m_sizeImage, rcItem);
//    point.x = GAP_BORDER;

    if(pMember->m_nIconAlign == BTN_ICON_TWOROWS) 
    {
        if(!strText.IsEmpty())
			point.y -= ((sizeText.cy/2)+(GAP_IMAGE/2));
		
        if((dwStyle & BS_CENTER) == BS_CENTER || (dwStyle & BS_CENTER) == 0)
            point.x = rcItem.left + __max(0, ((rcItem.Width() - pMember->m_sizeImage.cx) / 2));
        else if(dwStyle & BS_RIGHT)
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
			if((dwStyle & BS_CENTER) == BS_CENTER)
	        {
				int cx = rcItem.Width();
				if(cx >= pMember->m_sizeImage.cx)
					point.x = (cx - pMember->m_sizeImage.cx) / 2;
			}
			if((dwStyle & BS_VCENTER) == BS_VCENTER)
			{
				int cy = rcItem.Height();
				if(cy >= pMember->m_sizeImage.cy)
					point.y = (cy - pMember->m_sizeImage.cy) / 2;
			}
		}
	}

	if(nState & ODS_SELECTED)
	{
		if(!bHasPushedImage) 
			point.Offset(1, 1);
	}

    return point;
}

void CExButtonApp::OffsetPoint(CPoint& point, CSize size, CRect rcClient)
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
//		point.y += __max(0, ((rcClient.Height()-size.cy) / 2));
		point.y = int(rcClient.top + double(rcClient.bottom - rcClient.top - size.cy)/2.0);
	}
}

int CExButtonApp::ExitInstance() 
{
	return CWinApp::ExitInstance();
}

void CExButtonApp::SetText(LPCTSTR lpszText)
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	pMember->SetText(lpszText);

	pMember->m_pButton->Invalidate();
}

LPVOID CExButtonApp::InitCheckGroupBox(LPVOID lpPointer)
{
	CMember* pMember = new CMemberChkGrpBox(lpPointer);
	return pMember;
}

void CExButtonApp::PreSubclassWindow_CheckGroupBox()
{
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;
	CExCheckGroupBox* pBox = (CExCheckGroupBox*)pMember->m_pControl;

	pBox->ModifyStyle(0, BS_ICON|WS_TABSTOP|WS_GROUP);

	CString	strTitle;
	pBox->GetWindowText(strTitle);

	int nWidth = MoveTitle();

	CRect	r;
	pBox->GetWindowRect(&r);
	pBox->ScreenToClient(r);

	r.OffsetRect(LEFT_OFFSET, 0);
	r.bottom = r.top + CHECKBOX_SIZE;
	r.right = r.left + CHECKBOX_SIZE + nWidth;
	
	pMember->m_btnCheckBox.Create(strTitle, WS_CHILD|BS_AUTOCHECKBOX, r, pBox, ID_CHECKBOX);
	pMember->m_btnCheckBox.SetFont(pBox->GetFont(), true);
	pMember->m_btnCheckBox.ShowWindow(SW_SHOW);
}

int CExButtonApp::MoveTitle()
{
	if(!m_pCurMember)
		return 0;
	
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;
	CExCheckGroupBox* pBox = (CExCheckGroupBox*)pMember->m_pControl;

	// The group box title needs to be erased, but I need to keep the border away from the check box text.
	// I create a string of spaces (' ') that is the same length as the title was plus the size of the checkbox.
	// Plus a little more.

	CString	strOldTitle, strNewTitle;
	pBox->GetWindowText(strOldTitle);

	CClientDC dc(pBox);
	CFont*	  pOldFont = dc.SelectObject(pBox->GetFont());

	CSize	czText	= dc.GetTextExtent(strOldTitle);
	int		nRet	= czText.cx;
	int		nTarget = czText.cx + CHECKBOX_SIZE + 5;
	
	while(czText.cx < nTarget)
	{
		strNewTitle.Insert(0, ' ');
		czText = dc.GetTextExtent(strNewTitle);
	}

	dc.SelectObject(pOldFont);

	pBox->SetWindowText(strNewTitle);
	return nRet;
}

void CExButtonApp::SetCheck_CheckGroupBox(int nCheck)
{
	if(!m_pCurMember)
		return;
	
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;

	pMember->m_btnCheckBox.SetCheck(nCheck);
	GroupboxControls(nCheck==0? false:true);
}

int CExButtonApp::GetCheck_CheckGroupBox()
{
	if(!m_pCurMember)
		return -1;
	
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;

	return pMember->m_btnCheckBox.GetCheck();
}

void CExButtonApp::OnSetFocus_CheckGroupBox(CWnd* pOldWnd) 
{
	if(!m_pCurMember)
		return;
	
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;

	pMember->m_pButton->CButton::OnSetFocus(pOldWnd);	
	pMember->m_btnCheckBox.SetFocus();
}

LRESULT CExButtonApp::OnGetCheck_CheckGroupBox(WPARAM wParam, LPARAM lParam)
{
	return GetCheck_CheckGroupBox();
}

LRESULT CExButtonApp::OnSetCheck_CheckGroupBox(WPARAM wParam, LPARAM lParam)
{
	SetCheck_CheckGroupBox((int)wParam);
	return 0;
}

void CExButtonApp::OnEnable_CheckGroupBox(BOOL bEnable) 
{
	if(!m_pCurMember)
		return;
	
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;

	pMember->m_pButton->CButton::OnEnable(bEnable);
	
	pMember->m_btnCheckBox.EnableWindow(bEnable);
	GroupboxControls((BOOL)bEnable && GetCheck_CheckGroupBox());
}

void CExButtonApp::GroupboxControls(BOOL bActivate)
{
	if(!m_pCurMember)
		return;

	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;

	// Enable/Disable all controls that fall wholey inside of me.
	// Brute force: check every control in the dialog.

	CWnd*	pParent = pMember->m_pButton->GetParent();
	CWnd*	pCtrl = pParent->GetWindow(GW_CHILD);	// Gets first child

	CRect	rCtrl, rTest;
	CRect	rGroup;
	pMember->m_pButton->GetWindowRect(&rGroup);

	while(pCtrl)
	{
		// Don't do myself !!		
		if(pCtrl->GetSafeHwnd() == pMember->m_pButton->GetSafeHwnd())
		{
			pCtrl = pCtrl->GetWindow(GW_HWNDNEXT);
			continue;
		}

		if(pMember->m_nStyle == TCH_IGNORE && (short)pCtrl->GetDlgCtrlID() == IDC_STATIC)
		{
			pCtrl = pCtrl->GetWindow(GW_HWNDNEXT);
			continue;
		}

		pCtrl->GetWindowRect(&rCtrl);

		if(rTest.IntersectRect(rGroup, rCtrl))
			pCtrl->EnableWindow(bActivate);

		pCtrl = pCtrl->GetWindow(GW_HWNDNEXT);
	}
}

void CExButtonApp::OnClicked_CheckGroupBox() 
{
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;

	GroupboxControls(GetCheck_CheckGroupBox());

	::SendMessage(pMember->m_pButton->GetParent()->m_hWnd, WM_COMMAND, MAKEWPARAM(::GetDlgCtrlID(pMember->m_pButton->m_hWnd), BN_CLICKED), (LPARAM)pMember->m_pButton->m_hWnd);
}

void CExButtonApp::SetStyle_CheckGroupBox(TouchStyle nStyle)
{
	CMemberChkGrpBox* pMember = (CMemberChkGrpBox*)m_pCurMember;
	pMember->m_nStyle = nStyle;

	GroupboxControls(GetCheck_CheckGroupBox());
}

FpChangeSkinNotify CExButtonApp::GetSkinNotifyFunction()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	return pMember->GetNotifySkinTableFunction();
}

void CExButtonApp::SetColor_CheckBox(COLORREF clrText, COLORREF clrBackground)
{
	CMemberCheckBox* pMember = (CMemberCheckBox*)m_pCurMember;
	pMember->SetColor(clrBackground, clrText);
}

void CExButtonApp::SetSkinString_CheckBox(LPCTSTR lpszText, LPCTSTR lpszBackground)
{
	CMemberCheckBox* pMember = (CMemberCheckBox*)m_pCurMember;
	pMember->SetSkinTable(m_pISkinColorManager->GetIndex(lpszBackground), m_pISkinColorManager->GetIndex(lpszText));
}

void CExButtonApp::SetSkinIndex_CheckBox(UINT nText, UINT nBackground)
{
	CMemberCheckBox* pMember = (CMemberCheckBox*)m_pCurMember;
	pMember->SetSkinTable(nBackground, nText);
}

HBRUSH CExButtonApp::CtlColor_CheckBox(CDC* pDC, UINT nCtlColor)
{
	CMemberCheckBox* pMember = (CMemberCheckBox*)m_pCurMember;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(pMember->m_clrBackground);
	pDC->SetTextColor(pMember->m_clrText);

	return pMember->m_brush;
}

void CExButtonApp::SetText_ImgList(LPCTSTR lpszText)
{
	CMemberImgList* pMember = (CMemberImgList*)m_pCurMember;
	pMember->SetText(lpszText);

	pMember->m_pButton->Invalidate();
}

void CExButtonApp::PreSubclassWindow_DrawButton()
{
	CMemberDraw* pMember = (CMemberDraw*)m_pCurMember;
	CString strText;
	pMember->m_pButton->GetWindowText(strText);

	pMember->SetText(strText);
}

LPCTSTR CExButtonApp::GetText_DrawButton(LPVOID lpPointer)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	return pMember->GetText();
}

LPCTSTR CExButtonApp::GetText_ImgList(LPVOID lpPointer)
{
	CMemberImgList* pMember = (CMemberImgList*)lpPointer;
	return pMember->GetText();
}

void CExButtonApp::UseToggleMode(LPVOID lpPointer, BOOL bUse)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	pMember->UseToggleMode(bUse);

	if(!bUse)
	{
		pMember->m_bPushed = FALSE;
		pMember->m_pButton->Invalidate();

		pMember->m_listToggleGroup.RemoveAll();
	}
}

BOOL CExButtonApp::GetToggleState(LPVOID lpPointer)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	return pMember->GetToggleState();
}

void CExButtonApp::SetToggleState(LPVOID lpPointer, BOOL bPush)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;

	if(!pMember->m_bUseToggleMode || pMember->m_bPushed)
		return;

	pMember->SetToggleState(bPush);

	pMember->m_pButton->Invalidate();

	POSITION pos = pMember->m_listToggleGroup.GetHeadPosition();
	while(pos)
	{
		CMemberDraw* pAddMember = (CMemberDraw*)pMember->m_listToggleGroup.GetNext(pos);
		if(pMember->m_bPushed)
		{
			pAddMember->m_bPushed = FALSE;
			pAddMember->m_pButton->Invalidate();
		}
	}
}

void CExButtonApp::ReleasePushed_DrawButton(LPVOID lpPointer)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	if(!pMember->m_bPushed)
		return;

	pMember->m_bPushed = FALSE;
	pMember->m_pButton->Invalidate();
}

void CExButtonApp::OnClicked_DrawButton(LPVOID lpPointer)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	if(!pMember->m_bUseToggleMode || pMember->m_bPushed)
		return;

	pMember->m_bPushed = !pMember->m_bPushed;
	pMember->m_pButton->Invalidate();

	POSITION pos = pMember->m_listToggleGroup.GetHeadPosition();
	while(pos)
	{
		CMemberDraw* pAddMember = (CMemberDraw*)pMember->m_listToggleGroup.GetNext(pos);
		if(pMember->m_bPushed)
		{
			pAddMember->m_bPushed = FALSE;
			pAddMember->m_pButton->Invalidate();
		}
	}
}

void CExButtonApp::AddToggleGroup_DrawButton(LPVOID lpPointer, LPVOID lpAddPointer)
{
	CMemberDraw* pMember = (CMemberDraw*)lpPointer;
	pMember->m_listToggleGroup.AddTail(lpAddPointer);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CExButtonApp object

CExButtonApp theApp;

extern "C" __declspec(dllexport) long Ex_ActionControl(LPVOID lpPointer, LPVOID lpAction)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	long lResult = theApp.ActionControl(lpAction);
	theApp.m_pCurMember = NULL;

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_ExitButton(LPVOID lpPointer)
{
	theApp.ExitButton(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitBitmapButton(LPVOID lpPointer)
{
	return theApp.InitBitmapButton(lpPointer);
}

extern "C" __declspec(dllexport) BOOL Ex_SetBitmapFile(LPVOID lpPointer, LPCTSTR lpszUp, LPCTSTR lpszDown, LPCTSTR lpszDis)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetBitmapFile(lpszUp, lpszDown, lpszDis);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_DrawItemBitmap(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.DrawItemBitmap(lpDrawItemStruct);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) LPVOID Ex_InitDrawButton(LPVOID lpPointer)
{
	return theApp.InitDrawButton(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitDrawButton2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	return theApp.InitDrawButton(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) BOOL Ex_SetDrawButton(LPVOID lpPointer, UINT nButton)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetDrawButton(nButton);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_DrawItemButton(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.DrawItemButton(lpDrawItemStruct);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) LPVOID Ex_InitImgListButton(LPVOID lpPointer)
{
	return theApp.InitImageListButton(lpPointer);
}

extern "C" __declspec(dllexport) BOOL Ex_SetImageListButton(LPVOID lpPointer, CImageList* pImageList, int nNormal, int nDown, int nDisable, int nOver, LPCTSTR lpszText, COLORREF clrText, COLORREF clrTextDis)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetImageListButton(pImageList, nNormal, nDown, nDisable, nOver, lpszText, clrText, clrTextDis);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_DrawItemImageList(LPVOID lpPointer, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.DrawItemImageList(lpDrawItemStruct);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) BOOL Ex_SetIcon(LPVOID lpPointer, CSize size, HICON hIcon, HICON hIconPushed, UINT nIconAlign)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	BOOL bResult = theApp.SetIcon(size, hIcon, hIconPushed, nIconAlign);
	theApp.m_pCurMember = NULL;

	return bResult;
}

extern "C" __declspec(dllexport) void Ex_SetText(LPVOID lpPointer, LPCTSTR lpszText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetText(lpszText);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) LPVOID Ex_InitCheckGroupBox(LPVOID lpPointer)
{
	return theApp.InitCheckGroupBox(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_PreSubclassWindow_CheckGroupBox(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.PreSubclassWindow_CheckGroupBox();
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetCheck_CheckGroupBox(LPVOID lpPointer, int nCheck)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetCheck_CheckGroupBox(nCheck);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) int Ex_GetCheck_CheckGroupBox(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	int nResult = theApp.GetCheck_CheckGroupBox();;
	theApp.m_pCurMember = NULL;

	return nResult;
}

extern "C" __declspec(dllexport) void Ex_OnSetFocus_CheckGroupBox(LPVOID lpPointer, CWnd* pOldWnd)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnSetFocus_CheckGroupBox(pOldWnd);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) LRESULT Ex_OnGetCheck_CheckGroupBox(LPVOID lpPointer, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	LRESULT lResult = theApp.OnGetCheck_CheckGroupBox(wParam, lParam);
	theApp.m_pCurMember = NULL;

	return lResult;
}

extern "C" __declspec(dllexport) LRESULT Ex_OnSetCheck_CheckGroupBox(LPVOID lpPointer, WPARAM wParam, LPARAM lParam)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	LRESULT lResult = theApp.OnSetCheck_CheckGroupBox(wParam, lParam);
	theApp.m_pCurMember = NULL;

	return lResult;
}

extern "C" __declspec(dllexport) void Ex_OnEnable_CheckGroupBox(LPVOID lpPointer, BOOL bEnable)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnEnable_CheckGroupBox(bEnable);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_OnClicked_CheckGroupBox(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.OnClicked_CheckGroupBox();
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetStyle_CheckGroupBox(LPVOID lpPointer, TouchStyle nStyle)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetStyle_CheckGroupBox(nStyle);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) FpChangeSkinNotify Ex_GetSkinNotifyFunction(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	FpChangeSkinNotify pChangeSkinNotify = theApp.GetSkinNotifyFunction();
	theApp.m_pCurMember = NULL;

	return pChangeSkinNotify;
}

extern "C" __declspec(dllexport) LPVOID Ex_InitCheckBox(LPVOID lpPointer)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.InitCheckBox(lpPointer);
}

extern "C" __declspec(dllexport) LPVOID Ex_InitCheckBox2(LPVOID lpPointer, BOOL bUseDefSkin)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return theApp.InitCheckBox(lpPointer, bUseDefSkin);
}

extern "C" __declspec(dllexport) void Ex_SetColor_CheckBox(LPVOID lpPointer, COLORREF clrText, COLORREF clrBackground)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetColor_CheckBox(clrText, clrBackground);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetSkinString_CheckBox(LPVOID lpPointer, LPCTSTR lpszText, LPCTSTR lpszBackground)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetSkinString_CheckBox(lpszText, lpszBackground);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetSkinIndex_CheckBox(LPVOID lpPointer, UINT nText, UINT nBackground)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetSkinIndex_CheckBox(nText, nBackground);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_CtlColor_CheckBox(LPVOID lpPointer, CDC* pDC, UINT nCtlColor)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.CtlColor_CheckBox(pDC, nCtlColor);
//	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_SetText_ImgList(LPVOID lpPointer, LPCTSTR lpszText)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.SetText_ImgList(lpszText);
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) void Ex_PreSubclassWindow_DrawButton(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	theApp.PreSubclassWindow_DrawButton();
	theApp.m_pCurMember = NULL;
}

extern "C" __declspec(dllexport) LPCTSTR Ex_GetText_DrawButton(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	return theApp.GetText_DrawButton(lpPointer);
}

extern "C" __declspec(dllexport) LPCTSTR Ex_GetText_ImgList(LPVOID lpPointer)
{
	theApp.m_pCurMember = (CMember*)lpPointer;
	return theApp.GetText_ImgList(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_UseToggleMode_DrawButton(LPVOID lpPointer, BOOL bUse)
{
	theApp.UseToggleMode(lpPointer, bUse);
}

extern "C" __declspec(dllexport) BOOL Ex_GetToggleState_DrawButton(LPVOID lpPointer)
{
	return theApp.GetToggleState(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_SetToggleState_DrawButton(LPVOID lpPointer, BOOL bPush)
{
	theApp.SetToggleState(lpPointer, bPush);
}

extern "C" __declspec(dllexport) void Ex_ReleasePushed_DrawButton(LPVOID lpPointer)
{
	theApp.ReleasePushed_DrawButton(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_OnClicked_DrawButton(LPVOID lpPointer)
{
	theApp.OnClicked_DrawButton(lpPointer);
}

extern "C" __declspec(dllexport) void Ex_AddToggleGroup_DrawButton(LPVOID lpPointer, LPVOID lpAddPointer)
{
	theApp.AddToggleGroup_DrawButton(lpPointer, lpAddPointer);
}
