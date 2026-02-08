#include "stdafx.h"
#include "axis.h"
#include "EmpPassChangeDlg.h"
#include "../h/ledger.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CEmpPassChangeDlg::CEmpPassChangeDlg(CWnd* pParent, BOOL bCloseEnable)
	: CDialog(CEmpPassChangeDlg::IDD, pParent), m_bCloseEnable(bCloseEnable)
{
	//{{AFX_DATA_INIT(CEmpPassChangeDlg2)
	m_newpswd2 = _T("");
	m_newpswd = _T("");
	m_oldpswd = _T("");
	//}}AFX_DATA_INIT
	m_pMain = (CMainFrame *)pParent;
}


void CEmpPassChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEmpPassChangeDlg2)
	DDX_Text(pDX, IDC_CFR_PSWD, m_newpswd2);
	DDV_MaxChars(pDX, m_newpswd2, 16);
	DDX_Text(pDX, IDC_NEW_PSWD, m_newpswd);
	DDV_MaxChars(pDX, m_newpswd, 16);
	DDX_Text(pDX, IDC_OLD_PSWD, m_oldpswd);
	DDV_MaxChars(pDX, m_oldpswd, 16);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEmpPassChangeDlg, CDialog)
	//{{AFX_MSG_MAP(CEmpPassChangeDlg2)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_SEND, OnSend)
	ON_BN_CLICKED(IDC_BTN_CLOSE, OnBtnClose)
	ON_EN_MAXTEXT(IDC_NEW_PSWD, OnMaxText)
	ON_EN_MAXTEXT(IDC_CFR_PSWD, OnMaxText)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEmpPassChangeDlg2 message handlers

void CEmpPassChangeDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);
	
// 	CString emsg;
// 	emsg.Format("%d/%d %d/%d %X\n", rc.left, rc.top, rc.Width(), rc.Height(), m_hBack);
// 	OutputDebugString(emsg);
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap *pBmp = CBitmap::FromHandle(m_hBack);
	HBITMAP pOldBmp = (HBITMAP)memDC.SelectObject(pBmp);
	dc.BitBlt(0, 0, rc.Width(), rc.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
	memDC.DeleteDC();
	GetDlgItem(IDC_MSG)->Invalidate(TRUE);
}

BOOL CEmpPassChangeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_brush.CreateSolidBrush(RGB(0xDD,0xDD,0xDD));
	
	m_hBack = LoadBitmap(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDB_EMPPASSCHANGE));
	if (!m_hBack)
	{
		CString emsg;
		emsg.Format("Image is null - %d\n", GetLastError());
		OutputDebugString(emsg);
	}

	CRect rc;
	GetDlgItem(IDC_PHOLDER)->GetWindowRect(&rc); ScreenToClient(rc);
	m_btnSend.Create("비밀번호 변경", WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_BITMAP, rc, this, IDC_BTN_SEND);
	m_btnSend.m_hBitmap    = Axis::GetBitmap("6BTN_조회")->operator HBITMAP();
	m_btnSend.m_hBitmap_dn = Axis::GetBitmap("6BTN_조회_DN")->operator HBITMAP();
	m_btnSend.m_hBitmap_hv = Axis::GetBitmap("6BTN_조회_EN")->operator HBITMAP();

	if (m_bCloseEnable)
	{
		GetDlgItem(IDC_PHOLDER2)->GetWindowRect(&rc); ScreenToClient(rc);
		m_btnClose.Create("", WS_CHILD|WS_VISIBLE|BS_BITMAP, rc, this, IDC_BTN_CLOSE);
		m_btnClose.m_hBitmap    = Axis::GetBitmap("레이어팝업닫기")->operator HBITMAP();
		m_btnClose.m_hBitmap_dn = Axis::GetBitmap("레이어팝업닫기_DN")->operator HBITMAP();
		m_btnClose.m_hBitmap_hv = Axis::GetBitmap("레이어팝업닫기_EN")->operator HBITMAP();
	}

	return FALSE;
}

void CEmpPassChangeDlg::OnDestroy() 
{
	m_brush.DeleteObject();
	::DeleteObject(m_hBack);
	CDialog::OnDestroy();
}

