// batch.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "batch.h"
#include "afxdialogex.h"

#include "../h/axisvar.h"
#include "../amCom/formItem.h"
//#include "../awSock64/wsock.h"
//#include "../awSock64/upload.h"
//#include "../awSock64/download.h"
//#include "../awBuild64/libBuild.h"

#define	R_NONE		-1
#define R_SUB		1

// CBatch 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBatch, CDialogEx)

CBatch::CBatch(CWnd* pParent, CString dir)
	: CDialogEx(CBatch::IDD, pParent)
{
	m_export = FALSE;
	m_status = _T("");

	m_dir     = dir;
	m_item  = (HTREEITEM) NULL;
	m_batching = false;

	m_downArray.RemoveAll();
}

CBatch::~CBatch()
{
}

void CBatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ERR, m_list);
	DDX_Check(pDX, IDC_EXPORT, m_export);
	DDX_Text(pDX, IDC_STATUS, m_status);
	DDX_Control(pDX, IDC_TREE, m_tree);
}


BEGIN_MESSAGE_MAP(CBatch, CDialogEx)
	ON_BN_CLICKED(IDC_BATCH, &CBatch::OnBatch)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, &CBatch::OnTreeSelChanged)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, &CBatch::OnTreeDblClick)
	ON_MESSAGE(WM_STATE, &CBatch::OnState)
END_MESSAGE_MAP()


// CBatch 메시지 처리기입니다.

BOOL CBatch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CString tmps;
	CBitmap	bitmap;
	CImageList imglist;

	bitmap.LoadBitmap(IDB_TREE);
	imglist.Create(16, 16, ILC_COLORDDB|ILC_MASK, 5, 1);
	imglist.Add(&bitmap, RGB(255, 0, 255));
	imglist.SetBkColor(CLR_NONE);
	m_tree.SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();

	addColumn(_T("Group"),  0, 15);
	addColumn(_T("Name"),  1, 80);

	DWORD	dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SetExtendedStyle(dwStyle);

	//m_download = new CDownload();
	//wccSetTempPath(tmps, DN_FILE);
	//m_download->Download(_T(""), tmps, 2);
	//delete m_download;
	//loadFile();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CBatch::OnOK()
{
	if (m_batching)
	{
		m_batching = false;
		GetDlgItem(IDOK)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BATCH)->SetFocus();
		return;
	}
	CDialogEx::OnOK();
}

void CBatch::OnBatch()
{
	/*if (m_item == (HTREEITEM) NULL)
	{
		goState(stWAIT);
		return;
	}

	DWORD	nItemData;
	CString	tmps;

	UpdateData();
	m_batching = true;
	m_downArray.RemoveAll();
	nItemData = (DWORD)m_tree.GetItemData(m_item);
	tmps = m_tree.GetItemText(m_item);
	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
	{
		if (!tmps.Compare(CString(m_treeArr.GetAt(ii).mapn, (nItemData == R_NONE ? 2 : 3))))
			m_downArray.Add(m_treeArr.GetAt(ii).mapn);
	}
	m_list.DeleteAllItems();
	m_tree.EnableWindow(FALSE);
	GetDlgItem(IDOK)->SetWindowText(_T("Stop"));
	GetDlgItem(IDC_BATCH)->EnableWindow(FALSE);

	sortDownArray();

	for (int ii = 0; ii < m_downArray.GetSize(); ii++)
	{
		CString mapName = m_downArray.GetAt(ii), tmps, svrMap;
		goState(stIDLE, mapName + _T(" Downloading..."));
		svrMap.Format(_T("map\\%s\\%s\\%s"), mapName.Left(L_SGID), mapName.Left(L_TGID), mapName);
		
		// Download
		m_download = new CDownload();
		wccSetTempPath(tmps, (LPSTR)mapName.operator LPCTSTR());
		m_download->Download(svrMap, tmps, 0);
		delete m_download;

		goState(stIDLE, mapName + _T(" Compiling..."));
		if (compile(mapName))
		{
			goState(stIDLE, mapName + _T(" Uploading..."));

			// upload
			m_upload = new CUpload(m_export ? true : false);
			m_upload->Upload(svrMap, 1, false, &m_mapH);
			delete m_upload;
			goState(stIDLE, mapName + _T(" Completed..."));
		}
		if (!m_batching)
			break;
	}
	if (m_batching)
		goState(stCOMPLETE);
	else
		goState(stSTOP);*/
}


