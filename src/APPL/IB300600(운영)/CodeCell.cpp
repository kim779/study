// CodeCell.cpp : implementation file
//

#include "stdafx.h"
#include "IB300600.h"
#include "CodeCell.h"
#include "Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeCell

CCodeCell::CCodeCell(CString strTabPath)
	: m_pCodeCtrl(NULL)
	, m_pCodeButton(NULL)
	, m_strTabPath(strTabPath)
{
}

CCodeCell::~CCodeCell()
{
	_SAFEDESTROY(m_pCodeCtrl);
	_SAFEDESTROY(m_pCodeButton);
}


BEGIN_MESSAGE_MAP(CCodeCell, CWnd)
	//{{AFX_MSG_MAP(CCodeCell)
	ON_WM_CREATE()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeCell message handlers

int CCodeCell::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CRect rcClient, rcCtrl;
	GetClientRect(&rcClient);

	rcCtrl = rcClient;
	rcCtrl.DeflateRect(0, 0, 0, 21);

	m_pCodeCtrl = new CfxCodeCtrl(this, m_strTabPath);
	m_pCodeCtrl->Create(this, rcCtrl, IDC_CODECTRL);
	
	rcCtrl = rcClient;
	rcCtrl.DeflateRect(rcCtrl.Width() - 21, 0, 0, 0);

	m_pCodeButton = new CfxImgButton;
	m_pCodeButton->Create(_T(""), rcCtrl, this, IDC_CODEBUTTON);

	return 0;
}

void CCodeCell::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	GetClientRect(&rcClient);
	dc.FillSolidRect(rcClient, RGB(255, 0, 0));	
	// Do not call CWnd::OnPaint() for painting messages
}
