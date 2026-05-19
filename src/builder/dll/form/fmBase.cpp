// fmBase.cpp : implementation file
//

#include "stdafx.h"
#include "fmBase.h"
#include "axform.h"
#include <math.h>

#include "../../h/axiserr.h"
#include "../lib/axislib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	char*	LCcontrast[];

/////////////////////////////////////////////////////////////////////////////
// CfmBase

IMPLEMENT_DYNCREATE(CfmBase, CCmdTarget)

CfmBase::CfmBase()
{
	EnableAutomation();
}

CfmBase::CfmBase(CAxisForm* axform, struct _formR* formR)
{
	EnableAutomation();

	m_axform = axform;
	m_size   = CSize(0, 0);
	m_name   = _T("");
	m_rts    = -1;

	if (formR == (struct _formR *) NULL)
		return;

	m_form = formR;

	m_name = m_axform->GetString(m_form->name);
	if (m_form->fStyle == FS_NONE)
	{
		m_form->fStyle = m_axform->m_mapH->fStyle;
		m_form->fPoint = m_axform->m_mapH->fPoint;
		m_fonts = m_axform->m_fonts;
	}
	else
		m_fonts = m_axform->GetString(m_form->fName);

	if (m_form->kind != FM_GRID && m_form->isEdit == IE_CORGB)
	{
		m_form->isEdit = IE_NOP;
		if (m_form->attr & FA_CORGB && m_form->onEdit != NOVALUE)
			m_form->isEdit = IE_CORGB;
	}
	
	if (m_form->kind == FM_EDIT || m_form->kind == FM_OUT)
	{
		if (m_form->type != IO_NUM)
			m_form->attr &= ~(FA_COMMA|FA_FLOAT|FA_RIGHT);
	}

	m_strR = m_axform->GetString(m_form->vals[0]);
	switch (m_form->kind)
	{
	case FM_EDIT:
	case FM_COMBO:
	case FM_OUT:
	case FM_GRID:
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

	m_focus = false;
	m_hR    = 1.0;
	m_vR    = 1.0;
}

CfmBase::~CfmBase()
{
}


void CfmBase::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmBase, CCmdTarget)
	//{{AFX_MSG_MAP(CfmBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmBase, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CfmBase)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmBase to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6E6307E3-9BA5-489A-B7A2-B6F4AE0F3752}
static const IID IID_IfmBase =
{ 0x6e6307e3, 0x9ba5, 0x489a, { 0xb7, 0xa2, 0xb6, 0xf4, 0xae, 0xf, 0x37, 0x52 } };

BEGIN_INTERFACE_MAP(CfmBase, CCmdTarget)
	INTERFACE_PART(CfmBase, IID_IfmBase, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmBase message handlers

bool CfmBase::IsValid(int& guide)
{
	if (m_form->attr & FA_MAND)
	{
		if (!GetLength())
		{
			guide = AE_EMUST;
			return false;
		}
	}

	return true;
}

CRect CfmBase::GetRect(bool original)
{
	return original ? m_rect : m_pRc;
}

void CfmBase::SetRect(CRect rect, CRect pRc)
{
	m_rect = rect;
	m_pRc  = pRc;
}

void CfmBase::Adjust(CSize size)
{
	m_size = size;
}

void CfmBase::Adjust(CPoint pt, float hR, float vR)
{
	m_rect = calcRect(m_rect, hR, vR);
	m_rect.left   += pt.x;
	m_rect.right  += pt.x;
	m_rect.top    += pt.y;
	m_rect.bottom += pt.y;
	m_pRc = m_rect;

	m_fPoint = m_cfPoint = (int)((float)m_fPoint * vR);
	setFontSize(hR, vR);
}

void CfmBase::Offset(CSize offs)
{
	m_pRc.OffsetRect(offs);
}

void CfmBase::Resize(float hR, float vR, bool font, bool redraw)
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
		setFontSize(hR, vR);
	if (redraw)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&iRc, true);
		else
			invalidateRect(&iRc, false);
	}
}

void CfmBase::Reload()
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

int CfmBase::IsPointInRect(CPoint pt)
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