void CBatch::OnTreeSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_item = m_tree.GetSelectedItem();
	*pResult = 0;*/
}


void CBatch::OnTreeDblClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	/*NM_TREEVIEW* pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_item = m_tree.GetSelectedItem();
	if (m_tree.GetItemData(m_item) == R_SUB)
		OnBatch();
	*pResult = 0;*/
}


afx_msg LRESULT CBatch::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam, msg);
	return 0;
}

bool CBatch::isValidTabOrder(class CFormItem* formItem, int idx)
{
	switch (formItem->GetForm(idx)->kind)
	{
	case FM_EDIT:
	case FM_BUTTON:
	case FM_COMBO:
	case FM_OBJECT:
	case FM_GRID:
	case FM_GRIDEX:
		return true;

	case FM_CONTROL:
		if (formItem->GetForm(idx)->iok == EIO_INPUT || formItem->GetForm(idx)->iok == EIO_INOUT)
			return true;
		break;
	}
	return false;
}

bool CBatch::isValidIOOrder(class CFormItem* formItem, int idx)
{
	/*switch (formItem->GetForm(idx)->kind)
	{
	case FM_EDIT:
	case FM_OUT:
	case FM_COMBO:
	case FM_GRID:
	case FM_TABLE:
	case FM_OBJECT:
	case FM_MEMO:
		return true;

	case FM_BROWSER:
		if (formItem->GetForm(idx)->iok == EIO_OUTPUT)
			return true;
		break;

	case FM_BUTTON:
		if ((formItem->GetForm(idx)->type == BTN_RADIO || formItem->GetForm(idx)->type == BTN_CHECK)
			&& formItem->GetForm(idx)->iok != EIO_NOP)
			return true;
		break;

	case FM_CONTROL:
		if (formItem->GetForm(idx)->iok != EIO_NOP)
			return true;
	}*/
	return false;
}

void CBatch::sortDownArray()
{
	/*CString tmps;

	for (int ii = 0; ii < m_downArray.GetSize(); ii++)
	{
		for (int jj = ii + 1; jj < m_downArray.GetSize(); jj++)
		{
			if (strcmp(m_downArray.GetAt(ii).Mid(L_SGID), m_downArray.GetAt(jj).Mid(L_SGID)) > 0)
			{
				tmps = m_downArray.GetAt(ii);
				m_downArray.SetAt(ii, m_downArray.GetAt(jj));
				m_downArray.SetAt(jj, tmps);
			}
		}
	}*/
}

void CBatch::sortTabOrder()
{
	/*CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	int	ii, val, index, nMax = 0;
	CFormItem formItem(&m_mapH);

	mapArr.RemoveAll();
	sortArr.RemoveAll();

	for (ii = 0; ii < formItem.GetCount(); ii++)
		nMax = max(nMax, formItem.GetForm(ii)->index);

	for (ii = 0; ii < formItem.GetCount(); ii++)
	{
		if (isValidTabOrder(&formItem, ii))
		{
			index = formItem.GetForm(ii)->index;
			if (index)
			{
				while (mapArr.Lookup(index, val))
				{
					index = ++nMax;
					formItem.GetForm(ii)->index = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				index = ++nMax;
				formItem.GetForm(ii)->index = index;
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
		}
		else
			formItem.GetForm(ii)->index = 0;
	}
	
	for (ii = 1; ii <= nMax; ii++)
	{
		if (mapArr.Lookup(ii, val))
			sortArr.Add(val);
	}
	
	for (ii = 0; ii < sortArr.GetSize(); ii++)
		formItem.GetForm(sortArr.GetAt(ii))->index = ii + 1;

	sortArr.RemoveAll();
	mapArr.RemoveAll();*/
}

