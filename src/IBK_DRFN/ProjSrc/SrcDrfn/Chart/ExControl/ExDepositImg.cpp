// ExDepositImg.cpp : implementation file
//

#include "stdafx.h"
#include "../../Inc/ExDepositImg.h"
#include "Include/LoadHelper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExDepositImg

CExDepositImg::CExDepositImg()
{
	m_pLoader = new CLoaderDepositImg(this);
}

CExDepositImg::~CExDepositImg()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExDepositImg, CStatic)
	//{{AFX_MSG_MAP(CExDepositImg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExDepositImg message handlers

void CExDepositImg::PreSubclassWindow() 
{
	m_pLoader->PreSubclassWindow();
}

BOOL CExDepositImg::OnEraseBkgnd(CDC* pDC) 
{	
	return m_pLoader->OnEraseBkgnd(pDC);
}

void CExDepositImg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	m_pLoader->OnPaint(&dc);
}

void CExDepositImg::SetDepositIndex(UINT nIndex)
{
	m_pLoader->SetDepositIndex(nIndex);
}

void CExDepositImg::SizeToImage()
{
	m_pLoader->SizeToImage();
}