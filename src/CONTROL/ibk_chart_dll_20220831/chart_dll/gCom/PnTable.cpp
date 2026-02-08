// PnTable.cpp : implementation file
//

#include "stdafx.h"
#include "axisgcom.h"
#include "PnTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPnTable

CPnTable::CPnTable(CWnd *pView, CWnd* pParent, CFont* pFont, int ctrlID, char* info)
{
	m_pApp = (CAxisGComApp *)AfxGetApp();
	m_pView = pView; 
	m_pParent = pParent;
	m_pFont = pFont;
	m_pGrid = NULL;
}

CPnTable::~CPnTable()
{
}


BEGIN_MESSAGE_MAP(CPnTable, CWnd)
	//{{AFX_MSG_MAP(CPnTable)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(GEV_TABLE, OnTableEvent)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CPnTable message handlers
LRESULT CPnTable::OnTableEvent(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case tbItem:
		setItem((char*)lParam);
		break;
	case tbUpdate:
		setReal(false, (char*)lParam);
		break;
	case tbShift:
		setReal(true, (char*)lParam);
		break;
	case tbInsert:
		insertColumn(true, (char*)lParam);
		break;
	case tbSelect:
		selectIndex(lParam);
		break;
	case tbReset:
		if (m_pGrid)
		{
			m_pGrid->DestroyWindow();
			delete m_pGrid;
			m_pGrid = NULL;
		}
		Invalidate();
		break;
	}

	return TRUE;
}

void CPnTable::OnDestroy() 
{
	if (m_pGrid)
	{
		m_pGrid->DestroyWindow();
		delete m_pGrid;
		m_pGrid = NULL;
	}

	CWnd::OnDestroy();
}

void CPnTable::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect rc;
	GetClientRect(rc);
	if (rc.IsRectEmpty())
		return;

	rc.DeflateRect(1,1);
	if (m_pGrid)
		m_pGrid->MoveWindow(rc);	
}


int CPnTable::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_pGrid)
	{
		m_pGrid = new CPnWndGrid();
		if (!m_pGrid)
			return -1;
		m_pGrid->Create(CRect(0,0,1,1), this, 99999);
		m_pGrid->SetOwner(this);
		m_pGrid->SetFont(m_pFont);

		COLORREF bgColor = GetSysColor(COLOR_BTNFACE);	

		m_pGrid->SetFixedBkColor(bgColor);
		m_pGrid->SetGridColor(RGB(192,192,192));
		m_pGrid->SetScrollShow();
	}

	return 0;
}

void CPnTable::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	COLORREF bgColor = GetSysColor(COLOR_BTNFACE);	
	CRect rect;
	GetClientRect(rect);

	CBrush  *cBrush = m_pApp->GetBrush(m_pView, bgColor);
	dc.FillRect(rect, cBrush);
}

void CPnTable::setItem(char *pData)
{
	if (!m_pGrid)
	{
		m_pGrid = new CPnWndGrid();
		if (!m_pGrid)
			return;
		m_pGrid->Create(CRect(0,0,1,1), this, 99999);
		m_pGrid->SetOwner(this);
		m_pGrid->SetFont(m_pFont);

		COLORREF bgColor = GetSysColor(COLOR_BTNFACE);	

		m_pGrid->SetFixedBkColor(bgColor);
		m_pGrid->SetGridColor(RGB(192,192,192));

		CRect rc;
		GetClientRect(rc);
		rc.DeflateRect(1,1);
		m_pGrid->MoveWindow(rc);	
	}

	char	*buf;
	if (!(buf = setRowCol(pData)))
		return;

	setTable(buf);
}

char* CPnTable::setRowCol(char *pData)
{
	CString tmpS = CString(pData);
	if (tmpS.IsEmpty())
		return NULL;

	int pos;
	if ((pos = tmpS.Find('\n')) < 0)
		return NULL;
	tmpS = tmpS.Left(pos);
	if (tmpS.GetLength() != 10)
		return NULL;

	CString rowS = tmpS.Mid(0, 5);
	CString colS = tmpS.Mid(5, 5);

	m_row = atoi(rowS);
	m_col = atoi(colS);

	if (m_pGrid)
	{
		m_pGrid->DeleteAllItems();
		m_pGrid->SetRowCount(m_row+1);	// 수평
		m_pGrid->SetColumnCount(m_col);	// 수직
		m_pGrid->SetFixedRowCount(1);
	}

	pData += (pos+1);
	return pData;
}

