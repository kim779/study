// preSymbol.cpp : implementation file
//

#include "stdafx.h"
#include "preSymbol.h"
#include "../mapvar.h"
#include "../awsock/wsock.h"
#include "../awWcc/libwcc.h"
#include "../../h/form.h"
#include "../../h/axisvar.h"
#include "../../h/axisrsm.h"
#include "table.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CpreSymbol dialog

CpreSymbol::CpreSymbol(CWnd* frame, bool user)
	: CDialog(CpreSymbol::IDD, frame)
{
	m_user    = user;
	m_state   = stIDLE;
	m_request = reqNONE;
	m_mainFRM = frame;
	m_data    = m_curfile = _T("");
	m_show    = true;
	m_delta   = 0;
	m_clientRc.SetRectEmpty();

	m_wsock     = (CwSock *) NULL;
	m_dragImage = (CImageList *) NULL;

	m_currentP  = -1;
	m_default   = false;
	m_load      = false;

	m_ips  = _T("");
	m_port = -1;
	
	//{{AFX_DATA_INIT(CpreSymbol)
	//}}AFX_DATA_INIT
}

CpreSymbol::~CpreSymbol()
{
	if (m_wsock)
	{
		m_wsock->Close();
		delete m_wsock;
	}

	if (m_dragImage)
		delete m_dragImage;

	removeAll();
}

void CpreSymbol::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CpreSymbol)
	DDX_Control(pDX, IDC_PROJECT, m_combo);
	DDX_Control(pDX, IDC_SYMTREE, m_tree);
	DDX_Control(pDX, IDC_SYMLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CpreSymbol, CDialog)
	//{{AFX_MSG_MAP(CpreSymbol)
	ON_BN_CLICKED(IDC_RECEIVE, OnServer)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SYMTREE, OnTreeSelChanged)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_SYMLIST, OnListBeginDrag)
	ON_BN_CLICKED(IDC_SHOW, OnToggleShow)
	ON_CBN_SELCHANGE(IDC_PROJECT, OnProjectSelChange)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATE, OnState)
	ON_MESSAGE(WM_READDATA, OnReadData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CpreSymbol message handlers

BOOL CpreSymbol::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_UTREE);
	imglist.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
	imglist.Add(&bitmap, RGB(255, 0, 255));
	imglist.SetBkColor(CLR_NONE);
	m_tree.SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();

	if (m_user)
		addColumn("설 명",  0, 150);
	else
	{
		addColumn("Name",   0, 15, false);
		addColumn("설 명",  1, 80);
		addColumn("길 이",  2, 12, false);
	}
	DWORD dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	CButton* radio = (CButton *)GetDlgItem(IDC_R_OUT);
	radio->SetCheck(1);

	m_wsock = new CwSock(this);

	CWinApp* app = AfxGetApp();
	m_root = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
//	loadProject();
//	loadFile();
	m_arSELECT.RemoveAll();

	CRect	tRc, lRc;
	m_list.GetWindowRect(&lRc);
	m_tree.GetWindowRect(&tRc);
	m_delta = lRc.left - tRc.left;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CpreSymbol::OnServer() 
{
	m_currentP = -1;
	m_currentF = 0;
	m_default  = false;
	goState(stCONNECTING);
	if (!m_wsock->Open(m_ips, m_port))
	{
		goState(stIDLE);
		MessageBox("Socket을 open하는 중 Error가 발생하였습니다.", "ERROR", MB_OK);
		return;
	}

	m_SRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_RRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	m_request = reqPROJECT;
	goState(stCONNECTED);

	m_tree.EnableWindow(FALSE);
	m_list.EnableWindow(FALSE);
	m_combo.EnableWindow(FALSE);
	GetDlgItem(IDC_RECEIVE)->EnableWindow(FALSE);

	sendData();
}

void CpreSymbol::OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if (!m_tree.IsWindowEnabled())
		return;

	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	HTREEITEM hitem = pNMTreeView->itemNew.hItem;

	if (hitem == (HTREEITEM) NULL)
	{
		m_list.DeleteAllItems();
		return;
	}

	if (m_tree.GetParentItem(hitem) == (HTREEITEM) NULL)
	{
		GetDlgItem(IDC_SECTION)->SetWindowText("");
		m_list.DeleteAllItems();
		return;
	}

	m_item = hitem;
	CsymbolSET* symSET = (CsymbolSET *)m_tree.GetItemData(hitem);
	fillList(symSET);
}

