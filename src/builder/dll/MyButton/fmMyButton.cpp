// fmMyButton.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axMyButton.h"
#include "fmMyButton.h"
#include "image.h"


__declspec(dllexport) CfmObject* APIENTRY createObject(CfmProperties* foProperties, CiForm* pObject, CWnd* parent)
{
//	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return (CfmObject*) new CfmMyButton(foProperties, pObject, parent);
}

// CfmMyButton

IMPLEMENT_DYNAMIC(CfmMyButton, CWnd)

CfmMyButton::CfmMyButton(CfmProperties* formOb, CiForm* piObject, CWnd* parent) : CfmObject(formOb, piObject, parent)
{
	EnableAutomation();
	m_pic = nullptr;
	
	Reload();
	Create(NULL, _T("fmMyButton"), WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN, formOb->rect, parent, -1, NULL);
	EnableWindow(m_prop->styles & stENABLE ? TRUE : FALSE);
}

CfmMyButton::~CfmMyButton()
{
	if (m_pic)
		delete m_pic;
}

void CfmMyButton::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.
	CWnd::OnFinalRelease();
}

void CfmMyButton::Reload()
{
	m_bReload == true ? m_bReload = false : CfmObject::Reload();
	m_mousedown = false;
	m_image = PI_DEFAULT;

	if (m_pic)
	{
		delete m_pic;
		m_pic = nullptr;
	}

	if (m_prop->styles & (stIMAGE | stIMAGETEXT))
	{
		CString	path;
		path.Format("%s\\%s\\%s", m_ifm->m_root, IMAGEDIR, m_prop->images);
		m_pic = new Cimage(m_ifm, path);
		if (m_prop->styles & stIMAGETEXT)
			m_prop->styles |= stTRANSPARENT;
	}

	if (m_prop->styles & stXPAL)
		m_tRGB = getRGB(CLR_BTNTEXT);

	if (GetSafeHwnd())
		EnableWindow(m_prop->styles & stENABLE ? TRUE : FALSE);
}


