// MapForm.cpp : implementation file
//

#include "stdafx.h"
#include "IB0000A1.h"
#include "MapForm.h"
#include "accout.h"

#include "../../h/ledger.h"
#include "tooltipdlg.h"

#include "libDefine.h"

#pragma comment(lib, "Imm32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	ENCTEST

const COLORREF clrGridHead		= 74;
const COLORREF clrGridLine		= 75;
const COLORREF clrGridBG		= 68;
const COLORREF clrGridHFG		= 76;
const COLORREF clrGridFG		= 69;
const COLORREF clrGridFocusBG	= 78;
const COLORREF clrGrid1stRow	= clrGridBG;
const COLORREF clrBkGnd			= RGB(238, 238, 253);


#define COL_SEQ		0
#define COL_ACCT	1
#define COL_NAME	2
#define COL_PASS	3
#define COL_NICK	4
#define COL_RESULT	5

const int fontPoint = 9;
const int gridWidth = 428;
const int GRID_MAX_ROW = 300;
const int minCount = 10;

CMapForm* form;

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		MSG*	msg = (MSG *) lParam;
		bool	back = false;
		switch (msg->message)
		{
			case WM_KEYDOWN:
				switch (msg->wParam)
				{
				case VK_TAB:
					if (::GetKeyState(VK_SHIFT) & 0x8000)
						back = true;
					if (form->SetNext(back))
						return 1L;
				case VK_RETURN:
					form->InputEnter();
					return 1L;
				}
				break;
			case WM_MOUSEMOVE:	//2012.07.10 KSJ 마우스 이동시 툴팁 추가
				form->MouseMove(msg);
				return 1L;
			default:
				break;
		}
	}
	return CallNextHookEx(form->m_hook, nCode, wParam, lParam);
}

BOOL CALLBACK ProcChildWnd(HWND hWnd, LPARAM lParam)
{
	CArray<HWND,HWND> *pArr = (CArray<HWND,HWND> *)lParam;
	pArr->Add(hWnd);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMapFormenc

IMPLEMENT_DYNCREATE(CMapForm, CAxMapForm)

CMapForm::CMapForm()
	: CAxMapForm(CMapForm::IDD)
{
	//{{AFX_DATA_INIT(CMapForm)
	//}}AFX_DATA_INIT
	m_select = -1;
	m_checkpass = false;
	m_acs.RemoveAll();

	m_hook   = NULL;
	m_cursor = 0;
	m_pToolTip = NULL;
	m_grid = NULL;
	m_pFont = NULL;

	m_main = Axis::mainWnd;
	if (!m_main)
		m_main = AfxGetMainWnd();
	ASSERT(m_main);

	for (int i = 0; i < LABEL_COUNT; i++)
		m_label[i] = NULL;

	m_bkBrush.CreateSolidBrush(clrBkGnd);

	m_staff = FALSE;
	m_bSaveAllPasswdMode = FALSE;

	m_pTip = NULL;

	m_bFailed = FALSE;
}

CMapForm::~CMapForm()
{
	if (m_pFont)
		delete m_pFont;

	if (m_hook)	UnhookWindowsHookEx(m_hook);
	for (int ii = 0; ii < m_acs.GetSize(); ii++)
	{
		delete m_acs.GetAt(ii);
	}
	m_acs.RemoveAll();
}

void CMapForm::DoDataExchange(CDataExchange* pDX)
{
	CAxMapForm::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMapForm)
	DDX_Control(pDX, IDC_CHANGEALL, m_btnChangeAll);
	DDX_Control(pDX, IDC_CHANGE, m_btnChange);
	DDX_Control(pDX, IDC_USEALIAS, m_useAlias);
	DDX_Control(pDX, IDOK, m_btOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMapForm, CAxMapForm)
	//{{AFX_MSG_MAP(CMapForm)
	ON_BN_CLICKED(IDC_CHANGE, OnChangePass)
	ON_BN_CLICKED(IDC_DELPASS, OnDelpass)
	ON_BN_CLICKED(IDC_SAVEACCOUNT, OnSaveaccount)
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_USEALIAS, OnUsealias)
	ON_CBN_SELCHANGE(IDC_GROUP, OnSelchangeGroup)
	ON_BN_CLICKED(IDC_CHANGEALL, OnAllsavesend)
	ON_BN_CLICKED(IDC_GROUP_CONFIG, OnGroupConfig)
	ON_BN_CLICKED(IDOK, OnOK)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_CHECK_LOGIN, OnCheckLogin)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnCtrl)
	ON_NOTIFY(GVNM_LMOUSEDOWN, IDC_XXGRID, OnGridClick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapForm diagnostics

#ifdef _DEBUG
void CMapForm::AssertValid() const
{
	CAxMapForm::AssertValid();
}

void CMapForm::Dump(CDumpContext& dc) const
{
	CAxMapForm::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMapForm message handlers

BOOL CMapForm::CreateMap(CWnd* parent, CWnd* view)
{
	m_pParent = view;
	
	if (!Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), parent, (int) this, NULL))
		return FALSE;

	form = this;
	m_hook = SetWindowsHookEx(WH_GETMESSAGE, KeyboardProc, 
			AfxGetInstanceHandle(), GetCurrentThreadId());

	m_pFont = new CFont;
	m_pFont->CreatePointFont(90, "굴림");
	//struct _fontR fontR;
	//fontR.name = "굴림";
	//fontR.point = 9;
	//fontR.italic = false;
	//fontR.bold = FW_NORMAL;
	//m_pFont = (CFont*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(getFONT, 0), (LPARAM)&fontR);

	UINT id[LABEL_COUNT] = { IDC_STATUS, IDC_LABEL1, IDC_LABEL2, IDC_LABEL3, IDC_LABEL4, IDC_LABEL5};
	
	for (int i = 0; i < LABEL_COUNT; i++)
	{
		CWnd* wnd = GetDlgItem(id[i]);

		if (wnd)
		{
			CRect rc;
			CString caption;
			DWORD style;
			wnd->GetWindowRect(&rc);
			wnd->GetWindowText(caption);
			style = ::GetWindowLong(wnd->m_hWnd, GWL_STYLE);
			//caption.Replace(" ", "");
			ScreenToClient(&rc);

			m_label[i] = new CNeoStatic;
			m_label[i]->Create(caption, style, rc, this, (id[i] == IDC_STATUS) ? IDC_STATUS1: IDC_STATIC);
			m_label[i]->SetFont(m_pFont);
			m_label[i]->setColor(clrBkGnd, GetIndexColor(clrGridFG), clrBkGnd);
			m_label[i]->SetWindowText(caption);
			wnd->PostMessage(WM_CLOSE);
		}
	}

	Init();
	LoadConfig();
	OnUsealias();

// 	CWnd *pWnd = GetDlgItem(IDC_ALIAS);
// 	if (pWnd->IsWindowEnabled()) pWnd->SetFocus();
// 	else GetDlgItem(IDC_PASS)->SetFocus();

	return TRUE;
}

