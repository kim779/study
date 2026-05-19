// VbGuide.cpp : implementation file
//

#include "stdafx.h"
#include "VbGuide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVbGuide

IMPLEMENT_DYNCREATE(CVbGuide, CWinThread)

CVbGuide::CVbGuide(CString title, CString topic)
{
	m_title = title;
	m_topic = topic;
}

CVbGuide::CVbGuide()
{
}

CVbGuide::~CVbGuide()
{
}

BOOL CVbGuide::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CVbGuide::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CVbGuide, CWinThread)
	//{{AFX_MSG_MAP(CVbGuide)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVbGuide message handlers

int CVbGuide::Run() 
{
	if (AfxGetMainWnd())
		MessageBox(AfxGetMainWnd()->m_hWnd, m_topic, m_title, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL|MB_TOPMOST);
	else
	{
#ifdef	_DEBUG
		TRACE("\n>>> %s <<<\n", m_title);
		TRACE("%s\n", m_topic);
#else
		MessageBox(NULL, m_topic, m_title, MB_OK|MB_ICONERROR|MB_SYSTEMMODAL|MB_TOPMOST);
#endif
	}
	return 0;
}
