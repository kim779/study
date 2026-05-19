// symTbl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "axisMBuilder.h"
#include "symTbl.h"
#include "table.hxx"

//#include "awSock64/download.h"


// CSymTbl

IMPLEMENT_DYNAMIC(CSymTbl, CWnd)

CSymTbl::CSymTbl()
{
	m_root = _T("");
	m_canReceive = true;

	m_arSET.RemoveAll();
	m_currentSET = NULL;
}

CSymTbl::~CSymTbl()
{
	RemoveAll();
}


BEGIN_MESSAGE_MAP(CSymTbl, CWnd)
END_MESSAGE_MAP()

// CSymTbl 메시지 처리기입니다.


void CSymTbl::Initialize(CString root, bool can)
{
	m_root = root;
	m_canReceive = can;
}

void CSymTbl::LoadProject()
{
	/*char	wb[256];
	CString	tmps, path;
	Cproject*	project;
	CStringArray	strarr;

	RemoveAll();
	m_default  = false;

	path = getPath(PROJECT_INI);
	if (m_canReceive)
	{
		m_section.Lock();
		CDownload* dn = new CDownload();
		if (dn->Create())
			dn->Download(_T("work/project.ini"), path, 1);
		delete dn;
		m_section.Unlock();
	}

	for (int ii = 1; ; ii++)
	{
		tmps.Format(_T("%03d"), ii);
		GetPrivateProfileString(_T("General"), tmps, _T(""), wb, sizeof(wb), path);
		tmps = wb;
		if (tmps.IsEmpty())
			break;

		project = new Cproject;
		project->m_name = tmps;
		str2data(project->m_name, project->m_section);
		str2data(project->m_section, tmps);
		if (!tmps.IsEmpty() && atoi(tmps) == 0)
			project->m_default = true;
		else
			project->m_default = false;

		loadProject(project, path);
		m_arSET.Add(project);
	}
	
	if (m_arSET.GetSize() > 0)
		m_default = ((Cproject *)m_arSET.GetAt(0))->m_default;*/
}

void CSymTbl::LoadFile()
{
	Cproject*	project;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		project = (Cproject *)m_arSET.GetAt(ii);
		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
			loadFile(project->m_arPRO.GetAt(jj));
	}
}

void CSymTbl::RemoveAll()
{
	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	Croot*		root;
	Cproject*	project;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		project = (Cproject *)m_arSET.GetAt(ii);
		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
		{
			root = project->m_arPRO.GetAt(jj);
			for (int kk = 0; kk < root->m_arROOT.GetSize(); kk++)
			{
				group = root->m_arROOT.GetAt(kk);
				for (int ll = 0; ll < group->m_arGROUP.GetSize(); ll++)
				{
					symSET = group->m_arGROUP.GetAt(ll);
					for (int mm = 0; mm < symSET->m_arSYM.GetSize(); mm++)
					{
						symbol = symSET->m_arSYM.GetAt(mm);
						delete symbol;
					}
					symSET->m_arSYM.RemoveAll();
					delete symSET;
				}
				group->m_arGROUP.RemoveAll();
				delete group;
			}
			root->m_arROOT.RemoveAll();
			delete root;
		}
		project->m_arPRO.RemoveAll();
		delete project;
	}

	m_arSET.RemoveAll();
}

void CSymTbl::loadProject(Cproject* project, CString path)
{
	char	wb[256];
	CString	tmps;
	Croot*	root;
	CStringArray	strarr;

	for (int ii = 1; ; ii++)
	{
		tmps.Format(_T("%03d"), ii);
		GetPrivateProfileString(project->m_section, tmps, _T(""), wb, sizeof(wb), path);
		tmps = wb;
		if (tmps.IsEmpty())
			break;

		root = new Croot;
		root->m_name = tmps;
		str2data(root->m_name, root->m_file);
		project->m_arPRO.Add(root);
	}
}