void CMapForm::Init()
{
	CRect rc;
	GetDlgItem(IDC_GRID)->GetWindowRect(rc);
	ScreenToClient(rc);

	int width = GetSystemMetrics(SM_CXVSCROLL);

	for (int i = 0; i < ArraySize(grInfo); i++)
	{
		if (!(grInfo[i].attr & GVAT_HIDDEN))
		{
			width += grInfo[i].width;
		}
	}

	rc.right = rc.left + width;

	m_grid = new CXXGrid();
	m_grid->Create(rc, this, IDC_XXGRID, GVSC_VERT, NULL, WS_VISIBLE | WS_CHILD);
	InitGrid();

	fit();

	Variant(titleCC, "계좌비밀번호저장");
	m_home = Variant(homeCC, "");
	m_user = Variant(userCC, "");
	m_name = Variant(nameCC, "");

	m_staff = FALSE;
	if (m_user.GetLength()==6)
	{
		m_staff = TRUE;
		for(int n=0; n<6; ++n)
		{
			TCHAR ch = m_user.GetAt(n);
			if ('0'>ch || ch>'9')
			{
				m_staff = FALSE;
				break;
			}
		}		
	}

	((CEdit*)GetDlgItem(IDC_ALIAS))->SetLimitText(30);
	((CEdit*)GetDlgItem(IDC_PASS))->SetLimitText(8);

	// 직원일 경우에만 그룹관련 Control을 보여준다.
	GetDlgItem(IDC_GP_STATIC)->ShowWindow(m_staff ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_GROUP)->ShowWindow(m_staff ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_GROUP_CONFIG)->ShowWindow(m_staff ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_CHANGEALL)->ShowWindow(m_staff ? SW_HIDE : SW_SHOW);	
	GetDlgItem(IDC_STATIC_SP)->SetWindowText(
		m_staff ? 
		"- 특정그룹별로 비밀번호 저장이 가능합니다. 그룹등록은 상단의 설정버튼을 클릭하여 주십시오" :
		"- 전계좌가 동일한 비밀번호 사용시 비밀번호 일괄저장 기능을 쉽게 이     용할 수 있읍니다."
	);
	
	// 고객일경우 관리그룹관련 Control을 숨기고, 다른 모든 컨트롤을 올린다.
	if (!m_staff)
	{
		CString emsg;
		CArray<HWND,HWND> hArr;
		CRect rc1, rc2, rc3;
		POINT pt;
		GetDlgItem(IDC_GP_STATIC)->GetWindowRect(rc1);
		GetDlgItem(IDC_GRID)->GetWindowRect(rc2);
		if (EnumChildWindows(m_hWnd, ProcChildWnd, (LPARAM)&hArr))
		{
			for(int n=0; n<hArr.GetSize(); ++n)
			{
				::GetWindowRect(hArr[n], &rc3);
				pt.x = rc3.left;
				pt.y = rc3.top;
				ScreenToClient(&pt);

				rc3.SetRect(pt.x, pt.y, pt.x+rc3.Width(), pt.y+rc3.Height());
				rc3.OffsetRect(0, -20);

				::MoveWindow(hArr[n], rc3.left, rc3.top, rc3.Width(), rc3.Height(), TRUE);
			}
		}
	}

	CString file;

	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");

	int nChk = GetPrivateProfileInt("SCREEN", "POPUPACC", 1, file);
	/*
	CFont* font = GetDlgItem(IDC_CHECK_LOGIN)->GetFont();

	LOGFONT logFont;
	font->GetLogFont(&logFont);
	logFont.lfWeight = FW_BOLD;

	m_font.CreateFontIndirect(&logFont);
	GetDlgItem(IDC_CHECK_LOGIN)->SetFont(&m_font);
	*/

	((CButton*)GetDlgItem(IDC_CHECK_LOGIN))->SetCheck(nChk);
	
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CTooltipDlg;
		m_pToolTip->Create(CTooltipDlg::IDD, NULL);
	}

	if (m_pTip == NULL)
	{
		m_pTip = new CToolTipCtrl;			
		m_pTip->Create(this, TTS_ALWAYSTIP|TTS_NOPREFIX);
	}

	LoadAccountListFromWizard();
}

void CMapForm::sendTR(CString trC, char* pBytes, int nBytes, char key)
{
	if (!m_pParent)	return;

	int	len = L_userTH + nBytes;
	char*	sndB = new char[len+1];
	struct _userTH* udat = (struct _userTH*) sndB;

	ZeroMemory(sndB, sizeof(sndB));

	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = key;	
	udat->stat = US_ENC;
	
	CopyMemory(&sndB[L_userTH], pBytes, nBytes);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes), (LPARAM)sndB);
	delete [] sndB;
}

CString CMapForm::Variant(int comm, CString data)
{		
	CString result;
	char* dta = (char*) m_pParent->SendMessage(WM_USER,
			MAKEWPARAM(variantDLL, comm), (LPARAM)(LPCTSTR)data);
	if ((long)dta > 1)	result = dta;

	return result;
}

LRESULT CMapForm::OnUser(WPARAM wParam, LPARAM lParam)
{
	int len;
	CString emsg;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		switch (HIBYTE(LOWORD(wParam)))
		{
		case 'Q':	
			processQuery((char *) lParam);	break;
		case 'S':
			processSave((char *) lParam, HIWORD(wParam));	break;
		case 'P':
			len = HIWORD(wParam);
			if (m_bSaveAllPasswdMode)
			{
				//OutputDebugString("Start CheckAllPSWD\n");
				process_CheckAllPSWD((char*)lParam);
				emsg.Format("End   CheckAllPSWD S(%d) F(%d) A(%d)\n", m_iSuccessCnt, m_iFailedCnt, m_iSaveAllPasswdCnt);
				//OutputDebugString(emsg);
			}
			else
			{
				process_CheckPSWD((char*)lParam);	
			}
			break;
		case 'D':
			processGroup((char*)lParam, HIWORD(wParam)); break;
		}
		/*if (m_checkpass)
			process_CheckPSWD((char *) lParam);
		else
		{
			m_pParent->SendMessage(WM_USER,
				MAKEWPARAM(accountDLL, HIWORD(wParam)), lParam);
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(closeDLL, 0), 0);
		}*/
		break;
	case DLL_SETFONT:
		break;
	case DLL_DOMINO:
		break;
	case DLL_GUIDE:
		SetMsg((LPCSTR)lParam);
		return TRUE;
	}
	return 0;
}

void CMapForm::OnChangePass()
{
	SetMsg();
	
	if (!IsValidSelected(m_select))
		return;

	CString alias;
	GetDlgItemText(IDC_ALIAS, alias);
	SetAlias(m_select, alias);
	
	CString	pass, tmps;
	GetDlgItem(IDC_PASS)->GetWindowText(pass);
	pass.TrimRight();

	if (pass.IsEmpty())
	{
		SetPswd(m_select, "");

		m_grid->SetItemText(m_select, COL_PASS, "");
		m_grid->SetItemText(m_select, COL_RESULT, "");

		if (!SelectAccount(++m_select))
			return;

		GetDlgItem(IDC_PASS)->SetFocus();
		return;
	}
	
	Caccount*	acc = FindAccount(m_select);
	if (!acc) return;

	struct _userTH*	userH;
	char*		pBytes;
	int		datL;

	if (acc->m_accname.Find("*") != -1)
	{
		GetDlgItem(IDC_PASS)->GetWindowText(tmps);
		acc->m_pass = tmps;
		m_grid->SetItemText(m_select, COL_PASS, GetPasswordChar(tmps));
		SelectAccount(++m_select);
		return;
	}

#ifdef	ENCTEST
	CString passX;
	passX.Format("%s\t%s", pass, acc->m_account.Left(LEN_ACCT));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	passX.Replace("%", "%%");	// % -> %%로 해야 wizard에서 오류 안남
	pass = passX;

#endif
	datL = L_ledgerH + sizeof(struct _chkPwdMid);

	pBytes = new char[datL + L_userTH];
	FillMemory(pBytes, datL, ' ');

	userH = (struct _userTH*)&pBytes[0];
	memcpy(userH->trc, TRPSWD, strlen(TRPSWD));
	userH->key = 'P';
	userH->stat = US_ENC;

	struct _ledgerH* ledgerH = (struct _ledgerH*)&pBytes[L_userTH];
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(ledgerDLL, NULL), (LPARAM)ledgerH);
	ledgerH->fkey[0] = 'C';
	strncpy(ledgerH->ecod, "0000", 4);
	CopyMemory(ledgerH->svcd, "SACMT238", sizeof(ledgerH->svcd));
	ledgerH->mkty[0] = '3';
	ledgerH->odrf[0] = '1';
	CString tp = "1";
	CString acnt = acc->m_account;
