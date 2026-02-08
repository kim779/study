// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Amount.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TID_ONCHANGE_EVENT	10

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();
	m_capture = false;
	m_hBitmap = NULL;
	m_hBitmap_dn = NULL;
	m_hBitmap_hv = NULL;
	m_bTracking = FALSE;
	m_bHover = FALSE;
	m_bCheck = false;
	m_bChecked = false;
	m_pPopup = NULL;
}

CControlWnd::~CControlWnd()
{
	if (m_pPopup)
	{
		if (IsWindow(m_pPopup->GetSafeHwnd()))
			m_pPopup->SendMessage(WM_CLOSE);
		m_pPopup->Detach();
		delete m_pPopup; m_pPopup = NULL; 
	}
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	ON_WM_ENABLE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnMessage1)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_EX(CControlWnd, "Type", GetType, SetType, VT_I2)
	DISP_PROPERTY_EX(CControlWnd, "Enable", GetEnable, SetEnable, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "Visible", GetVisible, SetVisible, VT_BOOL)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {5957DE39-F281-4CE1-8B85-A9F95F56A2BF}
static const IID IID_IControlWnd =
{ 0x5957de39, 0xf281, 0x4ce1, { 0x8b, 0x85, 0xa9, 0xf9, 0x5f, 0x56, 0xa2, 0xbf } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		return (long)m_rtnStr.operator LPCTSTR();
		break;
	case DLL_OUB:
		{
			
		}
		break;
	case DLL_ALERT:
		WriteAlert((char*)lParam);
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		break;
	case DLL_SETFONT:
		{
			m_Param.point = HIWORD(wParam);
			CString tmp = CString((char*)lParam);
			if (!tmp.IsEmpty())
				m_Param.fonts = tmp;
			Invalidate();
		}
		break;
	}
	return 0;
}

void CControlWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	CString txt;
	GetClientRect(&rc);
	dc.FillSolidRect(rc, GetIndexColor(m_Param.pRGB));

	dc.SetBkMode(TRANSPARENT);

	if (m_hBitmap)
	{
		CDC		memDC;
		CBitmap		*pOldBitmap, *pBitmap;

		if (m_bCheck && m_bChecked && m_hBitmap_dn)
		{
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		}
		else if (m_capture && m_bHover && m_hBitmap_dn)
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		else if (m_bHover && m_hBitmap_hv)
			pBitmap = CBitmap::FromHandle(m_hBitmap_hv);
		else
			pBitmap = CBitmap::FromHandle(m_hBitmap);

		BITMAP stBitmapInfo ;
		pBitmap->GetBitmap(&stBitmapInfo);

		memDC.CreateCompatibleDC(&dc);	
		if (memDC.m_hDC != NULL) 
			pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
		//rc.DeflateRect(1, 1);
		/*
		int nX = (rc.Width() - BitmapSize(m_hBitmap).cx) / 2, 
		    nY = (rc.Height() - BitmapSize(m_hBitmap).cy) / 2;
		dc.BitBlt(nX, nY, nX + BitmapSize(m_hBitmap).cx, nY + BitmapSize(m_hBitmap).cy, &memDC, 0, 0, SRCCOPY);
		*/
		
		dc.StretchBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0,0, BitmapSize(m_hBitmap).cx, BitmapSize(m_hBitmap).cy, SRCCOPY);
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	
	if (!m_hBitmap)
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover?DFCS_PUSHED:NULL));
	
	CFont *pFont, *pOldFont;
	// set font
	pFont = getAxFont(m_Param.fonts, m_Param.point, m_Param.style);
	pOldFont = (CFont*)dc.SelectObject(pFont);
	
	if (IsWindowEnabled())
		dc.SetTextColor(RGB(0,0,0));
	else
		dc.SetTextColor(GetSysColor(COLOR_GRAYTEXT));
	m_sText.TrimLeft();
	/*
	if (m_hBitmap)
	{
		rc.right = BitmapSize(m_hBitmap).cx;
		rc.bottom = BitmapSize(m_hBitmap).cy;
	}
	*/
	
	//rc.DeflateRect(2, 2, 0, 0);
	if (m_capture && m_bHover)
		rc.OffsetRect(1, 1);
	
	dc.DrawText(m_sText, rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	dc.SelectObject(pOldFont);
}

void CControlWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CControlWnd::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_capture)
		ReleaseCapture();
	SetCapture();
	m_capture = true;
	Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CControlWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_capture)
	{
		CRect rc;
		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);
		if (rc.PtInRect(point))
		{
			if (m_bCheck)
				m_bChecked = !m_bChecked;
			// popup
			if (m_pPopup)
			{
				m_pPopup->DestroyWindow();
				delete m_pPopup;
				m_pPopup = NULL;
			}
			if (!m_pPopup)
			{
				CString sClassName  = AfxRegisterWndClass(0);
				GetWindowRect(&rc);
				
				m_pPopup = new CAmountPopup;
				m_pPopup->m_nStatus = m_kind;
				m_pPopup->m_pWizard = m_pParent;
				rc.SetRect(rc.left, rc.bottom, rc.left + 400, rc.bottom + 50 + (m_kind == 5?3:(m_kind == 4?1:m_kind))  * 20);
				//rc = adjustRect(rc);
				m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, sClassName, NULL,
					WS_POPUP|WS_BORDER, rc,	NULL, NULL, NULL);

				m_pPopup->setPathInfo(m_sRoot, m_sUserID);
				m_pPopup->setParent(this);
				m_pPopup->ShowWindow(SW_SHOW);
			}
		} 
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CControlWnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	// TODO: Add your message handler code here and/or call default
	m_bHover=TRUE;
	Invalidate();
	return 0;
}


LRESULT CControlWnd::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

BOOL CControlWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}


CSize CControlWnd::BitmapSize(HBITMAP hBitmap)
{
	if (!hBitmap)
		return CSize(0, 0);
	BITMAP BM;
	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CControlWnd::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

bool CControlWnd::IsChecked()
{
	return m_bChecked;
}

void CControlWnd::setCheck(bool bCheck)
{
	m_bChecked = bCheck;
	Invalidate();
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();	
}

COLORREF CControlWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

LOGFONT CControlWnd::setFont(CString fName, int fsize, int style)
{
	LOGFONT	lf;
	memset(&lf, 0, sizeof(LOGFONT));      
	lf.lfHeight = fsize * 10; 
	
	strcpy(lf.lfFaceName, fName);  
	
	lf.lfCharSet = DEFAULT_CHARSET;
	lf.lfOutPrecision = OUT_DEFAULT_PRECIS;
	lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	lf.lfQuality = DEFAULT_QUALITY;
	lf.lfPitchAndFamily = DEFAULT_PITCH;
	
	lf.lfWeight = FW_NORMAL;
	lf.lfItalic = FALSE;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		lf.lfItalic = TRUE;
		break;
	case 3: // bold
		lf.lfWeight = FW_BOLD;
		break;
	case 4: // both
		lf.lfItalic = TRUE;
		lf.lfWeight = FW_BOLD;
		break;
	}

	return lf;
}

void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString tmp = m_Param.options;
	if (!OptionParser(tmp, "/k").IsEmpty())
		m_kind = atoi(OptionParser(tmp, "/k"));
	else
		m_kind = 1;

	m_sImagePath = OptionParser(tmp, "/i");
	m_sText = OptionParser(tmp, "/t");
	m_sSymbol = OptionParser(tmp, "/r");

	m_type = 1;
	if (!m_sImagePath.IsEmpty())
	{
		m_type++;
		if (!m_sText.IsEmpty())
			m_type++;
	}

	m_sUserID = Variant(userCC, "");
	m_sRoot = Variant(homeCC, "");

	int	offs = m_sImagePath.ReverseFind('\\');
	if (offs == -1)
		m_sImagePath = m_sRoot + "\\image\\" + m_sImagePath;
	else
		m_sImagePath = m_sRoot + "\\image\\" + m_sImagePath.Mid(offs + 1);
}


CString CControlWnd::Parser(CString &srcstr, CString substr)
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

