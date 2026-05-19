// fmRadio.cpp : implementation file
//

#include "stdafx.h"
#include "amRadio.h"
#include "amform.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CamRadio

IMPLEMENT_DYNCREATE(CamRadio, CamBase)

CamRadio::CamRadio()
{
	EnableAutomation();
}

CamRadio::CamRadio(CAmForm* axform, struct _formR* formR)
	: CamBase(axform, formR)
{
	m_mousedown = false;
	m_iRc = m_pRc;
	m_tRc = m_pRc;
	
	if (m_form->properties & PR_CHECKED)
		m_image	= PI_DOWN;
	else	
		m_image	= PI_DEFAULT;
	
	m_pic = NULL;
	CString sImage;
	sImage = CString(m_form->str2);

	if (!sImage.IsEmpty())
		m_form->properties |= (PR_IMAGE|PR_IMAGETEXT) ;
	if (m_form->properties & (PR_IMAGE|PR_IMAGETEXT) && !sImage.IsEmpty()/*&& m_form->vals[1] != NOVALUE*/)
	{
		CString	path;

		path.Format("%s\\%s\\%s", m_axform->m_root, IMAGEDIR, sImage);
		m_pic = new Cimage(m_axform->m_pAmDraw, path);
	}

	m_name = CString(m_form->name);	
}

CamRadio::~CamRadio()
{
	SAFE_DELETE(m_pic);
}


void CamRadio::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamRadio, CamBase)
	//{{AFX_MSG_MAP(CamRadio)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamRadio, CamBase)
	//{{AFX_DISPATCH_MAP(CamRadio)
	/*DISP_PROPERTY_EX(CamRadio, "Text", _getText, _setText, VT_BSTR)
	DISP_PROPERTY_EX(CamRadio, "PRgb", _getPRgb, _setPRgb, VT_I4)
	DISP_PROPERTY_EX(CamRadio, "TRgb", _getTRgb, _setTRgb, VT_I4)
	DISP_PROPERTY_EX(CamRadio, "Visible", _getVisible, _setVisible, VT_BOOL)
	DISP_PROPERTY_EX(CamRadio, "Enable", _getEnable, _setEnable, VT_BOOL)
	DISP_PROPERTY_EX(CamRadio, "Checked", _getChecked, _setChecked, VT_BOOL)
	DISP_FUNCTION(CamRadio, "SetFocus", _SetFocus, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CamRadio, "Refresh", _Refresh, VT_EMPTY, VTS_NONE)*/
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmRadio to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {233923D8-D9A5-4F28-AFE2-ABB4819603BD}
static const IID IID_IfmRadio =
{ 0x233923d8, 0xd9a5, 0x4f28, { 0xaf, 0xe2, 0xab, 0xb4, 0x81, 0x96, 0x3, 0xbd } };

BEGIN_INTERFACE_MAP(CamRadio, CamBase)
	INTERFACE_PART(CamRadio, IID_IfmRadio, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamRadio message handlers

void CamRadio::_SetFocus() 
{
	m_axform->DoSomething(doFOCUS, this);
}

void CamRadio::_Refresh() 
{
	if (m_form->properties & PR_VISIBLE)
	{
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_iRc, true);
//		else
//			invalidateRect(&m_iRc, false);
	}
}

CString CamRadio::GetText() 
{
	CString strResult;

	ReadData(strResult);
	return strResult.AllocSysString();
}

void CamRadio::SetText(LPCTSTR lpszNewValue) 
{
	WriteData(lpszNewValue);
}

long CamRadio::GetPRgb() 
{
	return m_pRGB;
}

void CamRadio::SetPRgb(long nNewValue) 
{
	CamBase::SetBkColor(nNewValue);
}

long CamRadio::GetTRgb() 
{
	return m_tRGB;
}

void CamRadio::SetTRgb(long nNewValue) 
{
	CamBase::SetFgColor(nNewValue);
}

BOOL CamRadio::GetVisible() 
{
	return (m_form->properties & PR_VISIBLE) ? TRUE : FALSE;
}

void CamRadio::SetVisible(BOOL bNewValue) 
{
	CamBase::SetVisible(bNewValue ? true : false);
}

BOOL CamRadio::GetEnable() 
{
	return (m_form->properties & PR_ENABLE) ? TRUE : FALSE;
}

void CamRadio::SetEnable(BOOL bNewValue) 
{
	CamBase::SetEnable(bNewValue ? true : false);
}

BOOL CamRadio::_GetChecked() 
{
	return (m_form->properties & PR_CHECKED) ? TRUE : FALSE;
}

void CamRadio::_SetChecked(BOOL bNewValue) 
{
	SetChecked(bNewValue ? true : false);
}

void CamRadio::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	CRect rc;
	Graphics g(dc->GetSafeHdc());
	UINT	style;
	CRect	cRc1, cRc2;
	style = DFCS_BUTTONRADIO;
	cRc1 = cRc2 = m_pRc;

#ifdef _GDI
	CFont* oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
	
	int	ndc = dc->SaveDC();
	int	sMode;
#endif