//	2011.07.06 로그인 ID 계좌소유주 검증 테스트진행 결과 SACMT238 개별계좌로 올리도록 수정
// 	acnt.SetAt(3, '0');
// 	acnt.SetAt(4, '0');
	struct _chkPwdMid in;
	memset(&in, ' ', sizeof(in));
	strncpy(in.in, "00001", 5);
	strncpy(in.acctNo, acnt, acnt.GetLength());
	strncpy(in.password, pass, pass.GetLength());
	//strncpy(in.zTrxTp, tp, tp.GetLength());
	memcpy(&pBytes[L_userTH + L_ledgerH], &in, sizeof(in));

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) pBytes);

	delete [] pBytes;

	GetDlgItem(IDC_PASS)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGEALL)->EnableWindow(FALSE);
}

void CMapForm::OnDelpass() 
{
	/**
	if (!IsValidSelected())
		return;

	GetDlgItem(IDC_PASS)->SetWindowText("");
	addItem("", m_select, COL_ACCPASS, true);
	
	Caccount* acc = m_acs.GetAt(m_select);
	acc->m_pass.Empty();

	CString	data;
	data.Format("%s\t%s\t%s", acc->m_account, acc->m_pass, acc->m_accname);
//	m_pParent->SendMessage(WM_USER+5555, 0, (long) data.operator LPCTSTR());

  **/
}

void CMapForm::OnSaveaccount() 
{

	// 설정저장............서버 tr.........처리
	int	count = m_acs.GetSize(); 
	if (count == 0)
		return;

	int idx, n, bcnt;
	const int block_cnt = 200;
	
	bcnt = count/block_cnt;
	for(n=0; n<bcnt; ++n)
	{
		int	len = sizeof(_acSet) + block_cnt * L_acinfo;
		char *buffer = new char[len];
		memset(buffer, 0, len);
		
		struct _acSet* acSet = (struct _acSet*)buffer;
		struct _acInfo* acInfo = acSet->acinfo;
		
		acSet->func[0] = 'U';
		memcpy(acSet->usid, m_user, m_user.GetLength());

		CString name;
		int i = 0;
		for (i = 0; i < block_cnt; i++)
		{
			idx = (n*block_cnt) + i;
			Caccount *acc = m_acs.GetAt(idx);

			memcpy(acInfo[i].accn, acc->m_account, strlen(acc->m_account));
			name = EncodeAcctName(acc->m_accname, acc->m_acctAlias);
			memcpy(acInfo[i].acnm, (LPCTSTR)name, name.GetLength());
		}
		memcpy(acSet->nrec, Format("%04d", i), sizeof(acSet->nrec));
		sendTR(TRCODE, buffer, len, 'S');
		delete [] buffer;
	}

	if (count%block_cnt > 0)
	{
		int	len = sizeof(_acSet) + (count%block_cnt) * L_acinfo;
		char *buffer = new char[len];
		memset(buffer, 0, len);

		struct _acSet* acSet = (struct _acSet*)buffer;
		struct _acInfo* acInfo = acSet->acinfo;
		
		acSet->func[0] = 'U';
		memcpy(acSet->usid, m_user, m_user.GetLength());
	
		CString name;
		int i = 0;
		for (i = 0; i < (count%block_cnt); i++)
		{
			idx = (bcnt*block_cnt) + i;
			Caccount *acc = m_acs.GetAt(idx);

			memcpy(acInfo[i].accn, acc->m_account, strlen(acc->m_account));
			name = EncodeAcctName(acc->m_accname, acc->m_acctAlias);
			memcpy(acInfo[i].acnm, (LPCTSTR)name, name.GetLength());
		}
		memcpy(acSet->nrec, Format("%04d", i), sizeof(acSet->nrec));
		sendTR(TRCODE, buffer, len, 'S');
		delete [] buffer;
	}

	CString file,value;
	
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");
	
	value.Format("%d", 1);
	WritePrivateProfileString("SCREEN", "SAVEACC", value, file);
}


bool CMapForm::setAccountData(int sel)
{
	CString acct(m_grid->GetItemText(sel, COL_ACCT));
	if (acct.IsEmpty())
		return false;

// 	for(int n=0; n<m_acs.GetSize(); ++n)
// 		OutputDebugString(m_acs[n]->toString());

	CWnd *pWnd;
	if (pWnd=GetDlgItem(IDC_ANUM))  pWnd->SetWindowText(acct);
	if (pWnd=GetDlgItem(IDC_PASS))  pWnd->SetWindowText(GetPswd(sel));
	if (pWnd=GetDlgItem(IDC_ANAM)) pWnd->SetWindowText(GetName(sel));
	if (pWnd=GetDlgItem(IDC_ALIAS)) pWnd->SetWindowText(GetAlias(sel));

// 	CString emsg;
// 	emsg.Format("setData(%d) %s %s %s %s\n", sel, acct, GetPswd(sel), GetName(sel), GetAlias(sel));
// 	OutputDebugString(emsg);

	return true;
}


void CMapForm::process_CheckPSWD(char* dat)
{
	GetDlgItem(IDC_PASS)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHANGE)->EnableWindow(TRUE);
	GetDlgItem(IDC_CHANGEALL)->EnableWindow(TRUE);

	struct _ledgerH* ledgerH = (struct _ledgerH*) dat;

	CString msg(ledgerH->emsg, sizeof(ledgerH->emsg));
	msg.TrimRight();

	SetMsg(msg);

	struct _chkPwdMod* mod = (struct _chkPwdMod*)&dat[L_ledgerH];
	CString ecod(ledgerH->ecod, sizeof(ledgerH->ecod));
	CString valYn = "";
	
	valYn = mod->zAvalYn;

	//if (ecod.GetLength() && ecod[0] == '0')
	if ((!valYn.IsEmpty())&&(valYn == "Y"))
	{
		CString	data;

		CString strAcno(mod->in.acctNo, sizeof(mod->in.acctNo));
		strAcno.TrimRight();
	
		if (IsValidSelected(m_select))
		{
			Caccount*	acc = FindAccount(m_select);
			if (!acc) return;

			GetDlgItem(IDC_PASS)->GetWindowText(data);
			acc->m_pass = data;
			m_grid->SetItemText(m_select, COL_PASS, GetPasswordChar(data));
			m_grid->SetItemFgColor(m_select, COL_RESULT, RGB(0,0,0));
			m_grid->SetItemText(m_select, COL_RESULT, "정상");
	//save password
			data.Format("%s\t%s\t%s", acc->m_account, acc->m_pass, acc->m_accname);
	//		m_pParent->SendMessage(WM_USER+5555, 0, (long) data.operator LPCTSTR());
			
			SelectAccount(++m_select);
		}
		else
		{
			TRACE("select error\n");
		}
	}
	else
	{
		m_grid->SetItemFgColor(m_select, COL_RESULT, RGB(255,0,0));
		m_grid->SetItemText(m_select, COL_RESULT, "오류");
		GetDlgItem(IDC_PASS)->SetWindowText("");
		GetDlgItem(IDC_PASS)->SetFocus();
	}
}

//2012.07.10 KSJ 마우스 이동시 툴팁
void CMapForm::MouseMove(MSG* msg)
{
	CRect rect, rectAll;
	CPoint pt = CPoint(msg->pt);

	ScreenToClient(&pt);
	GetDlgItem(IDC_CHANGE)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDC_CHANGEALL)->GetWindowRect(rectAll);
	ScreenToClient(rectAll);

	if(pt.x > rect.left && pt.x < rect.right && pt.y > rect.top && pt.y < rect.bottom)
	{
		m_pTip->Activate(TRUE);
		m_pTip->AddTool(GetDlgItem(IDC_CHANGE), "선택한 계좌의 비밀번호와 별칭 저장");
		m_pTip->RelayEvent(msg);
	}
	else if(pt.x > rectAll.left && pt.x < rectAll.right && pt.y > rectAll.top && pt.y < rectAll.bottom)
	{
		m_pTip->Activate(TRUE);
		m_pTip->AddTool(GetDlgItem(IDC_CHANGEALL), "전계좌 동일한 비밀번호 사용시 비밀번호 일괄저장");
		m_pTip->RelayEvent(msg);
	}
}
//KSJ

