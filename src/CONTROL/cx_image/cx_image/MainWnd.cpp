// MainWnd.cpp : implementation file
//

#include "stdafx.h"
#include "cx_image.h"
#include "MainWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainWnd
#define TM_TOOLTIP  9898

CMainWnd::CMainWnd(CWnd* pParent, void* pParam)
{
	EnableAutomation();
	m_pWizard = pParent;

	m_Param.key = ((struct _param*)pParam)->key;
	m_Param.name = CString(((struct _param*)pParam)->name, ((struct _param*)pParam)->name.GetLength());
	m_Param.rect = CRect(((struct _param*)pParam)->rect.left, ((struct _param*)pParam)->rect.top, ((struct _param*)pParam)->rect.right, ((struct _param*)pParam)->rect.bottom);
	m_Param.fonts = CString(((struct _param*)pParam)->fonts, ((struct _param*)pParam)->fonts.GetLength());
	m_Param.point = ((struct _param*)pParam)->point;
	m_Param.style = ((struct _param*)pParam)->style;
	m_Param.tRGB = ((struct _param*)pParam)->tRGB;
	m_Param.pRGB = ((struct _param*)pParam)->pRGB;
	m_Param.options = CString(((struct _param*)pParam)->options, ((struct _param*)pParam)->options.GetLength());

	CString sval;
	sval = m_Param.options;
	if (sval.Find("k3") >= 0)
	{	
		CString stmp;
		stmp = Parser(sval, "/f");
		stmp = Parser(sval, "/t");
		stmp.MakeUpper();
	
		CString strHome, strPath;
		strHome = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
		strPath = strHome + "\\image\\" + stmp;
	
		m_hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strPath,
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
		if (m_hBmp)
			m_pStrechBmp = CBitmap::FromHandle(m_hBmp);
		
		int icol = atoi(m_Param.options.Mid(m_Param.options.Find("t") + 1));
		m_clrBack = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)icol);
	}
	//m_Param.options = m_Param.options.Right(2);
}

CString CMainWnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strRet;

	const	char* pRet = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)strData.GetString());
	if ((long)pRet > 1)
		strRet = pRet;

	return strRet;
}

CMainWnd::~CMainWnd()
{
}

void CMainWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	//{{AFX_MSG_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CMainWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CMainWnd)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CMainWnd, "SetImageFile", dispidSetImageFile, SetImageFile, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CMainWnd, "SetRect", dispidSetRect, SetRect, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION_ID(CMainWnd, "SetToolTipText", dispidSetToolTipText, SetToolTipText, VT_EMPTY, VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IMainWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {04EC68BD-8B6E-4C5C-82FA-47F2450863FF}
static const IID IID_IMainWnd =
{ 0x4ec68bd, 0x8b6e, 0x4c5c, { 0x82, 0xfa, 0x47, 0xf2, 0x45, 0x8, 0x63, 0xff } };

BEGIN_INTERFACE_MAP(CMainWnd, CWnd)
	INTERFACE_PART(CMainWnd, IID_IMainWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainWnd message handlers


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetWindowPos(nullptr, 0, 0, m_Param.rect.Width(), m_Param.rect.Height(), SWP_NOMOVE);
	SetTimer(TM_TOOLTIP, 1000, nullptr);
	

	return 0;
}

CString CMainWnd::Parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rec;
	GetClientRect(rec);

	if(m_Param.options.Find("k3") >= 0 && m_pStrechBmp)
	{
		dc.FillSolidRect(rec, m_clrBack);
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);
		BITMAP bmpInfo;
		m_pStrechBmp->GetBitmap(&bmpInfo);

		CBitmap* pOldBmp = nullptr;
		pOldBmp = (CBitmap*)memDC.SelectObject(m_pStrechBmp);

		::TransparentBlt(dc.m_hDC, rec.left, rec.top, bmpInfo.bmWidth, bmpInfo.bmHeight, memDC.m_hDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, RGB(255, 0, 255));

		memDC.SelectObject(pOldBmp);
		memDC.DeleteDC();
	}
	else if (m_Param.options.Find("k1") >= 0) //GID
	{
		CDC memDC;
		CBitmap* pOldBmp = nullptr;
		BITMAP bmpInfo;
		memDC.CreateCompatibleDC(&dc);

		dc.SetStretchBltMode(HALFTONE);
		
		if (memDC.m_hDC != NULL)
		{
			if (m_pStrechBmp)
			{
				m_pStrechBmp->GetBitmap(&bmpInfo);
				pOldBmp = (CBitmap*)memDC.SelectObject(m_pStrechBmp);

				dc.StretchBlt(rec.left, rec.top, rec.Width(), rec.Height(),
					&memDC, 0, 0, bmpInfo.bmWidth, bmpInfo.bmHeight, SRCCOPY);

				if (pOldBmp)
					memDC.SelectObject(pOldBmp);
			}
		}
		memDC.DeleteDC();
	}
	else if (m_Param.options.Find("k2") >= 0)//GID+
	{
		if (m_pGdiBitmap)
		{
			Gdiplus::Graphics graphics(dc.m_hDC);
			// 이미지 크기 조정하여 그리기
			graphics.DrawImage(m_pGdiBitmap,
				0, 0,                     // 그릴 위치 (좌상단)
				rec.Width() + 1, rec.Height()); // 그릴 크기 (클라이언트 영역 크기
		}
		
		//CString m_strimagepath;
		//m_strimagepath = "F:\\util\\HTS\\IBK투자증권 HTS\\image\\event01.bmp";
		//m_hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), m_strimagepath,
		//	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

		//Graphics graphics(dc.GetSafeHdc());
		//// BMP 파일 로드
		//if (m_hBmp)
		//{
		//	// 원하는 위치와 크기에 이미지를 그립니다.
		//	DrawBitmapWithGDIPlus(&graphics, m_hBmp, rec.left, rec.top, rec.Width(), rec.Height());
		//	// 리소스 해제
		//	::DeleteObject(m_hBmp);
		//}
	}

	
}


