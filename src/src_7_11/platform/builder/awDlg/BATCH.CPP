// batch.cpp : implementation file
//

#include "stdafx.h"
#include "batch.h"
#include "../awSock/wsock.h"
#include "../../h/axisrsm.h"
#include "../../h/axisvar.h"
#include "../awWcc/libwcc.h"
#include "../awBuild/libBuild.h"
#include "../awUser/DownLoad.h"
#include "../awUser/UpLoad.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	resC_EXPORT	0x80
#define	R_NONE		-1
#define R_SUB		1

/////////////////////////////////////////////////////////////////////////////
// CBatch dialog

CBatch::CBatch(CWnd* pParent, CString dir)
	: CDialog(CBatch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBatch)
	m_status = _T("");
	m_bExport = FALSE;
	//}}AFX_DATA_INIT

	m_dir     = dir;
	m_state   = stIDLE;
	m_current = _T("");
	m_files   = _T("");

	m_proc = sndINFO;
	m_item  = (HTREEITEM) NULL;

	m_source = true;
	m_offset = 0;
	m_sendL  = 0;
	m_hfile  = (HANDLE) NULL;

	m_bBatching = false;
}

CBatch::~CBatch()
{

}

void CBatch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBatch)
	DDX_Control(pDX, IDC_ERR, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Check(pDX, IDC_EXPORT, m_bExport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBatch, CDialog)
	//{{AFX_MSG_MAP(CBatch)
	ON_BN_CLICKED(IDC_BATCH, OnBatch)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnTreeDblClick)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnTreeSelChanged)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATE, OnState)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBatch message handlers

BOOL CBatch::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CImageList	imglist;
	CBitmap		bitmap;

	bitmap.LoadBitmap(IDB_TREE);
	imglist.Create(16, 16, ILC_COLORDDB|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(255, 0, 255));
	imglist.SetBkColor(CLR_NONE);
	m_tree.SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();

	addColumn("Group",  0, 15);
	addColumn("Name",  1, 80);

	DWORD dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	CString tmps;
	m_pDownLoad = new CDownLoad();
	wccSetTempPath(tmps, DN_FILE);
	m_pDownLoad->downLoad("", tmps, 2);
	delete m_pDownLoad;
	loadFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBatch::OnBatch() 
{
	if (m_item == (HTREEITEM) NULL)
	{
		goState(stWAIT);
		return;
	}

	UpdateData();
	m_bBatching = true;
	m_downArray.RemoveAll();
	DWORD nItemData = m_tree.GetItemData(m_item);
	CString xx = m_tree.GetItemText(m_item);
	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		if (!xx.Compare(CString(m_treeArr.GetAt(ii).mapn, (nItemData==R_NONE?2:3))))
			m_downArray.Add(m_treeArr.GetAt(ii).mapn);
	}

	m_list.DeleteAllItems();
	m_tree.EnableWindow(FALSE);
	//GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->SetWindowText("Stop");
	GetDlgItem(IDC_BATCH)->EnableWindow(FALSE);

	sortDownArray();

	for (int ii = 0; ii < m_downArray.GetSize(); ii++)
	{
		CString mapName = m_downArray.GetAt(ii), tmps, svrMap;
		goState(stIDLE, mapName + " Downloading...");
		svrMap.Format("map/%s/%s/%s", mapName.Left(2), mapName.Left(3), mapName);
		
		// Download
		m_pDownLoad = new CDownLoad();
		wccSetTempPath(tmps, (LPSTR)mapName.operator LPCTSTR());
		m_pDownLoad->downLoad(svrMap, tmps, 0);
		delete m_pDownLoad;

		m_files = mapName;
		goState(stIDLE, mapName + " Compiling...");
		if (compile())
		{
			goState(stIDLE, mapName + " Uploading...");

			// upload
			m_pUpLoad = new CUpLoad();
			m_pUpLoad->m_bExport = (m_bExport?true:false);
			m_pUpLoad->upLoad(tmps, svrMap, 1/*W_BIN*/, false, &m_mapH);
			delete m_pUpLoad;
			goState(stIDLE, mapName + " Completed...");
		}
		if (!m_bBatching)
			break;
	}
	if (m_bBatching)
		goState(stCOMPLETE);
	else
		goState(stSTOP);
}