void CpreSymbol::OnProjectSelChange() 
{
	m_currentF = 0;
	m_currentP = max(0, m_combo.GetCurSel());
	m_default  = m_arSET.GetAt(m_currentP)->m_default;

	GetDlgItem(IDC_R_IN)->EnableWindow(m_default  ? FALSE : TRUE);
	GetDlgItem(IDC_R_IO)->EnableWindow(m_default  ? FALSE : TRUE);
	GetDlgItem(IDC_R_OUT)->EnableWindow(m_default ? FALSE : TRUE);

	fillTree();
}

void CpreSymbol::OnListBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	
	*pResult = 0;
	if (m_list.GetSelectedCount() <= 0)
		return;

	CPoint	pt;
	m_dragImage = createDragImage(&pt);
	if (m_dragImage == (CImageList *)NULL)
		return;

	CPoint	startpt = pNMListView->ptAction;
	startpt -= pt;
	m_mainFRM->SendMessage(WM_STATUS, AWS_SDRAG, (LPARAM) 0);

	m_dragImage->BeginDrag(0, startpt);
	m_dragImage->DragEnter(GetDesktopWindow(), startpt);
	m_mainFRM->SetCapture();
}

void CpreSymbol::OnToggleShow() 
{
	CWnd*	item;
	CRect	rect;
	CString	text;
	int	left, width, delta;
	int	idcX[] = { IDC_SHOW, IDC_SECTION, IDC_FRAME, IDC_R_IN, IDC_R_IO, IDC_R_OUT };

	m_show = !m_show;
	GetWindowRect(&m_clientRc);
	width  = m_clientRc.Width();
	if (m_show)
	{
		delta = m_delta;
		text = ">>";
		m_tree.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RECEIVE)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PROJECT)->ShowWindow(SW_SHOW);
	}
	else
	{
		delta = 0 - m_delta;
		text = "<<";
		m_tree.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECEIVE)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_PROJECT)->ShowWindow(SW_HIDE);
	}

	width += delta;
	left = m_clientRc.right - width;
	m_list.GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	rect.OffsetRect(delta, 0);
	m_list.SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
	for (int ii = 0; ii < sizeof(idcX)/sizeof(int); ii++)
	{
		item = GetDlgItem(idcX[ii]);
		item->GetWindowRect(&rect);
		this->ScreenToClient(&rect);
		rect.OffsetRect(delta, 0);
		item->SetWindowPos(NULL, rect.left, rect.top, rect.Width(), rect.Height(), SWP_NOZORDER);
	}

	GetDlgItem(IDC_SHOW)->SetWindowText(text);
	SetWindowPos(NULL, left, m_clientRc.top, width, m_clientRc.Height(), SWP_NOZORDER);
}

LONG CpreSymbol::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam);

	return 0;
}

LONG CpreSymbol::OnReadData(WPARAM wParam, LPARAM lParam)
{
	int	datL;
	CString	tmps;

	if (m_state != stCONNECTED)
		return 0;

	tmps = CString(m_RRH->datL, sizeof(m_RRH->datL));
	datL = atoi(tmps);
	if (m_RRH->resK == resK_ERR)
	{
		goState(stIDLE);
		MessageBox(&m_wsock->m_recvB[L_rsmH], "ERROR !!", MB_OK);
		m_tree.EnableWindow();
		m_list.EnableWindow();
		m_combo.EnableWindow();
		GetDlgItem(IDC_RECEIVE)->EnableWindow();
		
		return 0;
	}

	if (m_RRH->resC == resC_ASCII)
	{
		switch (m_RRH->resF)
		{
		case resF_FIR:
			if (!m_file.Open(m_curfile, CFile::modeCreate|CFile::modeWrite))
				AfxMessageBox("File Create Fail");
    			else 
			{
				m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
				goState(stRECEIVING);
    			}
			break;

		case resF_MID:
			m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
			goState(stRECEIVING);
			break;

		case resF_LAS:
			m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
			m_file.Close();
			goState(stCONNECTED);
			break;

		case resF_ONLY :
			if (!m_file.Open(m_curfile, CFile::modeCreate|CFile::modeWrite))
				AfxMessageBox("File Create Fail");
			else
			{
				m_file.Write(&m_wsock->m_recvB[L_rsmH], datL);
				m_file.Close();
				goState(stCONNECTED);
			}
			break;
		}

		if (m_RRH->resF == resF_FIR || m_RRH->resF == resF_MID)
		{
			m_SRH->resK = resK_RSP;
			sendData();
		}
		else
		{
			switch (m_request)
			{
			case reqPROJECT:
				loadProject();
				if (m_arSET.GetSize() > 0)
				{
					m_request = reqFILE;
					sendData();
					break;
				}
				goState(stIDLE);
				break;
			case reqFILE:
				if (m_currentP < 0)
					break;
			
				Cproject* project;
				project = m_arSET.GetAt(m_currentP);
				if (m_currentP < m_arSET.GetSize())
				{
					m_currentF++;
					if (m_currentF >= project->m_arPRO.GetSize())
					{
						m_currentP++;
						if (m_currentP >= m_arSET.GetSize())
						{
							m_currentP = m_currentF = 0;
							loadFile();
							goState(stIDLE);
							break;
						}
						project = m_arSET.GetAt(m_currentP);
						m_currentF = 0;
					}

					if (m_currentF < project->m_arPRO.GetSize())
					{
						sendData();
						break;
					}
				}
				m_currentP = m_currentF = 0;
				m_default  = false;
				loadFile();
			case reqNONE:
				goState(stIDLE);
				break;
			}
		}
	}

	return 0;
}