BEGIN_MESSAGE_MAP(CfmMyButton, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSELEAVE()
	ON_WM_MOUSEHOVER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()
	ON_WM_KEYUP()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CfmMyButton, CWnd)
	//{{AFX_DISPATCH_MAP(CfmMyButton)
	DISP_PROPERTY_EX_ID(CfmMyButton, "Text", dispidText, _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX_ID(CfmMyButton, "PRgb", dispidPRgb, _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX_ID(CfmMyButton, "TRgb", dispidTRgb, _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX_ID(CfmMyButton, "Visible", dispidVisible, _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX_ID(CfmMyButton, "Enable", dispidEnable, _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX_ID(CfmMyButton, "TabStop", dispidTabStop, _getTabStop, _setTabStop, VT_BOOL)
	DISP_PROPERTY_EX_ID(CfmMyButton, "Left", dispidLeft, _getLeft, _setLeft, VT_I4)
	DISP_PROPERTY_EX_ID(CfmMyButton, "Top", dispidTop, _getTop, _setTop, VT_I4)	
	DISP_PROPERTY_EX_ID(CfmMyButton, "Right", dispidRight, _getRight, _setRight, VT_I4)
	DISP_PROPERTY_EX_ID(CfmMyButton, "Bottom", dispidBottom, _getBottom, _setBottom, VT_I4)
	DISP_FUNCTION_ID(CfmMyButton, "SetRect", dispidSetRect, _SetRect, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CfmMyButton, "SetFocus", dispidSetFocus, _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CfmMyButton, "Refresh", dispidRefresh, _Refresh, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// 참고: IID_IfmMyButton에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {E990B576-C6EA-431E-8F32-6AC17882C10E}
static const IID IID_IfmMyButton =
{ 0xE990B576, 0xC6EA, 0x431E, { 0x8F, 0x32, 0x6A, 0xC1, 0x78, 0x82, 0xC1, 0xE } };

BEGIN_INTERFACE_MAP(CfmMyButton, CWnd)
	INTERFACE_PART(CfmMyButton, IID_IfmMyButton, Dispatch)
END_INTERFACE_MAP()


// CfmMyButton 메시지 처리기입니다.


void CfmMyButton::_SetFocus() 
{
	CWnd::SetFocus();
}

void CfmMyButton::_Refresh() 
{//ok
	Invalidate();
}

void CfmMyButton::Refresh()
{
	CfmObject::Refresh();
}

BSTR CfmMyButton::_getText() 
{//ok
	CString strResult;

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CfmMyButton::_setText(LPCTSTR lpszNewValue) 
{//ok
	WriteData(lpszNewValue);
}

long CfmMyButton::_getPRgb() 
{//ok
	return (long)m_pRGB;
}

void CfmMyButton::_setPRgb(long nNewValue) 
{//ok
	CfmObject::SetBkColor(nNewValue);
}

long CfmMyButton::_getTRgb() 
{//ok
	return (long)m_tRGB;
}

void CfmMyButton::_setTRgb(long nNewValue) 
{//ok
	CfmObject::SetFgColor(nNewValue);
}

BOOL CfmMyButton::_getVisible() 
{//ok
	return (m_prop->styles & stVISIBLE) ? TRUE : FALSE;
}

void CfmMyButton::_setVisible(BOOL bNewValue) 
{//ok
	SetVisible(bNewValue ? true : false);
}

BOOL CfmMyButton::_getEnable() 
{//ok
	return (m_prop->styles & stENABLE) ? TRUE : FALSE;
}

void CfmMyButton::_setEnable(BOOL bNewValue) 
{//ok
	SetEnable(bNewValue ? true : false);
}

BOOL CfmMyButton::_getTabStop() 
{
	return (m_prop->attributes & faPASS) ? FALSE : TRUE;
}

void CfmMyButton::_setTabStop(BOOL bNewValue) 
{
	if (bNewValue)
		m_prop->attributes &= ~faPASS;
	else
		m_prop->attributes |= faPASS;

/*
#define	evTabstop		0x00aa		// set tapstop
						// void SetTabStop(int key, BOOL tabstop, (char *)name)
						// Cmsg.args[0] = int	tabstop (1 or 0)
						// Cmsg.args[1] = char*	form-object name
*/
}

void CfmMyButton::SetFont(int point, int style, CString fonts, bool resize)
{
	if (!(m_prop->styles & (stIMAGE|stIMAGETEXT)))
		CfmObject::SetFont(point, style, fonts, resize);
}

void CfmMyButton::Draw(CDC* dc)
{
	if (!(m_prop->styles & stVISIBLE))
		return;

	UINT	format;
	CRect	cRc;
	CBrush*	brush;
	CString	str = _T("");
	int	ndc = dc->SaveDC();
	bool	border;

	setPen(dc);
	border = true;
	if (m_prop->styles & (stIMAGE|stIMAGETEXT) && m_pic && m_pic->GetValid(m_image))
	{
		border = false;
		m_pic->DrawPicture(dc, m_pRc, m_prop->alignmentImage, m_image);
		if (!(m_prop->styles & stHOVER) && m_mousedown)
			border = true;
	}
	else if (!(m_prop->styles & stTRANSPARENT))
	{
		border = true;
		brush = setBrush(dc);
		dc->FillRect(m_pRc, brush);
	}

	if (!(m_prop->styles & stIMAGE) || m_prop->styles & stIMAGETEXT)
	{
		setFont(dc);
		setPaintColor(dc);

		format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		if (m_prop->styles & stIMAGETEXT)
			format |= DT_CENTER;
		else
		{
			switch (m_prop->alignment)
			{
			case atRIGHT:
				format |= DT_RIGHT;	break;
			case atLEFT:
				format |= DT_LEFT;	break;
			case atCENTER:
			default :
				format |= DT_CENTER;	break;
			}
		}

		str = m_prop->strings;
		cRc = m_pRc;
		if (m_prop->styles & stVERT)
		{
			int	gap, len = str.GetLength();

			str = convertVString(str);
			format &= ~DT_SINGLELINE;
			gap  = (cRc.Height() - dc->GetOutputTextExtent(str).cy * (str.GetLength() - len)) / 2;
			if (gap >= 0)
				cRc.OffsetRect(0, gap);
		}
		else
		{
			if (str.Find("\\n") != -1)
				str.Replace("\\n", "\n");
			if (str.Find('\n') != -1)
			{
				CRect	sRc = cRc;

				format &= ~DT_SINGLELINE;
				format |= DT_EXTERNALLEADING;
				dc->DrawText(str, sRc, format|DT_CALCRECT);
				if (sRc.Height() < cRc.Height())
				{
					cRc.top += (cRc.Height() - sRc.Height())/2;
					cRc.bottom = cRc.top + sRc.Height();
				}
			}
		}

		if (m_prop->styles & stENABLE)
		{
			setTextColor(dc);
			dc->DrawText(str, cRc, format);
		}
		else
		{
			setTextColor(dc, getRGB(cWHITE));
			dc->DrawText(str, cRc, format);
			cRc.left -= 1;
			cRc.top  -= 1;
			setTextColor(dc, getRGB(CLR_EDITDISTEXT));
			dc->SetBkMode(TRANSPARENT);
			dc->DrawText(str, cRc, format);
		}
	}

	if (border)
	{
		if (m_mousedown)
			drawEdge(dc, m_pRc, EDGE_SUNKEN);
		else
			drawEdge(dc, m_pRc, EDGE_RAISED);
	}

	cRc = m_pRc;
	cRc.DeflateRect(4, 4);
	if (m_focus)
	{
		setTextColor(dc, getRGB(cBLACK));
		dc->SelectStockObject(BLACK_PEN);
		dc->DrawFocusRect(cRc);
	}
	
	dc->RestoreDC(ndc);
}

void CfmMyButton::OnLMyButton(bool down, CPoint pt)
{
	m_mousedown = down;
		
	if (down && (m_prop->typeCursor >= csCURSOR))
		 ::SetCursor((HCURSOR)AxInfo(evGetCursor, "%d", m_prop->typeCursor));
	
	if (m_prop->styles & stTRANSPARENT)
		invalidateRect(&m_pRc, true);
	else
		invalidateRect(&m_pRc, false);
}

int CfmMyButton::OnDrag(CPoint pt)
{
	int	rc, state, image = m_image;
	if (image != m_image)
		invalidateRect(&m_pRc, false);

	return 0;
}

void CfmMyButton::WriteData(CString data, bool redraw, int col, int row)
{//ok
	bool	image = false;

	if (m_prop->styles & stIMAGE)
		image = true;
	else if (m_prop->styles & stIMAGETEXT)
	{
		CString	string = data;
		string.MakeLower();
		if (string.Find(".bmp") != -1 || string.Find(".gif") != -1)
			image = true;
	}

	if (image)
	{
		if (m_pic)
		{
			delete m_pic;
			m_pic = NULL;
		}

		CString	path;
		path.Format("%s\\%s\\%s", m_ifm->m_root, IMAGEDIR, data);
		m_pic   = new Cimage(m_ifm, path);
		m_image = PI_DEFAULT;
		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CfmObject::WriteData(data, redraw, col, row);
}


void CfmMyButton::SetFocus(bool focus)
{
	if (m_focus != focus)
	{
		if (!m_focus && focus)
			CWnd::SetFocus();

		m_focus = focus;
		if (!m_focus)
			m_mousedown = false;
		invalidateRect(&m_pRc, false);
	}
}

void CfmMyButton::drawObject(CRect rect, BOOL erase)
{
	InvalidateRect(rect, erase);
}

void CfmMyButton::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 __super::OnPaint()을(를) 호출하지 마십시오.

	Draw(&dc);
}

void CfmMyButton::OnSetFocus(CWnd* pOldWnd)
{
	__super::OnSetFocus(pOldWnd);
	SetFocus(true);
	AxEvent(evOnSetFocus);
}


void CfmMyButton::OnKillFocus(CWnd* pNewWnd)
{
	__super::OnKillFocus(pNewWnd);
	SetFocus(false);
}


void CfmMyButton::OnLMyButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	OnLMyButton(true, point);
	CWnd::SetFocus();
}


void CfmMyButton::OnLMyButtonUp(UINT nFlags, CPoint point)
{
	OnLMyButton(false, point);
	AxEvent(evOnClick);
}

void CfmMyButton::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	TRACKMOUSEEVENT tme;

	if (m_prop->styles & (stIMAGE | stIMAGETEXT) && (m_prop->styles & stHOVER))
	{
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.hwndTrack = GetSafeHwnd();
		tme.dwHoverTime = 100;
		TrackMouseEvent(&tme);
	}
	__super::OnMouseMove(nFlags, point);
}


void CfmMyButton::OnMouseLeave()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_image = PI_DEFAULT;
	AxEvent(evOnMouseLeave);
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	invalidateRect(&m_pRc, false);
	__super::OnMouseLeave();
}


void CfmMyButton::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_image = (nFlags & MK_LBUTTON) ? PI_DOWN : PI_ENTER;
	AxEvent(evOnMouseEnter);

	if (m_prop->typeCursor >= csCURSOR)
		::SetCursor((HCURSOR)AxInfo(evGetCursor, "%d", m_prop->typeCursor));

	invalidateRect(&m_pRc, false);
	__super::OnMouseHover(nFlags, point);
}


void CfmMyButton::OnLMyButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	AxEvent(evOnDoubleClick);
	invalidateRect(&m_pRc, false);
	__super::OnLButtonDblClk(nFlags, point);
}

void CfmMyButton::SetEnable(bool enable)
{//ok
	CfmObject::SetEnable(enable);
	EnableWindow(enable ? TRUE : FALSE);
}

void CfmMyButton::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	__super::OnChar(nChar, nRepCnt, nFlags);
}


void CfmMyButton::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	__super::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CfmMyButton::OnDestroy()
{
	__super::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

LONG CfmMyButton::_getLeft(void)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	return m_prop->rect.left;
}

void CfmMyButton::_setLeft(LONG newVal)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
	if (m_prop->rect.left != newVal)
	{
		m_prop->rect.left = newVal;
		MoveWindow(m_prop->rect);
		GetClientRect(m_pRc);	
	}
}

