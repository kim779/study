// Compile.h: interface for the CCompile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPILE_H__42330B21_905F_11D2_A459_00C00C02F5D7__INCLUDED_)
#define AFX_COMPILE_H__42330B21_905F_11D2_A459_00C00C02F5D7__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <afxtempl.h>
#include "status.h"
#include "../../h/mapform.h"

struct _domR {
	char	dSym[16+1];	// domino    FORM Symbol - string
	char	rSym[16+1];	// reference FORM Symbol - string
};

class CwccSTATE
{
public:
	BYTE	m_type;		// statement kind : SMT_IF
				//                  SMT_IFELSE
				//                  SMT_SWITCH
				//                  SMT_CASE
				//                  SMT_DEFAULT
	DWORD	m_ev;		// eventR offset
	int	m_dtype;	// data type

				// SMT_SWITCH only
	BYTE	m_depth;
	BYTE	m_swtype;	// switch expression type : SW_NONE
				//                          SW_SYMBOL
				//                          SW_VAR
				//                          SW_SYSTEM
				//                          SW_EVENT
	DWORD	m_swSym;	// offset in m_strR (SW_SYMBOL or SW_VAR)
	BYTE	m_swProp;	// properties       (SW_SYMBOL or SW_SYSTEM)
	CDWordArray m_case;	// case statement offset array
				// use in SMT_DEFAULT
};

class CwccVAR
{
public:
	BYTE	m_type;		// variables type : VA_INT
				//                  VA_STRING
				//                  VA_BOOL
	CString	m_name;
	DWORD	m_offs;		// offset in m_strR
};

class CCompile  
{
public:
	CCompile();
	virtual ~CCompile();
	void DebugData(char* buf, long bufsize);
public:
	CString	getFileData(CString sPath);
	CString Parser(CString &srcstr, CString substr);
	void	ConvertString(char *src, int len);
	int	writePage(_pageR pageR);
	int	writeScript(CString str, int len);
	int	mapSourceFileLoad(char* szPath, CWnd* hwnd, CString srcData = "");
	int	mapMemoryAllocation(int totLine);
	void	mapMemoryFree();
	
	char*	mapResParsing(int line, WORD* type);
	int	mapTokenParsing(CMapToken* mapToken, int line, WORD* type);
	int	mapTokenParsingBuilding(CMapToken* mapToken, int line, WORD* type);
	
	void	SetMapFilePath(CString& dir, char* fileN, bool source = true);
	void	SetTempPath(CString& dir, char* fileN, bool source = true);
	
	int	mapColumnAnalyze(int tLine);

	int	mapBuildObjectFile(char* szPath, CString nickN);
	void	setDominoForm();
	void	setCurrentDirectory(CString curdir)	{ m_curdir = curdir; }
	void	getTempName(char* fileN, char* prefix);

	void	lastCompileWork();

private:
	void	initVariables();
	int	writeString(CString str, int len);
	int	writeValue(DWORD value);

	int	mapMemoryAlloc(int formN);
	int	mapColMemAlloc(int cellN);

	void	mapGlobalStrBuilding(CMapToken* token, int tokCnt, int line);
	void	mapFormStrBuilding(CMapToken* token, int tokCnt, int line);
	void	mapResStrBuilding(CMapToken* token, int tokCnt, int line);

	bool	isCurrent(CString string);
	int	getFormIndex(char* symbol, int len);
	DWORD	getOffset(CString symbol, bool formOnly=false);
	DWORD	getOffset(int index);
	int	getIndex(DWORD offs);
	int	compareSymbol(CString string);
	int	compareColumnSymbol(CString symbol, int pIdx, int rIdx);
	int	getColumnIndex(char* symbol, int len, int idx);
	int	getRowIndex(char* symbol, int len, int idx);
	
	int	mapGlobalTokenParsing(CMapToken* token, int line, WORD *type);
	int	mapFormTokenParsing(CMapToken* token, int line, WORD *type);
	int	mapResourceTokenParsing(CMapToken* token, int line, WORD *type);
	
	int	getHexValue(char c);
	void	setMessage(int line, int errNo, CString str = _T(""), int pos = -1, bool isevent = true);
	CString	getEventName();
	
	bool	checkNumeric(CString str);

protected:
	CStatus*	m_status;
	char*	m_textB, *m_textB2;
	struct	_mapText	m_text[MAX_TEXT_LINE];

	struct	_mapH		m_mapH;
	struct	_formR*		m_formR;
	struct	_domR*		m_domR;
	struct	_pageR*		m_pageR;
	struct	_cellR*		m_cellR;

	char	*m_strR, *m_scriptR;
	char	m_srcN[16];
	DWORD*	m_valueR;

	HANDLE	m_memForm;
	HANDLE	m_memDom;
	HANDLE	m_memEvent;
	HANDLE	m_memAct;
	HANDLE	m_memCell;
	HANDLE	m_memMethod;

	int	m_fN;
	int	m_strN;
	int	m_repN;
	int	m_pageN;
	int	m_valueN;
	int	m_scriptN;
	
	int	m_err;
	int	m_paren;
	CString	m_curdir, m_symbol;

	bool	m_pyAwait;	// waiting for first content line of a script block to detect Python vs VBScript
	bool	m_pyBlock;	// current script block is Python (skip Sub/End Sub wrapping)
	CString	m_pyWrap;	// deferred "Sub ..." wrapper text, written only if block turns out to be VBScript


private:
	CArray	<CwccSTATE *, CwccSTATE *>	m_arSTATE;
	CArray	<CwccVAR *, CwccVAR *>		m_arVAR;
	int	m_index;		// current index in m_stARR
	CwccSTATE*	m_wcc;

	int	m_rtype,  m_dtype,  m_etype,  m_ptype, m_stype;
	int	m_ifN, m_caseN, m_exprN, m_ev, m_prevN;
	int	m_fmPos, m_cellPos, m_colPos;
	bool	m_iscolumn;
	DWORD	m_swSym;
	BYTE	m_swProp;
	int	m_tokFlag;

	int	m_oldPos;
	enum	{ scpNONE=0, scpSTART, scpSEND, scpRECEIVE, scpKEY, scpTRIGGER,
			scpCLICK, scpDBLCLK, scpCHANGE, scpCHAR, scpPROC } m_script;
};

#endif // !defined(AFX_COMPILE_H__42330B21_905F_11D2_A459_00C00C02F5D7__INCLUDED_)
