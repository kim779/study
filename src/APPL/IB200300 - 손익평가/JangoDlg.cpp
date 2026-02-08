// JangoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IB200300.h"
#include "JangoDlg.h"
#include "../../h/ledger.h"
#include "ibkjango.h"

#define	LEDGER_ECODE(param)	(((_ledgerH*)param)->ecod)
#define	LEDGER_CONF(param)	(((_ledgerH*)param)->next[0])

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CJangoDlg dialog


CJangoDlg::CJangoDlg(CWnd *pView, CWnd* pParent /*=NULL*/)
	: CDialog(CJangoDlg::IDD, pParent)
{
	m_pView = pView;
	m_pParent = pParent;
	m_pAccount = NULL;

	m_sAccn = "";
	m_sPass = "";

	m_arJango.RemoveAll();

	//{{AFX_DATA_INIT(CJangoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CJangoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CJangoDlg)
	DDX_Control(pDX, IDC_LISTJANGO, m_ListJango);
	DDX_Control(pDX, IDC_ACCNNAME, m_AccnName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CJangoDlg, CDialog)
	//{{AFX_MSG_MAP(CJangoDlg)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(ACNT_MSG, OnAccount)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CJangoDlg message handlers

void CJangoDlg::OnDestroy() 
{
	if (m_pAccount)
	{
		m_pAccount->DestroyWindow();
		m_pAccount = nullptr;
	}

	CDialog::OnDestroy();
}

BOOL CJangoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CRect	rc;
	GetDlgItem(IDC_ACCNPOS)->GetWindowRect(&rc);
	ScreenToClient(rc);

	m_sPass.Format("%s", Variant(passCCx, ""));
	CString	gubn = _T("");
	gubn.Format("%s", Variant(systemCC, ""));
	if (!gubn.IsEmpty())
		m_Gubn = gubn.GetAt(0);
	else
		m_Gubn = ' ';

	const int	clrBk = GetIndexColor(CLRBG);
	CString	sRoot = Variant(homeCC, ""); 
	m_pAccount = std::make_unique<CAccountCtrl>(m_pView, this, sRoot);
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, rc, this, 0);
	m_pAccount->createAccountCtrl("AN10", TR_DLGACCOUNT, clrBk);
	
	AddColumn("종목명", 0, 88);
	AddColumn("단가", 1, 35);
	AddColumn("수량", 2, 35);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CJangoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

// 계좌control
long CJangoDlg::OnAccount(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case AM_ACNTCHANGE:	// CAccountCtrl 계좌변경시 수신 Message
		{
			CString data = (char*)lParam;

			CString accn = Parser(data, "\t");
			CString name = Parser(data, "\t");
			accn.TrimRight();
			name.TrimRight();

			m_AccnName.SetWindowText(name);
			m_sAccn = accn;
			m_sPass.Format("%s", Variant(passCCx, ""));
			//m_sPass = "0423";
			m_sPass = (LPCTSTR)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, passCC), (LPARAM)accn.operator LPCTSTR());

			sendRemainTR(accn, m_sPass);
		}
		break;
	}
	return 0;
}

CString CJangoDlg::Variant(int cmd, CString data)
{
	if( !m_pParent )
		return "";

	char* dta = (char*)m_pView->SendMessage(WM_USER, 
			MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)	return dta;
	else			return "";
}

COLORREF CJangoDlg::GetIndexColor(int index)
{
	if (index & 0x02000000)
		return index;
	return m_pView->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CString CJangoDlg::Parser(CString &srcstr, CString substr)
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
		temp.TrimLeft();	temp.TrimRight();
		srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
		return temp;
	}
	return "";
}

void CJangoDlg::sendRemainTR(CString sAccn, CString sPswd)
{
	sPswd.TrimRight();
	if (sPswd.IsEmpty())
	{
		AfxGetMainWnd()->MessageBox("계좌비밀번호 저장 후 사용하십시오", "IBK증권");
		return;
	}

	CString sData = ledgerTR(sAccn, "SONAQ052", mkty_STOCK);

	CString passX;
	passX.Format("%s\t%s", sPswd, sAccn.Left(8));
	passX = (char *)m_pView->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	sPswd = passX;

	const bool	bFuture = false;
	if (bFuture)
	{
		struct f_mid mid;
		
		FillMemory(&mid, L_fmid, ' ');
		CopyMemory(&mid.accn, sAccn, sizeof(mid.accn));
		CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
//		CopyMemory(&mid.date, "", sizeof(mid.date));
		sData += CString((char*)&mid, L_fmid);	
		sendTR("PIBOFJGO", sData, TR_DLGREMAINF, 0);
	}
	else
	{
		struct s_mid mid;
		
		FillMemory(&mid, L_smid, ' ');
		CopyMemory(&mid.accn, sAccn, sizeof(mid.accn));
		CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
		mid.sygb[0] = '0';
		mid.fees[0] = '0';

		sData += CString((char*)&mid, L_smid);
		sendTR("PIBOSJGO", sData, TR_DLGREMAINS, 0);
	}

	m_ListJango.DeleteAllItems();
	m_arJango.RemoveAll();
}

BOOL CJangoDlg::sendTR(CString sTR, CString sData, int nKey, int nStat)
{
	CString sTRData = "";
	
	struct _userTH user;
	strncpy(&user.trc[0], sTR, 8);
	user.key  = nKey;
	user.stat = nStat;
	
	sTRData  = CString((char*)&user, sizeof(_userTH));
	sTRData += sData;
	
	return m_pView->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sData.GetLength()), 
		(LPARAM)(const char*)sTRData); 
}

