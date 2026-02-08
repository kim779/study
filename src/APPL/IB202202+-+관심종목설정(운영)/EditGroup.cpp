// EditGroup.cpp : implementation file
//

#include "stdafx.h"
#include "IB202201.h"
#include "EditGroup.h"
#include "NewGroup.h"

#include "../../h/axisvar.h"
#include "../../h/axisfire.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditGroup dialog


CEditGroup::CEditGroup(CWnd* pParent, CString root, CString name)
	: CDialog(CEditGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditGroup)
	//}}AFX_DATA_INIT
#ifdef DF_SEARCH
	m_userD.Format("%s/%s/%s", root, USRDIR, name);
	m_dragImage = nullptr;
	m_selindex = -1;
	m_pParent = pParent;
#else
	m_userD.Format("%s/%s/%s",    root, USRDIR, name);
	m_tempN.Format("%s/%s/%s/%s", root, USRDIR, name, "portfolio.ini");

	m_dragImage = nullptr;
	m_selindex = -1;

	m_pParent = pParent;
#endif
}


void CEditGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditGroup)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditGroup, CDialog)
	//{{AFX_MSG_MAP(CEditGroup)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_UP_BTN, OnUpBtn)
	ON_BN_CLICKED(IDC_DOWN_BTN, OnDownBtn)
	ON_BN_CLICKED(IDC_RENAME_BTN, OnRenameBtn)
	ON_BN_CLICKED(IDC_DEL_BTN, OnDelBtn)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST, OnBegindragList)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST, OnEndlabeleditList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST, OnBeginlabeleditList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditGroup message handlers

BOOL CEditGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_pEdit = std::make_unique<CEditEx>();
	InitList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditGroup::OnClose() 
{	
	CDialog::OnClose();
}

BOOL CEditGroup::ExistFile(CString fullfile)
{	
	CFileFind finder;
	if (!finder.FindFile(fullfile))
		return false;
	
	return true;
}

void CEditGroup::OnOK() 
{

	CDialog::OnOK();
}



