// SearchDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SearchDlg.h"
#include "SearchListCtrl.h"
#include "../awSock/wsock.h"
#include "../awSock/upload.h"
#include "../awSock/download.h"
#include "../h/mainvar.h"
#include "../h/workio.h"
#include "XLAutomation.h"
#include "XLEzAutomation.h"


// CSearchDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSearchDlg, CDialogEx)

CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearchDlg::IDD, pParent)
{

}

CSearchDlg::~CSearchDlg()
{
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_OBJECT, m_comboObject);
}


BEGIN_MESSAGE_MAP(CSearchDlg, CDialogEx)
	ON_COMMAND_RANGE(IDC_CHECK_DATE, IDC_CHECK_ALL, &CSearchDlg::OnCheckCommand)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CSearchDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_RESEARSH, &CSearchDlg::OnBnClickedCheckResearsh)
	ON_MESSAGE(WM_USER + 132, OnReceiveClose)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &CSearchDlg::OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CSearchDlg::OnBnClickedButtonSave)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_EDATE, &CSearchDlg::OnDtnDatetimechangeEdate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_SDATE, &CSearchDlg::OnDtnDatetimechangeSdate)
	ON_BN_CLICKED(IDC_BUTTON_EXCEL, &CSearchDlg::OnBnClickedButtonExcel)
END_MESSAGE_MAP()


// CSearchDlg 메시지 처리기입니다.


BOOL CSearchDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rc;
	GetDlgItem(IDC_STATIC_LIST)->GetWindowRect(rc);
	this->ScreenToClient(rc);
	if (!m_listResult.Create(WS_CHILD | WS_VISIBLE | WS_BORDER | WS_HSCROLL | LVS_REPORT | LVS_ALIGNTOP | LVS_NOSORTHEADER | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, rc, this, IDC_LIST_RESULT))
		return FALSE;
	//340 480 130 650 
	m_listResult.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT | LVS_EX_ONECLICKACTIVATE);
	m_listResult.InsertColumn(0, _T("파일명"), LVCFMT_LEFT, 128);
	m_listResult.InsertColumn(1, _T("파일설명"), LVCFMT_LEFT, 178);
	m_listResult.InsertColumn(2, _T("작성자"), LVCFMT_LEFT, 158);
	m_listResult.InsertColumn(3, _T("작성일"), LVCFMT_LEFT, 170);
	
	char* szCombo[] = {
		_T("Label"),
		_T("Box"),
		_T("Group"),
		_T("Panel"),
		_T("Push Button"),
		_T("Radio Button"),
		_T("CheckBox"),
		_T("Edit"),
		_T("Combo"),
		_T("Output"),
		_T("Memo"),
		_T("Grid"),
		_T("GridEx"),
		_T("Table"),
		_T("Treeview"),
		_T("Object"),
		_T("Tab"),
		_T("Browser") };

	char* szObjectName[] = {
		_T("<LABEL>"),
		_T("<BOX>"),
		_T("<GROUP>"),
		_T("<PANEL>"),
		_T("TP_PUSH"),
		_T("TP_RADIO"),
		_T("TP_CHECK"),
		_T("<EDIT>"),
		_T("<COMBO>"),
		_T("<OUT>"),
		_T("<MEMO>"),
		_T("<GRID>"),
		_T("<GRIDEX>"),
		_T("<TABLE>"),
		_T("<TREEVIEW>"),
		_T("<OBJECT>"),
		_T("<USRTAB>"),
		_T("<BROWSER>") };

	for (int ii = 0; ii < sizeof(szCombo) / sizeof(char*); ii++)
	{
		m_comboObject.InsertString(ii, szCombo[ii]);
		m_aryObject.Add(szObjectName[ii]);
	}

	((CEdit*)GetDlgItem(IDC_EDIT_SCRIPT))->LimitText(80);
	((CEdit*)GetDlgItem(IDC_EDIT_MAPTITLE))->LimitText(80);
	((CEdit*)GetDlgItem(IDC_EDIT_ALL))->LimitText(80);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSearchDlg::OnCheckCommand(UINT cmdID)
{
	CWnd* pWnd = GetDlgItem(cmdID);
	switch (cmdID)
	{
	case IDC_CHECK_DATE:
		GetDlgItem(IDC_SDATE)->EnableWindow(((CButton*)pWnd)->GetCheck());
		GetDlgItem(IDC_EDATE)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK_ID:
		GetDlgItem(IDC_EDIT_ID)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK_GROUP:
		GetDlgItem(IDC_EDIT_GROUP)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK_TRAN:
		GetDlgItem(IDC_EDIT_TRAN)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK5_OBJECT:
		GetDlgItem(IDC_COMBO_OBJECT)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK_SCRIPT:
		GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK_TITLE:
		GetDlgItem(IDC_EDIT_MAPTITLE)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	case IDC_CHECK_ALL:
		GetDlgItem(IDC_EDIT_ALL)->EnableWindow(((CButton*)pWnd)->GetCheck());
		break;
	}
}

