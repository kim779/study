// fmBase.cpp : implementation file
//

#include "stdafx.h"
#include "ambase.h"
#include "amform.h"
#include <math.h>

#include "../../h/axiserr.h"
//#include "../lib/axislib.h"
#include "../amLib/AmDraw.h"
#include "../amLib/AmPalette.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CamBase

IMPLEMENT_DYNCREATE(CamBase, CCmdTarget)

CamBase::CamBase()
{
	EnableAutomation();
}

CamBase::CamBase(CAmForm* axform, struct _formR* formR)
{
	EnableAutomation();

	m_axform = axform;
	m_size   = CSize(0, 0);
	m_name   = _T("");
	m_rts    = -1;

	if (formR == (struct _formR *) NULL)
		return;

	m_form = formR;
//	m_name = m_axform->GetString(m_form->name);
	
	m_name = m_form->name;
	
	m_form->fStyle = m_form->fStyle;
	m_form->fPoint = m_form->fPoint;
	m_fonts = m_form->fName;
	
	
	if (m_form->fStyle == FS_NONE)
	{
		m_form->fStyle = (BYTE)m_axform->m_mapH->fStyle;
		m_form->fPoint = (BYTE)m_axform->m_mapH->fPoint;
//		m_form->fPoint = m_form->fPoint;
//		CString ss = CString(m_axform->m_fonts);
//		if (m_fonts.IsEmpty())
//			m_fonts = ss;	

		if (m_fonts.IsEmpty())
			m_fonts = "굴림체";
		if (m_form->fPoint == 0)
			m_form->fPoint = 9;
	}


	/*
	else
	{
		if (m_fonts.IsEmpty())
			m_fonts = "굴림체";
		if (m_form->fPoint == 0)
			m_form->fPoint = 9;
	}
	*/

	
	//if (m_form->kind != FM_GRID && m_form->attr & FA_CORGB /*m_form->isEdit == IE_CORGB*/)
	//{
	//	m_form->isEdit = IE_NOP;
	//	if (m_form->attr & FA_CORGB && m_form->onEdit != NOVALUE)
	//		m_form->isEdit = IE_CORGB;
	//}
	
	if (m_form->kind == FM_EDIT || m_form->kind == FM_OUT)
	{
		if (m_form->type != IO_NUM)
			m_form->attr &= ~(FA_COMMA|FA_FLOAT/*|FA_RIGHT*/);
	}

	m_strR = CString(m_form->str);	//m_axform->GetString(m_form->vals[0]);
	switch (m_form->kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_OUT:
	case FM_GRID:
	case FM_GRIDEX:
	case FM_TABLE:
	case FM_CONTROL:
		if (!m_name.IsEmpty())
		{
			char*	rts;
			int	length;

			rts    = (char *)m_name.operator LPCSTR();
			length = m_name.GetLength();
			if (length >= offsRTS)
				rts += (length - offsRTS);
			m_rts = atoi(rts);
			if (m_rts == 0)
			{
				length = (int)strlen(rts);
				for (int ii = 0; ii < length; ii++)
				{
					if (!isdigit(*rts))
					{
						m_rts = -1;
						break;
					}
				}
			}
		}
	default:
		break;
	}

	m_rect.SetRect(m_form->left, m_form->top, m_form->right, m_form->bottom);
	m_pRc = m_rect;

	m_fPoint = m_cfPoint = m_form->fPoint;
	setFontSize();

	m_pRGB = (m_form->pRGB & userCOLOR) ? m_form->pRGB : getRGB(m_form->pRGB);
	if (m_form->tRGB & userCOLOR)
	{
		m_tRGB  = m_form->tRGB;
		m_itRGB = -1;
	}
	else
	{
		m_tRGB  = getRGB(m_form->tRGB);
		m_itRGB = m_form->tRGB;
	}
	m_bRGB = (m_form->bRGB & userCOLOR) ? m_form->bRGB : getRGB(m_form->bRGB);

	m_sRGB = (m_form->sRGB & userCOLOR) ? m_form->sRGB : getRGB(m_form->sRGB);

	m_focus = false;
	m_hR    = 1.0;
	m_vR    = 1.0;
}

CamBase::~CamBase()
{
}


void CamBase::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamBase, CCmdTarget)
	//{{AFX_MSG_MAP(CamBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamBase, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CamBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmBase to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6E6307E3-9BA5-489A-B7A2-B6F4AE0F3752}
static const IID IID_IfmBase =
{ 0x6e6307e3, 0x9ba5, 0x489a, { 0xb7, 0xa2, 0xb6, 0xf4, 0xae, 0xf, 0x37, 0x52 } };

BEGIN_INTERFACE_MAP(CamBase, CCmdTarget)
	INTERFACE_PART(CamBase, IID_IfmBase, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamBase message handlers

bool CamBase::IsValid(int& guide)
{
	/*
	if (m_form->attr & FA_MAND)
	{
		if (!GetLength())
		{
			guide = AE_EMUST;
			return false;
		}
	}
	*/
	return true;
}

CRect CamBase::GetRect(bool original)
{
	return original ? m_rect : m_pRc;
}

void CamBase::SetRect(CRect rect, CRect pRc)
{
	m_rect = rect;
	m_pRc  = pRc;
}

void CamBase::Adjust(CSize size)
{
	m_size = size;
}

void CamBase::Adjust(CPoint pt, float hR, float vR)
{
	m_rect = calcRect(m_rect, hR, vR);
	m_rect.left   += pt.x;
	m_rect.right  += pt.x;
	m_rect.top    += pt.y;
	m_rect.bottom += pt.y;
	m_pRc = m_rect;

	//m_fPoint = m_cfPoint = (int)((float)m_fPoint * vR);
	setFontSize(hR, vR);
}

void CamBase::Offset(CSize offs)
{
	m_pRc.OffsetRect(offs);
}

void CamBase::Resize(float hR, float vR, bool font, bool redraw)
{
	CRect	tRc, iRc, oldRc;

	m_hR = hR;
	m_vR = vR;

	oldRc = m_pRc;
	tRc   = calcRect(m_rect, hR, vR);
	if (m_pRc.EqualRect(tRc))
		return;

	iRc.left   = min(tRc.left, m_pRc.left);
	iRc.top    = min(tRc.top, m_pRc.top);
	iRc.right  = max(tRc.right, m_pRc.right);
	iRc.bottom = max(tRc.bottom, m_pRc.bottom);

	m_pRc = tRc;
	if (font)
	{
		//m_fPoint = m_cfPoint = (int)((float)m_fPoint * vR);
		setFontSize(hR, vR);
	}
	if (redraw)
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&iRc, true);
//		else
//			invalidateRect(&iRc, false);
	}
}