LONG CfmMyButton::_getTop(void)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	return m_prop->rect.top;
}

void CfmMyButton::_setTop(LONG newVal)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
	if (m_prop->rect.top != newVal)
	{
		m_prop->rect.top = newVal;
		MoveWindow(m_prop->rect);
		GetClientRect(m_pRc);	
	}
}

LONG CfmMyButton::_getRight(void)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	return m_prop->rect.right;
}

void CfmMyButton::_setRight(LONG newVal)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
	if (m_prop->rect.right != newVal)
	{
		m_prop->rect.right = newVal;
		MoveWindow(m_prop->rect);
		GetClientRect(m_pRc);	
	}
}

LONG CfmMyButton::_getBottom(void)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	return m_prop->rect.bottom;
}

void CfmMyButton::_setBottom(LONG newVal)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 속성 처리기 코드를 추가합니다.
	if (m_prop->rect.bottom!= newVal)
	{
		m_prop->rect.bottom = newVal;
		MoveWindow(m_prop->rect);
		GetClientRect(m_pRc);	
	}
}

void CfmMyButton::_SetRect(LONG left, LONG top, LONG right, LONG bottom)
{//ok
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	// TODO: 여기에 디스패치 처리기를 추가합니다.
	m_prop->rect = CRect(left, top, right, bottom);
	MoveWindow(m_prop->rect);
	GetClientRect(m_pRc);	
}

