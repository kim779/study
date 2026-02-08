// CX_Category.h : main header file for the CX_Category DLL
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCX_CategoryApp
// See CX_Category.cpp for the implementation of this class
//
#define PT_CX 365
#define PT_CY 412

#define	WM_CATEGORY	WM_USER + 1
#define	WM_POPUPMENU	WM_USER + 2
#define	WM_INITLOAD	WM_USER + 3

constexpr char* acd_KOSPI[] = {
	"",
	"KGG01P",	// 코스피		1
	"KGZ01P",	// 코스피 대형주	2	
	"KGZ02P",	// 코스피 중형주	3
	"KGZ03P",	// 코스피 소형주	4
	"KGS01P",	// 음식료품		5
	"KGS02P",	// 섬유, 의복		6
	"KGS03P",	// 종이, 목재		7
	"KGS04P",	// 화학			8
	"KGS05P",	// 의약품		9	
	"KGS06P",	// 비금속광물		10
	"KGS07P",	// 철강및금속		11
	"KGS08P",	// 기계			12
	"KGS09P",	// 전기, 전자		13
	"KGS19P",	// 의료정밀		14
	"KGS10P",	// 운수장비		15
	"KGS11P",	// 유통업		16
	"KGS20P",	// 전기가스업		17
	"KGS12P",	// 건설업		18	
	"KGS13P",	// 운수창고		19
	"KGS21P",	// 통신업		20
	"KGS14P",	// 금융업		21
	"KGS15P",	// 은행			22
	"",
	"KGS16P",	// 증권			24
	"KGS17P",	// 보험			25
	"KGS22P",	// 서비스업		26
	"KGS18P"	// 제조업		27
};

constexpr char* acd_KOSDAQ[] = {
	"",
	"",
	"",
	"",
	"",
	"",
	"QGS02P",	// 제조			6
	"QGS03P",	// 건설			7
	"QGS04P",	// 유통			8
	"",
	"QGS31P",	// 운송			10 
	"QGS05P",	// 금융			11
	"QGS06P",	// 통신방송		12
	"QGS07P",	// IT S / W & SVC	13
	"QGS08P",	// IT H / W		14      
	"QGS09P",	// 음식료·담배		15 
	"QGS10P",	// 섬유·의류		16
	"QGS11P",	// 종이 목재		17
	"QGS12P",	// 출판·매체복제	18
	"QGS13P",	// 화학			19
	"QGS33P",	// 제약			20
	"QGS14P",	// 비금속		21
	"QGS15P",	// 금 속		22
	"QGS16P",	// 기계·장비		23
	"QGS17P",	// 일반전기전자		24
	"QGS18P",	// 의료·정밀기기	25
	"QGS19P",	// 운송장비·부품	26
	"QGS20P",	// 기타 제조		27
	"QGS21P",	// 통신서비스		28
	"QGS22P",	// 방송서비스		29
	"QGS23P",	// 인터넷		30
	"QGS24P",	// 디지털컨텐츠		31
	"QGS25P",	// 소프트웨어		32
	"QGS26P",	// 컴퓨터서비스		33
	"QGS27P",	// 통신장비		34
	"QGS28P",	// 정보기기		35
	"QGS29P",	// 반도체		36
	"QGS30P",	// IT부품		37
	"QGS87P",	//         
	"",
	"",		//			39
	"",		//			40
	"QGS34P",	// 오락·문화		41
	"QGG03P",	// 코스닥 우량기업부	42
	"QGG04P",	// 코스닥 벤처기업부	43
	"QGG05P",	// 코스닥 중견기업부	44
	"QGG06P"	// 코스닥 기술성장기업부45 
};

class CCX_CategoryApp : public CWinApp
{
public:
	CCX_CategoryApp();

private:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCX_CategoryApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCX_CategoryApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