void CamBase::Reload()
{
	m_rect.left   = m_form->left;
	m_rect.top    = m_form->top;
	m_rect.right  = m_form->right;
	m_rect.bottom = m_form->bottom;
	m_pRc = m_rect;

	m_fPoint = m_cfPoint = m_form->fPoint;
	setFontSize();

	m_hR = 1.0;
	m_vR = 1.0;
}

int CamBase::IsPointInRect(CPoint pt)
{
	CRect	tRc = m_pRc;

	if (!m_axform->IsFormVisible(this))
		return P_OUT;

	if (m_form->kind == FM_OBJECT)
		tRc.InflateRect(-1, -1);

	if ((m_form->properties & PR_VISIBLE) && (m_form->properties & PR_ENABLE) && tRc.PtInRect(pt))
		return P_IN;

	return P_OUT;
}

void CamBase::SetFont(int point, int style, CString fonts, bool resize)
{
	if (m_form->fPoint == m_axform->m_mapH->fPoint)
	{
		bool	change = false;

		if (!fonts.IsEmpty() && m_fonts.CompareNoCase(fonts))
		{
			m_fonts = fonts;
			change  = true;
		}

		if (style >= 0 && m_form->fStyle != style)
		{
			m_form->fStyle = style;
			change = true;
		}

		if (m_fPoint > m_cfPoint)
		{
			m_fPoint = m_cfPoint = point;
			change = true;
		}

		if (change)
		{
			TEXTMETRIC metric;
			CDC*	dc = getDC();
			setFont(dc);
			dc->GetTextMetrics(&metric);
			m_fHeight = (BYTE)abs(metric.tmHeight);
			m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);
			releaseDC(dc);

//			if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
//			else
//				invalidateRect(&m_pRc, false);
		}
	}
}

int CamBase::OnDrag(CPoint pt, int& result)
{
	result = RC_NOP;
	if (IsPointInRect(pt))
		result = RC_IN;
	return -1;
}

int CamBase::OnKey(int key, int& result)
{
	switch (key)
	{
	case VK_ESCAPE:
		keyEscape(result);
		break;

	case VK_DELETE :
		if (m_form->attr & FA_PROTECT)
			break;
		keyDelete(result);
		break;
	
	case VK_BACK :
		if (m_form->attr & FA_PROTECT)
			break;
		keyBackspace(result);
		break;
	
	case VK_LEFT :
		return keyArrowLeft(result);
	
	case VK_RIGHT :
		keyArrowRight(result);
		break;
	
	case VK_UP :
		keyArrowUp(result);
		break;
	
	case VK_DOWN :
		keyArrowDown(result);
		break;
	
	case VK_RETURN :
		keyReturn(result);
		break;
	
	case VK_PRIOR:
		keyPrior(result);
		break;
	
	case VK_NEXT:
		keyNext(result);
		break;
	
	case VK_HOME:
		keyHome(result);
		break;
	
	case VK_END:
		keyEnd(result);
		break;
	
	case VK_TAB:
		keyTab(result);
		break;
	}

	return 0;
}

void CamBase::SetVisible(bool visible, int col)
{
	bool	isvisible = (m_form->properties & PR_VISIBLE) ? true : false;

	if (visible)
		m_form->properties |= PR_VISIBLE;
	else
		m_form->properties &= ~PR_VISIBLE;

	if (isvisible != visible)
		invalidateRect(m_pRc, true, true);
}

bool CamBase::IsVisible(int col)
{
	return ((m_form->properties & PR_VISIBLE) ? true : false);
}

void CamBase::SetEnable(bool enable)
{
	bool	isenable = (m_form->properties & PR_ENABLE) ? true : false;

	if (enable)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	if (isenable != enable)
		invalidateRect(&m_pRc, false);
}

bool CamBase::IsEnable()
{
	return ((m_form->properties & PR_ENABLE) ? true : false);
}

void CamBase::SetAttributes(DWORD attr, bool set, int col, int row)
{
	if (attr)
	{
		if (set)
			m_form->attr |= attr;
		else
			m_form->attr &= ~attr;
	}
}

void CamBase::SetFgColor(int rgb, int col, int row)
{
	DWORD	color;

	if (rgb < 0)
	{
		if (m_itRGB < 0)
			return;
		rgb = m_itRGB;
	}

#ifdef _last
	color = (rgb & userCOLOR) ? rgb : getgRGB(rgb);
#else
	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
#endif	

	if (m_tRGB == color && ((rgb & userCOLOR) || (!(rgb & userCOLOR) && m_itRGB == rgb)))
		return;

	m_tRGB  = color;
	m_itRGB = (rgb & userCOLOR) ? -1 : rgb;
	invalidateRect(&m_pRc, false);
}

void CamBase::SetBkColor(int rgb, int col, int row)
{
	if ((m_form->attr & FA_CORGB) && (m_form->attr2 & SA_CORGB) == SA_PAINT)
		return;
	if ((m_form->attr & FA_CORGB) && (m_form->attr2 & SA_CORGB) == SA_PAINTSUP)
		return;

	DWORD	color = (rgb & userCOLOR) ? rgb : getgRGB(rgb);
	if (m_pRGB == color)
		return;

	m_pRGB = GRGB(color);
	invalidateRect(&m_pRc, false);
}

int CamBase::GetFgColor(int col, int row)
{
	return m_tRGB;
}

int CamBase::GetBkColor(int col, int row)
{
	return m_pRGB;
}

void CamBase::ReadAll(CString& data, bool edit, bool head)
{
	ReadData(data, edit);
}

void CamBase::ReadData(CString& data, bool edit, int col, int row)
{
	data = m_strR;
}

void CamBase::WriteAll(CString str, bool redraw)
{
	WriteData(str, redraw);
}

void CamBase::WriteData(CString data, bool redraw, int col, int row)
{
	if (!m_strR.Compare(data))
		return;

	m_strR = data;
	if (redraw && (m_form->properties & PR_VISIBLE))
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, false);
//		else
//			invalidateRect(&m_pRc, false);
	}
}

void CamBase::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, false);
//		else
//			invalidateRect(&m_pRc, false);
	}
}

CDC* CamBase::getDC()
{
	CDC* dc = m_axform->m_view->GetDC();
	m_ndc = dc->SaveDC();
	return dc;
}

void CamBase::releaseDC(CDC* dc)
{
	dc->RestoreDC(m_ndc);
	m_axform->m_view->ReleaseDC(dc);
}


ARGB CamBase::getgRGB(int index)
{
	return m_axform->m_pPalette->GetPaletteARGB(index);
}

int CamBase::getgINDEX(COLORREF rgb)
{
	return m_axform->m_pPalette->GetIndexARGB(rgb);
}


COLORREF CamBase::getRGB(int index)
{
//	return m_axform->m_palette->GetPaletteRGB(index);
#ifdef _last
	return getgRGB(index);
#else 
	return ARGBtoRGB(getgRGB(index));
#endif
}

