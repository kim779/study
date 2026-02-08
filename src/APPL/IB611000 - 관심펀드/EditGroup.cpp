// EditGroup.cpp : implementation file
//

#include "stdafx.h"
#include "IB611000.h"
#include "EditGroup.h"
#include "../../h/axisvar.h"
#include "NewGroup.h"


#include "Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditGroup dialog
#define	WM_EDITGROUP	(WM_USER + 200)

CEditGroup::CEditGroup(CWnd* pParent, CWnd *m_pWizard, CString root, CString name, _grouplist *grouplist[], int grouplistsize)
	: CDialog(CEditGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditGroup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	std::unique_ptr<_groupEditlist> group{};
	m_EditGroup = pParent;
	m_main = m_pWizard;

	m_userD.Format("%s/%s/%s",    root, USRDIR, name);
	m_tempN.Format("%s/%s/%s/%s", root, USRDIR, name, "FDportfolio.ini.tmp");
	
	m_dragImage = NULL;
	m_pEdit = NULL;
	m_selindex = -1;

	m_delGroupcnt = 0;
	m_groupsize = grouplistsize;
	m_oldGroupLIst.RemoveAll();
	m_newGroupLIst.RemoveAll();
	//m_newGroupLIst.RemoveAll();
	
	CString gno;

	if(grouplistsize > 0)
	{
		for(int ii = 0; ii < m_groupsize; ii++)
		/*m_grouplist = grouplist;	*/
		{
			group = std::make_unique<_groupEditlist>();

			gno = CString(grouplist[ii]->gno, 4);

			
			strcpy(group->oldgno, gno);
			strcpy(group->newgno, gno);
			strcpy(group->oldgnm, grouplist[ii]->gnm);
			
// 			CopyMemory(group->oldgno, gno, strlen(gno));
// 			CopyMemory(group->newgno, gno, strlen(gno));
// 			CopyMemory(group->oldgnm, grouplist[ii]->gnm, , strlen(grouplist[ii]->gnm));

			m_oldGroupLIst.Add(group.release());
//			TRACE("%s %s \n", group->oldgnm, gno);
		}
	}

	
}


void CEditGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditGroup)
	DDX_Control(pDX, IDC_LIST1, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditGroup, CDialog)
	//{{AFX_MSG_MAP(CEditGroup)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDelBtn)
	ON_BN_CLICKED(IDC_DOWN_BTN, OnDownBtn)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList)
	ON_BN_CLICKED(IDC_RENAME_BTN, OnRenameBtn)
	ON_BN_CLICKED(IDC_UP_BTN, OnUpBtn)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST1, OnBeginlabeleditList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST1, OnEndlabeleditList)
	ON_BN_CLICKED(IDCOK, OnOK)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnEditGroupMessage)
	ON_MESSAGE(WM_USER, OnEditGroupMsg)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditGroup message handlers