void CBatch::sortIoOrder()
{
	/*CMap	<int, int, int, int> mapArr;
	CArray	<int, int>	sortArr;
	int	ii, kind, index, val;
	int	nMax = 0, nEtcSeq = 0;
	CFormItem formItem(&m_mapH);

	for (ii = 0; ii < formItem.GetCount(); ii++)
		nMax = max(nMax, formItem.GetForm(ii)->iorder);

	for (ii = 0; ii < formItem.GetCount(); ii++)
	{
		kind = formItem.GetForm(ii)->kind;
		if (isValidIOOrder(&formItem, ii))
		{
			index = formItem.GetForm(ii)->iorder;
			if (index)
			{
				if (kind == FM_TABLE)
				{
					if (!(formItem.GetForm(ii)->attr2 & GO_MULTI) && !(formItem.GetForm(ii)->attr2 & GO_XHEAD))
					{
						formItem.GetForm(ii)->iorder = 0;
						continue;
					}
				}
				while (mapArr.Lookup(index, val))
				{
					index = ++nMax;
					formItem.GetForm(ii)->iorder = index;
				}
				nMax = max(nMax, index);
				mapArr.SetAt(index, ii);
			}
			else
			{
				if (kind == FM_TABLE)
				{
					if (!(formItem.GetForm(ii)->attr2 & GO_MULTI) && !(formItem.GetForm(ii)->attr2 & GO_XHEAD))
						continue;
				}
				index = ++nEtcSeq;
				mapArr.SetAt(index + formItem.GetCount(), ii);
			}
		}
		else
			formItem.GetForm(ii)->iorder = 0;
	}

	for (ii = 1; ii <= nMax; ii++)
	{
		if (mapArr.Lookup(ii, val))
			sortArr.Add(val);
	}

	for (ii = 1; ii <= nEtcSeq; ii++)
	{
		if (mapArr.Lookup(ii + formItem.GetCount(), val))
			sortArr.Add(val);
	}

	for (ii = 0; ii < sortArr.GetSize(); ii++)
		formItem.GetForm(sortArr.GetAt(ii))->iorder = ii + 1;

	sortArr.RemoveAll();
	mapArr.RemoveAll();*/
}

HTREEITEM CBatch::getItemByName(HTREEITEM hItem, LPCTSTR szItemName)
{
	// If hItem is NULL, start search from root item.
	/*if (hItem == NULL)
		hItem = (HTREEITEM)m_tree.SendMessage(TVM_GETNEXTITEM, TVGN_ROOT, 0);

	while (hItem != NULL)
	{
		char	szBuffer[8+1];
		TV_ITEM	item;
		
		item.hItem = hItem;
		item.mask  = TVIF_TEXT | TVIF_CHILDREN;
		item.pszText = szBuffer;
		item.cchTextMax = 8;
		m_tree.SendMessage(TVM_GETITEM, 0, (LPARAM)&item);
		
		// find it?
		if (lstrcmpi(szBuffer, szItemName) == 0)
			return hItem;
		
		// check whether it have child item
		if (item.cChildren)
		{
			// recursively find child items.
			HTREEITEM hItemFound, hItemChild;
			
			hItemChild = (HTREEITEM)m_tree.SendMessage(TVM_GETNEXTITEM, TVGN_CHILD, (LPARAM)hItem);
			hItemFound = getItemByName(hItemChild, szItemName);
			
			// find it?
			if (hItemFound != NULL)
				return hItemFound;
		}
		
		// go to next sibling item.
		hItem = (HTREEITEM)m_tree.SendMessage(TVM_GETNEXTITEM, TVGN_NEXT, (LPARAM)hItem);
	}*/
	
	// not found.
	return NULL;
}

void CBatch::insert(CString mapName)
{
	HTREEITEM	hitem1, hitem2;

	hitem1 = getItemByName(NULL, mapName.Mid(0, L_SGID));
	if (!hitem1)
	{
		hitem1 = m_tree.InsertItem(mapName.Mid(0, L_SGID), 0, 1);
		m_tree.SetItemData(hitem1, R_NONE);
	}
	hitem2 = getItemByName(hitem1, mapName.Mid(0, L_TGID));
	if (!hitem2)
	{
		hitem2 = m_tree.InsertItem(mapName.Mid(0, L_TGID), 0, 1, hitem1);
		m_tree.SetItemData(hitem2, R_SUB);
		m_tree.SortChildren(hitem1);
	}
}

