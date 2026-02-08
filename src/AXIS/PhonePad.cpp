// PhonePad.cpp : implementation file
//

#include "stdafx.h"
#include "axis.h"
#include "PhonePad.h"
#include "trsmain.h"
#include "PhoneThread.h"
#include "axMsg.hxx"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhonePad dialog


CPhonePad::CPhonePad(CWnd* pParent /*=NULL*/)
	: CDialog(CPhonePad::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPhonePad)
	m_msg = _T("입력 수신 시작 버튼을 클릭해주세요.");
	m_pwd = _T("");
	//}}AFX_DATA_INIT
}


void CPhonePad::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPhonePad)
	DDX_Text(pDX, IDC_STATIC_MSG, m_msg);
	DDX_Text(pDX, IDC_EDIT1, m_pwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPhonePad, CDialog)
	//{{AFX_MSG_MAP(CPhonePad)
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDCANCEL, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhonePad message handlers

void CPhonePad::OnButton1() 
{
//	CPhoneThread* pt;
//	pt->CreateThread();
//	pt->Run();
	//pt.SuspendThread();
	//pt.ResumeThread();
//	return;
	GetDlgItem(IDC_BUTTON1)->EnableWindow(false);
	m_pwd = "";
	UpdateData(true);
	// TODO: Add your control notification handler code here
	if(PHONEPAD_PortOpen(0))
	{
		PHONEPAD_InitDevice();
		PHONEPAD_SetConfig(0,10);	
		PHONEPAD_GetPWSignalWindow(m_hWnd);
		//asdfasdf
		m_msg = "폰패드 연결 성공=>입력수신 대기중";
		//MessageBox("폰패드 연결 성공(Port Open Success)!!","IBK투자증권");
	}
	else
	{
		//char * tmp = "0423";
		//PostMessage(WM_PHONEPAD,0,(long)tmp);
		m_msg = "폰패드 연결 실패";
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		//MessageBox("폰패드 연결 실패(Port Open Failure)!!","IBK투자증권");
		UpdateData(true);
		return;
	}
	UpdateData(false);
	UpdateData(true);
	//==============비밀번호 읽어오기 시작============
	// TODO: Add your control notification handler code here
	char * temp = NULL;
	int  loop=1;
	int  limit = 0;
	
	
	while(loop)
	{			
		//temp = PHONEPAD_GetPassWDSTR();
		//if(temp != NULL) break;		
		Sleep(300);
		
		switch(PHONEPAD_Status())
		{
		case 0: 
			break;     // normal
			
		case 1: 
			loop = 0; 
			temp = PHONEPAD_GetPassWDSTR();
			break;     //  비밀번호 입력완료
			
		case 2: 
			m_msg = "시간을 초과하였습니다.";
			MessageBox("시간을 초과하였습니다.","IBK투자증권");
			GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
			PHONEPAD_PortClose();
			return; 
			break;       //  비밀번호 입력취소
		}
		Sleep(300);
		
		limit++;
		if (limit>=30) break;
	}
	if (limit == 30)
	{
		//m_phone_dlgs = NULL;
		m_msg = "입력 시간이 초과되었습니다.";
		MessageBox("입력 시간이 초과되었습니다.","IBK투자증권");
		GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
		UpdateData(true);
		PHONEPAD_PortClose();
		return;
	}	
	//받아온 비밀번호
	//AfxMessageBox((LPCTSTR)temp);
	CString g_pwd = temp;
	//AfxMessageBox(g_pwd);
	g_pwd.TrimLeft(); g_pwd.TrimLeft();
	//g_pwd.Delete(g_pwd.GetLength()-1);
	//AfxMessageBox("g_pwd: "+g_pwd);
	//SetPwd(g_pwd);
	//PostMessage(WM_PHONEPAD,0,(long)g_pwd.operator LPCTSTR());
	//m_stpw.SetWindowText((LPCTSTR)temp);
	m_pwd = g_pwd;
	m_msg = "입력이 완료되었습니다.";
	//==============비밀번호 읽어오기 끝============
	UpdateData(false);
	GetDlgItem(IDC_BUTTON1)->EnableWindow(true);
	//==============포트 닫기============
	PHONEPAD_PortClose();	
}

void CPhonePad::OnButton2() 
{
	// TODO: Add your control notification handler code here
	//CloseWindow();
	//DestroyWindow();
	CDialog::OnCancel();
}

void CPhonePad::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	//MessageBox(m_pwd);
	::SendMessage(main_hwnd,WM_PHONEPAD,0,(long)m_pwd.operator LPCTSTR());
	
	CDialog::OnOK();
}