void CBatch::OnOK() 
{
	if (m_bBatching)
	{
		m_bBatching = false;
		GetDlgItem(IDOK)->SetWindowText("Close");
		GetDlgItem(IDC_BATCH)->SetFocus();
		return;
	}
	CDialog::OnOK();
}

void CBatch::OnTreeDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	m_item = m_tree.GetSelectedItem();
	if (m_tree.GetItemData(m_item) == R_SUB)
		OnBatch();
	*pResult = 0;
}


void CBatch::OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	m_item = m_tree.GetSelectedItem();
	*pResult = 0;
}

LONG CBatch::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam, msg);
	return 0;
}

void CBatch::loadFile()
{
	CString	dir = m_dir + "\\" + DN_FILE;
	struct _mapTree	mapTree;
	m_treeArr.RemoveAll();
	
	TRY
	{
		CFile file(dir, CFile::modeRead);
		UINT nBytesRead;
		do
		{
			nBytesRead = file.Read(&mapTree, sizeof(mapTree));
			if (nBytesRead == sizeof(mapTree))
				m_treeArr.Add(mapTree);
		}while((int)nBytesRead);
		file.Close();
	}CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		insert(m_treeArr.GetAt(ii).mapn);
	}
	m_tree.SortChildren(NULL);
}

void CBatch::insert(CString mapName)
{
	HTREEITEM	hitem1, hitem2;
	hitem1 = GetItemByName(NULL, mapName.Mid(0, 2));
	if (!hitem1)
	{
		hitem1 = m_tree.InsertItem(mapName.Mid(0, 2), 0, 1);
		m_tree.SetItemData(hitem1, R_NONE);
	}
	hitem2 = GetItemByName(hitem1, mapName.Mid(0, 3));
	if (!hitem2)
	{
		hitem2 = m_tree.InsertItem(mapName.Mid(0, 3), 0, 1, hitem1);
		m_tree.SetItemData(hitem2, R_SUB);
		m_tree.SortChildren(hitem1);
	}
}

void CBatch::goState(int state, CString msg)
{
	m_state  = state;
	m_status = _T("  ");
	switch (m_state)
	{
	case stADDRESS:
		m_status += "None of IP Address";
		break;

	case stCONNECTING:
		m_status += "Connecting";
		break;
	
	case stCONNECTED:
		m_status += "Connected.";
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;
	
	case stWAIT:
		m_state = stCONNECTED;
		m_status += "Set Path.";
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;
	
	case stRECEIVING:
		m_state = stCONNECTED;
		break;

	case stCOMPLETE:
		m_status += "Batch Completed.";
		m_state = stCONNECTED;
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->SetWindowText("Close");
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;

	case stSTOP:
		m_status += "Stopped by user...";
		m_state = stCONNECTED;
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;
	case stIDLE:
	default:
		m_status += "Disconnected";
		break;
	}

	if (!msg.IsEmpty())
		m_status = msg;

	SetDlgItemText(IDC_STATUS, m_status);
}

BOOL CBatch::addColumn(LPCTSTR str, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_LEFT;
	lvc.pszText  = (LPTSTR) str;
	lvc.cx       = m_list.GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(item, &lvc);
}

BOOL CBatch::addItem(int item, int subItem, CString& str)
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

