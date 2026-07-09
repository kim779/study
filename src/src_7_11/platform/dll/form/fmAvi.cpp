// fmAvi.cpp : implementation file
//

#include "stdafx.h"
#include "axform.h"
#include "fmAvi.h"

#include "../../h/axisvar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CfmAvi

IMPLEMENT_DYNCREATE(CfmAvi, CfmBase)

CfmAvi::CfmAvi()
{
	EnableAutomation();

	ZeroMemory(&m_mciOpen, sizeof(MCI_ANIM_OPEN_PARMS));
	ZeroMemory(&m_mciRect, sizeof(MCI_ANIM_RECT_PARMS));
	ZeroMemory(&m_mciWindow, sizeof(MCI_ANIM_WINDOW_PARMS));
	ZeroMemory(&m_mciPlay, sizeof(MCI_PLAY_PARMS));
}

CfmAvi::CfmAvi(CAxisForm* axform, struct _formR* formR, int iform)
	: CfmBase(axform, formR, iform)
{
	EnableAutomation();

	CString	path;
	path.Format(_T("%s\\%s\\%s"), LPCTSTR(m_axform->m_root), IMAGEDIR, m_strR.GetString());
	m_strR = path;

	m_valid = true;
	if (::GetFileAttributes((char *)m_strR.operator LPCTSTR()) == 0xFFFFFFFF)
	{
		m_valid = false;
		return;
	}

	ZeroMemory(&m_mciOpen,   sizeof(MCI_ANIM_OPEN_PARMS));
	ZeroMemory(&m_mciRect,   sizeof(MCI_ANIM_RECT_PARMS));
	ZeroMemory(&m_mciWindow, sizeof(MCI_ANIM_WINDOW_PARMS));
	ZeroMemory(&m_mciPlay,   sizeof(MCI_PLAY_PARMS));

	m_mciOpen.lpstrDeviceType  = "AVIVideo";
	m_mciOpen.lpstrElementName = m_strR.operator LPCTSTR();
	mciSendCommand(0, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_OPEN_TYPE | MCI_ANIM_OPEN_WS, (DWORD)&m_mciOpen);
	m_aviDev = m_mciOpen.wDeviceID;
	mciSendCommand(m_aviDev, MCI_WHERE, MCI_ANIM_WHERE_SOURCE | MCI_ANIM_RECT, (DWORD)&m_mciRect);
	m_mciRect.rc = m_pRc;
	mciSendCommand(m_aviDev, MCI_PUT, MCI_ANIM_PUT_DESTINATION|MCI_ANIM_RECT, (DWORD)&m_mciRect);
	m_mciWindow.hWnd = m_axform->m_view->m_hWnd;
	mciSendCommand(m_aviDev, MCI_WINDOW, MCI_ANIM_WINDOW_HWND, (DWORD)&m_mciWindow);
}

CfmAvi::~CfmAvi()
{
	mciSendCommand(m_aviDev, MCI_CLOSE, 0, NULL);
}


void CfmAvi::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CfmAvi, CfmBase)
	//{{AFX_MSG_MAP(CfmAvi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CfmAvi, CfmBase)
	//{{AFX_DISPATCH_MAP(CfmAvi)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IfmAvi to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {C7240887-09C7-472F-AAB4-5198C20B1A01}
static const IID IID_IfmAvi =
{ 0xc7240887, 0x9c7, 0x472f, { 0xaa, 0xb4, 0x51, 0x98, 0xc2, 0xb, 0x1a, 0x1 } };

BEGIN_INTERFACE_MAP(CfmAvi, CfmBase)
	INTERFACE_PART(CfmAvi, IID_IfmAvi, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CfmAvi message handlers

void CfmAvi::Resize(float hR, float vR, bool font, bool redraw)
{
	CfmBase::Resize(hR, vR, font, redraw);
	Play();
}

bool CfmAvi::Play()
{
	if (!(m_form->properties & PR_VISIBLE) || !m_valid)
		return false;

	mciSendCommand(m_aviDev, MCI_PAUSE, MCI_NOTIFY, (DWORD)&m_mciPlay);
	mciSendCommand(m_aviDev, MCI_WHERE, MCI_ANIM_WHERE_SOURCE | MCI_ANIM_RECT, (DWORD)&m_mciRect);

	m_mciRect.rc.left    = m_pRc.left;
	m_mciRect.rc.top     = m_pRc.top;
	m_mciRect.rc.right   = m_pRc.right;
	m_mciRect.rc.right  -= m_pRc.left;
	m_mciRect.rc.bottom  = m_pRc.bottom;
	m_mciRect.rc.bottom -= m_pRc.top;

	mciSendCommand(m_aviDev, MCI_PUT, MCI_ANIM_PUT_DESTINATION | MCI_ANIM_RECT, (DWORD)&m_mciRect);
	m_mciPlay.dwCallback = (DWORD)m_axform->m_view->m_hWnd;
	mciSendCommand(m_aviDev, MCI_PLAY, MCI_NOTIFY, (DWORD)&m_mciPlay);
	return true;
}

bool CfmAvi::Replay()
{
	if (!(m_form->properties & PR_VISIBLE) || !m_valid)
		return false;

	mciSendCommand(m_aviDev, MCI_REALIZE, MCI_ANIM_REALIZE_NORM, NULL);
	mciSendCommand(m_aviDev, MCI_SEEK, MCI_SEEK_TO_START, NULL);
	m_mciPlay.dwCallback = (DWORD)m_axform->m_view->m_hWnd;
	mciSendCommand(m_aviDev, MCI_PLAY, MCI_NOTIFY, (DWORD)&m_mciPlay);
	return true;
}

void CfmAvi::Stop()
{
	if (!(m_form->properties & PR_VISIBLE) || !m_valid)
		return;

	mciSendCommand(m_aviDev, MCI_PAUSE, MCI_NOTIFY, (DWORD)&m_mciPlay);
	mciSendCommand(m_aviDev, MCI_CLOSE, 0, NULL);
	m_aviDev = 0;
}