void CMapForm::InputEnter()
{
	CWnd	*wnd, *focusWin = GetFocus();
	int	nID[] = { IDC_UP, IDC_DOWN, IDC_PASS, IDC_ALIAS, IDC_CHANGE, IDC_DELPASS, IDC_SAVEACCOUNT, IDOK, IDCANCEL };
	
	for (int ii = 0; ii < ArraySize(nID); ii++)
	{
		wnd = GetWnd(nID[ii]);
		if (wnd != focusWin)
			continue;

		switch (nID[ii])
		{
		case IDC_PASS:			OnChangePass();	break;
		case IDC_ALIAS:			SetNext(false); SetNext(false); break;
		case IDC_CHANGE:		OnChangePass();	break;
		case IDC_DELPASS:		OnDelpass();	break;
		case IDC_SAVEACCOUNT:	OnSaveaccount();break;
		case IDOK:				OnOK();		break;
		case IDCANCEL:			OnCancel();	break;
		}
		break;
	}
}

CWnd* CMapForm::GetWnd(UINT nID)
{
	return GetDlgItem(nID);
}

bool CMapForm::SetNext(bool back)
{
	CWnd	*focus = GetFocus();

	if (back)	
		PostMessage(WM_NEXTDLGCTL, 1, 0);
	else	PostMessage(WM_NEXTDLGCTL, 0, 0);
	return true;
}

void CMapForm::queryACC()
{
	m_cursor = 1;

	SetMsg("계좌정보 조회 중입니다.");
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

	char	sndB[256];
	
	memset(sndB, ' ', sizeof(sndB));
	struct _acSet*	acSet = (struct _acSet*) sndB;
	acSet->func[0] = 'Q';
#ifdef _DEBUG
//	m_user = "imahun";
#endif
	CopyMemory(acSet->usid, m_user, m_user.GetLength());
	sendTR(TRCODE, sndB, sizeof(struct _acSet), 'Q');
}

void CMapForm::processQuery(char* pBytes)
{
	struct _acSet*	acSet  = (struct _acSet*) pBytes;
	struct _acInfo*	acInfo = acSet->acinfo;
	
	CString	eMsg = CString(acSet->emsg, sizeof(acSet->emsg));
	eMsg.TrimRight();

	int	acCnt = atoi(CString(acSet->nrec, sizeof(acSet->nrec)));

	if (acCnt < 1)
	{
		LoadAccountListFromWizard();
		return;
	}

	if (acCnt < minCount)
		m_grid->SetRowCount(minCount);
	else
		m_grid->SetRowCount(acCnt + 1);

	GridRowInit(m_grid->GetRowCount());

	CString tmp, name, alias, account;
	int row;

	CString slog;
	
	for (int ii = 0; ii < acCnt; ii++)
	{
		account = CString(acInfo[ii].accn, sizeof(acInfo[ii].accn));
		account.TrimRight();
		if (account.IsEmpty())
			break;

		if (account.GetLength()>5)
		{
			CString mkgb = account.Mid(3, 2);
			if (mkgb=="25")	// 해외선물 계좌
				continue;
			if (mkgb=="27") // FX마진 계좌
				continue;
		}

		tmp = CString(acInfo[ii].acnm, sizeof(acInfo[ii].acnm));
		tmp.TrimRight();
		if (tmp.IsEmpty())
			break;

		DecodeAcctName(tmp, name, alias);
		
		Caccount* acc = new Caccount;
		acc->m_accname = name;
		acc->m_account = account;
		acc->m_acctAlias = alias;

		row = ii + 1;
		tmp.Format("%d ", row);
		
		m_grid->SetItemText(row, COL_SEQ, tmp);
		m_grid->SetItemText(row, COL_ACCT, EncodeAcctNo(acc->m_account));
		m_grid->SetItemText(row, COL_NAME, acc->m_accname);
		m_grid->SetItemText(row, COL_NICK, acc->m_acctAlias);

slog.Format("[0000A1] processQuery [%s][%s][%s][%s][%s]", acc->m_account, acc->m_accname, acc->m_acctAlias, acc->m_pass, acc->m_own);
OutputDebugString(slog);

		m_acs.Add(acc);
	}

	loadPass();
	m_cursor = 0;
	SetMsg("조회가 완료되었습니다.");
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));

	if (m_acs.GetSize()) {
		m_select = 1;
		SelectAccount(m_select);
	}
}

void CMapForm::processSave(char* pBytes, int nBytes)
{
	Caccount* acc;
	struct _acSet* acSet = (struct _acSet*) pBytes;
	CString	msg = CString(acSet->emsg, sizeof(acSet->emsg));
	msg.TrimRight();
	if (acSet->errc[0] == '0')
	{
		CString	dats;

		pBytes += sizeof(struct _acSet) - L_acinfo;
		nBytes -= sizeof(struct _acSet) - L_acinfo;
		// for save account setting
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(accountDLL, nBytes), (long) pBytes);
		if (msg.IsEmpty())	msg = _T("정상처리 되었습니다.");

		// for resave account
		for (int ii = 0; ii < m_acs.GetSize(); ii++)
		{
			acc = m_acs.GetAt(ii);
			/**
			if (acc->m_use == "01")
				continue;
			**/

	//		dats.Format("%s\t%s\t%s", acc->m_account, acc->m_pass, acc->m_accname);
			dats.Format("%s\t%s\t%s|%s|%s", acc->m_account, acc->m_pass, acc->m_accname, acc->m_acctAlias, acc->m_own);
			ChangePasswordNotifyToMain(dats);
		}
	}
	SetMsg(msg);
}

void CMapForm::loadPass(bool bQuery)
{
	if (bQuery)
	{
		CString	accS, pswdS, tmps, dat = Variant(accountCC, "");
		CMapStringToString	ary;
		
		while (!dat.IsEmpty())
		{
			int	pos = dat.Find('\n');
			if (pos == -1)	break;
			tmps = dat.Left(pos++);
			dat = dat.Mid(pos);
			
			pos = tmps.Find('\t');
			if (pos == -1)	break;
			accS = tmps.Left(pos++);
			tmps = tmps.Mid(pos);
			
			pos = tmps.Find('\t');
			if (pos == -1)	break;
			pswdS = tmps.Left(pos++);
			tmps = tmps.Mid(pos);
			
			ary.SetAt(accS, pswdS);
		}
		
		for (int ii = 0; ii < m_acs.GetSize(); ii++)
		{
			Caccount*	acc = m_acs.GetAt(ii);
			ary.Lookup(acc->m_account, acc->m_pass);
			if (!acc->m_pass.IsEmpty())
				m_grid->SetItemText(ii + 1, COL_PASS, GetPasswordChar(acc->m_pass));
		}
		
		ary.RemoveAll();
	}
	else
	{
		for (int ii = 0; ii < m_acs.GetSize(); ii++)
		{
			Caccount*	acc = m_acs.GetAt(ii);
			m_grid->SetItemText(ii + 1, COL_PASS, GetPasswordChar(acc->m_pass));
		}
	}
}

BOOL CMapForm::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_cursor == 1)
	{
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));
		return TRUE;	
	}
	return CAxMapForm::OnSetCursor(pWnd, nHitTest, message);
}

void CMapForm::CloseDll()
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(closeDLL, 0));
}

