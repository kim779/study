// usrObject.cpp : implementation file
//

#include "stdafx.h"
#include "usrObject.h"
#include "../mapvar.h"
#include "../mainfrm.h"
#include "../childfrm.h"
#include "../awsock/wsock.h"
#include "../awWcc/libwcc.h"
#include "../../h/axisvar.h"
#include "../../h/axisrsm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	UOB_INI		"userobj.ini"

/////////////////////////////////////////////////////////////////////////////
// CusrObject dialog

CusrObject::CusrObject(CWnd* frame, bool userScreen)
	: CDialog(CusrObject::IDD, frame)
{
	m_state   = stIDLE;
	m_mainFRM = frame;
	m_data    = _T("");
	m_show    = true;
	m_delta   = 0;
	m_getobject  = true;
	m_userScreen = userScreen;
	m_clientRc.SetRectEmpty();

	m_wsock     = (CwSock *) NULL;
	m_dragImage = (CImageList *) NULL;
	m_load      = false;
	m_type      = tpALL;

	m_ips  = _T("");
	m_port = -1;

	//{{AFX_DATA_INIT(CusrObject)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CusrObject::~CusrObject()
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

void CusrObject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CusrObject)
	DDX_Control(pDX, IDC_SYMTREE, m_tree);
	DDX_Control(pDX, IDC_SYMLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CusrObject, CDialog)
	//{{AFX_MSG_MAP(CusrObject)
	ON_NOTIFY(TVN_SELCHANGED, IDC_SYMTREE, OnTreeSelChanged)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_SYMLIST, OnListBeginDrag)
	ON_BN_CLICKED(IDC_RECEIVE, OnReceive)
	ON_BN_CLICKED(IDC_SHOW, OnToggleShow)
	ON_NOTIFY(NM_DBLCLK, IDC_SYMLIST, OnListDblClick)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_STATE, OnState)
	ON_MESSAGE(WM_READDATA, OnReadData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CusrObject message handlers

BOOL CusrObject::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CImageList imglist;
	CBitmap    bitmap;

	bitmap.LoadBitmap(IDB_UTREE);
	imglist.Create(16, 16, ILC_COLORDDB|ILC_MASK, 4, 1);
	imglist.Add(&bitmap, RGB(255, 0, 255));
	imglist.SetBkColor(CLR_NONE);
	m_tree.SetImageList(&imglist, TVSIL_NORMAL);
	imglist.Detach();
	bitmap.Detach();

	addColumn("객 체 명", 0, 100);
	if (!m_userScreen)
		addColumn("화면", 1, 50);
	DWORD dwStyle = m_list.SendMessage(LVM_GETEXTENDEDLISTVIEWSTYLE);
	dwStyle |= LVS_EX_FULLROWSELECT;
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE, 0, (LPARAM)dwStyle);

	m_wsock = new CwSock(this);

	CWinApp* app = AfxGetApp();
	m_root = (char*)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR, 0);
	m_path.Format("%s\\%s\\%s", m_root, TABDIR, UOB_INI);
//	loadFile();
	m_select = (Cobject *) NULL;

	CRect	tRc, lRc;
	m_list.GetWindowRect(&lRc);
	m_tree.GetWindowRect(&tRc);
	m_delta = lRc.left - tRc.left;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CusrObject::OnReceive() 
{
	m_getobject = false;
	goState(stCONNECTING);
	if (!m_wsock->Open(m_ips, m_port))
	{
		goState(stIDLE);
		MessageBox("Socket을 open하는 중 Error가 발생하였습니다.", "ERROR", MB_OK);
		return;
	}

	m_SRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_RRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	m_tree.EnableWindow(FALSE);
	m_list.EnableWindow(FALSE);
	GetDlgItem(IDC_RECEIVE)->EnableWindow(FALSE);

	goState(stCONNECTED);
	sendData();
}

void CusrObject::OnTreeSelChanged(NMHDR* pNMHDR, LRESULT* pResult) 
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
		m_list.DeleteAllItems();
		return;
	}

	m_item = hitem;
	CobjectSET* objSET = (CobjectSET *)m_tree.GetItemData(hitem);
	fillList(objSET);
}

