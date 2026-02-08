// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_Price.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	TRKEY_LIMIT	1
#define	TRKEY_HOGA	2

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent = nullptr;

	m_capture = false;
	m_hBitmap = NULL;
	m_hBitmap_dn = NULL;
	m_hBitmap_hv = NULL;
	m_bTracking = FALSE;
	m_bHover = FALSE;

	m_nType = 0;
	m_nKind = -2;
	m_kind = 0;
	m_type = 0;
	m_bKospi = false;
	m_bLhGd = false;
	m_bCfo = false;
	m_dCurr = 0;
	m_dMax = 0;
	m_dMin = 0;
	m_dJgga = 0;
	m_nPrice1 = m_nPrice2 = 0;
	m_dshga = m_dhhga = 0.0f;
	m_bRsvdMode = FALSE;
	m_bNoMaxMin = false;

	m_pToolTip = nullptr;
	m_pPopup = nullptr;
	m_bDragMode = FALSE;

	m_bNewUnit = FALSE;
}

CControlWnd::~CControlWnd()
{
	if (m_pPopup)
	{
		if (IsWindow(m_pPopup->GetSafeHwnd()))
			m_pPopup->SendMessage(WM_CLOSE);
		m_pPopup.reset();
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
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnMessage1)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_EX(CControlWnd, "JCode", GetJCode, SetJCode, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "Type", GetType, SetType, VT_I2)
	DISP_PROPERTY_EX(CControlWnd, "Enable", GetEnable, SetEnable, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "Visible", GetVisible, SetVisible, VT_BOOL)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetValues", SetValues, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SendCode", SendCode, VT_BOOL, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetReserveMode", SetReserveMode, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "SetDragMode", SetDragMode, VT_EMPTY, VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D63DFD26-89BE-4C46-8B75-C5A207198F51}
static const IID IID_IControlWnd =
{ 0xd63dfd26, 0x89be, 0x4c46, { 0x8b, 0x75, 0xc5, 0xa2, 0x7, 0x19, 0x8f, 0x51 } };

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
		return (long)m_rtnStr.GetString();
		break;
	case DLL_OUBx:
		{
			int	pos = 0;
			struct	_extTHx* exth = (struct _extTHx*)lParam;
			CString	str = exth->data;
			str.TrimRight();

			switch (exth->key)
			{
			case TRKEY_LIMIT:
				pos = str.Find('\t');
				if (pos == -1 && pos == str.ReverseFind('\t'))
				{
					m_dshga = atof(str.Mid(0, pos));
					m_dhhga = atof(str.Mid(pos + 1));
				}
				break;

			case TRKEY_HOGA:
				if (exth->size >= 37)
				{
					char	cGubn = str.Mid(0, 1).GetAt(0);

					m_sRTSCode = str.Mid(1, 12);
					m_sRTSCode.TrimRight();
					m_dCurr    = atof(str.Mid(13, 8));
					m_dMax     = atof(str.Mid(21, 8));
					m_dMin     = atof(str.Mid(29, 8));
					m_dJgga    = atof(str.Mid(37, 8));

					setKind(cGubn, m_nKind);
					if ((m_sRTSCode.GetLength() == 9) && (m_sRTSCode.GetAt(0) == 'J'))
						m_bNoMaxMin = true; //신주인수권일때 예외처리

					if ((!m_bNoMaxMin && (m_dMax < m_dMin || m_dCurr > m_dMax || m_dCurr < m_dMin))
						|| (m_bNoMaxMin && m_dCurr == 0))
					{
						m_nKind = 0;
						m_dJgga = 0;
						m_dCurr = 0;
						m_dMax = 0;
						m_dMin = 0;
					}

					if (!m_sTxCode.IsEmpty())
					{
						PostMessage(WM_USER + 1, 2);	// popup
					}
				}
				else
				{
					m_nKind = 0;
					m_bKospi = false;
					m_dCurr = 0;
					m_dMax = 0;
					m_dMin = 0;
					m_dJgga = 0;
				}
				break;
			default:break;
			}
		}
		break;
	case DLL_OUB:	// NOT USED. Use DLL_OUBx Only !!!
		{
			// 예약주문 TR 확인후 익일상한가/하한가 세팅.
			// SetParam으로 넘어온 Key를 쓰는데, 돌아오는 Key가 그것과
			// 같지 않아 아래와 같이 문자열을 보고 파싱하도록 코딩함.
			if (m_bRsvdMode)
			{
				CString data = (LPCSTR)lParam;
				data.TrimRight();
				const	int	pos = data.Find('\x09');
				if (pos !=- 1 && pos == data.ReverseFind('\x09'))
				{
					m_dshga = atof(data.Mid(0, pos));
					m_dhhga = atof(data.Mid(pos+1));
					return 0;
				}
			}

			if (HIWORD(wParam) >= 37)
			{  
				CString str = (char*)lParam;
				char	cGubn = str.Mid(0, 1).GetAt(0);

				m_sRTSCode = str.Mid(1, 12);
				m_sRTSCode.TrimRight();
				m_dCurr = atof(str.Mid(13, 8));
				m_dMax = atof(str.Mid(21, 8));
				m_dMin = atof(str.Mid(29, 8));
				m_dJgga = atof(str.Mid(37, 8));

				setKind(cGubn, m_nKind);
				if ((m_sRTSCode.GetLength()==9)&&(m_sRTSCode.GetAt(0)=='J'))	m_bNoMaxMin = true; //신주인수권일때 예외처리
				
				if ((!m_bNoMaxMin  && (m_dMax < m_dMin || m_dCurr > m_dMax || m_dCurr < m_dMin)) 
					|| (m_bNoMaxMin  && m_dCurr == 0))
				{
					m_nKind = 0;
					m_dJgga = 0;
					m_dCurr = 0;
					m_dMax = 0;
					m_dMin = 0;
				}
				
				if (!m_sTxCode.IsEmpty())
				{
					PostMessage(WM_USER + 1, 2);	// popup
				}
			}
			else
			{
				CString str = (char*)lParam;
				m_nKind = 0;
				m_bKospi = false;
				m_dCurr = 0;
				m_dMax = 0;
				m_dMin = 0;
				m_dJgga = 0;
			}
		}
		break; 
	case DLL_ALERTx:
		struct	_alertR* alertR;
		alertR = (struct _alertR*)lParam;

		if (m_sTxCode.IsEmpty())
			WriteAlertx((DWORD *)alertR->ptr[0]);
		else
		{
			if (!m_sRTSCode.CompareNoCase(alertR->code))
				WriteAlertx((DWORD*)alertR->ptr[0]);
		}
		break;
	case DLL_ALERT:
		/*
		if (m_sTxCode.IsEmpty())
			WriteAlert((char*)lParam);
		else
		{
			CString str = (char*)lParam;
			if (!Parser(str, "\t").CompareNoCase(m_sRTSCode))
				WriteAlert(str);
		}
		*/
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
	CRect	rc;
	CString txt;

	GetClientRect(&rc);
	dc.FillSolidRect(rc, GetIndexColor(m_Param.pRGB));
	dc.SetBkMode(TRANSPARENT);

	if (m_hBitmap)
	{
		CDC	memDC;
		BITMAP	stBitmapInfo{};
		CBitmap	*pOldBitmap = nullptr, *pBitmap = nullptr;

		if (m_capture && m_bHover && m_hBitmap_dn)
			pBitmap = CBitmap::FromHandle(m_hBitmap_dn);
		else if (m_bHover && m_hBitmap_hv)
			pBitmap = CBitmap::FromHandle(m_hBitmap_hv);
		else
			pBitmap = CBitmap::FromHandle(m_hBitmap);

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
		if (pOldBitmap)
			memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
	}
	
	if (!m_hBitmap)
		dc.DrawFrameControl(rc, DFC_BUTTON, DFCS_BUTTONPUSH | (m_capture && m_bHover?DFCS_PUSHED:NULL));
	
	// set font
	CFont*	pFont = getAxFont(m_Param.fonts, m_Param.point, m_Param.style);
	CFont*	pOldFont = (CFont*)dc.SelectObject(pFont);
	
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
	
	// Do not call CButton::OnPaint() for painting messages
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
		TRACKMOUSEEVENT tme{};

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
		CRect	rc;

		m_capture = false;
		ReleaseCapture();
		GetClientRect(&rc);

		if (rc.PtInRect(point))
		{
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, 11/*Click*/)),
						(LPARAM)m_Param.name.GetString());
			
			if (!m_sTxCode.IsEmpty() && !m_sCode.IsEmpty())
			{
				SendCode(m_sCode);
				return;
			}
			popPrice(rc);
		}
		Invalidate();
	}
	CWnd::OnLButtonUp(nFlags, point);
}

