#include "stdafx.h"
#include "axisgdlg.h"
#include "LoadFrameDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoadFrameDlg::CLoadFrameDlg(CWnd* pMain, char* pOutput, int nSize, CWnd* pParent /*=NULL*/)
	: CDialog(CLoadFrameDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLoadFrameDlg)
	//}}AFX_DATA_INIT

	m_pMain = pMain;
	m_pOutput = pOutput;
	m_nSize = nSize;
}

void CLoadFrameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLoadFrameDlg)
	DDX_Control(pDX, IDC_LIST, m_list);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLoadFrameDlg, CDialog)
	//{{AFX_MSG_MAP(CLoadFrameDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CLoadFrameDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString		path;
	CStringArray	arFilename;
	CString		filename, name;

	m_rootPath = "D:\\InnerPrj";
	path.Format("%s\\%s\\%s\\*.srn", m_rootPath, "gex", "scrinfo");	
	LIB_GetFilenameInPath(path, arFilename);	

	int	nCount = arFilename.GetSize(), ii = 0;
	int	nWidth = GetSystemMetrics(SM_CXVSCROLL);
	CRect	rect;

	m_list.GetWindowRect(rect);
	nWidth = rect.Width() - nWidth;
	m_list.DeleteAllItems();

	m_list.InsertColumn(0, "", LVCFMT_LEFT, nWidth, 0);
	m_list.InsertColumn(1, "", LVCFMT_LEFT, 0, 1);	

	for ( ii = 0 ; ii < nCount ; ii++ )
	{
		filename = arFilename.GetAt(ii);
		name = filename.Left(filename.Find(".", 0));
		
		m_list.AddItem(name, ii);
		m_list.SetItemText(ii, 1, filename);
	}

	return TRUE;
}

void CLoadFrameDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	int	nCurSel = 0;
	CString	filename;

	nCurSel = m_list.GetSelection();

	if (nCurSel < 0)
		return;

	filename = m_list.GetItemText(nCurSel, 1);
	memcpy(m_pOutput, filename, filename.GetLength());
	CDialog::OnOK();	
}
