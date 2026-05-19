// ResourceHistDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ResourceHistDlg.h"
#include "afxdialogex.h"
#include "FormLoadDlg.h"
#include "h/axisrsm.h"
#include "download.h"
#include "wsock.h"
#include "MainFrm.h"
#include "ChildFrm.h"


// CResourceHistDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CResourceHistDlg, CDialogEx)

CResourceHistDlg::CResourceHistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResourceHistDlg::IDD, pParent)
{
	m_row = -1;
	m_pDown = NULL;
}

CResourceHistDlg::~CResourceHistDlg()
{
}

void CResourceHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResourceHistDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CResourceHistDlg::OnBnClickedBtnSearch)
	ON_MESSAGE(WM_USER, OnReceiveClose)
	ON_BN_CLICKED(IDC_BTN_DIFF, &CResourceHistDlg::OnBnClickedBtnDiff)
END_MESSAGE_MAP()


// CResourceHistDlg 메시지 처리기입니다.
BOOL CResourceHistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int sHeight;
	
	CRect rc, mRc, listRc;
	GetClientRect(&mRc);

	((CStatic*)GetDlgItem(IDC_CAPTION1))->GetWindowRect(&rc);
	ScreenToClient(rc);
	
	listRc.top = rc.bottom + GAP;

	// 조회 버튼
	rc.top -= 2; rc.bottom += 4;
	rc.right = mRc.right - GAP - DIST_GAP ; rc.left = rc.right - 50;
	((CButton*)GetDlgItem(IDC_BTN_SEARCH))->MoveWindow(&rc);
	

	rc.OffsetRect(- rc.Width() - 5, 0);
	((CButton*)GetDlgItem(IDC_BTN_DIFF))->MoveWindow(&rc);




	// 하단 캡션 위치 조정	
	((CStatic*)GetDlgItem(IDC_CAPTION2))->GetWindowRect(&rc);
	sHeight = rc.Height();
	rc.bottom = mRc.bottom - GAP;
	rc.top = rc.bottom - sHeight;
	((CStatic*)GetDlgItem(IDC_CAPTION2))->MoveWindow(&rc);
	
	// 그리드 영역
	listRc.left = GAP;	
	listRc.right = mRc.right - GAP;		
	listRc.bottom = rc.top - GAP;


	if (!m_ResourceList.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, listRc, this, IDC_LIST_RESOURCE))
		return FALSE;

	m_ResourceList.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE);
	m_ResourceList.InsertColumn(0, _T("ID_No."), LVCFMT_CENTER, 60);
	m_ResourceList.InsertColumn(1, _T("File Name"), LVCFMT_CENTER, 70);
	m_ResourceList.InsertColumn(2, _T("Version"), LVCFMT_CENTER, 90);
	m_ResourceList.InsertColumn(3, _T("        Developer"), LVCFMT_LEFT, 110);
	m_ResourceList.InsertColumn(4, _T("Dev Date."), LVCFMT_CENTER, 120);
	m_ResourceList.InsertColumn(5, _T("Release"), LVCFMT_CENTER, 60);
	m_ResourceList.InsertColumn(6, _T("     Release User"), LVCFMT_LEFT, 110);
	m_ResourceList.InsertColumn(7, _T("Release Date."), LVCFMT_CENTER, 120);
	m_ResourceList.InsertColumn(8, _T("                                      Note."), LVCFMT_LEFT, 280);
	
	((CEdit*)GetDlgItem(IDC_MAP_NAME))->SetWindowTextA(m_sMap);


	setFont();
	OnBnClickedBtnSearch();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CResourceHistDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	SetWindowPos(NULL, cx, cy, 1050, 595, SWP_NOMOVE);
}


void CResourceHistDlg::setFont()
{
	::DeleteObject(m_font.Detach());

	LOGFONT	lf;
	NONCLIENTMETRICS info;

	afxGlobalData.fontRegular.GetLogFont(&lf);

	info.cbSize = sizeof(info);
	afxGlobalData.GetNonClientMetrics(info);

	lf.lfHeight = info.lfMenuFont.lfHeight;
	lf.lfWeight = info.lfMenuFont.lfWeight;
	lf.lfItalic = info.lfMenuFont.lfItalic;

	m_font.CreateFontIndirect(&lf);
	m_ResourceList.SetFont(&m_font);	
}

void CResourceHistDlg::SetGridData(char* pchData)
{
	struct o_loadhistrsc* gridData = (struct o_loadhistrsc*)pchData;

	CString strData;
	CString strDate;
		
	for (int ii = 0; ii < atoi(CString(gridData->nrec, sizeof(gridData->nrec))); ii++)
	{		
		m_ResourceList.AddItem(ii, 0, CString(gridData->rec[ii].idno, sizeof(gridData->rec[ii].idno)));
		
		strData = CString(gridData->rec[ii].fname, sizeof(gridData->rec[ii].fname));
		m_ResourceList.AddItem(ii, 1, strData.Trim());
		
		strData = CString(gridData->rec[ii].ver1, sizeof(gridData->rec[ii].ver1));
		strData += "." + CString(gridData->rec[ii].ver2, sizeof(gridData->rec[ii].ver2));
		strData += "." + CString(gridData->rec[ii].ver3, sizeof(gridData->rec[ii].ver3));
		m_ResourceList.AddItem(ii, 2, strData);

		strData = CString(gridData->rec[ii].usid, sizeof(gridData->rec[ii].usid));
		m_ResourceList.AddItem(ii, 3, strData.Trim());

		strData = CString(gridData->rec[ii].udat, sizeof(gridData->rec[ii].udat));
		strData.Insert(4,"/"); strData.Insert(7,"/");  strData.Insert(10," ");
		strData.Insert(13,":"); strData.Insert(16,":");
		m_ResourceList.AddItem(ii, 4, strData);

		m_ResourceList.AddItem(ii, 5, CString(gridData->rec[ii].rchk, sizeof(gridData->rec[ii].rchk)));

		strData = CString(gridData->rec[ii].rlid, sizeof(gridData->rec[ii].rlid));
		m_ResourceList.AddItem(ii, 6, strData.Trim());
		
		strData = CString(gridData->rec[ii].rdat, sizeof(gridData->rec[ii].rdat));
		if (!strData.Trim().IsEmpty())
		{
			strData.Insert(4,"/"); strData.Insert(7,"/");  strData.Insert(10," ");
			strData.Insert(13,":"); strData.Insert(16,":");
		}
		m_ResourceList.AddItem(ii, 7, strData);
		
		strData = CString(gridData->rec[ii].mdsc, sizeof(gridData->rec[ii].mdsc));
		m_ResourceList.AddItem(ii, 8, strData.Trim());
	}	
}


