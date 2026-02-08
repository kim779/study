#include "stdafx.h"
#include "AxisGDlg.h"
#include "ScreenCfg.h"
#include "MainPreview.h"
#include "ObjConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define DRAG_REST	0
#define DRAG_LIST1	1
#define DRAG_LIST2	2
#define DRAG_TREE	3


CScreenCfg::CScreenCfg(CWnd* pMain, COLORREF tkCOLOR, char* envinfo, char* ginfo, char* scrinfo, char** pOutput, CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCfg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScreenCfg)
	//}}AFX_DATA_INIT

	m_tkCOLOR = tkCOLOR;
	m_pMain = pMain;
	m_pScrInfo = scrinfo;
	m_pEnv = envinfo;
	m_pIndi = ginfo;
	m_pOutput = pOutput;

	m_pDragImage = NULL;
	m_hitemDrag = NULL;
	m_hitemDrop = NULL;
	m_nCurDrag = DRAG_REST;	
	m_pgraphUnion = NULL;

	int	nMaxCount = ((struct _cntInfo*)m_pIndi)->wMainCnt + ((struct _cntInfo*)m_pIndi)->wAidCnt;
	ginfo += SZ_CNTINFO;
	struct	_graph* pGraph = (struct _graph*)ginfo;

	m_mapInfo.RemoveAll();
	m_arKind.RemoveAll();
	
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();

	for ( int ii = 0 ; ii < nMaxCount ; ii++ )
	{
		if (pApp->IsExist(pGraph[ii].wGKind))
		{
			m_mapInfo.SetAt(pGraph[ii].wGKind, &pGraph[ii]);
			m_arKind.Add(pGraph[ii].wGKind);		
		}
	}
}

void CScreenCfg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScreenCfg)
	DDX_Control(pDX, IDC_LIST1, m_listPrice);
	DDX_Control(pDX, IDC_TREE, m_tree);
	DDX_Control(pDX, IDC_LIST2, m_listAid);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CScreenCfg, CDialog)
	//{{AFX_MSG_MAP(CScreenCfg)
	ON_BN_CLICKED(IDC_BT_RGN_ADD, OnBtRgnAdd)
	ON_BN_CLICKED(IDC_BT_RGN_DEL, OnBtRgnDel)
	ON_BN_CLICKED(IDC_BT_LEFT, OnBtLeft)
	ON_BN_CLICKED(IDC_BT_RIGHT, OnBtRight)
	ON_BN_CLICKED(IDC_BT_UP, OnBtUp)
	ON_BN_CLICKED(IDC_BT_DOWN, OnBtDown)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, OnSetfocusList1)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST2, OnSetfocusList2)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, OnDblclkList2)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE, OnBegindragTree)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()	
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnBegindragList1)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST2, OnBegindragList2)
	ON_MESSAGE(WM_MANAGE, OnManage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CImageList* CScreenCfg::createDragImageEx(CListCtrl *pList, LPPOINT lpPoint)
{
	if (pList->GetSelectedCount() <= 0)
		return NULL;

	CRect rectSingle;
	CRect rectComplete(0,0,0,0);

	pList->GetClientRect(rectSingle);
	int nWidth  = rectSingle.Width();

	int nIndex = pList->GetTopIndex() - 1;
	int nBottomIndex = pList->GetTopIndex() + pList->GetCountPerPage() - 1;
	if (nBottomIndex > (pList->GetItemCount() - 1))
		nBottomIndex = pList->GetItemCount() - 1;

	while ((nIndex = pList->GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{
		if (nIndex > nBottomIndex)
			break; 

		pList->GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);

		if (rectSingle.left < 0) 
			rectSingle.left = 0;

		if (rectSingle.right > nWidth)
			rectSingle.right = nWidth;

		rectComplete.UnionRect(rectComplete, rectSingle);
	}
		
	CClientDC dcClient(this);
	CDC dcMem;
	CBitmap bitmap;

	if (!dcMem.CreateCompatibleDC(&dcClient))
		return NULL;

	if (!bitmap.CreateCompatibleBitmap(&dcClient, rectComplete.Width(), rectComplete.Height()))
		return NULL;

	CBitmap *pOldMemDCBitmap = dcMem.SelectObject(&bitmap);
 	dcMem.FillSolidRect(0, 0, rectComplete.Width(), rectComplete.Height(), RGB(255,255,0));

	nIndex = pList->GetTopIndex() - 1;
	while ((nIndex = pList->GetNextItem(nIndex, LVNI_SELECTED)) != -1)
	{	
		if (nIndex > nBottomIndex)
			return NULL;

		CPoint pt;
		CImageList* pSingleImageList = pList->CreateDragImage(nIndex, &pt);

		if (pSingleImageList)
		{
			pList->GetItemRect(nIndex, rectSingle, LVIR_BOUNDS);
			pSingleImageList->Draw( 
				&dcMem, 
				0, 
				CPoint(rectSingle.left - rectComplete.left, 
				rectSingle.top - rectComplete.top), 
				ILD_MASK);
			pSingleImageList->DeleteImageList();
			delete pSingleImageList;
		}
	}

 	dcMem.SelectObject(pOldMemDCBitmap);
	m_pCompleteImageList = new CImageList;
	m_pCompleteImageList->Create(rectComplete.Width(), rectComplete.Height(), ILC_COLOR | ILC_MASK, 0, 1);
	m_pCompleteImageList->Add(&bitmap, RGB(0, 255, 0)); 
	bitmap.DeleteObject();

	if (lpPoint)
	{
		lpPoint->x = rectComplete.left;
		lpPoint->y = rectComplete.top;
	}

	return m_pCompleteImageList;
}

void CScreenCfg::OnBtRgnAdd() 
{	
	int		nCount = m_tree.GetChildCount(NULL);
	HTREEITEM	hItem = NULL;
	CString		tmpstr;

	tmpstr.Format("영역 %d", nCount + 1);
	hItem = m_tree.AddItem(NULL, tmpstr, nCount + 1);
	m_tree.SelectItem(hItem);
}

void CScreenCfg::OnBtRgnDel() 
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();

	DeleteRgn(hItem);
}

void CScreenCfg::OnBtLeft() 
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();

	AddToRight(hItem);
}

