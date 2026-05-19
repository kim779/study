// fmLabel.cpp : implementation file
//

#include "stdafx.h"
#include "amLabel.h"
#include "amform.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamLabel

IMPLEMENT_DYNCREATE(CamLabel, CamBase)

CamLabel::CamLabel()
{
	EnableAutomation();
}

CamLabel::CamLabel(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_pic = NULL;
	CString ImgPath = CString(m_form->str2);

	if (!ImgPath.IsEmpty())
		m_form->properties |= (PR_IMAGE|PR_IMAGETEXT);

	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && !ImgPath.IsEmpty())
	{
		CString	path;
		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, ImgPath);
		m_pic = new Cimage(m_axform->m_pAmDraw, path);

		if (m_form->properties & PR_IMAGETEXT)
			m_form->type = PN_NONE;
	}
}

CamLabel::~CamLabel()
{
	if (m_pic)	delete m_pic;
}


void CamLabel::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamLabel, CamBase)
	//{{AFX_MSG_MAP(CamLabel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamLabel, CamBase)
	//{{AFX_DISPATCH_MAP(CamLabel)
	/*
	DISP_PROPERTY_EX(CamLabel, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CamLabel, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CamLabel, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CamLabel, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CamLabel, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamLabel, "Blink", _Blink, VT_EMPTY, VTS_BOOL VTS_I4)
	DISP_FUNCTION(CamLabel, "Push", _Push, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CamLabel, "Pop", _Pop, VT_EMPTY, VTS_BSTR)
	*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmLabel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {4846F8F7-758E-4CF7-870F-D1AE218F75B1}
static const IID IID_IfmLabel =
{ 0x4846f8f7, 0x758e, 0x4cf7, { 0x87, 0xf, 0xd1, 0xae, 0x21, 0x8f, 0x75, 0xb1 } };

BEGIN_INTERFACE_MAP(CamLabel, CamBase)
	INTERFACE_PART(CamLabel, IID_IfmLabel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamLabel message handlers

void CamLabel::_Refresh() 
{
	Refresh();
}

void CamLabel::_Blink(BOOL set, long color) 
{
	m_axform->DoSomething(doBLINK, this, set ? 1 : 0, (LPARAM)color);
}

void CamLabel::_Push(LPCTSTR name) 
{
	m_axform->DoSomething(doPUSH, this, (WPARAM)0, (LPARAM)name);
}

void CamLabel::_Pop(LPCTSTR name) 
{
	m_axform->DoSomething(doPOP, this, (WPARAM)0, (LPARAM)name);
}

LPCTSTR CamLabel::GetText() 
{
	CString strResult;

	CamBase::ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamLabel::SetText(LPCTSTR lpszNewValue) 
{
	CamBase::WriteData(lpszNewValue);
}

long CamLabel::GetPRgb() 
{
	return m_pRGB;
}

void CamLabel::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamLabel::GetTRgb() 
{
	return m_tRGB;
}

void CamLabel::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

bool CamLabel::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? true : false;
}

void CamLabel::SetVisible(bool bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

void CamLabel::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());

	CRect	tRc;
	CSize	tSz;
	CString	tmps;

#ifdef _GDI
	UINT	format;
	CFont*	oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
	
	int	ndc = dc->SaveDC();
	int	sMode;	

	setTextColor(dc);
#endif
	
	StringFormat sf;	
	sf.SetLineAlignment(StringAlignmentCenter);
	sf.SetHotkeyPrefix(HotkeyPrefixNone);
	sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);

	tSz = CSize(0, 0);
	tRc = m_pRc;
//	if (!(m_form->properties & PR_TRANSPARENT))

	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_pic /*&& m_pic->GetValid(m_image)*/)
			m_pic->DrawPicture(dc, m_pRc, IA_STRETCH, PI_DEFAULT);
		else //if (!(m_form->properties & PR_TRANSPARENT))
			g.FillRectangle(setgBrush(), RECTG(m_pRc));

	}
	else
		g.FillRectangle(setgBrush(), RECTG(tRc));

	format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
	switch (m_form->alignment)
	{
	case AL_RIGHT:
		format |= DT_RIGHT;
		sf.SetAlignment(StringAlignmentFar);
		break;
	case AL_CENTER:
		format |= DT_CENTER;	break;
		sf.SetAlignment(StringAlignmentCenter);
		break;
	case AL_LEFT:
	default:
		format |= DT_LEFT;	break;
		sf.SetAlignment(StringAlignmentNear);
		break;
	}

	tmps = m_strR;
	DWORD edgeStyle;

		switch (m_form->borders)
		{
		case BD_RAISED:
			edgeStyle = EDGE_RAISED;	break;
		case BD_SUNKEN:
			edgeStyle = EDGE_SUNKEN;	break;
		case BD_BUMP:
			edgeStyle = EDGE_BUMP;		break;
		case BD_ETCHED:
			edgeStyle = EDGE_ETCHED;	break;
		default:
			edgeStyle = 0;
			break;

		}
	//	drawEdge(dc, m_pRc, edgeStyle, BF_RECT);
//		break;

//	}
	
//	g.DrawRectangle(setgPen(clWHITE), RECTG(m_pRc));

	if (m_form->borders == BD_LINE)
		g.DrawRectangle(setgPen(m_bRGB), RECTG(m_pRc));
	else
		drawEdge(dc, m_pRc, edgeStyle, BF_RECT);
	/*
	if (m_form->properties & PR_VERT)
	{
		int	gap, len = tmps.GetLength();

		tmps = convertVString(tmps);
		sf.SetLineAlignment(StringAlignmentNear);
		sf.SetFormatFlags(sf.GetFormatFlags()&~StringFormatFlagsLineLimit&~StringFormatFlagsNoWrap);
		gap  = (tRc.Height() - dc->GetOutputTextExtent(tmps).cy * (tmps.GetLength() - len)) / 2;
		if (gap >= 0)
			tRc.OffsetRect(0, gap);
	}
	else
	*/
	{
		if (tmps.Find("\\n") != -1)
			tmps.Replace("\\n", "\n");
	
		if (tmps.Find('\n') != -1)
		{
			format &= ~DT_SINGLELINE;
			format |= DT_EXTERNALLEADING;
			sf.SetFormatFlags(sf.GetFormatFlags()&~StringFormatFlagsLineLimit&~StringFormatFlagsNoWrap);
		}
	}
	//g.DrawString(tmps.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, setgFBrush());

#ifdef _GDI
	sMode = dc->SetBkMode(TRANSPARENT);
	dc->DrawText(tmps, tRc, format);
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);	
#endif
}
