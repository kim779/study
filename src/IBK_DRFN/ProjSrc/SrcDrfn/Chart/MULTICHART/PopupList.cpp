// PopupList.cpp : implementation file
//

#include "stdafx.h"
#include "PopupList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPopupList dialog
CPopupList::CPopupList(CHistoryList* HistoryInfo, int type, CWnd* pParent /*=NULL*/)
	: CRscDlg(CPopupList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPopupList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pParent = pParent;
	m_lstItemInfo = HistoryInfo;
	m_lstType = type;
}


void CPopupList::DoDataExchange(CDataExchange* pDX)
{
	CRscDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPopupList)
	DDX_Control(pDX, IDC_LIST, m_lstPopup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPopupList, CRscDlg)
	//{{AFX_MSG_MAP(CPopupList)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST, OnKillfocusList)
	ON_NOTIFY(NM_CLICK, IDC_LIST, OnClickList)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPopupList message handlers

BOOL CPopupList::OnInitDialog() 
{
	CRscDlg::OnInitDialog();
	
	m_lstPopup.SetExtendedStyle(m_lstPopup.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_FLATSB |LVS_EX_TRACKSELECT );

	MakeListCtrlColumn();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPopupList::OnKillfocusList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	PostMessage(WM_CLOSE);	
	
	*pResult = 0;
}

void CPopupList::OnClickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	int inx = m_lstPopup.GetSelectionMark();

	if(inx > -1 && inx < m_lstPopup.GetItemCount())
	{
		//char code[30];
		//memset(code, 0x00, sizeof(code));
		//m_lstPopup.GetItemText(m_lstPopup.GetSelectionMark(), 0, code, sizeof(code));
		//m_pParent->SendMessage(WM_POPUPLIST_SELITEM, m_lstType, (LPARAM)&code);		
		//m_pParent->PostMessage(WM_POPUPLIST_SELITEM, m_lstType, (LPARAM)&code);		
		//PostMessage(WM_CLOSE);		
		memset(m_Code, 0x00, sizeof(m_Code));
		m_lstPopup.GetItemText(m_lstPopup.GetSelectionMark(), 0, m_Code, sizeof(m_Code));
		m_pParent->PostMessage(WM_POPUPLIST_SELITEM, m_lstType, (LPARAM)&m_Code);				
	}
	
	*pResult = 0;
}

void CPopupList::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CRscDlg::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		m_lstPopup.SetFocus();

		MakeInitHistoryIntoListCtrl();	
	}	
}

void CPopupList::MakeListCtrlColumn()
{
	CRect rc;
	GetClientRect(&rc);

	LVCOLUMN lvcolumn;
	lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lvcolumn.fmt = LVCFMT_LEFT;

	if(m_lstType == 0)
	{
		lvcolumn.pszText = _T(" 코 드 ");
		lvcolumn.iSubItem = 0;	
		lvcolumn.cx = int(rc.Width() * 0.3) ;
		m_lstPopup.InsertColumn(0, &lvcolumn);

		lvcolumn.pszText = _T(" 종목명 ");
		lvcolumn.iSubItem = 1;
		lvcolumn.cx = int(rc.Width() * 0.55);
		m_lstPopup.InsertColumn(1, &lvcolumn);

		lvcolumn.pszText = _T("");
		lvcolumn.iSubItem = 2;
		lvcolumn.cx = 3;
		m_lstPopup.InsertColumn(2, &lvcolumn);
	}
//	if(m_lstType == 1)
//	{
//		lvcolumn.pszText = _T("계좌번호");
//		lvcolumn.iSubItem = 0;	
//		lvcolumn.cx = int(rc.Width() * 0.5) ;
//		m_lstPopup.InsertColumn(0, &lvcolumn);
//
//		lvcolumn.pszText = _T(" 계좌명 ");
//		lvcolumn.iSubItem = 1;
//		lvcolumn.cx = int(rc.Width() * 0.4);
//		m_lstPopup.InsertColumn(1, &lvcolumn);
//
//		lvcolumn.pszText = _T("비밀번호");
//		lvcolumn.iSubItem = 2;
//		lvcolumn.cx = 0;
//		m_lstPopup.InsertColumn(2, &lvcolumn);
//
//		lvcolumn.pszText = _T("");
//		lvcolumn.iSubItem = 3;
//		lvcolumn.cx = 3;
//		m_lstPopup.InsertColumn(3, &lvcolumn);
//	}
	if(m_lstType == 2)
	{
		lvcolumn.pszText = _T(" 선 택 ");
		lvcolumn.iSubItem = 0;	
		lvcolumn.cx = int(rc.Width() * 0.75) ;
		m_lstPopup.InsertColumn(0, &lvcolumn);

		lvcolumn.pszText = _T("");
		lvcolumn.iSubItem = 1;
		lvcolumn.cx = 3;
		m_lstPopup.InsertColumn(1, &lvcolumn);

//		lvcolumn.pszText = _T("숨김");
//		lvcolumn.iSubItem = 1;	
//		lvcolumn.cx = 0 ;
//		m_lstPopup.InsertColumn(1, &lvcolumn);
	}
}

