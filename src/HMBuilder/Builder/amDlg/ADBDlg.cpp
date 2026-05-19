// ADBDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ADBDlg.h"
#include "afxdialogex.h"
#include "../h/mainvar.h"

#define DEVICE "\tdevice"
#define DEVICES "devices"
#define DEVICES_LIST "List of devices attached "
#define SEQ_CRLF	"\r\n"
#define LOGCAT_ALL "ALL"

// CADBDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CADBDlg, CDialogEx)

CADBDlg::CADBDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CADBDlg::IDD, pParent)
{
	m_run_flag = FALSE;
	m_bHold = TRUE;
	m_bPipeDataShow = FALSE;
	m_iLineNum = 1;
	m_strSearchText = _T("");
	m_iSearchIdx = 0;
	m_cLogLev = 'v';
	m_bInit = false;
}

CADBDlg::~CADBDlg()
{
	HANDLE hProcess;
	bool bProcess;
	do{
		bProcess = false;
		hProcess = GetProcessList("adb.exe", true);
		if (hProcess != INVALID_HANDLE_VALUE)
		{
			bProcess = true;
			SafeTerminateProcess(hProcess, 1);
		}
	}while(bProcess);

	hProcess = GetProcessList("cmd.exe", true);
	if (hProcess != INVALID_HANDLE_VALUE)
	{
		bProcess = true;
		SafeTerminateProcess(hProcess, 1);
	}
}

void CADBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RICHEDIT2_OUTPUT, m_richEdlt);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_cbDevice);
	DDX_Control(pDX, IDC_COMBO_TAG, m_cbTag);
	DDX_Control(pDX, IDC_COMBO_LOG, m_cbLogLev);
}


BEGIN_MESSAGE_MAP(CADBDlg, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CADBDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_LOGSAVE, &CADBDlg::OnBnClickedButtonLogsave)
	ON_BN_CLICKED(IDC_BUTTON_DEVICE, &CADBDlg::OnBnClickedButtonDevice)
	ON_BN_CLICKED(IDC_BUTTON_ENTER, &CADBDlg::OnBnClickedButtonEnter)
	ON_BN_CLICKED(IDC_CHECK_HOLD, &CADBDlg::OnBnClickedCheckHold)
	ON_EN_KILLFOCUS(IDC_EDIT_INPUT, &CADBDlg::OnEnKillfocusEditInput)
	ON_EN_SETFOCUS(IDC_EDIT_INPUT, &CADBDlg::OnEnSetfocusEditInput)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CADBDlg::OnEnChangeEditInput)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CADBDlg::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CADBDlg::OnBnClickedButtonNext)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDOK, &CADBDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CADBDlg 메시지 처리기입니다.