int CamBase::getINDEX(COLORREF rgb)
{
//	return m_axform->m_palette->GetIndexRGB(rgb);
	return ARGBtoRGB(getgINDEX(GRGB(rgb)));
}

CRect CamBase::calcRect(CRect rect, float hR, float vR)
{
	CRect	tRc;

	tRc.left   = (int)(rect.left   * hR);
	tRc.right  = (int)(rect.right  * hR);
	tRc.top    = (int)(rect.top    * vR);
	tRc.bottom = (int)(rect.bottom * vR);

	return tRc;
}

void CamBase::setFontSize(float hR, float vR)
{
	TEXTMETRIC metric;
	CDC*	dc = getDC();

/*	if (fabs(hR - vR) > 0.35)	// 2006_06_27
	{
		if (hR > vR)
			vR++;
		else
			hR++;
	}
*/	float	point = (float)m_fPoint * __min(hR, vR);
	m_cfPoint = (int)point;
	if (point - (float)m_cfPoint > 0.5)
		m_cfPoint++;
	CFont *pfont, *old;
	pfont = setFont(dc);
	old = dc->SelectObject(pfont);

	dc->GetTextMetrics(&metric);
	m_fHeight = (BYTE)abs(metric.tmHeight);
	m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);

	dc->SelectObject(old);
	releaseDC(dc);
}

Font* CamBase::setgFont(int state)
{
	struct _fontR mf;
	mf.italic = false;
	mf.bold = 0;
	mf.name = m_fonts.GetBuffer(0);
	mf.point = m_cfPoint;
	
	if (m_form->fStyle == FS_ITALIC || m_form->fStyle == FS_IBOLD)
		mf.italic = true;
	if (state == 1 ||m_form->fStyle == FS_IBOLD || m_form->fStyle == FS_BOLD)
		mf.bold = 1;

	return m_axform->m_pAmDraw->SetFont(&mf);
}

CFont* CamBase::setFont(CDC* dc, int state)
{
	int	weight = FW_NORMAL;
	bool	italic = false;

	if (m_form->fStyle == FS_ITALIC || m_form->fStyle == FS_IBOLD)
		italic = true;

	switch (state)
	{
	case -1:	// deselected TAB
		weight = FW_NORMAL;	
		break;

	case 1:		// selected TAB
		weight = FW_BOLD;	
		break;

	case 0:		// normal FORM
	default:
		if (m_form->fStyle == FS_BOLD || m_form->fStyle == FS_IBOLD)
			weight = FW_BOLD;
		/*if (weight == FW_BOLD)
		{
			if (m_cfPoint < m_fPoint)
				weight = FW_MEDIUM;
		}*/

		if (GetSystemMetrics(SM_CXSCREEN) < 1024)
			weight = FW_MEDIUM;
		break;
	}

	struct _fontR mf;
	mf.italic = false;
	if (state == 1)
		mf.bold = 1;
	else 
		mf.bold = 0;

	mf.name = m_fonts.GetBuffer(0);
	mf.point = m_cfPoint;
	
	if (m_form->fStyle == FS_ITALIC || m_form->fStyle == FS_IBOLD)
		mf.italic = true;
	if (state == 1 ||m_form->fStyle == FS_IBOLD || m_form->fStyle == FS_BOLD)
		mf.bold = weight;
	return m_axform->m_pAmDraw->SetDcFont(&mf);
}


CFont* CamBase::setCellFont(CDC* dc, int resolution, struct _cellR* cellR)
{
	int	weight = FW_NORMAL;
	bool	italic = false;
			
	struct _fontR mf;
	mf.italic = false;
	
	mf.name = CString(cellR->fName);

	HDC hDC = dc->m_hDC;
	int nFontSize = -MulDiv( cellR->fPoint, 72, GetDeviceCaps( hDC, LOGPIXELSY ) );
	mf.point = nFontSize * (1 + resolution / 100);;

	//mf.point = cellR->fPoint * (1 + resolution / 100);	
	if (cellR->fStyle == FS_ITALIC || cellR->fStyle == FS_IBOLD)
		mf.italic = true;
	if (cellR->fStyle == FS_BOLD || cellR->fStyle == FS_IBOLD)
		mf.bold = FW_BOLD;

	return m_axform->m_pAmDraw->SetDcFont(&mf);
}

void CamBase::setTextColor(CDC* dc, COLORREF rgb)
{
	dc->SetTextColor(dc->IsPrinting() ? getRGB(cBLACK) : ARGBtoRGB(rgb));
}

void CamBase::setTextColor(CDC* dc)
{
	dc->SetTextColor(dc->IsPrinting() ? getRGB(cBLACK) : ARGBtoRGB(m_tRGB));
}

void CamBase::setPaintColor(CDC* dc, COLORREF rgb)
{
	if (dc->IsPrinting())
	{
		dc->SetBkMode(TRANSPARENT);
		return;
	}

//	if (m_form->properties & PR_TRANSPARENT)
//		dc->SetBkMode(TRANSPARENT);
//	else
//	{
		dc->SetBkMode(OPAQUE);
		dc->SetBkColor(rgb);
//	}
}

void CamBase::setPaintColor(CDC* dc)
{
	if (dc->IsPrinting())
	{
		dc->SetBkMode(TRANSPARENT);
		return;
	}

//	if (m_form->properties & PR_TRANSPARENT)
//		dc->SetBkMode(TRANSPARENT);
//	else
//	{
		dc->SetBkMode(OPAQUE);
		dc->SetBkColor(m_pRGB);
//	}
}

Pen* CamBase::setgPen(ARGB argb, int  width)
{
	struct	_penR pen;
	pen.clr = argb;
	pen.width = width; 

	Pen* pPen = m_axform->m_pAmDraw->SetPen(&pen);
	pPen->SetDashStyle(DashStyleSolid);
	return pPen;
}

Pen* CamBase::setgPen(int width)
{
#ifdef _last
	return setgPen(m_pRGB, width);
#else
	return setgPen(_ARGB(255, GetRValue(m_pRGB),  GetGValue(m_pRGB),  GetBValue(m_pRGB)), width);
#endif 
}

void CamBase::setPen(CDC* dc, COLORREF rgb, int width, int style)
{
	//m_axform->m_draw->SetPen(dc, rgb, width, style);
}

void  CamBase::setPen(CDC* dc, int width, int style)
{
//	if (m_form->properties & PR_TRANSPARENT)
//	{
		dc->SelectStockObject(NULL_PEN);
		return;
//	}
//	m_axform->m_draw->SetPen(dc, m_pRGB, width, style);
}

SolidBrush* CamBase::setgBrush(ARGB crARGB)
{
	return m_axform->m_pAmDraw->SetBrush(crARGB);
}