void CBatch::loadFile()
{
	/*CString	dir;
	struct	_mapTree	mapTree;

	dir.Format(_T("%s\\%s"), m_dir, DN_FILE);
	m_treeArr.RemoveAll();
	
	TRY
	{
		CFile	file(dir, CFile::modeRead);
		UINT nBytesRead;

		do
		{
			nBytesRead = file.Read(&mapTree, sizeof(mapTree));
			if (nBytesRead == sizeof(mapTree))
				m_treeArr.Add(mapTree);
		} while((int)nBytesRead);
		file.Close();
	} CATCH(CFileException, e )
	{
#ifdef _DEBUG
		afxDump << "File could not be opened " << e->m_cause << "\n";
#endif
	}
	END_CATCH

	for (int ii = 0; ii < m_treeArr.GetSize(); ii++)
		insert(m_treeArr.GetAt(ii).mapn);
	m_tree.SortChildren(NULL);*/
}

void CBatch::goState(int state, CString msg)
{
	/*m_status = _T("  ");
	switch (state)
	{
	case stADDRESS:
		m_status += _T("None of IP Address");
		break;

	case stCONNECTING:
		m_status += _T("Connecting");
		break;
	
	case stCONNECTED:
		m_status += _T("Connected.");
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;
	
	case stWAIT:
		m_status += _T("Set Path.");
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;
	
	case stRECEIVING:
		break;

	case stCOMPLETE:
		m_status += _T("Batch Completed.");
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;

	case stSTOP:
		m_status += _T("Stopped by user...");
		m_tree.EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDC_BATCH)->EnableWindow(TRUE);
		break;
	case stIDLE:
	default:
		m_status += _T("Disconnected");
		break;
	}

	if (!msg.IsEmpty())
		m_status = msg;*/

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
	/*int	totL, tokcnt;
	int	formN, repN;
	WORD	type;

	totL = wccLoadSource(fileN, (CWnd *) NULL);
	if (totL == -1)
		return false;

	ZeroMemory((char *)&m_mapH, sizeof(_mapH));
	m_mapH.mapGroup = 1;
	strcpy_s(m_mapH.mapN, sizeof(m_mapH.mapN), fileN.Mid(fileN.ReverseFind(_T('\\')) + 1));
	strcpy_s(m_mapH.mapdesc, sizeof(m_mapH.mapdesc), _T("xx"));

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
	m_mapH.onFocus   = false;
	m_mapH.onClose   = false;
	m_mapH.onDevice  = false;
	m_mapH.onKey     = false;
	m_mapH.onTrigger = false;
	m_mapH.onDeclaration = false;
	m_mapH.onInDeclaration = false;

	tokcnt = 0;
	CString sBuf = _T("");
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
		sBuf = _T("}");
		buildResource((LPSTR)sBuf.operator LPCTSTR(), totL, formN);
	}

	m_mapH.formN    = formN;
	m_mapH.source   = true;
	m_mapH.modified = false;

	sortIoOrder();
	sortTabOrder();

	switch (generateSource(false, &m_mapH))
	{
	case 0:
		m_mapH.source = true;
		break;
	case 1:
	case 2:
	default:
		CFormItem FormItem(&m_mapH);
		FormItem.RemoveAll();

		SetCursor(LoadCursor(NULL, IDC_ARROW));
		return false;
	}

	CFormItem FormItem(&m_mapH);
	FormItem.RemoveAll();*/

	return true;
}

bool CBatch::compile(CString fileN)
{
	/*int	totL, rc;
	WORD	type;
	CString	srcN, objN;

	srcN = objN = _T("");
	wccSetTempPath(srcN, (char *)fileN.operator LPCTSTR());
	wccSetTempPath(objN, (char *)fileN.operator LPCTSTR(), false);

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

	if (wccBuildObject(objN) < 0)
	{
		wccFreeMemory();
		return false;
	}

	wccFreeMemory();*/
	return true;
}