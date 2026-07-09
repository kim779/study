// UserSaveDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "cx_fileupdown.h"
#include "UserSaveDlg.h"
#include "afxdialogex.h"
#include "Controlwnd.h"
#include "../../../h/axisfire.h"

// CUserSaveDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CUserSaveDlg, CDialogEx)

CUserSaveDlg::CUserSaveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CUserSaveDlg::IDD, pParent)
{
	m_pParent = (CControlwnd*)pParent;
}

CUserSaveDlg::~CUserSaveDlg()
{
}

void CUserSaveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_UPDOWN, m_tab);
}


BEGIN_MESSAGE_MAP(CUserSaveDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_UPDOWN, &CUserSaveDlg::OnSelchangeTabUpdown)
	ON_BN_CLICKED(IDC_BTN_UPDOWN, &CUserSaveDlg::OnBnClickedBtnUpdown)
	ON_BN_CLICKED(IDCANCEL, &CUserSaveDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CUserSaveDlg 메시지 처리기입니다.


BOOL CUserSaveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	m_pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_UPDOWN);
	//ScreenToClient(&rect);
	m_pTab->GetClientRect(&rect);
	TC_ITEM item;
	item.mask = TCIF_TEXT;

	item.pszText = _T("Up");
	m_pTab->InsertItem(0, &item);

	item.pszText = _T("Down");
	m_pTab->InsertItem(1, &item);

	m_pUpDlg = new CUpdlg;
	m_pUpDlg->Create(IDD_DLGUP, m_pTab);
	m_pUpDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pUpDlg->ShowWindow(SW_SHOW);


	m_pDownDlg = new CDowndlg;
	m_pDownDlg->Create(IDD_DLGDOWN, m_pTab);
	m_pDownDlg->MoveWindow(0, 20, rect.Width(), rect.Height());
	m_pDownDlg->ShowWindow(SW_HIDE);



	CPoint pt;
	GetCursorPos(&pt);
	//ClientToScreen(&pt);
	SetDlgPos(pt.x, pt.y);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}



void CUserSaveDlg::SetDlgPos(int x, int y)
{
	if(x < 0 || y < 0)
		return;

	CRect rc;
	GetClientRect(rc);
	SetWindowPos(NULL, x, y, rc.Width(), rc.Height() + 25, SWP_SHOWWINDOW);
	/*
	int scWidth, scHeight;
	scWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	scHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	CRect rc;
	GetClientRect(&rc);

	int gabX = scWidth - (x - rc.Width());
	int	gabY = scHeight - (y + rc.Height());
	if (gabX < 0)
		x += gabX - 7;
	if (gabY < 0)
		y += gabY - 7;

	SetWindowPos(NULL, x, y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
	*/
}


void CUserSaveDlg::OnSelchangeTabUpdown(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nTab = m_pTab->GetCurSel();

	switch(nTab)
	{
		case 0:
			m_pUpDlg->ShowWindow(SW_SHOW);
			m_pDownDlg->ShowWindow(SW_HIDE);
			((CWnd*)GetDlgItem(IDC_BTN_UPDOWN))->SetWindowTextA(_T("Up load"));
			break;
		case 1:
			m_pUpDlg->ShowWindow(SW_HIDE);
			m_pDownDlg->ShowWindow(SW_SHOW);
			((CWnd*)GetDlgItem(IDC_BTN_UPDOWN))->SetWindowTextA(_T("Down load"));
			break;
	}
	*pResult = 0;
}

void CUserSaveDlg::OnBnClickedBtnUpdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	
}

void CUserSaveDlg::sendTR(CString trC, char* pBytes, int nBytes)
{
	CParam *pParam =(CParam*)m_pParent->Getparam();
	int	len = L_userTH + nBytes;
	char*	sndB = new char[len];
	struct _userTH* udat = (struct _userTH*) sndB;

	ZeroMemory(sndB, sizeof(sndB));
	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = pParam->key;	
	udat->stat = 0;
	CopyMemory(&sndB[L_userTH], pBytes, nBytes);
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)sndB);
	delete [] sndB;

	/*
	CString tmpS, temp;
	struct	_userTH* userTH;	
	int nsize = sizeof(struct _ledgerH) + sizeof(struct mid) + 1;
	//char*	cDataBuffer = new char [nsize];
	char	cDataBuffer[10000];
	memset(cDataBuffer, ' ' , nsize);
	CParam *pParam =(CParam*)m_pParent->Getparam();
	sprintf(cDataBuffer, "%s\t", pParam->name);

	userTH = (struct _userTH *)&cDataBuffer[pParam->name.GetLength() + 1];

	strcpy(userTH->trc, _T("PIBOPFRM"));
	userTH->key = pParam->key;
	userTH->stat = 0;

	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	FillMemory(pledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH);

	tmpS = CString((char *)&ledgerH, sizeof(struct  _ledgerH));
	*/
	/*
	struct mid* pmid = new struct mid;
	FillMemory(pmid, sizeof(struct  mid), ' ');
	
	memcpy((char*)pmid->user, (char*)"test", 30);
	memcpy((char*)pmid->pswd, (char*)"1111", 16);
	temp = CString((char *)pmid, sizeof(struct  mid));
	int nn = temp.GetLength();
	tmpS += temp;
	*/
	/*
	CopyMemory(&cDataBuffer[L_userTH + pParam->name.GetLength() + 1], tmpS, tmpS.GetLength());
	
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, tmpS.GetLength()), (LPARAM) cDataBuffer);

	delete[] cDataBuffer;
	*/
	/*
	CParam *pParam =(CParam*)m_pParent->Getparam();
	CString strData(pBytes, nBytes);
	CString strTRData = "";
	CString tmpS, temp;
	tmpS = _T("PIBOPFRM");
	int nOutLen = 0;
		
	strTRData = pParam->name;
	strTRData += _T('\t');
	
	struct _userTH user;
	FillMemory(&user, sizeof(struct  _userTH), ' ');
	memcpy(user.trc, (LPSTR)(LPCTSTR)tmpS, tmpS.GetLength());
	user.key  = pParam->key;
	user.stat = 0;
	
	strTRData += CString((char*)&user, sizeof(_userTH));
	nOutLen = strTRData.GetLength();
	
	//Ledger
	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	FillMemory(pledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH);

	tmpS = CString((char *)&ledgerH, sizeof(struct  _ledgerH));
	strTRData += tmpS;
	
	struct mid
	{
		char user[30];
		char pswd[16];
		char uuid[36];
		char uuidrec[1];
		char ip[15];
	};

	struct mid* pmid = new struct mid;
	FillMemory(pmid, sizeof(struct  mid), ' ');

	memcpy((char*)pmid->user, (char*)"test", 30);
	memcpy((char*)pmid->pswd, (char*)"1111", 16);
	tmpS = CString((char *)pmid, sizeof(struct  mid));
	int nn = tmpS.GetLength();
	strTRData += tmpS;
	
	char* psend = new char[strTRData.GetLength()];
	sprintf(psend, "%s", strTRData);
//	int nret = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strTRData.GetLength() - nOutLen), (LPARAM)(LPCTSTR)strTRData);
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx,  strTRData.GetLength() - nOutLen), (LPARAM) psend);
	return;
	*/
}

void CUserSaveDlg::GetledgerH(struct _ledgerH* pLedgerH)
{
	_ledgerH ledger;
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

	strtemp = "FITSComLoginSO";
	memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

	strtemp = "selectLoginInfo";
	memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
}



void CUserSaveDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}