void CMapForm::OnOK() 
{
	SaveConfig();
	
	Caccount *acc = NULL;
	CString dats;
	for (int ii = 0; ii < m_acs.GetSize(); ii++)
	{
		acc = m_acs.GetAt(ii);
		
		dats.Format("%s\t%s\t%s|%s|%s", acc->m_account, acc->m_pass, acc->m_accname, acc->m_acctAlias, acc->m_own);
// 		OutputDebugString(dats);
// 		OutputDebugString("\n");
		ChangePasswordNotifyToMain(dats);
	}

	OnSaveaccount();

	CString file,value;
	
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");
	
	int nChk = 0;

	if (((CButton *) GetDlgItem(IDC_CHECK_LOGIN))->GetCheck() == BST_CHECKED)	nChk = 1;
		else	nChk = 0;
	
	value.Format("%d", nChk);
	WritePrivateProfileString("SCREEN", "POPUPACC", value, file);

	CloseDll();
}

void CMapForm::OnCancel() 
{
	CloseDll();
}

CString CMapForm::GetPswd(int sel)
{
	if (!IsValidSelected(sel))
		return "";

	Caccount *pAccn = FindAccount(sel);
	if (pAccn)	return pAccn->m_pass;
	else		return "";
}

void CMapForm::SetPswd(int sel, CString pswd)
{
	if (!IsValidSelected(sel))
		return;

	Caccount *pAccn = FindAccount(sel);
	if (pAccn) pAccn->m_pass = pswd;
}

void CMapForm::SetPswd(CString acc, CString pswd)
{
	Caccount *pAccn = FindAccount( (LPCSTR)acc );
	if (pAccn) pAccn->m_pass = pswd;
}

/** file load&save removed by macho
void CMapForm::OnLoadfile() 
{
	CString sAccntNum, sPassword;
	CString sUserDir;
	
	sUserDir.Format("%s\\user\\%s\\ACCOUNT.AIN", m_home, Variant(nameCC, ""));
	
	CFileDialog fDlg(TRUE, "AIN", sUserDir, OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT, "AIN Files (*.AIN)|*.AIN||");
	fDlg.m_ofn.lpstrInitialDir = NULL;
	
	if (fDlg.DoModal() != IDOK)
		return;

	CFile file(fDlg.GetPathName(), CFile::modeRead);
	CArchive archive(&file, CArchive::load);
	
	do
	{
		if (!archive.Read(sAccntNum.GetBuffer(LEN_ACCT), LEN_ACCT))
			break;
		
		if (!archive.Read(sPassword.GetBuffer(10), 10))
			break;
		
		EnDecrypt((LPSTR)(LPCTSTR)sPassword, 10);
		
		sAccntNum.ReleaseBuffer();
		sPassword.ReleaseBuffer();
		SetPswd(sAccntNum, sPassword);
	} while (!sAccntNum.IsEmpty());
	
	archive.Close();

	loadPass(false);
	m_select = 0;
	setAccountData(m_select);
}

void CMapForm::OnSavefile()
{
	CMapStringToString AccMap;
	
	CFileDialog fDlg(FALSE, "AIN", Format("%s\\user\\%s\\ACCOUNT.AIN", m_home, Variant(nameCC, "")), 
		OFN_OVERWRITEPROMPT, "AIN Files (*.AIN)|*.AIN||");
	fDlg.m_ofn.lpstrInitialDir = NULL;
	
	if (fDlg.DoModal() != IDOK)
		return;
	
	for (int ii = 0; ii < m_acs.GetSize(); ii++)
	{
		Caccount *pAcc = m_acs.GetAt(ii);
		if (!pAcc->m_pass.IsEmpty())
			AccMap.SetAt(pAcc->m_account, pAcc->m_pass);
	}
	
	TRY
	{
		CFile file(fDlg.GetPathName(), CFile::modeWrite | CFile::modeCreate);
		CArchive archive(&file, CArchive::store);
		
		POSITION pos = AccMap.GetStartPosition();
		
		while (pos)
		{
			CString sAcc, sPass;
			char sPassword[10];
			AccMap.GetNextAssoc(pos, sAcc, sPass);
			
			ZeroMemory(sPassword, 10);
			strcpy(sPassword, sPass);
			
			archive.Write(sAcc, 10);
			EnDecrypt(sPassword, 10);
			archive.Write(sPassword, 10);
		}
		archive.Close();
	}
	CATCH(CFileException, e )
	{
		e->Delete();
	}
	END_CATCH
		AccMap.RemoveAll();
}

void CMapForm::EnDecrypt(char *pass, int nCnt)
{
	for (int ii = 0; ii < nCnt; ii++)
	{
		switch (ii % 5)
		{
		case 0:  pass[ii] ^= 0x57; break;
		case 1:  pass[ii] ^= 0x49; break;
		case 2:  pass[ii] ^= 0x4E; break;
		case 3:  pass[ii] ^= 0x49; break;
		case 4:  pass[ii] ^= 0x58; break;
		}
	}
}
**/

#include <Imm.h>
void CMapForm::ImeSet(bool bOn) 
{
    // IME 저장 폴더
    static HIMC hImc = NULL;
    if (bOn)
    {
        // IME 해제
        hImc = ImmAssociateContext(GetDlgItem(IDC_PASS)->GetSafeHwnd(), NULL);
    }
    else
    {
        // IME 설정
        hImc = ImmAssociateContext(GetDlgItem(IDC_PASS)->GetSafeHwnd(), hImc);
    }
}

void CMapForm::OnDestroy() 
{
	FreeGroupData();
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_GROUP);
	if (pCombo)
	{
		CString strGroup, iniConf = m_home + "\\user\\" + m_name + "\\userconf.ini";
		pCombo->GetWindowText(strGroup);
		WritePrivateProfileString("IB0000A1", "Group", (LPCSTR)strGroup, (LPCSTR)iniConf);
	}

	CAxMapForm::OnDestroy();
	
	if (m_pToolTip)
	{
		m_pToolTip->DestroyWindow();
		delete m_pToolTip;
	}

	if (m_pTip)
	{
		delete m_pTip; 
		m_pTip = NULL;
	}
	

	if (m_grid)
	{
		m_grid->DestroyWindow();
		delete m_grid;
	}

	for (int i = 0; i < LABEL_COUNT; i++)
	{
		if (m_label[i])
		{
			m_label[i]->DestroyWindow();
			delete m_label[i];
		}
	}
}


LONG CMapForm::OnCtrl(WPARAM wParam, LPARAM lParam)
{
//	OutputDebugString("[KSJ]OnCtrl");
	LONG	ret = 0;

	switch (LOWORD(wParam))
	{
	case IDOK:
		{
			switch (HIWORD(wParam))
			{

			case 0:
				if (m_pToolTip)
					m_pToolTip->ShowWindow(SW_HIDE);
				break;
			case 1:
				if (m_pToolTip && !m_pToolTip->IsWindowVisible())
				{
					CRect	rect;
					CRect	rcTip;

					m_btOK.GetWindowRect(rect);					
					m_pToolTip->GetWindowRect(rcTip);
					m_pToolTip->SetWindowPos(NULL, rect.CenterPoint().x, rect.bottom + 2, rcTip.Width(), rcTip.Height(), SWP_SHOWWINDOW|SWP_NOACTIVATE);		
				}
				break;
			}
		}
		break;
	}

	return ret;
}

void CMapForm::SetMsg(const char* lpszMsg, COLORREF fgColor)
{
	ASSERT(m_label[0]);
	m_label[0]->setColor(m_label[0]->m_bkClr, fgColor, m_label[0]->m_bxClr);
	m_label[0]->SetWindowText(lpszMsg);
}

