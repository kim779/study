// fmPanel.cpp : implementation file
//

#include "stdafx.h"
#include "amPanel.h"
#include "amform.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamPanel

IMPLEMENT_DYNCREATE(CamPanel, CamBase)

CamPanel::CamPanel()
{
	EnableAutomation();
}

CamPanel::CamPanel(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_image = PI_DEFAULT;
	m_pic   = NULL;
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

CamPanel::~CamPanel()
{
	if (m_pic)	delete m_pic;
}


void CamPanel::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamPanel, CamBase)
	//{{AFX_MSG_MAP(CamPanel)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamPanel, CamBase)
	//{{AFX_DISPATCH_MAP(CamPanel)
	/*
	DISP_PROPERTY_EX(CamPanel, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CamPanel, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CamPanel, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CamPanel, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_FUNCTION(CamPanel, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)
	*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmPanel to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {F7303843-6D61-43A1-BEB7-C03A141BC84B}
static const IID IID_IfmPanel =
{ 0xf7303843, 0x6d61, 0x43a1, { 0xbe, 0xb7, 0xc0, 0x3a, 0x14, 0x1b, 0xc8, 0x4b } };

BEGIN_INTERFACE_MAP(CamPanel, CamBase)
	INTERFACE_PART(CamPanel, IID_IfmPanel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamPanel message handlers

void CamPanel::_Refresh() 
{
	Refresh();
}

LPCTSTR CamPanel::GetText() 
{
	CString strResult;

	ReadData(strResult);
	return (LPCTSTR)strResult;
}

void CamPanel::SetText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CamPanel::GetPRgb() 
{
	return m_pRGB;
}

void CamPanel::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamPanel::GetTRgb() 
{
	return m_tRGB;
}

void CamPanel::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

BOOL CamPanel::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamPanel::SetVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

void CamPanel::Draw(CDC* dc)
{
 	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());

#ifdef _DEBUG
	Bitmap *pbitmap = ::new Bitmap(m_pRc.Width(), m_pRc.Height());
#else
	Bitmap *pbitmap = new Bitmap(m_pRc.Width(), m_pRc.Height());
#endif

	Graphics mg(pbitmap);
	Matrix mx(1, 0, 0, 1,(REAL)-m_pRc.left, (REAL)-m_pRc.top);
	mg.SetTransform(&mx);


	UINT	edgeStyle;
	UINT	format;
	CBrush*	brush;
	CRect	tRc;
	CString	tmps;
	StringFormat sf;
//	int	ndc = dc->SaveDC();

	setPen(dc);

	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_pic && m_pic->GetValid(m_image))
			m_pic->DrawPicture(dc, m_pRc, m_form->alignment, m_image);
		else //if (!(m_form->properties & PR_TRANSPARENT))
			g.FillRectangle(setgBrush(), RECTG(m_pRc));

	}

	if (!(m_form->properties & PR_IMAGE) || m_form->properties & PR_IMAGETEXT)
	{
		setFont(dc);
		setTextColor(dc);
		brush = setBrush(dc);

		if (!(m_form->properties & (PR_IMAGETEXT/*|PR_TRANSPARENT*/)))
		{
			g.FillRectangle(setgBrush(), RECTG(m_pRc));
		}


		sf.SetLineAlignment(StringAlignmentCenter);
		sf.SetFormatFlags(StringFormatFlagsLineLimit|StringFormatFlagsNoWrap);
		sf.SetHotkeyPrefix(HotkeyPrefixNone);
		format = DT_SINGLELINE|DT_VCENTER|DT_NOPREFIX;
		switch (m_form->alignment)
		{
		case AL_RIGHT:
			sf.SetAlignment(StringAlignmentFar);
			format |= DT_RIGHT;	break;
		case AL_LEFT:
			sf.SetAlignment(StringAlignmentNear);
			format |= DT_LEFT;	break;
		case AL_CENTER:
		default :
			sf.SetAlignment(StringAlignmentCenter);
			format |= DT_CENTER;	break;
		}

		tRc  = m_pRc;
		tmps = m_strR;
		/*
		if (m_form->properties & PR_VERT)
		{
			sf.SetLineAlignment(StringAlignmentCenter);
			sf.SetAlignment(StringAlignmentCenter);
			int	len = tmps.GetLength();

			tmps = convertVString(tmps);
			format &= ~DT_SINGLELINE;
				
			sf.SetFormatFlags(sf.GetFormatFlags()&~StringFormatFlagsNoWrap&~StringFormatFlagsLineLimit);	
		//	gap  = (tRc.Height() - dc->GetOutputTextExtent(tmps).cy * (tmps.GetLength() - len)) / 2;
		//	if (gap >= 0)
			//	tRc.OffsetRect(0, gap);
		}
		else
		*/
			tRc.DeflateRect(3, 0);
		
		//dc->DrawText(tmps, tRc, format);
		g.DrawString(tmps.AllocSysString(), -1, setgFont(), RECTF(tRc), &sf, setgFBrush()); 
	}

//	switch (m_form->type)
//	{
//	case PN_BORDER:
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
			//edgeStyle = EDGE_RAISED;
			edgeStyle = 0;
			break;
		case BD_LINE:
//		setPen(dc, m_bRGB);
//		dc->SelectStockObject(NULL_BRUSH);
//		dc->Rectangle(m_pRc);
			g.DrawRectangle(setgPen(m_bRGB), RECTG(m_pRc));
			return;
		break;


		}
	//	drawEdge(dc, m_pRc, edgeStyle, BF_RECT);
//		break;

//	}
	
//	g.DrawRectangle(setgPen(clWHITE), RECTG(m_pRc));
	if (edgeStyle != 0)
		drawEdge(dc, m_pRc, edgeStyle, BF_RECT);

//	CachedBitmap cache(pbitmap, &g);
//	g.DrawCachedBitmap(&cache, m_pRc.left, m_pRc.top);
/*	
#ifdef _DEBUG	
	::delete pbitmap;
#else
	delete pbitmap;
#endif
//	dc->RestoreDC(ndc);
*/
}

void CamPanel::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	if (down)
		m_axform->m_cursor = pt;
	if (!(m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER)))
		return;

	m_image = down ? PI_DOWN : PI_DEFAULT;
//	if (!(m_form->properties & PR_TRANSPARENT))
		invalidateRect(&m_pRc, false);
}

int CamPanel::OnDrag(CPoint pt, int& result)
{
	int	rc, state, image = m_image;

	rc = CamBase::OnDrag(pt, result);
	state = result;
	if (!(m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && (m_form->properties & PR_HOVER)))
		return -1;

	result = (rc == RC_IN) ? RC_HOVER : RC_OUT;

	switch (result)
	{
	case RC_HOVER:
		m_image = PI_ENTER;	break;
	case RC_OUT:
		m_image = PI_DEFAULT;	break;
	default:
		return -1;
	}

	if (/*!(m_form->properties & PR_TRANSPARENT) && */image != m_image)
		invalidateRect(&m_pRc, false);
	return -1;
}

void CamPanel::WriteData(CString data, bool redraw, int col, int row)
{
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT))
	{
		if (m_pic)
		{
			delete m_pic;
			m_pic = NULL;
		}

		CString	path;
		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, data);
		m_pic = new Cimage(m_axform->m_pAmDraw, path);
		if (redraw)
			invalidateRect(&m_pRc, false);
	}
	else
		CamBase::WriteData(data, redraw, col, row);
}

void CamPanel::ReadData(CString& data, bool edit, int col, int row)
{
	CamBase::ReadData(data, edit, col, row);
}