void CMainWnd::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnClose();
}


void CMainWnd::OnDestroy()
{
	CWnd::OnDestroy();
	GdiplusShutdown(gdiplusToken);

	if (m_pTooltip)
	{
		m_pTooltip->DestroyWindow();
		delete m_pTooltip;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	Invalidate();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CMainWnd::DrawBitmapWithGDIPlus(Graphics* pGraphics, HBITMAP hBmp, int x, int y, int width, int height)
{
	Bitmap bmp(hBmp, NULL);

	// Set high-quality interpolation mode for better scaling.
	pGraphics->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	// Draw the image scaled.
	pGraphics->DrawImage(&bmp, x, y, width, height);
}

void CMainWnd::SetImageFile(BSTR sImage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strHome, strPath, stemp;
	strHome = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	stemp.Format("%s", sImage);
	stemp.Trim();
	strPath = strHome + "\\image\\" + stemp;
	
	m_hBmp = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), strPath,
	IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (m_hBmp)
		m_pStrechBmp = CBitmap::FromHandle(m_hBmp);
	else
		Variant(guideCC, strPath);

	CStringW str1;
	str1 = CA2W(strPath, CP_ACP);
	m_pGdiBitmap = Gdiplus::Bitmap::FromFile(str1);
	Invalidate();
}


void CMainWnd::SetRect(SHORT left, SHORT top, SHORT right, SHORT bottom)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CRect rec;
	rec.SetRect(static_cast<int>(left), static_cast<int>(top), static_cast<int>(right), static_cast<int>(bottom));
	MoveWindow(rec);
	Invalidate();
}


void CMainWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonDown(nFlags, point);

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick/*Click*/)),
		(LPARAM)m_Param.name.GetString());
}


void CMainWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnLButtonDblClk(nFlags, point);

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*Click*/)),
		(LPARAM)m_Param.name.GetString());
}


BOOL CMainWnd::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nHitTest == HTCLIENT)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_HAND));
		return TRUE;
	}
	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}


BOOL CMainWnd::PreTranslateMessage(MSG* pMsg)
{
	if (m_pTooltip->m_hWnd != nullptr)
		m_pTooltip->RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}


void CMainWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TM_TOOLTIP:
	{
		KillTimer(TM_TOOLTIP);
		m_pTooltip = new CToolTipCtrl;
		if (!m_pTooltip->Create(this, TTS_ALWAYSTIP | TTS_NOPREFIX )) {
			m_slog.Format("err=[%d] createfail", GetLastError());
			Variant(guideCC, m_slog);
			return;
		}
		m_pTooltip->AddTool(this, m_strToolText);
		m_pTooltip->SetMaxTipWidth(300);
		
		m_pTooltip->Activate(TRUE);
	}
	break;
	}
	CWnd::OnTimer(nIDEvent);
}


LRESULT CMainWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (message)
	{
	case WM_LBUTTONDOWN:
	case WM_MOUSEMOVE:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
	case WM_RBUTTONUP:
	case WM_MBUTTONUP:
	{
		if (!m_pTooltip)
			break;

		MSG	msg{};
		POINT	point;

		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);

		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		msg.pt = point;

		m_pTooltip->RelayEvent(&msg);
	}
	break;
	default:
		break;
	}

	return CWnd::WindowProc(message, wParam, lParam);
}


void CMainWnd::SetToolTipText(BSTR sText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strToolText.Format("%s", sText);
	// TODO: 여기에 디스패치 처리기 코드를 추가합니다.
}