void CSymTbl::loadFile(Croot* root)
{
	char	wb[1024+1];
	CString	tmps, data = _T("");
	CString path = getPath(root->m_file);

	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	CStringArray	strarr;
	CFile	file;

	/*if (m_canReceive)
	{
		m_section.Lock();
		CDownload* dn = new CDownload();
		tmps.Format(_T("work\\%s"), root->m_file);
		if (dn->Create())
			dn->Download(tmps, path, 1);
		delete dn;
		m_section.Unlock();
	}*/

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return;

	UINT nBytesRead;
	do
	{
		nBytesRead = file.Read(wb, 1024);
		data += CString(wb, nBytesRead);
	}
	while((int)nBytesRead);
	file.Close();

	group = new Cgroup;
	group->m_name = _T("현물:KPHM");
	str2data(group->m_name, group->m_data);
	root->m_arROOT.Add(group);
	
	while (!data.IsEmpty())
	{
		tmps = parse(data, _T("\n"));
		if (tmps.IsEmpty())
			continue;

		if (tmps.Find(_T("[")) >= 0 && tmps.Find(_T("]")) >= 0)
		{
			tmps.Replace(_T("["), _T(""));
			tmps.Replace(_T("]"), _T(""));
			symSET = new CsymbolSET;
			symSET->m_name = tmps;
			str2data(symSET->m_name, symSET->m_data);
			group->m_arGROUP.Add(symSET);
		}
		else if (tmps.Find(_T(";")) >= 0)
		{
		}
		else if (tmps.Find(_T(",")) >= 0)
		{
			symbol = new Csymbol;
			strarr.RemoveAll();
			makeToken(_T(','), tmps, strarr);
			parse(symbol, strarr);
			
			symSET->m_arSYM.Add(symbol);
		}
	}
}

BYTE CSymTbl::getIO(CString str)
{
	for (int ii = 0; ii < IOTBL; ii++)
	{
		if (!str.CompareNoCase(ioTbl[ii].strName))
			return ioTbl[ii].iok;
	}
	return 0;
}

BYTE CSymTbl::getKind(CString str, int& type)
{
	for (int ii = 0; ii < KINDTBL; ii++)
	{
		if (!str.CompareNoCase(kindTbl[ii].strName))
		{
			type = kindTbl[ii].type;
			return kindTbl[ii].kind;
		}
	}
	type = 0;
	return 0;
}

CString CSymTbl::getMask(int type, int size)
{
	int	ii;
	CString	mask = _T("");
	
	switch (type)
	{
	case EMASK_DATE:
		for (ii = 0; ii < DATETBL; ii++)
		{
			if (size == dateTbl[ii].size)
			{
				mask = dateTbl[ii].strName;
				break;
			}
		}
		break;

	case EMASK_TIME:
		for (ii = 0; ii < TIMETBL; ii++)
		{
			if (size == timeTbl[ii].size)
			{
				mask = timeTbl[ii].strName;
				break;
			}
		}
	default:
		break;
	}

	return mask;
}

DWORD CSymTbl::getAttributes(CString& str, int size)
{
	CString	mask = _T("");
	DWORD	attr = 0;
	CStringArray	strArr;

	makeToken(_T('|'), str, strArr);
	for (int ii = 0; ii < strArr.GetSize(); ii++)
	{
		str = strArr.GetAt(ii);
		for (int jj = 0; jj < ATTRTBL; jj++)
		{
			if (!str.CompareNoCase(attrTbl[jj].strName))
			{
				switch (attrTbl[jj].attr)
				{
				//case EMASK_DATE:
				//	attr |= FA_DATE;
				case EMASK_TIME:
					mask = getMask(attrTbl[jj].attr, size);
					break;
				default:
					attr |= attrTbl[jj].attr;
					break;
				}
			}
		}
	}
	str = mask;
	return attr;
}