void CusrObject::OnListBeginDrag(NMHDR* pNMHDR, LRESULT* pResult) 
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
	m_mainFRM->SendMessage(WM_STATUS, AWS_TDRAG, (LPARAM) 0);

	m_dragImage->BeginDrag(0, startpt);
	m_dragImage->DragEnter(GetDesktopWindow(), startpt);
	m_mainFRM->SetCapture();
}

void CusrObject::OnToggleShow() 
{
	CWnd*	item;
	CRect	rect;
	CString	text;
	int	left, width, delta;
	int	idcX[] = { IDC_SHOW, IDC_SECTION };

	m_show = !m_show;
	GetWindowRect(&m_clientRc);
	width  = m_clientRc.Width();
	if (m_show)
	{
		delta = m_delta;
		text = ">>";
		m_tree.ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RECEIVE)->ShowWindow(SW_SHOW);
	}
	else
	{
		delta = 0 - m_delta;
		text = "<<";
		m_tree.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RECEIVE)->ShowWindow(SW_HIDE);
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

LONG CusrObject::OnState(WPARAM wParam, LPARAM lParam)
{
	CString	msg = (char *)lParam;
	goState((int)wParam);

	return 0;
}

LONG CusrObject::OnReadData(WPARAM wParma, LPARAM lParam)
{
	CString	tmps;
	int	datL;

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
		GetDlgItem(IDC_RECEIVE)->EnableWindow();

		return 0;
	}

	if (m_RRH->resC == resC_ASCII)
	{
		if (m_getobject)
               		wccSetTempPath(tmps, m_RRH->resN);
		else
			tmps = m_path;

		switch (m_RRH->resF)
		{
		case resF_FIR:
			if (!m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite))
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

		case resF_ONLY:
			if (!m_file.Open(tmps, CFile::modeCreate|CFile::modeWrite))
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
			if (m_getobject)
				openObject(tmps);
			else
				loadFile();

			goState(stIDLE);
		}
	}

	return 0;
}

void CusrObject::SetData(CString data)
{
	CobjectSET* objSET;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		CmainSET* mainSET = m_arSET.GetAt(ii);
		for (int jj = 0; jj < mainSET->m_arMAIN.GetSize(); jj++)
		{
			objSET = mainSET->m_arMAIN.GetAt(jj);
			if (!data.CompareNoCase(objSET->m_data))
				break;
		}
		if (jj < mainSET->m_arMAIN.GetSize())
			break;
	}

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
				if (tvi.lParam = (DWORD) objSET)
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
		fillList((CobjectSET *)m_tree.GetItemData(m_item));
		m_tree.SelectSetFirstVisible(m_item);
		m_tree.Select(m_item, TVGN_CARET);
	}
}

Cobject* CusrObject::GetSelectedData()
{
	CobjectSET*	objSET;

	objSET   = (CobjectSET *)m_tree.GetItemData(m_item);
	m_select = (Cobject *) NULL;
	for (int ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		if (m_list.GetItemState(ii, LVIS_SELECTED) == LVIS_SELECTED)
		{
			DWORD data = m_list.GetItemData(ii);
			m_select = objSET->m_arOBJ.GetAt(data);
		}
	}
	return m_select;
}

void CusrObject::Load()
{
	if (m_load)
		return;

	GetDlgItem(IDC_MSG)->ShowWindow(SW_SHOW);
	loadFile();
	GetDlgItem(IDC_MSG)->ShowWindow(SW_HIDE);
	m_select = (Cobject *) NULL;
	m_load   = true;
}

void CusrObject::SetType(TYPE type)
{
	if (!m_userScreen)
		return;

	m_type = type;
	Load();
	if (m_item == (HTREEITEM) NULL)
		return;

	CobjectSET* objSET = (CobjectSET *)m_tree.GetItemData(m_item);
	fillList(objSET);
}

void CusrObject::SetConnectInfo(CString ips, int port)
{
	m_ips  = ips;
	m_port = port;
}