void CScreenCfg::OnBtRight() 
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();	

	DeleteItem(hItem);
}

void CScreenCfg::OnBtUp() 
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();

	if (hItem == NULL)
		return;
	
	hItem = m_tree.MoveUp(hItem);
	m_tree.SelectItem(hItem);
}

void CScreenCfg::OnBtDown() 
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();

	if (hItem == NULL)
		return;

	hItem = m_tree.MoveDown(hItem);
	m_tree.SelectItem(hItem);
}

void CScreenCfg::OnDestroy() 
{
	CDialog::OnDestroy();

	SaveInfo();

	if (m_pDragImage)
	{
		m_pDragImage->DeleteImageList();
		delete m_pDragImage;
		m_pDragImage = NULL;
	}

	m_mapInfo.RemoveAll();
	m_arKind.RemoveAll();
}

BOOL CScreenCfg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int	nCount = m_arKind.GetSize();
	int	nIndex = 0, nKind = 0;
	int	ii = 0, jj = 0;
	CString	tmpstr;
	
	nCount = m_arKind.GetSize();
	CAxListCtrl*	pList = NULL;
	CHART_INFO*	pChart = NULL;
	char*		ptr = NULL;
	CRect		rect;
	int		nWidth = GetSystemMetrics(SM_CXVSCROLL);
	int		nSize = sizeof(struct _graph);
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	
	m_listPrice.GetWindowRect(rect);
	nWidth = rect.Width() - nWidth * 2;

	m_listPrice.InsertColumn(0, "", LVCFMT_LEFT, nWidth);
	m_listAid.InsertColumn(1, "", LVCFMT_LEFT, nWidth);	

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		nKind = m_arKind[ii];
		if (pApp->IsMain(nKind))
			continue;

		pChart = pApp->GetChartObj(nKind);
		
		if (pChart == NULL)
			continue;
		
		if (pApp->IsPrice(nKind))
			pList = &m_listPrice;
		else
			pList = &m_listAid;			
		
		nIndex = pList->AddItem(pChart->m_sztitle);
		pList->SetItemData(nIndex, pChart->m_nKind);
	}

	m_tmpImage.Create(IDB_SCRCFG, 16, 0, RGB(255, 255, 255));
	m_tree.SetImageList(&m_tmpImage, TVSIL_NORMAL);
		
	if (m_pScrInfo)
	{
		ptr = m_pScrInfo;
		struct _interChart*	pInterChart = (struct _interChart*)ptr;
		int		rcnt = pInterChart->rgncnt;
		int		gcnt = pInterChart->grpcnt;		
		int		nIndex = 0;
		HTREEITEM	hItem = NULL;
		HTREEITEM	hParent = NULL;
		CString		tmpstr, tmpstr1;
		CMap<int, int, HTREEITEM, HTREEITEM>	mapTree;
		CArray<CRect, CRect>	arRegion;
		CArray<int, int>	arRkey;
		CRect	rect;
		struct _indiInfo*	pIndi = NULL;
		int	nRcSize = 0, rKey = 0;
		
		if (rcnt == 0 || gcnt == 0)
			return TRUE;

		ptr += sz_INTERCHART;

		if (*m_pOutput == NULL)
		{
			for ( ii = 0 ; ii < rcnt ; ii++ )
			{
				rect.left = ((struct _region*)ptr)->wRLeft;
				rect.right = ((struct _region*)ptr)->wRRight;
				rect.top = ((struct _region*)ptr)->wRTop;
				rect.bottom = ((struct _region*)ptr)->wRBottom;
				
				nRcSize = arRegion.GetSize();

				if (nRcSize == 0)
				{
					arRegion.Add(rect);
					arRkey.Add(((struct _region*)ptr)->btRKey);
				}
				else
				{
					CRect	rcTmp;

					for ( jj = 0 ; jj < nRcSize ; jj++ )
					{
						rcTmp = arRegion.GetAt(jj);

						if (rect.bottom <= rcTmp.top)
						{
							arRegion.InsertAt(jj, rect);
							arRkey.InsertAt(jj, ((struct _region*)ptr)->btRKey);
							break;
						}
					}

					if (jj == nRcSize)
					{
						arRegion.Add(rect);
						arRkey.Add(((struct _region*)ptr)->btRKey);
					}
				}
				ptr += SZ_REGION;
			}
		}
		else
		{	
			for ( ii = 0 ; ii < rcnt ; ii++ )
			{
				rKey = ((struct _region*)ptr)->btRKey;
				arRkey.Add(rKey);
				ptr += SZ_REGION;
			}
		}

		for ( ii = 0 ; ii < rcnt ; ii++ )
		{
			tmpstr.Format("영역 %d", ii + 1);
			hItem = m_tree.AddItem(NULL, tmpstr, ii + 1);
			nIndex = arRkey.GetAt(ii);
			mapTree.SetAt(nIndex, hItem);			
		}

		if (ii <= 0)
			return TRUE;		
		
		if (ptr == NULL)
			return TRUE;

	
		for ( ii = 0 ; ii < gcnt ; ii++ )
		{
			nIndex = ((struct _graph*)ptr)->btRgnKey;
			nKind = ((struct _graph*)ptr)->wGKind;
			
			if (!mapTree.Lookup(nIndex, hItem))
				continue;

			// 합성차트 skip
			if (((struct _graph*)ptr)->dwDraOption & GO_UNION)
			{
				m_pgraphUnion = new struct _graph;
				CopyMemory(m_pgraphUnion, ptr, SZ_GRAPH);
				ptr += SZ_GRAPH;
				continue;
			}

			
			tmpstr = pApp->GetTitle(nKind);
			pIndi = pApp->GetIndiObj(nKind);

			if (!pApp->IsMain(nKind))
				tmpstr1 = LIB_MakeParen(((struct _graph*)ptr), pIndi);
			else
				tmpstr1 = _T("");				
			
			tmpstr	+= tmpstr1;
			hItem = m_tree.AddItem(hItem, tmpstr, nKind, (struct _graph*)ptr);

			if (pApp->IsMain(nKind))
			{
				m_tree.SetItemImage(hItem, 3, 3);
				m_tree.SelectItem(hItem);
			}
			
			ptr += nSize;
		}

		POSITION	pos = mapTree.GetStartPosition();
		hItem = NULL;

		while (pos)
		{
			mapTree.GetNextAssoc(pos, nIndex, hItem);

			if (hItem)
				m_tree.Expand(hItem, TVE_EXPAND);
		}
	}

	_envInfo	*envinfo = (_envInfo *)m_pEnv;
	switch (envinfo->datainfo.wMainGrpKind)
	{
	case GK_EQV:	case GK_CDV:
	case GK_CLOCK:	case GK_PV:	case GK_THREE:	case GK_PNF:	case GK_SWING:
	case GK_KAGI:	case GK_RENKO:
		{
			int	pID[8] = {IDC_LIST1, IDC_LIST2, IDC_BT_LEFT, IDC_BT_RIGHT, 
			IDC_BT_UP, IDC_BT_DOWN, IDC_BT_RGN_ADD, IDC_BT_RGN_DEL};

			CWnd	*pWnd;
			for (int ii = 0; ii < 8; ii++)
			{
				pWnd = (CWnd *)GetDlgItem(pID[ii]);
				pWnd->EnableWindow(FALSE);
			}
		}
		break;
	default:
		break;
	}

	return TRUE;
}

