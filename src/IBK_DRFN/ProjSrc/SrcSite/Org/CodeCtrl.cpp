// CodeCtrl.cpp : implementation file
//

#include <stdafx.h>
#include "CodeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl

CCodeCtrl::CCodeCtrl()
{
	m_nID = 0;

	m_hiCodeCtrl = NULL;

	m_pwndCodeCtrl = NULL;
	m_pwndParent = NULL;
	m_pwndWizard = NULL;
}

CCodeCtrl::~CCodeCtrl()
{
	if (m_pwndCodeCtrl)
	{
		m_pwndCodeCtrl->DestroyWindow();
		delete m_pwndCodeCtrl;
		m_pwndCodeCtrl = NULL;
	}

	if (m_hiCodeCtrl)
	{
		BOOL bResult = FreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
	}
}


BEGIN_MESSAGE_MAP(CCodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CCodeCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_COMMAND, OnCommand)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl message handlers

BOOL CCodeCtrl::CreateCodeCtrl(CWnd* pwndParent, CWnd* pwndWizard, CRect rect, UINT nID)
{
	m_pwndParent = pwndParent;
	m_pwndWizard = pwndWizard;
	m_nID = nID;

	return CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, m_pwndParent, m_nID);
}

int CCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_hiCodeCtrl = LoadLibrary("EX_CODECTRL.DLL");
	if (!m_hiCodeCtrl)
		return -1;

	CWnd* (APIENTRY *axCreateDLL)(CWnd*, CWnd*, CRect, int);
	axCreateDLL = (CWnd* (APIENTRY*)(CWnd*, CWnd*, CRect, int))GetProcAddress(m_hiCodeCtrl, "axCreateDLL");
	if (!axCreateDLL)
	{
		FreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
		return -1;
	}

	CRect rectC;
	GetClientRect(rectC);
	m_pwndCodeCtrl = axCreateDLL(this, m_pwndWizard, rectC, 1);
	if (!m_pwndCodeCtrl)
	{
		FreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
		return -1;
	}

	return 0;
}

bool CCodeCtrl::SetUnit(CString strUnit)
{
	if (strUnit.IsEmpty() || !m_hiCodeCtrl || !m_pwndCodeCtrl)
		return false;

	bool (APIENTRY *axSetUnit)(CWnd* , CString);
	axSetUnit = (bool (APIENTRY*)(CWnd*, CString))GetProcAddress(m_hiCodeCtrl, "axSetUnit");
	if (!axSetUnit)
	{
		return false;
	}

	return axSetUnit(m_pwndCodeCtrl, strUnit);
}

BOOL CCodeCtrl::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if ((HWND)lParam == m_pwndCodeCtrl->m_hWnd)
		return m_pwndParent->SendMessage(WM_COMMAND, MAKEWPARAM(m_nID, HIWORD(wParam)), (LPARAM)m_hWnd);
	
	return CWnd::OnCommand(wParam, lParam);
}

CString CCodeCtrl::GetCode()
{
	if (!m_pwndCodeCtrl)
		return _T("");

	char* (APIENTRY *axGetCode)(CWnd*);
	axGetCode = (char* (APIENTRY*)(CWnd*))GetProcAddress(m_hiCodeCtrl, "axGetCode");
	if (!axGetCode)
	{
		return _T("");
	}

	CString strCode = CString(axGetCode(m_pwndCodeCtrl));
	return strCode;
}

CString CCodeCtrl::GetName()
{
	if (!m_pwndCodeCtrl)
		return _T("");

	char* (APIENTRY *axGetName)(CWnd*);
	axGetName = (char* (APIENTRY*)(CWnd*))GetProcAddress(m_hiCodeCtrl, "axGetName");
	if (!axGetName)
	{
		return _T("");
	}

	CString strName = CString(axGetName(m_pwndCodeCtrl));
	return strName;
}

bool CCodeCtrl::SetCode(CString strCode, bool bOnlySet, bool bSetHistory)
{
	if (!m_pwndCodeCtrl)
		return false;

	bool (APIENTRY *axSetCode)(CWnd*, char*, bool, bool);
	axSetCode = (bool (APIENTRY*)(CWnd*, char*, bool, bool))GetProcAddress(m_hiCodeCtrl, "axSetCode");
	if (!axSetCode)
	{
		return false;
	}

	return axSetCode(m_pwndCodeCtrl, (char*)(LPCTSTR)strCode, bOnlySet, bSetHistory);
}

bool CCodeCtrl::SetColor(COLORREF rgbFG, COLORREF rgbBG)
{
	if (!m_pwndCodeCtrl)
		return false;

	bool (APIENTRY *axSetColor)(CWnd*, COLORREF, COLORREF);
	axSetColor = (bool (APIENTRY*)(CWnd*, COLORREF, COLORREF))GetProcAddress(m_hiCodeCtrl, "axSetColor");
	if (!axSetColor)
	{
		return false;
	}

	return axSetColor(m_pwndCodeCtrl, rgbFG, rgbBG);
}

void CCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_pwndCodeCtrl && IsWindow(m_pwndCodeCtrl->m_hWnd))
		m_pwndCodeCtrl->MoveWindow(0, 0, cx, cy);
}

bool CCodeCtrl::SetFont(CFont* pfont)
{
	if (!m_pwndCodeCtrl)
		return false;

	bool (APIENTRY *axSetFont)(CWnd*, CFont*);
	axSetFont = (bool (APIENTRY*)(CWnd*, CFont*))GetProcAddress(m_hiCodeCtrl, "axSetFont");
	if (!axSetFont)
	{
		return false;
	}

	return axSetFont(m_pwndCodeCtrl, pfont);
}