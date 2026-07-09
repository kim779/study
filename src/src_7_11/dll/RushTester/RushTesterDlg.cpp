// RushTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RushTester.h"
#include "RushTesterDlg.h"
#include "excel9.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <afxsock.h>
#define	TID_CHECKINFO	100
#define	TID_CHECKSTATUS	101
#define	nCOL		10
int	width[nCOL] = { 44, 49, 47, 47, 48, 51, 51, 51, 47, 47 };

/////////////////////////////////////////////////////////////////////////////
// CRushTesterDlg dialog

CRushTesterDlg::CRushTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRushTesterDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRushTesterDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_period = 3;
	m_alive  = false;
	clear();
}

void CRushTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRushTesterDlg)
	DDX_Control(pDX, IDC_LST_ERROR, m_lstError);
	DDX_Control(pDX, IDC_REPORT, m_report);
	DDX_Control(pDX, IDC_LST_QRYFILE, m_lstQryFile);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CB_TRLIST, m_cblist);
}

BEGIN_MESSAGE_MAP(CRushTesterDlg, CDialog)
	//{{AFX_MSG_MAP(CRushTesterDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnconnect)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_QRYFILE, OnBtnQryfile)
	ON_BN_CLICKED(IDC_BTN_CLEARFILE, OnBtnClearfile)
	ON_BN_CLICKED(IDC_BTN_FILE, OnBtnFile)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_RUSHMAN_MSG, OnRushManMsg)
	ON_BN_CLICKED(IDC_BUTTON1, &CRushTesterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BTN_SEND, &CRushTesterDlg::OnBnClickedBtnSend)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRushTesterDlg message handlers

BOOL CRushTesterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	m_report.SetExtendedStyle(m_report.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	for (int ii = 0; ii < nCOL; ii++)
		addColumn(ii, width[ii]);

	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDT_CONCOUNT)->SetWindowText(_T("1"));
	GetDlgItem(IDC_EDT_CONINTERVAL)->SetWindowText(_T("100"));

	GetDlgItem(IDC_EDT_QRYINTERVAL)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDT_WAITTIME)->SetWindowText(_T("0"));
	GetDlgItem(IDC_EDT_QRYPERIOD)->SetWindowText(_T("0"));

	/*
	char szHostName[40] = {0};
	CString sDefault = "127.000.000.001";

	if(::gethostname(szHostName, sizeof(szHostName)) != 0)
		return 0;
	if(lstrcmp(szHostName, "") == 0)
		return 0;
	HOSTENT FAR* lphostent = ::gethostbyname(szHostName);
	if(lphostent == NULL)
		return 0;
	sprintf(szHostName, "%s", inet_ntoa(*((struct in_addr*)lphostent->h_addr)));
	*/
	//IDC_LST_QRYFILE  IDC_BTN_CLEARFILE IDC_BTN_QRYFILE

	((CWnd*)GetDlgItem(IDC_LST_QRYFILE))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDC_BTN_CLEARFILE))->ShowWindow(SW_HIDE);
	((CWnd*)GetDlgItem(IDC_BTN_QRYFILE))->ShowWindow(SW_HIDE);

	CRect rc;
	GetWindowRect(rc);
	SetWindowPos (NULL, 0, 0, rc.Width()/2- 60, rc.Height() - 150, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);

	m_rushMan.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 0, 0), this, 100);
	LoadSet();

	m_cblist.AddString("selectUsrNm 아이디로 이름가져오기");
	m_cblist.AddString("selectAttGrpList 관심종목그룹리스트 가져오기");
	m_cblist.AddString("menuLog 화면로그남기기");
	m_cblist.SetCurSel(0);
	((CWnd*)GetDlgItem(IDC_EDIT1))->SetWindowText("2");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRushTesterDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRushTesterDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CRushTesterDlg::OnBtnconnect() 
{
	int	nCount;
	CString sIP, sPort, sID, sPwd;
	CString sConnectCount, sConInterval;
	CString	sQueryInterval, sResponseWaitTime, sQueryPeriod, sQueryFile;

	m_alive = false;
	SaveSet();
	registerControl();
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(false);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(true);
	GetDlgItem(IDC_BTN_FILE)->EnableWindow(false);

	GetDlgItem(IDC_EDT_IP)->GetWindowText(sIP);
	GetDlgItem(IDC_EDT_PORT)->GetWindowText(sPort);
	GetDlgItem(IDC_EDT_ID)->GetWindowText(sID);
	GetDlgItem(IDC_EDT_PWD)->GetWindowText(sPwd);

	GetDlgItem(IDC_EDT_CONCOUNT)->GetWindowText(sConnectCount);
	GetDlgItem(IDC_EDT_CONINTERVAL)->GetWindowText(sConInterval);

	GetDlgItem(IDC_EDT_QRYINTERVAL)->GetWindowText(sQueryInterval);
	GetDlgItem(IDC_EDT_WAITTIME)->GetWindowText(sResponseWaitTime);
	GetDlgItem(IDC_EDT_QRYPERIOD)->GetWindowText(sQueryPeriod);

	CString uuid;
	GetDlgItem(IDC_EDIT_UUID)->GetWindowText(uuid);

	m_rushMan.SetUser(sID, sPwd, uuid);
	m_rushMan.InitQuery(atoi(sQueryInterval), atoi(sResponseWaitTime));
	CString tmps;
	((CWnd*)GetDlgItem(IDC_EDIT1))->GetWindowText(tmps);
	m_rushMan.m_iCount = atoi(tmps);
	m_rushMan.m_iQueryGubn = m_cblist.GetCurSel();

	m_rushMan.ClearQueryData();
	nCount = m_lstQryFile.GetCount();
	for (int ii = 0; ii < nCount; ii++)
	{
		m_lstQryFile.GetText(ii, sQueryFile);
		m_rushMan.AddQuery(sQueryFile);
	}
	m_report.DeleteAllItems();
	m_lstError.ResetContent();
	m_rushMan.StartConnect(sIP, atoi(sPort), atoi(sConnectCount), atoi(sConInterval), "4 1");

	SetTimer(TID_CHECKSTATUS, 3000, NULL);
}