void CScreenCfg::OnSetfocusList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_listAid.SetSelection(-1);
	*pResult = 0;
}

void CScreenCfg::OnSetfocusList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_listPrice.SetSelection(-1);
	*pResult = 0;
}

void CScreenCfg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnBtLeft();
	*pResult = 0;
}

void CScreenCfg::OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnBtLeft();
	*pResult = 0;
}

void CScreenCfg::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM	hItem = m_tree.GetSelectedItem();
	if (!hItem)
		return;

	HTREEITEM	hParent = m_tree.GetParentItem(hItem);
	if (!hParent)
		return;

	INDI_OBJ*	pObject = m_tree.GetIndiObj(hItem);
	if (!pObject)
		return;

	int	iGrpKind = pObject->GetKind();
	if (iGrpKind == GK_CDVV || iGrpKind == GK_EQVV)	// 캔들,이퀴면 리턴
		return;

	// 합성정보 저장
	int	iIndex = ((_envInfo*)m_pEnv)->datainfo.btIndex;
	int	iUnit = ((_envInfo*)m_pEnv)->datainfo.btUnit;
	WORD	wUnionGKindOld = ((_envInfo*)m_pEnv)->datainfo.aaDayInfo[iUnit-GU_CODE][iIndex - GI_DAY].wUnionGKind;

	CObjConfig	dlg(m_pMain, m_tkCOLOR, m_pEnv, m_pIndi, (char*)pObject->GetGraph()); // 수정요함
	if (dlg.DoModal() == IDOK)
	{
		CString	strName = pObject->GetName();
		if (strName.Find("(") > 0)
			strName = strName.Left(strName.Find("(", 0));

		CString	strGraph;
		CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
		if (!pApp->IsMain(pObject->GetKind()))
		{
			struct	_indiInfo*	pIndi = pApp->GetIndiObj(pObject->GetKind());
			strGraph = LIB_MakeParen(pObject->GetGraph(), pIndi);
		}

		strName += strGraph;
		m_tree.SetItemText(strName, hItem);

		// 합성정보 변경검사
		WORD wUnionGKindNew = ((_envInfo*)m_pEnv)->datainfo.aaDayInfo[iUnit-GU_CODE][iIndex - GI_DAY].wUnionGKind;
		if (wUnionGKindOld != wUnionGKindNew)
		{
			if (wUnionGKindNew == 0) //없음)
			{
				if (m_pgraphUnion)
				{
					delete m_pgraphUnion;
					m_pgraphUnion = NULL;
				}
			}
			else
			{
				struct _graph* pGraph;
				if (m_mapInfo.Lookup(wUnionGKindNew, pGraph))
				{
					if (!m_pgraphUnion)
					{
						m_pgraphUnion = new struct _graph;
					}

					CopyMemory(m_pgraphUnion, pGraph, SZ_GRAPH);

					// 데이터 key, 리전 Key 세팅
					m_pgraphUnion->btDataKey = pObject->GetGraph()->btDataKey;
					m_pgraphUnion->btRgnKey = pObject->GetGraph()->btRgnKey;
					m_pgraphUnion->dwDraOption |= GO_UNION;
				}
			}
		}
	}

	*pResult = 0;
}


