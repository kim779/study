// fmButton.cpp : implementation file
//

#include "stdafx.h"
#include "amButton.h"
#include "amform.h"
#include "image.h"
#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAmButton

IMPLEMENT_DYNCREATE(CAmButton, CamBase)

CAmButton::CAmButton()
{
	EnableAutomation();
}

CAmButton::CAmButton(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_mousedown = false;
	m_image     = PI_DEFAULT;
	m_pic = NULL;
	CString sImage;
	sImage = CString(m_form->str2);

	
	if (!sImage.IsEmpty())
	{
		m_form->properties |= (PR_IMAGE|PR_IMAGETEXT) ;
	}

	m_iRc = m_pRc;
	m_tRc = m_pRc;


	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && !sImage.IsEmpty()/*&& m_form->vals[1] != NOVALUE*/)
	{
		CString	path;

		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, sImage);
		m_pic = new Cimage(m_axform->m_pAmDraw, path);
//		if (m_form->properties & PR_IMAGETEXT)
//			m_form->properties |= PR_TRANSPARENT;
	}
}

CAmButton::~CAmButton()
{
	if (m_pic)
		delete m_pic;
}


void CAmButton::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CAmButton, CamBase)
	//{{AFX_MSG_MAP(CAmButton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CAmButton, CamBase)
	//{{AFX_DISPATCH_MAP(CAmButton)	
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmButton to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {6ED765AC-16A4-4038-A181-7D5D0C00FD3A}
static const IID IID_IfmButton =
{ 0x6ed765ac, 0x16a4, 0x4038, { 0xa1, 0x81, 0x7d, 0x5d, 0xc, 0x0, 0xfd, 0x3a } };

BEGIN_INTERFACE_MAP(CAmButton, CamBase)
	INTERFACE_PART(CAmButton, IID_IfmButton, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAmButton message handlers

void CAmButton::_SetFocus() 
{
	if (m_form->properties & PR_CHECKED)
		m_axform->DoSomething(doFOCUS, this);
}

void CAmButton::_Refresh() 
{
	if (m_form->properties & PR_VISIBLE)
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_iRc, true);
//		else
//			invalidateRect(&m_iRc, false);
	}
}


LPCTSTR CAmButton::GetText() 
{
	CString strResult;

	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CAmButton::SetText(LPCTSTR lpszNewValue)
{
	WriteData(lpszNewValue);
}

long CAmButton::GetPRgb() 
{
	return (long)m_pRGB;
}

void CAmButton::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CAmButton::GetTRgb() 
{
	return (long)m_tRGB;
}

void CAmButton::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

BOOL CAmButton::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CAmButton::SetVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

BOOL CAmButton::GetEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CAmButton::SetEnable(BOOL bNewValue) 
{
	CamBase::SetEnable(bNewValue ? true : false);
}

void CAmButton::SetFont(int point, int style, CString fonts, bool resize)
{
	if (!(m_form->properties & (PR_IMAGE|PR_IMAGETEXT)))
		CamBase::SetFont(point, style, fonts, resize);
}

void CAmButton::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());
	
	CRect	cRc;
	CString	str = _T("");
	bool	border = false;
	StringFormat sf;

#ifdef _GDI
	UINT	format;
	int	sMode;
	int	ndc = dc->SaveDC();

	CFont*	oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
#endif
	
	g.FillRectangle(setgBrush(), RECTG(m_pRc));

	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && m_pic && m_pic->GetValid(m_image))
	{
		border = false;
		m_pic->DrawPicture(g, m_pRc, m_form->alignment, m_image);
		if (!(m_form->properties & PR_HOVER) && m_mousedown)
			border = true;
	}
	else
		border = true;

	if (!(m_form->properties & PR_IMAGE) || m_form->properties & PR_IMAGETEXT)
	{
		str = m_strR;
		cRc = m_pRc;

#ifdef _GDI
		format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		if (m_form->properties & PR_IMAGETEXT)
			format |= DT_CENTER;
		else
		{
			switch (m_form->alignment)
			{
			case AL_RIGHT:
				format |= DT_RIGHT;	break;
			case AL_LEFT:
				format |= DT_LEFT;	break;
			case AL_CENTER:
			default :
				format |= DT_CENTER;	break;
			}
		}

		sMode = dc->SetBkMode(TRANSPARENT);
		if (m_form->properties & PR_ENABLE)
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
			setTextColor(dc, getRGB(cXFG));
			dc->DrawText(str, cRc, format);
		}
#else