void CpreSymbol::SetData(CString data)
{
	CsymbolSET* symSET;

	if (m_currentP < 0)
		return;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		Cproject* project = m_arSET.GetAt(ii);
		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
		{
			Croot* root = project->m_arPRO.GetAt(jj);
			for (int kk = 0; kk < root->m_arROOT.GetSize(); kk++)
			{
				Cgroup* group = root->m_arROOT.GetAt(kk);
				for (int ll = 0; ll < group->m_arGROUP.GetSize(); ll++)
				{
					symSET = group->m_arGROUP.GetAt(ll);
					if (!data.CompareNoCase(symSET->m_data))
						break;
				}
				if (ll < group->m_arGROUP.GetSize())
					break;
			}
			if (kk < root->m_arROOT.GetSize())
				break;
		}
		if (jj < project->m_arPRO.GetSize())
			break;
	}

	if (ii >= m_arSET.GetSize())
		return;

	m_currentP = ii;
	m_default  = m_arSET.GetAt(m_currentP)->m_default;
	fillTree();

	bool	find = false;
	TVITEM	tvi;
	HTREEITEM childItem, item = m_tree.GetRootItem();

	while (item != NULL)
	{
		if (m_tree.ItemHasChildren(item))
		{
			childItem = m_tree.GetChildItem(item);
			while (childItem != NULL)
			{
				tvi.mask   = TVIF_HANDLE | TVIF_PARAM;
				tvi.hItem  = childItem;
				tvi.lParam = 0;

				m_tree.GetItem(&tvi);
				if (tvi.lParam = (DWORD) symSET)
				{
					find = true;
					break;
				}

				childItem  = m_tree.GetNextItem(childItem, TVGN_NEXT);
			}
		}

		if (find)
			break;

		item = m_tree.GetNextSiblingItem(item);
	}

	if (find)
	{
		m_item = childItem;
		fillList((CsymbolSET *)m_tree.GetItemData(m_item));
		m_tree.SelectSetFirstVisible(m_item);
		m_tree.Select(m_item, TVGN_CARET);
	}
}

int CpreSymbol::GetSelectedCount()
{
	CsymbolSET*	symSET;
	BYTE	iok   = getIOKind();
	int	count = m_list.GetItemCount();

	m_arSELECT.RemoveAll();
	symSET = (CsymbolSET *)m_tree.GetItemData(m_item);
	for (int ii = 0; ii < count; ii++)
	{
		if (m_list.GetItemState(ii, LVIS_SELECTED) == LVIS_SELECTED)
		{
			if (!m_default)
				symSET->m_arSYM.GetAt(ii)->m_iok = iok;
			else if (m_default && symSET->m_arSYM.GetAt(ii)->m_iok <= 0)
				continue;

			m_arSELECT.Add(symSET->m_arSYM.GetAt(ii));
		}
	}

	count = min(m_arSELECT.GetSize(), (int)m_list.GetSelectedCount());
	return count;
}

