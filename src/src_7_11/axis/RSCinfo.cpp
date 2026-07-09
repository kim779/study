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

#pragma warning(disable:4996)
CRSCinfo::CRSCinfo(CString home, CWnd* pParent /*=NULL*/)
	: CDialog(CRSCinfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRSCinfo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_home = home;
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
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRSCinfo message handlers

BOOL CRSCinfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	((CButton *)GetDlgItem(IDC_MAPF))->SetCheck(TRUE);
	initList();
	loadInfo();
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
	char		name[32], ver[32], vkey[32], size[32], path[32], buf[256];
	FILE		*fp;
	CString		file, kind;

	m_sRes = selectN;
	if (selectN == S_PLATFORM)
		file.Format("%s\\tab\\infoAXIS", m_home);
	else	file.Format("%s\\tab\\infoRSC", m_home);

	m_list.DeleteAllItems();

	errno_t err = fopen_s(&fp, file, "r");
	if (err)	return;

	//long ltime;
	
	time_t ltime;
	struct tm*	date = NULL;
	int	nItem = 0, nSubItem;
	while (fgets(buf, sizeof(buf), fp) != NULL)
	{
		nSubItem = 0;
		if (selectN == S_PLATFORM)
			sscanf(buf, "%s %s %s", name, ver, size);
		else
		{
			sscanf(buf, "%s %s %s %s %s", name, ver, vkey, size, path);
			kind = path;
			int pos = kind.Find('/');
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

			//2011-11-05 kjs 타임 다시 살펴봐야 함
		  // 	ltime = (long) ;
			
			ltime = (long) atof(vkey);
			date = localtime(&ltime);
//			date = gmtime(&ltime);	gmt time + 9 hour => seoul time
			sprintf_s(vkey, sizeof(vkey), "[%04d/%02d/%02d]  %02d:%02d:%02d ",
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
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

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
		::MessageBox(m_hWnd, "선택된 정보가 없습니다.", "확인", MB_ICONINFORMATION);
		return;
	}

	int	nItem = m_list.GetNextSelectedItem(pos);
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

void CRSCinfo::UpdateInfo(bool rsc)
{
	char		name[32], ver[32], vkey[32], size[32], path[32], buf[256];
	FILE		*fp, *wfp;
	CString		writefile, readfile, delRsc, tmps;

	if (rsc)
	{
		writefile.Format("%s\\tab\\infoRSC", m_home);
		readfile.Format("%s\\tab\\infoRSC_", m_home);
		CopyFile(writefile, readfile, TRUE);

		errno_t err;
		err = fopen_s(&fp, readfile, "r");
		if (fp && m_arDelRsc.GetSize())
		{
			err = fopen_s(&wfp, writefile, "w+");
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				bool	match  = false;
				sscanf_s(buf, "%s %s %s %s %s", name, ver, vkey, size, path);
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
		writefile.Format("%s\\tab\\infoAXIS", m_home);
		readfile.Format("%s\\tab\\infoAXIS_", m_home);
		CopyFile(writefile, readfile, TRUE);

		errno_t err;
		err = fopen_s(&fp, readfile, "r");
		if (fp && m_arDelAxis.GetSize())
		{
			err = fopen_s(&wfp, writefile, "w+");
			while (fgets(buf, sizeof(buf), fp) != NULL)
			{
				bool	match  = false;
				sscanf_s(buf, "%s %s %s", name, ver, size);
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
