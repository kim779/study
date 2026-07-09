// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE 클래스입니다.
#include <afxodlgs.h>       // MFC OLE 대화 상자 클래스입니다.
#include <afxdisp.h>        // MFC 자동화 클래스입니다.
#endif // _AFX_NO_OLE_SUPPORT

#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>                      // MFC ODBC 데이터베이스 클래스입니다.
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>                     // MFC DAO 데이터베이스 클래스입니다.
#endif // _AFX_NO_DAO_SUPPORT

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>                     // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxcontrolbars.h>
#pragma warning(disable:4996)


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

struct	_ledgerLoginH	{
	char	tmno[15];		/* terminal no		*/
	char	tmps[15];		/* terminal position	*/
	char	dept[8];		/* user department	*/
	char	brnc[8];		/* user branch		*/
	char	sale[8];		/* business branch	*/
	char	agnc[8];		/* agency		*/
	char	fill[38];		/* filler		*/
	char	usid[20];		/* user id		*/
	char	menu[20];		/* menu			*/
};