BOOL CADBDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetCurrentDirectory(MAX_PATH, m_current_path);

	// 선언된 구조체 변수들을 초기화한다.
	ZeroMemory(&m_security_attributes, sizeof(SECURITY_ATTRIBUTES));

	// 현재 프로세스가 생성한 파이프 객체를 실행될 프로세스가 사용할수 있도록 보안 설정을 한다.
	m_security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
	m_security_attributes.bInheritHandle = TRUE;
	m_security_attributes.lpSecurityDescriptor = NULL;

	// 리다이렉션에 사용할 파이프 객체를 생성한다.
	if (!CreatePipe(&mh_pipe_read, &mh_pipe_write, &m_security_attributes, 0))
		AfxMessageBox("CreatePipe() error");

	m_richEdlt.SendMessage(EM_SETLANGOPTIONS, 0,
		(LPARAM)(m_richEdlt.SendMessage(EM_GETLANGOPTIONS, 0, 0) & ~IMF_AUTOFONT));
	RunPipe(DEVICES, FALSE);

	((CButton*)GetDlgItem(IDC_CHECK_HOLD))->SetCheck(BST_CHECKED);
	///////////////////////////////////////////////////////////////////////////////////////////////
	m_sRoot = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	CString strReadLine, strData;
	CStdioFile file;
	CFileException e;

	int ii = 0;
	m_cbTag.InsertString(ii++, LOGCAT_ALL);
	if (file.Open(m_sRoot + _T("\\tab\\axislogcat.ini"), CFile::modeRead, &e))
	{
		file.ReadString(strReadLine);
		while (file.ReadString(strReadLine)){
			strData = strReadLine.Mid(strReadLine.Find("=") + 1);
			m_cbTag.InsertString(ii++, strData);
		}
		file.Close();
	}
	else
	{
		m_cbTag.InsertString(ii++, "axis");
		m_cbTag.InsertString(ii++, "axis01");
		m_cbTag.InsertString(ii++, "axis02");

		if (file.Open(m_sRoot + _T("\\tab\\axislogcat.ini"), CFile::modeCreate | CFile::modeWrite, &e))
		{
			CString strTmp(""), strLine("");
			file.WriteString("[TAG]\n");
			for (int ii = 1; ii < m_cbTag.GetCount(); ii++)
			{
				m_cbTag.GetLBText(ii, strTmp);
				strLine.Format("%02d=%s\n", ii, strTmp);
				file.WriteString(strLine);
			}
			file.Close();
		}
	}
	m_cbTag.SetCurSel(0);
	///////////////////////////////////////////////////////////////////////////////////////////////
	ii = 0;
	m_cbLogLev.InsertString(ii++, "verbose");
	m_cbLogLev.InsertString(ii++, "debug");
	m_cbLogLev.InsertString(ii++, "info");
	m_cbLogLev.InsertString(ii++, "warn");
	m_cbLogLev.InsertString(ii++, "error");
	m_cbLogLev.InsertString(ii++, "assert");
	m_cbLogLev.SetCurSel(0);
	m_bInit = true;

	///////////////////////////////////////////////////////////////////////////////////////////////
	//CHARFORMAT2 cf2;
	//memset(&cf, 0, sizeof(CHARFORMAT2));
	//cf2.cbSize = sizeof(CHARFORMAT2);
	//SendMessage(m_richEdlt, EM_GETCHARFORMAT, 1, (LPARAM)&cf2);
	CFont          cFont;
	cFont.CreateFont(12,                     // 글자높이
		0,                     // 글자너비
		0,                      // 출력각도
		0,                      // 기준 선에서의각도
		FW_NORMAL,              // 글자굵기
		FALSE,                  // Italic 적용여부
		FALSE,                  // 밑줄적용여부
		FALSE,                  // 취소선적용여부
		DEFAULT_CHARSET,       // 문자셋종류
		OUT_DEFAULT_PRECIS,    // 출력정밀도
		CLIP_DEFAULT_PRECIS,   // 클리핑정밀도
		DEFAULT_QUALITY,       // 출력문자품질
		DEFAULT_PITCH,         // 글꼴Pitch
		_T("돋움체")           // 글꼴
		);
	m_richEdlt.SetFont(&cFont);

	//CHOOSEFONT CFT; // 글꼴 공통 대화상자 사용
	//LOGFONT lf;
	//// ... CFT 대화상자에서 글꼴 설정
	//// 

	//CHARFORMAT2 cf2;
	//memset(&cf, 0, sizeof(CHARFORMAT2));
	//cf2.cbSize = sizeof(CHARFORMAT2);
	//cf2.dwMask = CFM_COLOR | CFM_FACE | CFM_SIZE | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT;
	//cf2.crTextColor = CFT.rgbColors;
	//cf2.yHeight = CFT.iPointSize * 2;
	//strcpy(cf2.szFaceName, lf.lfFaceName); // lf는
	//df2.dwEffects = 0;
	//if (lf.lfItalic) cf.dwEffects |= CFE_ITALIC;
	//if (lf.lfwight == FW_BOLD) cf.dwEffects |= CFE_BOLD;
	////...(생략)

	//SendMessage(hRich, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf2);
	///////////////////////////////////////////////////////////////////////////////////////////////
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

#include "Tlhelp32.h"
#include "Psapi.h"

HANDLE CADBDlg::GetProcessList(LPCTSTR szFilename, bool bMultiAdb)
{
	HANDLE hProcessSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;

	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	if (hProcessSnapshot == INVALID_HANDLE_VALUE)
		return INVALID_HANDLE_VALUE;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	Process32First(hProcessSnapshot, &pe32);

	do
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess != NULL)
		{
			if (_tcscmp(pe32.szExeFile, szFilename) == 0)
			{
				wprintf(L"Process ID of %s : %d\n", pe32.szExeFile, pe32.th32ProcessID);
				if (bMultiAdb)
					return hProcess;
				else
					bMultiAdb = true;
			}
		}
	} while (Process32Next(hProcessSnapshot, &pe32));

	return INVALID_HANDLE_VALUE;
}