void CMapForm::InitGrid()
{
	_GVITEM gvitem;
	LOGFONT lf;
	int colCount;
	COLORREF	gridHead(GetIndexColor(clrGridHead)),
				gridLine(GetIndexColor(clrGridLine));

	m_grid->SetBkColor(GetIndexColor(clrGrid1stRow));
	m_grid->SetGridColor(gridLine);
	m_grid->SetGridFocusLine(TRUE, TRUE);
	m_grid->SetFont(m_pFont);

	m_pFont->GetLogFont(&lf);
	lf.lfHeight =  fontPoint * 10;

	gvitem.row = 0;
	gvitem.mask = GVMK_TEXT | GVMK_FORMAT | GVMK_FONT | GVMK_FGCOLOR | GVMK_BKCOLOR;
	gvitem.mask |= GVMK_ATTR | GVMK_MISC | GVMK_PARAM;
	//gvitem.font = lf;
	memcpy(&gvitem.font, &lf, sizeof(LOGFONT));
	gvitem.format = GVFM_HEADER;
	gvitem.fgcol = GetIndexColor(clrGridHFG);
	gvitem.bkcol = gridHead;

	colCount = sizeof(grInfo) / sizeof(_grInfo);
	m_grid->Initial(GRID_MAX_ROW, colCount, 1, 0);

	m_grid->m_nPassCol = COL_PASS;

	gvitem.param = 10; // ??
	for (int i = 0; i < colCount; i++)
	{
		gvitem.symbol = grInfo[i].symbols;
		gvitem.text = grInfo[i].heads;
		gvitem.attr = grInfo[i].attr;
		if (!i)
		{
			gvitem.attr &= ~GVAT_CHECK;
		}

		gvitem.col = i;
		m_grid->SetItem(&gvitem);
		m_grid->SetColumnWidth(i, grInfo[i].width);

	}
	m_grid->SetRowHeight(0, GRIDROW_HEIGHT);
	m_grid->SetRowColResize(FALSE, FALSE);
	m_grid->SetBKSelColor(GetIndexColor(clrGridFocusBG));

	GridRowInit(GRID_MAX_ROW);
	m_grid->Adjust();

}


void CMapForm::fit()
{
}


void CMapForm::OnGridClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_GVITEM *pgvitem = (NM_GVITEM*)pNMHDR;
	if (pResult)	*pResult = 0;

	int row = pgvitem->row;

	if (row==0) return;

	CString no = m_grid->GetItemText(row, COL_SEQ);
	if (row > 0 && no.IsEmpty())
	{
		m_select = -1;
		Clear();
		return;
	}

	m_grid->SetSelectRow(row);
	setAccountData(m_select = row);
	SetFocusAliasOrPass();
	((CEdit*)GetDlgItem(IDC_PASS))->SetSel(0, -1);
}

BOOL CMapForm::SelectAccount(int row)
{
	if (row <= m_acs.GetSize())
	{
		m_grid->SetSelectRow(row);
		setAccountData(row);
		SetFocusAliasOrPass();
		return TRUE;
	}
	else
	{
		GetDlgItem(IDOK)->SetFocus();
		return FALSE;
	}
}

BOOL CMapForm::IsValidSelected(int sel)
{
	return (sel > 0 && sel <= m_acs.GetSize());
}

void CMapForm::Clear()
{
	SetDlgItemText(IDC_ANUM, "");
	SetDlgItemText(IDC_PASS, "");
	SetDlgItemText(IDC_ALIAS, "");
}

void CMapForm::ChangePasswordNotifyToMain(const char* data)
{
	CString slog;
	if (m_main)
	{
		slog.Format("[0000A1]ChangePasswordNotifyToMain  [%s]", data);
		OutputDebugString(slog);
		m_main->SendMessage(WM_USER + 5555, 0, (LPARAM)data);
	}
}

void CMapForm::GridRowInit(int rows)
{
	LOGFONT lf;
	m_pFont->GetLogFont(&lf);
	lf.lfHeight =  fontPoint * 10;
	int colCount = sizeof(grInfo) / sizeof(_grInfo);

	for (int i = 1; i < rows; i++)
	{
		m_grid->SetRowHeight(i, DEFAULT_ROW_HEIGHT);

		for (int j = 0; j < colCount; j++)
		{
			m_grid->SetItemFont(i, j, &lf);
			m_grid->SetItemFormat(i, j, grInfo[j].format);
		}
	}
}

void CMapForm::GotoAlias()
{
	CWnd* wnd;
	if (wnd = GetDlgItem(IDC_ALIAS))
	{
		wnd->SetFocus();
		((CEdit*)wnd)->SetSel(0, -1);
	}
}

void CMapForm::SetAlias(int sel, const char* szAlias)
{
	if (!IsValidSelected(sel))
		return;

	m_grid->SetItemText(sel, COL_NICK, szAlias);
	Caccount *pAccn = FindAccount( sel );
	if (pAccn)
		pAccn->m_acctAlias = szAlias;
}

CString CMapForm::GetAlias(int sel)
{
	if (!IsValidSelected(sel))
		return CString("");

	Caccount *pAccn = FindAccount(sel);
	if (pAccn)
		return pAccn->m_acctAlias;
	else
		return "";
}


void CMapForm::LoadAccountListFromWizard()
{
	CString str = Variant(accountCC, "1");
	CString buf, account, name, password, alias, own;
	Caccount* acc;
CString slog;
	while (TRUE)
	{
		buf = Parse(str, "\n");
		
		if (buf.IsEmpty())
			break;

		account = Parse(buf);
		if (account.IsEmpty())
			break;

		if (account.Mid(3, 2) == "00") // 종합계좌
			continue;

		if (account.Mid(3, 2) == "25") // 해외선물계좌
			continue;

		if (account.Mid(3, 2) == "27") // FX마진 계좌
			continue;

		password = Parse(buf);
		name = Parse(buf, "|");
		alias = Parse(buf, "|");
		own = Parse(buf);

// 		CString emsg;
// 		emsg.Format("acno(%s) pass(%s) name(%s) alias(%s) own(%s)\n", account, password, name,  alias, own);
// 		OutputDebugString(emsg);
		
		acc = new Caccount;
		acc->m_account = account;
		acc->m_accname = name;
		acc->m_acctAlias = alias;
		acc->m_pass = password;
		acc->m_own = own;

slog.Format("[0000A1]LoadAccountListFromWizard [%s][%s][%s][%s][%s]", acc->m_account, acc->m_accname, acc->m_acctAlias, acc->m_pass, acc->m_own);
OutputDebugString(slog);

		m_acs.Add(acc);
	}
	
	if (m_staff) 
	{
		LoadGroupInfo();
		return;
	}
	else
	{
		m_filter = "";
		FillAccountGrid();
	}
}

CString CMapForm::GetUserConfigFileName()
{
	CString home(Variant(homeCC, "")), user(Variant(nameCC, ""));
	
	NormalizeFolderName(home); 

	home += "user\\";
	home += user;

	if (!IsFileExist(home))
	{ 
		if (!CreateDirectory(home, NULL))
			return CString("");
	}

	return home + "\\" + user + ".ini";
}

const char* account = "Account";
const char* useAlias = "UseAlias";

void CMapForm::LoadConfig()
{
	CProfile profile(GetUserConfigFileName());
	int bCheck = profile.GetInt(account, useAlias);
	GetDlgItem(IDC_USEALIAS)->SendMessage(BM_SETCHECK, bCheck);
	GetDlgItem(IDC_ALIAS)->EnableWindow(bCheck);
}

void CMapForm::SaveConfig()
{
	CProfile profile(GetUserConfigFileName());	
	profile.Write(account, useAlias, GetDlgItem(IDC_USEALIAS)->SendMessage(BM_GETCHECK));
}

HBRUSH CMapForm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CAxMapForm::OnCtlColor(pDC, pWnd, nCtlColor);
	
	int nId = pWnd->GetDlgCtrlID();
	if (nId == IDC_STATIC || nId == IDC_GP_STATIC || nId == IDC_STATIC_SP || nId == IDC_STATIC_SP2 || nId == IDC_STATIC_SP3 || nId == IDC_CHECK_LOGIN)
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetBkColor(clrBkGnd);
	}

	if (pWnd->m_hWnd == m_useAlias.m_hWnd || nId == IDC_STATIC || nId == IDC_GP_STATIC || nId == IDC_STATIC_SP  || nId == IDC_STATIC_SP2 || nId == IDC_STATIC_SP3 || nId == IDC_CHECK_LOGIN)
	{
		return (HBRUSH)m_bkBrush.m_hObject;
	}
	return hbr;
}

