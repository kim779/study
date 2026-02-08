// AxisCode.h : main header file for the AXISCODE DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "../../h/jmcode.h"
#include <afxtempl.h>

//using HCODE  = struct hjcode;
using HCODEX = struct hjcodex;

typedef struct listitem
{
	CString code;
	CString name;
	CString date;
}ITEMS;

typedef struct _elwcode
{
	CString		code;	// ELW 종목코드
	CString		name;	// 종목명
	CString		ktype;	// 권리유형 01:Call, 02:Put, 03:기타
	CString		htype;	// 권리행사방식 01:유럽형, 02:미국형, 03:기타
	CString		pcode;	// 발행사 코드
	CString		pname;	// 발행사명
	CString		mjmt;	// 만기월 (YYYYMMDD)
	CString		lpnum[5];	// LP 회원번호 (3자씩 5개)
	CString		bcode[5];	// 기초자산 코드들 "기초자산1;기초자산2;기초자산3;.."
	BOOL		bkoba{};		// 조기종료ELW여부
}ELWCODE;

typedef struct _pbcode
{
	CString		code;	// 발행사 코드
	CString		name;	// 발행사명
}PBCODE;

typedef struct _basecode
{
	CString		code;	// 기초자산 코드
	CString		name;	// 기초자산명

	_basecode(CString scode = "", CString sname = "")
	{
		code = scode;
		name = sname;
	}

}BASECODE;

#define ASCENDING	0
#define	DESCENDING	1

typedef struct _sortStr
{
	CString code;
	CString name;
}SORTSTR;

struct _efopcode
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	code[6]{};			/*종목코드*/
	char	hnam[39]{};			/*종목명*/
	char	opcd[6]{};			/*운용사 코드*/
	char	etfm[1]{};			/*테마코드*/
	char	etfl[1]{};			/*국내외 구분*/
};

struct _efopitem
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	opcd[6]{};			/*운용사 코드*/
	char	hnam[30]{};			/*운용사 한글약명*/
	char	lnfd[1]{};			/*라인피드*/
	
	//	_efopcode *efopcode;
};


struct _efoptema
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	tmcd[1]{};			/*테마코드*/
	char	tmnm[20]{};			/*테마명*/
	char	lnfd[1]{};			/*라인피드*/
};

struct _efopfore
{
	char	type[1]{};			/*데이터 타입*/
	/*'1':운용사별*/
	/*'2':테마별*/
	/*'3':국내외 구분별*/
	/*'4':종목별*/
	char	frcd[1]{};			/*국내외 구분('D':국내, 'F':해외)*/
	char	frnm[10]{};			/*국내외 구분명*/
	char	lnfd[1]{};			/*라인피드*/
};
/////////////////////////////////////////////////////////////////////////////
// CAxisCodeApp
// See AxisCode.cpp for the implementation of this class
//

class CAxisCodeApp : public CWinApp
{
public:
	BOOL	LoadCode();
	BOOL	LoadETFCode();
	BOOL    loadCJCode();
	int	GetVirtualScreenX();

public:
	CString m_root;
	CString m_folder;
//	std::vector<HCODE*>  _vETN;
	std::vector<HCODEX*> _vETNx;
	std::vector<std::tuple<char, CString, CString>>	_vETFitem;
	std::vector<std::tuple<char, char, CString>>	_vETFtema;
	std::vector<std::tuple<char, char, CString>>	_vETFfore;
	std::vector<std::tuple<char, CString, CString, CString, char, char>>	_vETFcode;
//	std::map<CString, HCODE*>			      _mapCODE;
	std::map<CString, HCODEX*>			      _mapCODEx;
	std::map<CString, std::pair<CString, int>>	      _mapACODE;
	std::map<CString, CString>			      _mapNAME;
	std::vector<std::tuple<CString, CString, char, char>> _vBond;

private:
	std::unique_ptr<char[]> m_hcB;
	
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisCodeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAxisCodeApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