Csymbol* CpreSymbol::GetSelectedData(int index)
{
	if (index >= m_arSELECT.GetSize())
		return (Csymbol *) NULL;

	return m_arSELECT.GetAt(index);
}

void CpreSymbol::Load()
{
	if (m_load)
		return;

	GetDlgItem(IDC_MSG)->ShowWindow(SW_SHOW);
	loadProject();
	loadFile();
	GetDlgItem(IDC_MSG)->ShowWindow(SW_HIDE);
	m_arSELECT.RemoveAll();

	m_load = true;
}

void CpreSymbol::SetConnectInfo(CString ips, int port)
{
	m_ips  = ips;
	m_port = port;
}

bool CpreSymbol::sendData()
{
	CString	file;

	m_SRH->dirF = dirF_OUTB;
	if (m_SRH->resK != resK_RSP)
		m_SRH->resK = resK_SYMB;
	m_SRH->resC = m_RRH->resC;
	m_SRH->resF = m_RRH->resF;

	switch (m_request)
	{
	case reqPROJECT:
		m_SRH->resC = resC_ASCII;
		m_SRH->resF = resF_ONLY;
		file = PROJECT_INI;
		break;
	case reqFILE:
		if (m_currentP < 0)
			return false;
		file = m_arSET.GetAt(m_currentP)->m_arPRO.GetAt(m_currentF)->m_file;
		break;
	case reqNONE:
		return false;
	}
	m_curfile = getPath(file);
	strcpy(m_SRH->resN, file);
	wccToBUFFER(m_SRH->datL, sizeof(m_SRH->datL), 0);
	if (!m_wsock->Write(L_rsmH))
	{
		goState(stIDLE);
		return false;
	}

	return true;
}

void CpreSymbol::loadProject()
{
	char	wb[256];
	CString	tmps, path;
	Cproject*	project;
	CStringArray	strarr;

	removeAll();
	m_combo.ResetContent();
	m_tree.DeleteAllItems();
	m_list.DeleteAllItems();

	m_item     = (HTREEITEM) NULL;
	m_currentP = -1;
	m_default  = false;

	path = getPath(PROJECT_INI);
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

void CpreSymbol::loadProject(Cproject* project, CString path)
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

void CpreSymbol::loadFile()
{
	Cproject*	project;
	
	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		project = m_arSET.GetAt(ii);
		m_combo.AddString(project->m_name);
		for (int jj = 0; jj < project->m_arPRO.GetSize(); jj++)
			loadFile(project->m_arPRO.GetAt(jj));
	}

	fillTree();

	m_tree.EnableWindow();
	m_list.EnableWindow();
	m_combo.EnableWindow();
	GetDlgItem(IDC_RECEIVE)->EnableWindow();

	if (m_currentP >= 0)
	{
		m_combo.SetCurSel(m_currentP);
		m_default = m_arSET.GetAt(m_currentP)->m_default;
		if (m_default)
		{
			GetDlgItem(IDC_R_IN)->EnableWindow(m_default  ? FALSE : TRUE);
			GetDlgItem(IDC_R_IO)->EnableWindow(m_default  ? FALSE : TRUE);
			GetDlgItem(IDC_R_OUT)->EnableWindow(m_default ? FALSE : TRUE);
		}
	}

//SetData("DJDHOGA");	manie
}

void CpreSymbol::loadFile(Croot* root)
{
	char	wb[256];
	CString	tmps, path;
	Csymbol*	symbol;
	CsymbolSET*	symSET;
	Cgroup*		group;
	CStringArray	strarr;

	path = getPath(root->m_file);
	for (int ii = 1; ; ii++)
	{
		tmps.Format("%03d", ii);
		GetPrivateProfileString("General", tmps, "", wb, sizeof(wb), path);
		tmps = wb;
		if (tmps.IsEmpty())
			break;

		group = new Cgroup;
		group->m_name = tmps;
		str2data(group->m_name, group->m_data);

		for (int jj = 1; ; jj++)
		{
			tmps.Format("%03d", jj);
			GetPrivateProfileString(group->m_data, tmps, "", wb, sizeof(wb), path);
			tmps = wb;
			if (tmps.IsEmpty())
				break;

			symSET = new CsymbolSET;
			symSET->m_name = tmps;
			str2data(symSET->m_name, symSET->m_data);

			for (int kk = 1; ; kk++)
			{
				tmps.Format("%03d", kk);
				GetPrivateProfileString(symSET->m_data, tmps, "", wb, sizeof(wb), path);
				tmps = wb;
				if (tmps.IsEmpty())
					break;

				symbol = new Csymbol;
				strarr.RemoveAll();
				makeToken(',', tmps, strarr);
				parse(symbol, strarr);

				symSET->m_arSYM.Add(symbol);
			}
			group->m_arGROUP.Add(symSET);
		}
		root->m_arROOT.Add(group);
	}
}