int CADBDlg::GetProcessCnt(LPCTSTR szFilename)
{
	HANDLE hProcessSnapshot;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	int icnt = 0;

	hProcessSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

	if (hProcessSnapshot == INVALID_HANDLE_VALUE)
		return icnt;

	pe32.dwSize = sizeof(PROCESSENTRY32);

	Process32First(hProcessSnapshot, &pe32);

	do
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess != NULL)
		{
			if (_tcscmp(pe32.szExeFile, szFilename) == 0)
			{
				wprintf(L"Process ID of %s : %d\n", pe32.szExeFile, pe32.th32ProcessID);
				icnt++;
			}
		}

	} while (Process32Next(hProcessSnapshot, &pe32));

	return icnt;
}

BOOL CADBDlg::SafeTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
	DWORD dwTID, dwCode, dwErr = 0;
	HANDLE hProcessDup = INVALID_HANDLE_VALUE;
	HANDLE hRT = NULL;
	HINSTANCE hKernel = GetModuleHandle("Kernel32");

	BOOL bSuccess = FALSE;
	BOOL bDup = DuplicateHandle(GetCurrentProcess(),
		hProcess,
		GetCurrentProcess(),
		&hProcessDup,
		PROCESS_ALL_ACCESS,
		FALSE,
		0);
	if (GetExitCodeProcess((bDup) ? hProcessDup : hProcess, &dwCode)
		&& (dwCode == STILL_ACTIVE))
	{
		FARPROC pfnExitProc;
		pfnExitProc = GetProcAddress(hKernel, "ExitProcess");
		hRT = CreateRemoteThread((bDup) ? hProcessDup : hProcess,
			NULL,
			0,
			(LPTHREAD_START_ROUTINE)pfnExitProc,
			(PVOID)uExitCode, 0, &dwTID);
		if (hRT == NULL) dwErr = GetLastError();
	}
	else
	{
		dwErr = ERROR_PROCESS_ABORTED;
	}
	if (hRT)
	{
		WaitForSingleObject((bDup) ? hProcessDup : hProcess, INFINITE);
		CloseHandle(hRT);
		bSuccess = TRUE;
	}
	if (bDup)
		CloseHandle(hProcessDup);
	if (!bSuccess)
		SetLastError(dwErr);

	return bSuccess;
}