void CScreenCfg::OnBegindragTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	*pResult = 0;

	m_hitemDrag = pNMTreeView->itemNew.hItem;
	m_tree.SelectItem(m_hitemDrag);
	m_hitemDrop = NULL;

	m_pDragImage = m_tree.CreateDragImage(m_hitemDrag);
	
	if (!m_pDragImage)
		return;	

	m_nCurDrag = DRAG_TREE;
	m_pDragImage->BeginDrag(0, CPoint(-15,-15));
	m_pDragImage->SetBkColor(RGB(255, 0, 0));
	POINT pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter(NULL, pt);
	SetCapture();
}

void CScreenCfg::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM	hitem;
	UINT		flags;

	if (m_nCurDrag != DRAG_REST)
	{
		POINT pt = point;
	

		switch (m_nCurDrag)
		{
		case DRAG_LIST1:
		case DRAG_LIST2:
			{	
				ClientToScreen(&pt);		
				m_pDragImage->DragMove(pt);
				m_pDragImage->DragShowNolock(FALSE);	
				m_tree.ScreenToClient(&pt);

				

				if ((hitem = m_tree.HitTest(pt, &flags)) != NULL)
					m_tree.Select(hitem, TVGN_DROPHILITE);
				m_pDragImage->DragShowNolock(TRUE);				
			}
			break;
		case DRAG_TREE:
			{
				m_tree.SetFocus();
				ClientToScreen( &pt );
				CImageList::DragMove(pt);
				m_tree.ScreenToClient(&pt);

				if ((hitem = m_tree.HitTest(pt, &flags)) != NULL)
				{
					CImageList::DragShowNolock(FALSE);
					m_tree.SelectDropTarget(hitem);
					CImageList::DragShowNolock(TRUE);		
				}	
			}
			break;
		}
		
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CScreenCfg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);

	if (m_nCurDrag != DRAG_REST)
	{
		HTREEITEM	hItem = NULL;
		UINT		flags;
		CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();		
		
		CImageList::DragLeave(this);
		CImageList::EndDrag();
		ReleaseCapture();

		m_pDragImage->DeleteImageList();
		delete m_pDragImage;
		m_pDragImage = NULL;

		switch (m_nCurDrag)
		{
		case DRAG_LIST1:			
		case DRAG_LIST2:
			{
				m_tree.SelectDropTarget(NULL);
				ClientToScreen( &point );
				m_tree.ScreenToClient(&point);

				CPoint	pt;
				GetCursorPos(&pt);
				CWnd*	pWnd = WindowFromPoint(pt);

				if (pWnd)
				{
					if (pWnd->m_hWnd == m_tree.m_hWnd)
					{						
						hItem = m_tree.HitTest(point, &flags);
						AddToRight(hItem);
						m_tree.SetFocus();
					}
				}				
			}
			break;		
		case DRAG_TREE:
			{			
				m_tree.SelectDropTarget(NULL);
				ClientToScreen( &point );
				m_tree.ScreenToClient(&point);

				if ((hItem = m_tree.HitTest(point, &flags)) != NULL)
				{
					m_hitemDrop = hItem;			
				}
				
				if( m_hitemDrag == m_hitemDrop )
				{
					m_nCurDrag = DRAG_REST;
					return;
				}

				HTREEITEM	hParent = m_tree.GetParentItem(m_hitemDrag);

				if (m_hitemDrop == NULL)
				{
					CPoint	pt;
					GetCursorPos(&pt);
					CWnd*	pWnd = WindowFromPoint(pt);

					if (pWnd)
					{
						if (pWnd->m_hWnd == m_tree.m_hWnd)
						{
							int	nCount = m_tree.GetChildCount(NULL);
							CString	tmpstr;
							tmpstr.Format("영역 %d", nCount + 1);
							m_hitemDrop = m_tree.AddItem(NULL, tmpstr, nCount + 1);
							m_tree.SelectItem(m_hitemDrop);
							hItem = m_tree.Move(m_hitemDrag, m_hitemDrop);
							
							if (hParent == NULL)
							{
								hParent = m_hitemDrag;
							}
							
							if (!m_tree.ItemHasChildren(hParent))
							{
								DeleteRgn(hParent);
							}					
						}
						else
						{
							if (hParent == NULL)
								DeleteRgn(m_hitemDrag);
							else
								DeleteItem(m_hitemDrag);							
						}
					}				
				}
				else
				{
					hItem = m_tree.Move(m_hitemDrag, m_hitemDrop);
					
					if (hParent == NULL)
						hParent = m_hitemDrag;
					
					if (!m_tree.ItemHasChildren(hParent))
						DeleteRgn(hParent);					
				}				
				
				m_tree.Expand( m_hitemDrop, TVE_EXPAND );
				m_tree.SelectItem(hItem);

				CString		tmpstr;
				HTREEITEM	hChild = m_tree.GetChildItem(NULL);
				int		ii = 0;
				int		nKind = 0;
	
				while (hChild)
				{
					tmpstr.Format("영역 %d", ++ii);
					m_tree.SetItemText(tmpstr, hChild);
					hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
				}

				if (m_hitemDrop)
				{
					hParent = m_tree.GetParentItem(m_hitemDrop);
					
					if (hParent == NULL)
						hParent = m_hitemDrop;

					hChild = m_tree.GetChildItem(hParent);

					while (hChild)
					{
						nKind = m_tree.GetItemData(hChild);

						if (pApp->IsMain(nKind))
							m_tree.SetItemImage(hChild, 3, 3);
						hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
					}
				}
			}
			break;
		}

		m_nCurDrag = DRAG_REST;
	}

	m_tree.ModifyStyle(0, TVS_DISABLEDRAGDROP);
}

