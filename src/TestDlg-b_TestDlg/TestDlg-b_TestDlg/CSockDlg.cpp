// CSockDlg.cpp: 구현 파일
//

#include "pch.h"
#include "TestDlg.h"
#include "CSockDlg.h"
#include "afxdialogex.h"
//#include "../../h/axis.h"
#include "h/axis.h"
#include "h/axisfm.h"
#include "socket.h"

// CSockDlg 대화 상자
#define DF_TRKEY_POOP 1

#define TM_CONNECT 9999
#define TM_RECEIVE    9998

IMPLEMENT_DYNAMIC(CSockDlg, CDialogEx)

CSockDlg::CSockDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SOCK, pParent)
{

}

CSockDlg::~CSockDlg()
{
}

void CSockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSockDlg, CDialogEx)
	ON_WM_TIMER()
	ON_MESSAGE(WM_USER + 12, OnSockManage)
	ON_BN_CLICKED(IDC_BTN_TESTSEND, &CSockDlg::OnBnClickedBtnTestsend)
END_MESSAGE_MAP()


// CSockDlg 메시지 처리기


BOOL CSockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (m_sock)
		m_sock.reset();

	m_sock = std::make_unique<Csocket>(this);
	if (m_sock->Open(("211.255.204.134"), 15201))
	{
		SetTimer(TM_CONNECT, 2000, nullptr);
	}
	else
	{
		m_sock.reset();
		AfxMessageBox("csock connect fail");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CSockDlg::SockWrite_piboac10()
{
	CString strTemp;

	//std::unique_ptr<char[]>buff = std::make_unique<char[]>(12);
	//int ilen = strlen(buff.get());
	int ilen = L_fmH;
	ilen = L_axisH;
	char* buff = "            ";
	ilen = strlen(buff);

	const int datl = L_fmH + L_axisH + ilen;
	std::unique_ptr<char[]>datb = std::make_unique<char[]>(datl);

	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];

	char* pdata = (char*)&datb[L_fmH + L_axisH];
	memcpy(pdata, buff, ilen);


	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	strTemp.Format("%05d", L_axisH + strlen(pdata));
	CopyMemory(fmH->datL, strTemp, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = DF_TRKEY_POOP;
	CopyMemory(axisH->trxC, "piboac10", sizeof(axisH->trxC));
	strTemp.Format("%05d", strlen(pdata));
	CopyMemory(axisH->datL, strTemp, sizeof(axisH->datL));

	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 2000, NULL);
}

void CSockDlg::SockWrite_Something()
{
	CString strTemp;

	std::unique_ptr<char[]>buff = std::make_unique<char[]>(1024);
	sprintf_s((char*)buff.get(), 1024, "1301%c%s\t1021\t1023\t1306\t1034\t", 0x7f, "005930");
	int ilen = strlen(buff.get());
	
	const int datl = L_fmH + L_axisH + ilen;
	std::unique_ptr<char[]>datb = std::make_unique<char[]>(datl);

	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];

	char* pdata = (char*)&datb[L_fmH + L_axisH];
	memcpy(pdata, buff.get(), ilen);
	

	// fmH
	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	strTemp.Format("%05d", L_axisH + strlen(pdata));
	CopyMemory(fmH->datL, strTemp, sizeof(fmH->datL));

	// axisH
	axisH->msgK = msgK_AXIS;
	axisH->winK = winK_NORM;
	axisH->trxK = DF_TRKEY_POOP;
	CopyMemory(axisH->trxC, "pooppoop", sizeof(axisH->trxC));
	strTemp.Format("%05d", strlen(pdata));
	CopyMemory(axisH->datL, strTemp, sizeof(axisH->datL));

	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 2000, NULL);
}

void CSockDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_CONNECT:
		{
			KillTimer(nIDEvent);
			if (m_sock)
				m_sock.reset();
		}
		case TM_RECEIVE:
			KillTimer(nIDEvent);
			if (m_sock)
				m_sock.reset();
			break;
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

LONG CSockDlg::OnSockManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case sm_CLOSE:
		if (m_sock)
			m_sock.reset();
		break;

	case sm_CONNECT:
		KillTimer(TM_CONNECT);
		
		break;

	case sm_RECEIVE:
		KillTimer(TM_CONNECT);
		char* pdata = (char*)lParam;
	//	KillTimer(TM_RECEIVE);
	//	dispatch((char*)lParam, (int)HIWORD(wParam));
		break;
	}
	return 0;
}

void CSockDlg::OnBnClickedBtnTestsend()
{
	//SockWrite_Something();
	SockWrite_piboac10();
}
