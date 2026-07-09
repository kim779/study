// SymTbl.cpp: implementation of the CSymTbl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SymTbl.h"
#include "../../h/form.h"
#include "../../h/axisvar.h"
#include "table.hxx"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSymTbl::CSymTbl()
{
	m_bOffline = false;
	m_bReceive = false;
}

CSymTbl::~CSymTbl()
{
	removeAll();
}

BEGIN_MESSAGE_MAP(CSymTbl, CWnd)
	//{{AFX_MSG_MAP(CSymTbl)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSymTbl::loadProject()
{
	char	wb[256];
	CString	tmps, path;
	Cproject*	project;
	CStringArray	strarr;
	CFileFind	finder;

	removeAll();

	m_currentP = -1;
	m_default  = false;

	path = getPath(PROJECT_INI);
	if ((!finder.FindFile(path) && !m_bOffline) || m_bReceive)
	{
		m_pDownLoad = new CDownLoad();
		
		m_pDownLoad->downLoad("work/project.ini", path, 1);
		delete m_pDownLoad;
	}
	
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%03d", ii);
		GetPrivateProfileString("General", tmps, "", wb, sizeof(wb), path);
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
	{
		m_currentP = m_currentF = 0;
		m_default  = m_arSET.GetAt(m_currentP)->m_default;
	}
}

CString CSymTbl::getPath(CString fileN)
{
	CString	path;
	path.Format("%s\\%s\\%s", m_root, TABDIR, fileN);

	return path;
}

int CSymTbl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CSymTbl::str2data(CString &str, CString &data)
{
	int	offs = str.Find(':');
	if (offs != -1)
	{
		data = str.Mid(offs+1);
		str  = str.Left(offs);
	}
	else
		data = _T("");

	trim(str);
	trim(data);
}

void CSymTbl::loadProject(Cproject *project, CString path)
{
	char	wb[256];
	CString	tmps;
	Croot*		root;
	CStringArray	strarr;

	for (int ii = 1; ; ii++)
	{
		tmps.Format("%03d", ii);
		GetPrivateProfileString(project->m_section, tmps, "", wb, sizeof(wb), path);
		tmps = wb;
		if (tmps.IsEmpty())
			break;

		root = new Croot;
		root->m_name = tmps;
		str2data(root->m_name, root->m_file);
		project->m_arPRO.Add(root);
	}
}

void CSymTbl::trim(CString &str)
{
	str.TrimLeft();
	str.TrimRight();
}

void CSymTbl::loadFile()
{
	Cproject*	project;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		project = m_arSET.GetAt(ii);
		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
			loadFile(project->m_arPRO.GetAt(jj));
	}
}

void CSymTbl::loadFile(Croot *root)
{
	char buf[1025];
	CString	sData = "";
	CString path = getPath(root->m_file);

	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	CStringArray	strarr;
	CFileFind	finder;

	if ((!finder.FindFile(path) && !m_bOffline) || m_bReceive)
	{
		CString tmps;
		m_pDownLoad = new CDownLoad();
		tmps.Format("work/%s", root->m_file);
		m_pDownLoad->downLoad(tmps, path, 1);
		delete m_pDownLoad;
	}

	TRY
	{
		CFile file(path, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(buf, 1024);
			sData += CString(buf, nBytesRead);
		}while((int)nBytesRead);
		file.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	group = new Cgroup;
	group->m_name = "Çö¹°:KPHM";
	str2data(group->m_name, group->m_data);
	root->m_arROOT.Add(group);
	
	while(!sData.IsEmpty())
	{
		CString sToken = Parser(sData, "\n");
		if (sToken.IsEmpty())
			continue;
		if (sToken.Find("[") >= 0 && sToken.Find("]") >= 0)
		{
			sToken.Replace("[", "");
			sToken.Replace("]", "");
			symSET = new CsymbolSET;
			symSET->m_name = sToken;
			str2data(symSET->m_name, symSET->m_data);
			group->m_arGROUP.Add(symSET);
		}
		else if (sToken.Find(";") >= 0)
		{
		}
		else if (sToken.Find(",") >= 0)
		{
			symbol = new Csymbol;
			strarr.RemoveAll();
			makeToken(',', sToken, strarr);
			parse(symbol, strarr);
			
			symSET->m_arSYM.Add(symbol);
		}
	}
}

void CSymTbl::makeToken(char ch, CString str, CStringArray &tokArr)
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

		trim(tmps);
		tokArr.Add(tmps);
	}
}

