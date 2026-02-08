// IB202201.h : main header file for the IB202201 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#define	TOmapWnd	(WM_USER + 1000)
#define	WM_MSG	(WM_USER + 1001)

#define	upACTION			1
#define	dnACTION			2
#define	dnACTIONX			3
#define	ssACTION			4
#define	dnACTIONR			5
#define ELWIssueDndnACTION  6
#define ELWAssetDndnACTION  7
#define RecommnadDnACTION	8

#define upsequenceACTION	9
#define upnewgroupACTION	10
#define upEachGroupACTION	11
#define upBackupACTION		12
#define upRecoveryACTION	13
#define upEndACTION			14
#define upCancelACTION		15
#define upOKACTION				16
#define getBOOLFILEPROCESS 17


#define TRKEY_GROUPARR							40
#define TRKEY_GROUPCODE						41
#define TRKEY_GROUPCODE_PAGE4			42
#define TRKEY_upBackupACTION				43
#define TRKEY_upEachGroupACTION			44
#define TRKEY_upOKACTION						45
#define TRKEY_upEndACTION						46
#define TRKEY_upsequenceACTION			47
#define TRKEY_upnewgroupACTION			48
#define TRKEY_upRecoveryACTION			49
#define TRKEY_upCancelACTION				50
#define TRKEY_dnACTION							51
#define TRKEY_dnACTIONR							52

#define MSG_SEARCH_GROUPCODE						17
#define MSG_GET_GROUPARR								18
#define MSG_MTP_CPL_SEARCH_GROUPCODE		19   //그룹종목코드 조회 완료후 페이지에 통보
#define MSG_GET_GROUPCODE								20
#define MSG_GET_GROUPCOUNT							21
#define MSG_SET_NEWGROUP								22
#define MSG_SEARCH_GROUPCODE_PAGE4			23
#define MSG_SET_ACTIVE_PAGE								24
#define MSG_GET_ACTIVE_PAGE								25

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