LRESULT CControlWnd::OnMouseHover(WPARAM wparam, LPARAM lparam) 
{
	m_bHover = TRUE;
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

	BITMAP	BM{};

	GetObject(hBitmap, sizeof(BITMAP), &BM);
	return CSize((int)BM.bmWidth, (int)BM.bmHeight);
}

void CControlWnd::OnEnable(BOOL bEnable) 
{
	CWnd::OnEnable(bEnable);
	
	Invalidate();	
}

void CControlWnd::OnDestroy() 
{
	if (m_pToolTip)
		m_pToolTip.reset();

	CWnd::OnDestroy();	
}

COLORREF CControlWnd::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
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
	m_sTxCode = OptionParser(tmp, "/x");

	m_sTxCode.Trim();

	if (!OptionParser(tmp, "/p").IsEmpty())
		m_nType = atoi(OptionParser(tmp, "/p"));

	if (!OptionParser(tmp, "/d").IsEmpty())
		m_bDragMode = TRUE;

	m_type = 1;
	if (!m_sImagePath.IsEmpty())
	{
		m_type++;
		if (!m_sText.IsEmpty())
			m_type++;
	}

	m_sUserID = Variant(userCC, "");
	m_sRoot = Variant(homeCC, "");

	tmp.Format(_T("%s/tab/axis.ini"), m_sRoot);
	m_bNewUnit = GetPrivateProfileInt("K2023SyStem", "Newhgunit", 0, tmp);

	CONST	int	offs = m_sImagePath.ReverseFind('\\');
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

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CControlWnd::WriteAlert(CString str)
{
	CString sym, dat;

	while (!str.IsEmpty())
	{
		sym = Parser(str, "\t");
		dat = Parser(str, "\t");
		switch (atoi(sym))
		{
		case 23:	// 현재가
			dat.Replace("+", "");
			dat.Replace("-", "");
			m_dCurr = atof(dat);
			break;
		}
	}
}