void CEditGroup::OnUpBtn() 
{
	int	nItem{}, data{};
	CString	text;
	CString	gname, gno;
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		nItem = m_list.GetNextSelectedItem(pos);

		//매니징 배열 수정
		CString temp1, temp2, temp3;

		if (nItem != 0)
		{
			temp1 = ((CPage1*)m_pParent)->m_manageGroup[nItem-1][0];
			temp2 = ((CPage1*)m_pParent)->m_manageGroup[nItem-1][1];
			temp3 = ((CPage1*)m_pParent)->m_manageGroup[nItem-1][3];
			
			((CPage1*)m_pParent)->m_manageGroup[nItem-1][0] = ((CPage1*)m_pParent)->m_manageGroup[nItem][0];
			((CPage1*)m_pParent)->m_manageGroup[nItem-1][1] = ((CPage1*)m_pParent)->m_manageGroup[nItem][1];
			((CPage1*)m_pParent)->m_manageGroup[nItem-1][3] = ((CPage1*)m_pParent)->m_manageGroup[nItem][3];

			((CPage1*)m_pParent)->m_manageGroup[nItem][0] = temp1;
			((CPage1*)m_pParent)->m_manageGroup[nItem][1] = temp2;
			((CPage1*)m_pParent)->m_manageGroup[nItem][3] = temp3;

	
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
	if (pos != nullptr)
	{
		nItem = m_list.GetNextSelectedItem(pos);

		if (nItem != m_list.GetItemCount() - 1)
		{
			//매니징 배열 수정
			CString temp1, temp2, temp3;
			temp1 = ((CPage1*)m_pParent)->m_manageGroup[nItem][0];
			temp2 = ((CPage1*)m_pParent)->m_manageGroup[nItem][1];
			temp3 = ((CPage1*)m_pParent)->m_manageGroup[nItem][3];

			((CPage1*)m_pParent)->m_manageGroup[nItem][0] = ((CPage1*)m_pParent)->m_manageGroup[nItem+1][0];
			((CPage1*)m_pParent)->m_manageGroup[nItem][1] = ((CPage1*)m_pParent)->m_manageGroup[nItem+1][1];
			((CPage1*)m_pParent)->m_manageGroup[nItem][3] = ((CPage1*)m_pParent)->m_manageGroup[nItem+1][3];
			
			((CPage1*)m_pParent)->m_manageGroup[nItem+1][0] = temp1;
			((CPage1*)m_pParent)->m_manageGroup[nItem+1][1] = temp2;
			((CPage1*)m_pParent)->m_manageGroup[nItem+1][3] = temp3;

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
#ifdef DF_SEARCH
	int	select_count;
	int* nItemIndex = NULL;
	POSITION pos = m_list.GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		select_count = m_list.GetSelectedCount();
		nItemIndex = (int*)new int[select_count];

		for (int i = 0; i < select_count; i++)
			nItemIndex[i] = m_list.GetNextSelectedItem(pos);

		int i = select_count - 1;
		while (i > -1)
		{
			if (nItemIndex != NULL)
			{
				if (select_count <= m_list.GetItemCount())
				{
					int idata = nItemIndex[i];
					idata = m_list.GetItemData(nItemIndex[i]);
					int aryIndex = m_list.GetItemData(nItemIndex[i]) - 1;
					((CPage1*)m_pParent)->m_manageGroup[aryIndex][1] = "D";
					m_list.DeleteItem(nItemIndex[i]);
					select_count--;	//2015.03.23 KSJ 전체지우면 안지워지는 현상 수정
				}
			}
			i--;
		}

		m_list.RedrawItems(0, select_count);

		if (nItemIndex != NULL)
			delete nItemIndex;
	}
#else
	int	select_count;
	int* nItemIndex = NULL;

	POSITION pos = m_list.GetFirstSelectedItemPosition();

	if (pos != NULL)
	{
		select_count = m_list.GetSelectedCount();
		nItemIndex = (int*)new int[select_count];


		for (int i = 0; i < select_count; i++)
		{
			nItemIndex[i] = m_list.GetNextSelectedItem(pos);
		}

		int i = select_count - 1;

		while (i > -1)
		{
			if (nItemIndex != NULL)
			{
				if (select_count <= m_list.GetItemCount())
				{
					CString dir, tmpdir, bookdir, booktmpdir;
					CFileFind finder, bookfinder;

					//매니징 배열 수정
					int idata = nItemIndex[i];
					idata = m_list.GetItemData(nItemIndex[i]);
					int aryIndex = m_list.GetItemData(nItemIndex[i]) - 1;
					((CPage1*)m_pParent)->m_manageGroup[aryIndex][1] = "D";

					dir.Format("%s/portfolio.i%02d", m_userD, m_list.GetItemData(nItemIndex[i]));
					tmpdir.Format("%s.tmp", dir);

					bookdir.Format("%s/bookmark.i%02d", m_userD, m_list.GetItemData(nItemIndex[i]));
					booktmpdir.Format("%s", bookdir);

					m_list.DeleteItem(nItemIndex[i]);
					select_count--;	//2015.03.23 KSJ 전체지우면 안지워지는 현상 수정

					CFile file(tmpdir, CFile::modeWrite | CFile::modeCreate);
					CFile bookfile(booktmpdir, CFile::modeWrite | CFile::modeCreate);
					file.Write("", 0);
					file.Close();
					bookfile.Write("", 0);
					bookfile.Close();
				}
			}

			i--;
		}

		m_list.RedrawItems(0, select_count);

		if (nItemIndex != NULL)
		{
			delete nItemIndex;
		}
	}
#endif
}

void CEditGroup::OnRenameBtn() 
{
	CString	gname, gno;
	
	m_selindex = m_list.GetSelectionMark();
	if (m_selindex < 0) return;

	CString	string = m_list.GetItemText(m_selindex, 0); // 0번째 인덱스 
	
	if (!string.IsEmpty()) m_clickItem = string;

	CNewGroup newGroup(this, 1, m_clickItem);
	if (newGroup.DoModal() == IDOK)
	{
		gname = newGroup.m_defName; gname.TrimLeft();
		if (gname.IsEmpty()) gname = m_clickItem;

		m_list.SetItemText(m_selindex, 0, gname);
		
		//매니징 배열 수정
		((CPage1*)m_pParent)->m_manageGroup[m_selindex][1] = "M";
		((CPage1*)m_pParent)->m_manageGroup[m_selindex][3] = gname;
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
	m_dragImage.reset();
	
	CPoint	pt;
	m_dragImage = CreateDragImageSymbol(&pt, &m_list);
	if (m_dragImage == nullptr)
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

	//매니징 배열 수정
	((CPage1*)m_pParent)->m_manageGroup[m_selindex][1] = "M";
	((CPage1*)m_pParent)->m_manageGroup[m_selindex][3] = gname;

	*pResult = 0;
}

void CEditGroup::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW * pDispInfo = (NM_LISTVIEW *)pNMHDR;
	
	m_selindex = pDispInfo->iItem;
	
	CString	string = m_list.GetItemText(pDispInfo->iItem, 0); // 0번째 인덱스 

	if (!string.IsEmpty()) m_clickItem = string;

//TRACE("m_selindex[%d] m_clickItem[%s]\n", m_selindex, m_clickItem);
	
	*pResult = 0;
}

void CEditGroup::InitList()
{
#ifdef DF_SEARCH
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	addColumn("", 0, 150);

	int nIdx = 0;
	CString sGroupArr, gname, gno, stmp;
	sGroupArr.Empty();
	CWnd* wnd = ((CPage1*)m_pParent)->GetParent()->GetParent();

	memset(m_pdata, 0x00, 1024 * 2);
	const bool ret = wnd->SendMessage(WM_MSG, MAKEWPARAM(MSG_GET_GROUPARR, 0), (LPARAM)m_pdata);
	
	if (ret == 0)
		return;

	sGroupArr.Format("%s", m_pdata);
	int addIdx = 0;
	while (!sGroupArr.IsEmpty())
	{
		stmp = parseX(sGroupArr, "\t");
		int igroup = atoi(parseX(stmp, ";"));
		gname = stmp;
		addIdx = addItem(gname, nIdx++);
		m_list.SetItemData(addIdx, igroup);
	}


#else
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	addColumn("", 0, 150);

	int nIdx = 0;
	int readL = 0; char readB[2048];
	CString	string, gno, gname;

	readL = GetPrivateProfileString("GROUPORDER", "00", "", readB, sizeof(readB), m_tempN);
	string = CString(readB, readL);
	
	int addIdx = 0;
	while (!string.IsEmpty())
	{
		gno = parseX(string, ";");
		readL = GetPrivateProfileString(_T("GROUPNAME"), gno, "", readB, sizeof(readB), m_tempN);
		gname = CString(readB, readL);

		if (!gname.IsEmpty())
		{
			addIdx = addItem(gname, nIdx++);
			m_list.SetItemData(addIdx, atoi(gno));
		}
	}
#endif
}

void CEditGroup::KillSelected(CListCtrl *ListCtrl)
{
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (pos != nullptr)
	{
		while (pos)
		{
			const int nItem = ListCtrl->GetNextSelectedItem(pos);
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

std::shared_ptr<CImageList> CEditGroup::CreateDragImageSymbol(LPPOINT lpPoint, CListCtrl *ListCtrl)
{
	const int iCount = ListCtrl->GetSelectedCount();
	if (iCount < 1) return nullptr;

	// 전체 Rect를 구함
	CRect rectItem;
	CRect rectTotal; rectTotal.SetRectEmpty();
	
	POSITION pos = ListCtrl->GetFirstSelectedItemPosition();
	if (!pos) return nullptr;

	const int iItem = ListCtrl->GetNextSelectedItem(pos);
	ListCtrl->GetItemRect(iItem, rectItem, LVIR_BOUNDS);
	rectTotal = rectItem;
	rectTotal.bottom = rectTotal.top + rectTotal.Height() * iCount;

	CDC memDC; CClientDC dcClient(this);
	if (!memDC.CreateCompatibleDC(&dcClient))
		return nullptr;

	CBitmap bitmap;
	if (!bitmap.CreateCompatibleBitmap(&dcClient, rectTotal.Width(), rectTotal.Height()))
		return nullptr;

	CBitmap* pOldMemDCBitmap = memDC.SelectObject(&bitmap);
	memDC.FillSolidRect(0, 0, rectTotal.Width(), rectTotal.Height(), RGB(255, 255, 255));

	auto pCompleteImageList = std::make_shared<CImageList>();
	pCompleteImageList->Create(rectTotal.Width(), rectTotal.Height(), ILC_COLOR | ILC_MASK, 0, 1);

	int iIndex = 0;
	pos = ListCtrl->GetFirstSelectedItemPosition();
	while (pos)
	{
		const int iItem = ListCtrl->GetNextSelectedItem(pos);
		ListCtrl->GetItemRect(iItem, rectItem, LVIR_LABEL);

		CPoint	pt;
		auto pimglstSingle = std::unique_ptr<CImageList>(ListCtrl->CreateDragImage(iItem, &pt));
		if (pimglstSingle)
		{
			pimglstSingle->Draw(&memDC, 0, CPoint(0, rectItem.Height() * iIndex++), ILD_MASK);
			pimglstSingle.reset();
		}
	}

	memDC.SelectObject(pOldMemDCBitmap);
	pCompleteImageList->Add(&bitmap, RGB(255, 255, 0)); 
	
	if (lpPoint)
	{
		lpPoint->x = rectItem.left;
		lpPoint->y = rectItem.top;
	}

	return pCompleteImageList;
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

		
		//매니징 배열 수정
		((CPage1*)m_pParent)->sortManageGroup(m_nItem, nItem);

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

int CEditGroup::addItem(CString &strItem, int nItem)
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

void CEditGroup::OnBeginlabeleditList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	CEdit*	pEdit = m_list.GetEditControl();
	
	m_pEdit->SubclassWindow(pEdit->m_hWnd);
	m_pEdit->SetLimit(18);
	*pResult = 0;
}

void CEditGroup::OnDestroy() 
{
	CDialog::OnDestroy();

}