void CADBDlg::RunPipe(CString strData, BOOL bShow)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// 명령어가 실행중이면 오류처리를 한다.
	//if (m_run_flag == 1){
	//	MessageBox("이전 명령어가 실행중입니다.", "잠시후에 다시 시도하세요", MB_ICONSTOP);
	//	return;
	//}
	////////////////////////////////////////////////////////////////////////////////////////////
	HANDLE hProcess;
	int gg = GetProcessCnt("adb.exe");

	for (int ii = 1; ii < gg; ii++)
	{
		hProcess = GetProcessList("adb.exe", false);
		if (hProcess != INVALID_HANDLE_VALUE)
		{
			SafeTerminateProcess(hProcess, 1);
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////

	CString command_str, tmp;
	CString path("");
	m_sRoot = (char *)AfxGetMainWnd()->SendMessage(WM_USER, ID_USR_GETROOTDIR);
	path.Format("%s\\adb", m_sRoot);
	STARTUPINFO startup_info = { 0, };

	// 사용자가 입력한 명령문을 얻는다.
	//GetDlgItemText(IDC_EDIT_INPUT, tmp);
	if (strData == DEVICES)
		command_str.Format("/k adb %s", strData);
	else if (strData.Left(3) == LOGCAT_ALL)
		command_str.Format("/k adb logcat");
	else if (strData == "exit")
		command_str.Format("exit");
	else
		command_str.Format("/k adb logcat -s %s", strData);
		
	// 선언된 구조체 변수들을 초기화한다.
	ZeroMemory(&m_process_info, sizeof(PROCESS_INFORMATION));

	// 콘솔 명령문을 실행할때 사용할 실행 인자를 구성한다.
	startup_info.cb = sizeof(startup_info);

	// STARTF_USESTDHANDLES 속성을 명시하지 않으면 파이프로 입출력이 
	// 리다이렉트 되지 않는다.
	startup_info.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;

	// 실행시킬 콘솔창은 HIDE 시킨다.
	startup_info.wShowWindow = SW_HIDE;

	// 실행시킬 콘솔의 표준출력과 표준에러출력값을 파이프로 리다이렉트 시키기 위해
	// 생성한 파이프의 핸들을 설정한다.
	startup_info.hStdOutput = mh_pipe_write;
	startup_info.hStdError = mh_pipe_write;

	// 콘솔을 실행시키기 위한 새로운 프로세스와 메인 스레드를 생성한다.
	//if (FALSE == CreateProcess(NULL, (char *)(const char *)command_str, NULL, NULL, TRUE, 0,
	//	NULL, m_current_path, &startup_info, &m_process_info)){
	if (!CreateProcess("C:\\WINDOWS\\system32\\cmd.exe", // No module name (use command line). 
		(char *)(const char *)command_str, // Command line. 
		NULL,             // Process handle not inheritable. 
		NULL,             // Thread handle not inheritable. 
		TRUE,            // Set handle inheritance to FALSE. 
		0,                // No creation flags. 
		NULL,             // Use parent's environment block. 
		path,             // Use parent's starting directory. 
		&startup_info,              // Pointer to STARTUPINFO structure.
		&m_process_info)             // Pointer to PROCESS_INFORMATION structure.
		)
	{
		MessageBox("Failed to run adb.exe", "AxisBuilder", MB_ICONSTOP);
	}

	//if (FALSE == CreateProcess(NULL, (char *)(const char *)command_str, NULL, NULL, TRUE, 0,
	//	NULL, m_current_path, &startup_info, &m_process_info)){
	//	// 프로세스의 생성에 실패한 경우, m_run_flag 에 0(zero)값 을 대입한다.
	//	m_run_flag = 0;
	//	MessageBox("내부 오류로 명령어를 실행할수 없습니다.", "명령어 실행 오류", MB_ICONSTOP);
	//}
	else {
		// 프로세스의 생성에 성공한 경우, m_run_flag 에 1값 을 대입한다.
		m_run_flag = 1;
		m_bPipeDataShow = bShow;
		// 1번 타이머를 실행한다.
		SetTimer(1, 300, NULL);
	}
}

void CADBDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 1){
		// 파이프를 통해 전달받은 정보를 저장하기 위한 변수를 선언한다.
		TCHAR pipe_data[MAX_PIPE_BUFF_SIZE];
		CString strTmp, strPipe, strPiece(""), strLineNum;
		DWORD total_bytes_available = 0, num_bytes_read = 0, bytes_left_this_message = 0;
		// 파이프 객체에 출력된 정보를 얻어온다.
		BOOL success_flag = PeekNamedPipe(mh_pipe_read, pipe_data, 1,
			&num_bytes_read, &total_bytes_available, &bytes_left_this_message);
		//if (success_flag == TRUE){
		//while (success_flag == TRUE){
		while (success_flag && total_bytes_available)
		{
			strTmp = strPiece = strPipe = _T("");
			// 읽을 정보가 실제로 존재하는 경우
			if (num_bytes_read > 0){
				// pipe_data 에 읽은 정보를 저장한다.
				success_flag = ReadFile(mh_pipe_read, pipe_data,
					MAX_PIPE_BUFF_SIZE - 1, &num_bytes_read, NULL);

				if (success_flag == TRUE){
					// 읽어들인 정보의 맨 뒤에 '\0' 를 추가하여 문자열의 끝임을 명시한다.
					pipe_data[num_bytes_read] = '\0';

					// 읽어들인 정보가 만약 "Press any key to continue" 이라면 
					// 해당 문자열은 "\0" 로 대체하여 화면상에 출력이 되지 않도록 한다. 
					TCHAR *p_pos = _tcsstr(pipe_data, _T("Press any key to continue"));

					if (p_pos != NULL) *p_pos = '\0';
					
					if (m_bPipeDataShow)
					{// 읽어들인 정보가 출력된 현재 에디트 내의 문자열의 길이를 구한다.
						int len = m_richEdlt.GetWindowTextLength();

						// 현재 커서가 위치한 곳을 시작지점으로 읽어들인 정보를 에디트에 출력한다.
						int iFind;
						strTmp.Format("%s", pipe_data);
						while (!strTmp.IsEmpty())
						{		
							if ((iFind = strTmp.Find(SEQ_CRLF)) != -1)
							{
								strPiece = strTmp.Left(iFind + 2);
								strTmp = strTmp.Mid(iFind + 2);
							}
							else
							{
								strPiece = strTmp;
								strTmp.Empty();
							}

							//if (ChkCurLogLev(strPiece.GetAt(0)))
							{
								strLineNum.Format("[%04d] ", m_iLineNum++);
								strPipe += strLineNum;
								if (strPiece.GetAt(strPiece.GetLength() - 1) != '\n')
									strPiece += '\n';
								strPipe += strPiece;
							}
						}

						////////////////////////////
						//CPoint point;
						//m_richEdlt.SetSel(m_arrySearch.GetAt(m_iSearchIdx));

						//point = m_richEdlt.PosFromChar(m_arrySearch.GetAt(m_iSearchIdx).cpMin);
						//m_richEdlt.SetCaretPos(point);
						//m_richEdlt.SetFocus();
						////////////////////////////
						
						strPipe.Replace("\r", "");
						// 현재 에디트에 출력되어있는 문자열의 끝 위치로 커서를 이동한다.
						if (GetFocus() == GetDlgItem(IDC_RICHEDIT2_OUTPUT))
							::SetFocus(GetSafeHwnd());
						m_richEdlt.SetSel(len, len);
						m_richEdlt.ReplaceSel(strPipe);
						//if (m_bHold) m_richEdlt.SetSel(-1, -1);
					}
					else
					{
						PipeDataParsor(pipe_data);
					}
				}
			}
			success_flag = PeekNamedPipe(mh_pipe_read, pipe_data, 1,
				&num_bytes_read, &total_bytes_available, &bytes_left_this_message);
		}
		// 프로세스의 실행이 완료된 경우
		if (WaitForSingleObject(m_process_info.hProcess, 0) == WAIT_OBJECT_0){
			// 1번 타이머를 종료한다.
			if (!m_strSearchText.IsEmpty()) Search();
			if (m_bHold) m_richEdlt.SetSel(-1, -1);
			KillTimer(1);
			m_run_flag = 0;

			// 생성된 프로세스의 메인 스레드와 프로세스를 닫는다.
			CloseHandle(m_process_info.hThread);
			CloseHandle(m_process_info.hProcess);
			//MessageBox("명령어 실행이 완료되었습니다.", "작업 완료!!", MB_OK);
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CADBDlg::OnBnClickedButtonClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_iLineNum = 1;
	m_arrySearch.RemoveAll();
	m_iSearchIdx = 0;
	m_richEdlt.SetWindowText("");
}


BOOL CADBDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			CWnd *p_combo = GetDlgItem(IDC_COMBO_TAG);
			HWND h_wnd = ::FindWindowEx(p_combo->m_hWnd, NULL, _T("Edit"), NULL);
			CWnd* pCBWnd = CWnd::FromHandle(h_wnd);
			CWnd* pWnd = GetFocus();

			if (pWnd == GetDlgItem(IDC_EDIT_INPUT))
			{
				OnBnClickedButtonNext();
				GetDlgItem(IDC_EDIT_INPUT)->SetFocus();
			}
			else if (pWnd == pCBWnd)
			{
				OnBnClickedButtonEnter();
				pCBWnd->SetFocus();
			}
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CADBDlg::OnBnClickedButtonLogsave()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nLineLength;
	CString strText, strLine;
	CStdioFile file;

	char name_filter[] = "All Files (*.*)|*.*|";
	CFileDialog ins_dlg(FALSE, "", "", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |
		OFN_NOCHANGEDIR, name_filter, NULL);

	if (ins_dlg.DoModal() != IDOK){
		return;
	}
	CString filePath;
	filePath = ins_dlg.GetPathName();

	CFileException e;
	if (!file.Open(filePath, CFile::modeCreate | CFile::modeWrite, &e)){
		e.ReportError();
		return;
	}

	// Dump every line of text of the rich edit control.
	strLine.Empty();
	CString strTxt("");
	int iLineIndex = -1;
	for (int ii = 0; ii < m_richEdlt.GetLineCount(); ii++)
	{
		iLineIndex = m_richEdlt.LineIndex(ii);
		if (-1 == iLineIndex) continue;

		nLineLength = m_richEdlt.LineLength(iLineIndex);
		if (nLineLength <= 0) continue;

		m_richEdlt.GetLine(ii, strLine.GetBuffer(nLineLength), nLineLength);
		strTxt.Format("%s\n", CString(strLine, nLineLength));
		file.WriteString(strTxt);
		strLine.ReleaseBuffer();
	}
	file.Close();
}

