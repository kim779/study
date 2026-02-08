// Rband.cpp : implementation file
//

#include "stdafx.h"
#include "Rband.h"
#include "NButton.h"
#include "../../axis/axMsg.hxx"

#define OPENBTN_1	1001
#define OPENBTN_2	1002
#define OPENBTN_3	1003
#define OPENBTN_4	1004

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DOCTORQNA		// Ã´Ã´¹Ú»ç
/////////////////////////////////////////////////////////////////////////////
// CRband

CRband::CRband(bool (*axiscall)(int, WPARAM, LPARAM), CString home)
{
	m_axiscall	= axiscall;
	
	m_home		= home;
	m_btn1		= NULL;
	m_btn2		= NULL;
	m_btn3		= NULL;
	m_btn4		= NULL;
	
	m_bCustomer = false;
}

CRband::~CRband()
{
	if (m_img.GetSafeHandle())	m_img.DeleteObject();
	if (m_btn1)	delete m_btn1;
	if (m_btn2)	delete m_btn2;
	if (m_btn3)	delete m_btn3;
	if (m_btn4)	delete m_btn4;
}


BEGIN_MESSAGE_MAP(CRband, CWnd)
	//{{AFX_MSG_MAP(CRband)
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_XMSG, OnXMSG)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRband message handlers

void CRband::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CDC	 memDC;
	CRect	 cRc;
	GetClientRect(cRc);

	if (memDC.CreateCompatibleDC(&dc))
	{
		CBitmap* pbitmap = memDC.SelectObject(&m_img);

		dc.BitBlt(0, 0, cRc.Width(), cRc.Height(), &memDC, 0, 0, SRCCOPY);
		memDC.SelectObject(pbitmap);
		memDC.DeleteDC();
	}
}

CSize CRband::GetBandSize()
{
	if (!m_img.GetSafeHandle())
		return CSize(0, 0);

	BITMAP	bm;
	m_img.GetBitmap(&bm);
	return CSize(bm.bmWidth, bm.bmHeight);
}

int CRband::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_bCustomer = (*m_axiscall)(AXI_CUSTOMER, 0, 0);
	//m_bCustomer = false;

	CString		file;
	file.Format("%s\\image\\%s", m_home, "BAR1_RBTN5.BMP");

	HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), file,
			IMAGE_BITMAP,0,0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)	m_img.Attach(hBitmap);

	DWORD	dwStyle = WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	
	/*if (!m_bPaxnet)
	{
		m_btn1 = new CNButton(OPENBTN_1);
		if (!m_btn1->Create(NULL, "¿Â¶óÀÎÄ¿¹Â´ÏÆ¼", dwStyle, CRect(4, 3, 35, 28), this, -1))
		{
			delete m_btn1;
			m_btn1 = NULL;
		}
		else
		{
			m_btn1->set_ToolTipText("ÅõÀÚÄ«¿î¼¿·¯");
			m_btn1->set_Image(CAxButtonName("bar1_rbtn1"));
		}
	}
	else*/	// paxnet
	{
		m_btn1 = new CNButton(OPENBTN_1);
		if (!m_btn1->Create(NULL, "ÇÏ³ªN ÆÅ½ºÇÃ¶óÀÚ", dwStyle, CRect(0, 0, 32, 26), this, -1))
		{
			delete m_btn1;
			m_btn1 = NULL;
		}
		else  
		{
			m_btn1->set_ToolTipText("ÇÏ³ªN ÆÅ½ºÇÃ¶óÀÚ");
			m_btn1->set_Image(CAxButtonName("bar1_rbtn5"));
		}
	}

	/*m_btn2 = new CNButton(OPENBTN_2);
	if (!m_btn2->Create(NULL, "¿ä±¸»çÇ×µî·Ï", dwStyle, CRect(38, 3, 69, 28), this, -1))
	{
		delete m_btn2;
		m_btn2 = NULL;
	}
	else
	{
		m_btn2->set_ToolTipText("°í°´ÀÇ ¼Ò¸®");
		m_btn2->set_Image(CAxButtonName("bar1_rbtn2"));
	}

	m_btn3 = new CNButton(OPENBTN_3);
	if (!m_btn3->Create(NULL, "Àå¾Ö½Å°í", dwStyle, CRect(72, 3, 103, 28), this, -1))
	{
		delete m_btn3;
		m_btn3 = NULL;
	}
	else
	{
		m_btn3->set_ToolTipText("Àå¾Ö½Å°í 119");
		m_btn3->set_Image(CAxButtonName("bar1_rbtn3"));
	}
	

#ifdef	DOCTORQNA
	if (!m_bCustomer)
	{
		m_btn4 = new CNButton(OPENBTN_4);
		if (!m_btn4->Create(NULL, "Ã´Ã´¹Ú»ç", dwStyle, CRect(106, 3, 137, 28), this, -1))
		{
			delete m_btn4;
			m_btn4 = NULL;
		}
		else
		{
			m_btn4->set_ToolTipText("Ã´Ã´¹Ú»ç");
			m_btn4->set_Image(CAxButtonName("bar1_rbtn4"));
		}
	}
#endif*/
	return 0;
}

LONG CRband::OnXMSG(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case OPENBTN_1:	
		/**
		if (!m_bPaxnet)
		{
			(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)"DH558300");	
		}
		else	// paxnet
		{
			(*m_axiscall)(AXI_HOMEPAXNET, 0, 0);
		}
		**/
		break;
	case OPENBTN_2:	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)"DH600700");	break;
	case OPENBTN_3:	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)"DH611900");	break;
	case OPENBTN_4:	(*m_axiscall)(AXI_CHANGEVIEW, 0, (LPARAM)"DH900400");	break;
	}
	return 0;
}