//	if (!(m_form->properties & PR_TRANSPARENT))
	{
		g.DrawRectangle(setgPen(),  RECTG(m_pRc));
		g.FillRectangle(setgBrush(), RECTG(m_pRc));
	}

	float	width  = m_hR * 100;
	float	height = m_vR * 100;



	CString ss("1");
	RectF rt;
	g.MeasureString(ss.AllocSysString(), -1, setgFont(0), PointF(0, 0), &rt);

	float	fwidth  = rt.Width - 1;		//m_fWidth;
	float	fheight = rt.Height - 1;	//m_fHeight;


	if (abs((int)(width - height)) > 35)
	{
		if (m_hR > m_vR)
			fwidth--;
		else
			fheight--;
	}

	if (width < 50 && height > 70)
	{
		fwidth += float(0.25);
	}

	if (width < 50)
	{
		fheight -= 3;
		if (width < 35)
			fheight--;
	}


	cRc1.right  = m_pRc.left + (LONG)fheight - 1;
	cRc1.bottom = m_pRc.top + (LONG)fheight - 1;
	cRc2.left = cRc1.right + 3;

	if (m_pRc.Height() < m_pRc.Width())
	{
#ifdef _GDI
		sMode = dc->SetBkMode(TRANSPARENT);
		cRc2.left = cRc1.right + 3;
		if (m_form->properties & PR_ENABLE)
		{
			setTextColor(dc);
			dc->DrawText(m_strR, cRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
		}
		else
		{
			setTextColor(dc, getRGB(clGRAY));
			dc->DrawText(m_strR, cRc2, DT_SINGLELINE|DT_VCENTER|DT_LEFT|DT_NOPREFIX);
			style |= DFCS_INACTIVE;
		}
#else
		StringFormat sf;
		sf.SetAlignment(StringAlignmentNear);
		sf.SetLineAlignment(StringAlignmentCenter);	
		if (m_form->properties & PR_ENABLE)
		{
			g.DrawString(m_strR.AllocSysString(), -1, setgFont(0), RECTF(cRc2), &sf, setgFBrush());
		}
		else
		{
			g.DrawString(m_strR.AllocSysString(), -1, setgFont(1), RECTF(cRc2), &sf, setgBrush(clGRAY));			
			style |= DFCS_INACTIVE;
		}
#endif
	}


	if (m_form->properties & PR_CHECKED)
		style |= DFCS_CHECKED;

	if (m_pRc.Height() > cRc1.Height())
	{
		int	dx = (m_pRc.Height() - cRc1.Height())/2;
		cRc1.top   += dx;
		cRc1.bottom = cRc1.top + (int)fheight - 1;	
	}

	Image *pImage = NULL;
	if (m_form->properties & (PR_IMAGE) && m_pic && m_pic->GetValid(m_image))
	{
		m_pic->DrawPicture(g, cRc1, m_form->alignImage, m_image);
	}
	else 
	{
		if (m_mousedown)
		{
			style |= DFCS_INACTIVE;
		//	dc->DrawFrameControl(cRc1, DFC_BUTTON, style);
			pImage = DrawFrameControl(dc, cRc1, DFC_BUTTON, style);
			g.DrawImage(pImage, RECTG(cRc1));
		}
		else
		{	
			pImage = DrawFrameControl(dc, cRc1, DFC_BUTTON, style);
			g.DrawImage(pImage, RECTG(cRc1));
		}
	}
	SAFE_DELETE(pImage);

	m_iRc = cRc1;
	m_iRc.left   += 2;
	m_iRc.top    +=2;
	m_iRc.right  -=2;
	m_iRc.bottom -=2;
	m_tRc = cRc2;
	m_mousedown = false;

	if (m_focus)
	{
		Pen* pPen = setgPen(clBLACK, 1);
		pPen->SetDashStyle(DashStyleDot);
		g.DrawRectangle(pPen, RECTG(cRc2));
		g.DrawRectangle(pPen, RECTG(cRc1));
	}

//	::delete pbitmap;
#ifdef _GDI
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);
#endif
}

void CamRadio::OnLButton(bool down, CPoint pt, int& result)
{
	result      = RC_NOP;
	m_mousedown = down;
	if (!m_mousedown && IsPointInRect(pt))
	{
		m_axform->m_cursor = pt;
		SetChecked(true);
		return;
	}
	invalidateRect(&m_iRc, false);
}

void CamRadio::OnDblClick(CPoint pt, int& result)
{
	OnLButton(false, pt, result);
}

void CamRadio::SetFocus(bool focus)
{
	if (m_focus != focus)
	{
		m_focus = focus;
		if (!m_focus)
			m_mousedown = false;
//		if (m_form->properties & PR_TRANSPARENT)
			invalidateRect(&m_tRc, true);
//		else
//			invalidateRect(&m_tRc, false);
	}

//	SetChecked(true);
}

void CamRadio::SetChecked(bool checked)
{
	bool	ischecked = (m_form->properties & PR_CHECKED) ? true : false;
	if (ischecked != checked)
	{
		if (checked)
		{
			m_form->properties |= PR_CHECKED;
			m_image = PI_DOWN;
		}
		else
		{
			m_form->properties &= ~PR_CHECKED;
			m_image = PI_DEFAULT;
		}

		if (m_form->type == BTN_RADIO && checked)
			m_axform->UnCheckRadioGroup(m_name);
	}
	invalidateRect(&m_iRc, false);
}

void CamRadio::ReadData(CString& data, bool edit, int col, int row)
{
	data = (m_form->properties & PR_CHECKED) ? '1' : '0';
}

void CamRadio::WriteData(CString data, bool redraw, int col, int row)
{
	switch (data[0])
	{
	case '1':
		SetChecked(true);
		m_axform->SetRadioGroup(m_name, true);
		break;
	case '0':
		SetChecked(false);
		m_axform->SetRadioGroup(m_name, false);
		break;
	default:
		CamBase::WriteData(data, redraw, col, row);
		break;
	}
}