bool CBatch::open(CString fileN)
{
	int	totL, tokcnt;
	int	formN, repN;
	WORD	type;

	totL = wccLoadSource(fileN.operator LPCTSTR(), (CWnd *) NULL);
	if (totL == -1)
		return false;

	ZeroMemory((char *)&m_mapH, sizeof(_mapH));
	m_mapH.mapGroup = 1;
	CString sName = fileN.Mid(fileN.ReverseFind('\\') + 1);
	strcpy_s(m_mapH.mapN, sName);
	strcpy_s(m_mapH.mapdesc, "xx");

	m_mapH.formN = 0;
	m_mapH.pathsetted = false;
	m_mapH.modified   = false;
	m_mapH.source = m_mapH.compile = false;

	formN = 0; repN = -1;
	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_token[ii].Clear();

	load(&m_mapH, m_dir);
	for (int ii = 0; ii < totL; ii++)
	{
		tokcnt = wccParseToken(m_token, ii, &type);
		switch (type)
		{
		case TYPE_GLOBAL:
			buildHeader(m_token, tokcnt);
			break;
		case TYPE_FORM:
			buildForm(m_token, tokcnt, formN);
			formN++;
			break;
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
		default:
			break;
		}

		for (int jj = 0; jj < tokcnt; jj++)
			m_token[jj].Clear();
	}

	buildTab();

	m_mapH.onStart   = false;
	m_mapH.onSend    = false;
	m_mapH.onReceive = false;
	m_mapH.onAlert   = false;
	m_mapH.onService = false;
	m_mapH.onFile    = false;
	m_mapH.onSelect  = false;
	m_mapH.onTimer   = false;
	m_mapH.onTimerX  = false;
	m_mapH.onFocus   = false;
	m_mapH.onClose   = false;
	m_mapH.onDevice  = false;
	m_mapH.onKey     = false;
	m_mapH.onTrigger = false;
	m_mapH.onDeclaration = false;
	m_mapH.onInDeclaration = false;

	tokcnt = 0;
	CString sBuf = "";
	for (int ii = 0; ii < totL; ii++)
	{
		sBuf = wccParseResource(ii, &type);
		switch (type)
		{
		case TYPE_RESOURCE:
		case TYPE_COMMENT:
			tokcnt++;
			buildResource((LPSTR)sBuf.operator LPCTSTR(), ii, formN); 
		default:	break;
		}
	}
	if (tokcnt > 0)
	{
		sBuf = "}";
		buildResource((LPSTR)sBuf.operator LPCTSTR(), totL, formN);
	}

	m_mapGroup = m_mapH.mapGroup;
	m_mapDesc = m_mapH.mapdesc;
	m_mapH.formN    = formN;
	m_mapH.source   = true;
	m_mapH.modified = false;

	sortIoOrder();
	sortTabOrder();

	switch (generateSource(false, &m_mapH))
	{
	case 0:
		m_mapH.source = true;	break;
	case 1:
	case 2:
	default:
		CFormItem FormItem(&m_mapH);
		FormItem.RemoveAllForm();

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return false;
	}

	CFormItem FormItem(&m_mapH);
	FormItem.RemoveAllForm();

	return true;
}

bool CBatch::compile()
{
	int	totL, rc;
	WORD	type;
	CString	srcN, objN;

	srcN = objN = _T("");
	wccSetTempPath(srcN, (char *)m_files.operator LPCTSTR());
	wccSetTempPath(objN, (char *)m_files.operator LPCTSTR(), false);

	if (!open(srcN))
		return false;

	totL = wccLoadSource(srcN, (CWnd *) 1);
	if (totL == -1)
		return false;

	if (wccAllocateMemory(totL) < 0)
	{
		wccFreeMemory();
		return false;
	}

	if (wccAnalyzeColumn(totL) < 0)
	{
		wccFreeMemory();
		return false;
	}

	for (int ii = 0; ii < MAX_TOKEN_N; ii++)
		m_token[ii].Clear();

	rc = 0;
	for (int ii = 0; ii < totL; ii++)
	{
		rc += wccBuildToken(m_token, ii, &type);
		if (rc < 0)
			rc = 0;
	}
	wccSetDominoForm();
	wccLastCompileWork();

	if (wccBuildObject(objN) < 0)
	{
		wccFreeMemory();
		return false;
	}

	wccFreeMemory();
	return true;
}

