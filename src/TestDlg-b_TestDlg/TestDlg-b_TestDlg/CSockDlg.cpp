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
	m_ss = nullptr;
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
	ON_BN_CLICKED(IDC_BTN_AXISENCX, &CSockDlg::OnBnClickedBtnAxisencx)
	ON_BN_CLICKED(IDC_BTN_piboac10, &CSockDlg::OnBnClickedBtnpiboac10)
END_MESSAGE_MAP()


// CSockDlg 메시지 처리기


BOOL CSockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	if (m_sock)
		m_sock.reset();   //최초 초기화

	m_sock = std::make_unique<Csocket>(this);
	if (m_sock->Open(("211.255.204.104"), 15201))
	{
		SetTimer(TM_CONNECT, 2000, nullptr);
	}
	else
	{
		m_sock.reset();  //소켓 connect 실패
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
			//if (m_sock)
			//	m_sock.reset();
		}
		case TM_RECEIVE:
			KillTimer(nIDEvent);
		//	if (m_sock)
			//	m_sock.reset();
			break;
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}

LONG CSockDlg::OnSockManage(WPARAM wParam, LPARAM lParam)
{
	CString msg;
	switch (LOWORD(wParam))
	{
	case sm_CLOSE:
		if (m_sock)
			m_sock.reset();   //sm_close
		break;

	case sm_CONNECT:
		KillTimer(TM_CONNECT);
	
		msg.Format("[Csocket] 소켓연결성공");
		OutputDebugString(msg);
		break;

	case sm_RECEIVE:
		KillTimer(TM_RECEIVE);
		char* frame = (char*)lParam;
		struct _axisH* axisH = (struct _axisH*)&frame[L_fmH];
		int datL = atoi(CString(axisH->datL, sizeof(axisH->datL)));
		char* pdata = &frame[L_fmH + L_axisH];

		CString hex;
		for (int i = 0; i < datL && i < 60; i++)
			hex.AppendFormat("%02X ", (unsigned char)pdata[i]);

		CString msg;
		msg.Format("[Csocket] 응답 수신: msgK=%d trxC=%.8s datL=%d\n%s", axisH->msgK, axisH->trxC, datL, hex);
		OutputDebugString(msg);

		if (axisH->msgK == msgK_ENC && m_ss)
		{
			unsigned char outBuf[65536];
			int outLen = sizeof(outBuf);
			int rc = SS_Handshake(m_ss, (unsigned char*)pdata, datL, outBuf, &outLen);

			CString hsMsg;
			hsMsg.Format("[Csocket][SS_Handshake] rc=%d outLen=%d", rc, outLen);
			OutputDebugString(hsMsg);
			AfxMessageBox(hsMsg);
		}

		if (axisH->stat & statENC && m_ss)
		{
			unsigned char decBuf[65536] {};
			int decLen = sizeof(decBuf);
			int rc = SS_Decrypt(m_ss, (unsigned char*)pdata, datL, decBuf, &decLen);

			CString decMsg;
			if (rc == 0)
				decMsg.Format("[Csocket][SS_Decrypt] 성공 decLen=%d 내용=[%.*s]", decLen, min(decLen, 100), (char*)decBuf);
			else
				decMsg.Format("[Csocket][SS_Decrypt] 실패 rc=%d", rc);
			OutputDebugString(decMsg);
			AfxMessageBox(decMsg);
		}

		msg.Format("[Csocket]-------------------------------------------");
		 OutputDebugString(msg);
		break;
	}
	return 0;
}

void CSockDlg::OnBnClickedBtnTestsend()
{
	//SockWrite_Something();
	SockWrite_EncPooppoop();
//	SockWrite_piboac10();
//SockWrite_AXISENCX();
}