void CControlWnd::WriteAlert(CString str)
{
	/*CString sym, dat;
	bool bChanged = false;
	while (!str.IsEmpty())
	{
		sym = Parser(str, "\t");
		dat = Parser(str, "\t");

		switch (atoi(sym))
		{
		case 23:	// 현재가
			m_jinfo.jisu = dat;
			m_jinfo.jisu.Replace("+", "");
			m_jinfo.jisu.Replace("-", "");
			bChanged = true;
			break;
		case 24:	// 전일대비
			m_jinfo.diff = dat;
			bChanged = true;
			break;
		case 33:	// 등락율
			m_jinfo.rate = dat;
			bChanged = true;
			break;
		case 27:	// 거래량
			m_jinfo.gvol = dat;
			bChanged = true;
			break;
		}
	}
	if (bChanged && m_pPopup)
	{
		m_pPopup->setJinfo(m_jinfo);
		m_pPopup->Invalidate();
	}*/
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

void CControlWnd::SetButtonImages()
{
	CString sImageDir = "";
	sImageDir.Format("%s\\image\\", m_sRoot);

	if (m_type == 1)
		m_hBitmap = m_hBitmap_dn = m_hBitmap_hv = NULL;
	else if (m_type == 2 || m_type == 3)
	{
		CFileFind	finder;
		m_sImagePath.MakeLower();
		m_sImagePath.Replace(".bmp", "");
		if (finder.FindFile(m_sImagePath + ".bmp"))
			m_hBitmap = getBitmap(m_sImagePath + ".bmp")->operator HBITMAP();
		else
			m_hBitmap = NULL;
	
		if (finder.FindFile(m_sImagePath + "_dn.bmp"))
			m_hBitmap_dn = getBitmap(m_sImagePath + "_dn.bmp")->operator HBITMAP();
		else
			m_hBitmap_dn = m_hBitmap;

		if (finder.FindFile(m_sImagePath + "_en.bmp"))
			m_hBitmap_hv = getBitmap(m_sImagePath + "_en.bmp")->operator HBITMAP();
		else
			m_hBitmap_hv = m_hBitmap;
	}
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetButtonImages();	
	return 0;
}

long CControlWnd::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 1: // Popup 수량선택
		m_rtnStr.Format("%s\t%d", m_sSymbol, lParam);
		m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (long)m_rtnStr.operator LPCTSTR());
		break;
	case 2: // Popup 수량선택 없이 Close
		SetTimer(TID_ONCHANGE_EVENT, 150, NULL);
		break;
	}
	return 0;
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

//
// properties & method
//

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;

	return strResult.AllocSysString();
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	m_Param.options = sParam;
}


short CControlWnd::GetType() 
{
	switch (m_kind)
	{
	case 1:	// 거래소
		return '1';
	case 2:	// 선물
		return 'F';
	case 3:	// 옵션
		return 'O';
	case 4:	// ECN
		return 'E';
	case 5:	// 주식옵션
		return 'P';
	case 6:	// KOFEX
		return 'Q';
	}
	return 0;
}

void CControlWnd::SetType(short nNewValue) 
{
	switch (nNewValue)
	{
	case '1':	// 거래소
		m_kind = 1;
		break;
	case 'F':	// 선물
		m_kind = 2;
		break;
	case 'O':	// 옵션
		m_kind = 3;
		break;
	case 'P':	// 주식옵션
		m_kind = 5;
		break;
	case 'E':	// ECN
		m_kind = 4;
		break;
	case 'Q':	// KOFEX
		m_kind = 6;
		break;
	}
}

BOOL CControlWnd::GetEnable() 
{
	return IsWindowEnabled();
}

void CControlWnd::SetEnable(BOOL bNewValue) 
{
	EnableWindow(bNewValue);
}

BOOL CControlWnd::GetVisible() 
{
	return IsWindowVisible();
}

void CControlWnd::SetVisible(BOOL bNewValue) 
{
	if (bNewValue)
		ShowWindow(SW_SHOW);
	else
		ShowWindow(SW_HIDE);
}

CBitmap* CControlWnd::getBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CFont* CControlWnd::getAxFont(CString fName, int point, int style)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)fName.operator LPCTSTR();
	fontR.point = point;
	fontR.italic = false;
	fontR.bold = 0;
	switch (style)
	{
	case 0: // none
	case 1: // none
		break;
	case 2: // italic
		fontR.italic = true;
		break;
	case 3: // bold
		fontR.bold = FW_BOLD;
		break;
	case 4: // both
		fontR.italic = true;
		fontR.bold = FW_BOLD;
		break;
	}
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CRect CControlWnd::adjustRect(CRect rc)
{
	int nDx = 0, nDy = 0;
	if (rc.right > GetSystemMetrics(SM_CXFULLSCREEN))
		nDx = GetSystemMetrics(SM_CXFULLSCREEN) - rc.right;
	if (rc.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		nDy = GetSystemMetrics(SM_CYFULLSCREEN) - rc.bottom;
	rc.OffsetRect(nDx, nDy);
	return rc;
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TID_ONCHANGE_EVENT:
		KillTimer(nIDEvent);
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (long)m_Param.name.operator LPCTSTR());
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