BOOL CSearchDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR pHdr;
	LPNMLISTVIEW pNlv;

	pHdr = (LPNMHDR)lParam;
	pNlv = (LPNMLISTVIEW)lParam;

	switch (LOWORD(wParam))
	{
	case IDC_LIST_RESULT:
	{
		switch (pHdr->code)
		{
		case NM_CLICK:
		{
			int row, col;
			CString tmp;
			row = m_listResult.m_FocusRow;
			col = m_listResult.m_FocusCol;

			tmp = m_listResult.GetItemText(row, 2);

			//((CEdit*)GetDlgItem(IDC_MAP_NAME))->SetWindowTextA(tmp);
		}
		break;
		case NM_DBLCLK:
		{
			int row, col;
			CString tmp, tmps;
			bool bgrid = false;
			CString text;

			row = m_listResult.m_FocusRow;
			col = m_listResult.m_FocusCol;

			tmp = m_listResult.GetItemText(row, 1);
			tmps = m_listResult.GetItemText(row, 2);
		}
		break;
		}
	}
	break;
	}

	return CDialogEx::OnNotify(wParam, lParam, pResult);
}


void CSearchDlg::OnBnClickedButtonSearch()
{
	if (!((CButton*)GetDlgItem(IDC_CHECK_RESEARSH))->GetCheck())
	{
		m_aryTerms.RemoveAll();
	}

	CString strTerms("");
	if (((CButton*)GetDlgItem(IDC_CHECK_DATE))->GetCheck())
	{
		CString strTerms("");
		CTime timeSTime, timeETime;
		((CDateTimeCtrl*)GetDlgItem(IDC_SDATE))->GetTime(timeSTime);
		((CDateTimeCtrl*)GetDlgItem(IDC_EDATE))->GetTime(timeETime);

		strTerms.Format("01%04d%02d%02d%04d%02d%02d", timeSTime.GetYear(), timeSTime.GetMonth(), timeSTime.GetDay(), timeETime.GetYear(), timeETime.GetMonth(), timeETime.GetDay());
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_ID))->GetCheck())
	{
		CString tmp;
		GetDlgItem(IDC_EDIT_ID)->GetWindowText(tmp);
		strTerms.Format("02%s", tmp);
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_GROUP))->GetCheck())
	{
		CString tmp;
		GetDlgItem(IDC_EDIT_GROUP)->GetWindowText(tmp);
		strTerms.Format("03%s", tmp);
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_TRAN))->GetCheck())
	{
		CString tmp;
		GetDlgItem(IDC_EDIT_TRAN)->GetWindowText(tmp);
		strTerms.Format("05%s", tmp);
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	if (((CButton*)GetDlgItem(IDC_CHECK5_OBJECT))->GetCheck())
	{
		CString tmp;
		int index = ((CComboBox*)GetDlgItem(IDC_COMBO_OBJECT))->GetCurSel();
		if (index >= 0)
		{
			strTerms.Format("06%s", m_aryObject.GetAt(index));
			if (!FindString(m_aryTerms, strTerms))
			{
				m_aryTerms.Add(strTerms);
			}
		}
		
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_SCRIPT))->GetCheck())
	{
		CString tmp;
		GetDlgItem(IDC_EDIT_SCRIPT)->GetWindowText(tmp);
		strTerms.Format("07%s", tmp);
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_TITLE))->GetCheck())
	{
		CString tmp;
		GetDlgItem(IDC_EDIT_MAPTITLE)->GetWindowText(tmp);
		strTerms.Format("10%s", tmp);
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	if (((CButton*)GetDlgItem(IDC_CHECK_ALL))->GetCheck())
	{
		CString tmp;
		GetDlgItem(IDC_EDIT_ALL)->GetWindowText(tmp);
		strTerms.Format("11%s", tmp);
		if (!FindString(m_aryTerms, strTerms))
		{
			m_aryTerms.Add(strTerms);
		}
	}

	SearchRun();
}