void CfmBase::SetFont(int point, int style, CString fonts, bool resize)
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

		if (m_fPoint != point)
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

			if (m_form->properties & PR_TRANSPARENT)
				invalidateRect(&m_pRc, true);
			else
				invalidateRect(&m_pRc, false);
		}
	}
}

int CfmBase::OnDrag(CPoint pt, int& result)
{
	result = RC_NOP;
	if (IsPointInRect(pt))
		result = RC_IN;
	return -1;
}

int CfmBase::OnKey(int key, int& result)
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

void CfmBase::SetVisible(bool visible, int col)
{
	bool	isvisible = (m_form->properties & PR_VISIBLE) ? true : false;

	if (visible)
		m_form->properties |= PR_VISIBLE;
	else
		m_form->properties &= ~PR_VISIBLE;

	if (isvisible != visible)
		invalidateRect(m_pRc, true, true);
}

bool CfmBase::IsVisible(int col)
{
	return ((m_form->properties & PR_VISIBLE) ? true : false);
}

void CfmBase::SetEnable(bool enable)
{
	bool	isenable = (m_form->properties & PR_ENABLE) ? true : false;

	if (enable)
		m_form->properties |= PR_ENABLE;
	else
		m_form->properties &= ~PR_ENABLE;

	if (isenable != enable)
		invalidateRect(&m_pRc, false);
}

bool CfmBase::IsEnable()
{
	return ((m_form->properties & PR_ENABLE) ? true : false);
}

void CfmBase::SetAttributes(DWORD attr, bool set, int col, int row)
{
	if (attr)
	{
		if (set)
			m_form->attr |= attr;
		else
			m_form->attr &= ~attr;
	}
}

void CfmBase::SetFgColor(int rgb, int col, int row)
{
	DWORD	color;

	if (rgb < 0)
	{
		if (m_itRGB < 0)
			return;
		rgb = m_itRGB;
	}

	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (m_tRGB == color && ((rgb & userCOLOR) || (!(rgb & userCOLOR) && m_itRGB == rgb)))
		return;

	m_tRGB  = color;
	m_itRGB = (rgb & userCOLOR) ? -1 : rgb;
	invalidateRect(&m_pRc, false);
}

void CfmBase::SetBkColor(int rgb, int col, int row)
{
	if ((m_form->attr & FA_CONTRAST) && (m_form->attr2 & SA_CONTRAST) == SA_PAINT)
		return;

	DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (m_pRGB == color)
		return;

	m_pRGB = color;
	invalidateRect(&m_pRc, false);
}

int CfmBase::GetFgColor(int col, int row)
{
	return m_tRGB;
}

int CfmBase::GetBkColor(int col, int row)
{
	return m_pRGB;
}

void CfmBase::ReadAll(CString& data, bool edit, bool head)
{
	ReadData(data, edit);
}

void CfmBase::ReadData(CString& data, bool edit, int col, int row)
{
	data = m_strR;
}

void CfmBase::WriteAll(CString str, bool redraw)
{
	WriteData(str, redraw);
}

void CfmBase::WriteData(CString data, bool redraw, int col, int row)
{
	if (!m_strR.Compare(data))
		return;

	m_strR = data;
	if (redraw && (m_form->properties & PR_VISIBLE))
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

void CfmBase::Refresh()
{
	if (m_form->properties & PR_VISIBLE)
	{
		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_pRc, true);
		else
			invalidateRect(&m_pRc, false);
	}
}

CDC* CfmBase::getDC()
{
	CDC* dc = m_axform->m_view->GetDC();
	m_ndc = dc->SaveDC();
	return dc;
}

void CfmBase::releaseDC(CDC* dc)
{
	dc->RestoreDC(m_ndc);
	m_axform->m_view->ReleaseDC(dc);
}

COLORREF CfmBase::getRGB(int index)
{
	return m_axform->m_palette->GetPaletteRGB(index);
}

int CfmBase::getINDEX(COLORREF rgb)
{
	return m_axform->m_palette->GetIndexRGB(rgb);
}

