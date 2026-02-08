// SearchList.cpp : implementation file
//

#include "stdafx.h"
//#include "NeoInputEx.h"
#include "SearchList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearchList

CSearchList::CSearchList()
{
}

CSearchList::~CSearchList()
{
}


BEGIN_MESSAGE_MAP(CSearchList, CWnd)
	//{{AFX_MSG_MAP(CSearchList)
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSearchList message handlers
BOOL CSearchList::Create(CRect rc, CWnd* pParentWnd,LPCTSTR szDefaultText /* = NULL */,LPCTSTR szCustomText  /* = NULL */)
{
	   ASSERT(pParentWnd && ::IsWindow(pParentWnd->GetSafeHwnd()));	  
	   m_pParent  = pParentWnd; 
	   m_parentRect = rc;
	   // Get the class name and create the window
	   CString szClassName = AfxRegisterWndClass(CS_CLASSDC|CS_SAVEBITS|CS_HREDRAW|CS_VREDRAW,
		   0,
		   (HBRUSH) (COLOR_BTNFACE+1), 
		   0);	   		

	   if (!CWnd::CreateEx(0, szClassName, _T(""), WS_POPUP|WS_BORDER, 
		   -200,-200,200,120, // size updated soon
		   pParentWnd->GetSafeHwnd(), 0, NULL))
		   return FALSE;		

	   ShowWindow(SW_HIDE);
	   CRect clRect;
	   GetClientRect(&clRect);

	   clRect.InflateRect(0,0,5,5);
	   MoveWindow(&clRect);   

	   if(m_ListCtrl.GetSafeHwnd() == NULL)
	   {		   
			m_ListCtrl.Create(LVS_REPORT,clRect,this,9090);
//			m_ListCtrl.InsertColumn(0,"종목명",LVCFMT_LEFT,clRect.Width()/2-8,0);
//			m_ListCtrl.InsertColumn(1,"코  드",LVCFMT_LEFT,clRect.Width()/2-8,1);
			m_ListCtrl.SetHeadings( _T("종목명,120;코 드,65") );
			m_ListCtrl.SetExtendedStyle(LVS_EX_FLATSB|LVS_EX_FULLROWSELECT);
	   }
	   return TRUE;	   
}

void CSearchList::FP_vShowList(BOOL bShow)
{
	if(bShow )//&& !IsWindowVisible())
	{
//		m_ListCtrl.Sort(0,TRUE);
		CRect listRect = m_parentRect;
		m_pParent->ClientToScreen(&listRect);
		CRect clRect;
		GetClientRect(&clRect);
		clRect.InflateRect(0,0,2,2);
		m_crClientRect = clRect;
		MoveWindow(listRect.left,listRect.bottom,clRect.Width(),clRect.Height());
		ShowWindow(SW_SHOWNOACTIVATE);
		m_ListCtrl.ShowWindow(SW_SHOWNOACTIVATE);
	}
	else
	{
		CPoint pt(0,0);
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		CRect crClient;
		GetClientRect(&crClient);
		if(!crClient.PtInRect(pt))
		{
			ShowWindow(SW_HIDE);
			m_ListCtrl.ShowWindow(SW_HIDE);			
		}
	}
}

//void CSearchList::FP_vAttachList(CMyListCtrl* list)
void CSearchList::FP_vAttachList(CListCtrl* listCtl)
{
//	AfxMessageBox("한글서치리스트삽입");
	if(m_ListCtrl.GetSafeHwnd() == NULL)
		return;
	if(m_ListCtrl.GetItemCount() > 0 )
		return;
	for(int i=0; i < listCtl->GetItemCount() ; i++)
	{		
		CString csCode = listCtl->GetItemText(i,0);
		CString csName = listCtl->GetItemText(i,1);
		m_ListCtrl.AddItem(csName,csCode);
//		m_ListCtrl.InsertItem(i,csName);
//		m_ListCtrl.SetItemText(i,0,csName);
//		m_ListCtrl.SetItemText(i,1,csCode);
	}	
	m_ListCtrl.Sort(0,TRUE);
//	SortTextItems(0,TRUE);
}

void CSearchList::OnKillFocus(CWnd* pNewWnd) 
{
//	ShowWindow(SW_HIDE);
//	m_ListCtrl.ShowWindow(SW_HIDE);
	CWnd::OnKillFocus(pNewWnd);
}

BOOL CSearchList::PreTranslateMessage(MSG* pMsg) 
{
	
	return CWnd::PreTranslateMessage(pMsg);
}

void CSearchList::FP_vEventUpDown(UINT nKey)
{
	m_ListCtrl.FP_vEventUpDown(nKey);
}