void CScreenCfg::SaveInfo()
{
	if (*m_pOutput)
	{
		delete [] *m_pOutput;
		*m_pOutput = NULL;
	}

	
	HTREEITEM	hRoot = m_tree.GetRootItem();
	HTREEITEM	hChild = m_tree.GetChildItem(NULL);
	HTREEITEM	hSubChild = NULL;
	INDI_OBJ*	pIndi = NULL;
	struct _graph*	pGraph = NULL;
	int		nSize = sizeof(struct _graph);
	CIHArray<struct _graph*, struct _graph*> arGraph;
	int		rcnt = 0, ncnt = 0;
	int		ntot = 0, ii = 0;
	char*		ptr = NULL;

	if (hRoot == NULL)
	{
		*m_pOutput = NULL;
		return;
	}

	while (hChild)
	{
		hSubChild = m_tree.GetChildItem(hChild);
		rcnt++;
		ncnt = 0;
		while (hSubChild)
		{
			ncnt++;
			pIndi = m_tree.GetIndiObj(hSubChild);
			pGraph = new struct _graph;
			memcpy(pGraph, pIndi->GetGraph(), nSize);
			pGraph->btRgnKey = rcnt;
			arGraph.Add(pGraph);
			hSubChild = m_tree.GetNextItem(hSubChild, TVGN_NEXT);
		}		

		if (ncnt <= 0)
			rcnt--;
			

		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
	}

	if (m_pgraphUnion)	// 합성차트 추가
		arGraph.Add(m_pgraphUnion);

	ncnt = arGraph.GetSize();

	if (rcnt == 0 || ncnt == 0)
	{
		*m_pOutput = NULL;
		return;
	}
	
	ntot = sz_INTERCHART + SZ_REGION * rcnt + SZ_GRAPH * ncnt;
	*m_pOutput = new char[ntot + 1];
	memset(*m_pOutput, 0x00, ntot + 1);
	ptr = *m_pOutput;
	
	((struct _interChart*)ptr)->rgncnt = rcnt;
	((struct _interChart*)ptr)->grpcnt = ncnt;

	ptr += sz_INTERCHART;
	nSize = SZ_REGION;

	for ( ii = 0 ; ii < rcnt ; ii++ )
	{
		((struct _region*)ptr)->btRKey = ii + 1;
		ptr += nSize;
	}	

	nSize = SZ_GRAPH;

	for ( ii = 0 ; ii < ncnt ; ii++ )
	{
		pGraph = arGraph.GetAt(ii);
		memcpy(ptr, pGraph, nSize);
		ptr += nSize;
	}
}