bool CSearchDlg::FindString(const CStringArray& arr, LPCTSTR pszString)
{
	bool bFound = false;
	if (strlen(pszString) == 2) return true;
	const int nSize = arr.GetSize();
	for (int nIndex = 0; nIndex < nSize; nIndex++)
	{
		if (!arr[nIndex].Compare(pszString))
		{
			bFound = true;
			break;
		}
	}
	return bFound;
}

void CSearchDlg::OnBnClickedCheckResearsh()
{
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//if (((CButton*)GetDlgItem(IDC_CHECK_RESEARSH))->GetCheck())
	//{
	//	GetDlgItem(IDC_SDATE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDATE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_GROUP)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_TRAN)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_COMBO_OBJECT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_MAPTITLE)->EnableWindow(FALSE);
	//	GetDlgItem(IDC_EDIT_ALL)->EnableWindow(FALSE);

	//	for (UINT ii = IDC_CHECK_DATE; ii <= IDC_CHECK_ALL; ii++)
	//	{
	//		((CButton*)GetDlgItem(ii))->SetCheck(FALSE);
	//	}
	//}
}

void CSearchDlg::SearchRun()
{
	if (m_aryTerms.GetCount() < 1)
		return;
	CString sRoot;
	CString UserID = (CString)(char *)AfxGetMainWnd()->SendMessage(WM_USER, MAKEWPARAM(ID_USR_USERID, 0), 0);
	sRoot.Format("%s\\exe\\tmp\\FormSearch.dat", m_sRoot);
	CDownload* dn = new CDownload();
	if (!dn->Create())
	{
		delete dn;
	}
	dn->SetParent(this);
	dn->setSearchData(m_aryTerms);
	dn->Download(UserID, _T(""), _T(""), sRoot, TRGB_FORMSEARCH);
	delete dn;
}

LRESULT CSearchDlg::OnReceiveClose(WPARAM wParam, LPARAM lParam)
{
	CString sFile;
	CString sData;
	sFile.Format("%s\\exe\\tmp\\FormSearch.dat", m_sRoot);

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

void CSearchDlg::SetGridData(char* pchData)
{
	struct search_info* gridData = (struct search_info*)pchData;
	CString strData;
	CString strDate;

	m_listResult.DeleteAllItems();
	for (int ii = 0; ii < atoi(CString(gridData->nrec, sizeof(gridData->nrec))); ii++)
	{
		strData.Format("%s", CString(gridData->FormInfo[ii].resn, sizeof(gridData->FormInfo[ii].resn)));
		m_listResult.AddItem(ii, 0, strData.Trim());

		strData.Format("%s", CString(gridData->FormInfo[ii].desc, sizeof(gridData->FormInfo[ii].desc)));
		m_listResult.AddItem(ii, 1, strData.Trim());

		strData.Format("%s", CString(gridData->FormInfo[ii].usid, sizeof(gridData->FormInfo[ii].usid)));
		m_listResult.AddItem(ii, 2, strData.Trim());

		strData.Format("%s", CString(gridData->FormInfo[ii].yymd, sizeof(gridData->FormInfo[ii].yymd)));
		strData.Insert(4, "/");
		strData.Insert(7, "/");
		m_listResult.AddItem(ii, 3, strData.Trim());
	}
}

void CSearchDlg::OnBnClickedButtonReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_aryTerms.RemoveAll();
	//////////////////////////////////////////////////////////////////////////////
	((CButton*)GetDlgItem(IDC_CHECK_RESEARSH))->SetCheck(FALSE);
	GetDlgItem(IDC_SDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ID)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_GROUP)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_TRAN)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_OBJECT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_SCRIPT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_MAPTITLE)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ALL)->EnableWindow(FALSE);
	for (UINT ii = IDC_CHECK_DATE; ii <= IDC_CHECK_ALL; ii++)
	{
		((CButton*)GetDlgItem(ii))->SetCheck(FALSE);
	}
	/////////////////////////////////////////////////////////////////////////////
	GetDlgItem(IDC_SDATE)->SetWindowText("");
	GetDlgItem(IDC_EDATE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ID)->SetWindowText("");
	GetDlgItem(IDC_EDIT_GROUP)->SetWindowText("");
	GetDlgItem(IDC_EDIT_TRAN)->SetWindowText("");
	GetDlgItem(IDC_COMBO_OBJECT)->SetWindowText("");
	GetDlgItem(IDC_EDIT_SCRIPT)->SetWindowText("");
	GetDlgItem(IDC_EDIT_MAPTITLE)->SetWindowText("");
	GetDlgItem(IDC_EDIT_ALL)->SetWindowText("");

	m_listResult.DeleteAllItems();
}

