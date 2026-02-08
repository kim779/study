// ExEdit.cpp: implementation of the CExEdit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../../Inc/ExEdit.h"
#include "../../Inc/IBaseDefine.h"
#include "../../Inc/IMainExInfoMng.h"

#include "Include/LoadHelper.h"
//#include "c:\Program Files\Microsoft Visual Studio\VC98\MFC\SRC\winhand_.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CExEdit, CEdit)
/////////////////////////////////////////////////////////////////////////////
// CExEdit

CExEdit::CExEdit(BOOL bUseDefSkin/* = FALSE*/)
{
	m_pLoader = new CLoaderEdit(this, bUseDefSkin);
}

CExEdit::~CExEdit()
{
	delete m_pLoader;
}


BEGIN_MESSAGE_MAP(CExEdit, CEdit)
	//{{AFX_MSG_MAP(CExEdit)
	ON_WM_PAINT()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExEdit message handlers
long CExEdit::ActionControl(LPVOID lpAction)
{
	return m_pLoader->ActionControl(lpAction);
}

void CExEdit::OnPaint() 
{
	Default();
	m_pLoader->OnPaint();
}

void CExEdit::UseNumericType(BOOL bUse)
{
	m_pLoader->UseNumericType(bUse);
}

void CExEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pLoader->OnChar(nChar, nRepCnt, nFlags);
}

BOOL CExEdit::PreTranslateMessage(MSG* pMsg)
{
	return CEdit::PreTranslateMessage(pMsg);
}

void CExEdit::PreSubclassWindow() 
{
	CEdit::PreSubclassWindow();
#ifndef _DEBUG
	SetPermanentHandle(m_hWnd, this);
#endif
}