BOOL CEditGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
 	if (m_pEdit == NULL)
 		m_pEdit = std::make_unique<CEditEx>();
	
	QueryExcelSise();
	InitList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LONG CEditGroup::OnEditGroupMessage(WPARAM wParam, LPARAM lParam)
{
	//	int tets= wParam;
	const int key = HIBYTE(LOWORD(wParam));
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:		// Wizard request TR Header
		// return char*
		break;
	case DLL_OUB:		// Received Data
		// HIBYTE(LOWORD(WPARAM)) : key, HIWORD(WPARAM) : DATA size
		// HIBYTE(LOWORD(WPARAM)) : DATA size for MSB 8bits (control FORM OUB)
		switch (key)
		{
		case FUNDSEARCH:
			//ParsingData((char *)lParam, HIWORD(wParam));
			AfxMessageBox("EDitGOR");
			break;
// 		case FUNDNEWGROUP:
// 			ParsingDataNGroup((char *)lParam, HIWORD(wParam));
// 			break;
// 		case FUNDCREATEGROUP:
// 			ParsingCreateNGroup((char *)lParam, HIWORD(wParam));
// 			break;
// 		case FUNDLIST:
// 			ParsingGroupList((char *)lParam, HIWORD(wParam));
// 			break;
		case FUNDDELETEGROUP:
			AfxMessageBox("Cdlg FundDelete");
			break;
			
		}
		// 		ParsingData((char *)lParam, HIWORD(wParam));
		// 		break;
		case DLL_ALERT:	// RealTime data 
			// HIWORD(WPARAM) : stat
			// update : if control DLL,  DATAs
			// otherwise  CODE \t DATAs				
			break;
		case DLL_TRIGGER:	// Trigger data
			// HIBYTE(LOWORD(WPARAM)) : key
			// HIWORD(WPARAM) : trigger stat
			// trigger stat(true)  -> text : PROC \t PARAM
			//           (false) -> text : SYM \t DATA		
			break;
		case DLL_DOMINO:	// Domino data
			// LPARAM : text, SYM \t DATA (\n ... )
			// HIWORD(WPARAM) : domino stat			
			break;
		case DLL_NOTICE:	// account contract
			// lParam : account \t account name \n….
			break;
		case DLL_DROP:	// OnDrop data
			// lParam : point.x \t point.y \n SYM \t DATA (\t ...)
			// LOBYTE(HIWORD(WPARAM)) : source window ID
			// HIBYTE(HIWORD(WPARAM)) : destination window ID
			break;
		case DLL_AXIS:
			break;
		case DLL_EXCEL:	// Wizard request excel data
			break;
		case DLL_INVOKE:
			break;
		case DLL_SETPAL:	// Change Palette
			
			break;
		case DLL_SETFONT:	// set font only. not resize screen
			break;
		case DLL_SETFCB:	// blink
			break;
		case DLL_SETFONTx:	// set font & resize screen
			break;
		case DLL_SELECT:
			break;
		case DLL_ACCOUNT:	// update account combo items
			break;
		case DLL_GUIDE:	// set guide message
			break;
	}
	return TRUE;
}

long CEditGroup::OnEditGroupMsg(WPARAM wParam, LPARAM lParam)
{
	CString	string;
	switch ((int) wParam)
	{
	case GROUPINIT:
		AfxMessageBox("editmsg");
		break;
// 	case wpENTER:		m_search.GetWindowText(string);
// 		if (!string.IsEmpty())
// 		{
// 			m_search.SetSel(0, -1);
// 			insertList1toList2();
// 		}
// 		break;
// 	case wpSEARCH1:
// 		searchAction(0);	// number search by kwon
// 		break;
// 	case wpSEARCH2:
// 		m_search.GetWindowText(string);
// 		CString temp = string.GetAt(0);
// 		if(atoi(temp) >= 5 && atoi(temp) < 9)
// 		{
// 			searchAction(0);	// char search by kwon
// 		}
// 		else
// 		{
// 			searchAction(1);	// char search by kwon
// 		}
// 		
// 		break;
	}
	
	return 0;
}

void CEditGroup::OnClose() 
{
	if (m_dragImage) delete m_dragImage;
	
	CDialog::OnClose();
}

