// PhoneThread.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "PhoneThread.h"
#include "trsmain.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoneThread

IMPLEMENT_DYNCREATE(CPhoneThread, CWinThread)

CPhoneThread::CPhoneThread()
{
}

CPhoneThread::~CPhoneThread()
{
}

BOOL CPhoneThread::InitInstance()
{
	// TODO:  perform and per-thread initialization here
	return TRUE;
}

int CPhoneThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CPhoneThread, CWinThread)
	//{{AFX_MSG_MAP(CPhoneThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoneThread message handlers

int CPhoneThread::Run() 
{
	// TODO: Add your specialized code here and/or call the base class

	
	return CWinThread::Run();
}
