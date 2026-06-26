// CDlg_Thread.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CDlg_Thread.h"
#include "afxdialogex.h"

#include "CThreadWnd.h"
//#include "CControlWnd.h"
#include "CTD_test.h"
// CDlg_Thread 대화 상자

IMPLEMENT_DYNAMIC(CDlg_Thread, CDialog)
CString CDlg_Thread::parser(CString& srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}
	else
	{
		CString  temp = srcstr.Left(srcstr.Find(substr));
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

CDlg_Thread::CDlg_Thread(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DLG_THREAD, pParent)
{
	OutputDebugString("\r\n[test] ");
	OutputDebugString("\r\n[test] ------------------------------------");
	OutputDebugString("\r\n[test] CDlg_Thread 생성자 CDlg_Thread");
}

CDlg_Thread::~CDlg_Thread()
{
#ifdef DF_USETHREAD	
	if (!m_bThreadUSE) return;

	OutputDebugString("\r\n[test] !!!!!! 소멸자 CDlg_Thread");
	CString str;

#endif
}

void CDlg_Thread::TerminateRTM()
{
#ifdef DF_USETHREAD
	if (!m_bThreadUSE) return;

	CString str;
	str.Format("\r\n[test][%s][%x]  m_balive=[%d] m_threadID=[%d]", __FUNCTION__, this, m_balive, m_threadID);
	OutputDebugString(str);

	m_balive = false;
	
	switch (WaitForSingleObject(m_event_TrdDlg, INFINITE))
	{
	case WAIT_OBJECT_0:
	{
		str.Format("\r\n[test][%s] **********************   WAIT_OBJECT_0  *********************", __FUNCTION__);
		OutputDebugString(str);
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			m_pWnd[ii]->DestroyWindow();
			delete m_pWnd[ii];

			str.Format("\r\n[test] [%d] [%s] *************윈도우 삭제 끝***********", ii,__FUNCTION__ );
			OutputDebugString(str);
		}

	}
	break;
	case WAIT_FAILED:
	case WAIT_ABANDONED:
	{
		str.Format("\r\n[test][%s]     WAIT_FAILED    WAIT_ABANDONED error=[%d]", __FUNCTION__, GetLastError());
		OutputDebugString(str);
	}
	break;
	case WAIT_TIMEOUT:
	{
		str.Format("\r\n[test][%s]    CDlg_Thread의  WAIT_TIMEOUT", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	default:
	{
		str.Format("\r\n[test][%s] CDlg_Thread의  default", __FUNCTION__);
		OutputDebugString(str);
	}
	break;
	}
#endif
}

void CDlg_Thread::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlg_Thread, CDialog)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CDlg_Thread::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_TEST, &CDlg_Thread::OnBnClickedBtnTest)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_TEST2, &CDlg_Thread::OnBnClickedBtnTest2)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg_Thread::OnBnClickedButton1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDlg_Thread::OnCbnSelchangeCombo1)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_TEST3, &CDlg_Thread::OnBnClickedBtnTest3)
	ON_BN_CLICKED(IDC_BTN_TEST4, &CDlg_Thread::OnBnClickedBtnTest4)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