CRect CfmBase::calcRect(CRect rect, float hR, float vR)
{
	CRect	tRc;

	tRc.left   = (int)(rect.left   * hR);
	tRc.right  = (int)(rect.right  * hR);
	tRc.top    = (int)(rect.top    * vR);
	tRc.bottom = (int)(rect.bottom * vR);

	return tRc;
}

void CfmBase::setFontSize(float hR, float vR)
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
	setFont(dc);
	dc->GetTextMetrics(&metric);
	m_fHeight = (BYTE)abs(metric.tmHeight);
	m_fWidth  = (BYTE)abs(metric.tmAveCharWidth);
	releaseDC(dc);
}

CFont* CfmBase::setFont(CDC* dc, int state)
{
	int	weight = FW_NORMAL;
	bool	italic = false;

	if (m_form->fStyle == FS_ITALIC || m_form->fStyle == FS_IBOLD)
		italic = true;

	switch (state)
	{
	case -1:	// deselected TAB
		weight = FW_NORMAL;	break;

	case 1:		// selected TAB
		weight = FW_BOLD;	break;

	case 0:		// normal FORM
	default:
		if (m_form->fStyle == FS_BOLD || m_form->fStyle == FS_IBOLD)
			weight = FW_BOLD;
		if (weight == FW_BOLD)
		{
			if (m_cfPoint < m_fPoint)
				weight = FW_MEDIUM;
		}

		if (GetSystemMetrics(SM_CXSCREEN) < 1024)
			weight = FW_MEDIUM;
		break;
	}

	return m_axform->m_draw->SetFont(dc, (char *)m_fonts.operator LPCTSTR(), m_cfPoint, italic, weight);
}

void CfmBase::setTextColor(CDC* dc, COLORREF rgb)
{
	dc->SetTextColor(dc->IsPrinting() ? getRGB(cBLACK) : rgb);
}

void CfmBase::setTextColor(CDC* dc)
{
	dc->SetTextColor(dc->IsPrinting() ? getRGB(cBLACK) : m_tRGB);
}

void CfmBase::setPaintColor(CDC* dc, COLORREF rgb)
{
	if (dc->IsPrinting())
	{
		dc->SetBkMode(TRANSPARENT);
		return;
	}

	if (m_form->properties & PR_TRANSPARENT)
		dc->SetBkMode(TRANSPARENT);
	else
	{
		dc->SetBkMode(OPAQUE);
		dc->SetBkColor(rgb);
	}
}

void CfmBase::setPaintColor(CDC* dc)
{
	if (dc->IsPrinting())
	{
		dc->SetBkMode(TRANSPARENT);
		return;
	}

	if (m_form->properties & PR_TRANSPARENT)
		dc->SetBkMode(TRANSPARENT);
	else
	{
		dc->SetBkMode(OPAQUE);
		dc->SetBkColor(m_pRGB);
	}
}

void CfmBase::setPen(CDC* dc, COLORREF rgb, int width, int style)
{
	m_axform->m_draw->SetPen(dc, rgb, width, style);
}

void  CfmBase::setPen(CDC* dc, int width, int style)
{
	if (m_form->properties & PR_TRANSPARENT)
	{
		dc->SelectStockObject(NULL_PEN);
		return;
	}
	m_axform->m_draw->SetPen(dc, m_pRGB, width, style);
}

CBrush* CfmBase::setBrush(CDC* dc, COLORREF rgb)
{
	if (dc->IsPrinting())
		return m_axform->m_draw->SetBrush(dc, getRGB(cWHITE));

	return m_axform->m_draw->SetBrush(dc, rgb);
}

CBrush* CfmBase::setBrush(CDC* dc)
{
	if (dc->IsPrinting())
		return m_axform->m_draw->SetBrush(dc, getRGB(cWHITE));

	return m_axform->m_draw->SetBrush(dc, m_pRGB);
}

void CfmBase::setBkColor(int rgb)
{
	DWORD	color = (rgb & userCOLOR) ? rgb : getRGB(rgb);
	if (m_pRGB == color)
		return;

	m_pRGB = color;
	invalidateRect(&m_pRc, false);
}