void CpreSymbol::removeAll()
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

void CpreSymbol::makeToken(char ch, CString str, CStringArray& tokArr)
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

void CpreSymbol::parse(Csymbol* symbol, CStringArray& tokArr)
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

void CpreSymbol::trim(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
}

void CpreSymbol::str2data(CString& str, CString& data)
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

BYTE CpreSymbol::getType(CString str)
{
	for (int ii = 0; ii < TYPETBL; ii++)
	{
		if (!str.CompareNoCase(typeTbl[ii].strName))
			return typeTbl[ii].type;
	}

	return IO_NUM;
}

BYTE CpreSymbol::getAlignment(CString str)
{
	for (int ii = 0; ii < ALIGNTBL; ii++)
	{
		if (!str.CompareNoCase(alignTbl[ii].strName))
			return alignTbl[ii].align;
	}

	return AL_LEFT;
}

DWORD CpreSymbol::getAttributes(CString& str, int size)
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

CString CpreSymbol::getMask(int type, int size)
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

BYTE CpreSymbol::getKind(CString str, int& type)
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

BYTE CpreSymbol::getIO(CString str)
{
	for (int ii = 0; ii < IOTBL; ii++)
	{
		if (!str.CompareNoCase(ioTbl[ii].strName))
			return ioTbl[ii].iok;
	}

	return 0;
}

BOOL CpreSymbol::addColumn(LPCTSTR str, int item, int width, bool align_left)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = align_left ? LVCFMT_LEFT : LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = m_list.GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(item, &lvc);
}

BOOL CpreSymbol::addItem(int item, int subItem, CString& str)
{
        LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();
        if (subItem == 0)
                return m_list.InsertItem(&lvi);

        return m_list.SetItem(&lvi);
}

void CpreSymbol::fillTree()
{
	Cproject*   project;
	Croot*      root;
	Cgroup*     group;
	CsymbolSET* symSET;
	HTREEITEM   hitem, item;

	if (m_currentP < 0)
		return;

	m_tree.DeleteAllItems();
	m_tree.Invalidate();
	m_tree.UpdateWindow();

	project = m_arSET.GetAt(m_currentP);
	for (int ii = 0; ii < project->m_arPRO.GetSize(); ii++)
	{
		root = project->m_arPRO.GetAt(ii);
		for (int jj = 0; jj < root->m_arROOT.GetSize(); jj++)
		{
			group = root->m_arROOT.GetAt(jj);
			hitem = m_tree.InsertItem(group->m_name, 0, 1);
			m_tree.SetItemData(hitem, (DWORD)group);

			for (int kk = 0; kk < group->m_arGROUP.GetSize(); kk++)
			{
				symSET = group->m_arGROUP.GetAt(kk);
				item = m_tree.InsertItem(symSET->m_name, 2, 3, hitem);
				m_tree.SetItemData(item, (DWORD)symSET);
				if (!m_item)
				{
					if (m_data.IsEmpty() || (!m_data.IsEmpty() && !m_data.CompareNoCase(symSET->m_data)))
					{
						m_item = item;
						fillList(symSET);
						continue;
					}
				}
			}
		}
	}

	if (m_item != (HTREEITEM) NULL)
	{
		m_tree.SelectSetFirstVisible(m_item);
		m_tree.Select(m_item, TVGN_CARET);
	}
}

