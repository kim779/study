// ResourceHistDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ResourceHistDlg.h"
#include "afxdialogex.h"
#include "FormLoadDlg.h"
#include "../h/axisrsm.h"
#include "awSock/download.h"
#include "awSock/wsock.h"
#include "MainFrm.h"
#include "ChildFrm.h"
#include "h/mainvar.h"

// CResourceHistDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CResourceHistDlg, CDialogEx)

CResourceHistDlg::CResourceHistDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CResourceHistDlg::IDD, pParent)
{
	m_row = 0;
	m_col = 0;
	m_pDown = NULL;
	m_seqn = NULL;
}

CResourceHistDlg::~CResourceHistDlg()
{
	if (m_seqn) free(m_seqn);
}

void CResourceHistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResourceHistDlg, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CResourceHistDlg::OnBnClickedBtnSearch)
	ON_MESSAGE(WM_USER + 132, OnReceiveClose)
	ON_BN_CLICKED(IDC_BTN_NEXT, &CResourceHistDlg::OnBnClickedBtnNext)
	ON_BN_CLICKED(IDC_BTN_PREV, &CResourceHistDlg::OnBnClickedBtnPrev)
	ON_BN_CLICKED(IDC_BTN_CHAGNES, &CResourceHistDlg::OnBnClickedBtnChagnes)
END_MESSAGE_MAP()


// CResourceHistDlg 메시지 처리기입니다.
BOOL CResourceHistDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	int sHeight;
	
	CRect rc, mRc, listRc, aRc;
	GetClientRect(&mRc);

	((CStatic*)GetDlgItem(IDC_CAPTION1))->GetWindowRect(&rc);
	ScreenToClient(rc);
	
	listRc.top = rc.bottom + GAP;

	// 조회 버튼
	rc.top -= 2; rc.bottom += 4;
	rc.right = mRc.right - GAP - DIST_GAP ; rc.left = rc.right - 50;
	((CButton*)GetDlgItem(IDC_BTN_SEARCH))->MoveWindow(&rc);

	aRc.CopyRect(rc);
	aRc.OffsetRect(-180, 0);
	((CButton*)GetDlgItem(IDC_BTN_CHAGNES))->MoveWindow(&aRc);

	aRc.OffsetRect(aRc.Width() + 10, 0);
	((CButton*)GetDlgItem(IDC_BTN_PREV))->MoveWindow(&aRc);
	aRc.OffsetRect(aRc.Width() + 5, 0);
	((CButton*)GetDlgItem(IDC_BTN_NEXT))->MoveWindow(&aRc);
	
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
	m_ResourceList.InsertColumn(0, _T(""), LVCFMT_CENTER, 0);
	m_ResourceList.InsertColumn(1, _T("ID No"), LVCFMT_CENTER, 60);
	m_ResourceList.InsertColumn(2, _T("File Name"), LVCFMT_CENTER, 70);
	m_ResourceList.InsertColumn(3, _T("Version"), LVCFMT_CENTER, 90);
	m_ResourceList.InsertColumn(4, _T("Developer"), LVCFMT_CENTER, 80);
	m_ResourceList.InsertColumn(5, _T("Update time"), LVCFMT_CENTER, 120);
	m_ResourceList.InsertColumn(6, _T("Release user"), LVCFMT_CENTER, 80);
	m_ResourceList.InsertColumn(7, _T("Release time"), LVCFMT_CENTER, 120);
	m_ResourceList.InsertColumn(8, _T("message"), LVCFMT_LEFT, 393);
	
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
	char seqn[9] = { 0, };
	memcpy(seqn, gridData->seqn, sizeof(gridData->seqn));
	if (m_seqn == NULL)
	{
		m_seqn = (char*)malloc(SEQN_MAX);
		ZeroMemory(m_seqn, SEQN_MAX);
	}
	sprintf_s(seqn, _T("%08d"), atoi(gridData->seqn));
	memcpy(m_seqn, seqn, SEQN_MAX);
	m_ResourceList.m_ArryMdsc.RemoveAll();
	for (int ii = 0; ii < atoi(CString(gridData->nrec, sizeof(gridData->nrec))); ii++)
	{		
		m_ResourceList.AddItem(ii, 0, "");
		strData.Format("%d", atoi(CString(gridData->rhist[ii].seqn, sizeof(gridData->rhist[ii].seqn))));
		m_ResourceList.AddItem(ii, 1, strData.Trim());
		
		strData = CString(gridData->fnam, sizeof(gridData->fnam));
		m_ResourceList.AddItem(ii, 2, strData.Trim());
		
		strData.Format("%04d.%04d.%04d", atoi(gridData->rhist[ii].ver1),
			atoi(gridData->rhist[ii].ver2),
			atoi(gridData->rhist[ii].ver3));
		m_ResourceList.AddItem(ii, 3, strData);

		strData = CString(gridData->rhist[ii].usid, sizeof(gridData->rhist[ii].usid));
		m_ResourceList.AddItem(ii, 4, strData.Trim());

		strData = CString(gridData->rhist[ii].udat, sizeof(gridData->rhist[ii].udat));
		strData.Insert(4,"/"); strData.Insert(7,"/");  strData.Insert(10," ");
		strData.Insert(13,":"); strData.Insert(16,":");
		m_ResourceList.AddItem(ii, 5, strData);

		strData = CString(gridData->rhist[ii].ruid, sizeof(gridData->rhist[ii].ruid));
		m_ResourceList.AddItem(ii, 6, strData.Trim());
		strData = CString(gridData->rhist[ii].rdat, sizeof(gridData->rhist[ii].rdat));
		strData.Insert(4, "/"); strData.Insert(7, "/");  strData.Insert(10, " ");
		strData.Insert(13, ":"); strData.Insert(16, ":");
		m_ResourceList.AddItem(ii, 7, strData);
		strData = CString(gridData->rhist[ii].mdsc, sizeof(gridData->rhist[ii].mdsc));
		m_ResourceList.m_ArryMdsc.Add(strData);
		strData.Replace("\r\n", " ");
		m_ResourceList.AddItem(ii, 8, strData.Trim());
		
		//strData = CString(gridData->rhist[ii].rdat, sizeof(gridData->rhist[ii].rdat));
		//if (!strData.Trim().IsEmpty())
		//{
		//	strData.Insert(4,"/"); strData.Insert(7,"/");  strData.Insert(10," ");
		//	strData.Insert(13,":"); strData.Insert(16,":");
		//}
		//m_ResourceList.AddItem(ii, 7, strData);
		//
		//strData = CString(gridData->rhist[ii].mdsc, sizeof(gridData->rhist[ii].mdsc));
		//m_ResourceList.AddItem(ii, 8, strData.Trim());
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
					CString tmp;
					m_row = m_ResourceList.m_FocusRow;
					m_col = m_ResourceList.m_FocusCol;

					tmp = m_ResourceList.GetItemText(m_row, 2);

					((CEdit*)GetDlgItem(IDC_MAP_NAME))->SetWindowTextA(tmp);
				}
				break;
			case NM_DBLCLK:
				{
					int row;
					CString tmp, tmps;
					bool bgrid = false;
					CString text;

					row = m_ResourceList.m_FocusRow;

					tmp = m_ResourceList.GetItemText(row, 1);
					tmps = m_ResourceList.GetItemText(row, 2);
					
					CMainFrame* pMainFrm = NULL;
					CChildFrame* pChild = NULL;
					while (pMainFrm == NULL)
						pMainFrm = (CMainFrame*)AfxGetMainWnd();
					
					tmps = tmps.MakeUpper();
					//if (tmps.Find(_T(".XML")) == -1)
					//	tmps += _T(".XML");

					if (!pMainFrm->DownMapDlg(tmps, (int)'W', atoi(tmp)))
					{
						break;
					}
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
	SearchHistory('S', ' ', m_seqn);
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

void CResourceHistDlg::OnBnClickedBtnNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SearchHistory('S', 'N', m_seqn);
}