void CSymTbl::parse(Csymbol *symbol, CStringArray &tokArr)
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

BYTE CSymTbl::getType(CString str)
{
	for (int ii = 0; ii < TYPETBL; ii++)
	{
		if (!str.CompareNoCase(typeTbl[ii].strName))
			return typeTbl[ii].type;
	}

	return IO_NUM;
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

DWORD CSymTbl::getAttributes(CString &str, int size)
{
	CString	mask = _T("");
	DWORD	attr = 0;
	CStringArray	strArr;

	makeToken('|', str, strArr);
	for (int ii = 0; ii < strArr.GetSize(); ii++)
	{
		str = strArr.GetAt(ii);
		for (int jj = 0; jj < ATTRTBL; jj++)
		{
			if (!str.CompareNoCase(attrTbl[jj].strName))
			{
				switch (attrTbl[jj].attr)
				{
				case EMASK_DATE:
					attr |= FA_DATE;
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

BYTE CSymTbl::getKind(CString str, int &type)
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

BYTE CSymTbl::getIO(CString str)
{
	for (int ii = 0; ii < IOTBL; ii++)
	{
		if (!str.CompareNoCase(ioTbl[ii].strName))
			return ioTbl[ii].iok;
	}

	return 0;
}


void CSymTbl::fillList(CListCtrl *LC, CsymbolSET *symSET)
{
	Csymbol*  symbol;
	int	item  = 0;
	CString	items = _T("");
	m_pCurrentSymSET = symSET;

	LC->DeleteAllItems();
	for (int ii = 0; ii < symSET->m_arSYM.GetSize(); ii++, item++)
	{
		symbol = symSET->m_arSYM.GetAt(ii);
		
		//items = symbol->m_head;
		addItem(LC, item, 0, symbol->m_head);
		addItem(LC, item, 1, symbol->m_name);
	}
}


BOOL CSymTbl::addItem(CListCtrl *LC, int item, int subItem, CString &str)
{
	LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();
        if (subItem == 0)
                return LC->InsertItem(&lvi);

        return LC->SetItem(&lvi);
}

void CSymTbl::removeAll()
{
	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	Croot*		root;
	Cproject*	project;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		project = m_arSET.GetAt(ii);
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

void CSymTbl::fillCombo(Cproject *project, CComboBox *grpCBO)
{
	CsymbolSET*	symSET;
	Cgroup*		group;
	Croot*		root;
	grpCBO->ResetContent();

	for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
	{
		root = project->m_arPRO.GetAt(jj);
		for (int kk = 0; kk < root->m_arROOT.GetSize(); kk++)
		{
			group = root->m_arROOT.GetAt(kk);
			for (int ll = 0; ll < group->m_arGROUP.GetSize(); ll++)
			{
				symSET = group->m_arGROUP.GetAt(ll);
				grpCBO->SetItemData(grpCBO->AddString(symSET->m_name), (long)symSET);
			}
		}
	}
}

void CSymTbl::fillCombo(CComboBox *prjCBO)
{
	prjCBO->ResetContent();
	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		Cproject*	project = m_arSET.GetAt(ii);
		prjCBO->SetItemData(prjCBO->AddString(project->m_name), (long)project);
	}
}

int CSymTbl::GetSelectedCount(CListCtrl *LC, BYTE iok)
{
	CsymbolSET*	symSET;
	int	count = LC->GetItemCount();

	m_arSELECT.RemoveAll();
	symSET = m_pCurrentSymSET;
	for (int ii = 0; ii < count; ii++)
	{
		if (LC->GetItemState(ii, LVIS_SELECTED) == LVIS_SELECTED)
		{
			if (!m_default)
				symSET->m_arSYM.GetAt(ii)->m_iok = iok;
			else if (m_default && symSET->m_arSYM.GetAt(ii)->m_iok <= 0)
				continue;

			m_arSELECT.Add(symSET->m_arSYM.GetAt(ii));
		}
	}

	count = min(m_arSELECT.GetSize(), (int)LC->GetSelectedCount());
	return count;
}

Csymbol* CSymTbl::GetSelectedData(int index)
{
	if (index >= m_arSELECT.GetSize())
		return (Csymbol *) NULL;

	return m_arSELECT.GetAt(index);
}

CString CSymTbl::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
			return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

int CSymTbl::GetSelectedCount(CTreeCtrl *TC, BYTE iok)
{
	if (!TC || !::IsWindow(TC->m_hWnd))
		return 0;

	CsymbolSET*	symSET;
	HTREEITEM hitem = getFirstSelectedItem(TC);

	if (hitem == (HTREEITEM) NULL)
		return 0;

	m_arSELECT.RemoveAll();
	if (!TC->ItemHasChildren(hitem))
	{
		symSET = (CsymbolSET*)TC->GetItemData(TC->GetParentItem(hitem));
		int nIdx = TC->GetItemData(hitem);
		if (!m_default)
			symSET->m_arSYM.GetAt(nIdx)->m_iok = iok;
		else if (m_default && symSET->m_arSYM.GetAt(nIdx)->m_iok <= 0)
			nIdx = -1;
		if (nIdx >= 0)
			m_arSELECT.Add(symSET->m_arSYM.GetAt(nIdx));
	}
	for (hitem = getNextSelectedItem(TC, hitem); hitem != NULL; hitem = getNextSelectedItem(TC, hitem))
	{
		if (hitem == (HTREEITEM) NULL)
			break;

		if (TC->ItemHasChildren(hitem))
			continue;

		symSET = (CsymbolSET*)TC->GetItemData(TC->GetParentItem(hitem));
		int nIdx = TC->GetItemData(hitem);
		if (!m_default)
			symSET->m_arSYM.GetAt(nIdx)->m_iok = iok;
		else if (m_default && symSET->m_arSYM.GetAt(nIdx)->m_iok <= 0)
			continue;

		m_arSELECT.Add(symSET->m_arSYM.GetAt(nIdx));
	}
	return m_arSELECT.GetSize();
}

HTREEITEM CSymTbl::getFirstSelectedItem(CTreeCtrl *TC)
{
	for (HTREEITEM hitem = TC->GetRootItem(); hitem != NULL; hitem = TC->GetNextVisibleItem(hitem))
	{
		if (TC->GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

HTREEITEM CSymTbl::getNextSelectedItem(CTreeCtrl *TC, HTREEITEM hitem)
{
	for (hitem = TC->GetNextVisibleItem(hitem); hitem != NULL; hitem = TC->GetNextVisibleItem(hitem))
	{
		if (TC->GetItemState(hitem, TVIS_SELECTED) & TVIS_SELECTED)
			return hitem;
	}
	return (HTREEITEM) NULL;
}

void CSymTbl::fillTree(CTreeCtrl *TC)
{
	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	Croot*		root;
	Cproject*	project;

	TC->DeleteAllItems();
	HTREEITEM	hItemProject;
	HTREEITEM	hItemGroup;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		project = m_arSET.GetAt(ii);
		hItemProject = TC->InsertItem(project->m_name, 0, 0);
		TC->SetItemData(hItemProject, (long)project);

		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
		{
			root = project->m_arPRO.GetAt(jj);
			for (int kk = 0; kk < root->m_arROOT.GetSize(); kk++)
			{
				group = root->m_arROOT.GetAt(kk);

				for (int ll = 0; ll < group->m_arGROUP.GetSize(); ll++)
				{
					symSET = group->m_arGROUP.GetAt(ll);
					hItemGroup = TC->InsertItem(symSET->m_name, 0, 0, hItemProject);
					TC->SetItemData(hItemGroup, (long)symSET);

					for (int mm = 0; mm < symSET->m_arSYM.GetSize(); mm++)
					{
						symbol = symSET->m_arSYM.GetAt(mm);
						HTREEITEM hItem = TC->InsertItem(symbol->m_head + "  (" + symbol->m_name + ")", 0, 0, hItemGroup);
						TC->SetItemData(hItem, mm);
						TC->SetItemImage(hItem, 3, 3);
					}
				}
			}
		}
	}
}