SolidBrush* CamBase::setgBrush()
{
#ifdef _last
	return setgBrush(m_pRGB);
#else
	return setgBrush(_ARGB(255, GetRValue(m_pRGB),  GetGValue(m_pRGB),  GetBValue(m_pRGB)));
#endif
}


SolidBrush* CamBase::setgFBrush()
{
#ifdef _last
	return setgBrush(m_tRGB);
#else
	return setgBrush(_ARGB(255, GetRValue(m_tRGB),  GetGValue(m_tRGB),  GetBValue(m_tRGB) ));
#endif
}

CBrush* CamBase::setBrush(CDC* dc, COLORREF rgb)
{
	return NULL;
//	if (dc->IsPrinting())
//		return m_axform->m_draw->SetBrush(dc, getRGB(cWHITE));

//	return m_axform->m_draw->SetBrush(dc, rgb);
}


CBrush* CamBase::setBrush(CDC* dc)
{
	/*
	if (dc->IsPrinting())
		return m_axform->m_draw->SetBrush(dc, getRGB(cWHITE));

	return m_axform->m_draw->SetBrush(dc, m_pRGB);
	*/
	return NULL;
}

void CamBase::setBkColor(int rgb)
{
	DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (m_pRGB == color)
		return;

	m_pRGB = color;
	invalidateRect(&m_pRc, false);
}

void CamBase::invalidateRect(LPCRECT lpRect, bool erase, bool force)
{
	if (!force && !(m_form->properties & PR_VISIBLE))
		return;
	
	CRect	tRc;
	BOOL	Erase = erase ? TRUE : FALSE;
	
	tRc = lpRect;

	// GDI+ 글자 안 그려지는 원인 
	//tRc.OffsetRect(m_size);
	m_axform->m_view->InvalidateRect(tRc, Erase);
	
}

void CamBase::setCaretPos(CPoint pt)
{
	CPoint	point = pt;
	point.Offset(m_size);
	m_axform->m_view->SetCaretPos(point);
}



void CamBase::makeData(CString& data, int flen, DWORD attr, DWORD attr2, int type)
{
	int	ii;
	flen = min(data.GetLength(), flen);
	if (flen <= 0)
		return;

	if (attr & FA_ZEROSUP)
	{		
		for (ii = 0; ii < flen; ii++)
		{
			if (data[ii] != '0')
				break;
		}
		if (ii >= flen)
		{
			data = CString(' ', flen);
			switch (attr2 & SA_ZEROSUP)
			{
			case SA_SPACE:
				data.SetAt(flen-1, ' ');
				break;
			case SA_ASTER:
				data.SetAt(flen-1, '*');
				break;
			case SA_ZERO:
				data.SetAt(flen-1, '0');
			default:
				break;
			}
		}		
	}

	char	sign, zero;
	int	index;

	sign = zero = ' ';
	switch (attr & FA_COMMA)
	{
	case FA_ZEROSUP:
		{
			int	idx = 0;
			char	suppress;
			switch (attr2 & SA_ZEROSUP)
			{
			case SA_ASTER:
				suppress = '*';
				break;
			case SA_SPACE:
				suppress = ' ';
				break;
			case SA_ZERO:
				suppress = '0';
				break;
			}

			for (ii = 0; ii < flen; ii++)
			{				
				switch (suppress)
				{				
				case '0':
					idx++;
					continue;
				case ' ':
					idx++;
					continue;
				case '*':
					idx++;
					continue;
				default:					
					break;
				}				
			}
			
			switch (attr2 & SA_ZEROSUP)
			{
			case SA_ASTER:
			case SA_SPACE:
			case SA_ZERO:
				data = data.Mid(idx);
				break;
			}
		
		}
		break;
	case FA_COMMA:
		for (ii = 0; ii < flen; ii++)
		{
			switch (data[ii])
			{
			case '+':
			case '-':
				sign  = data[ii];
				index = ii;
			case '0':
				zero = data[ii];
				data.SetAt(ii, ' ');
				continue;
			case ' ':
				zero = ' ';
				continue;
			case '.':
				if (zero == '0')
					data.SetAt(--ii, zero);
				break;
			default:
				if ((attr2 & SA_ZEROSUP) == SA_ZERO && sign != ' ')
					index = ii-1;
				break;
			}
			break;
		}

		if (ii >= flen && zero != ' ')
		{
			switch (attr2 & SA_ZEROSUP)
			{
			case SA_ASTER:
				break;
			case SA_SPACE:
				if (attr & FA_ZEROSUP)
					break;
			case SA_ZERO:
			default:
				ii = flen -1;
				data.SetAt(ii, '0');
				break;
			}
		}
		if (sign != ' ' && ii < flen && ii)
			data.SetAt(index, sign);
		break;

	default:
	//	if (!(attr2 & SA_ZEROSUP))
		if (!(attr & FA_ZEROSUP))
			break;

		if (/*(attr & FA_RIGHT) &&*/ (type == IO_NUM || type == IO_DIGIT))
		{
			int	space = 0;
			for (ii = 0; ii < flen; ii++)
			{
				switch (data[ii])
				{
				case '+':
				case '-':
					sign  = data[ii];
					index = ii;
				case '0':
					zero = data[ii];
					data.SetAt(ii, ' ');					
					continue;
				case ' ':
					zero = ' ';
					space++;
					continue;				
				case '.':
					if (zero == '0')
						data.SetAt(--ii, zero);
					break;
				default:
					if ((attr2 & SA_ZEROSUP) == SA_ZERO && sign != ' ')
						index = ii-1;
					break;
				}
				if (sign != ' ' && ii < flen && ii)
					data.SetAt(index, sign);
				break;
			}


			



			if (ii >= flen)
			{
				switch (attr2 & SA_ZEROSUP)
				{
				case SA_ASTER:
					break;
				case SA_SPACE:
					if (attr & FA_ZEROSUP)
						break;
				default:
					if (space >= flen)
						break;
				case SA_ZERO:
					data.SetAt(flen-1, '0');
					break;
				}
			}
		}
		break;
	}
}


void CamBase::doEditCORGB(CString& data)
{
//	if (m_form->onEdit == NOVALUE)
//		return;

	int	rgb = getCORGB(m_form->onEdit, data, m_form->size, m_form->attr2 & SA_CORGB);
	rgb = (rgb < 0) ? m_form->tRGB : rgb;
	SetFgColor(rgb);
}