void CEditGroup::OnOK() 
{
	
	WritePrivateProfileString(_T("GROUPNAME"), NULL, NULL, m_tempN);
	
	CString	string = _T(""), gnoS, gnameS;
	
	int ii = 0;
	for (ii = 0; ii < m_list.GetItemCount(); ii++)
	{
		gnameS = m_list.GetItemText(ii, 0);
		gnoS.Format("%02d", m_list.GetItemData(ii));
		
		string += gnoS; 
		string += ";";
		
		WritePrivateProfileString(_T("GROUPNAME"), gnoS, gnameS, m_tempN);
	}
	
	WritePrivateProfileString(_T("GROUPORDER"), "00", string, m_tempN);
	WritePrivateProfileString(NULL, NULL, NULL, m_tempN);

	const int ncnt = m_list.GetItemCount();
	CString code, name;
	std::unique_ptr<_grouplist> newGrouplist{};
	//_grouplist *oldList;

	const int size = sizeof(newGrouplist->gnm);

	for(ii = 0; ii < ncnt; ii++)
	{
		
		newGrouplist = std::make_unique<_grouplist>();
		
		ZeroMemory(newGrouplist.get(), sizeof(_grouplist));
		name = m_list.GetItemText(ii, 0);
		code.Format("%04d", m_list.GetItemData(ii));
		
		//code.Format("04d", ii);		

		
		CopyMemory(newGrouplist->gno, code, strlen(code));

		CopyMemory(newGrouplist->gnm, name, strlen(name));
//		strncpy(newGrouplist->gnm, name, strlen(name));


		m_newGroupLIst.Add(newGrouplist.release());
	}
	
	if(m_delGroupcnt > 0)
		SendDeleteGroup();


	SendUpdaeGroup(ncnt);

	for(ii =0; ii < m_newGroupLIst.GetSize(); ii++)
	{
//		newGrouplist = m_newGroupLIst.GetAt(ii);

//		TRACE("New list : %s\n", newGrouplist->gno);
	}
//	else
// 	{
// 		for(ii = 0; ii < ncnt; ii++)
// 		{
// 			oldList = m_oldGroupLIst.GetAt(ii);
// 			newGrouplist = m_newGroupLIst.GetAt(ii);
// 
// 			if(strcmp(oldList->gno, newGrouplist->gno) != 0 || strcmp(oldList->gnm, newGrouplist->gnm) != 0)
// 			{
// 				
// 				
// 			}
// 		}
// 	
// 	}	



	CDialog::OnOK();
	
}

void CEditGroup::OnUpBtn() 
{
	int	nItem{}, data{};
	CString	text;
	CString	gname, gno;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	//_groupEditlist *groupEditlist;
	const int ncnt = m_oldGroupLIst.GetSize();


	if (pos != NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);

		if (nItem != 0)
		{
			text = m_list.GetItemText(nItem, 0);
			data = m_list.GetItemData(nItem);
			m_list.DeleteItem(nItem);
			
			nItem = addItem(text, nItem -1);
			m_list.SetItemData(nItem, data);
			
			KillSelected(&m_list);
			SetSelected(&m_list, nItem, FALSE);
			
			m_selindex = nItem;
		}

	}
	




}

void CEditGroup::OnDownBtn() 
{
	int	nItem{}, data{};
	CString	text;
	CString	gname, gno;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		nItem = m_list.GetNextSelectedItem(pos);
		if (nItem != m_list.GetItemCount())
		{
			text = m_list.GetItemText(nItem, 0);
			data = m_list.GetItemData(nItem);
			m_list.DeleteItem(nItem);
			
			nItem = addItem(text, nItem + 1);
			m_list.SetItemData(nItem, data);
			
			KillSelected(&m_list);
			SetSelected(&m_list, nItem, FALSE);
			
			m_selindex = nItem;
			
		}
	}		
	
}