void CRushTesterDlg::OnBtnDisconnect() 
{
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(false);	
	GetDlgItem(IDC_BTN_FILE)->EnableWindow(true);

	KillTimer(TID_CHECKINFO);
	KillTimer(TID_CHECKSTATUS);
	m_rushMan.DisConnectAll();
}

void CRushTesterDlg::OnDestroy() 
{
	KillTimer(TID_CHECKINFO);
	KillTimer(TID_CHECKSTATUS);
	m_rushMan.DisConnectAll();
	m_rushMan.DestroyWindow();

	CDialog::OnDestroy();	
}

LRESULT CRushTesterDlg::OnRushManMsg(WPARAM wParam, LPARAM lParam)
{
	CString sMsg, sCur;
	int nAbsIdx = HIWORD(wParam);
	CWnd* pwnd = NULL;
	
	switch(LOWORD(wParam))
	{
	case WL_ENABLEFALSE:
		((CWnd*)GetDlgItem(IDC_BTN_SEND))->EnableWindow(FALSE);
		((CWnd*)GetDlgItem(IDC_BTN_DISCONNECT))->EnableWindow(FALSE);
		break;
	case WL_ENABLETRUE:
		((CWnd*)GetDlgItem(IDC_BTN_SEND))->EnableWindow(TRUE);
		((CWnd*)GetDlgItem(IDC_BTN_DISCONNECT))->EnableWindow(TRUE);
		break;
	case WL_TRYCONNECT:	sMsg = _T("WL_TRYCONNECT\n");	break;
	case WL_DISCONNECT:	sMsg = _T("WL_DISCONNECT\n");	break;
	case WL_RUN:
		if (!m_alive)
		{
			m_alive = true;
			clear();
			SetTimer(TID_CHECKINFO, 1000, NULL);
		}
		sMsg = _T("WL_RUN\n");
		break;
	case WL_QUERY:
		m_qryCnt++;
		sMsg = _T("WL_QUERY\n");
		break;
	case WL_RECEIVE:
		m_rcvCnt++;
		m_totalBytes += (int)lParam;
		m_subBytes   += (int)lParam;
		sMsg.Format("%s [%.f][%d]\n", "WL_RECEIVE", m_totalBytes, m_subBytes);
		break;
	case WL_ALERT:
		m_alertCnt++;
		sMsg = _T("WL_ALERT\n");
		break;
	case WL_NOTICE:
		m_noticeCnt++;
		sMsg = _T("WL_NOTICE\n");
		break;
	case WL_ERROR:
		m_lstError.AddString((char *)lParam);
		sMsg = _T("WL_ERROR\n");
		break;
	}
	TRACE(sMsg);
	return 0;
}

