// Compile.cpp: implementation of the CCompile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "libwcc.h"
#include "Compile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCompile::CCompile()
{
	for (int ii = 0; ii < MAX_TEXT_LINE; ii++)
	{
		m_text[ii].text = NULL;
		m_text[ii].type = TYPE_NOTDEF;
	}
	m_textB  = NULL;
	m_textB2 = NULL;
	m_status = NULL;
	m_valueR = NULL;
	m_pageR = NULL;

	initVariables();
}

CCompile::~CCompile()
{
	if (m_textB)
		delete[] m_textB;
	if (m_textB2)
		delete[] m_textB2;
	if (m_status)
		delete m_status;
}

void CCompile::initVariables()
{
	m_memForm   = NULL;
	m_memDom    = NULL;
	m_memEvent  = NULL;
	m_memAct    = NULL;
	m_memCell   = NULL;
	m_memMethod = NULL;

	m_rtype = m_dtype = m_etype = m_ptype = m_stype = 0;
	m_fmPos = m_ifN = m_caseN = m_exprN = m_colPos = 0;

	m_iscolumn = false;
	m_swSym    = 0;
	m_swProp   = 0;

	m_oldPos   = -1;
	m_script   = scpNONE;

	m_arSTATE.RemoveAll();
	m_arVAR.RemoveAll();
	m_index = -1;
	m_wcc = (CwccSTATE *) NULL;
}

int CCompile::mapMemoryAllocation(int totLine)
{
	WORD	formN;
	CString	tmps;

	if (m_status)
		m_status->ShowBitmap(1, true);

	m_fN  = 0;  m_strN = 0;	m_scriptN = 0;
	m_err = 0;  formN  = 0; m_pageN = 0; m_valueN = 0;
	m_pyAwait = false; m_pyBlock = false; m_pyWrap.Empty();

	for (int ii = 0; ii < totLine; ii++)
	{
		switch(m_text[ii].type)
		{	
		case TYPE_FORM:
			formN++;
		default:
			continue;
		}
	}

	initVariables();
	ZeroMemory(&m_mapH, L_MAPH);

	// init value
	m_mapH.helpN = NOVALUE;

	if (mapMemoryAlloc(formN) == -1)
	{
		setMessage(-1, 4);
		mapMemoryFree();
		return(-1);
	}

	if (m_status)
	{
		m_status->SetRange(0, totLine);
		m_status->SetStep(1);
	}

	for (int ii = 0; ii < formN+1; ii++)
	{
		ZeroMemory(&m_formR[ii], L_FORMR);
		ZeroMemory(&m_domR[ii], sizeof(struct _domR));
		m_formR[ii].keys      = NOVALUE;
		m_formR[ii].isEdit    = IE_NOP;
		m_formR[ii].onEdit    = NOVALUE;
		m_formR[ii].onHint    = NOVALUE;
		for (int jj = 0; jj < 3; jj++)
			m_formR[ii].vals[jj] = NOVALUE;
	}

	return(0);
}

int CCompile::mapTokenParsing(CMapToken *mapToken, int line, WORD *type)
{
	if (m_text[line].type == TYPE_NOTDEF)
	{
		*type = TYPE_NOTDEF;
		return(-1);
	}

	mapToken->Clear();

	switch (m_text[line].type)
	{
	case TYPE_COMMENT:
		return (0);

	case TYPE_GLOBAL:
		return (mapGlobalTokenParsing(mapToken, line, type));

	case TYPE_FORM:
	case TYPE_GRAPH:
		return (mapFormTokenParsing(mapToken , line, type));

	case TYPE_RESOURCE:
		return (mapResourceTokenParsing(mapToken, line, type));

	default:
		*type = TYPE_NOTDEF;
		return(-1);
	}

	return(0);
}