BOOL CSearchList::SortTextItems(int nCol, BOOL bAscending,int low /*= 0*/, int high /*= -1*/ )
{
//	Sleep(0);
//	if( nCol >= pControl->m_ctlColCnt )
//		return FALSE;

	if( high == -1 ) high = m_ListCtrl.GetItemCount() - 1;

	int lo = low;
	int hi = high;
	CString midItem;

	if( hi <= lo ) return FALSE;

	midItem = m_ListCtrl.GetItemText( (lo+hi)/2, nCol );

	// loop through the list until indices cross
	while( lo <= hi )
	{
		// rowText will hold all column text for one row
		CStringArray rowText;

		// find the first element that is greater than or equal to 
		// the partition element starting from the left Index.
		if( bAscending )
			while( ( lo < high ) && ( m_ListCtrl.GetItemText(lo, nCol) < midItem ) )
				++lo;
		else
			while( ( lo < high ) && ( m_ListCtrl.GetItemText(lo, nCol) > midItem ) )
				++lo;

		// find an element that is smaller than or equal to 
		// the partition element starting from the right Index.
		if( bAscending )
			while( ( hi > low ) && ( m_ListCtrl.GetItemText(hi, nCol) > midItem ) )
				--hi;
		else
			while( ( hi > low ) && ( m_ListCtrl.GetItemText(hi, nCol) < midItem ) )
				--hi;

		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( m_ListCtrl.GetItemText(lo, nCol) != m_ListCtrl.GetItemText(hi, nCol))
			{
				// swap the rows
				LV_ITEM lvitemlo, lvitemhi;
				int nColCount = 2;
				rowText.SetSize( nColCount );
				int i;
				for( i=0; i<nColCount; i++)
					rowText[i] = m_ListCtrl.GetItemText(lo, i);
				lvitemlo.mask = LVIF_IMAGE | LVIF_PARAM | LVIF_STATE;
				lvitemlo.iItem = lo;
				lvitemlo.iSubItem = 0;
				lvitemlo.stateMask = LVIS_CUT | LVIS_DROPHILITED | 
						LVIS_FOCUSED |  LVIS_SELECTED | 
						LVIS_OVERLAYMASK | LVIS_STATEIMAGEMASK;

				lvitemhi = lvitemlo;
				lvitemhi.iItem = hi;

				m_ListCtrl.GetItem( &lvitemlo );
				m_ListCtrl.GetItem( &lvitemhi );

				for( i=0; i<nColCount; i++)
					m_ListCtrl.SetItemText(lo, i, m_ListCtrl.GetItemText(hi, i));

				lvitemhi.iItem = lo;
				m_ListCtrl.SetItem( &lvitemhi );

				for( i=0; i<nColCount; i++)
					m_ListCtrl.SetItemText(hi, i, rowText[i]);

				lvitemlo.iItem = hi;
				m_ListCtrl.SetItem( &lvitemlo );
			}

			++lo;
			--hi;
		}
	}

	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortTextItems( nCol, bAscending , low, hi);

	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortTextItems( nCol, bAscending , lo, high );
	
	return TRUE;
}

CString g_DupString;

void CSearchList::SetSelection(CString str, int Column)
{
	int count = m_ListCtrl.GetItemCount();
	int index = 0;
	CString text;
//	int n;

	if(!str.Compare(g_DupString) || str.IsEmpty())
		return;
	
	g_DupString = str;

	if(Column ==0)
	{
//		while((n = str.Find('.')) != -1)
//			str = str.Left(n) + str.Mid(n+1);
	}

	while(index < count) 
	{
		text = m_ListCtrl.GetItemText(index++, Column);

		if(Column == 0)
		{	
//			while((n = text.Find('.')) != -1)
//				text = text.Left(n) + text.Mid(n+1);
		}

		if(TRUE)
		{
			if(text.CompareNoCase(str)>=0)
			{
				Selection(index-1);
				return;
			}
		}
		else
		{
			if(text.CompareNoCase(str)<=0)
			{
				Selection(index -1);
				return;
			}
		}
	}
}


void CSearchList::Selection(int index)
{
//	m_ListCtrl.m_nCurFocus = index;
	m_ListCtrl.SetItemState(index, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
	int nBottom = m_ListCtrl.GetCountPerPage()+index;
	m_ListCtrl.EnsureVisible(nBottom, true);
	m_ListCtrl.EnsureVisible(index, false);
}

CString CSearchList::FP_csEditEnter()
{
	CString csData =  m_ListCtrl.FP_csEditEnter();
	ShowWindow(SW_HIDE);
	return csData;
}

#include "inputcontrol.h"
//#include "../CONTENTS/MultiChart/inputcontrol.h"
void CSearchList::FP_vEventSeachList(CString csData)
{
	((CInputControl*)m_pParent)->FP_vEventSeachList(csData);
}

LRESULT CSearchList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
//	TRACE("message = %d , wParam = %d , lParam = %d\n",message,wParam,lParam);	
	if(message == 877 && wParam == 4)
		FP_vShowList(FALSE);
	return CWnd::WindowProc(message, wParam, lParam);
}
