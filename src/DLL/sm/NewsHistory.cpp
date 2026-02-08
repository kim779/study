// NewsHistory.cpp : implementation file
//

#include "stdafx.h"
#include "NewsHistory.h"
#include "../../axis/axMsg.hxx"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewsHistory dialog


CNewsHistory::CNewsHistory(bool (*axiscall)(int, WPARAM, LPARAM), CWnd* pParent /*=NULL*/)
	: CDialog(CNewsHistory::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewsHistory)
	//}}AFX_DATA_INIT
	m_axiscall = axiscall;
}


void CNewsHistory::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewsHistory)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewsHistory, CDialog)
	//{{AFX_MSG_MAP(CNewsHistory)
	ON_WM_SIZE()
	ON_LBN_DBLCLK(IDC_LIST, OnDblclkList)
	ON_LBN_KILLFOCUS(IDC_LIST, OnKillfocusList)
	ON_WM_MOUSEMOVE()
	ON_LBN_SETFOCUS(IDC_LIST, OnSetfocusList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewsHistory message handlers

BOOL CNewsHistory::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewsHistory::Init()
{
	m_list.Init();
} 

void CNewsHistory::fit()
{
	if (m_list.GetSafeHwnd())
	{
		CRect rc;
		GetClientRect(rc);
		m_list.MoveWindow(0, 0, rc.Width(), rc.Height());
	}
}

void CNewsHistory::SetData(const CStringArray& ary)
{
	CMapStringToString	fms;
	CString			dat, title, time, source, itemCd;
	LV_ITEM			item{};

//	m_ary.Copy(ary);
	m_ary.RemoveAll();

	for(int ii = 0 ; ii < ary.GetSize(); ii++)
	{
		m_ary.Add(ary.GetAt(ii));
		if(ii > 50)
			break;
	}

	
	item.mask = LVIF_TEXT;

//	const int sourceCount = 13;
	const int sourceCount = 16;
	const char* const sourceNames[sourceCount] = 
	{
//		"시황", "공시", "이투", "인포", "전자", "이데", "", "연합", "머니", "한경", "매경", "아경", "헤경"
		"시황", "공시", "이투", "인포", "전자", "이데", "", "연합", "머니", "한경", "매경", "아경", "헤경", "", "IR", "컨슈"
	};

//	for (int ii = 0; ii < m_ary.GetSize(); ii++)
	for (int ii = 0; ii < m_ary.GetSize(); ii++)
	{
		fms.RemoveAll();

		dat = m_ary.GetAt(ii);	
		parseDat(dat, fms);
		item.iItem = item.iSubItem = 0;

		fms.Lookup(SNEW_TITLE, title);
		fms.Lookup(SNEW_TIME, time);
		fms.Lookup(SNEW_INFO, source);
		fms.Lookup(SNEW_JCODE, itemCd);

		if (time.GetLength() > 4)
		{
			time = time.Left(4);
			time.Insert(2, ':');
		}

		const int sourceIndex = atoi(source) - 1;
		if (sourceIndex >= 0 && sourceIndex < sourceCount)
			source = sourceNames[sourceIndex];
		else
			fms.Lookup(SNEW_NEWSK, source);
		
		m_list.AddItem(time, source, title, itemCd);
	}

	const int minimumCount = 12;

	for (int i = 0; i < minimumCount - m_ary.GetSize(); i++)
	{
		m_list.AddEmptyItem();
	}
	
	fit();
	fms.RemoveAll();

	m_list.SetCurSel(0);

	m_list.SetFocus();
	//OutputDebugString("NEWSHISTORY SETDATA\n");
}

void CNewsHistory::OnKillfocusList()
{
	m_ary.RemoveAll();
	m_list.DeleteAll();
	EndDialog(IDCANCEL);
}

void CNewsHistory::OnDblclkList() 
{
	const int	nItem = m_list.GetCurSel();
	if (nItem == LB_ERR)
		return;

	const LPNEWSITEM item = (LPNEWSITEM)m_list.GetItemDataPtr(nItem);
	ASSERT(item);

	if (item->title.IsEmpty())
		return;

	if (nItem >= 0 && nItem < m_ary.GetSize())
	{
		CString			data, tmps, value;
		CMapStringToString	fms;

		tmps = m_ary.GetAt(nItem);
		parseDat(tmps, fms);

		data = _T("IBNEWSXX");

		if (fms.Lookup(SNEW_TITLE, value))
		{
			tmps.Format("%s\t%s", SNEW_TITLE, value);
			data += tmps;
		}

		if (fms.Lookup(SNEW_JCODE, value))
		{
			if (!value.IsEmpty())
			{
				tmps.Format("\n1%s\t%s", SNEW_JCODE, value);
				data += tmps;
			}
		}

		if (fms.Lookup(SNEW_KEY, value))
		{
			tmps.Format("\n%s\t%s", SNEW_KEY, value);
			data += tmps;

			(*m_axiscall)(AXI_CHANGEPOPUP, 0, (LPARAM)(const char *)data);
		}
		fms.RemoveAll();
	}
	
	m_ary.RemoveAll();
	//m_list.DeleteAll();
	
	EndDialog(IDCANCEL);
}

void CNewsHistory::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	fit();
}

void CNewsHistory::parseDat(CString dat, CMapStringToString& ary)
{
	CString sym, value;
	int	pos{};
	while (!dat.IsEmpty())
	{
		pos = dat.Find('\t');
		if (pos == -1)	break;
		sym = dat.Left(pos++);
		dat = dat.Mid(pos);

		pos = dat.Find('\t');
		if (pos == -1)
		{
			value = dat;
			dat.Empty();
		}
		else
		{
			value = dat.Left(pos++);
			dat = dat.Mid(pos);
		}

		ary.SetAt(sym, value);
	}
}

void CNewsHistory::OnMouseMove(UINT nFlags, CPoint point) 
{	
	
	CDialog::OnMouseMove(nFlags, point);
}

void CNewsHistory::OnSetfocusList() 
{
	// TODO: Add your control notification handler code here
	
}
