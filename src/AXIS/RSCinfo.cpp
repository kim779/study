// RSCinfo.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "RSCinfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define S_MAP		0
#define	S_DEV		1
#define	S_HELP		2
#define	S_IMAGE		3
#define	S_MTBL		4
#define	S_TAB		5
#define	S_EXE		6
#define	S_PLATFORM	7
#define VERSIONW	140
/////////////////////////////////////////////////////////////////////////////
// CRSCinfo dialog


CRSCinfo::CRSCinfo(CWnd* pParent /*=NULL*/)
	: CDialog(CRSCinfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRSCinfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sRes = S_MAP;
	m_arFolder.Add("map");
	m_arFolder.Add("dev");
	m_arFolder.Add("help");
	m_arFolder.Add("image");
	m_arFolder.Add("mtbl");
	m_arFolder.Add("tab");
	m_arFolder.Add("exe");
}

CRSCinfo::~CRSCinfo()
{
	m_arFolder.RemoveAll();
}

void CRSCinfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRSCinfo)
	DDX_Control(pDX, IDC_INFOLIST, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRSCinfo, CDialog)
	//{{AFX_MSG_MAP(CRSCinfo)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSCinfo message handlers

BOOL CRSCinfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
#ifdef DF_USE_CPLUS17
	m_changer = std::make_unique<CControlChanger>(this);
#else
	m_changer = new CControlChanger(this);
#endif
	
	((CButton *)GetDlgItem(IDC_MAPF))->SetCheck(TRUE);
	initList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRSCinfo::initList()
{
	addColumn(m_list, "파 일 명", 0, 120);
	addColumn(m_list, "버 전 정 보", 1, VERSIONW);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CRSCinfo::loadInfo(int selectN)
{
	char		name[32]{}, ver[32]{}, vkey[32]{}, size[32]{}, path[32]{}, buf[256]{};
	FILE* fp{};
	CString		file, kind;

	m_sRes = selectN;
	if (selectN == S_PLATFORM)
		file.Format("%s\\tab\\infoAXIS", Axis::home);
	else	
		file.Format("%s\\tab\\infoRSC", Axis::home);

	m_list.DeleteAllItems();
	fopen_s(&fp, file, "r");
	if (fp == NULL)	return;

	long ltime{};
	//struct tm*	date = new struct tm;
	std::shared_ptr<struct tm> date = std::make_shared<struct tm>();

	int	nItem = 0, nSubItem{};
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		nSubItem = 0;
		if (selectN == S_PLATFORM)
			sscanf_s(buf, "%s %s %s", name, 32, ver, 32, size, 32); 
		else
		{
			sscanf_s(buf, "%s %s %s %s %s", name, 32, ver, 32, vkey, 32, size, 32, path, 32);
			kind = path;
			const int pos = kind.Find('/');
			if (pos >= 0)
				kind = kind.Left(pos);
			if (kind.Compare(m_arFolder.GetAt(selectN)))
				continue;
		}
		
		addItem(m_list, name, nItem, nSubItem++);
		if (selectN == S_PLATFORM)
		{
			addItem(m_list, ver, nItem, nSubItem++);
		}
		else
		{
			ltime = (long) atof(vkey);
		//	date = localtime(&ltime);
			_localtime32_s(date.get(), &ltime);
//			date = gmtime(&ltime);	gmt time + 9 hour => seoul time
			sprintf(vkey, "[%04d/%02d/%02d]  %02d:%02d:%02d ",
				date->tm_year + 1900,
				date->tm_mon + 1,
				date->tm_mday,
				date->tm_hour,
				date->tm_min,
				date->tm_sec);
			addItem(m_list, vkey, nItem, nSubItem++);
		}
		nItem++;
	}

	fclose(fp);
	fit();
}

BOOL CRSCinfo::addColumn(CListCtrl& list, LPCTSTR columns, int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt      = LVCFMT_CENTER;
	lvc.pszText  = (LPTSTR)columns;
	lvc.cx	     = width;
	lvc.iSubItem = -1;

	return list.InsertColumn(col, &lvc);
}

BOOL CRSCinfo::addItem(CListCtrl& list, CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (char*)(const char*)items;

	if (!subi)	return list.InsertItem(&lvi);
	return list.SetItem(&lvi);
}

void CRSCinfo::fit()
{
	CRect	cRc;
	m_list.GetClientRect(cRc);
	m_list.SetColumnWidth(0, cRc.Width() - VERSIONW);
}