bool CusrObject::sendData()
{
	if (m_getobject)
	{
		CChildFrame* child;
		CMainFrame*  frame = (CMainFrame *) m_mainFRM;
		for (int ii = 0; ii < frame->m_children.GetSize(); ii++)
		{
			child = frame->m_children.GetAt(ii);
			if (child == (CChildFrame *)NULL || child->m_mapH.mapK != MK_USEROB)
				continue;

			if (!m_select->m_data.CompareNoCase(child->m_mapH.mapN))
			{
				CString	tmps;
				tmps.Format("\n[%s] 같은 사용자 객체가 편집중입니다.\n\n", m_select->m_data);
				MessageBox(tmps, "AXIS Workshop", MB_OK);
				return true;
			}
		}
	}

	m_SRH->dirF = dirF_OUTB;
	if (m_SRH->resK != resK_RSP)
		m_SRH->resK = m_getobject ? resK_UOB : resK_UINFO;
	m_SRH->resC = m_RRH->resC;
	m_SRH->resF = m_RRH->resF;

	if (m_getobject)
		strcpy(m_SRH->resN, m_select->m_data);
	else
		strcpy(m_SRH->resN, UOB_INI);
	wccToBUFFER(m_SRH->datL, sizeof(m_SRH->datL), 0);
	if (!m_wsock->Write(L_rsmH))
	{
		goState(stIDLE);
		return false;
	}

	return true;
}

void CusrObject::loadFile()
{
	char	wb[256];
	CString	tmps;
	Cobject*    object;
	CobjectSET* objSET;
	CmainSET*   mainSET;

	m_item = (HTREEITEM) NULL;
	m_tree.DeleteAllItems();
	removeAll();

	for (int ii = 1; ; ii++)
	{
		tmps.Format("%03d", ii);
		GetPrivateProfileString("ROOT", tmps, "", wb, sizeof(wb), m_path);
		tmps = wb;
		if (tmps.IsEmpty())
			break;

		mainSET = new CmainSET;
		mainSET->m_name = tmps;
		str2data(mainSET->m_name, mainSET->m_data);

		for (int jj = 1; ; jj++)
		{
			tmps.Format("%03d", jj);
			GetPrivateProfileString(mainSET->m_data, tmps, "", wb, sizeof(wb), m_path);
			tmps = wb;
			if (tmps.IsEmpty())
				break;

			objSET = new CobjectSET;
			objSET->m_name = tmps;
			str2data(objSET->m_name, objSET->m_data);

			for (int kk = 1; ; kk++)
			{
				tmps.Format("%03d", kk);
				GetPrivateProfileString(objSET->m_data, tmps, "", wb, sizeof(wb), m_path);
				tmps = wb;
				if (tmps.IsEmpty())
					break;

				object = new Cobject;
				object->m_name = tmps;
				str2data(object->m_name, object->m_data);
				object->m_uob = str2data(object->m_data);
				objSET->m_arOBJ.Add(object);
			}
			mainSET->m_arMAIN.Add(objSET);
		}
		m_arSET.Add(mainSET);
	}

	HTREEITEM hitem, item;
	for (ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		mainSET = m_arSET.GetAt(ii);
		hitem = m_tree.InsertItem(mainSET->m_name, 0, 1);
		m_tree.SetItemData(hitem, (DWORD)mainSET);

		for (int jj = 0; jj < mainSET->m_arMAIN.GetSize(); jj++)
		{
			objSET = mainSET->m_arMAIN.GetAt(jj);
			item = m_tree.InsertItem(objSET->m_name, 2, 3, hitem);
			m_tree.SetItemData(item, (DWORD)objSET);
			if (!m_item)
			{
				if (m_data.IsEmpty() || (!m_data.IsEmpty() && !m_data.CompareNoCase(objSET->m_data)))
				{
					m_item = item;
					fillList(objSET);
					continue;
				}
			}
		}
	}

	if (m_item != (HTREEITEM) NULL)
	{
		m_tree.SelectSetFirstVisible(m_item);
		m_tree.Select(m_item, TVGN_CARET);
	}

	m_tree.EnableWindow();
	m_list.EnableWindow();
	GetDlgItem(IDC_RECEIVE)->EnableWindow();
}