void CEditGroup::OnDelBtn() 
{
	//int	select_count;
	int* nItemIndex = NULL;
	int rmlist[MYFD_MAXCNT]{};
	int _rmlist[MYFD_MAXCNT]{};
	int ii = 0;
	int kk = 0;
	int jj = 0;
	int ncnt = m_list.GetItemCount();

// 	_rmlist[MYFD_MAXCNT] = -1;
// 	rmlist[MYFD_MAXCNT] = -1;

	for(int i =0; i < MYFD_MAXCNT; i++)
	{
		_rmlist[i] = -1;
		rmlist[i] = -1;
	}

	_groupEditlist* grouplist{};

	POSITION pos = m_list.GetFirstSelectedItemPosition();
	


	for(ncnt = m_list.GetItemCount(); ncnt >= 0; ncnt--)										// 리스트에서 삭제부분
	{
		
		
		if(m_list.GetItemState(ncnt, LVIS_SELECTED) != 0)
		{
			// 			CopyMemory(fdcd, listitem->code, sizeof(listitem->code));
			
			m_list.DeleteItem(ncnt);	
			_rmlist[jj] = ncnt;

			
			m_delGroupcnt++;
			/*m_listitem2.RemoveAt(jj);*/
		}
	
		jj++;
	
	}

	const int cnt = m_oldGroupLIst.GetSize();

	for(jj = cnt, ii = 0; jj >= 0; jj--, ii++)
	{
		rmlist[jj] = _rmlist[ii];
	}

	CString gno;

	for(ii = 0, jj =0; ii < cnt; ii++, jj++)
	{
		grouplist = m_oldGroupLIst.GetAt(jj);
		
		
		if(rmlist[ii] >= 0)
		{
			FillMemory(m_afcd[kk], sizeof(m_afcd[kk]), ' ');
			
	//		m_afcd[ii] = CString(grouplist->oldgno, strlen(grouplist->oldgno));
		//	CopyMemory(m_afcd[kk], grouplist->oldgno,  strlen(grouplist->oldgno));
			strcpy(m_afcd[kk], grouplist->oldgno);
			
//			delete m_oldGroupLIst.GetAt(jj);
			jj--;
			kk++;
		}
	}
	m_oldGroupLIst.RemoveAll();
}

void CEditGroup::OnRenameBtn() 
{
	CString	gname, gno;
	
	m_selindex = m_list.GetSelectionMark();
	if (m_selindex < 0) return;
	
	CNewGroup newGroup(this, 1, m_clickItem);
	if (newGroup.DoModal() == IDOK)
	{
		gname = newGroup.m_defName; gname.TrimLeft();
		if (gname.IsEmpty()) gname = m_clickItem;
		
		m_list.SetItemText(m_selindex, 0, gname);
		
		//gno.Format("%02d", m_list.GetItemData(m_selindex));
		
		//WritePrivateProfileString(_T("GROUPNAME"), gno, gname, m_tempN);
		//WritePrivateProfileString(NULL, NULL, NULL, m_tempN);
	}
	
}

void CEditGroup::OnDestroy() 
{
	CDialog::OnDestroy();
	

	for(int ii = 0; ii < m_oldGroupLIst.GetSize(); ii++)
	{
//		delete m_oldGroupLIst.GetAt(ii);
		m_oldGroupLIst.RemoveAt(ii);
	
	}


	for(int ii =0; ii < m_newGroupLIst.GetSize(); ii++)
	{
//		delete m_newGroupLIst.GetAt(ii);
		m_newGroupLIst.RemoveAt(ii);
	}


	
}

void CEditGroup::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CEditGroup::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();
	
	if (m_dragImage)
	{
		m_dragImage->DragLeave(GetDesktopWindow());
		m_dragImage->EndDrag();
		m_dragImage->DeleteImageList();
		
		endDrag(point);
	}
	
	CDialog::OnLButtonUp(nFlags, point);
}

void CEditGroup::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_dragImage)
	{
		CPoint	ptMove = point;
		ClientToScreen(&ptMove);
		
		m_dragImage->DragMove(ptMove);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CEditGroup::OnBegindragList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	*pResult = 0;

	if (m_list.GetSelectedCount() == 0)
		return;
	
	m_nItem = pNMListView->iItem;
	
	if (m_dragImage) delete m_dragImage;
	
	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list);
	
	if (m_dragImage == (CImageList *)NULL)
		return;
	
	CPoint	ptStart = pNMListView->ptAction;
	m_list.ClientToScreen(&ptStart);
	
	m_dragImage->BeginDrag(0, CPoint(0, 0));
	m_dragImage->DragEnter(GetDesktopWindow(), ptStart);
	SetCapture();
}