void CADBDlg::PipeDataParsor(CString strPipe)
{
	int iFind = strPipe.Find(SEQ_CRLF);
	if (iFind < 0) return;

	CString strData, strDevice;
	int ii = 0;
	bool bAddItem = true;
	m_cbDevice.ResetContent();
	while ((iFind = strPipe.Find(DEVICE)) != -1)
	{
		strData = strPipe.Left(iFind);
		strPipe = strPipe.Mid(iFind + 1);

		iFind = strData.ReverseFind('\n');
		strData = strData.Mid(iFind + 1);

		for (int jj = 0; jj < m_cbDevice.GetCount(); jj++)
		{
			m_cbDevice.GetLBText(jj,strDevice);
			if (strDevice == strData)
			{
				bAddItem = false;
				break;
			}
		}

		if (bAddItem)
			m_cbDevice.InsertString(ii++, strData);
	}
	m_cbDevice.SetCurSel(0);
}

void CADBDlg::OnBnClickedButtonDevice()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	RunPipe(DEVICES, FALSE);
}

void CADBDlg::OnBnClickedButtonEnter()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strLogLev, strLogName, strData;
	//int iSel = m_cbTag.GetCurSel();
	m_cbLogLev.GetWindowText(strLogLev);
	m_cLogLev = strLogLev.GetAt(0);
	//m_cbTag.GetLBText(iSel, strLogName);
	m_cbTag.GetWindowText(strLogName);
	strData.Format("%s:%c", strLogName, m_cLogLev);
	//RunPipe("exit", FALSE);
	RunPipe(strData, TRUE);
}