void CRushTesterDlg::OnTimer(UINT nIDEvent) 
{
	CString text;
	int	index = m_report.GetItemCount();

	switch (nIDEvent)
	{
	case TID_CHECKSTATUS:
		text.Format("status: %d / %d ", m_rushMan.GetLoginCount(), m_rushMan.GetCount());
		GetDlgItem(IDC_STC_STATUS)->SetWindowText(text);
		break;
	case TID_CHECKINFO:
		m_maxBytes = max(m_maxBytes, m_subBytes);
		if (m_minBytes <= 0)
			m_minBytes = m_subBytes;
		else
			m_minBytes = min(m_minBytes, m_subBytes);
		m_subBytes = 0;

		if (++m_second >= m_period)
		{
			m_second = 0;
			// Elpased Time (Sec.)
			text.Format("%d", m_period * index + m_period);
			addItem(text, index, 0);
			// Try Connect Count
			text.Format("%d", m_rushMan.GetCount());
			addItem(text, index, 1);
			// Login Success Count
			text.Format("%d", m_rushMan.GetLoginCount());
			addItem(text, index, 2);
			// Total Query Count
			text.Format("%d", m_qryCnt);
			addItem(text, index, 3);
			// Total Receive Count
			text.Format("%d", m_rcvCnt);
			addItem(text, index, 4);
			// Average Bytes / Sec.
			text.Format("%d", (int)m_totalBytes / m_period);
			addItem(text, index, 5);
			// Max. Bytes / Sec.
			text.Format("%d", m_maxBytes);
			addItem(text, index, 6);
			// Min. Bytes / Sec.
			text.Format("%d", m_minBytes < 0 ? 0 : m_minBytes);
			addItem(text, index, 7);
			// Alert Count
			text.Format("%d", m_alertCnt);
			addItem(text, index, 8);
			// Notice Count
			text.Format("%d", m_noticeCnt);
			addItem(text, index, 9);
			clear();
		}
		break;
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CRushTesterDlg::OnBtnQryfile() 
{
	POSITION pos;
	CString sFile;
	CFileDialog dlg(TRUE, "dat", NULL, 
		OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT);

	if (dlg.DoModal() == IDOK)
	{
		pos = dlg.GetStartPosition();
		while (pos != NULL)
		{
			sFile = dlg.GetNextPathName(pos);
			m_lstQryFile.AddString(sFile);
		}
	}
}

void CRushTesterDlg::OnBtnClearfile() 
{
	m_lstQryFile.ResetContent();
}

void CRushTesterDlg::LoadSet()
{
	char	dat[512];
	int	nQryCnt;
	CString	sFile, sValue;

	GetModuleFileName(NULL, dat, sizeof(dat));
	sFile.Format("%s", dat);
	sFile = sFile.Left(sFile.ReverseFind('\\') + 1);
	sValue = sFile;
	sFile += "Set.ini";

	GetPrivateProfileString("RushTest", "IP", "", dat, sizeof(dat), sFile);			sValue = dat;
	GetDlgItem(IDC_EDT_IP)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "Port", "80", dat, sizeof(dat), sFile);		sValue = dat;
	GetDlgItem(IDC_EDT_PORT)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "ID", "", dat, sizeof(dat), sFile);			sValue = dat;
	GetDlgItem(IDC_EDT_ID)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "PWD", "", dat, sizeof(dat), sFile);		sValue = dat;
	GetDlgItem(IDC_EDT_PWD)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "CCount", "1", dat, sizeof(dat), sFile);		sValue = dat;
	GetDlgItem(IDC_EDT_CONCOUNT)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "CInterval", "100", dat, sizeof(dat), sFile);	sValue = dat;
	GetDlgItem(IDC_EDT_CONINTERVAL)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "QryInterval", "0", dat, sizeof(dat), sFile);	sValue = dat;
	GetDlgItem(IDC_EDT_QRYINTERVAL)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "ResponseWaitTime", "0", dat, sizeof(dat), sFile);	sValue = dat;
	GetDlgItem(IDC_EDT_WAITTIME)->SetWindowText(sValue);
	GetPrivateProfileString("RushTest", "QryPeriod", "3", dat, sizeof(dat), sFile);		sValue = dat;
	if (atoi(sValue) == 0)
		sValue = _T("3");
	GetPrivateProfileString("RushTest", "uuid", "0", dat, sizeof(dat), sFile);	sValue = dat;
	GetDlgItem(IDC_EDIT_UUID)->SetWindowText(sValue);

	GetDlgItem(IDC_EDT_QRYPERIOD)->SetWindowText(sValue);
	m_period = atoi(sValue);
	GetPrivateProfileString("RushTest", "QryCount", "0", dat, sizeof(dat), sFile);		sValue = dat;
	nQryCnt = atoi(dat);
	for (int ii = 0; ii < nQryCnt; ii++)
	{
		CString sParam;
		sParam.Format("QFile%d", ii);
		GetPrivateProfileString("RushTest", sParam, "", dat, sizeof(dat), sFile);
		sValue = dat;
		if (sValue.IsEmpty()) break;

		m_lstQryFile.AddString(sValue);
	}	
}