void CMapForm::OnSize(UINT nType, int cx, int cy) 
{
	CAxMapForm::OnSize(nType, cx, cy);
}

void CMapForm::OnUsealias() 
{
	GetDlgItem(IDC_ALIAS)->EnableWindow( ((CButton*)GetDlgItem(IDC_USEALIAS))->GetCheck() );
}

void CMapForm::LoadGroupInfo()
{
	if (!m_staff) return;

	struct pidoseta_mid mid;

	FillMemory(&mid, sizeof(mid), 0);
	mid.xflg[0] = '1';
	memcpy(mid.usid, (LPCSTR)m_user, m_user.GetLength());
	mid.sflg[0] = '3';

	sendTR("pidoseta", (char*)&mid, sizeof(mid), 'D');

}

void CMapForm::processGroup( char *data, int dlen )
{
	struct pidoseta_mod *mod = (struct pidoseta_mod*)data;
	if (mod->okgb[0]!='Y') return;

	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_GROUP);
	pCombo->ResetContent();
	pCombo->AddString("전체");
	m_filter = "전체";

	char *pos = &mod->data[0];
	int gcnt = _ttoi(CString(mod->grec, sizeof(mod->grec)));

	int n = 0;
	for(n=0; n<m_acs.GetSize(); ++n)
		m_acs.GetAt(n)->m_gpnm = "";

	FreeGroupData();

	for(n=0; n<gcnt; ++n)
	{
		struct t_grup *gp = (struct t_grup*)pos;
		int acnt = _ttoi(CString(gp->nrec, sizeof(gp->nrec)));

		CString gpnm(gp->gnam, sizeof(gp->gnam)); gpnm.TrimRight();
		if (pCombo)
			pCombo->AddString(gpnm);

		CStringArray *accs = new CStringArray();

		for(int j=0; j<acnt; ++j)
		{
			CString acno(gp->rec[j].accn, sizeof(gp->rec[j].accn));
			accs->Add(acno);
			for(int i=0, k=0; i<m_acs.GetSize(); ++i)
			{
				if (m_acs[i]->m_account==acno)
				{
					m_acs[i]->m_gpnm = gpnm;
					break;
				}
			}
		}

		m_groups.SetAt(gpnm, accs);

		pos += sizeof(struct t_grup) + ((acnt-1)*sizeof(struct t_accn));
	}

	char buff[128];
	CString iniConf = m_home + "\\user\\" + m_name + "\\userconf.ini";
	GetPrivateProfileString("IB0000A1", "Group", "전체", buff, sizeof(buff)-1, iniConf);
	int idx = pCombo->FindString(-1, buff);
	if (idx<0) 
	{
		idx = 0;
	}
	else
	{
		m_filter = buff;
	}
	pCombo->SetCurSel(idx);

	FillAccountGrid();
}

void CMapForm::FillAccountGrid()
{
	Caccount* acc;
	int row, total_row = 0;

	m_grid->SetRowCount(m_acs.GetSize() + 1);
	GridRowInit(m_grid->GetRowCount());

	CStringArray *pAccs = NULL;
	if (m_filter=="전체" || !m_groups.Lookup(m_filter, pAccs))
	{
		int idx = 0;
		for (int i=0; i < m_acs.GetSize(); i++)
		{
			acc = m_acs[i];
			row = idx + 1;
			m_grid->SetItemText(row, COL_SEQ, Format("%d", row));
			m_grid->SetItemText(row, COL_ACCT, EncodeAcctNo(acc->m_account));
			m_grid->SetItemText(row, COL_NAME, acc->m_accname);
			m_grid->SetItemText(row, COL_NICK, acc->m_acctAlias);
			m_grid->SetItemText(row, COL_PASS, GetPasswordChar(acc->m_pass));
			m_grid->SetItemText(row, COL_RESULT, "");
			++idx;
		}
		m_grid->SetRowCount(idx+1);
	}
	else
	{
		int idx = 0;
		for(int i=0; i<pAccs->GetSize(); ++i)
		{
			acc = FindAccount(pAccs->GetAt(i));
			if (acc)
			{
				row = idx + 1;
				m_grid->SetItemText(row, COL_SEQ, Format("%d", row));
				m_grid->SetItemText(row, COL_ACCT, EncodeAcctNo(acc->m_account));
				m_grid->SetItemText(row, COL_NAME, acc->m_accname);
				m_grid->SetItemText(row, COL_NICK, acc->m_acctAlias);
				m_grid->SetItemText(row, COL_PASS, GetPasswordChar(acc->m_pass));
				m_grid->SetItemText(row, COL_RESULT, "");
				++idx;
			} 
		}
		m_grid->SetRowCount(idx+1);
	}

	m_grid->ShowWindow(SW_SHOW);
	
	m_cursor = 0;
	SetMsg("조회가 완료되었습니다.");
	::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	
	if (m_acs.GetSize()) {
		m_select = 1;
		SelectAccount(m_select);
	}
}

void CMapForm::OnSelchangeGroup() 
{
	CComboBox *pCombo = (CComboBox*)GetDlgItem(IDC_GROUP);
	if (pCombo)
	{
		CString newFilter;
		pCombo->GetWindowText(newFilter);
		if (m_filter!=newFilter)
		{
			m_filter = newFilter;
			FillAccountGrid();
		}
	}
}

Caccount* CMapForm::FindAccount( LPCSTR szAcno )
{
	CString acno = szAcno;
	acno.Replace("-", "");
	acno.TrimLeft();
	acno.TrimRight();

	for(int n=0; n<m_acs.GetSize(); ++n)
		if (m_acs.GetAt(n)->m_account==acno)
			return m_acs.GetAt(n);
	return NULL;
}

Caccount* CMapForm::FindAccount( int sel )
{
	return FindAccount( (LPCSTR)m_grid->GetItemText(sel, COL_ACCT) );
}

void CMapForm::SaveAllPasswd( LPCSTR password )
{
	//OutputDebugString("SaveAllPasswd #1\n");
	EnableWindow(FALSE);
	GetDlgItem(IDC_GROUP)->EnableWindow(FALSE);
	GetDlgItem(IDC_GROUP_CONFIG)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGEALL)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHANGE)->EnableWindow(FALSE);
	GetDlgItem(IDOK)->EnableWindow(FALSE);    
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_PASS)->EnableWindow(FALSE);

	//m_iSaveAllPasswdCnt = 0;