void CADBDlg::ResetRichEditFormat()
{
	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	ft.lpstrText = m_strSearchText;

	CHARFORMAT2 cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_STRIKEOUT | CFM_COLOR | CFM_BACKCOLOR;
	cf.dwEffects = 0;
	cf.crTextColor = RGB(0, 0, 0);
	cf.crBackColor = RGB(255, 255, 255);

	{
		m_richEdlt.SetSel(0, -1);
		m_richEdlt.SetSelectionCharFormat(cf);
	}
}

void CADBDlg::Search()
{
	m_richEdlt.HideSelection(TRUE, TRUE);
	ResetRichEditFormat();
	///////////////////////////////////////////////////////////////////////////////////////////////////
	CString tmp;
	CPoint point;
	GetDlgItemText(IDC_EDIT_INPUT, tmp);
	if (m_strSearchText != tmp)
		m_iSearchIdx = 0;
	m_strSearchText = tmp;
	FINDTEXTEX ft;
	ft.chrg.cpMin = 0;
	ft.chrg.cpMax = -1;
	ft.lpstrText = tmp;

	CHARFORMAT2 cf;
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_STRIKEOUT | CFM_COLOR | CFM_BACKCOLOR;
	cf.dwEffects = 0;
	cf.crTextColor = RGB(255, 255, 255);
	cf.crBackColor = RGB(150, 58, 70);

	m_arrySearch.RemoveAll();
	int ii = 0;
	while ((m_richEdlt.FindText(FR_DOWN, &ft)) != -1)
	{
		m_arrySearch.InsertAt(ii++, ft.chrgText);
		m_richEdlt.SetSel(ft.chrgText);
		m_richEdlt.SetSelectionCharFormat(cf);

		ft.chrg.cpMin = ft.chrgText.cpMax;
		ft.chrg.cpMax = -1;
	}
	if (m_arrySearch.GetCount() > 0)
	{
		m_richEdlt.SetSel(m_arrySearch.GetAt(m_iSearchIdx));

		point = m_richEdlt.PosFromChar(m_arrySearch.GetAt(m_iSearchIdx).cpMin);
		m_richEdlt.SetCaretPos(point);
		m_richEdlt.SetFocus();
	}
	else
	{
		m_richEdlt.SetSel(-1, 0);
	}
	m_richEdlt.HideSelection(!m_bHold, TRUE);
}

