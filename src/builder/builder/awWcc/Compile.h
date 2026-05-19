
#pragma once

#include "../../h/mapform.h"
#include "libWcc.h"
#include "XMLMnger.h"

struct _domR {
	char	dSym[16+1];	// domino    FORM Symbol - string
	char	rSym[16+1];	// reference FORM Symbol - string
};
class CCompile
{
public:
	CCompile();
	virtual ~CCompile();

public:
	int	LoadSource(char* szPath, CWnd* hwnd, CString srcData = _T(""));
	bool	makeBinaryFile(CString strTmp, class mapForm* mapH);
	bool	makeHTSBinaryFile(CString strTmp, class mapForm* mapH);
	bool	makeMTSBinaryFile(CString strTmp, class mapForm* mapH);
	
	void	SetTempPath(CString& dir, char* fileN, bool source = true);

	int	BuildObject(char* szPath, CString nickN);
	void	SetDominoForm();
	void	SetCurrentDirectory(CString curdir)	{ m_curdir = curdir; }
	void	GetTempName(char* fileN, char* prefix);
	void	GetXmlNode(CNode &pElementNode, CString ElementKind);
	int	GetCellNode(CNode &pElementNode, int *idx);
	int	GetFormNode(CNode &pElementNode, int *idx);

private:
	void	initVariables();

	int	parseGlobalToken(CMapToken* token, int line, WORD *type);
	int	parseFormToken(CMapToken* token, int line, WORD *type);
	int	parseResourceToken(CMapToken* token, int line, WORD *type);

	void	buildFormStr(CMapToken* token, int tokCnt, int line);
	void	buildResourceStr(CMapToken* token, int tokCnt, int line);

	int	getFormIndex(char* symbol, int len);
	bool	compareSymbol(CString string);
	bool	compareColumnSymbol(CString symbol, int pIdx, int rIdx);
	int	getColumnIndex(char* symbol, int len, int idx);

	int	writeString(CString str);
	int	writeValue(DWORD value);
	int	writePage(_pageR pageR);
	int	writeScript(CString str);

	void	setMessage(int line, int errNo, CString str = _T(""), int pos = -1, bool isevent = true);

protected:
	class	CStatus*	m_status;
	char*	m_textB, *m_textB2;
	struct	_mapText	m_text[MAX_TEXT_LINE];

	struct	_mapH		m_mapH;
	struct	_formR*		m_formR;
	struct	_domR*		m_domR;
	struct	_pageR*		m_pageR;
	struct	_cellR*		m_cellR;

	char	*m_strR, *m_scriptR;
	DWORD*	m_valueR;

	HANDLE	m_memForm;
	HANDLE	m_memDom;
	HANDLE	m_memCell;

	int	m_fN;
	int	m_strN;
	int	m_repN;
	int	m_pageN;
	int	m_valueN;
	int	m_scriptN;

	int	m_err;
	CString	m_curdir, m_symbol;

private:
	int	m_tokFlag;
	int	m_oldPos;
	class CXMLMnger* m_XMLMnger;
	CWnd* m_pWnd;
};


