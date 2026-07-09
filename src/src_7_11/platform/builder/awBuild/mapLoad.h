// mapLoad.h: interface for the CMapLoad class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPLOAD_H__55AD7880_54B6_11D4_A024_00001CD7F9BE__INCLUDED_)
#define AFX_MAPLOAD_H__55AD7880_54B6_11D4_A024_00001CD7F9BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "../mapvar.h"
#include "../awWcc/libwcc.h"
#include "../awCommon/FormItem.h"

class CMapLoad  
{
public:
	CMapLoad();
	virtual ~CMapLoad();

// Attributes
protected:
	CString	m_curdir;
	struct	_mapH*		m_mapH;
	CFormItem		*m_pFormItem;

	int	m_rtype;
	int	m_formP,  m_colP;
	int	m_formN;
	bool	m_column;
	CMapToken       token[MAX_TOKEN_N];

	CString	m_script;
	struct _tabPage
	{
		int tabN;
		int pageN;
	};
	CMap	<int, int, _tabPage, _tabPage>	m_tabPageArr;
	CMap	<int, int, int, int>	m_groupArr;

// Implementation
public:
	void	SaveScriptFile(CString sPath, CString sScript);
	void	BuildTab(int idx = 0);
	void	Load(struct _mapH* mapH, CString curdir);
	void	BuildHeader(CMapToken* token, int count);
	void	BuildForm(CMapToken* token, int count, int& fN);
	void	BuildResource(char* text, int line, int formN);

protected:
	int	getFormIndex(char* symbol, int len, int count);
	int	getColumnIndex(char* symbol, int len, int pos);
};

#endif // !defined(AFX_MAPLOAD_H__55AD7880_54B6_11D4_A024_00001CD7F9BE__INCLUDED_)
