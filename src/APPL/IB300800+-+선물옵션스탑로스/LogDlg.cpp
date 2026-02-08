// LogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB300800.h"
#include "LogDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLogDlg dialog


CLogDlg::CLogDlg(CString sFile, CWnd* pParent /*=NULL*/)
	: CDialog(CLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_strLogPath = sFile;
}


void CLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogDlg)
	DDX_Control(pDX, IDC_LIST1, m_LogList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogDlg, CDialog)
	//{{AFX_MSG_MAP(CLogDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_NEXTBUTTON, OnNextbutton)
	ON_BN_CLICKED(IDC_SEARCHBUTTON, OnSearchbutton)
	ON_BN_CLICKED(IDC_CLOSEBUTTON, OnClosebutton)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_RESETBUTTON, OnResetLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogDlg message handlers
int CLogDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

BOOL CLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
	m_LogList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE |LVS_EX_INFOTIP | LVS_EX_GRIDLINES); 	
	
	// TODO: Add extra initialization here
	m_LogList.InsertColumn(0, _T("KEY"), LVCFMT_CENTER, 0, 0);
    m_LogList.InsertColumn(1, _T("날 짜"), LVCFMT_CENTER, 75, 0);
	m_LogList.InsertColumn(2, _T("시 간"), LVCFMT_CENTER, 70, 0);
	m_LogList.InsertColumn(3, _T("   구 분"), LVCFMT_LEFT, 85, 0);
	m_LogList.InsertColumn(4, _T("코드"), LVCFMT_LEFT, 70, 0);
	m_LogList.InsertColumn(5, _T("   종 목"), LVCFMT_LEFT, 80, 0);
    m_LogList.InsertColumn(6, _T("내             용"), LVCFMT_LEFT, 660, 0);
//	
	LoadLogFile();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLogDlg::InsertLog(CString sDate, CString sTime, CString sType, CString sCode, CString sItem, CString sLog, int nItem, int ikey) 
{
	int icnt = m_LogList.GetItemCount();
	if( icnt > 0)
	{
		if(nItem == 0)
			ikey = CInt( m_LogList.GetItemText(0, 0) ) + 1; 
	}

	m_LogList.InsertItem(nItem, CStr(ikey) ); 
	m_LogList.SetItemText(nItem, 1, SetDataFormat(4, sDate, "/") ); 
	m_LogList.SetItemText(nItem, 2, SetDataFormat(6, sTime, ":")); 
	m_LogList.SetItemText(nItem, 3, sType); 
	m_LogList.SetItemText(nItem, 4, sCode); 
	m_LogList.SetItemText(nItem, 5, sItem); 
	m_LogList.SetItemText(nItem, 6, sLog); 



}

void CLogDlg::LoadLogFile()
{
	CFile pFile;
	if (!pFile.Open(m_strLogPath, CFile::modeRead|CFile::shareDenyNone)) return;

	CString strRead, strTmp;
	UINT dFileLength = (UINT)pFile.GetLength();
	strRead.ReleaseBuffer(pFile.Read(strRead.GetBufferSetLength(dFileLength), dFileLength));
	pFile.Close();

	///오늘 로그가 아니면 파일 삭제한다
	CTime time = CTime::GetCurrentTime(); 
	CString sCurDate;
	sCurDate.Format("%04d%02d%02d", time.GetYear(), time.GetMonth(), time.GetDay());    
	strTmp = strRead;
	CString sDate = SplitString(strTmp, tabCHAR);
	if(sCurDate != sDate)
	{
		DeleteFile(m_strLogPath);
		return;
	}
	//////////////////////////////
	

	CStringArray arList, arTabList;
	if( !SplitString(strRead, newlineCHAR, arList, 999) ) return;

	for(int i=arList.GetSize()-1; i>=0; i--)
	{
		if( SplitString(arList[i], tabCHAR, arTabList) < 6) continue;
		InsertLog(arTabList[0], arTabList[1], arTabList[2], arTabList[3], arTabList[4], arTabList[5], m_LogList.GetItemCount(), i+1) ;
		if(m_LogList.GetItemCount() > 20) break; 
	}

	if(arList.GetSize() < 20)
		GetDlgItem(IDC_NEXTBUTTON)->EnableWindow(FALSE);
}

void CLogDlg::LoadLogFile(int istart, int icount)
{
	CFile pFile;
	if (!pFile.Open(m_strLogPath, CFile::modeRead|CFile::shareDenyNone)) return;

	CString strRead, strTmp;
	UINT dFileLength = (UINT)pFile.GetLength();
	strRead.ReleaseBuffer(pFile.Read(strRead.GetBufferSetLength(dFileLength), dFileLength));
	pFile.Close();

	CStringArray arList, arTabList;
	if( !SplitString(strRead, newlineCHAR, arList, 999) ) return;

	istart = arList.GetSize() - istart;
	if(istart < 0) return;
	int iend = istart - icount;
	if(iend < 0) iend = 0;

	for(int i=istart; i>=iend; i--)
	{
		if( SplitString(arList[i], tabCHAR, arTabList) < 6) continue;
		InsertLog(arTabList[0], arTabList[1], arTabList[2], arTabList[3], arTabList[4], arTabList[5], m_LogList.GetItemCount(), i+1) ;
	}

	BOOL bNext = TRUE;
	if(iend == 0) bNext = FALSE;
	GetDlgItem(IDC_NEXTBUTTON)->EnableWindow(bNext);
}

void CLogDlg::OnNextbutton() 
{
	// TODO: Add your control notification handler code here
	int istart = m_LogList.GetItemCount()+1;
	LoadLogFile(istart, 20);
}

void CLogDlg::OnSearchbutton() 
{
	// TODO: Add your control notification handler code here
	m_LogList.DeleteAllItems(); 
	LoadLogFile(1, 20);
}

void CLogDlg::OnClosebutton() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CLogDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if( !GetDlgItem(IDC_STATICF)) return;

	CRect rc = CRect(2, 38, cx-4, cy-4);
	GetDlgItem(IDC_STATICF)->MoveWindow(rc);
	rc.DeflateRect(1,1,1,1); 
	GetDlgItem(IDC_LIST1)->MoveWindow(rc, TRUE);
//1~5= 380
//6=330
	if(rc.Width() > 720)
	{
		m_LogList.SetColumnWidth(6, rc.Width() - 385);
		m_LogList.RedrawWindow(); 
	}
}

void CLogDlg::OnResetLog() 
{
	// TODO: Add your control notification handler code here
	m_LogList.DeleteAllItems(); 
}
