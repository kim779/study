// xscreen.cpp : implementation file
//

#include "stdafx.h"
#include "xscreen.h"
#include "screen.h"
#include "client.h"
#include "script.h"
#include "stream.h"
#include "mouse.h"

#include "../dll/form/amgrid.h"
#include "../dll/form/amctrl.h"
#include "../dll/lua/scriptEngine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CxScreen

IMPLEMENT_DYNCREATE(CxScreen, CCmdTarget)

CxScreen::CxScreen(CScreen* screen)
{
	EnableAutomation();
	m_screen  = screen;
	m_guard   = m_screen->m_guard;
	m_return  = TRUE;

	m_pBytes  = NULL;
	m_service = svREADY;
}

CxScreen::~CxScreen()
{
	if (m_pBytes)
	{
		delete [] m_pBytes;
		m_pBytes = NULL;
	}
}


void CxScreen::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CxScreen, CCmdTarget)
	//{{AFX_MSG_MAP(CxScreen)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CxScreen, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CxScreen)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IxScreen to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {892925C0-9BED-4662-9932-DB3E6ECB329B}
static const IID IID_IxScreen =
{ 0x892925c0, 0x9bed, 0x4662, { 0x99, 0x32, 0xdb, 0x3e, 0x6e, 0xcb, 0x32, 0x9b } };

BEGIN_INTERFACE_MAP(CxScreen, CCmdTarget)
	INTERFACE_PART(CxScreen, IID_IxScreen, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CxScreen message handlers

void CxScreen::LuaSend(long target, CString strTR)
{
	vector<string> vstr;
	split(vstr, string(strTR), is_any_of("/"), token_compress_on);

	if (vstr.size() < 1)
		return;
	_Send(atoi(vstr[0].data()), vstr[1].data());
}

void CxScreen::_Send(long target, CString strTR) 
{
	if (m_screen->m_client->m_vm->m_script)
		return;		// ignore dup

	CScreen* screen;
	switch (target)
	{
	case targetMAIN:
		if (m_screen->m_client->GetAtScreen(screen))
			break;
	case targetTHIS:
		screen = m_screen;
		break;
	case targetALL:
		m_screen->m_client->m_stream->InStream();
	default:
		return;		// invalid script
	}

	m_screen->m_client->m_stream->InStream(strTR, screen);
}

bool CxScreen::OnService(char* pBytes, int nBytes)
{
	if (pBytes == NULL)
		return (m_service == svWAIT) ? true : false;

	switch (m_service)
	{
	case svTIMEOUT:
		m_service = svREADY;
		return true;
	case svWAIT:
		break;
	default:
		return false;
	}

	if (m_pBytes)
		delete [] m_pBytes;

	m_nBytes = nBytes;
	m_pBytes = new char[nBytes];
	CopyMemory(m_pBytes, pBytes, nBytes);
	m_service = svDONE;
	m_screen->m_client->WaitDone(m_screen, false, true);
	return true;
}