void CADBDlg::OnBnClickedCheckHold()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bHold = m_bHold == TRUE ? FALSE : TRUE;
	m_richEdlt.HideSelection(!m_bHold, TRUE);

	if (m_bHold) m_richEdlt.SetSel(-1, -1);
}

BOOL CADBDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	//TRACE("==========================%x\n", HIWORD(wParam));
	switch (LOWORD(wParam))
	{
	case IDC_RICHEDIT2_OUTPUT:
		//TRACE("==========================%x\n", HIWORD(wParam));
		switch (HIWORD(wParam))
		{
		case EN_UPDATE:
		{
			//TRACE("======================================%x,%x\n", LOWORD(lParam), LOWORD(lParam));
			//SCROLLINFO si = { 0 };
			//si.cbSize = sizeof(SCROLLINFO);
			//si.fMask = SIF_TRACKPOS;
			//m_richEdlt.GetScrollInfo(SB_VERT, &si);
			//if (m_bHold)
			//{
			//	if (si.nMax > si.nPos + si.nPage)
			//	{
			//		OnBnClickedCheckHold();
			//	}
			//}
			//else
			//{
			//	if (si.nMax <= si.nPos + si.nPage)
			//	{
			//		OnBnClickedCheckHold();
			//	}
			//}
		}
			break;
		}
	}
	return CDialogEx::OnCommand(wParam, lParam);
}


void CADBDlg::OnEnKillfocusEditInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CADBDlg::OnEnSetfocusEditInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//Search();
}


void CADBDlg::OnEnChangeEditInput()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	Search();
	GetDlgItem(IDC_EDIT_INPUT)->SetFocus();
}


void CADBDlg::OnBnClickedButtonPrev()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (m_arrySearch.GetCount() > 0)
	{
		m_iSearchIdx--;
		if (m_iSearchIdx < 0)
			m_iSearchIdx = m_arrySearch.GetCount() - 1;
		m_richEdlt.SetSel(m_arrySearch.GetAt(m_iSearchIdx));
		CPoint point;
		point = m_richEdlt.PosFromChar(m_arrySearch.GetAt(m_iSearchIdx).cpMin);
		m_richEdlt.SetCaretPos(point);
		m_richEdlt.SetFocus();
	}
}


void CADBDlg::OnBnClickedButtonNext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_arrySearch.GetCount() > 0)
	{
		m_iSearchIdx++;
		if (m_iSearchIdx >= m_arrySearch.GetCount())
			m_iSearchIdx = 0;
		m_richEdlt.SetSel(m_arrySearch.GetAt(m_iSearchIdx));

		CPoint point;
		point = m_richEdlt.PosFromChar(m_arrySearch.GetAt(m_iSearchIdx).cpMin);
		m_richEdlt.SetCaretPos(point);
		m_richEdlt.SetFocus();
	}
}

int CADBDlg::GetLogLev(char cSeq)
{
	int iLogLev = 0;
	switch (cSeq)
	{
	case 'A':
		iLogLev = 5;
		break;
	case 'E':
		iLogLev = 4;
		break;
	case 'W':
		iLogLev = 3;
		break;
	case 'I':
		iLogLev = 2;
		break;
	case 'D':
		iLogLev = 1;
		break;
	case 'V':
		iLogLev = 0;
		break;
	}

	return iLogLev;
}

bool CADBDlg::ChkCurLogLev(char cSeq)
{
	int iReadLogLev = GetLogLev(cSeq);
	int iLogLev = GetLogLev(m_cLogLev);
	
	if (iReadLogLev <= iLogLev)
		return true;
	else
		return false;
}

void CADBDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (!m_bInit)
		return;

	CRect rc, wRect;
	GetClientRect(&rc);
	m_richEdlt.GetWindowRect(&wRect);
	ScreenToClient(wRect);
	wRect.right = rc.right - 8;
	wRect.bottom = rc.bottom - 6;
	m_richEdlt.MoveWindow(wRect);
}

void CADBDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMinTrackSize.x = 939;		 // 최소값
	lpMMI->ptMaxTrackSize.x = 939;		 // 최대값

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


void CADBDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnOK();
}