void CControlWnd::WriteAlertx(DWORD* ptr)
{
	CString data;

	if (ptr[23])	// 현재가
	{
		data = (char*)ptr[23];
		data.Replace("+", "");
		data.Replace("-", "");
		m_dCurr = atof(data);
	}
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	CONST	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

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

	CRect	rc;

	GetClientRect(&rc);
	m_pToolTip = std::make_unique<CToolTipCtrl>();
	m_pToolTip->Create(this);
	m_pToolTip->ShowWindow(SW_HIDE);
	m_pToolTip->AddTool(this, m_sTooltip, &rc, GetDlgCtrlID());
	m_pToolTip->Activate(TRUE);
	
	if (m_bDragMode)
		m_sTooltip = "알림호가범위";
	else
		m_sTooltip = "호가선택";

	SetButtonImages();	
	return 0;
}       

long CControlWnd::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{ 
	case 1:		// 데이터 선택
		if (((m_nKind?m_nKind:m_kind) == 2 || (m_nKind?m_nKind:m_kind) == 6 || (m_nKind?m_nKind:m_kind) == 3) && !m_bLhGd)
			m_rtnStr.Format("%s\t%.2f", m_sSymbol, *((double*)lParam));
		else
			m_rtnStr.Format("%s\t%.f", m_sSymbol, *((double*)lParam));
		m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)m_rtnStr.GetString());
		break;
	case 2:		// popup SendMessage의 종료를 위해서 message로 처리한다.
		{
			CRect	rc;

			GetClientRect(&rc);
			popPrice(rc);
		}
		break;
	case 3:		// Double Click Message
		m_pParent->PostMessage(WM_LBUTTONDBLCLK, MK_LBUTTON, lParam);
		break;
	case 5:		// 데이터 선택
		{
			CString str;
			CString s1, s2, tmp;

			str.Format("%s",(char*)lParam);
			m_pParent->PostMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)str.GetString());

			s1 = Parser(str,",");
			m_nPrice1 = atoi(s1);
			s2 = Parser(str,",");
			m_nPrice2 = atoi(s2);

			if (m_nPrice2 > m_nPrice1)
				m_sTooltip.Format("알림호가범위:%d~%d",m_nPrice1,m_nPrice2);
			else
				m_sTooltip.Format("알림호가범위:%d~%d",m_nPrice2,m_nPrice1);
		}
		break;
	}
	return 0;
}