char* CCompile::mapResParsing(int line, WORD *type)
{
	if (m_text[line].type == TYPE_NOTDEF)
	{
		*type = TYPE_NOTDEF;
		return "";
	}

	switch (m_text[line].type)
	{
	case TYPE_RESOURCE:
	case TYPE_COMMENT:
		*type = TYPE_RESOURCE;
		if (AfxIsValidString(m_text[line].text))
			return m_text[line].text;
		break;
	default:
		*type = TYPE_NOTDEF;
		break;
	}
	return "";
}

int CCompile::mapTokenParsingBuilding(CMapToken* mapToken, int line, WORD *type)
{
	int	tokCnt;

	if (m_text[line].type == TYPE_NOTDEF)
	{
		*type = TYPE_NOTDEF;
		if (m_status)
			m_status->StepIt();
		return(0);
	}

	mapToken->Clear();
	mapToken[1].Clear();
	switch (m_text[line].type)
	{
	case TYPE_COMMENT:
		return(0);

	case TYPE_GLOBAL:
		tokCnt = mapGlobalTokenParsing(mapToken, line, type);
		mapGlobalStrBuilding(mapToken, tokCnt, line);
		break;

	case TYPE_FORM:
		tokCnt = mapFormTokenParsing(mapToken , line, type);
		if (tokCnt <= 0)
			break;
		mapFormStrBuilding(mapToken, tokCnt, line);
		m_fN++;
		break;

	case TYPE_RESOURCE:
		tokCnt = mapResourceTokenParsing(mapToken, line, type);
		mapResStrBuilding(mapToken, tokCnt, line);
		break;

	default:
		*type = TYPE_NOTDEF;
		break;
	}

	for (int ii = 0; ii < tokCnt; ii++)
		mapToken[ii].Clear();

	if (m_status)
		m_status->StepIt();
	return (m_paren);
}

int CCompile::mapMemoryAlloc(int formN)
{
	char*	gP;
	int	fSize;

	fSize = L_FORMR * (formN +1);
	m_memForm = GlobalAlloc(GPTR, fSize);
	if (m_memForm == (HANDLE) 0)
		return(-1);
	
	gP = (char *)GlobalLock(m_memForm);
	if (gP == (char *) 0)
		return(-1);
	m_formR = (struct _formR *) gP;

	fSize = sizeof(struct _domR) * (formN +1);
	m_memDom = GlobalAlloc(GPTR, fSize);
	if (m_memDom == (HANDLE) 0)
		return(-1);
	
	gP = (char *)GlobalLock(m_memDom);
	if (gP == (char *) 0)
		return(-1);
	m_domR = (struct _domR *) gP;
	return(0);
}

int CCompile::mapColMemAlloc(int cellN)
{
	m_repN = -1;
	if (cellN == 0)
		return (0);

	char	*gP;
	int	szCell;

	szCell = sizeof(struct _cellR) * cellN;
	m_memCell = GlobalAlloc(GPTR, szCell);
	if (m_memCell == (HANDLE) 0)
		return(-1);
	
	gP = (char *)GlobalLock(m_memCell);
	if (gP == (char *) 0)
		return(-1);
	m_cellR = (struct _cellR *) gP;
	for (int ii = 0; ii < cellN; ii++)
	{
		ZeroMemory(&m_cellR[ii], sizeof(struct _cellR));
		m_cellR[ii].name    = NOVALUE;
		m_cellR[ii].isEdit  = IE_NOP;
		m_cellR[ii].onEdit  = NOVALUE;
		m_cellR[ii].head    = NOVALUE;
		for (int jj = 0; jj < 2; jj++)
			m_cellR[ii].vals[jj] = NOVALUE;
	}
	return(0);
}

