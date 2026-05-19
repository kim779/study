#pragma once

#include "symbol.h"

// CSymTbl

class CSymTbl : public CWnd
{
	DECLARE_DYNAMIC(CSymTbl)

public:
	CSymTbl();
	virtual ~CSymTbl();

	bool	m_default;
	CPtrArray	m_arSET;

	void	Initialize(CString root, bool can);
	void	LoadProject();
	void	LoadFile();
	void	RemoveAll();
	Csymbol* GetSelectedData(int index);
	void	SetCurrect(CsymbolSET* set)	{ m_currentSET = set; }

protected:
	CString	m_root;
	bool	m_canReceive;
	CCriticalSection m_section;

	CsymbolSET* m_currentSET;

	void	loadProject(Cproject* project, CString path);
	void	loadFile(Croot* root);
	BYTE	getIO(CString str);
	BYTE	getKind(CString str, int& type);
	CString	getMask(int type, int size);
	DWORD	getAttributes(CString& str, int size);
	BYTE	getAlignment(CString str);
	BYTE	getType(CString str);
	void	parse(Csymbol* symbol, CStringArray& tokArr);
	void	makeToken(char ch, CString str, CStringArray& tokArr);
	void	str2data(CString& str, CString& data);
	CString getPath(CString fileN);
	CString	parse(CString &src, CString sub);

protected:
	DECLARE_MESSAGE_MAP()
};