int CamBase::getCORGB(DWORD coRGB, CString& data, int flen, int supress)
{
	int	value = 0;
	int	index;
	DWORD	upRGB, dnRGB;

	flen = min(data.GetLength(), flen);
	if (flen <= 0)
 		return	-1;

	if (coRGB != 0)
	{
		upRGB = HIWORD(coRGB);
		dnRGB = LOWORD(coRGB);
	}
	else
		upRGB = dnRGB = -1;

	for (int ii = 0; ii < flen; ii++)
	{
		if (data[ii] != ' ')
		{
			index = ii;
			if (data[ii] & 0x80)
				value = (data[ii++] & 0xff) << 8;
			value |= data[ii] & 0xff;
			break;
		}
	}

	bool	contrast;
	contrast = m_axform->IsContrast(value);

	switch (value)
	{
	case '+':
		switch (supress)
		{
		case SA_COSUP:
		case SA_COSIGN:
			if (contrast)
				data.SetAt(index++, ' ');
			data.SetAt(index, ' ');
		default:
			break;
		}
		return	(int)upRGB;
	case '-':
		if (supress == SA_COSUP)
		{
			if (contrast)
				data.SetAt(index++, ' ');
			data.SetAt(index, ' ');
		}
		return	(int)dnRGB;
	default:
		return	-1;
	}

	return	-1;
}


void CamBase::makeSignData(CString& data, int col, int row)
{
	if (!(m_form->attr & FA_CORGB) || data.IsEmpty())
		return;

	// add Supress + Background 2014. 01. 29.
	if (!((m_form->attr2 & SA_CORGB) & SA_PAINT) || 
		!((m_form->attr2 & SA_CORGB) & SA_TEXT) ||
		!((m_form->attr2 & SA_CORGB) & SA_PAINTSUP) ||
		!((m_form->attr2 & SA_CORGB) & SA_TEXTSUP))
		return;

	CString	tmps;
	int		state;
	int		rgb;
	DWORD	upRGB, dnRGB;


	if (data.FindOneOf("+-") != -1)
	{
		data.Remove('+');
		data.Remove('-');
	}

	int len = m_form->size - data.GetLength();
	state	= m_axform->GetContrast(data[0], tmps);
//	tmps   += CString(' ', len);
	tmps	+= CString(' ', 2);

	data = data.Mid(1);	
	TRIM(data);

	// add Supress + Background 2014. 01. 29.
	switch (m_form->attr2 & SA_CORGB)		
	{
	case SA_TEXT:
	case SA_PAINT:
		tmps += data;
		data = tmps;
		break;
	}

	if (m_form->onEdit != 0)
	{
		upRGB =	HIWORD(m_form->onEdit);		
		dnRGB = LOWORD(m_form->onEdit);	
	}
	else 
		upRGB =	dnRGB = m_form->tRGB;	

	switch (state)
	{
	case ct_uLimit:
	case ct_Rise:
		rgb = /*(m_form->onEdit == NOVALUE) ? cRGB_UP :*/ upRGB;
		break;
	case ct_lLimit:
	case ct_Fall:
		rgb = /*(m_form->onEdit == NOVALUE) ? cRGB_DN :*/ dnRGB;
		break;
	case ct_Flat:
	default:
		rgb = m_form->tRGB;	
		break;
	}
	rgb = (rgb < 0) ? m_form->tRGB : rgb;
	
//	if ((m_form->attr2 & SA_CORGB) == SA_COSUP)
//		data = data.Mid(2);

	// add Supress + Background 2014. 01. 29.
	if ((m_form->attr2 & SA_CORGB) == SA_PAINT || (m_form->attr2 & SA_CORGB) == SA_PAINTSUP)
	{
		// 상,하한 Condition color 변경
		//if (state != ct_Flat)
		if (state == ct_uLimit || state == ct_lLimit)
		{
			setBkColor(rgb);
			SetFgColor(getgINDEX(clWHITE));
		}
		else
		{
			setBkColor(m_form->pRGB);
			SetFgColor(rgb);
		}
	}
	else	
		SetFgColor(rgb);
}

bool CamBase::checkKey(int& key, BYTE type, DWORD attr)
{
	switch (type)
	{
	case IO_NUM:
		switch (key)
		{
		case ',':
			if (attr & FA_COMMA || attr & FA_FLOAT)
				return false;
		case '.':
		case '+':
		case '-':
			break;
		default:
			if (key >= '0' && key <= '9')
				break;
			return false;
		}
		break;

	case IO_DIGIT:
		if (key >= '0' && key <= '9')
			break;
		return false;

	case IO_ALPHA:
		if (key & 0x80)
			return false;
		break;

	case IO_HAN:
		key = m_axform->GetDouble(key);
	case IO_ANY:
	case IO_MIXED:
		break;
	}

	return true;
}


ARGB CamBase::getgReverseColor(ARGB argb)
{
	BYTE	rr, gg, bb, aa;
	
	aa = (argb & 0xFF000000) >> 24;
	rr = 0xff - GetRValue(argb);
	gg = 0xff - GetGValue(argb);
	bb = 0xff - GetBValue(argb);
	return _ARGB(aa, rr, gg, bb);
}

COLORREF CamBase::getReverseColor(COLORREF rgb)
{
	BYTE	rr, gg, bb;

	rr = 0xff - GetRValue(rgb);
	gg = 0xff - GetGValue(rgb);
	bb = 0xff - GetBValue(rgb);

	return PALETTERGB(rr, gg, bb);
}

wch_t CamBase::whichHangulEdit(int pos, CString str)
{
	if (pos >= str.GetLength())
		return WC_AN;

	int	idx;

	for (idx = 0; idx < pos; idx++)
		if (str[idx] & 0x80)
			idx++;

	switch (idx - pos)
	{
	case 0:
		if (str[idx] & 0x80)
			return WC_H1;
		return WC_AN;
	case 1:
		return WC_H2;
	}
	return WC_AN;
}

int CamBase::getSIGN(CString str, int& offs)
{
	switch (m_axform->m_LCtype)
	{
	case LANG_KOREAN:
		offs = str.Find(m_axform->m_LCcontrast[ct_uLimit]);
		if (offs != -1)
			return ct_uLimit;

		offs = str.Find(m_axform->m_LCcontrast[ct_lLimit]);
		if (offs != -1)
			return ct_lLimit;

		offs = str.Find(m_axform->m_LCcontrast[ct_Rise]);
		if (offs != -1)
			return  ct_Rise;

		offs = str.Find(m_axform->m_LCcontrast[ct_Fall]);
		if (offs != -1)
			return ct_Fall;
		break;

	case LANG_CHINESE:
		offs = str.Find(m_axform->m_LCcontrast[ct_uLimit]);
		if (offs != -1)
			return ct_Rise;

		offs = str.Find(m_axform->m_LCcontrast[ct_lLimit]);
		if (offs != -1)
			return ct_Fall;

		offs = str.Find(m_axform->m_LCcontrast[ct_Rise]);
		if (offs != -1)
			return  ct_Rise;

		offs = str.Find(m_axform->m_LCcontrast[ct_Fall]);
		if (offs != -1)
			return ct_Fall;
		break;
	}
	return ct_Flat;
}