#define DF_memory11
// CDlg_Thread 메시지 처리기
#ifdef DF_USETHREAD
CCriticalSection g_td;
UINT Production_td(LPVOID pParam)
{
	CDlg_Thread* pwnd = (CDlg_Thread*)pParam;
	int inum = GetCurrentThreadId();
	pwnd->m_threadID = inum;
	char pdata[100]{};
	
	CString str;
	str.Format("[test][%s]  Production_td ID=[%d]", __FUNCTION__, inum);
	OutputDebugString(str);
	
	while (pwnd->m_balive)
	{
		CTime time = CTime::GetCurrentTime();
		str.Format("[%d][%d][%d]", time.GetHour(), time.GetMinute(),GetTickCount());
		//OutputDebugString("[test]  " + str); 


		if (pwnd->GetSafeHwnd())
		{
		/*	g_td.Lock();
			str.Format("[test][%s]  Production_td g_td.Lock", __FUNCTION__);
			OutputDebugString(str);*/

#ifdef DF_memory1
			strncpy_s(pdata, (char*)str.GetBuffer(0), str.GetLength());
			pwnd->SendDataToThWnd((char*)&pdata[0], str.GetLength());
#else
			char* pbuff = new char[str.GetLength() + 1];
			memset(pbuff, 0x00, str.GetLength() + 1);
			memcpy(pbuff, (char*)str.GetBuffer(0), str.GetLength());
			pwnd->SendDataToThWnd(pbuff, str.GetLength());
			delete pbuff;
			pbuff = nullptr;
#endif

			
		/*	str.Format("[test][%s]  Production_td g_td.UnLock", __FUNCTION__);
			OutputDebugString(str);
			g_td.Unlock();*/
		}
		else
		{
			str.Format("[test][%s] ******** [%x]  CDlg_Thread  에서 getsafehwnd 이 false 라서  return  !!!!!!!---", __FUNCTION__, pwnd);
			OutputDebugString(str);

			return 0;
		}
	
		Sleep(1);
	}

//	
	pwnd->SelfSetEvent();
//	str.Format("[test] ******** [%x]  CDlg_Thread 스레드를 가지는 윈도우를 가지는 다이아로그의  스레드 return  !!!!!!!---", pwnd);
	OutputDebugString(str);
	//pwnd->m_event_TrdDlg.ResetEvent();
	
	return 0;
}
#endif
BOOL CDlg_Thread::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	//int itest = 0;
	//while (1)
	//{
	//	itest++;
	//	if (itest > 10000000 )
	//		break;
	//	else
	//		Invalidate();
	//}
	initctrl();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlg_Thread::initctrl()
{//CComboBox
	((CComboBox*)GetDlgItem(IDC_EDIT_DATA))->AddString("1개");
	((CComboBox*)GetDlgItem(IDC_EDIT_DATA))->AddString("2개");
	((CComboBox*)GetDlgItem(IDC_EDIT_DATA))->AddString("3개");
	((CComboBox*)GetDlgItem(IDC_EDIT_DATA))->AddString("4개");


	((CComboBox*)GetDlgItem(IDC_EDIT_DATA))->SetCurSel(3);


	CString  sProcessCore = GetProcessAffinity();
	CString sThreadCore = GetThreadAffinity();
	CString stmp;

	while (!sProcessCore.IsEmpty())
	{
		stmp = parser(sProcessCore, ";");
		switch (atoi(stmp))
		{
			case 1: ((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1); break;
			case 2: ((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1); break;
			case 3: ((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1); break;
			case 4: ((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1); break;
			case 5: ((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1); break;
			case 6: ((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(1); break;
			case 7: ((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(1); break;
			case 8: ((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(1); break;
		}
	}

	while (!sThreadCore.IsEmpty())
	{
		stmp = parser(sThreadCore, ";");
		switch (atoi(stmp))
		{
			case 1: ((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(1); break;
			case 2: ((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(1); break;
			case 3: ((CButton*)GetDlgItem(IDC_CHECK11))->SetCheck(1); break;
			case 4: ((CButton*)GetDlgItem(IDC_CHECK12))->SetCheck(1); break;
			case 5: ((CButton*)GetDlgItem(IDC_CHECK13))->SetCheck(1); break;
			case 6: ((CButton*)GetDlgItem(IDC_CHECK14))->SetCheck(1); break;
			case 7: ((CButton*)GetDlgItem(IDC_CHECK15))->SetCheck(1); break;
			case 8: ((CButton*)GetDlgItem(IDC_CHECK16))->SetCheck(1); break;
		}
	}
}

#ifdef DF_USETHREAD
bool CDlg_Thread::SendDataToThWnd(char* pdata, int ilen)
{
	if (m_bThreadUSE)
	{
	//	m_cs.Lock();
		for (int ii = 0; ii < m_nGroup; ii++)
		{
			if (m_pWnd[ii]->GetSafeHwnd())
			{
				m_pWnd[ii]->Dispatch(pdata, ilen);
			}
		}
	//	m_cs.Unlock();
	}
	return true;
}
#endif

void CDlg_Thread::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

}

void CDlg_Thread::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialog::OnOK();
}

void CDlg_Thread::OnBnClickedBtnTest()
{
#ifdef  DF_USETHREAD

	m_bThreadUSE = ((CButton*)(GetDlgItem(IDC_CK_TRHEAD)))->GetCheck();

	m_nGroup = ((CComboBox*)GetDlgItem(IDC_EDIT_DATA))->GetCurSel() + 1;

	for (int ii = 0; ii < m_nGroup; ii++)
	{
		m_pWnd[ii] = new CThreadWnd;
		m_pWnd[ii]->m_id = ii;
		CRect rect;
		GetClientRect(rect);
		int iwidth = rect.Width() / 4;
		int ileft = ii * iwidth;

		m_pWnd[ii]->m_bThreadUSE = m_bThreadUSE;
		if (m_pWnd[ii]->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL, CRect(ileft, 200, ileft + iwidth, rect.Height()), this, 1000) == FALSE)
			return ;

		int r = GetTickCount() % 255;
		Sleep(100);
		int g = GetTickCount() % 255;
		Sleep(100);
		int b = GetTickCount() % 255;

		m_pWnd[ii]->m_col = RGB(r, g, b);
	}

	if (!m_bThreadUSE) return;

	m_balive = true;
	m_hThread = AfxBeginThread(Production_td, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	m_event_TrdDlg = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	ResetEvent(m_event_TrdDlg);

	SetTimer(9898, 5000, nullptr);
	CString str;
	switch (WaitForSingleObject(m_event_TrdDlg, INFINITE))
	{
		case WAIT_OBJECT_0:
		{
			str.Format("\r\n[test][%s] **********************   WAIT_OBJECT_0  *********************", __FUNCTION__);
			OutputDebugString(str);
			for (int ii = 0; ii < m_nGroup; ii++)
			{
				m_pWnd[ii]->DestroyWindow();
				delete m_pWnd[ii];

				str.Format("\r\n[test] [%d] [%s] *************윈도우 삭제 끝***********", ii, __FUNCTION__);
				OutputDebugString(str);
			}

		}
		break;
		case WAIT_FAILED:
		case WAIT_ABANDONED:
		{
			str.Format("\r\n[test][%s]     WAIT_FAILED    WAIT_ABANDONED error=[%d]", __FUNCTION__, GetLastError());
			OutputDebugString(str);
		}
		break;
		case WAIT_TIMEOUT:
		{
			str.Format("\r\n[test][%s]    CDlg_Thread의  WAIT_TIMEOUT", __FUNCTION__);
			OutputDebugString(str);
		}
		break;
		default:
		{
			str.Format("\r\n[test][%s] CDlg_Thread의  default", __FUNCTION__);
			OutputDebugString(str);
		}
		break;
	}

#endif
}

void CDlg_Thread::OnDestroy()
{
	CDialog::OnDestroy();

	StopRaceDemo();  // stop race demo threads before m_raceArr is destroyed

#ifdef DF_USETHREAD
	if (!m_bThreadUSE) return;
	CString str;
	str.Format("\r\n[test][%s] CDlg_Thread OnDestroy 스레드를 가지는 윈도우들을 삭제한다", __FUNCTION__);
	OutputDebugString(str);

	TerminateRTM();

#endif
}


void CDlg_Thread::OnBnClickedBtnTest2()
{
#ifdef DF_USETHREAD
	if (!m_bThreadUSE) return;

	CString str;
	TerminateRTM();
	
#endif
}

void CDlg_Thread::SelfSetEvent()
{
	CString str;
	str.Format("\r\n[test][%s]  CDlg_Thread SelfSetEvent", __FUNCTION__);
	OutputDebugString(str);
	SetEvent(m_event_TrdDlg);

}

CString CDlg_Thread::GetProcessAffinity()
{
	char chdata[256];
	GetModuleFileName(nullptr, chdata, 256);

	CString spath, sData;
	spath.Format("%s", chdata);

	int ifind = spath.ReverseFind('\\');
	spath = spath.Left(ifind);
	spath += "\\Affinity.ini";

	memset(chdata, 0x00, 256);
	GetPrivateProfileString("PROCESS", "CORE", "", chdata, 256, spath);
	CString sRes;
	sRes.Format("%s", chdata);
	sRes.TrimRight();

	return sRes;
}

void CDlg_Thread::SetProcessAffinity(int icore)
{
	char chdata[256];
	GetModuleFileName(nullptr, chdata, 256);

	CString spath, sData, sResult;
	spath.Format("%s", chdata);

	int ifind = spath.ReverseFind('\\');
	spath = spath.Left(ifind);
	spath += "\\Affinity.ini";

	if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
		sResult += "1";
	if (((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck())
	{
		sResult += ";";  sResult += "2";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck())
	{
		sResult += ";"; sResult += "3";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK4))->GetCheck())
	{
		sResult += ";"; sResult += "4";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK5))->GetCheck())
	{
		sResult += ";"; sResult += "5";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK6))->GetCheck())
	{
		sResult += ";"; sResult += "6";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK7))->GetCheck())
	{
		sResult += ";"; sResult += "7";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK8))->GetCheck())
	{
		sResult += ";"; sResult += "8";
	}


	WritePrivateProfileString("PROCESS", "CORE", (LPCSTR)sResult.GetBuffer(0), spath);
}

CString CDlg_Thread::GetThreadAffinity()
{
	char chdata[256];
	GetModuleFileName(nullptr, chdata, 256);

	CString spath, sData;
	spath.Format("%s", chdata);

	int ifind = spath.ReverseFind('\\');
	spath = spath.Left(ifind);
	spath += "\\Affinity.ini";
	
	memset(chdata, 0x00, 256);
	GetPrivateProfileString("THREAD", "CORE", "", chdata, 256, spath);
	CString sRes;
	sRes.Format("%s", chdata);
	sRes.TrimRight();

	return sRes;
}


void CDlg_Thread::SetThreadAffinity(int icore)
{
	char chdata[256];
	GetModuleFileName(nullptr, chdata, 256);

	CString spath, sData, sResult;
	spath.Format("%s", chdata);

	int ifind = spath.ReverseFind('\\');
	spath = spath.Left(ifind);
	spath += "\\Affinity.ini";
	
	if (((CButton*)GetDlgItem(IDC_CHECK9))->GetCheck())
		sResult += "1";
	if (((CButton*)GetDlgItem(IDC_CHECK10))->GetCheck())
	{
		sResult += ";";  sResult += "2";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK11))->GetCheck())
	{
		sResult += ";"; sResult += "3";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK12))->GetCheck())
	{
		sResult += ";"; sResult += "4";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK13))->GetCheck())
	{
		sResult += ";"; sResult += "5";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK14))->GetCheck())
	{
		sResult += ";"; sResult += "6";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK15))->GetCheck())
	{
		sResult += ";"; sResult += "7";
	}
	if (((CButton*)GetDlgItem(IDC_CHECK16))->GetCheck())
	{
		sResult += ";"; sResult += "8";
	}

	WritePrivateProfileString("THREAD", "CORE", (LPCSTR)sResult.GetBuffer(0), spath);
}