void CScreenCfg::OnBegindragList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nCurDrag = DRAG_LIST1;
	POINT pt;
	
	if (m_pDragImage) 
	{
		m_pDragImage->DeleteImageList();
		delete m_pDragImage;
	}
	
	m_pDragImage = this->createDragImageEx(&m_listPrice, &pt);

	if (m_pDragImage == NULL) 
	{
		m_nCurDrag = DRAG_REST;
		return;
	}

	CPoint	ptStart = pNMListView->ptAction;

	ptStart -= pt;

	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), ptStart);
	
	//SET THE FLAGS INDICATING A DRAG IN PROGRESS
	m_listPrice.SetFocus();
	
	//CAPTURE ALL MOUSE MESSAGES IN CASE THE USER DRAGS OUTSIDE OF THE VIEW
	SetCapture();	
	*pResult = 0;
}

void CScreenCfg::OnBegindragList2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_nCurDrag = DRAG_LIST2;
	POINT pt;
	
	if (m_pDragImage) 
	{
		m_pDragImage->DeleteImageList();
		delete m_pDragImage;
	}
	
	m_pDragImage = this->createDragImageEx(&m_listAid, &pt);

	if (m_pDragImage == NULL) 
	{
		m_nCurDrag = DRAG_REST;
		return;
	}

	CPoint	ptStart = pNMListView->ptAction;

	ptStart -= pt;

	m_pDragImage->BeginDrag(0, ptStart);
	m_pDragImage->DragEnter(GetDesktopWindow(), ptStart);
	
	//SET THE FLAGS INDICATING A DRAG IN PROGRESS
	m_listAid.SetFocus();
	
	//CAPTURE ALL MOUSE MESSAGES IN CASE THE USER DRAGS OUTSIDE OF THE VIEW
	SetCapture();	
	*pResult = 0;
}