void CamBase::drawPolygonText(CDC* dc, CRect rect, CString str, BYTE align, BYTE type)
{
	CRect	tRc, pRc, lRc;
	CSize	size;
	int	sign;
	int	offs = 0;
	CString	tmps;
	Graphics g(dc->GetSafeHdc());
	StringFormat sf;
	RectF	rf;
	CString s;


	sf.SetLineAlignment(StringAlignmentCenter);
	sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
	sf.SetHotkeyPrefix(HotkeyPrefixNone);

	UINT	format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

	switch (align)
	{
	case AL_RIGHT:
		sf.SetAlignment(StringAlignmentFar);
		format |= DT_RIGHT;	break;
	case AL_CENTER:
		sf.SetAlignment(StringAlignmentCenter);

		format |= DT_CENTER;	break;
	case AL_LEFT:
	default:
		sf.SetAlignment(StringAlignmentNear);
		format |= DT_LEFT;	break;
	}

	lRc = tRc = pRc = rect;
	if (m_form->kind == FM_OUT)
	{
		pRc.top++;
		pRc.bottom -= 2;
	}

	size = CSize(0, 0);
	sign = getSIGN(str, offs);
	if (sign != ct_Flat && offs != -1 && align != AL_CENTER && type != IO_MIXED)
	{
		Point	pts[7];
		float	hh = m_fHeight+2;

		tmps = (offs > 0) ? str.Left(offs) : _T("");
		str  = (str.GetLength() > offs+2) ? str.Mid(offs+2) : _T("");
		switch (align)
		{
		case AL_LEFT:
			if (!tmps.IsEmpty())
			{
				g.MeasureString(tmps.AllocSysString(), -1, setgFont(), PointF(0, 0), &rf);
				size = CSize((int)rf.Width + 3, (int)rf.Height);
				//size = dc->GetOutputTextExtent(tmps);
				lRc.right = lRc.left + size.cx;
				pRc.left  = lRc.right;
			}

			s = m_axform->m_LCcontrast[ct_uLimit];
			g.MeasureString(s.AllocSysString(), -1, setgFont(), PointF(0, 0), &rf);
			size = CSize((int)rf.Width + 3, (int)rf.Height);
			
			//size = dc->GetOutputTextExtent(m_axform->m_LCcontrast[ct_uLimit]);
			pRc.right = pRc.left + size.cx;
			tRc.left = pRc.right;
			break;

		case AL_RIGHT:
			g.MeasureString(str.AllocSysString(), -1, setgFont(), PointF(0, 0), &rf);
			size = CSize((int)rf.Width + 3, (int)rf.Height);
			//size = dc->GetOutputTextExtent(str);
			tRc.left = tRc.right - size.cx;
			s = m_axform->m_LCcontrast[ct_uLimit];
			g.MeasureString(s.AllocSysString(), -1, setgFont(), PointF(0, 0), &rf);
			size = CSize((int)rf.Width + 3, (int)rf.Height);
			//size = dc->GetOutputTextExtent(m_axform->m_LCcontrast[ct_uLimit]);
			pRc.right = tRc.left;
			pRc.left  = pRc.right - size.cx;
			if (!tmps.IsEmpty())
			{
				size = dc->GetOutputTextExtent(tmps);
				lRc.right = pRc.left;
				lRc.left  = lRc.right - size.cx;
			}
			break;
		}

		if (!tmps.IsEmpty())
		{
			lRc.left   = max(lRc.left, rect.left);
			lRc.right  = min(lRc.right, rect.right);
			dc->DrawText(tmps, lRc, format);
		}

		pRc.top = (tRc.top + tRc.Height()/2) - (int)(hh/2);
		pRc.bottom = (int)(pRc.top + hh);
		
		s = m_axform->m_LCcontrast[ct_uLimit];
		g.MeasureString(s.AllocSysString(), -1, setgFont(), PointF(0, 0), &rf);
		size = CSize((int)rf.Width, (int)rf.Height);
		//size = dc->GetOutputTextExtent(m_axform->m_LCcontrast[ct_uLimit]);
		if (size.cx > 10)
			pRc.left = (int)(pRc.right - (size.cx * 0.8));
		else
			pRc.left = pRc.right - size.cx;

		hh = (float)(int)(pRc.Height()+1)/2;
		hh = (float)(int)((hh+1)/2) * 2 - 1;
		int	ww = pRc.Width();
		if (ww >= 4)
			ww = (ww / 2 * 2);
		pRc.right = pRc.left + ww;
		int	rr = ww % 3;
		ww /= 3;
		if (rr >= 2 && pRc.Width() > 3)
			ww++;

	//	dc->FillSolidRect(pRc, RGB(255, 0 ,0));

		if (pRc.left >= rect.left && pRc.right <= rect.right)
		{
			//dc->SetPolyFillMode(ALTERNATE);
			switch (sign)
			{
			case ct_uLimit:
				pts[1].Y = pRc.top + (int)hh;
				pts[2].Y = pRc.top + (int)hh;
				pts[5].Y = pRc.top + (int)hh;
				pts[6].Y = pRc.top + (int)hh;
				pts[0].Y = pRc.top;
				pts[3].Y = pRc.bottom;
				pts[4].Y = pRc.bottom;

				pts[0].X = pRc.left + (pRc.Width()/2);
				pts[1].X = pRc.left;
				pts[6].X = pRc.right;
				pts[2].X = pRc.left + ww;
				pts[3].X = pRc.left + ww;
				pts[4].X = pRc.right - ww;
				pts[5].X = pRc.right - ww;
				//dc->Polygon(pts, 7);
				g.FillPolygon(setgFBrush(), pts, 7, FillModeAlternate);
				break;

			case ct_lLimit:
				pts[1].Y = pRc.top + (int)hh;
				pts[2].Y = pRc.top + (int)hh;
				pts[5].Y = pRc.top + (int)hh;
				pts[6].Y = pRc.top + (int)hh;
				pts[0].Y = pRc.bottom;
				pts[3].Y = pRc.top;
				pts[4].Y = pRc.top;

				pts[0].X = pRc.left + (pRc.Width()/2);
				pts[1].X = pRc.right;
				pts[6].X = pRc.left;
				pts[2].X = pRc.right - ww;
				pts[3].X = pRc.right - ww;
				pts[4].X = pRc.left + ww;
				pts[5].X = pRc.left + ww;
	//			dc->Polygon(pts, 7);
				g.FillPolygon(setgFBrush(), pts, 7, FillModeAlternate);
				break;

			case ct_Rise:
				pRc.top += 2;
				pRc.bottom -= 2;
				if (!(pRc.Height() % 2))
					pRc.top++;
				pts[0].Y = pRc.top;
				pts[1].Y = pRc.bottom;
				pts[2].Y = pRc.bottom;

				pts[0].X = pRc.left + (pRc.Width()/2);
				pts[1].X = pRc.left;
				pts[2].X = pRc.right;
				g.FillPolygon(setgFBrush(), pts, 3, FillModeAlternate);
				//dc->Polygon(pts, 3);
				break;

			case ct_Fall:
				pRc.top += 2;
				pRc.bottom -= 2;
				if (!(pRc.Height() % 2))
					pRc.top++;
				pts[0].Y = pRc.bottom;
				pts[1].Y = pRc.top;
				pts[2].Y = pRc.top;

				pts[0].X = pRc.left + (pRc.Width()/2);
				pts[1].X = pRc.right;
				pts[2].X = pRc.left;
				//dc->Polygon(pts, 3);
				g.FillPolygon(setgFBrush(), pts, 3, FillModeAlternate);
				break;
			}
		}
	}
	tRc.left   = max(tRc.left, rect.left);
	tRc.right  = min(tRc.right, rect.right);
	dc->DrawText(str, tRc, format);
	//g.DrawString(str.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, setgFBrush());
}