HTREEITEM CBatch::GetItemByName(HTREEITEM hItem, LPCTSTR szItemName)
{
	// If hItem is NULL, start search from root item.
	if (hItem == NULL)
		hItem = (HTREEITEM)m_tree.SendMessage(TVM_GETNEXTITEM,
		TVGN_ROOT, 0);
	while (hItem != NULL)
	{
		char szBuffer[8+1];
		TV_ITEM item;
		
		item.hItem = hItem;
		item.mask = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = szBuffer;
		item.cchTextMax = 8;
		m_tree.SendMessage(TVM_GETITEM, 0, (LPARAM)&item);
		
		// Did we find it?
		if (lstrcmpi(szBuffer, szItemName) == 0)
			return hItem;
		
		// Check whether we have child items.
		if (item.cChildren)
		{
			// Recursively traverse child items.
			HTREEITEM hItemFound, hItemChild;
			
			hItemChild = (HTREEITEM)m_tree.SendMessage(TVM_GETNEXTITEM,
				TVGN_CHILD, (LPARAM)hItem);
			hItemFound = GetItemByName(hItemChild, szItemName);
			
			// Did we find it?
			if (hItemFound != NULL)
				return hItemFound;
		}
		
		// Go to next sibling item.
		hItem = (HTREEITEM)m_tree.SendMessage(TVM_GETNEXTITEM,
			TVGN_NEXT, (LPARAM)hItem);
	}
	
	// Not found.
	return NULL;
}

CString CBatch::setMapPath(CString mapN)
{
	if (mapN.GetLength() != 8)
		return "";
	CString tmp;
	tmp.Format("map/%s/%s/%s", mapN.Left(2), mapN.Left(3), mapN);
	return tmp;
}