void CEmpPassChangeDlg::OnSend()
{
//	OutputDebugString("OnSend\n");
	UpdateData(TRUE);

	CEdit *pEdit = NULL;
	if (m_oldpswd.IsEmpty())
	{
		MessageBox("기존비밀번호를 입력하십시오.", "IBK투자증권", MB_OK);
		pEdit = (CEdit*)GetDlgItem(IDC_OLD_PSWD);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}
	if (m_newpswd.IsEmpty())
	{
		MessageBox("변경될 비밀번호를 입력하십시오.", "IBK투자증권", MB_OK);
		pEdit = (CEdit*)GetDlgItem(IDC_NEW_PSWD);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}
	if (m_newpswd != m_newpswd2)
	{
		MessageBox("변경될 비밀번호가 일치하지 않습니다.", "IBK투자증권", MB_OK);
		pEdit = (CEdit*)GetDlgItem(IDC_CFR_PSWD);
		pEdit->SetSel(0, -1);
		pEdit->SetFocus();
		return;
	}

	char data[1024];
	FillMemory(data, sizeof(data), ' ');
	_ledgerH *ledger = (_ledgerH*)&data[0];
	sbpgt104_mid *mid = (sbpgt104_mid*)&data[L_ledgerH];
	
	memcpy(ledger->svcd, "SBPGT104", 8);
	memcpy(ledger->fkey, "5   ", 4);
	memcpy(ledger->usid, Axis::userID, Axis::userID.GetLength());
	ledger->odrf[0] = '1';
	ledger->mkty[0] = '3';
	memcpy(ledger->pcip, Axis::userIP, Axis::userIP.GetLength());
	
	memcpy(mid->In, "00001", 5);
	memcpy(mid->LoginPwd, m_oldpswd, m_oldpswd.GetLength());
	memcpy(mid->ChgLoginPwd, m_newpswd, m_newpswd.GetLength());
	
	m_pMain->sendTR("pibopbxq", data, L_ledgerH+sizeof(sbpgt104_mid), US_ENC, 'k');

	GetDlgItem(IDC_MSG)->Invalidate(TRUE);
}

BOOL CEmpPassChangeDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_RETURN)
		{
			const int nID = ::GetDlgCtrlID(pMsg->hwnd);
			
			if (nID==IDC_NEW_PSWD || nID==IDC_OLD_PSWD || nID==IDC_CFR_PSWD)
			{
				NextDlgCtrl();
				return TRUE;
			}
			else if (nID==IDC_BTN_SEND)
			{
				OnSend();
			}
		}	
		else if (pMsg->wParam==VK_ESCAPE)
			return TRUE;
		else if (pMsg->wParam==VK_TAB)
		{
			NextDlgCtrl();
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

void CEmpPassChangeDlg::processOUB( LPCSTR dat, int len )
{
	const  _ledgerH *ledger = (_ledgerH *)dat;
	
	CString ecod(ledger->ecod, sizeof(ledger->ecod));
	CString emsg(ledger->emsg, sizeof(ledger->emsg));
	emsg.TrimRight();
	
	CWnd *pWnd = GetDlgItem(IDC_MSG);
	if (pWnd)
	{
		pWnd->SetWindowText(emsg);
	}
	
	if (ledger->ecod[0]<'1')
	{
		EndDialog(IDOK);
	}
	else
	{
		CEdit *pEdit = NULL;
		if (ecod=="1801")
		{
			pEdit = (CEdit*)GetDlgItem(IDC_OLD_PSWD);
			pEdit->SetSel(0, -1);
			pEdit->SetFocus();
		}
		else
		{
			pEdit = (CEdit*)GetDlgItem(IDC_NEW_PSWD);
			pEdit->SetSel(0, -1);
			pEdit->SetFocus();
		}
	}
	
}

void CEmpPassChangeDlg::OnOk()
{
//	OutputDebugString("OnOk\n");
}

void CEmpPassChangeDlg::OnCancel()
{
//	OutputDebugString("OnCancel\n");
}

HBRUSH CEmpPassChangeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	const int nCtrlID = pWnd->GetDlgCtrlID();
	if (nCtrlID==IDC_MSG)
	{
		HGDIOBJ hBrush = m_brush.GetSafeHandle();
		SetBkMode(pDC->m_hDC, TRANSPARENT);
		return (HBRUSH)hBrush;
	}

	return hbr;
}

void CEmpPassChangeDlg::OnBtnClose()
{
	EndDialog(IDCANCEL);
}

void CEmpPassChangeDlg::OnMaxText()
{
	MessageBox("비밀번호는 최대 16자리까지 가능합니다", "IBK투자증권", MB_OK|MB_ICONINFORMATION);
}