void CamBase::drawEdge(CDC *pDC, CRect rect, UINT edge, UINT flags)
{
	Graphics g(pDC->GetSafeHdc());
	drawEdge(g, rect, edge, flags);
}

void CamBase::drawEdge(Graphics& g, CRect rect, UINT edge, UINT flags)
{
//	Graphics g(dc->GetSafeHdc());
	CRect	tRc;
	PointF	pts[6];
	int	gap = 1;

	switch (edge)
	{
	case EDGE_LINE:
		tRc = rect;
		g.DrawRectangle(setgPen(), RECTG(tRc));
		break;
	case EDGE_BUMP:		// raised_outer | sunken_inner
		tRc = rect;
		tRc.DeflateRect(1, 1, 0, 0);
		g.DrawRectangle(setgPen(clEDGE), RECTG(tRc));
		
		tRc = rect;
		tRc.DeflateRect(0, 0, 1, 1);
		g.DrawRectangle(setgPen(clYWHITE), RECTG(tRc));
		break;

	case EDGE_ETCHED:	// sunken_outer | raised_inner
		{
			PointF	pLT, pLB, pRT, pRB, p1, p2;

			Pen dnPen(Color(255, 65, 65, 65));
			Pen upPen(Color(255, 195, 195, 195));

			pLT.X = (REAL)rect.left;
			pLT.Y = (REAL)rect.top;

			pLB.X = (REAL)rect.left;
			pLB.Y = (REAL)rect.bottom;

			pRT.X = (REAL)rect.right;
			pRT.Y = (REAL)rect.top;

			pRB.X = (REAL)rect.right;
			pRB.Y = (REAL)rect.bottom;

			g.DrawLine(&upPen, pLT, pLB);
			g.DrawLine(&upPen, pLB, pRB);
			g.DrawLine(&upPen, pRB, pRT);
			g.DrawLine(&upPen, pRT, pLT);

			p1.X = (REAL)(rect.left + 1);
			p1.Y = (REAL)(rect.top + 1);
			p2.X = (REAL)(rect.left + 1);
			p2.Y = (REAL)(rect.bottom - 1);
			g.DrawLine(&dnPen, p1, p2);

			p1.X = (REAL)rect.left;
			p1.Y = (REAL)(rect.bottom + 1);
			p2.X = (REAL)(rect.right + 1);
			p2.Y = (REAL)(rect.bottom + 1);
			g.DrawLine(&dnPen, p1, p2);

			p1.X = (REAL)(rect.right + 1);
			p1.Y = (REAL)(rect.bottom + 1);
			p2.X = (REAL)(rect.right + 1);
			p2.Y = (REAL)rect.top;
			g.DrawLine(&dnPen, p1, p2);

			p1.X = (REAL)(rect.left + 1);
			p1.Y = (REAL)(rect.top + 1);
			p2.X = (REAL)(rect.right - 1);
			p2.Y = (REAL)(rect.top + 1);
			g.DrawLine(&dnPen, p1, p2);
		}
		break;

	case EDGE_RAISED:	// raised_outer | raised_inner
		pts[0].X = (REAL)(rect.right-gap);	pts[0].Y = (REAL)rect.top;
		pts[1].X = (REAL)rect.left;			pts[1].Y = (REAL)rect.top;
		pts[2].X = (REAL)rect.left;			pts[2].Y = (REAL)(rect.bottom-gap);
		pts[3].X = (REAL)rect.left;			pts[3].Y = (REAL)pts[2].Y;
		pts[4].X = (REAL)pts[3].X;			pts[4].Y = (REAL)rect.top;
		pts[5].X = (REAL)pts[0].X;			pts[5].Y = (REAL)pts[4].Y;
	
		g.DrawPolygon(setgPen(clYWHITE), pts, 6);
		g.FillPolygon(setgBrush(clYWHITE), pts, 6, FillModeAlternate);

		pts[0].X = (REAL)(rect.right-gap);	pts[0].Y = (REAL)rect.top;
		pts[1].X = (REAL)pts[0].X;			pts[1].Y = (REAL)(rect.bottom-gap);
		pts[2].X = (REAL)rect.left;			pts[2].Y = (REAL)pts[1].Y;
		pts[3].X = (REAL)rect.left;			pts[3].Y = (REAL)(rect.bottom-gap);
		pts[4].X = (REAL)(rect.right-gap);	pts[4].Y = (REAL)(rect.bottom-gap);
		pts[5].X = (REAL)(rect.right-gap);	pts[5].Y = (REAL)rect.top;
		
		g.DrawPolygon(setgPen(clEDGE), pts, 6);
		g.FillPolygon(setgBrush(clEDGE), pts, 6, FillModeAlternate);

		break;

	case EDGE_SUNKEN:	// sunken_outer | sunken_inner
		pts[0].X = (REAL)(rect.right-gap);	pts[0].Y = (REAL)rect.top;
		pts[1].X = (REAL)rect.left;			pts[1].Y = (REAL)rect.top;
		pts[2].X = (REAL)rect.left;			pts[2].Y = (REAL)(rect.bottom-gap);
		pts[3].X = (REAL)rect.left;			pts[3].Y = (REAL)pts[2].Y;
		pts[4].X = (REAL)pts[3].X;			pts[4].Y = (REAL)rect.top;
		pts[5].X = (REAL)pts[0].X;			pts[5].Y = (REAL)pts[4].Y;
		
		g.DrawPolygon(setgPen(clEDGE), pts, 6);
		g.FillPolygon(setgBrush(clEDGE), pts, 6, FillModeAlternate);

		pts[0].X = (REAL)(rect.right-gap);	pts[0].Y = (REAL)rect.top;
		pts[1].X = (REAL)pts[0].X;			pts[1].Y = (REAL)(rect.bottom-gap);
		pts[2].X = (REAL)rect.left;			pts[2].Y = (REAL)pts[1].Y;
		pts[3].X = (REAL)rect.left;			pts[3].Y = (REAL)(rect.bottom-gap);
		pts[4].X = (REAL)(rect.right-gap);	pts[4].Y = (REAL)(rect.bottom-gap);
		pts[5].X = (REAL)(rect.right-gap);	pts[5].Y = (REAL)rect.top;

		g.DrawPolygon(setgPen(clYWHITE), pts, 6);
		g.FillPolygon(setgBrush(clYWHITE), pts, 6, FillModeAlternate);
		break;
	}
}