void CPnTable::setTable(char *pData)
{
	CString tmpS = CString(pData);
	if (tmpS.IsEmpty())
		return;

	GV_ITEM item;
	item.mask = GVIF_TEXT|GVIF_FORMAT;
	int idx, pos;
	CString tmpS_, tmpS__;

	int col, row;
	for (row = 0; row < m_row+1; row++)
	{
		if ((idx = tmpS.Find("\n")) < 0)
			break;
		tmpS_ = tmpS.Left(idx);
		item.row = row;
		
		for (col = 0; !tmpS.IsEmpty() && col < m_col; col++)
		{
			if (col == 0)
				item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
			else
			{
				if (row == 0)
					item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
				else
					item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
			}
			item.col = col;
			if ((pos = tmpS_.Find("\t")) < 0)
				break;
			tmpS__ = tmpS_.Left(pos);
			item.szText = tmpS__;

			m_pGrid->SetItem(&item);
			tmpS_ = tmpS_.Mid(pos+1);
		}
		tmpS = tmpS.Mid(idx+1);
	}

	m_pGrid->ResetScrollBars();
	m_pGrid->SetFirst();
}

void CPnTable::setReal(bool bShift, CString realS)
{
	if (!m_pGrid)	return;

	CStringArray stArray;
	stArray.RemoveAll();

	int pos;
	CString tmpS;
	for (int ii = 0; !realS.IsEmpty(); ii++)
	{
		pos = realS.Find("\t");
		if (pos == -1)
			break;
		tmpS = realS.Left(pos);
		tmpS.TrimRight();
		stArray.Add(tmpS);
		realS = realS.Mid(pos+1);
	}
	
	GV_ITEM item;
	item.mask = GVIF_TEXT|GVIF_FORMAT;
	item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	
	if (bShift)
	{
		for (int ii = 0; ii < stArray.GetSize(); ii++)
		{
			if (ii == 0)
			{
				CString dateS = m_pGrid->GetItemText(1, 0);
				m_pGrid->InsertRow(dateS, DT_CENTER|DT_VCENTER|DT_SINGLELINE, 1);
				m_pGrid->DeleteRow(m_pGrid->GetRowCount()-1);
				item.nFormat = DT_CENTER|DT_VCENTER|DT_SINGLELINE;
				setItemData(item, 1, ii, stArray.GetAt(ii));
			}
			else
			{
				item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
				setItemData(item, 1, ii, stArray.GetAt(ii));
			}
		}
	}
	else
	{
		CCellID cellID = m_pGrid->GetTopleftNonFixedCell();
		if (cellID.row == 1)
		{
			for (int ii = 1; ii < stArray.GetSize(); ii++)
			{
				CString	str = stArray.GetAt(ii);
				CString	str2 = m_pGrid->GetItemText(1, ii);
				if (str != str2)
				{
					item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
					setItemData(item, 1, ii, str);

					CRect	rc;
					m_pGrid->GetTextRect(1, ii, &rc);
					rc.InflateRect(3,0);
					InvalidateRect(&rc);
				}
			}
		}
		else
		{
			for (int ii = 1; ii < stArray.GetSize(); ii++)
			{
				CString	str = stArray.GetAt(ii);
				CString	str2 = m_pGrid->GetItemText(1, ii);
				if (str != str2)
				{
					item.nFormat = DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
					setItemData(item, 1, ii, str);
				}
			}
		}
	}

	m_pGrid->Invalidate();
//	m_pGrid->SetFirst();
		
	stArray.RemoveAll();
}

void CPnTable::insertColumn(bool bInsert, CString insertS)
{
}

void CPnTable::selectIndex(int index)
{
	if (!m_pGrid)	return;

	int row = m_pGrid->GetRowCount();
	if (row < 1)	return;

	if (index < 0 || index >= m_pGrid->GetRowCount() - 1)
		return;
	
	if (index >= row-1)	index = row-2;

	CCellID cellID = m_pGrid->GetTopleftNonFixedCell();
	m_pGrid->SetFocusRowCell(index+1, cellID.col);
}

void CPnTable::setItemData(GV_ITEM item, int row, int col, CString text)
{
	if (text.IsEmpty())
		return;
	item.row = row;
	item.col = col;
	item.szText = text;
	m_pGrid->SetItem(&item);
}

void CPnTable::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnLButtonDown(nFlags, point);
}
