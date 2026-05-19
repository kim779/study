// amCc.h : amCc DLL의 기본 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.
#include "WriteXml.h"
#include "LoadXml.h"


// CamCcApp
// 이 클래스의 구현을 보려면 amCc.cpp를 참조하십시오.
//

class CamCcApp : public CWinApp
{
public:
	CamCcApp();
	virtual ~CamCcApp();

public:
	class CWriteXml	m_build;
	class CLoadXml	m_load;
	CString		m_curdir;
	CString		m_root;
	CMapStringToString	m_trlayout;
	CMapStringToString	m_trlayoutxml;
	CMapStringToString	m_rtssymbol;
	CMapStringToString	m_rtssymbolxml;
	CMapStringToString	m_control;

	void	SetTempPath(CString& dir, char* fileN, bool source = true);
	void	SetMapFilePath(CString& dir, char* fileN, bool source);
	void	setCurrentDirectory(CString curdir)	{ m_curdir = curdir;}
	void	GetTempName(char* fileN, char* prefix);

	void	setTrLayOut(int kind, CString tr);
	CString	GetTrLayOut();
	void	setRtsSymbol(int kind, CString tr);
	CString	GetRtsSymbol();
	CString	Parser(CString &srcstr, CString substr);
	void	removeAllCMap();
	void	LoadControl();

private:
	bool	m_loadControl;

// 재정의입니다.
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};