void CControlWnd::SendTR(CString name, CString data, BYTE type, int key)
{
	char	buff[1024]{};
	const	int	datl = data.GetLength();

	if (m_bDragMode)
	{
		// Header setting
		struct _userTH *udat = (struct _userTH*)buff;
		CopyMemory(udat->trc, name, 8);
		udat->key  = m_Param.key;
		udat->stat = type | US_KEY;
		// Data setting
		CopyMemory(&buff[L_userTH], data, datl);
	}
	else
	{
		struct	_userTH	udat{};
		int	idx = 0;

		buff[idx++] = key;
		CopyMemory(&buff[idx], m_Param.name, m_Param.name.GetLength());
		idx += m_Param.name.GetLength();
		buff[idx++] = '\t';

		strcpy(udat.trc, name);
		udat.key = m_Param.key;
		udat.stat = type | US_KEY;
		CopyMemory(&buff[idx], &udat, L_userTH);
		idx += L_userTH;
		CopyMemory(&buff[idx], (char*)data.GetString(), datl);
	}

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)buff);
}

void CControlWnd::popPrice(CRect rc)
{
	// popup
	if (m_pPopup)
		m_pPopup.reset();

	if (!m_pPopup)
	{
		if ((m_nKind > 0 || m_kind > 0) && 
			((!m_bNoMaxMin && (m_dCurr != 0 || m_dMax != 0 || m_dMin != 0)) || (m_bNoMaxMin && m_dCurr != 0)))
		{
			CString sClassName  = AfxRegisterWndClass(CS_DBLCLKS);

			GetWindowRect(&rc);
			m_pPopup = std::make_unique<CPricePopup>();
			m_pPopup->m_bNoMaxMin = m_bNoMaxMin;
			m_pPopup->m_pWizard = m_pParent;
			m_pPopup->m_nStatus = (m_nKind>=0?m_nKind:m_kind);
			m_pPopup->m_bKospi = m_bKospi;
			m_pPopup->m_dCurr = m_dCurr;
			m_pPopup->m_bNewUnit = m_bNewUnit;

			// 예약주문모드 세팅시, 현재가 기준으로 상/하한가 설정하도록 변경함. 20090824 by lkm
			if (m_bRsvdMode && IsRsvdKind(m_nKind>=0?m_nKind:m_kind) && m_dshga!=0.0f && m_dhhga!=0.0f)
			{
				m_pPopup->m_dMax = m_dshga;
				m_pPopup->m_dMin = m_dhhga;
				m_pPopup->m_dGjga = m_dCurr;
				m_pPopup->m_dCurr = m_dCurr;
			}
			else
			{
				m_pPopup->m_dMax = m_dMax;
				m_pPopup->m_dMin = m_dMin;
				m_pPopup->m_dGjga = m_dJgga;
				m_pPopup->m_dCurr = m_dCurr;
			}

			m_pPopup->m_sCode   = m_sCode;
			m_pPopup->m_nType = m_nType;

			if (!m_bNoMaxMin)
				rc.SetRect(rc.right, rc.top, rc.right + 140, rc.top + 250);
			else
				rc.SetRect(rc.right, rc.top, rc.right + 75, rc.top + 250);

			m_pPopup->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST, sClassName, NULL,
					WS_POPUP|WS_BORDER, rc, NULL, NULL, NULL);
			
			m_pPopup->setParent(this);
			m_pPopup->setColor(GetIndexColor(94), GetIndexColor(95));
			m_pPopup->ShowWindow(SW_SHOW);
		}
		//m_nKind = -2;
	}
}