CString CJangoDlg::ledgerTR(CString sAccn, CString sSvcn, CString sGubn)
{
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	m_pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CString	strUser = (LPCTSTR)m_pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0);
	
	CopyMemory(&ledger.svcd, sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, sAccn.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = sGubn.GetAt(0);
	ledger.odrf[0] = odty_QUERY;

	return CString((char*)&ledger, L_ledgerH);
}

void CJangoDlg::parsingRemainData(bool bFuture, char* pData, int len)
{
	struct _ledgerH *ledger = (_ledgerH*)pData;	

	CString	sErrCode = CString((char*)ledger->emsg, 4);
	CString	sErrText = CString((char*)ledger->emsg, 94);
	if (sErrCode.GetAt(0) != _T('0'))	// 오류
		return;

	char	*pJango = pData + L_ledgerH;
	CString sAccn = CString((char*)pJango, 11);
	int nrec = 0;
	int sizeL = 0;
	int checkL = 0;

	if (sAccn.IsEmpty())	return;

	int	idx = 0;
	_Jango	Jango;
	m_arJango.RemoveAll();
	if (bFuture)
	{
		struct f_mod	fmod;
		CopyMemory(&fmod, pJango, L_fmod);
		nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
		pJango += L_fmod;
		sizeL += L_fmod;	
		checkL = L_fmod + L_frec * nrec;

		for (int ii = 0; ii < nrec; ii++)
		{
			struct f_rec *frec = (struct f_rec *)pJango;

			Jango.code = CString(frec->cod2, sizeof(frec->cod2));	// code
			Jango.name = CString(frec->hnam, sizeof(frec->hnam));
			Jango.name.TrimRight();
			Jango.num = CString(frec->cqty, sizeof(frec->cqty));
			Jango.num.TrimLeft();
			Jango.price = CString(frec->pprc, sizeof(frec->pprc));
			Jango.price.TrimLeft();

			AddItem(Jango.name, idx, 0);
			AddItem(editComma(Jango.price), idx, 1);
			AddItem(editComma(Jango.num), idx, 2);
			idx++;
			m_arJango.Add(Jango);

			pJango += L_frec;	sizeL += L_frec;
			if (sizeL > len || sizeL > checkL)
				break;
		}
	}
	else
	{
		struct s_mod	smod;
		CopyMemory(&smod, pJango, L_smod);
		nrec = atoi(CString(smod.nrec, sizeof(smod.nrec)));
		pJango += L_smod;
		sizeL += L_smod;	
		checkL = L_smod + L_srec * nrec;

		for (int ii = 0; ii < nrec; ii++)
		{
			struct s_rec *srec = (struct s_rec *)pJango;

			Jango.code = CString(srec->cod2, sizeof(srec->cod2));	// code
			Jango.code.TrimRight();
			Jango.name = CString(srec->hnam, sizeof(srec->hnam));
			Jango.name.TrimRight();
			Jango.num = CString(srec->jqty, sizeof(srec->jqty));
			Jango.num.TrimLeft();
			Jango.price = CString(srec->pamt, sizeof(srec->pamt));
			Jango.price.TrimLeft();

			AddItem(Jango.name, idx, 0);
			AddItem(editComma(Jango.price), idx, 1);
			AddItem(editComma(Jango.num), idx, 2);
			idx++;
			m_arJango.Add(Jango);

			pJango += L_srec;	sizeL += L_srec;
			if (sizeL > len || sizeL > checkL)
				break;
		}
	}
}

void CJangoDlg::SendToAccount(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pAccount)
		m_pAccount->m_pAccountCtrl->SendMessage(msg, wParam, lParam);
}

BOOL CJangoDlg::AddColumn(LPCTSTR string, int item, int width)
{
	LV_COLUMN lvc;

	lvc.mask    = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT;
	lvc.fmt     = LVCFMT_RIGHT;
	lvc.pszText = (LPTSTR) string;
	lvc.cx      = m_ListJango.GetStringWidth(lvc.pszText) + width;

	return m_ListJango.InsertColumn(item, &lvc);
}

int CJangoDlg::AddItem(LPCSTR string, int nItem, int nSubItem)
{
	if (nSubItem <= 0) // name
		return m_ListJango.InsertItem(LVIF_TEXT, nItem, string, NULL, NULL, 0, 0);
	else
		return m_ListJango.SetItemText(nItem, nSubItem, string);
}

CString CJangoDlg::editComma(CString sVal)
{
	CString tmpS = _T("");

	CString dotRight;
	
	CString tmp;
	tmp = sVal;

	if (atof(tmp) == 0)	return tmp;

	const int find = tmp.Find('.');
	if (find > 0)  
	{
		dotRight = tmp.Right(tmp.GetLength() - (find));
		tmp = tmp.Left(find);
	}
	else 
	{
		dotRight.Empty();
	}

	int jj = 1;
	for (int ii = tmp.GetLength() - 1; ii >= 0 ;ii--, jj++)
	{
		tmpS.Insert(0, tmp.GetAt(ii));
		if (jj % 3 == 0 && ii > 0)
			tmpS.Insert(0, ",");
	}

	tmpS += dotRight;
	return tmpS;
}