//		setPaintColor(dc);
		sf.SetLineAlignment(StringAlignmentCenter);
		sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap|StringFormatFlagsNoFitBlackBox);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);

		if (m_form->properties & PR_IMAGETEXT)
			sf.SetAlignment(StringAlignmentCenter);	//break;format |= DT_CENTER;
		else
		{
			sf.SetLineAlignment(StringAlignmentCenter);
			switch (m_form->alignment)
			{
			case AL_RIGHT:
				sf.SetAlignment(StringAlignmentFar);	break;
			case AL_LEFT:
				sf.SetAlignment(StringAlignmentNear);	break;
			case AL_CENTER:
			default :
				sf.SetAlignment(StringAlignmentCenter);	break;
			}
		}


		
/*
		if (m_form->properties & PR_VERT)
		{
			sf.SetLineAlignment(StringAlignmentNear);
			sf.SetAlignment(StringAlignmentCenter);
//			int	gap, len = str.GetLength();
			str = convertVString(str);
			sf.SetFormatFlags(sf.GetFormatFlags()&~StringFormatFlagsNoWrap&~StringFormatFlagsLineLimit);
			
		//	format &= ~DT_SINGLELINE;
		//	gap  = (cRc.Height() - dc->GetOutputTextExtent(str).cy * (str.GetLength() - len)) / 2;
		//	if (gap >= 0)
		//		cRc.OffsetRect(0, gap);
		}
*/
		if (m_form->properties & PR_ENABLE)
		{
			g.DrawString(str.AllocSysString(), -1, setgFont(), RECTF(cRc), &sf, setgBrush(GRGB(m_tRGB)));
		}
		else
		{
			g.DrawString(str.AllocSysString(), -1, setgFont(), RECTF(cRc), &sf, setgBrush(clWHITE));
			cRc.left -= 1;
			cRc.top  -= 1;
			g.DrawString(str.AllocSysString(), -1, setgFont(), RECTF(cRc), &sf, setgBrush(clGRAY));
		}
#endif
	}
	
	if (border)
	{
		if (m_mousedown)
			drawEdge(g, m_pRc, EDGE_SUNKEN, BF_RECT);
		else
			drawEdge(g, m_pRc, EDGE_RAISED, BF_RECT);
	}	

	cRc = m_pRc;
	cRc.DeflateRect(4, 4);
	
	if (m_focus)
	{
		Pen* pPen = setgPen(clBLACK, 1);
		pPen->SetDashStyle(DashStyleDot);
		g.DrawRectangle(pPen, RECTG(cRc));
	}

#ifdef _GDI
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);
#endif

}

void CAmButton::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (down)
		m_axform->m_cursor = pt;
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT)/* && (m_form->properties & PR_HOVER)*/)
		m_image = m_mousedown ? PI_ENTER : PI_DEFAULT;

//	if (m_form->properties & PR_TRANSPARENT)
		invalidateRect(&m_pRc, true);
//	else
//		invalidateRect(&m_pRc, false);
}

int CAmButton::OnDrag(CPoint pt, int& result)
{
	int	rc , state, image = m_image;

	rc = CamBase::OnDrag(pt, result);
	state = result;
	if (m_mousedown || !(m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER)))
		return (-1);

	result = (result == RC_IN) ? RC_HOVER : RC_OUT;

	switch (result)
	{
	case RC_HOVER:
		m_image = PI_ENTER;	break;
	case RC_OUT:
		m_image = PI_DEFAULT;	break;
	default:
		return (-1);
	}

	if (image != m_image)
		invalidateRect(&m_pRc, false);
	return (-1);
}

void CAmButton::WriteData(CString data, bool redraw, int col, int row)
{
	bool	image = false;
	if (m_form->properties & PR_IMAGE)
		image = true;
	else if (m_form->properties & PR_IMAGETEXT)
	{
		data.MakeLower();
		if (data.Find(".bmp") != -1 || data.Find(".gif") != -1 || data.Find(".png") != -1)
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
		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, data);
		m_pic   = new Cimage(m_axform->m_pAmDraw, path);
		m_image = PI_DEFAULT;
		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CamBase::WriteData(data, redraw, col, row);
}

void CAmButton::ReadData(CString& data, bool edit, int col, int row)
{
	CamBase::ReadData(data, edit, col, row);
}

void CAmButton::SetFocus(bool focus)
{
	m_focus = focus;
	if (!m_focus)
		m_mousedown = false;
	invalidateRect(&m_pRc, false);
}