void CCompile::mapMemoryFree()
{
	CwccSTATE*	state;
	CwccVAR*	var;
	for (int ii = 0; ii < m_arSTATE.GetSize(); ii++)
	{
		state = m_arSTATE.GetAt(ii);
		delete state;
	}
	m_arSTATE.RemoveAll();

	for (int ii = 0; ii < m_arVAR.GetSize(); ii++)
	{
		var = m_arVAR.GetAt(ii);
		delete var;
	}
	m_arVAR.RemoveAll();

	if (m_memForm != (HANDLE) NULL)
	{
		GlobalUnlock(m_memForm);
		GlobalFree(m_memForm);
		m_memForm = (HANDLE) NULL;
	}

	if (m_memDom != (HANDLE) NULL)
	{
		GlobalUnlock(m_memDom);
		GlobalFree(m_memDom);
		m_memDom = (HANDLE) NULL;
	}

	if (m_memEvent != (HANDLE) NULL)
	{
		GlobalUnlock(m_memEvent);
		GlobalFree(m_memEvent);
		m_memEvent = (HANDLE) NULL;
	}

	if (m_memAct != (HANDLE) NULL)
	{
		GlobalUnlock(m_memAct);
		GlobalFree(m_memAct);
		m_memAct = (HANDLE) NULL;
	}

	if (m_memCell != (HANDLE) NULL)
	{
		GlobalUnlock(m_memCell);
		GlobalFree(m_memCell);
		m_memCell = (HANDLE) NULL;
	}

	if (m_memMethod != (HANDLE) NULL)
	{
		GlobalUnlock(m_memMethod);
		GlobalFree(m_memMethod);
		m_memMethod = (HANDLE) NULL;
	}

	if (m_valueR)
	{
		delete []m_valueR;
		m_valueR = NULL;
	}
	if (m_pageR)
	{
		delete []m_pageR;
		m_pageR = NULL;
	}

	if (!m_err)
	{
		if (m_status)
		{
			m_status->DestroyWindow();
			delete m_status;
			m_status = NULL;
		}
	}
}

void CCompile::setMessage(int line, int errNo, CString str, int pos, bool isevent)
{
	if (m_status == (CStatus *) NULL)
		return;

	CString	name  = _T("");
	CString	event = _T("");
	m_err++;
	
	line = (line < 0) ? line : line+1;
	if (isevent)
	{
		event = getEventName();
		switch (m_script)
		{
		case scpSTART:
		case scpSEND:
		case scpRECEIVE:
		case scpKEY:
		case scpTRIGGER:
			pos = -1;	break;
		default:
			break;
		}
	}
	if (pos >= 0 && pos <= m_fN)
	{
		name = CString(&m_strR[m_formR[pos].name]);
		pos  = m_formR[pos].kind;
	}
	else
		pos = -1;

	m_status->SetMessage(m_err, pos, name, event, errNo, str, line);
}

CString CCompile::getEventName()
{
	switch (m_script)
	{
	case scpSTART:
		return _T("ON_START");
	case scpSEND:
		return _T("ON_SEND");
	case scpRECEIVE:
		return _T("ON_RECEIVE");
	case scpKEY:
		return _T("ON_KEY");
	case scpTRIGGER:
		return _T("ON_TRIGGER");
	case scpCLICK:
		return _T("ON_CLICK");
	case scpDBLCLK:
		return _T("ON_DBLCLK");
	case scpCHANGE:
		return _T("ON_CHANGE");
	case scpCHAR:
		return _T("ON_CHAR");
	case scpPROC:
		return _T("PROCEDURE");
	case scpNONE:
	default:
		return _T("");
	}
}

void CCompile::getTempName(char* fileN, char* prefix)
{
	_putenv("TMP=NULL");

	char*   tmpN  = _tempnam((char *)m_curdir.operator LPCTSTR(), prefix);
if (tmpN == NULL)
AfxMessageBox("XError [f1]");
	CString fileS = CString(tmpN);
	CopyMemory(fileN, (char*)fileS.operator LPCTSTR(), fileS.GetLength());
	fileN[fileS.GetLength()] = '\0';
	free(tmpN);
}
