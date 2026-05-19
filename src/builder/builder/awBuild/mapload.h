
#pragma once

#include <afxtempl.h>
#include "../h/mapvar.h"
#include "../awWcc/libWcc.h"

// CMapLoad 명령 대상입니다.

class CMapLoad
{
public:
	CMapLoad();
	virtual ~CMapLoad();

protected:
	class mapForm*	m_mapH;

	int	m_rtype;
	int	m_formP,  m_colP;
	bool	m_column;
	CMapToken	token[MAX_TOKEN_N];

	CString	m_script;
	struct _tabPage
	{
		int tabN;
		int pageN;
	};
	CMap	<int, int, _tabPage, _tabPage>	m_tabPageArr;
	CMap	<int, int, int, int>	m_groupArr;

public:
	void	Load(class mapForm* mapForm, CString curdir);
	void	BuildTab(int idx = 0);

	void	BuildXmlHeader(CNode* ElementNode);
	void	BuildXmlForm(CNode* pElementNode, CNode* pCellPropNode);
	void	BuildXmlResource(CNode* pElementNode);
	void	BuildXmlTRlayout(CNode* pElementNode);
	void	BuildXmlLayoutEdit(CNode* pElementNode);
	void	BuildXmlRTS(CNode* pElementNode);

protected:
	void	saveScriptFile(CString path, CString script);
	int	getFormIndex(char* symbol, int len, int count);
	int	getColumnIndex(char* symbol, int len, int pos);
	CString Parser(CString &srcstr, CString substr);
};