#pragma comment (lib, "D:\\src\\IBKS\\src\\ibks\\securesession\\Release\\securesession.lib")
void CSockDlg::SockWrite_AXISENCX()
{
	CString msg;
	unsigned char helloBuf[65536];
	int helloLen = sizeof(helloBuf);

	m_ss = SS_Open("qwer1234", "C:\\IBKS\\IBK투자증권 HTS\\exe\\xc_conf.ini", helloBuf, &helloLen);
	if (!m_ss)
	{
		AfxMessageBox("SS_Open 실패");
		return;
	}
	msg.Format("[Csocket][%s]<%d> SockWrite_AXISENCX   ------------ m_ss =  [%x]", __FUNCTION__, __LINE__, m_ss);
	OutputDebugString(msg);

	const int datl = L_fmH + L_axisH + helloLen;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);

	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	char* pdata = (char*)&datb[L_fmH + L_axisH];
	memcpy(pdata, helloBuf, helloLen);

	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	CString strTemp;
	strTemp.Format("%05d", L_axisH + helloLen);
	CopyMemory(fmH->datL, strTemp, sizeof(fmH->datL));

	axisH->msgK = msgK_ENC;
	axisH->winK = winK_NORM;
	CopyMemory(axisH->trxC, "AXISENCX", sizeof(axisH->trxC));
	strTemp.Format("%05d", helloLen);
	CopyMemory(axisH->datL, strTemp, sizeof(axisH->datL));

	CString hex;
	for (int i = 0; i < datl && i < 50; i++)
		hex.AppendFormat("%02X ", (unsigned char)datb[i]);

	
	msg.Format("[Csocket][%s]<%d> SockWrite_AXISENCX hex =  [%.100s]", __FUNCTION__, __LINE__,hex);
	OutputDebugString(msg);
	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 5000, NULL);
}

void CSockDlg::OnBnClickedBtnAxisencx()
{
	SockWrite_AXISENCX();
}


void CSockDlg::OnBnClickedBtnpiboac10()
{
	SockWrite_piboac10();
}

void CSockDlg::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
	delete this;   // 모달리스 다이얼로그는 닫힐 때 스스로 메모리를 정리해야 함
}

void CSockDlg::OnCancel()
{
	DestroyWindow();   // 모달용 EndDialog() 대신 DestroyWindow() 사용
}

void CSockDlg::SockWrite_EncPooppoop()
{
	CString msg;
	if (!m_ss)
	{
		AfxMessageBox("먼저 AXISENCX로 핸드셰이크를 완료하세요");
		return;
	}

	char plain[1024];
	sprintf_s(plain, sizeof(plain), "1301%c%s\t1021\t1023\t1306\t1034\t", 0x7f, "005930");
	int plainLen = (int)strlen(plain);

	unsigned char encBuf[65536];
	int encLen = sizeof(encBuf);
	if (SS_Encrypt(m_ss, (unsigned char*)plain, plainLen, encBuf, &encLen) != 0)
	{
		AfxMessageBox("SS_Encrypt 실패");
		return;
	}

	msg.Format("[Csocket][%s]<%d> SS_Encrypt encLen =[%d] encBuf =  [%.100s]", __FUNCTION__, __LINE__, encLen, encBuf);
	OutputDebugString(msg);

	const int datl = L_fmH + L_axisH + encLen;
	std::unique_ptr<char[]> datb = std::make_unique<char[]>(datl);

	struct _fmH* fmH = (struct _fmH*)datb.get();
	struct _axisH* axisH = (struct _axisH*)&datb[L_fmH];
	char* pdata = (char*)&datb[L_fmH + L_axisH];
	memcpy(pdata, encBuf, encLen);

	fmH->fmF[0] = fmF_FS;
	fmH->fmF[1] = fmF_FS;
	fmH->fmC = fmC_SSM;
	fmH->ssM = ssM_WS;
	fmH->stat = stat_WS;
	CString strTemp;
	strTemp.Format("%05d", L_axisH + encLen);
	CopyMemory(fmH->datL, strTemp, sizeof(fmH->datL));

	axisH->msgK = msgK_AXIS;
	axisH->stat |= statENC;      // 암호화된 데이터임을 표시
	axisH->winK = winK_NORM;
	axisH->trxK = DF_TRKEY_POOP;
	CopyMemory(axisH->trxC, "pooppoop", sizeof(axisH->trxC));
	strTemp.Format("%05d", encLen);
	CopyMemory(axisH->datL, strTemp, sizeof(axisH->datL));

	if (m_sock->Write(datb.get(), datl))
		SetTimer(TM_RECEIVE, 5000, NULL);
}