// 	m_iSuccessCnt = 0;
// 	m_iFailedCnt = 0;
// 	m_bSaveAllPasswdMode = TRUE;

	CList<CString, LPCSTR> sendlist;

	int row = m_nAllRow;
	//for(int row=1; row<m_grid->GetRowCount(); ++row)
	{
		m_grid->SetItemText(row, COL_RESULT, "");
	}
	
	
	//OutputDebugString("SaveAllPasswd #2\n");
	//for(row=1; row<m_grid->GetRowCount(); ++row)
	{
		Caccount *acc = FindAccount(row);
		if (!acc)
		{
			m_nAllRow++;
			SaveAllPasswd(password);
			return;
		}
		
		//++m_iSaveAllPasswdCnt;

		struct _userTH*	userH;
		char*		pBytes;
		int		datL;

		datL = L_ledgerH + sizeof(struct _chkPwdMid);
		
		pBytes = new char[datL + L_userTH];
		FillMemory(pBytes, datL, ' ');
		
		userH = (struct _userTH*)&pBytes[0];
		memcpy(userH->trc, TRPSWD, strlen(TRPSWD));
		userH->key = 'P';
		userH->stat = US_ENC;
		
		//OutputDebugString("SaveAllPasswd #2-2\n");
		struct _ledgerH* ledgerH = (struct _ledgerH*)&pBytes[L_userTH];
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(ledgerDLL, NULL), (LPARAM)ledgerH);
		ledgerH->fkey[0] = 'C';
		strncpy(ledgerH->ecod, "0000", 4);
		CopyMemory(ledgerH->svcd, "SACMT238", sizeof(ledgerH->svcd));
		ledgerH->mkty[0] = '3';
		ledgerH->odrf[0] = '1';
		CString tp = "1";

		struct _chkPwdMid in;
		memset(&in, ' ', sizeof(in));
		strncpy(in.in, "00001", 5);
		strncpy(in.acctNo, acc->m_account, acc->m_account.GetLength());
		strncpy(in.password, password, strlen(password));
		//strncpy(in.zTrxTp, tp, tp.GetLength());
		memcpy(&pBytes[L_userTH + L_ledgerH], &in, sizeof(in));

		//OutputDebugString("SaveAllPasswd #2-3\n");
		
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datL), (LPARAM) pBytes);
		
		//OutputDebugString("SaveAllPasswd #2-4\n");
		delete [] pBytes;

		{
			// 이렇게 안하면 가끔 위의 SendMessage에서 맛간다 (리턴안됨)
			MSG msg;
			Invalidate(TRUE);
			while(PeekMessage(&msg, m_hWnd, 0, 0, PM_REMOVE)==TRUE)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		if(m_bFailed)
		{
			m_bFailed = FALSE;
			row = m_grid->GetRowCount();	//2014.09.05 KSJ 일괄저장시 오류나면 멈춘다.
		}
	}
	//OutputDebugString("SaveAllPasswd #3\n");
	if (m_iSaveAllPasswdCnt==0)
	{
		EnableWindow(TRUE);
	}
	else
	{
		SetMsg("조회중입니다.");
	}
}

void CMapForm::process_CheckAllPSWD( LPCSTR dat )
{
	struct _ledgerH* ledgerH = (struct _ledgerH*) dat;
	
	CString msg(ledgerH->emsg, sizeof(ledgerH->emsg));
	msg.TrimRight();
	
	SetMsg(msg);
	
	struct _chkPwdMod* mod = (struct _chkPwdMod*)&dat[L_ledgerH];
	CString ecod(ledgerH->ecod, sizeof(ledgerH->ecod));
	CString valYn = "";
	
	valYn = mod->zAvalYn;
	
	CString acno(mod->in.acctNo, sizeof(mod->in.acctNo)); acno.TrimRight();
	CString pswd(mod->in.password, sizeof(mod->in.password)); pswd.TrimRight();
	CString mask;
	mask.Format("%.*s", pswd.GetLength(), "*********");

	for(int row=1; row<m_grid->GetRowCount(); ++row)
	{
		CString acno2 = m_grid->GetItemText(row, COL_ACCT);
		acno2.Replace("-", "");
		if (acno==acno2)
		{
			Caccount *acc = FindAccount(row);
			if ( !valYn.IsEmpty() && valYn == "Y")
			{
				++m_iSuccessCnt;
				if (acc) acc->m_pass = pswd;
				m_grid->SetItemText(row, COL_PASS, mask);
				m_grid->SetItemFgColor(row, COL_RESULT, RGB(0,0,0));
				m_grid->SetItemText(row, COL_RESULT, "정상");
			}
			else
			{
				++m_iFailedCnt;
				if (acc) acc->m_pass = "";
				m_grid->SetItemText(row, COL_PASS, "");
				m_grid->SetItemFgColor(row, COL_RESULT, RGB(255,0,0));
				m_grid->SetItemText(row, COL_RESULT, "오류");
				m_bFailed = TRUE;
			}
		}
	}

	if(!m_bFailed)
	{
		m_nAllRow++;

		if(m_nAllRow < m_grid->GetRowCount())
		{
			SaveAllPasswd(pswd);
			return;
		}
	}

// 	--m_iSaveAllPasswdCnt;
// 	if (m_iSaveAllPasswdCnt==0)
	{
		m_bSaveAllPasswdMode = FALSE;
		EnableWindow(TRUE);
		GetDlgItem(IDC_GROUP)->EnableWindow(TRUE);
		GetDlgItem(IDC_GROUP_CONFIG)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHANGEALL)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHANGE)->EnableWindow(TRUE);
		GetDlgItem(IDOK)->EnableWindow(TRUE);    
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_PASS)->EnableWindow(TRUE);
		
		//m_iFailedCnt = (m_grid->GetRowCount()-1) - m_iSuccessCnt;
		
		CString emsg;
		emsg.Format("조회가 완료되었습니다. 정상[%d] 오류[%d]", m_iSuccessCnt, m_iFailedCnt);
		SetMsg(emsg);
	}
}

int CMapForm::FindRow( LPCSTR szAcno )
{
	CString acno = szAcno;
	acno.Replace("-","");
	if (acno.GetLength()!=11) return -1;

	for(int row=1; row<m_grid->GetRowCount(); ++row)
	{
		CString acno2 = m_grid->GetItemText(row, COL_ACCT);
		acno2.Replace("-", "");
		if (acno==acno2)
			return row;
	}
	return -1;
}

void CMapForm::OnAllsavesend() 
{
	CWnd *pWnd = GetDlgItem(IDC_PASS);
	if (pWnd)
	{
		CString pswd;
		pWnd->GetWindowText(pswd);
		
		pswd.TrimLeft();
		pswd.TrimRight();

		if (pswd.GetLength()<4)
		{
			MessageBox("비밀번호를 확인하십시오", "IBK투자증권");
			((CEdit*)pWnd)->SetSel(0, -1, FALSE);
			pWnd->SetFocus();
			return;
		}

		m_nAllRow = 1;
		m_iSaveAllPasswdCnt = m_grid->GetRowCount()-1;
		m_iSuccessCnt = 0;
		m_iFailedCnt = 0;
		m_bSaveAllPasswdMode = TRUE;
		SaveAllPasswd(pswd);
	}
}

void CMapForm::OnGroupConfig() 
{
	if (m_staff) SetView(typeMODAL, "IB0000A2");
	else         SetView(typeMODAL, "IB0000A4");
	LoadGroupInfo();
}

void CMapForm::FreeGroupData()
{
	CString key;
	CStringArray *val;
	POSITION pos = m_groups.GetStartPosition();
	while(pos!=NULL)
	{
		m_groups.GetNextAssoc(pos, key, val);
		delete val;
	}
	m_groups.RemoveAll();
}

CString CMapForm::GetName( int sel )
{
	if (!IsValidSelected(sel))
		return CString("");

	Caccount *pAccn = FindAccount(sel);
	if (pAccn)	return pAccn->m_accname;
	else		return "";
}

void CMapForm::SetFocusAliasOrPass()
{
	CEdit *pWnd = (CEdit*)GetDlgItem(IDC_ALIAS);
	if (!pWnd->IsWindowEnabled()) 
	{
		pWnd = (CEdit*)GetDlgItem(IDC_PASS);
	}
	pWnd->SetFocus();
	pWnd->SetSel(0, -1);
}

void CMapForm::OnCheckLogin() 
{
	// TODO: Add your control notification handler code here
	CString file,value;
	
	file.Format("%s\\%s\\%s\\%s", Axis::home, USRDIR, Axis::user, "axisensetup.ini");

	int nChk = 0;
	
	if (((CButton *) GetDlgItem(IDC_CHECK_LOGIN))->GetCheck() == BST_CHECKED)	nChk = 1;
	else	nChk = 0;
	
	value.Format("%d", nChk);
	WritePrivateProfileString("SCREEN", "POPUPACC", value, file);
}
