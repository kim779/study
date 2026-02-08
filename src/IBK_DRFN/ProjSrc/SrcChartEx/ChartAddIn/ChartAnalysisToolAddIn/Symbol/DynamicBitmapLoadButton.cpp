// DynamicBitmapLoadButton.cpp : implementation file
//

#include "stdafx.h"
#include "DynamicBitmapLoadButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDynamicBitmapLoadButton
void ShowErrorMessage()
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	::MessageBox( NULL, (LPCTSTR)lpMsgBuf, "Error", MB_OK | MB_ICONINFORMATION );
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

BEGIN_MESSAGE_MAP(CDynamicBitmapLoadButton, CButton)
	//{{AFX_MSG_MAP(CDynamicBitmapLoadButton)
	ON_WM_KEYUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicBitmapLoadButton message handlers

CDynamicBitmapLoadButton::CDynamicBitmapLoadButton()
{
	m_bToggle = false;
}

void CDynamicBitmapLoadButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    HDC hdcMem; 
 
    hdcMem = CreateCompatibleDC(lpDrawItemStruct->hDC); 
 
	CBitmap currentBitmap;
    if (lpDrawItemStruct->itemState & ODS_SELECTED){  // if selected 
		m_TYPEButton = Button_Down;
		GetBitmap(&currentBitmap, m_downImageFileName);
		SelectObject(hdcMem, currentBitmap);
	}
    else{
		m_TYPEButton = Button_Up;
		GetBitmap(&currentBitmap, m_upImageFileName);
        SelectObject(hdcMem, currentBitmap);
	}

	BITMAP bmInfo;
	currentBitmap.GetObject(sizeof(bmInfo), &bmInfo);

    // Destination 
    StretchBlt( 
        lpDrawItemStruct->hDC,         // destination DC 
        lpDrawItemStruct->rcItem.left, // x upper left 
        lpDrawItemStruct->rcItem.top,  // y upper left 

        // The next two lines specify the width and 
        // height. 
        lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left, 
        lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top, 
        hdcMem,    // source device context 
        0, 0,      // x and y upper left 
		bmInfo.bmWidth,
		bmInfo.bmHeight,
        SRCCOPY);  // raster operation 

    DeleteDC(hdcMem);
}

// public =================================================================================
// 이미지를 버튼 크기에 맞춘다.
void CDynamicBitmapLoadButton::SizeToContent()
{
	if(m_TYPEButton == Button_Up)
		SizeToContent(m_upImageFileName);
	else if(m_TYPEButton == Button_Down)
		SizeToContent(m_downImageFileName);
}

void CDynamicBitmapLoadButton::Load(LPCSTR upImageFileName, LPCSTR downImageFileName)
{
	m_upImageFileName = upImageFileName;
	m_downImageFileName = downImageFileName;
}

// private ================================================================================
// 해당 image file을 CBitmap에 attach한다.
void CDynamicBitmapLoadButton::GetBitmap(CBitmap* bitmap, const CString& imageFileName)
{
	HBITMAP hBitmap = NULL; 
	hBitmap = (HBITMAP)LoadImage(NULL, imageFileName, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_DEFAULTSIZE);

	if (hBitmap == NULL)
	{
		ShowErrorMessage();
	}

	bitmap->Attach(hBitmap);
}

// 해당 file의 image를 button크기에 맞춘다.
void CDynamicBitmapLoadButton::SizeToContent(const CString& imageFileName)
{
	CBitmap currentBitmap;
	GetBitmap(&currentBitmap, imageFileName);
	CSize bitmapSize;
	BITMAP bmInfo;
	VERIFY(currentBitmap.GetObject(sizeof(bmInfo), &bmInfo) == sizeof(bmInfo));
	VERIFY(SetWindowPos(NULL, -1, -1, bmInfo.bmWidth, bmInfo.bmHeight,
			SWP_NOMOVE|SWP_NOZORDER|SWP_NOREDRAW|SWP_NOACTIVATE));
}

//이곳에서 toggle을 설정하지 않으면..
//차트 화면에 기호를 그릴때 toggle이 사라진다.
void CDynamicBitmapLoadButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_bToggle == true)  
		SetState(TRUE);
	else 
		SetState(FALSE);
	return;
	CButton::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CDynamicBitmapLoadButton::SetToggleState(const bool bToggle)
{
	m_bToggle = bToggle;
	SetState(bToggle);
}

bool CDynamicBitmapLoadButton::GetToggleState()
{
	return m_bToggle;
}	