void CEditGroup::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	m_selindex = m_list.GetSelectionMark();
	if (m_selindex < 0) return;
	
	m_list.EditLabel(m_selindex);
	
	*pResult = 0;
}

void CEditGroup::OnEndlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	
	if (m_selindex < 0 || m_selindex > m_list.GetItemCount())
		return;
	
	CString gname = pDispInfo->item.pszText; gname.TrimLeft();
	if (gname.IsEmpty()) gname = m_clickItem;
	
	m_pEdit->UnsubclassWindow();
	m_list.SetItemText(m_selindex, 0, gname);

	*pResult = 0;
}



void CEditGroup::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_LISTVIEW * pDispInfo = (NM_LISTVIEW *)pNMHDR;
	
	m_selindex = pDispInfo->iItem;
	
	CString	string = m_list.GetItemText(pDispInfo->iItem, 0); // 0번째 인덱스 
	
	if (!string.IsEmpty()) m_clickItem = string;
	
//	TRACE("m_selindex[%d] m_clickItem[%s]\n", m_selindex, m_clickItem);
	*pResult = 0;
}





void CEditGroup::OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CEdit*	pEdit = m_list.GetEditControl();
	
	m_pEdit->SubclassWindow(pEdit->m_hWnd);
	m_pEdit->SetLimit(18);
	*pResult = 0;
}


void CEditGroup::InitList()
{
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	addColumn("", 0, 150);

	int nIdx = 0;
//	int readL = 0; char readB[2048];
	CString	string, gno, gname;
	_groupEditlist* grouplist{};

	m_list.DeleteAllItems();
	

 	int addIdx = 0;



	for(int ii = 0; ii < m_groupsize; ii++)
	{
//		gname = CString((char*)m_grouplist[ii]->gnm, sizeof(struct m_grouplist->gnm));
 		grouplist = m_oldGroupLIst.GetAt(ii);
		
		gname = CString(grouplist->oldgnm, sizeof(grouplist->oldgnm));
		gno = CString(grouplist->oldgno, sizeof(grouplist->oldgno));

// 		gname.Format("%s", grouplist->gnm);
// 		gno.Format("%04d", grouplist->gno);

	//	TRACE("%s %s \n", grouplist->gnm, gno);
		addIdx = addItem(gname, nIdx++);
 		m_list.SetItemData(addIdx, atoi(gno));
	}




}

void CEditGroup::KillSelected(CListCtrl *ListCtrl)
{
	int nItem{};
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (pos != NULL)
	{
		while (pos)
		{
			nItem = ListCtrl->GetNextSelectedItem(pos);
			ListCtrl->SetItemState(nItem, NULL, LVIS_SELECTED | LVIS_FOCUSED);
		}
	}
}

void CEditGroup::SetSelected(CListCtrl *ListCtrl, int nItem, BOOL scroll/* = TRUE */)
{
	CRect	rc;
	CSize	size;

	ListCtrl->SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);

	if (scroll)
	{
		ListCtrl->GetItemRect(0, rc, LVIR_LABEL);
		size.cx = 0;
		size.cy = rc.Height() * (nItem - ListCtrl->GetTopIndex());
		ListCtrl->Scroll(size);
	}
}