BYTE CSymTbl::getAlignment(CString str)
{
	for (int ii = 0; ii < ALIGNTBL; ii++)
	{
		if (!str.CompareNoCase(alignTbl[ii].strName))
			return alignTbl[ii].align;
	}

	return AL_LEFT;
}

BYTE CSymTbl::getType(CString str)
{
	for (int ii = 0; ii < TYPETBL; ii++)
	{
		if (!str.CompareNoCase(typeTbl[ii].strName))
			return typeTbl[ii].type;
	}
	return IO_NUM;
}

void CSymTbl::parse(Csymbol* symbol, CStringArray& tokArr)
{
	CString	tmps;
	int	tmpn;

	for (int ii = 0; ii < SYM_ITEMN; ii++)
	{
		if (ii <= tokArr.GetUpperBound())
			tmps = tokArr.GetAt(ii);
		else
			tmps = _T("");

		switch (ii)
		{
		case 0:		// symbol name
			symbol->m_name = tmps;			break;

		case 1:		// label or head
			symbol->m_head = tmps;			break;

		case 2:		// label size width
			symbol->m_headsz.cx = atoi(tmps);	break;

		case 3:		// label size height
			symbol->m_headsz.cy = atoi(tmps);	break;

		case 4:		// label text  RGB
			symbol->m_htRGB = (DWORD) atof(tmps);	break;

		case 5:		// label paint RGB
			symbol->m_hpRGB = (DWORD) atof(tmps);	break;

		case 6:		// data size
			symbol->m_size = atoi(tmps);		break;

		case 7:		// data type
			symbol->m_type = getType(tmps);		break;

		case 8:		// data alignment
			symbol->m_align = getAlignment(tmps);	break;

		case 9:		// data size width
			symbol->m_datasz.cx = atoi(tmps);	break;

		case 10:	// data size height
			symbol->m_datasz.cy = atoi(tmps);	break;

		case 11:	// data text  RGB
			symbol->m_tRGB = (DWORD) atof(tmps);	break;

		case 12:	// data paint RGB
			symbol->m_pRGB = (DWORD) atof(tmps);	break;

		case 13:	// data attributes
			symbol->m_attr = getAttributes(tmps, symbol->m_size);
			if (!tmps.IsEmpty())
				symbol->m_edit = tmps;
			break;

		case 14:	// float point
			symbol->m_float = atoi(tmps);		break;

		case 15:	// form kind
			symbol->m_kind = getKind(tmps, tmpn);
			if (tmpn > 0)
				symbol->m_type = tmpn;
			break;

		case 16:	// I/O kind
			symbol->m_iok = getIO(tmps);		break;

		case 17:	// nickname
			symbol->m_nick = tmps;			break;
		}
	}
}

void CSymTbl::makeToken(char ch, CString str, CStringArray& tokArr)
{
	int	offs;
	CString	tmps;

	tokArr.RemoveAll();
	for ( ; !str.IsEmpty(); )
	{
		offs = str.Find(ch);
		if (offs != -1)
		{
			tmps = str.Left(offs);
			str  = (offs+1 < str.GetLength()-1) ? str.Mid(offs+1) : _T("");
		}
		else
		{
			tmps = str;
			str  = _T("");
		}
		tmps.Trim();
		tokArr.Add(tmps);
	}
}

void CSymTbl::str2data(CString& str, CString& data)
{
	int	offs = str.Find(_T(':'));
	if (offs != -1)
	{
		data = str.Mid(offs+1);
		str  = str.Left(offs);
	}
	else
		data = _T("");

	str.Trim();
	data.Trim();
}

CString CSymTbl::getPath(CString fileN)
{
	CString	path;

	path.Format(_T("%s\\tab\\%s"), m_root, fileN);
	return path;
}

CString CSymTbl::parse(CString &src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);
	
	if (pos == -1)
	{
		tmps = src;
		src  = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src  = src.Mid(pos + sub.GetLength());
	return tmps;
}