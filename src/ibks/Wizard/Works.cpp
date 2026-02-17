// Works.cpp: implementation of the CWorks class.
//
//////////////////////////////////////////////////////////////////////

#include "pch.h"
#include "Wizard.h"
#include "Works.h"
#include "OleDrop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorks::CWorks(CGuard* guard, CWnd* view, int key, int type)
{
	m_key   = key;
	m_view  = view;
	m_type  = type;
	m_group = 0;					// trigger disable
	m_vid   = guard->m_vid;
	m_axisL = 0;
	m_axisB = NULL;
	m_guard = guard;
	m_font  = m_guard->m_font;

	m_status   = 0;
//	m_drop     = NULL;
	m_callproc = NULL;
	SetWindowLong(m_view->GetSafeHwnd(), GWL_USERDATA, long(this));
}

CWorks::~CWorks()
{
	if (m_callproc)
		SetWindowLong(m_view->GetSafeHwnd(), GWL_WNDPROC, (LONG)m_callproc);

	m_saves.RemoveAll();
	if (m_axisL && m_axisB)
		delete [] m_axisB;

//	if (m_drop)
//	{
//		m_drop->Revoke();
//		delete m_drop;
//	}
}

bool CWorks::Attach(CString maps, bool only, bool fix)
{
	return false;
}

void CWorks::OnStream(_axisH *axisH, char *pBytes, int nBytes)
{
	if (m_axisL > 0)
	{
		char*	axisB = m_axisB;
		m_axisB = new char[m_axisL+nBytes+1];
		CopyMemory(m_axisB, axisB, m_axisL);
		delete [] axisB;
	}
	else
	{
		m_axisL = L_axisH;
		m_axisB = new char[L_axisH+nBytes+1];
		CopyMemory(m_axisB, axisH, L_axisH);
	}
	CopyMemory(&m_axisB[m_axisL], pBytes, nBytes);
	m_axisL += nBytes;

	if (!(axisH->stat & statCON))
	{
		MSG	msg;
		while (m_status & S_ING)
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			Sleep(50);
		}
		if (m_status & S_LOAD)
			OnAxis((struct _axisH *)m_axisB, &m_axisB[L_axisH], m_axisL-L_axisH);
		m_axisL = 0;
		delete [] m_axisB;
	}
}

void CWorks::OnAxis(_axisH *axisH, char *pBytes, int nBytes)
{

}

void CWorks::OnAlert(CString code, CString update, int stat)
{

}

void CWorks::OnTrigger(CString name, CString text, int key)
{

}

void CWorks::OnProcedure(CString proc, CString param, int key)
{

}

void CWorks::OnNotice(CString text)
{

}

bool CWorks::isWorks()
{
	return false;
}

void CWorks::OnSave(CString name, CString text)
{
	m_saves.SetAt(name, text);
}

BOOL CWorks::GetAt(CString name, CString &text)
{
	return m_saves.Lookup(name, text);
}

BOOL CWorks::GetData(CString& name, CString& tmps)
{
	return TRUE;
}

void CWorks::OnDomino(CString domino)
{
}

int CWorks::OnInvoke(int wParam, int lParam)
{
	return 0;
}

void CWorks::OnDrop(CPoint point, CString text, CWnd* pWnd)
{
}

void CWorks::SetPalette()
{
}

void CWorks::SetFCB(bool blink, int flash)
{
}

BOOL CWorks::SetFont(int point, bool resize, bool attach)
{
	return FALSE;
}

bool CWorks::isTrigger(int key, CString name, CString text)
{
	return false;
}

bool CWorks::isYours(int key)
{
	return false;
}

BOOL CWorks::Register(HWND hWnd, bool tips)
{
	if (!tips)
		m_status |= S_DROP;
	if (hWnd)
		return m_guard->m_drop.Register(this, hWnd);
//	if (m_drop == NULL)
//		m_drop = new COleDrop();
//	return m_drop->Register(this);
	return m_drop.Register(this);
}

void CWorks::Revoke(HWND hWnd)
{
	if (hWnd)
		m_guard->m_drop.Revoke(hWnd);
//	else if (m_drop)
	else
	{
//		m_drop->Revoke(); delete m_drop; m_drop = NULL;
		m_drop.Revoke();
	}
}