void CBatch::sortIoOrder()
{
	CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	int nMax = 0, nEtcSeq = 0;
	CFormItem formItem(&m_mapH);

	for (int ii = 0; ii < formItem.getFormCount(); ii++)
		nMax = max(nMax, formItem.getFormPoint(ii)->iorder);

	for (int ii = 0; ii < formItem.getFormCount(); ii++)
	{
		int kind = formItem.getFormPoint(ii)->kind;
		if (isValidIOOrder(&formItem, ii))
		{
			int index = formItem.getFormPoint(ii)->iorder;
			if (index)
			{
				/*if (formItem.getFormPoint(ii)->attr & FA_SKIP)
				{
					formItem.getFormPoint(ii)->iorder = 0;
					continue;
				}*/
				
				if (kind == FM_TABLE)
				{
					if (!(formItem.getFormPoint(ii)->attr2 & GO_MULTI) && !(formItem.getFormPoint(ii)->attr2 & GO_XHEAD))
					{
						formItem.getFormPoint(ii)->iorder = 0;
						continue;
					}
				}
				int tmp;
				while(mapArr.Lookup(index, tmp))
				{
					index = ++nMax;
					formItem.getFormPoint(ii)->iorder = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				/*if (formItem.getFormPoint(ii)->attr & FA_SKIP)
					continue;*/
				
				if (kind == FM_TABLE)
				{
					if (!(formItem.getFormPoint(ii)->attr2 & GO_MULTI) && !(formItem.getFormPoint(ii)->attr2 & GO_XHEAD))
						continue;
				}
				index = ++nEtcSeq;
				mapArr.SetAt(index + formItem.getFormCount(), ii);
			}
		}
		else
			formItem.getFormPoint(ii)->iorder = 0;
	}
	for (int ii = 1; ii <= nMax; ii++)
	{
		int tmp;
		if (mapArr.Lookup(ii, tmp))
			sortArr.Add(tmp);
	}
	for (int ii = 1; ii <= nEtcSeq; ii++)
	{
		int tmp;
		if (mapArr.Lookup(ii + formItem.getFormCount(), tmp))
			sortArr.Add(tmp);
	}

	for (int ii = 0; ii < sortArr.GetSize(); ii++)
		formItem.getFormPoint(sortArr.GetAt(ii))->iorder = ii + 1;

	sortArr.RemoveAll();
	mapArr.RemoveAll();
}

void CBatch::sortTabOrder()
{
	CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	int nMax = 0;
	CFormItem formItem(&m_mapH);

	mapArr.RemoveAll();
	sortArr.RemoveAll();

	for (int ii = 0; ii < formItem.getFormCount(); ii++)
		nMax = max(nMax, formItem.getFormPoint(ii)->index);

	for (int ii = 0; ii < formItem.getFormCount(); ii++)
	{
		int kind = formItem.getFormPoint(ii)->kind;
		if (isValidTabOrder(&formItem, ii))
		{
			int index = formItem.getFormPoint(ii)->index;
			if (index)
			{
				int tmp;
				while(mapArr.Lookup(index, tmp))
				{
					index = ++nMax;
					formItem.getFormPoint(ii)->index = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				index = ++nMax;
				formItem.getFormPoint(ii)->index = index;
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
		}
		else
			formItem.getFormPoint(ii)->index = 0;
	}
	
	for (int ii = 1; ii <= nMax; ii++)
	{
		int tmp;
		if (mapArr.Lookup(ii, tmp))
			sortArr.Add(tmp);
	}
	
	for (int ii = 0; ii < sortArr.GetSize(); ii++)
		formItem.getFormPoint(sortArr.GetAt(ii))->index = ii + 1;

	sortArr.RemoveAll();
	mapArr.RemoveAll();
}

void CBatch::sortDownArray()
{
	CString sTmp;
	for (int ii = 0; ii < m_downArray.GetSize(); ii++)
	{
		for (int jj = ii + 1; jj < m_downArray.GetSize(); jj++)
		{
			if (strcmp(m_downArray.GetAt(ii).Mid(2), m_downArray.GetAt(jj).Mid(2)) > 0)
			{
				sTmp = m_downArray.GetAt(ii);
				m_downArray.SetAt(ii, m_downArray.GetAt(jj));
				m_downArray.SetAt(jj, sTmp);
			}
		}
	}
}

bool CBatch::isValidIOOrder(CFormItem *pFormItem, int idx)
{
	int kind = pFormItem->getFormPoint(idx)->kind;

	switch(kind)
	{
	case FM_EDIT:
	case FM_OUT:
	case FM_COMBO:
	case FM_GRID:
	case FM_TABLE:
	case FM_OBJECT:
	case FM_MEMO:
	case FM_TREE:
	case FM_SHEET:
		return true;
	case FM_BROWSER:
		if (pFormItem->getFormPoint(idx)->iok == EIO_OUTPUT)
			return true;
		break;
	case FM_BUTTON:
		if ((pFormItem->getFormPoint(idx)->type == BTN_RADIO || pFormItem->getFormPoint(idx)->type == BTN_CHECK)
			&& pFormItem->getFormPoint(idx)->iok != EIO_NOP)
			return true;
		break;
	case FM_CONTROL:
		if (pFormItem->getFormPoint(idx)->iok != EIO_NOP)
			return true;
	}
	return false;
}

bool CBatch::isValidTabOrder(CFormItem *pFormItem, int idx)
{
	int kind = pFormItem->getFormPoint(idx)->kind;
	switch(kind)
	{
	case FM_EDIT:
	case FM_BUTTON:
	case FM_COMBO:
	case FM_OBJECT:
	case FM_GRID:
	case FM_MEMO:
		return true;
	case FM_CONTROL:
		if (pFormItem->getFormPoint(idx)->iok == EIO_INPUT || 
			pFormItem->getFormPoint(idx)->iok == EIO_INOUT)
			return true;
		break;
	}
	return false;
}