CString CamBase::convertVString(CString src)
{
	CString text = _T("");

	for (int ii = 0; ii < src.GetLength(); ii++)
	{
		if (src[ii] & 0x80)
		{
			text += src.Mid(ii, 2);
			ii++;
		}
		else
			text += src.Mid(ii, 1);

		text += '\n';
	}

	return text;
}

void CamBase::formatData(int attr, CString& data, int nfloat)
{
	if (!(attr & (FA_COMMA|FA_FLOAT)))
		return;

	data.TrimLeft();
	data.TrimRight();

	if (data.IsEmpty())
		return;

	int	idx;
	CString	signs, floats;

	signs = m_axform->TrimSign(data);
	if (!(attr & FA_FLOAT))
		nfloat = 0;

	idx = data.Find('.');
	if (idx != -1)
	{
		floats = data.Mid(idx+1);
		data = data.Left(idx);
	}
	else if (nfloat > 0)
	{
		if (nfloat < data.GetLength())
		{
			floats = data.Right(nfloat);
			data = data.Left(data.GetLength()-nfloat);
		}
		else
		{
			floats = data;
			data.Empty();
		}
	}

	for (idx = 0; idx < data.GetLength(); idx++)
	{
		switch (data[idx])
		{
		case ' ':
		case '0':
			signs += ' ';
			continue;
		default:
			break;
		}
		break;
	}
	data = data.Mid(idx);
	if (data.IsEmpty())
	{
		data += '0';
		if (!signs.IsEmpty())
			signs.Delete(signs.GetLength()-1);
	}

	if (attr & FA_COMMA)
	{
		CString	tmps;

		idx += data.GetLength();
		--idx /= 3;

		data.MakeReverse();
		for (int ii = 0, jj = 0; ii < data.GetLength(); ii++)
		{
			if (isdigit(data.GetAt(ii)))
			{
				if (jj && (jj % 3) == 0)
				{
					idx--;
					tmps += ',';
				}
				jj++;
			}
			else	jj = 0;
			tmps += data.GetAt(ii);
		}
		for (; idx > 0; idx--)
			signs += ' ';

		tmps.MakeReverse();
		data = tmps;
	}

	data = signs + data;
	if (nfloat > 0)
	{
		floats.Replace(' ', '0');
		for (idx = floats.GetLength(); idx < nfloat; idx++)
			floats += '0';

		floats = floats.Left(nfloat);
//		if (attr & FA_ENUM && !atoi(floats))
//			return;

	}
	else if (nfloat == -1 || floats.IsEmpty())
		return;

	data += '.';
	data += floats;
}


void CamBase::drawGRoundRect(Graphics* g, SolidBrush* brush, CRect rc, int radius)
{
	int x = rc.left;
	int y = rc.top;
	int width = rc.Width();
	int height = rc.Height();

	GraphicsPath pg;

	pg.AddLine( x + radius, y, x + width - radius, y );   
	pg.AddArc( x + width - radius, y, radius, radius, 270, 90 );			// rigthtop
	pg.AddLine( x + width, y + 0, x + width, y + height - 0 );   
	pg.AddArc( x + width - 0, y + height - 0, 0, 0, 0, 90 );			// bottomright
	pg.AddLine( x + width - 0, y + height, x + 0, y + height );   
	pg.AddArc( x, y + height - 0, 0, 0, 90, 90 );					// bottomLeft   
	pg.AddLine( x, y + height - radius, x, y + radius );   
	pg.AddArc( x, y, radius, radius, 180, 90 );					// lefttop
	pg.CloseFigure();

	g->FillPath(brush, &pg);

}

void CamBase::drawG3DRect(Graphics* g, CRect rect, ARGB clUp, ARGB clDn)
{
	Pen *upPen, *dnPen;
	PointF	pLT, pLB, pRT, pRB;

	struct	_penR sclUp, sclDn;

	sclUp.clr = clUp;
	sclDn.clr = clDn;
	sclUp.width = sclDn.width = 1;

	upPen = m_axform->m_pAmDraw->SetPen(&sclUp);
	dnPen = m_axform->m_pAmDraw->SetPen(&sclDn);

	pLT.X = (REAL)rect.left;
	pLT.Y = (REAL)rect.top;

	pLB.X = (REAL)rect.left;
	pLB.Y = (REAL)rect.bottom;

	pRT.X = (REAL)rect.right;
	pRT.Y = (REAL)rect.top;

	pRB.X = (REAL)rect.right;
	pRB.Y = (REAL)rect.bottom;

	g->DrawLine(dnPen, pLT, pLB);
	g->DrawLine(dnPen, pLB, pRB);
	g->DrawLine(upPen, pLT, pRT);
	g->DrawLine(upPen, pRT, pRB);
}

COLORREF CamBase::ARGBtoRGB(DWORD argb)
{
	bool	bret = false;
	DWORD	rgb;

	int a = (argb & 0xFF000000) >> 24;
	int r = (argb & 0x00FF0000) >> 16;
	int g = (argb & 0x0000FF00) >> 8;
	int b = (argb & 0x000000FF);

	rgb = PALETTERGB(r, g, b);

	if (rgb & 0x02000000)
		bret = true;

	return rgb;
//	return bret;
}

Bitmap* CamBase::DrawFrameControl(CDC* dc, CRect rc, UINT nType, UINT nState)
{
	CDC memDC;
	memDC.CreateCompatibleDC(dc);
	HBITMAP hbmp;
	hbmp = CreateCompatibleBitmap(dc->m_hDC, rc.Width(), rc.Height());
	memDC.SelectObject(hbmp);

//	CFont 
//	setFont(&memDC, 0);


#ifdef _last
	memDC.FillSolidRect(&CRect(0, 0, rc.Width(), rc.Height()), ARGBtoRGB(m_pRGB));
#else
	memDC.FillSolidRect(&CRect(0, 0, rc.Width(), rc.Height()), m_pRGB);
#endif

	memDC.DrawFrameControl(CRect(0,0, rc.Width(), rc.Height()), nType, nState);
	Bitmap* pBitmap = Bitmap::FromHBITMAP(hbmp, NULL);

	DeleteObject(hbmp);
	
	return pBitmap;
}