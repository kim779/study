// IB202201.h : main header file for the IB202201 DLL
//


#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#define	TOmapWnd	(WM_USER + 1000)

#define	upACTION			1
#define	dnACTION			2
#define	dnACTIONX			3
#define	ssACTION			4
#define	dnACTIONR			5
#define ELWIssueDndnACTION		6
#define ELWAssetDndnACTION		7
#define RecommnadDnACTION		8
#define upsequenceACTION		9
#define upnewgroupACTION		10
#define upEachGroupACTION		11
#define upBackupACTION			12
#define upRecoveryACTION		13
#define upEndACTION			14
#define upCancelACTION			15
#define upOKACTION			16
#define dnGROUPLIST			17
#define dnGROUPCODE			18
#define getBOOLFILEPROCESS 19
#define upnewgroupFIRSTACTION		20



const int maxGROUP = 100, maxJONGMOK = 100;

/////////////////////////////////////////////////////////////////////////////
// CIB202201App
// See IB202201.cpp for the implementation of this class
//

class CIB202201App : public CWinApp
{
public:
	CIB202201App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIB202201App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CIB202201App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