CImageList* CEditGroup::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl)
{
	const int iCount = ListCtrl->GetSelectedCount();
	if (iCount < 1) return NULL;

	// 전체 Rect를 구함
	CRect rectItem;
	CRect rectTotal; rectTotal.SetRectEmpty();
	
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (!pos) return NULL;

	const int iItem = ListCtrl->GetNextSelectedItem(pos);
	ListCtrl->GetItemRect(iItem, rectItem, LVIR_BOUNDS);
	rectTotal = rectItem;
	rectTotal.bottom = rectTotal.top + rectTotal.Height() * iCount;

	CDC memDC; CClientDC dcClient(this);
	if (!memDC.CreateCompatibleDC(&dcClient))
		return NULL;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, rectTotal.Width(), rectTotal.Height()))
		return NULL;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, rectTotal.Width(), rectTotal.Height(), RGB(255, 255, 255));

	std::unique_ptr<CImageList> pCompleteImageList = std::make_unique<CImageList>();
	pCompleteImageList->Create(rectTotal.Width(), rectTotal.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	int iIndex = 0;
	pos = ListCtrl->GetFirstSelectedItemPosition();
	while (pos)
	{
		const int iItem = ListCtrl->GetNextSelectedItem(pos);
		ListCtrl->GetItemRect(iItem, rectItem, LVIR_LABEL);

		CPoint	pt;
		CImageList* pimglstSingle = ListCtrl->CreateDragImage(iItem, &pt);
		
		if (pimglstSingle)
		{
			pimglstSingle->Draw(&memDC, 0, CPoint(0, rectItem.Height() * iIndex++), ILD_MASK);
			delete pimglstSingle; pimglstSingle = NULL;
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = rectItem.left;
		lpPoint->y = rectItem.top;
	}

	return pCompleteImageList.release();
}

void CEditGroup::endDrag(CPoint point)
{
	CRect	ListRc;
	CString	sItem;

	m_list.GetClientRect(&ListRc);
	m_list.ClientToScreen(&ListRc);
	
	ClientToScreen(&point);
	
	if (ListRc.PtInRect(point))
	{
		LVHITTESTINFO lvhti;
		m_list.ScreenToClient(&point);
		lvhti.pt = point;
		
		int nItem = m_list.HitTest(&lvhti);
		
		if (nItem < 0)
			nItem = m_list.GetItemCount() -1;
		
		sItem = m_list.GetItemText(m_nItem, 0);
		const DWORD dData = m_list.GetItemData(m_nItem);
		m_list.DeleteItem(m_nItem);
		
		nItem = addItem(sItem, nItem);
		m_list.SetItemData(nItem, dData);
		
		KillSelected(&m_list);
		SetSelected(&m_list, nItem, FALSE);
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//

BOOL CEditGroup::addColumn(LPCTSTR strItem, int nItem, int nWidth)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_LEFT;
	lvc.pszText = (LPTSTR)strItem;
	lvc.cx = m_list.GetStringWidth(lvc.pszText) + nWidth;
	lvc.iSubItem = -1;

	return m_list.InsertColumn(nItem, &lvc);
}

int CEditGroup::addItem(const CString &strItem, int nItem)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = nItem;
	lvi.iSubItem = 0;
	lvi.pszText  = (LPTSTR) strItem.operator LPCTSTR();

	return m_list.InsertItem(&lvi);
}

CString CEditGroup::parseX(CString &srcstr, CString substr)
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

void CEditGroup::SendDeleteGroup()
{
	CString strData;
	CString gname;
	struct _mid4 mid4 {};
	const int ncnt = m_delGroupcnt;
	CString nrce;
	nrce.Format("%03d", ncnt);

	ZeroMemory(&mid4, sizeof(struct _mid4));
	
	CopyMemory(mid4.gubn, "d", sizeof(mid4.gubn));
	CopyMemory(mid4.nrce, nrce, sizeof(mid4.nrce));
	
	for(int ii = 0; ii < ncnt ; ii++)
	{
		//listitem = m_listitem2.GetAt(ii);
		FillMemory(&mid4.agcd[ii], sizeof(mid4.agcd[ii]), ' ');
		CopyMemory(mid4.agcd[ii], m_afcd[ii], strlen(mid4.agcd[ii]));
			
	}

	
	strData = CString((char *)&mid4, sizeof(struct _mid4));
//	TRACE("Create group : %s\n", strData);
	
	//	SendTR("pidomyfd", strData, FUNDCREATEGROUP, 0);
	SendTR("pidomyfd", strData, FUNDDELETEGROUP, 0);

	GetParent()->SendMessage(WM_EDITGROUP, DELETEGROUP, (LPARAM)strData.operator LPCTSTR());
}

LRESULT CEditGroup::SendTR(CString strCode, CString strData, int iKey, int iStat)
{

	std::unique_ptr<char[]> pcDataBuffer = std::make_unique<char[]>(L_userTH + strData.GetLength());
	//ZeroMemory(pcDataBuffer.get(), sizeof(pcDataBuffer));
	struct	_userTH* puserTH;
	puserTH = (struct _userTH *)pcDataBuffer.get();
	
	strcpy(puserTH->trc, strCode.operator LPCTSTR());
	puserTH->key = iKey;
	puserTH->stat = iStat;
	
	CopyMemory(&pcDataBuffer.get()[L_userTH], strData.operator LPCTSTR(), strData.GetLength());
	
	const LRESULT lResult = m_main->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM) pcDataBuffer.get());
	
	pcDataBuffer = nullptr;
	return lResult;
}