void CRushTesterDlg::SaveSet()
{
	char	dat[256];
	int	nQryCnt;
	CString	sFile, sValue;

	GetModuleFileName(NULL, dat, sizeof(dat));
	sFile.Format("%s", dat);
	sFile = sFile.Left(sFile.ReverseFind('\\') + 1);
	sFile += "Set.ini";

	GetDlgItem(IDC_EDT_IP)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "IP", sValue, sFile);			
	GetDlgItem(IDC_EDT_PORT)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "Port", sValue, sFile);		
	GetDlgItem(IDC_EDT_ID)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "ID", sValue, sFile);			
	GetDlgItem(IDC_EDT_PWD)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "PWD", sValue, sFile);		
	GetDlgItem(IDC_EDT_CONCOUNT)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "CCount", sValue, sFile);		
	GetDlgItem(IDC_EDT_CONINTERVAL)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "CInterval", sValue, sFile);	
	GetDlgItem(IDC_EDT_QRYINTERVAL)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "QryInterval", sValue, sFile);
	GetDlgItem(IDC_EDT_WAITTIME)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "ResponseWaitTime", sValue, sFile);
	GetDlgItem(IDC_EDT_QRYPERIOD)->GetWindowText(sValue);
	if (atoi(sValue) == 0)
		sValue = _T("3");
	WritePrivateProfileString("RushTest", "QryPeriod", sValue, sFile);
	m_period = atoi(sValue);
	
	sValue.Format("%d", m_lstQryFile.GetCount());
	WritePrivateProfileString("RushTest", "QryCount", sValue, sFile);		

	GetDlgItem(IDC_EDIT_UUID)->GetWindowText(sValue);
	WritePrivateProfileString("RushTest", "uuid", sValue, sFile);		

	nQryCnt = m_lstQryFile.GetCount();
	for (int ii = 0; ii < nQryCnt; ii++)
	{
		CString sParam;
		sParam.Format("QFile%d", ii);
		m_lstQryFile.GetText(ii, sValue);
		WritePrivateProfileString("RushTest", sParam, sValue, sFile);
	}
}

void CRushTesterDlg::OnOK() 
{
//	CDialog::OnOK();
}

void CRushTesterDlg::registerControl()
{
	char	dat[512];
	CString	sFile;
	HINSTANCE hLib;

	GetModuleFileName(NULL, dat, sizeof(dat));
	sFile.Format("%s", dat);
	sFile = sFile.Left(sFile.ReverseFind('\\') + 1);
	sFile += "axsock.ocx";

	hLib = LoadLibrary(sFile);
	if (hLib != (HINSTANCE)HINSTANCE_ERROR)
	{
		FARPROC	lpDllEntryPoint;

		(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, _T("DllUnRegisterServer"));
		if (lpDllEntryPoint)
			(*lpDllEntryPoint)();

		(FARPROC &)lpDllEntryPoint = GetProcAddress(hLib, _T("DllRegisterServer"));
		if (lpDllEntryPoint)
			(*lpDllEntryPoint)();
		FreeLibrary(hLib);
	}
}