BOOL CDlg_Thread::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch (wParam)
	{
		case IDC_CHECK1:  //프로세스 1코어
		{
			SetProcessAffinity(1);
		}
		break;
		case IDC_CHECK2: //프로세스 2  코어
		{
			SetProcessAffinity(2);
		}
		break;
		case IDC_CHECK3:  //프로세스 3  코어
		{
			SetProcessAffinity(3);
		}
		break;
		case IDC_CHECK4:  //프로세스 4  코어
		{
			SetProcessAffinity(4);
		}
		break;
		case IDC_CHECK5:  //프로세스 5  코어
		{
			SetProcessAffinity(5);
		}
		break;
		case IDC_CHECK6:  //프로세스 6  코어
		{
			SetProcessAffinity(6);
		}
		break;
		case IDC_CHECK7:  //프로세스 7  코어
		{
			SetProcessAffinity(7);
		}
		break;
		case IDC_CHECK8:  //프로세스 8  코어
		{
			SetProcessAffinity(8);
		}
		break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
		case IDC_CHECK9:  //스레드 1 코어
		{
			SetThreadAffinity(1);
		}
		break;
		case IDC_CHECK10:  //스레드 2  코어
		{
			SetThreadAffinity(2);
		}
		break;
		case IDC_CHECK11:  //스레드 3 코어
		{
			SetThreadAffinity(3);
		}
		break;
		case IDC_CHECK12:  //스레드 4  코어
		{
			SetThreadAffinity(4);
		}
		break;
		case IDC_CHECK13:  //스레드 5  코어
		{
			SetThreadAffinity(5);
		}
		break;
		case IDC_CHECK14:  //스레드 6  코어
		{
			SetThreadAffinity(6);
		}
		break;
		case IDC_CHECK15:  //스레드 7  코어
		{
			SetThreadAffinity(7);
		}
		break;
		case IDC_CHECK16:  //스레드 8 코어
		{
			SetThreadAffinity(8);
		}
		break;
	}
	return CDialog::OnCommand(wParam, lParam);
}