void CfmBase::invalidateRect(LPCRECT lpRect, bool erase, bool force)
{
	if (!force && !(m_form->properties & PR_VISIBLE))
		return;
	
	CRect	tRc;
	BOOL	Erase = erase ? TRUE : FALSE;
	
	tRc = lpRect;
	tRc.OffsetRect(m_size);
	m_axform->m_view->InvalidateRect(tRc, Erase);
}

void CfmBase::setCaretPos(CPoint pt)
{
	CPoint	point = pt;
	point.Offset(m_size);
	m_axform->m_view->SetCaretPos(point);
}

void CfmBase::doEditCORGB(CString& data)
{
	if (m_form->onEdit == NOVALUE)
		return;

	int	rgb = getCORGB(m_form->onEdit, data, m_form->size, m_form->attr2 & SA_CORGB);
	rgb = (rgb < 0) ? m_form->tRGB : rgb;
	SetFgColor(rgb);
}

int CfmBase::getCORGB(DWORD coRGB, CString& data, int flen, int supress)
{
	int	value = 0;
	int	index;
	DWORD	upRGB, dnRGB;

	if (coRGB == NOVALUE)
		return -1;

	flen = min(data.GetLength(), flen);
	if (flen <= 0)
 		return	-1;

	upRGB = m_axform->m_valueR[coRGB];
	dnRGB = m_axform->m_valueR[coRGB+1];
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

void CfmBase::makeData(CString& data, int flen, DWORD attr, DWORD attr2, int type)
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

		if ((attr & FA_RIGHT) && (type == IO_NUM || type == IO_DIGIT))
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

void CfmBase::makeSignData(CString& data, int col, int row)
{
	if (!(m_form->attr & FA_CONTRAST) || data.IsEmpty())
		return;

	CString	tmps;
	int	state;

	state = m_axform->GetContrast(data[0], tmps);
	tmps += CString(' ', m_form->size-data.GetLength());
	tmps += data.Mid(1);
	data = tmps;

	int	rgb;
	DWORD	upRGB, dnRGB;
	if (m_form->onEdit != NOVALUE)
	{
		upRGB = m_axform->m_valueR[m_form->onEdit];
		dnRGB = m_axform->m_valueR[m_form->onEdit+1];
	}
	switch (state)
	{
	case ct_uLimit:
	case ct_Rise:
		rgb = (m_form->onEdit == NOVALUE) ? cRGB_UP : upRGB;
		break;
	case ct_lLimit:
	case ct_Fall:
		rgb = (m_form->onEdit == NOVALUE) ? cRGB_DN : dnRGB;
		break;
	case ct_Flat:
	default:
		rgb = m_form->tRGB;	break;
	}
	rgb = (rgb < 0) ? m_form->tRGB : rgb;

	if ((m_form->attr2 & SA_CORGB) == SA_COSUP)
		data = data.Mid(2);

	if ((m_form->attr2 & SA_CONTRAST) == SA_PAINT)
	{
		if (state == ct_uLimit || state == ct_lLimit)
		{
			setBkColor(rgb);
			SetFgColor(getRGB(cWHITE));
		}
		else
		{
			setBkColor(m_form->pRGB);
			SetFgColor(rgb);
		}
	}
	else	SetFgColor(rgb);
}

bool CfmBase::checkKey(int& key, BYTE type, DWORD attr)
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

COLORREF CfmBase::getReverseColor(COLORREF rgb)
{
	BYTE	rr, gg, bb;

	rr = 0xff - GetRValue(rgb);
	gg = 0xff - GetGValue(rgb);
	bb = 0xff - GetBValue(rgb);

	return PALETTERGB(rr, gg, bb);
}

wch_t CfmBase::whichHangulEdit(int pos, CString str)
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

int CfmBase::getSIGN(CString str, int& offs)
{
	switch (m_axform->m_LCtype)
	{
	case LANG_KOREAN:
		offs = str.Find(LCcontrast[ct_uLimit]);
		if (offs != -1)
			return ct_uLimit;

		offs = str.Find(LCcontrast[ct_lLimit]);
		if (offs != -1)
			return ct_lLimit;

		offs = str.Find(LCcontrast[ct_Rise]);
		if (offs != -1)
			return  ct_Rise;

		offs = str.Find(LCcontrast[ct_Fall]);
		if (offs != -1)
			return ct_Fall;
		break;

	case LANG_CHINESE:
		offs = str.Find(LCcontrast[ct_uLimit]);
		if (offs != -1)
			return ct_Rise;

		offs = str.Find(LCcontrast[ct_lLimit]);
		if (offs != -1)
			return ct_Fall;

		offs = str.Find(LCcontrast[ct_Rise]);
		if (offs != -1)
			return  ct_Rise;

		offs = str.Find(LCcontrast[ct_Fall]);
		if (offs != -1)
			return ct_Fall;
		break;
	}
	return ct_Flat;
}

void CfmBase::drawPolygonText(CDC* dc, CRect rect, CString str, BYTE align, BYTE type)
{
	CRect	tRc, pRc, lRc;
	CSize	size;
	int	sign;
	int	offs = 0;
	CString	tmps;
	UINT	format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;

	switch (align)
	{
	case AL_RIGHT:
		format |= DT_RIGHT;	break;
	case AL_CENTER:
		format |= DT_CENTER;	break;
	case AL_LEFT:
	default:
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
		CPoint	pts[7];
		float	hh = m_fHeight+2;

		tmps = (offs > 0) ? str.Left(offs) : _T("");
		str  = (str.GetLength() > offs+2) ? str.Mid(offs+2) : _T("");
		switch (align)
		{
		case AL_LEFT:
			if (!tmps.IsEmpty())
			{
				size = dc->GetOutputTextExtent(tmps);
				lRc.right = lRc.left + size.cx;
				pRc.left  = lRc.right;
			}
			size = dc->GetOutputTextExtent(LCcontrast[ct_uLimit]);
			pRc.right = pRc.left + size.cx;
			tRc.left = pRc.right;
			break;

		case AL_RIGHT:
			size = dc->GetOutputTextExtent(str);
			tRc.left = tRc.right - size.cx;
			size = dc->GetOutputTextExtent(LCcontrast[ct_uLimit]);
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

		size = dc->GetOutputTextExtent(LCcontrast[ct_uLimit]);
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

		if (pRc.left >= rect.left && pRc.right <= rect.right)
		{
			dc->SetPolyFillMode(ALTERNATE);
			switch (sign)
			{
			case ct_uLimit:
				pts[1].y = pRc.top + (int)hh;
				pts[2].y = pRc.top + (int)hh;
				pts[5].y = pRc.top + (int)hh;
				pts[6].y = pRc.top + (int)hh;
				pts[0].y = pRc.top;
				pts[3].y = pRc.bottom;
				pts[4].y = pRc.bottom;

				pts[0].x = pRc.left + (pRc.Width()/2);
				pts[1].x = pRc.left;
				pts[6].x = pRc.right;
				pts[2].x = pRc.left + ww;
				pts[3].x = pRc.left + ww;
				pts[4].x = pRc.right - ww;
				pts[5].x = pRc.right - ww;
				dc->Polygon(pts, 7);
				break;

			case ct_lLimit:
				pts[1].y = pRc.top + (int)hh;
				pts[2].y = pRc.top + (int)hh;
				pts[5].y = pRc.top + (int)hh;
				pts[6].y = pRc.top + (int)hh;
				pts[0].y = pRc.bottom;
				pts[3].y = pRc.top;
				pts[4].y = pRc.top;

				pts[0].x = pRc.left + (pRc.Width()/2);
				pts[1].x = pRc.right;
				pts[6].x = pRc.left;
				pts[2].x = pRc.right - ww;
				pts[3].x = pRc.right - ww;
				pts[4].x = pRc.left + ww;
				pts[5].x = pRc.left + ww;
				dc->Polygon(pts, 7);
				break;

			case ct_Rise:
				pRc.top += 2;
				pRc.bottom -= 2;
				if (!(pRc.Height() % 2))
					pRc.top++;
				pts[0].y = pRc.top;
				pts[1].y = pRc.bottom;
				pts[2].y = pRc.bottom;

				pts[0].x = pRc.left + (pRc.Width()/2);
				pts[1].x = pRc.left;
				pts[2].x = pRc.right;
				dc->Polygon(pts, 3);
				break;

			case ct_Fall:
				pRc.top += 2;
				pRc.bottom -= 2;
				if (!(pRc.Height() % 2))
					pRc.top++;
				pts[0].y = pRc.bottom;
				pts[1].y = pRc.top;
				pts[2].y = pRc.top;

				pts[0].x = pRc.left + (pRc.Width()/2);
				pts[1].x = pRc.right;
				pts[2].x = pRc.left;
				dc->Polygon(pts, 3);
				break;
			}
		}
	}
	tRc.left   = max(tRc.left, rect.left);
	tRc.right  = min(tRc.right, rect.right);
	dc->DrawText(str, tRc, format);
}

void CfmBase::drawEdge(CDC* dc, CRect rect, UINT edge, UINT flags)
{
	CRect	tRc;
	CPoint	pts[6];
	CBrush*	brush;
	int	gap = 1;

	switch (edge)
	{
	case EDGE_BUMP:		// raised_outer | sunken_inner
		tRc = rect;
		tRc.DeflateRect(1, 1, 0, 0);
		setPen(dc, getRGB(cEDGE));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(&tRc);

		tRc = rect;
		tRc.DeflateRect(0, 0, 1, 1);
		setPen(dc, getRGB(cYWHITE));
		dc->SelectStockObject(NULL_BRUSH);
		dc->Rectangle(&tRc);
		break;

	case EDGE_ETCHED:	// sunken_outer | raised_inner
		dc->DrawEdge(&rect, edge, BF_RECT);
		break;

	case EDGE_RAISED:	// raised_outer | raised_inner
		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = rect.left;		pts[1].y = rect.top;
		pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
		pts[3].x = rect.left;		pts[3].y = pts[2].y;
		pts[4].x = pts[3].x;		pts[4].y = rect.top;
		pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
		setPen(dc, getRGB(cYWHITE));
		brush = setBrush(dc, getRGB(cYWHITE));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
		pts[2].x = rect.left;		pts[2].y = pts[1].y;
		pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
		pts[4].x = rect.right-gap;	pts[4].y = rect.bottom-gap;
		pts[5].x = rect.right-gap;	pts[5].y = rect.top;
		setPen(dc, getRGB(cEDGE));
		brush = setBrush(dc, getRGB(cEDGE));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
		break;

	case EDGE_SUNKEN:	// sunken_outer | sunken_inner
		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = rect.left;		pts[1].y = rect.top;
		pts[2].x = rect.left;		pts[2].y = rect.bottom-gap;
		pts[3].x = rect.left;		pts[3].y = pts[2].y;
		pts[4].x = pts[3].x;		pts[4].y = rect.top;
		pts[5].x = pts[0].x;		pts[5].y = pts[4].y;
		setPen(dc, getRGB(cEDGE));
		brush = setBrush(dc, getRGB(cEDGE));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);

		pts[0].x = rect.right-gap;	pts[0].y = rect.top;
		pts[1].x = pts[0].x;		pts[1].y = rect.bottom-gap;
		pts[2].x = rect.left;		pts[2].y = pts[1].y;
		pts[3].x = rect.left;		pts[3].y = rect.bottom-gap;
		pts[4].x = rect.right-gap;	pts[4].y = rect.bottom-gap;
		pts[5].x = rect.right-gap;	pts[5].y = rect.top;
		setPen(dc, getRGB(cYWHITE));
		brush = setBrush(dc, getRGB(cYWHITE));
		dc->SetPolyFillMode(ALTERNATE);
		dc->Polygon(pts, 6);
		break;
	}
}

CString CfmBase::convertVString(CString src)
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

void CfmBase::formatData(int attr, CString& data, int nfloat)
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
		if (attr & FA_ENUM && !atoi(floats))
			return;

	}
	else if (nfloat == -1 || floats.IsEmpty())
		return;

	data += '.';
	data += floats;
}
