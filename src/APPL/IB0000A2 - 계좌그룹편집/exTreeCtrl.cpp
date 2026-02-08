#include "stdafx.h"
#include "IB0000A2.h"
#include "exTreeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CExTreeCtrl::CExTreeCtrl()
{
	m_pParent = NULL;
	m_pFont = NULL;
	m_ImageList = NULL;
	m_bDrag = false;
	m_bDragging = false;
	m_hItemDrag = NULL;
	m_hItemDrop = NULL;

}

CExTreeCtrl::~CExTreeCtrl()
{
	if (m_ImageList)
		m_ImageList->DeleteImageList();
}


BEGIN_MESSAGE_MAP(CExTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CExTreeCtrl)
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINRDRAG, OnBeginrdrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExTreeCtrl::Init(CWnd *pParent, CFont* pFont)
{
	m_pParent = pParent;
	m_pFont = pFont;
	if (!m_ImageList)
	{
		m_ImageList = std::make_unique<CImageList>();
		m_ImageList->Create(IDB_TREE, 16, 1, RGB(255, 0, 255));
		SetImageList(m_ImageList.release(), TVSIL_NORMAL);
	}
}


void CExTreeCtrl::OnBeginrdrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (!m_bDrag)
		return;
	
	m_hItemDrag = pNMTreeView->itemNew.hItem;
	m_hItemDrop = NULL;
	m_bDragging = true;
	
	*pResult = 0;
}

void CExTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (!m_bDrag)
		return;

	if (m_bDragging)
	{
		SetCursor(LoadCursor(NULL, IDC_SIZEALL));
		HTREEITEM hItem = HitTest(point);
		SelectItem(hItem);
	}
	else
		SetCursor(LoadCursor(NULL,IDC_ARROW));
	
	CTreeCtrl::OnMouseMove(nFlags, point);
}
void CExTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl::OnLButtonUp(nFlags, point);
	if (!m_bDrag || !m_bDragging)
		return;
	
	const int tot = 0;
	m_bDragging = false;
	SetCursor(LoadCursor(NULL,IDC_ARROW));

	m_hItemDrop = HitTest(point);
	if (m_hItemDrag == m_hItemDrop || m_hItemDrop == NULL)
		return;

	HTREEITEM hNewItem = copyItem(m_hItemDrag, GetParentItem(m_hItemDrop), m_hItemDrop);

	DeleteItem(m_hItemDrag);
	SelectItem(hNewItem);
}

HTREEITEM CExTreeCtrl::copyItem(HTREEITEM hItem, HTREEITEM htiNewParent, 
					HTREEITEM htiAfter /*= TVI_LAST*/ )
{
	TV_INSERTSTRUCT         tvstruct{};
		HTREEITEM               hNewItem{};
        CString                 sText;
        
        tvstruct.item.hItem = hItem;
        tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | 
                                TVIF_IMAGE | TVIF_SELECTEDIMAGE;
        GetItem(&tvstruct.item);  
        sText = GetItemText(hItem);
        
        tvstruct.item.cchTextMax = sText.GetLength();
        tvstruct.item.pszText = sText.LockBuffer();

        tvstruct.hParent = htiNewParent;
        tvstruct.hInsertAfter = htiAfter;
        tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
        hNewItem = InsertItem(&tvstruct);
        sText.ReleaseBuffer();

        SetItemData(hNewItem, GetItemData(hItem));
        SetItemState(hNewItem, GetItemState(hItem, TVIS_STATEIMAGEMASK), 
                                                        TVIS_STATEIMAGEMASK);

        return hNewItem;
}