int CEditGroup::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

void CEditGroup::QueryExcelSise()
{
	
	CString strData;
	struct _4mid1 mid1;
	ZeroMemory(&mid1, sizeof(struct _4mid1));
	FillMemory(&mid1, sizeof(struct _4mid1), ' ');
	
	//	CopyMemory(&mid1.gubn, "", sizeof(mid1.gubn));
	//	CopyMemory(&mid1.nrce, "000", sizeof(mid1.nrce));
	
	// 	memset(&mid1, ' ', sizeof(mid1));
	// 	strncpy(mid1.grcd, "AAAAAAAA", 8);
	
	strncpy(mid1.gubn, "A", 1);
	strData = CString((char *)&mid1, sizeof(struct _4mid1));
	
	/*SendTR("pihomyfd", (char*)&mid1, FUNDSEARCH, 0);*/
	SendTR("pihomyfd", strData, FUNDSEARCH, 0);
}

void CEditGroup::SendUpdaeGroup(int ncnt)
{
	CString strData;
//	struct _mid3 mid3;
	CString nrce;
	_grouplist* newlist{};
	_groupEditlist* oldlist{};
	CString oldno, newno, gname;
	CString data;
	
	const int szMem = 5 + (100 * 28);
	std::unique_ptr<char[]> sendB = std::make_unique<char[]>(szMem);

	int datL = 4;
	struct _mid3* mid3 = (struct _mid3 *)&sendB.get()[datL];
	struct _item1* item1{};

	FillMemory(sendB.get(), szMem, ' ');
	
	nrce.Format("%03d", ncnt);
			
	CopyMemory(sendB.get(), "u", sizeof(mid3->gubn));
	CopyMemory(&sendB.get()[1], nrce, sizeof(mid3->nrce));

	for(int ii = 0; ii < ncnt ; ii++)
	{
		newlist = m_newGroupLIst.GetAt(ii);

		oldlist = m_oldGroupLIst.GetAt(ii);
	
		oldno = CString(oldlist->oldgno, sizeof(oldlist->oldgno));
		newno = CString(newlist->gno, sizeof(newlist->gno));
		gname = CString(newlist->gnm, strlen(newlist->gnm));
		gname.TrimRight();
//		gname.Format("%*.s", newlist->gnm);		
		item1 = (struct _item1 *)&sendB.get()[datL];
		
		datL += sizeof(struct _item1);

// 		strcpy(item1->ngrd, oldno);
// 		strcpy(item1->ogrd, newno);
		CopyMemory(item1->ngrd, oldno, sizeof(item1->ngrd));
		CopyMemory(item1->ogrd, newno, sizeof(item1->ogrd));
	//	strcpy(item1->grnm, gname);
		CopyMemory(item1->grnm, gname, strlen(gname));

//		TRACE("new Group: %s %s %s\n ",oldlist->newgno, oldlist->oldgno, oldlist->oldgnm);
		
	}
	
 	GetParent()->SendMessage(WM_EDITGROUP, UPDATEGROUP, (LPARAM)sendB.get());
}