void CusrObject::removeAll()
{
	CobjectSET* objSET;
	CmainSET*   mainSET;

	for (int ii = 0; ii < m_arSET.GetSize(); ii++)
	{
		mainSET = m_arSET.GetAt(ii);
		for (int jj = 0; jj < mainSET->m_arMAIN.GetSize(); jj++)
		{
			objSET = mainSET->m_arMAIN.GetAt(jj);
			for (int kk = 0; kk < objSET->m_arOBJ.GetSize(); kk++)
				delete objSET->m_arOBJ.GetAt(kk);

			objSET->m_arOBJ.RemoveAll();
			delete objSET;
		}
		mainSET->m_arMAIN.RemoveAll();
		delete mainSET;
	}
	m_arSET.RemoveAll();
}

void CusrObject::trim(CString& str)
{
	str.TrimLeft();
	str.TrimRight();
}

void CusrObject::str2data(CString& str, CString& data)
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

bool CusrObject::str2data(CString& str)
{
	int	offs = str.Find(':');
	if (offs == -1)
		return true;
	CString	data = str.Mid(offs+1);
	str = str.Left(offs);
	trim(str);
	if (!data.CompareNoCase("O"))
		return false;

	return true;
}

BOOL CusrObject::addColumn(LPCTSTR str, int item, int width, bool align_left)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = align_left ? LVCFMT_LEFT : LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) str;
	lvc.cx      = m_list.GetStringWidth(lvc.pszText) + width;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(item, &lvc);
}

BOOL CusrObject::addItem(int item, int dwData, int subItem, CString& str)
{
        LV_ITEM lvi;

        lvi.mask     = LVIF_TEXT;
        lvi.iItem    = item;
        lvi.iSubItem = subItem;
        lvi.pszText  = (LPTSTR) str.operator LPCTSTR();
        if (subItem == 0)
	{
		lvi.mask  |= LVIF_PARAM;
		lvi.lParam = (LPARAM) dwData;
                return m_list.InsertItem(&lvi);
	}
        return m_list.SetItem(&lvi);
}

void CusrObject::fillList(CobjectSET* objSET)
{
	CmainSET* mainSET;
	Cobject*  object;
	HTREEITEM hitem;
	int	count = 0;
	CString	items = _T("");

	hitem = m_tree.GetParentItem(m_item);
	if (hitem != (HTREEITEM) NULL)
	{
		mainSET = (CmainSET *)m_tree.GetItemData(hitem);
		items = _T("  ") + mainSET->m_name + " / " + objSET->m_name;
		GetDlgItem(IDC_SECTION)->SetWindowText(items);
	}

	m_list.DeleteAllItems();
	count = 0;
	for (int ii = 0; ii < objSET->m_arOBJ.GetSize(); ii++)
	{
		object = objSET->m_arOBJ.GetAt(ii);
		switch (m_type)
		{
		case tpOBJ:
			if (object->m_uob)
				continue;
			break;
		case tpUOB:
			if (!object->m_uob)
				continue;
		case tpALL:
		default:
			break;
		}
		items = object->m_name;
		addItem(count, ii, 0, items);
		if (!m_userScreen)
		{
			items = object->m_data;
			addItem(count, ii, 1, items);
		}
		count++;
	}
}

CImageList* CusrObject::createDragImage(LPPOINT lpPoint)
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

void CusrObject::goState(int state, CString msg)
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
		m_wsock->Close();
		status += "연결이 해제되었습니다.";
		break;
	}

	if (!msg.IsEmpty())
		status = msg;
}

void CusrObject::OnListDblClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
	if (m_userScreen)
		return;

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem < 0)
		return;

	m_getobject = true;
	CobjectSET* objSET = (CobjectSET *)m_tree.GetItemData(m_item);
	m_select = (Cobject *)objSET->m_arOBJ.GetAt(pNMListView->iItem);

	goState(stCONNECTING);
	if (!m_wsock->Open())
	{
		goState(stIDLE);
		MessageBox("Socket을 open하는 중 Error가 발생하였습니다.", "ERROR", MB_OK);
		return;
	}

	m_SRH = (struct _rsmH *) &m_wsock->m_sendB[0];
	m_RRH = (struct _rsmH *) &m_wsock->m_recvB[0];

	goState(stCONNECTED);
	sendData();
}

void CusrObject::openObject(CString name)
{
	m_mainFRM->SendMessage(WM_OPENOBJ, (WPARAM)0, (LPARAM)(char *)name.operator LPCTSTR());
}
