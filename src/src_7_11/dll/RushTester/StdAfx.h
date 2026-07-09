// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__59E1F556_2C51_4232_8BA7_38D420CE5E37__INCLUDED_)
#define AFX_STDAFX_H__59E1F556_2C51_4232_8BA7_38D420CE5E37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


struct	_ledgerH {
	//char	len[8];
	char	anam[40];		/* application name	*/
	char	snam[40];		/* service name		*/
	char	fnam[32];		/* function name	*/
	char	fncd[2];		/* function code	*/
	char	guid[32];		/* global id		*/
	char	ctyp[2];		/* channel type		*/
	char	eflg[1];		/* environment type	*/
	char	tflg[1];		/* tran flag		*/
	char	tdat[8];		/* tran send date	*/
	char	ttim[20];		/* tran send time	*/
	char	clip[31];		/* client ip		*/
	char	rtyp[1];		/* response type	*/
	char	rcod[8];		/* response code	*/
	char	rlcd[20];		/* response log code	*/
	char	rtit[50];		/* response title	*/
	char	rbmg[100];		/* response message	*/
	char	rdmg[500];		/* response message	*/
	char	usid[12];		/* user id		*/
	char	crst[2];		/* character set	*/
	char	fill[78];		/* filler		*/
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	sfill[38];		/* filler		*/
	char	susid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__59E1F556_2C51_4232_8BA7_38D420CE5E37__INCLUDED_)