BOOL CRSCinfo::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	switch (LOWORD(wParam))
	{
	case IDC_PLATFORMF:	loadInfo(S_PLATFORM);	break;
	case IDC_EXEF:		loadInfo(S_EXE);	break;
	case IDC_TABF:		loadInfo(S_TAB);	break;
	case IDC_MTBLF:		loadInfo(S_MTBL);	break;
	case IDC_IMAGEF:	loadInfo(S_IMAGE);	break;
	case IDC_HELPF:		loadInfo(S_HELP);	break;
	case IDC_DEVF:		loadInfo(S_DEV);	break;
	case IDC_MAPF:		loadInfo(S_MAP);	break;
	case IDC_CHANGEINFO:	delRsc();		break;
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CRSCinfo::delRsc() 
{
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (!pos)
	{
		//**::MessageBox(m_hWnd, "선택된 정보가 없습니다.", "확인", MB_ICONINFORMATION);
		Axis::MessageBox(this, "선택된 정보가 없습니다.", MB_ICONINFORMATION);
		return;
	}

	const int	nItem = m_list.GetNextSelectedItem(pos);
	switch(m_sRes)
	{
	case 0:	/* map */
	case 1:	/* dev */
	case 2:	/* help */
	case 3:	/* mtbl */
	case 4:	/* image */
	case 5:	/* tab */
	case 6:	/* exe */
		m_arDelRsc.Add(m_list.GetItemText(nItem, 0));
		UpdateInfo();
		break;
	case 7:	/* platform */
		m_arDelAxis.Add(m_list.GetItemText(nItem, 0));
		UpdateInfo(false);
		break;
	default:
		return;
	}

	m_list.DeleteItem(nItem);
}
#pragma warning (disable : 6387 )
void CRSCinfo::UpdateInfo(bool rsc)
{
	char		name[32]{}, ver[32]{}, vkey[32]{}, size[32]{}, path[32]{}, buf[256]{};
	FILE* fp{}, * wfp{};
	CString		writefile, readfile, delRsc, tmps;

	if (rsc)
	{
		writefile.Format("%s\\tab\\infoRSC", Axis::home);
		readfile.Format("%s\\tab\\infoRSC_", Axis::home);
		CopyFile(writefile, readfile, TRUE);

		fopen_s(&fp, readfile, "r");
		if (fp && m_arDelRsc.GetSize())
		{
			fopen_s(&wfp, writefile, "w+");
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				bool	match  = false;
				sscanf_s(buf, "%s %s %s %s %s", name, 32, ver, 32, vkey, 32, size, 32, path, 32);
				for (int ii = 0; ii < m_arDelRsc.GetSize(); ii++)
				{
					delRsc = m_arDelRsc.GetAt(ii);
					if (!delRsc.CompareNoCase(name))
					{
						match = true;
						break;
					}
				}
				if (!match)	fprintf(wfp, buf);
			}

			fclose(wfp);
			fclose(fp);
			DeleteFile(readfile);
		}
	}
	else
	{
		writefile.Format("%s\\tab\\infoAXIS", Axis::home);
		readfile.Format("%s\\tab\\infoAXIS_", Axis::home);
		CopyFile(writefile, readfile, TRUE);

		fopen_s(&fp, readfile, "r");
		if (fp && m_arDelAxis.GetSize())
		{
			fopen_s(&wfp, writefile, "w+");
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				bool	match  = false;
				sscanf_s(buf, "%s %s %s", name, 32, ver, 32, size, 32);
				for (int ii = 0; ii < m_arDelAxis.GetSize(); ii++)
				{
					delRsc = m_arDelAxis.GetAt(ii);
					if (!delRsc.CompareNoCase(name))
					{
						match = true;
						break;
					}
				}
				if (!match)	fprintf(wfp, buf);
			}

			fclose(wfp);
			fclose(fp);
			DeleteFile(readfile);
		}
	}
}
#pragma warning (default : 6387 )

BOOL CRSCinfo::OnEraseBkgnd(CDC* pDC) 
{
	if (Axis::dialogBgColor > 0)
	{
		CRect rc;
		GetClientRect(&rc);
		
		CBrush* oldBrush = pDC->SelectObject(&Axis::dialogBgBrush);

		pDC->PatBlt(0, 0, rc.Width(), rc.Height(), PATCOPY);
		pDC->SelectObject(oldBrush);

		return TRUE;
	}
	else
	{
		return CDialog::OnEraseBkgnd(pDC);
	}
}

HBRUSH CRSCinfo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = Axis::GetCtlBrush(pWnd);

	pDC->SetBkMode(TRANSPARENT);
	if (hbr)
	{
		return hbr;
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}


void CRSCinfo::OnDestroy() 
{
	CDialog::OnDestroy();
#ifndef DF_USE_CPLUS17	
	delete m_changer;	
#endif
}
