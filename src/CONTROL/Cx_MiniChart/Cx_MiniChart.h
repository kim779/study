// Cx_MiniChart.h : main header file for the CX_MINICHART DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/axisfire.h"
#include "../../h/axiscgrp.h"


/////////////////////////////////////////////////////////////////////////////
// CCx_MiniChartApp
// See Cx_MiniChart.cpp for the implementation of this class
//
#pragma pack(1)	// start pack

#define	MAX_JISU	4
#define	MJ_VERSION	8

struct _jisu
{
	char	code[8]{};
	char	name[32]{};
	BYTE	view{};	// 1
	DWORD	color{};
	BYTE	style{};
	BYTE	width{};
};
#define	sz_JISU	sizeof(struct _jisu)

struct _minijisu
{
	WORD		version{};
	struct _jisu	jisu[MAX_JISU];	// kospi, kospi200, kosadq, kosdaq50, kospi¼±¹°
	BYTE		compare{};
	BYTE		tick{};
	BYTE		fixmode{};
};
#define	sz_MINIJISU	sizeof(struct _minijisu)

#define	CMP_CSIGA	0x01
#define	CMP_PJGGA	0x02

#define	TK_LEFT		0x01
#define	TK_RIGHT	0x02

struct _cgMJ {
	struct _cgTime	time;
	int		val[MAX_JISU]{0,0,0,0};
	int		ratio[MAX_JISU]{ 0,0,0,0 };
};
#define sz_CGMJ	sizeof(struct _cgMJ)

#pragma pack()	// end pack


class CCx_MiniChartApp : public CWinApp
{
public:
	CCx_MiniChartApp();


	void	SetView(CWnd *pView)	{ m_pView = pView; }

	HINSTANCE	m_hDlg{};
	CString	m_imgPath;
	CString	m_infoPath;

	CWnd		*m_pView{};
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCx_MiniChartApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCx_MiniChartApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	
private:


};