void CRushTesterDlg::clear()
{
	m_second     = 0;
	m_qryCnt     = 0;
	m_rcvCnt     = 0;
	m_totalBytes = 0;
	m_subBytes   = 0;
	m_maxBytes   = 0;
	m_minBytes   = -1;
	m_alertCnt   = 0;
	m_noticeCnt  = 0;
}

BOOL CRushTesterDlg::addColumn(int col, int width)
{
	LV_COLUMN lvc;

	lvc.mask     = LVCF_FMT | LVCF_WIDTH;
	lvc.fmt      = LVCFMT_RIGHT;
	lvc.cx       = width;
	lvc.iSubItem = -1;

	return m_report.InsertColumn(col, &lvc);
}

BOOL CRushTesterDlg::addItem(CString items, int item, int subi)
{
	LV_ITEM lvi;

	lvi.mask     = LVIF_TEXT;
	lvi.iItem    = item;
	lvi.iSubItem = subi;
	lvi.pszText  = (LPTSTR)items.operator LPCTSTR();

	if (!subi)
		return m_report.InsertItem(&lvi);
	return m_report.SetItem(&lvi);
}

void CRushTesterDlg::OnBtnFile() 
{
	int	count;

	count = m_report.GetItemCount();
	if (count <= 0)
		return;

	_Application	excel;
	if (!excel.CreateDispatch("Excel.Application"))
	{
		AfxMessageBox("Fail to create Excel instance");
		return;
	}

	bool	except = false;
	COleVariant  covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	TRY
	{
		Workbooks	Wbs;
		_Workbook	Wb;
		Range		Rng;
		LPDISPATCH	lpDisp;
		COleSafeArray	sa;
		DWORD	numElements[2];
		long	index[2];
		CString	tmps, string;

		lpDisp = excel.GetWorkbooks();
		Wbs.AttachDispatch(lpDisp);

		lpDisp = Wbs.Add(covOptional);
		Wb.AttachDispatch(lpDisp);
		excel.SetVisible(true);

		numElements[0] = 1;
		numElements[1] = nCOL;
		sa.Create(VT_VARIANT, 2, numElements);

		index[0] = 0;
		for (int row = 0; row < count+1; )
		{
			row++;
			string.Format("A%d", row);
			tmps.Format("%c%d", (char)('@'+nCOL), row);		// for 'A' index
			Rng = excel.GetRange(COleVariant((LPCTSTR)string, VT_BSTR), COleVariant((LPCTSTR)tmps, VT_BSTR));

			for (int col = 0; col < nCOL; col++)
			{
				if (row == 1)
				{
					switch (col)
					{
					case 0:	tmps = _T("Elapsed Time (Sec.)");	break;
					case 1:	tmps = _T("Try Connect Count");		break;
					case 2:	tmps = _T("Login Success Count");	break;
					case 3:	tmps = _T("Total Query Count");		break;
					case 4:	tmps = _T("Total Receive Count");	break;
					case 5:	tmps = _T("Average Bytes/Sec.");	break;
					case 6:	tmps = _T("Max. Bytes/Sec.");		break;
					case 7:	tmps = _T("Min. Bytes/Sec.");		break;
					case 8:	tmps = _T("Alert Count");		break;
					case 9:	tmps = _T("Notice Count");		break;
					}
				}
				else
					tmps = m_report.GetItemText(row-2, col);

				index[1] = col;
				sa.PutElement(index, COleVariant(tmps));
			}
			Rng.SetValue(sa);
		}
		sa.Clear();
		sa.Detach();

		Rng = Rng.GetEntireColumn();
		Rng.AutoFit();
		Rng.ReleaseDispatch();
	}
	CATCH(COleDispatchException, e)
	{
		except = true;
	}
	END_CATCH;

	if (except)
		excel.Quit();
	excel.ReleaseDispatch();
}


void CRushTesterDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CRushTesterDlg::OnBnClickedBtnSend()
{
	CString tmps;
	((CWnd*)GetDlgItem(IDC_EDIT1))->GetWindowText(tmps);
	m_rushMan.m_iCount = atoi(tmps);
	m_rushMan.m_iQueryGubn = m_cblist.GetCurSel();

	((CWnd*)GetDlgItem(IDC_EDT_QRYINTERVAL))->GetWindowText(tmps);
	
	m_rushMan.Send(m_rushMan.m_iCount, m_rushMan.m_iQueryGubn, atoi(tmps));
}