void CPopupList::MakeInitHistoryIntoListCtrl()
{
	CString szKey, rValue, szValue;
	LV_ITEM lvitem;	
	lvitem.mask = LVIF_TEXT;
	m_lstPopup.DeleteAllItems();
//	CFont* pFont = m_lstPopup.GetFont();
//	LOGFONT pLogFont;
//	pFont->GetLogFont(&pLogFont);
//	int lfWidth = pLogFont.lfWidth;

	int i=0; 
	CHistory* pHistory;
	for(POSITION pos=m_lstItemInfo->GetTailPosition(); pos; i++)
	{
		pHistory = (CHistory*)m_lstItemInfo->GetPrev(pos);
		
		lvitem.iItem = i;
		lvitem.iSubItem = 0;		
		lvitem.pszText = (LPTSTR)(LPCTSTR)pHistory->_Code;
		m_lstPopup.InsertItem(&lvitem);	
		m_lstPopup.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
		//		m_lstPopup.SetColumnWidth(0, pHistory->_Code.GetLength() * lfWidth);
		
		if(m_lstType != 2)
		{		
			lvitem.mask = LVIF_TEXT;
			lvitem.iItem = i;
			lvitem.iSubItem = 1;
			lvitem.pszText = (LPTSTR)(LPCTSTR)pHistory->_Name;
			m_lstPopup.SetItem(&lvitem);	
			m_lstPopup.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);
		}
		//		m_lstPopup.SetColumnWidth(1, pHistory->_Code.GetLength() * lfWidth);
	}
	AdjustListCtrl();
}

void CPopupList::AdjustListCtrl()
{
	CRect rc;
	UINT uiHeight = 0;
	UINT uiWidth = 0;
	UINT niCount = 0;

	CHeaderCtrl * pHeader = (CHeaderCtrl*)m_lstPopup.GetHeaderCtrl();
	
	VERIFY(pHeader);

	if(pHeader)
	{
		niCount = pHeader->GetItemCount();
		for(UINT i = 0; i < niCount; i++)
		{
			pHeader->GetItemRect(i,&rc);
			uiWidth += rc.Width();
		}
		uiHeight += rc.Height();
	}
	
	niCount = m_lstPopup.GetItemCount();
	if(niCount>0 && niCount <=MAX_ITMESIZE)
	{
		m_lstPopup.GetItemRect(1,&rc,LVIR_BOUNDS);
		uiHeight += (rc.Height() * niCount);
	}
	else if( niCount >MAX_ITMESIZE)// 
	{
		m_lstPopup.GetItemRect(1,&rc,LVIR_BOUNDS);
		uiHeight += (rc.Height() * MAX_ITMESIZE);
		uiWidth += ::GetSystemMetrics(SM_CXVSCROLL);
	}
	else
	{
		if( m_lstType == 0 )
		{
			m_lstPopup.SetColumnWidth(0, 54);
			m_lstPopup.SetColumnWidth(1, 60);
			m_lstPopup.SetColumnWidth(2, 3);
			uiWidth = 117;
		}
		else if( m_lstType == 1 )
		{
			m_lstPopup.SetColumnWidth(0, 86);
			m_lstPopup.SetColumnWidth(1, 60);
			m_lstPopup.SetColumnWidth(2, 3);
			uiWidth = 149;
		}
		uiHeight = 12;
	}
	
//	uiHeight += ::GetSystemMetrics(SM_CYEDGE);
//	uiWidth += ::GetSystemMetrics(SM_CXEDGE);

	// 이부분 수정해야함
//	uiWidth += 10;
	uiHeight += 10;

//	if(uiWidth > 100)
//		uiWidth = 100;
	
	//SetWindowPos(NULL, 0, 0, uiWidth, uiHeight, SWP_NOMOVE | SWP_NOZORDER);
	//m_lstPopup.SetWindowPos(NULL, 0, 0, uiWidth+10, uiHeight, SWP_NOMOVE | SWP_NOZORDER);
	if( niCount > MAX_ITMESIZE )
	{
		uiWidth += 5;
		SetWindowPos(NULL, 0, 0, uiWidth, uiHeight, SWP_NOMOVE | SWP_NOZORDER);
		m_lstPopup.SetWindowPos(NULL, 0, 0, uiWidth, uiHeight, SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL, 0, 0, uiWidth, uiHeight, SWP_NOMOVE | SWP_NOZORDER);
		m_lstPopup.SetWindowPos(NULL, 0, 0, uiWidth+10, uiHeight, SWP_NOMOVE | SWP_NOZORDER);
	}	

	rc = CRect(0, 0, uiWidth, uiHeight);
	CRgn rgn;
	rc.DeflateRect(1,1);
	rgn.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);
	SetWindowRgn(rgn, TRUE);
	rgn.DeleteObject();//KHD
}