void CpreSymbol::fillList(CsymbolSET* symSET)
{
	Cgroup*   group;
	Csymbol*  symbol;
	HTREEITEM hitem;
	int	item  = 0;
	CString	items = _T("");

	hitem = m_tree.GetParentItem(m_item);
	if (hitem != (HTREEITEM) NULL)
	{
		group = (Cgroup *)m_tree.GetItemData(hitem);
		items = _T(" ") + m_arSET.GetAt(m_currentF)->m_name + " / " + group->m_name + " / " + symSET->m_name;
		GetDlgItem(IDC_SECTION)->SetWindowText(items);
	}

	m_list.DeleteAllItems();
	for (int ii = 0; ii < symSET->m_arSYM.GetSize(); ii++, item++)
	{
		symbol = symSET->m_arSYM.GetAt(ii);
		if (m_user)
		{
			items = symbol->m_head;
			addItem(item, 0, items);
		}
		else
		{
			items = symbol->m_name;
			addItem(item, 0, items);
			items = symbol->m_head;
			addItem(item, 1, items);
			items.Format("%d", symbol->m_size);
			addItem(item, 2, items);
		}
	}
}

CImageList* CpreSymbol::createDragImage(LPPOINT lpPoint)
{
	if (m_list.GetSelectedCount() <= 0)
		return NULL;

	CRect	singleRc, completeRc;
	int	width, itop, ibottom;

	m_list.GetClientRect(singleRc);
	completeRc.SetRectEmpty();
	width = singleRc.Width();
	itop  = m_list.GetTopIndex() - 1;
	ibottom = m_list.GetTopIndex() + m_list.GetCountPerPage() - 1;

	if (ibottom > (m_list.GetItemCount() - 1))
		ibottom = m_list.GetItemCount()-1;

	while ((itop = m_list.GetNextItem(itop, LVNI_SELECTED)) != -1)
	{
		if (itop > ibottom)
			break; 

		m_list.GetItemRect(itop, singleRc, LVIR_BOUNDS);
		singleRc.left  = max(0, singleRc.left);
		singleRc.right = min(width, singleRc.right);
		completeRc.UnionRect(completeRc, singleRc);
	}
		
	CClientDC dcClient(this);
	CDC	memDC;
	CBitmap bitmap;

	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, completeRc.Width(), completeRc.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
 	memDC.FillSolidRect(0, 0, completeRc.Width(), completeRc.Height(), RGB(255, 255,0));

	itop = m_list.GetTopIndex() - 1;
	while ((itop = m_list.GetNextItem(itop, LVNI_SELECTED)) != -1)
	{	
		if (itop > ibottom)
			return NULL;

		CPoint	pt;
		CImageList* pSingleImageList = m_list.CreateDragImage(itop, &pt);
		if (pSingleImageList)
		{
			m_list.GetItemRect(itop, singleRc, LVIR_BOUNDS);
			pSingleImageList->Draw(&memDC, 0, CPoint(singleRc.left - completeRc.left,
						singleRc.top - completeRc.top), ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
		}
	}

 	memDC.SelectObject(pOldMemDCBitmap);
	CImageList* pCompleteImageList = new CImageList;
	pCompleteImageList->Create(completeRc.Width(), completeRc.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = completeRc.left;
		lpPoint->y = completeRc.top;
	}

	return pCompleteImageList;
}

void CpreSymbol::goState(int state, CString msg)
{
	CString	status = " STATUS : ";
	m_state = state;

	switch (m_state)
	{
	case stADDRESS:
		status += "주소가 입력되지 않았습니다.";
		break;
	case stCONNECTING:
		status += "연결중입니다.";
		break;
	case stCONNECTED:
		status += "연결되었습니다.";
		break;
	case stWAIT:
		m_state = stCONNECTED;
		break;
	case stRECEIVING:
		m_state = stCONNECTED;
		status += "Data를 Download 받고 있습니다.";
		break;
	case stIDLE:
	default:
		m_request = reqNONE;
		m_wsock->Close();
		status += "연결이 해제되었습니다.";
		break;
	}

	if (!msg.IsEmpty())
		status = msg;
}

CString CpreSymbol::getPath(CString fileN)
{
	CString	path;
	path.Format("%s\\%s\\%s", m_root, TABDIR, fileN);

	return path;
}

BYTE CpreSymbol::getIOKind()
{
	CButton* radio = (CButton *)GetDlgItem(IDC_R_IN);
	if (radio->GetCheck() == 1)
		return EIO_INPUT;
	
	radio = (CButton *)GetDlgItem(IDC_R_IO);
	if (radio->GetCheck() == 1)
		return EIO_INOUT;
	
	radio = (CButton *)GetDlgItem(IDC_R_OUT);
	if (radio->GetCheck() == 1)
		return EIO_OUTPUT;

	return EIO_OUTPUT;
}