void CDlg_Thread::OnBnClickedButton1()
{
	int isize = sizeof(LPARAM);
	 isize = sizeof(WPARAM);
	CString str;
}


void CDlg_Thread::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CDlg_Thread::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case 9898:
		{
			KillTimer(nIDEvent);
			SetEvent(m_event_TrdDlg);
		}
		break;
	}
	CDialog::OnTimer(nIDEvent);
}


void CDlg_Thread::StopRaceDemo()
{
	if (!m_hThd1 && !m_hThd2) return;

	HANDLE hs[2] = {};
	int n = 0;

	if (m_hThd1) { m_hThd1->m_bAlive = false; hs[n++] = m_hThd1->m_hThread; }
	if (m_hThd2) { m_hThd2->m_bAlive = false; hs[n++] = m_hThd2->m_hThread; }

	WaitForMultipleObjects(n, hs, TRUE, 3000);

	delete m_hThd1; m_hThd1 = nullptr;
	delete m_hThd2; m_hThd2 = nullptr;
}

void CDlg_Thread::StartRaceDemo(bool bUseLock)
{
	StopRaceDemo();  // stop previous threads before touching m_raceArr
	m_raceArr.RemoveAll();

	CString log;
	log.Format("[RaceDemo] START - lock=%s", bUseLock ? "ON (safe)" : "OFF (CRASH expected)");
	OutputDebugString(log);

	// writer thread: Add/RemoveAll in a tight loop
	m_hThd1 = (CTD_test*)AfxBeginThread(RUNTIME_CLASS(CTD_test),
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_hThd1->m_bAutoDelete = FALSE;  // we manage lifetime via StopRaceDemo
	m_hThd1->m_id      = 1;
	m_hThd1->m_role    = CTD_test::TD_WRITER;
	m_hThd1->m_pArr    = &m_raceArr;
	m_hThd1->m_pLock   = &m_raceLock;
	m_hThd1->m_bUseLock = bUseLock;
	m_hThd1->m_bAlive  = true;  // set before ResumeThread
	m_hThd1->ResumeThread();

	// reader thread: GetSize + GetAt in a tight loop
	m_hThd2 = (CTD_test*)AfxBeginThread(RUNTIME_CLASS(CTD_test),
		THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED);
	m_hThd2->m_bAutoDelete = FALSE;
	m_hThd2->m_id      = 2;
	m_hThd2->m_role    = CTD_test::TD_READER;
	m_hThd2->m_pArr    = &m_raceArr;
	m_hThd2->m_pLock   = &m_raceLock;
	m_hThd2->m_bUseLock = bUseLock;
	m_hThd2->m_bAlive  = true;
	m_hThd2->ResumeThread();
}

void CDlg_Thread::OnBnClickedBtnTest3()  // race: NO lock -> CRASH
{
	StartRaceDemo(false);
}


void CDlg_Thread::OnBnClickedBtnTest4()  // race: WITH lock -> safe
{
	StartRaceDemo(true);
}

void CDlg_Thread::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ScreenToClient(&point);
	CDialog::OnLButtonUp(nFlags, point);
}