void CfmMyButton::SetRect(CRect rect, CRect pRc)
{
	m_prop->rect.CopyRect(pRc);
	MoveWindow(m_prop->rect);
	GetClientRect(m_pRc);

}

void CfmMyButton::SetVisible(bool visible, int col)
{//ok
	CfmObject::SetVisible(visible);
	ShowWindow(visible ? SW_SHOW : SW_HIDE);
}

void CfmMyButton::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (VK_TAB == nChar)
		AxInfo(evKeyDown, "%d", VK_TAB);

	__super::OnKeyUp(nChar, nRepCnt, nFlags);
}

CString CfmMyButton::GetProperties()
{
	static CString strProp = "Name|1|||name|Enter Symbol Name.\tLeft|1|||left|Set left location.\tTop|1|||top|Set top location.\tWidth|1|||width|Set Width.\tHeight|1|||height|Set Height.\tType|3|0-Text;1-Bmp;2-Both;|0x00000008;0x00000010|styles\tImage|2||1003|images\tImageType|3|0-Center;6-Tile;5-Stretch;||alignmentImage\tText|1||1|strings\tVerticalText|3|$bool|0x00000080|styles\tAlignment|3|$alignment||alignment\tFont|2|1|1001|fontName|Choose Font Name.\tFontSize|1|||fontPoint|Set Font Size.\tFontStyle|3|$fontStyle||fontStyle|Choose Font Style.\tBackColor|2||1002|paintColor|Specify Background Color.\tForeColor|2||1002|textColor|Specify letter color\tKeyMap|3|0-NotMapping;1-PageUp;2-PageDown;3-Home;4-End;5-F2;6-F3;7-F4;8-F5;10-F7;11-F8;12-F9;13-F11;14-F12;15-NPad(+);16-NPad(-);17-NPad(/);18-NPad(*);||keyParam\tTabOrder|1|1||index|Set tab order\tHintType|3|$typeHint||typeHint\tHintText|1|||hintText\tVisible|3|$bool|0x00000002|styles|Choose Visibility\tEnable|3|$bool|0x00000001|styles|Choose control usability \tHoverImage|3|$bool|0x00000020|styles\tFocus|3|$bool|0x00000004|styles";
	return strProp;
}

CString CfmMyButton::GetMethod()
{
	static CString strMethod = "Text:1\tPRgb:1\tTRgb:1\tVisible:1\tEnable:1\tChecked:1\tSetFocus:0\tRefresh:0\t";
	return strMethod;
}

CString CfmMyButton::GetEvent()
{
	static CString strEvent = "None|OnClick|OnDoubleClick";
	return strEvent;
}