void CResourceHistDlg::OnBnClickedBtnPrev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	SearchHistory('S', 'P', m_seqn);
}

void CResourceHistDlg::SearchHistory(char Type, char dirt, char* seqn)
{
	m_ResourceList.DeleteAllItems();

	CString sRoot;
	CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
	sRoot.Format("%s\\exe\\tmp\\history.dat", m_sRoot);

	CDownload* dn = new CDownload();
	if (!dn->Create())
	{
		delete dn;
	}
	dn->SetParent(this);
	dn->setHistoryData(Type, dirt, seqn);
	dn->Download(UserID, m_sMap, _T(""), sRoot, TRGB_FORMHISTORY);
	delete dn;
}

void CResourceHistDlg::OnBnClickedBtnChagnes()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strNew, strDiffData;
	int nNewSel, nOldSel;
	bool bgrid = false;
	CString text;

	nNewSel = atoi(m_ResourceList.GetItemText(m_row, 1));
	strNew = m_ResourceList.GetItemText(m_row, 2);

	nOldSel = atoi(m_ResourceList.GetItemText(m_row + 1, 1));
	//strOld = m_ResourceList.GetItemText(m_row - 1, 2);

	CMainFrame* pMainFrm = NULL;
	CChildFrame* pChild = NULL;
	while (pMainFrm == NULL)
		pMainFrm = (CMainFrame*)AfxGetMainWnd();

	strNew = strNew.MakeUpper();
	//if (tmps.Find(_T(".XML")) == -1)
	//	tmps += _T(".XML");

	
	if (!pMainFrm->DownDiff(strNew, nNewSel, nOldSel))
	{
		return;
	}
	strDiffData.Format("\"%s\\exe\\tmp\\%s_rev%d\" \"%s\\exe\\tmp\\%s_rev%d\"", m_sRoot, strNew, nNewSel, m_sRoot, strNew, nOldSel);

	CString strDiffViewer = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_DIFFVIEWER, 0), 0);
	ShellExecute(NULL, "open", strDiffViewer, strDiffData, NULL, SW_SHOW);
}