void CScreenCfg::AddToRight(HTREEITEM hItem)
{
	int			nCurSel = 0;
	int			nKind = 0;
	int			nIndex = 0;	
	HTREEITEM		hParent = NULL;
	struct _indiInfo*	pIndi = NULL;
	CAxListCtrl*		pList = NULL;
	CString			tmpstr;
	CString			tmpstr1;
	struct _graph*		pGraph = NULL;
	int			nSize = sizeof(struct _graph);
	CAxisGDlgApp*		pApp = (CAxisGDlgApp*)AfxGetApp();
	bool			bMain = false;
	bool			bPrice = false;

	nCurSel = m_listPrice.GetSelection();

	if (nCurSel < 0)
	{
		nCurSel = m_listAid.GetSelection();

		if (nCurSel < 0)
			return;

		pList = &m_listAid;
	}
	else
	{
		pList = &m_listPrice;
		bPrice = true;
	}

	nKind	= pList->GetItemData(nCurSel);
	tmpstr	= pList->GetItemText(nCurSel, 0);
	pIndi	= pApp->GetIndiObj(nKind);

	m_mapInfo.Lookup(nKind, pGraph);
	tmpstr1 = LIB_MakeParen(pGraph, pIndi);
	tmpstr	+= tmpstr1;

	if (bPrice)
	{
		hParent = GetMainItem();
		
		if (hParent == NULL)
			return;

		HTREEITEM	hChild = m_tree.GetChildItem(hParent);
		bool		bExist = false;

		while (hChild)
		{
			if (nKind == (int)m_tree.GetItemData(hChild))
			{
				bExist = true;
				break;
			}

			hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);			
		}

		if (bExist)		
		{
			int	nCount = m_tree.GetChildCount(NULL);
			CString	title;
		
			title.Format("영역 %d", nCount + 1);
			hItem = m_tree.AddItem(NULL, title, nCount + 1);
			m_tree.SelectItem(hItem);
			hParent = hItem;			
		}

		hItem = m_tree.AddItem(hParent, tmpstr, nKind, pGraph);
	}
	else
	{
		HTREEITEM	hMain = GetMainItem();

		if (hItem != NULL)
		{
			hParent = m_tree.GetParentItem(hItem);

			if (hParent == hMain || hItem == hMain)
				hItem = NULL;
		}

		if (hItem == NULL)
		{
			int	nCount = m_tree.GetChildCount(NULL);
			CString	title;
		
			title.Format("영역 %d", nCount + 1);
			hItem = m_tree.AddItem(NULL, title, nCount + 1);
			m_tree.SelectItem(hItem);
			hParent = m_tree.GetParentItem(hItem);
		}			
		
		if (hParent == NULL)
			m_tree.AddItem(hItem, tmpstr, nKind, pGraph, TVI_FIRST);
		else
			m_tree.AddItem(hParent, tmpstr, nKind, pGraph, hItem);		
	}

	m_tree.Expand(hItem, TVE_EXPAND);
	m_tree.SelectItem(hItem);

	if (hParent == NULL)
		m_tree.SetItemImage(hItem, 1, 1);
	else
		m_tree.SetItemImage(hParent, 1, 1);

	m_tree.Invalidate(FALSE);

}

void CScreenCfg::DeleteItem(HTREEITEM hItem)
{
	HTREEITEM	hParent = NULL;
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();

	if (hItem == NULL)
		return;

	hParent = m_tree.GetParentItem(hItem);

	if (hParent == NULL)
		return;

	int	nKind = m_tree.GetItemData(hItem);
	
	if (!pApp->IsMain(nKind))
		m_tree.DeleteItem(hItem);

	if (!m_tree.ItemHasChildren(hParent))
		DeleteRgn(hParent);
}

void CScreenCfg::DeleteRgn(HTREEITEM hItem)
{	
	HTREEITEM	hParent = NULL;
	int		nMainChart = 0;
	int		nKind = 0;
	int		nCount = 0;
	CArray <HTREEITEM, HTREEITEM>	arTree;
	CString		tmpstr;
	int		ii = 0;
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	bool		bMain = false;

	if (hItem == NULL)
		return;
	
	hParent = m_tree.GetParentItem(hItem);

	if (hParent == NULL)
		hParent = hItem;

	HTREEITEM	hChild = m_tree.GetChildItem(hParent);

	arTree.RemoveAll();

	while (hChild)
	{
		nKind = m_tree.GetItemData(hChild);
		if (pApp->IsMain(nKind))
			bMain = true;
		else
			arTree.Add(hChild);

		hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
	}

	if (!bMain)
	{
		m_tree.DeleteItem(hParent);
		hChild = m_tree.GetChildItem(NULL);

		while (hChild)
		{
			tmpstr.Format("영역 %d", ++ii);
			m_tree.SetItemText(tmpstr, hChild);
			hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
		}
	}
	else
	{	
		nCount = arTree.GetSize();

		for ( ii = 0 ; ii < nCount ; ii++ )
		{
			hItem = arTree.GetAt(ii);
			m_tree.DeleteItem(hItem);
		}		
	}
}

