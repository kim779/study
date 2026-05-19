// fmCheck.cpp : implementation file
//

#include "stdafx.h"
#include "amCheck.h"
#include "amform.h"
#include "image.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CamCheck

IMPLEMENT_DYNCREATE(CamCheck, CamRadio)

CamCheck::CamCheck()
{
	EnableAutomation();
}

CamCheck::CamCheck(CAmForm* axform, struct _formR* formR)
	: CamRadio(axform, formR)
{
	if (m_form->properties & PR_CHECKED)
		m_image     = PI_DOWN;
	else	
		m_image     = PI_DEFAULT;
		
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

}


CamCheck::~CamCheck()
{
	SAFE_DELETE(m_pic);
}


void CamCheck::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CamCheck, CamRadio)
	//{{AFX_MSG_MAP(CamCheck)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CamCheck, CamRadio)
	//{{AFX_DISPATCH_MAP(CamCheck)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmCheck to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7E2C96E8-FBD9-4332-831A-8FC50D65554B}
static const IID IID_IfmCheck =
{ 0x7e2c96e8, 0xfbd9, 0x4332, { 0x83, 0x1a, 0x8f, 0xc5, 0xd, 0x65, 0x55, 0x4b } };

BEGIN_INTERFACE_MAP(CamCheck, CamRadio)
	INTERFACE_PART(CamCheck, IID_IfmCheck, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CamCheck message handlers

void CamCheck::Draw(CDC* dc)
{
	if (!(m_form->properties & PR_VISIBLE))
		return;

	Graphics g(dc->GetSafeHdc());

	UINT	style;
	CRect	cRc1, cRc2;
	style = DFCS_BUTTONCHECK;
	cRc1 = cRc2 = m_pRc;

#ifdef _GDI
	CFont* oldfont;
	CFont*	font = setFont(dc);
	oldfont = dc->SelectObject(font);
	
	int	ndc = dc->SaveDC();
	int	sMode;
#endif
	
//	if (!(m_form->properties & PR_TRANSPARENT))
//	{
		g.DrawRectangle(setgPen(),  RECTG(m_pRc));
		g.FillRectangle(setgBrush(), RECTG(m_pRc));
//	}

	float	width  = m_hR * 100;
	float	height = m_vR * 100;

	CString ss("1");
	RectF rt;
	g.MeasureString(ss.AllocSysString(), -1, setgFont(0), PointF(0, 0), &rt);


//	float	fwidth  = m_fWidth;
//	float	fheight = m_fHeight;

	float	fwidth  = rt.Width - 1;
	float	fheight = rt.Height - 1;


	if (abs((int)(width - height)) > 35)
	{
		if (m_hR > m_vR)
			fwidth--;
		else
			fheight--;
	}

	if (width < 50 && height > 70)
		fwidth += float(0.25);

	if (width < 50)
	{
		fheight -= 3;
		if (width < 35)
			fheight--;
	}

	cRc1.right  = m_pRc.left + (int)fheight - 1;
	cRc1.bottom = m_pRc.top + (int)fheight - 1;
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

		cRc2.left = cRc1.right + 3;
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
		cRc1.top += dx;
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
	//		dc->DrawFrameControl(cRc1, DFC_BUTTON, style);
	}
	SAFE_DELETE(pImage);

	m_iRc = cRc1;
	m_iRc.left++;
	m_iRc.top++;
	m_iRc.right--;
	m_iRc.bottom--;
	m_tRc = cRc2;
	m_mousedown = false;

	if (m_focus)
	{
		Pen* pPen = setgPen(clBLACK, 1);
		pPen->SetDashStyle(DashStyleDot);
		g.DrawRectangle(pPen, RECTG(cRc2));
	}
#ifdef _GDI
	dc->SelectObject(oldfont);
	dc->SetBkMode(sMode);
	dc->RestoreDC(ndc);
#endif
}

void CamCheck::OnLButton(bool down, CPoint pt, int& result)
{
	result = RC_NOP;
	m_mousedown = down;
	if (down)
		m_axform->m_cursor = pt;

	if (!m_mousedown && IsPointInRect(pt))
	{
		m_axform->m_cursor = pt;
		if (m_form->properties & PR_CHECKED)
		{
			m_form->properties &= ~PR_CHECKED;
			m_image = PI_DEFAULT;
		}
		else
		{
			m_form->properties |= PR_CHECKED;
			m_image = PI_DOWN;
		}
	}
	invalidateRect(&m_iRc, false);
}

void CamCheck::OnDblClick(CPoint pt, int& result)
{
	OnLButton(false, pt, result);
}

void CamCheck::SetFocus(bool focus)
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
}

void CamCheck::UpdateData(int key, bool moving, int& result)
{
	if (key = ' ')
	{
		bool	checked = (m_form->properties & PR_CHECKED) ? true : false;
		SetChecked(!checked);
	}
}

void CamCheck::InsertData(int key, bool moving, int& result)
{
	UpdateData(key, moving, result);
}