BOOL CResourceHistDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR pHdr;
	LPNMLISTVIEW pNlv;

	pHdr = (LPNMHDR)lParam;
	pNlv = (LPNMLISTVIEW)lParam;

	switch (LOWORD(wParam))
	{	
	case IDC_LIST_RESOURCE:
		{
			switch(pHdr->code)
			{
			case NM_CLICK:
				{
					int row, col;
					CString tmp;
					row = m_ResourceList.m_FocusRow;
					col = m_ResourceList.m_FocusCol;

					tmp = m_ResourceList.GetItemText(row, 1);

					((CEdit*)GetDlgItem(IDC_MAP_NAME))->SetWindowTextA(tmp);
				}
				break;
			case NM_DBLCLK:
				{
					int row, col;
					CString tmp, tmps;
					bool bgrid = false;
					CString text;

					row = m_ResourceList.m_FocusRow;
					col = m_ResourceList.m_FocusCol;

					tmp = m_ResourceList.GetItemText(row, 0);
					tmps = m_ResourceList.GetItemText(row, 1);

					//m_formDlg = new CFormLoadDlg(this, tmp, tmps);
					//m_formDlg->Create(IDD_FORM_LOAD);
					//m_formDlg->DoModal();	
					CMainFrame* pMainFrm = NULL;
					CChildFrame* pChild = NULL;
					while (pMainFrm == NULL)
						pMainFrm = (CMainFrame*)AfxGetMainWnd();
					
					tmps = tmps.MakeUpper();
					if (tmps.Find(_T(".XML")) == -1)
						tmps += _T(".XML");

					pMainFrm->DownMapDlg(tmps, 0, atoi(tmp));
				//	pMainFrm->openFile(m_sPath);			// open file in user's form-source path?
					tmp.Format("%s\\exe\\tmp\\%s", m_sRoot, tmps);	// open download-file in temporary path
					pMainFrm->openFile(tmp);		
				}
				break;
			}
		}
		break;
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void CResourceHistDlg::OnBnClickedBtnSearch()
{
	// Reflash Grid
	m_ResourceList.DeleteAllItems();

	CString sRoot;
	sRoot.Format("%s\\exe\\tmp\\history.dat", m_sRoot);
	m_pDown = new CDownload();
	m_pDown->Create(NULL, NULL, WS_CHILD, CRect(0, 0, 0, 0), this, 10);
	m_pDown->Download(m_sID, m_sMap, "", sRoot, 7);

	SAFE_DELETEW(m_pDown);
	
}

LRESULT CResourceHistDlg::OnReceiveClose(WPARAM wParam, LPARAM lParam)
{
	CString sFile;
	CString sData;
	sFile.Format("%s\\exe\\tmp\\history.dat", m_sRoot);

	HANDLE hFile = CreateFile(sFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
		return false;

	DWORD dwDataL = GetFileSize(hFile, NULL);
	DWORD dwReadLen = 0;
	BOOL  bRc = ReadFile(hFile, sData.GetBuffer(dwDataL), dwDataL, &dwReadLen, NULL);
	CloseHandle(hFile);
	sData.ReleaseBuffer();
	DeleteFile(sFile);

	SetGridData(sData.GetBuffer(0));
	return 0;
}

void CResourceHistDlg::OnBnClickedBtnDiff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString tmp, tmps;
	bool bgrid = false;

	int row = m_ResourceList.GetSelectionMark();
	tmp  = m_ResourceList.GetItemText(row, 0);
	tmps = m_ResourceList.GetItemText(row, 1);

	CMainFrame* pMainFrm = nullptr;
	while (pMainFrm == nullptr)
		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	int nSel = atoi(tmp);
	tmp = m_ResourceList.GetItemText(row + 1, 0);
	if (tmp.IsEmpty())
		return;

	int nPre = atoi(tmp);
	tmps = tmps.MakeUpper();	
	if (tmps.Find(_T(".XML")) == -1)
		tmps += _T(".XML");

	if (!pMainFrm->DownDiff(tmps, nSel, nPre))
		return;

	tmp.Format("\"%s\\exe\\tmp\\%s_rev%d\" \"%s\\exe\\tmp\\%s_rev%d\"", m_sRoot, tmps, nSel, m_sRoot, tmps, nPre);
	CString strDiffViewer = pMainFrm->GetMergePath();

	ShellExecute(NULL, "open", strDiffViewer, tmp, NULL, SW_SHOW);
}