HTREEITEM CScreenCfg::GetMainItem()
{
	HTREEITEM	hParent = m_tree.GetRootItem();
	HTREEITEM	hChild = NULL;
	int		nKind = 0;
	CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
	CString		tmpstr;

	while (hParent)
	{
		tmpstr = m_tree.GetItemText(hParent);
		hChild = m_tree.GetChildItem(hParent);		
		
		while (hChild)
		{
			tmpstr = m_tree.GetItemText(hChild);
			nKind = m_tree.GetItemData(hChild);

			if (pApp->IsMain(nKind))
				return hParent;

			hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
		}

		hParent = m_tree.GetNextItem(hParent, TVGN_NEXT);
	}

	return NULL;
}
LRESULT CScreenCfg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CScreenCfg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			if (m_nCurDrag != DRAG_REST)
			{
				m_nCurDrag = DRAG_REST;
				m_tree.SelectDropTarget(NULL);		
				
				CImageList::DragLeave(this);
				CImageList::EndDrag();
				ReleaseCapture();

				m_pDragImage->DeleteImageList();
				delete m_pDragImage;
				m_pDragImage = NULL;
				return TRUE;
			}
		}
	}	
	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CScreenCfg::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MNG_DEFAULT:
		{
			CAxisGDlgApp*	pApp = (CAxisGDlgApp*)AfxGetApp();
			struct	_graph*	pDefault = NULL;
			struct	_graph* pGraph = NULL;
			int	nKind = 0, nKey = 0, ncnt = 0;

			ncnt = m_arKind.GetSize();

			for ( int ii = 0 ; ii < ncnt ; ii++ )
			{
				nKey = m_arKind.GetAt(ii);
				pDefault = pApp->GetDefault(nKey);
				m_mapInfo.Lookup(nKey, pGraph);	

				if (pDefault == NULL || pGraph == NULL)
					continue;

				pGraph->wCalOption = pDefault->wCalOption;
				pGraph->dwDraOption = pDefault->dwDraOption;
				memcpy(&pGraph->cInfo, &pDefault->cInfo, SZ_CHARTINFO);
				memcpy(&pGraph->tInfo, &pDefault->tInfo, SZ_TICKINFO);
			}	
			
			HTREEITEM	hRoot = m_tree.GetRootItem();
			HTREEITEM	hChild = m_tree.GetChildItem(NULL);
			HTREEITEM	hSubChild = NULL;
			INDI_OBJ*	pObject = NULL;
			CString		tmpstr, tmpstr1;
			struct	_indiInfo*	pIndi = NULL;
			
			if (hRoot == NULL)
				return 0;
			
			while (hChild)
			{
				hSubChild = m_tree.GetChildItem(hChild);
			
				while (hSubChild)
				{
					pObject = m_tree.GetIndiObj(hSubChild);
					nKey = pObject->GetKind();

					if (nKey != GK_CDVV && nKey != GK_EQVV)
					{
						pGraph = pObject->GetGraph();
						pDefault = pApp->GetDefault(nKey);
						pIndi = pApp->GetIndiObj(nKey);
						
						if (pDefault != NULL && pGraph != NULL)
						{
							pGraph->wCalOption = pDefault->wCalOption;
							pGraph->dwDraOption = pDefault->dwDraOption;
							memcpy(&pGraph->cInfo, &pDefault->cInfo, SZ_CHARTINFO);
							memcpy(&pGraph->tInfo, &pDefault->tInfo, SZ_TICKINFO);

							tmpstr = pObject->GetName();

							if (tmpstr.Find("(") >= 0)
								tmpstr = tmpstr.Left(tmpstr.Find("(", 0));

							if (!pApp->IsMain(nKey))
								tmpstr1 = LIB_MakeParen(pGraph, pIndi);

							tmpstr += tmpstr1;
							m_tree.SetItemText(tmpstr, hSubChild);
						}
					}

					hSubChild = m_tree.GetNextItem(hSubChild, TVGN_NEXT);
				}

				hChild = m_tree.GetNextItem(hChild, TVGN_NEXT);
			}

			// 기본값 복원시 합성차트 clear
			int iIndex = ((_envInfo*)m_pEnv)->datainfo.btIndex;
			int	iUnit = ((_envInfo*)m_pEnv)->datainfo.btUnit;
			((_envInfo*)m_pEnv)->datainfo.aaDayInfo[iUnit-GU_CODE][iIndex - GI_DAY].wUnionGKind = NULL;
			if (m_pgraphUnion)
			{
				delete m_pgraphUnion;
				m_pgraphUnion = NULL;
			}
		}
		break;
	}

	return 0;
}