void CSearchDlg::OnBnClickedButtonSave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	char name_filter[] = "All Files (*.*)|*.*|Text Files (*.txt)|*.txt|";
	CFileDialog ins_dlg(FALSE, "txt", "result.txt", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	ins_dlg.m_ofn.nFilterIndex = 2;

	if (ins_dlg.DoModal() != IDOK){
		return;
	}
	CString filePath;
	filePath = ins_dlg.GetPathName();
	CStdioFile file; // 파일 열기
	file.Open(filePath, CFile::modeWrite | CFile::modeCreate | CFile::typeText);
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_listResult.GetHeaderCtrl(); // 헤더컨트롤 얻어오기
	int nRow = m_listResult.GetItemCount(); // Row 갯수
	int nCol = pHeader->GetItemCount(); // Col 갯수
	CString text;
	for (int i = 0; i < nRow; i++)
	{
		text = "";
		for (int j = 0; j < nCol; j++)
		{
			text += m_listResult.GetItemText(i, j); // 아이템 텍스트 가져오기
			text += "\t";
		}
		file.WriteString(text + "\r\n"); // 파일에 쓰기
	}
	file.Close();
}

void CSearchDlg::OnDtnDatetimechangeEdate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTime timeSTime, timeETime;
	((CDateTimeCtrl*)GetDlgItem(IDC_SDATE))->GetTime(timeSTime);
	((CDateTimeCtrl*)GetDlgItem(IDC_EDATE))->GetTime(timeETime);
	if (timeETime < timeSTime)
	{
		((CDateTimeCtrl*)GetDlgItem(IDC_SDATE))->SetTime(&timeETime);
	}
	*pResult = 0;
}


void CSearchDlg::OnDtnDatetimechangeSdate(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CTime timeSTime, timeETime;
	((CDateTimeCtrl*)GetDlgItem(IDC_SDATE))->GetTime(timeSTime);
	((CDateTimeCtrl*)GetDlgItem(IDC_EDATE))->GetTime(timeETime);
	if (timeETime < timeSTime)
	{
		((CDateTimeCtrl*)GetDlgItem(IDC_EDATE))->SetTime(&timeSTime);
	}
	*pResult = 0;
}


void CSearchDlg::OnBnClickedButtonExcel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다. int m_iMax;
	int nColumncount = m_listResult.GetItemCount();
	//m_iMax = nColumncount;
	int i, j;

	int columnNum = 0;
	CString strFileName;

	CXLEzAutomation XL(FALSE);

	strFileName = "검색결과"; //파일 저장할 때 초기에 나타나는 파일명. 변경가능

	XL.SetCellValue(++columnNum, 1, _T("파일명")); //엑셀에 뿌려질 각 칼럼명들
	XL.SetCellValue(++columnNum, 1, _T("파일설명"));
	XL.SetCellValue(++columnNum, 1, _T("작성자"));
	XL.SetCellValue(++columnNum, 1, _T("작성일"));
	for (i = 1; i <= nColumncount; i++)
	{
		XL.SetCellValue(1, i + 1, m_listResult.GetItemText(i - 1, 0));

		for (j = 1; j <= nColumncount; j++)
			XL.SetCellValue(j + 1, i + 1, m_listResult.GetItemText(i - 1, j));
	}

	CFileDialog dlg(false, "xlsx", strFileName + ".xlsx", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, "xlsx 파일 (*.xlsx)|*.xlsx|", NULL);

	if (dlg.DoModal() == IDOK)
		XL.SaveFileAs(dlg.GetPathName());

	XL.ReleaseExcel();
}