char CControlWnd::getCharKind()
{
	switch (m_kind)
	{
	case 1:	// 주식
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
	case 7:	// ETF
		return '4';
	case 8:	// KOSTAR
		return 'S';
	case 9:	// ELW
		return 'W';
	case 10:// 주식선물
		return 'J';
	default:
		return '1';
	}
	return '1';
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

short CControlWnd::GetType() 
{
	return getCharKind();
}

void CControlWnd::SetType(short nNewValue) 
{
	setKind((char)nNewValue, m_kind);
}

void CControlWnd::setKind(char cGubn, int &kind)
{ 
	switch (cGubn)
	{
	case '1':	// 거래소 & ELW
	case '3':	// 제3시장
		kind = 1;
		m_bKospi = true;
		break;
	case '4':	// KOSPI ETF
		/*
		kind = 1;
		if (!m_sRTSCode.IsEmpty() && m_sRTSCode[0] == 'J')
			m_bNoMaxMin = true;
		else
			m_bNoMaxMin = false;
		m_bKospi = true;
		break;
		*/
		kind = 7;
		break;
	case 'O':	// Options
		kind = 3;
		m_bKospi = true;
		break;
	case 'P':	// 주식옵션
	case '5':	// KOSDAQ ETF 20040824
		kind = 7;
		m_bKospi = false;
		break;
	case '2':	// Kosdaq
		kind = 1;
		m_bKospi = false;
		break;
	case 'f':	// 상품선물
	case 'F':	// 선물
 		kind = 2;
		m_bKospi = true;
		break;
	case 'Q':	// KOFEX
		kind = 6;
		m_bKospi = false;
		break;
	case 'X':	// KOFEX Options
		kind = 3;
		m_bKospi = false;
		break;
	case 'E':	// ECN
		kind = 4;
		m_bKospi = true;
		break;
	case 'S':	// KOSTAR
		kind = 8;
		m_bKospi = true;
		break;
	case 'W':	// ELW
		kind = 9;
		m_bKospi = false;
		break;
	case 'J':	// 주식선물
		kind = 10;
		m_bKospi = true;
		break;
	default:
		kind = 0;
		m_bKospi = false;
		m_dCurr = 0;
		m_dMax = 0;
		m_dMin = 0;
		break;
	}
}
// Propertis & Methods

BSTR CControlWnd::GetProperties() 
{
	CString strResult = m_Param.options;

	return strResult.AllocSysString();
}

void CControlWnd::SetProperties(LPCTSTR sParam) 
{
	CString	tmp;

	m_Param.options = tmp = sParam;
	if (!OptionParser(tmp, "/k").IsEmpty())
		m_kind = atoi(OptionParser(tmp, "/k"));
	else
		m_kind = 1;
	m_sImagePath = OptionParser(tmp, "/i");
	m_sText = OptionParser(tmp, "/t");
	m_sSymbol = OptionParser(tmp, "/r");
	m_sTxCode = OptionParser(tmp, "/x");
	if (!OptionParser(tmp, "/p").IsEmpty())
		m_nType = atoi(OptionParser(tmp, "/p"));

	m_type = 1;
	if (!m_sImagePath.IsEmpty())
	{
		m_type++;
		if (!m_sText.IsEmpty())
			m_type++;
	}
}

BOOL CControlWnd::SendCode(LPCTSTR sCode) 
{
	CString sTmp;

	// 익일 상한가/하한가 조회하도록 추가 TR 2009.08.24
	if (m_bRsvdMode)
	{
#if 1
		sTmp.Format("1301%c%s\t1403\t1404\n\t", 0x7f, m_sCode);
#else
		sTmp = "";
		sTmp += "1301" "\x7F" + m_sCode;
		sTmp += "\x09" "1403" "\x09" "1404" "\x0A\x09";
#endif
		SendTR("pooppoop", sTmp, US_OOP, TRKEY_LIMIT);
	}
	// ------------------------------------------------

	char	gubn = getCharKind();
	if (strlen(sCode) == 8)
	{
		CString mkgb = CString(sCode+1, 2);
		const	int	imkgb = _ttoi((LPCTSTR)mkgb);
		if (imkgb >= 60 && imkgb <= 99)
		{
			m_bCfo = true;
			gubn = 'f';
		}
	}

	sTmp.Format("%c%-12s", gubn, sCode);
	SendTR(m_sTxCode, sTmp, 0, TRKEY_HOGA);

	return TRUE;
}

void CControlWnd::SetValues(LPCTSTR str) 
{
	if (strlen(str) <= 7)
		return;

	CString sTmp = str, tmp = Parser(sTmp, "\t");
	const	char	cGubn = (tmp.GetLength() ? tmp.GetAt(0) : '0');
	m_dCurr = atof(Parser(sTmp, "\t"));
	m_dMax = atof(Parser(sTmp, "\t"));
	m_dMin = atof(Parser(sTmp, "\t"));
	switch (cGubn)
	{
	case '1':	// 거래소 & ELW
	case '3':	// 제3시장
	case '4':	// ETF
		m_kind = 1;
		/*
		if (!m_sRTSCode.IsEmpty() && (m_sRTSCode[0] == 'J' || m_sRTSCode[0] == '5'))
			m_bNoMaxMin = true;
		else
			m_bNoMaxMin = false;
		*/
		m_bKospi = true;
		break;
	case '5':	// KOSDAQ ETF 20040824
		m_kind = 7;
		m_bKospi = false;
		break;
	case '2':	// Kosdaq
		m_kind = 1;
		m_bKospi = false;
		break;
	case 'F':	// 선물
		m_kind = 2;
		break;
	case 'O':	// Options
		m_kind = 3;
		m_bKospi = true;
		break;
	case 'P':	// 주식옵션
		m_kind = 5;
		m_bKospi = true;
		break;
	case 'Q':	// KOFEX
		m_kind = 6;
		m_bKospi = false;
		break;
	case 'X':	// KOFEX Options
		m_kind = 3;
		m_bKospi = false;
		break;
	case 'E':	// ECN
		m_kind = 4;
		m_bKospi = true;
		break;
	case 'S':	// KOSTAR
		m_kind = 8;
		m_bKospi = true;
		break;
	case 'W':	// ELW
		m_kind = 9;
		m_bKospi = false;
		break;
	case 'J':	// 주식선물
		m_kind = 10;
		m_bKospi = true;
		break;
	default:
		m_kind = 0;
		m_bKospi = false;
		m_dCurr = 0;
		m_dMax = 0;
		m_dMin = 0;
		break;
	}
}

BSTR CControlWnd::GetJCode() 
{
	CString strResult = m_sCode;

	return strResult.AllocSysString();
}

void CControlWnd::SetJCode(LPCTSTR lpszNewValue) 
{
	m_sCode = lpszNewValue;
	CString spcd = m_sCode.Mid(1,2);
	if (m_sCode.GetLength()==8)
	{
		switch (atoi(spcd))
		{
		case 61: case 62: case 63: case 64:
		case 65: case 66: case 67:
		case 75: case 76: case 77:
			m_bCfo = true;
			m_bLhGd = false;
			break;
		case 85: case 86: case 87:
			m_bCfo = true;
			m_bLhGd = true;
			break;
		default:
			m_bCfo = false;
			m_bLhGd = false;
			break;
		}
	}
	else
	{
		m_bCfo = m_bLhGd = false;
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
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (LPARAM)path.GetString());
}

CFont* CControlWnd::getAxFont(CString fName, int point, int style)
{
	struct	_fontR	fontR{};

	fontR.name = (LPSTR)fName.GetString();
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
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (LPARAM)&fontR);
}

void CControlWnd::SetReserveMode(BOOL bReserveMode) 
{
	m_bRsvdMode = bReserveMode;
}

BOOL CControlWnd::IsRsvdKind(int type)
{
	// KOSPI, KOSDAQ, 프리보드, ETF, ELW 
	// ( ELW의 경우 m_bNoMaxMin 에 걸리므로 Pass )
	return (type==1 || type==2 || type==3 || type==4 || type==5);
}

LRESULT CControlWnd::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{	
	if (message >= WM_MOUSEFIRST && message <= WM_MOUSELAST)
	{
		CRect	rc;
		MSG	msg{};

		GetClientRect(&rc);
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.wParam = wParam;
		msg.lParam = lParam;
		if (m_pToolTip) 
		{
			m_pToolTip->AddTool(this, m_sTooltip, &rc, GetDlgCtrlID());
			m_pToolTip->Activate(TRUE);
			m_pToolTip->RelayEvent(&msg);
		}
	}
	return CWnd::WindowProc(message, wParam, lParam);
}

void CControlWnd::SetDragMode(BOOL flag) 
{
	// TODO: Add your dispatch handler code here
	m_bDragMode = flag;
}

BOOL CControlWnd::GetDragMode()
{
	return m_bDragMode;
}
