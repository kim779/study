#include "stdafx.h"
#include "IB871000.h"
#include "MapWnd.h"

#pragma comment(lib,"msimg32.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMapWnd::CMapWnd(CWnd *pParent)
: CAxisExt(pParent)
{
	m_edPass = nullptr;
	m_btnConf = nullptr;
	m_btnSend = nullptr;
	m_chkCode = nullptr;
	m_chkAcno = nullptr;
	m_stTMasu[0] = nullptr;
	m_stTMasu[1] = nullptr;
	m_stTMado[0] = nullptr;
	m_stTMado[1] = nullptr;
	m_stTotal[0] = nullptr;
	m_stTotal[1] = nullptr;
	m_stHist = nullptr;
	m_pHistory = nullptr;
	m_edPass = nullptr;

	m_bRelayCode = m_bRelayAcno = TRUE;

	m_home = Variant(homeCC, _T(""));
	m_user = Variant(userCC, _T(""));
	m_name = Variant(nameCC, _T(""));
	Variant(titleCC, _T("[8710] 관리그룹체결내역"));

	LoadImage();

	m_hiSha256 = NULL;
}

CMapWnd::~CMapWnd()
{
	if (m_hiSha256)
	{
		AfxFreeLibrary(m_hiSha256);
		m_hiSha256 = NULL;
	}
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnUser)
	ON_CBN_SELCHANGE(IDC_CB_GROUP, OnGroupChange)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnCellDblClick)
	ON_NOTIFY(NM_CLICK, IDC_TREE, OnCellClick)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnClickSave)
	ON_BN_CLICKED(IDC_BTN_CONF, OnClickConf)
	ON_BN_CLICKED(IDC_CHK_ACNO, OnRelayAcno)
	ON_BN_CLICKED(IDC_CHK_CODE, OnRelayCode)
	ON_BN_CLICKED(IDC_BTN_SEND, OnClickSend)
	ON_BN_CLICKED(IDC_BTN_HIST, OnClickHist)
	ON_MESSAGE(WMX_INIT_FOCUS, OnInitFocus)
	ON_MESSAGE(WMX_INIT_SIZE, OnInitSize)
END_MESSAGE_MAP()

void CMapWnd::LoadImage()
{
	char buff[1024];
	CString path, img_path;
	
	path.Format(_T("%s\\tab\\PALETTE.INI"), (LPCSTR)m_home);
	const int readL = GetPrivateProfileString("General", "Palette", "Blue", buff, sizeof(buff), path);
	if (readL==0) return;
	
	img_path.Format(_T("%s\\image\\axpanel%s1_"), (LPCSTR)m_home, buff);
	
	m_hRndBmp[0] = GetAxBitmap((LPCSTR)( img_path + _T("lt.bmp")));
	m_hRndBmp[1] = GetAxBitmap((LPCSTR)( img_path + _T("rt.bmp")));
	m_hRndBmp[2] = GetAxBitmap((LPCSTR)( img_path + _T("lb.bmp")));
	m_hRndBmp[3] = GetAxBitmap((LPCSTR)( img_path + _T("rb.bmp")));
	
	m_hBtn2[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn.bmp")));
	m_hBtn2[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_dn.bmp")));
	m_hBtn2[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\2btn_en.bmp")));
	
	m_hBtn4[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn.bmp")));
	m_hBtn4[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_dn.bmp")));
	m_hBtn4[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\4btn_en.bmp")));
	
	m_hSearch[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색.bmp")));
	m_hSearch[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색_dn.bmp")));
	m_hSearch[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\검색_en.bmp")));

	m_hSend[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\조회BTN.bmp")));
	m_hSend[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\조회BTN_DN.bmp")));
	m_hSend[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\조회BTN_EN.bmp")));

	m_hHist[0] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\ARROW.bmp")));
	m_hHist[1] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\ARROW_DN.bmp")));
	m_hHist[2] = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\ARROW_EN.bmp")));
	
	m_hGuide     = GetAxBitmap((LPCSTR)( m_home + _T("\\image\\알림_ic.bmp")));
}

void CMapWnd::DrawRoundRectangle( CDC *pDC, CRect rc )
{
	const CSize size(2,2);
	pDC->FillSolidRect((LPRECT)rc, m_crBk);
	rc.DeflateRect(size);
	pDC->FillSolidRect((LPRECT)rc, m_crBodyBk);
	DrawBitmap(pDC, rc.left,    rc.top,      5, 5, m_hRndBmp[0]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.top,      5, 5, m_hRndBmp[1]->operator HBITMAP());
	DrawBitmap(pDC, rc.left,    rc.bottom-5, 5, 5, m_hRndBmp[2]->operator HBITMAP());
	DrawBitmap(pDC, rc.right-5, rc.bottom-5, 5, 5, m_hRndBmp[3]->operator HBITMAP());
}

void CMapWnd::DrawBitmap( CDC *pDC, int x, int y, int w, int h, HBITMAP hBitmap, COLORREF tCol/*=0xFF00FF*/ )
{
	HDC hDC = ::CreateCompatibleDC(pDC->m_hDC);
	HBITMAP tBmp = (HBITMAP)::SelectObject(hDC, hBitmap);
	::TransparentBlt(pDC->m_hDC, x, y, w, h, hDC, 0, 0, w, h, tCol);
	::SelectObject(hDC, tBmp);
	::DeleteObject(hDC);
}

void CMapWnd::DrawBitmap( CDC *pDC, CRect *pRC, HBITMAP hBitmap, COLORREF tCol/*=0xFF00FF*/ )
{
	DrawBitmap(pDC, pRC->left, pRC->top, pRC->Width(), pRC->Height(), hBitmap, tCol);
}

void CMapWnd::MakeBackground()
{
	CRect tRc;
	GetClientRect(&tRc);
	
	const int width = tRc.Width(), height = tRc.Height();
	
	CBitmap *oldBmp=nullptr;
	CDC* pdc = GetDC(), mdc{};
	
	m_hBackground.DeleteObject();
	m_hBackground.CreateCompatibleBitmap(pdc, width, height);
	
	mdc.CreateCompatibleDC(pdc);
	oldBmp = mdc.SelectObject(&m_hBackground);
	
	// Drawing 
	DrawRoundRectangle(&mdc, m_rcCtrl);		// Control window
	DrawRoundRectangle(&mdc, m_rcBody);		// Body window
	DrawRoundRectangle(&mdc, m_rcGuide);	// Guide window
	
	// guide
	CRect rc(m_rcGuide);
	rc.top += (rc.Height()-14)/2;
	rc.left = PADDING_VALUE;
	rc.bottom = rc.top + 14;
	rc.right = rc.left + 14;
	DrawBitmap(&mdc, &rc, m_hGuide->operator HBITMAP(), RGB(0xDD,0xDD,0xDD));
	
	mdc.SelectObject(oldBmp);
	mdc.DeleteDC();
	
	ReleaseDC(pdc);
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this);
	Draw(&dc);
}

void CMapWnd::Draw( CDC *pdc )
{
	CDC mdc;
	CRect rc;
	
	GetClientRect(rc);
	mdc.CreateCompatibleDC(pdc);	
	CBitmap *oldBmp = mdc.SelectObject(&m_hBackground);
	pdc->BitBlt(rc.left, rc.top, rc.Width(), rc.Height(), &mdc, 0, 0, SRCCOPY);
	mdc.SelectObject(oldBmp);
	
	mdc.DeleteDC();	
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect tRc;
	GetClientRect(&tRc);
	
	m_rcCtrl.SetRect(0, 0, tRc.Width(), CONTROL_WND_HEIGHT);
	m_rcBody.SetRect(0, CONTROL_WND_HEIGHT, tRc.Width(), tRc.Height()-GUIDE_WND_HEIGHT); 
	m_rcGuide.SetRect(0, m_rcBody.bottom, tRc.Width(), tRc.bottom);

	{
		CRect rc = m_rcCtrl;
		rc.DeflateRect(PADDING_VALUE, PADDING_VALUE);
		int xpos = rc.left;
		const int ypos = rc.top, height = 18, st_margin = 4;
		
		m_stGroup.MoveWindow(xpos, ypos+st_margin, 30, height, FALSE); xpos += 30 + PADDING_VALUE;
		m_cbGroup.MoveWindow(xpos, ypos, 100, 100, FALSE); xpos += 100 + PADDING_VALUE;
    
		m_stPass.MoveWindow(xpos, ypos+st_margin, 50, height, FALSE); xpos += 50 + PADDING_VALUE;
		m_edPass->MoveWindow(xpos, ypos, 80, 20, FALSE); xpos += 80 + PADDING_VALUE;
		m_btnConf->MoveWindow(xpos, ypos, 60, 20, FALSE); xpos += 60 + PADDING_VALUE;
		m_chkCode->MoveWindow(xpos, ypos, 70, 20, FALSE); xpos += 70 + PADDING_VALUE;
		m_chkAcno->MoveWindow(xpos, ypos, 70, 20, FALSE); xpos += 70 + PADDING_VALUE;

		m_btnSend->MoveWindow(rc.right-40, ypos, 40, 20, FALSE);
	}

	{
		CRect rc = m_rcBody;
		rc.DeflateRect(PADDING_VALUE, PADDING_VALUE);

		int pos_x = rc.left;
		const int pos_y = rc.top + PADDING_VALUE / 2;
		const int line_h = 20;
		m_stTMado[0]->MoveWindow(pos_x, pos_y, 50, line_h, FALSE);	pos_x += 50 - 1;
		m_stTMado[1]->MoveWindow(pos_x, pos_y, 100, line_h, FALSE); pos_x += 100 + PADDING_VALUE/2;
		m_stTMasu[0]->MoveWindow(pos_x, pos_y, 50, line_h, FALSE);	pos_x += 50 - 1;
		m_stTMasu[1]->MoveWindow(pos_x, pos_y, 100, line_h, FALSE); pos_x += 100 + PADDING_VALUE/2;
		m_stTotal[0]->MoveWindow(pos_x, pos_y, 50, line_h, FALSE);	pos_x += 50 - 1;
		m_stTotal[1]->MoveWindow(pos_x, pos_y, 100, line_h, FALSE); pos_x += 100 + PADDING_VALUE/2;

		if (m_tree.GetSafeHwnd())
		{
			rc.top += 20 + PADDING_VALUE + PADDING_VALUE/2;
			rc.bottom -= 20 + PADDING_VALUE;
			m_tree.MoveWindow(&rc, FALSE);
		}

		rc = m_rcBody;
		rc.DeflateRect(PADDING_VALUE, PADDING_VALUE);
		m_stHist->MoveWindow(rc.left, rc.bottom-22, rc.Width()-14, 20, FALSE);
		m_btnHist->MoveWindow(rc.right-14, rc.bottom-22, 14, 20, FALSE);

	}

	MakeBackground();
}

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	LoadConfig();

	LOGFONT logf;
	m_pFont->GetLogFont(&logf);
	SetFont(m_pFont);

	CreateStatic(&m_stGroup, "그룹");
	CreateStatic(&m_stPass, "접속비번");
	
	m_cbGroup.Create(CBS_DROPDOWNLIST|WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_CB_GROUP);
	m_cbGroup.SetFont(m_pFont);

	m_edPass = std::make_unique<CfxEdit>(m_pFont);
	if(m_edPass)
	{
		m_edPass->Create(WS_VISIBLE|WS_CHILD|ES_LEFT|ES_PASSWORD, CRect(), this, IDC_ED_PASSWD);
		m_edPass->SetAlignment(alLEFT, false);
		m_edPass->LimitText(12); 
	}
	
	m_btnConf = CreateImgButton("그룹설정", IDC_BTN_CONF, m_hBtn4);
	m_btnSend = CreateImgButton("조회", IDC_BTN_SEND, m_hSend);

	m_chkCode = std::make_unique <CButton>();
	if (m_chkCode)
	{
		if (m_chkCode->Create("종목연동", BS_AUTOCHECKBOX|WS_VISIBLE|WS_CHILD, CRect(), this, IDC_CHK_CODE))
		{
			m_chkCode->SetFont(m_pFont);
			m_chkCode->SetCheck(m_bRelayCode);
		}
	}
	m_chkAcno = std::make_unique <CButton>();
	if (m_chkAcno)
	{
		if (m_chkAcno->Create("계좌연동", BS_AUTOCHECKBOX|WS_VISIBLE|WS_CHILD, CRect(), this, IDC_CHK_ACNO))
		{
			m_chkAcno->SetFont(m_pFont);
			m_chkAcno->SetCheck(m_bRelayAcno);
		}
	}
		
	m_btnSend->SetFont(m_pFontB);
		
	const COLORREF crBorder = GetIndexColor(75);
	
	m_stTMado[0] = std::make_unique <CfxStatic>(m_pFont);
	m_stTMado[0]->Create("총매도", WS_VISIBLE|WS_CHILD|SS_LEFT, CRect(0, 0, 0, 0), this);
	m_stTMado[0]->Init(ssCOLOR|ssBORDER, 0, "총매도", "", m_crMadoBk, m_crLine, m_crMinus);
	m_stTMado[0]->SetBorderColor(m_crLine);

	m_stTMado[1] = std::make_unique <CfxStatic>(m_pFont);
	m_stTMado[1]->Create("", WS_VISIBLE|WS_CHILD|SS_RIGHT, CRect(0, 0, 0, 0), this);
	m_stTMado[1]->Init(ssCOLOR|ssBORDER, 0, "", "", m_crBodyBk, m_crLine);
	m_stTMado[1]->SetBorderColor(m_crLine);
	m_stTMado[1]->SetAlignment(alRIGHT);

	m_stTMasu[0] = std::make_unique <CfxStatic>(m_pFont);
	m_stTMasu[0]->Create("총매수", WS_VISIBLE|WS_CHILD|SS_LEFT, CRect(0, 0, 0, 0), this);
	m_stTMasu[0]->Init(ssCOLOR|ssBORDER, 0, "총매수", "", m_crMasuBk, m_crLine, m_crPlus);
	m_stTMasu[0]->SetBorderColor(m_crLine);

	m_stTMasu[1] = std::make_unique <CfxStatic>(m_pFont);
	m_stTMasu[1]->Create("", WS_VISIBLE|WS_CHILD|SS_RIGHT, CRect(0, 0, 0, 0), this);
	m_stTMasu[1]->Init(ssCOLOR|ssBORDER, 0, "", "", m_crBodyBk, m_crLine);
	m_stTMasu[1]->SetBorderColor(m_crLine);
	m_stTMasu[1]->SetAlignment(alRIGHT);

	m_stTotal[0] = std::make_unique <CfxStatic>(m_pFont);
	m_stTotal[0]->Create("총약정", WS_VISIBLE|WS_CHILD|SS_LEFT, CRect(0, 0, 0, 0), this);
	m_stTotal[0]->Init(ssCOLOR|ssBORDER, 0, "총약정", "", GetIndexColor(75), m_crLine);
	m_stTotal[0]->SetBorderColor(m_crLine);

	m_stTotal[1] = std::make_unique <CfxStatic>(m_pFont);
	m_stTotal[1]->Create("", WS_VISIBLE|WS_CHILD|SS_RIGHT, CRect(0, 0, 0, 0), this);
	m_stTotal[1]->Init(ssCOLOR|ssBORDER, 0, "", "", m_crBodyBk, m_crLine);
	m_stTotal[1]->SetBorderColor(m_crLine);
	m_stTotal[1]->SetAlignment(alRIGHT);

	m_stHist = std::make_unique <CfxStatic>(m_pFont);
	m_stHist->Create("", WS_VISIBLE|WS_CHILD|SS_LEFT, CRect(0, 0, 0, 0), this);
	m_stHist->Init(ssCOLOR|ssBORDER, 0, "123123", "", m_crBodyBk, m_crLine, m_crBlack);
	m_stHist->SetBorderColor(m_crLine);
	m_stHist->SetAlignment(alLEFT);

	m_btnHist = CreateImgButton("", IDC_BTN_HIST, m_hHist);
	
	m_pHistory = std::make_unique<CHistoryDlg>(m_pParent);
	m_pHistory->Create(IDD_HISTORY);
	
	const DWORD dStyle = TVS_HASBUTTONS|TVS_FULLROWSELECT|TVS_LINESATROOT|TVS_HASLINES;
	const DWORD dExStyle	= TVS_EX_ITEMLINES|TVS_EX_ALTERNATECOLOR|TVS_EX_FULLROWMARK;
	m_tree.Create(dStyle|WS_CHILD|WS_VISIBLE, CRect(0, 0, 0, 0), this, IDC_TREE);
	m_tree.SetExtendedStyle(dExStyle);
	m_tree.SetItemHeight(18);
	InitTree();

	req_group();

	PostMessage(WMX_INIT_SIZE, 0, 0);
	PostMessage(WMX_INIT_FOCUS, 0, 0);
	return 0;
}

void CMapWnd::SafeDeleteWnd( CWnd *pWnd )
{
	if (pWnd)
	{
		if (pWnd->GetSafeHwnd()) pWnd->DestroyWindow();
		delete pWnd;
	}
}

void CMapWnd::req_group()
{
	m_cbGroup.ResetContent();
	m_groups.RemoveAll();
	m_chegdata.RemoveAll();
	m_name2codes.RemoveAll();
	m_req_acno.RemoveAll();

	struct pidoseta_mid mid;
	
	FillMemory(&mid, sizeof(mid), 0);
	mid.xflg[0] = '1';
	memcpy(mid.usid, (LPCSTR)m_user, m_user.GetLength());
	mid.sflg[0] = '3';
	
	SendTR("pidoseta", 0, (char*)&mid, sizeof(mid), TK_PIDOSETA);
}

void CMapWnd::res_group( LPCSTR data, int dlen )
{
	struct pidoseta_mod *mod = (struct pidoseta_mod*)data;
	if (mod->okgb[0]!='Y') return;

	// Wizard로부터 비밀번호 정보를 먼저  Loading
	CMap<CString, LPCSTR, CAccount, CAccount&> accounts;
	{
		LPCSTR st = Variant(accountCC, "");
		LPCSTR ed = st + strlen(st);
		
		vector<StringProxy> vLine;
		ParseString(st, ed, '\n', vLine);
		for(size_t n=0; n<vLine.size(); ++n)
		{
			vector<StringProxy> vField;
			ParseAccount(vLine[n].first, vLine[n].second, vField);

// 			CString emsg;
// 			emsg.Format("%03d - %s\n", vField.size(), vLine[n].ToCStr());
// 			OutputDebugString(emsg);
			
			// 정상적이면 5개가 나와야함. "계좌번호\t패스워드\t계좌명|닉네임|소유여부"
			if (vField.size()!=5) continue;
			
			CAccount acno(vField[0].ToCStr());
			acno.pass = vField[1].ToCStr();
			acno.acnm = vField[2].ToCStr();
			acno.alias = vField[3].ToCStr();
			acno.wrap = ( atoi(vField[4].ToCStr()) & 0x02 ) ? TRUE : FALSE;
			if (acno.wrap)
				acno.pass = "";

			const int mkgb = Str2Int(((LPCSTR)acno.acno)+3, 2);
			if (mkgb!=10 && mkgb!=11 && mkgb!=30 && mkgb!=31 && mkgb!=32) continue;
			
			accounts.SetAt(acno.acno, acno);
		}
	}

	char *pos = &mod->data[0];
	int gcnt = _ttoi(CString(mod->grec, sizeof(mod->grec)));
	
	for(int n=0; n<gcnt; ++n)
	{
		struct pidoseta_grop *gp = (struct pidoseta_grop*)pos;
		int acnt = _ttoi(CString(gp->nrec, sizeof(gp->nrec)));
		
		CString gpnm(gp->gnam, sizeof(gp->gnam)); gpnm.TrimRight();
		
		CGroup group(gpnm);
	
		for(int j=0; j<acnt; ++j)
		{
			CAccount account;
			CString acno(gp->accn[j].accn, sizeof(gp->accn[j].accn));

			if (accounts.Lookup(acno, account))
			{
				if (!account.wrap)	group.Accounts.Add(account);
			}
		}
		
		if (group.Accounts.GetSize()>0)
			m_groups.Add(group);

		pos += sizeof(struct pidoseta_grop) + ((acnt-1)*sizeof(struct pidoseta_accn));
	}

	if (m_cbGroup.GetSafeHwnd())
	{
		m_cbGroup.ResetContent();
		for(int n=0; n<m_groups.GetSize(); ++n)
		{
			m_cbGroup.AddString(m_groups[n].gpnm);
		}
		if (m_groups.GetSize()>0)
		{
			m_cbGroup.SetCurSel(0);
			OnGroupChange();
		}
	}
}

LRESULT CMapWnd::OnUser( WPARAM wParam, LPARAM lParam )
{
	const int msg = LOBYTE(LOWORD(wParam));
	const int key = HIBYTE(LOWORD(wParam));
	const int len = HIWORD(wParam);
	LPCSTR dat = (LPCSTR)lParam;
	
	switch(msg)
	{
	case DLL_INB:
		break;
	case DLL_OUB:
		switch(key)
		{
		case TK_PIDOSETA: res_group(dat, len); break;
		case TK_PIBOCHEG: res_scheg(dat, len); break;
		}
		break;
	case DLL_ALERT:
//			proc_alert(dat, strlen(dat));
		break;
	case DLL_TRIGGER:
//			proc_trigger(dat, strlen(dat));
		break;
	case DLL_NOTICE:
		proc_notice(dat, strlen(dat));
		break;
	case DLL_SETPAL:
	case DLL_SETFONT:
	case DLL_SETFONTx:
//			ChangeTheme();
		break;
	case DLL_GUIDE:
//			proc_guide(dat);
		return true;
	case DLL_DOMINO:
//			proc_domino(dat, strlen(dat));
		break;
	}
	return 0;
}

HBRUSH CMapWnd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);
	
	const int ctrlId = pWnd->GetDlgCtrlID();
	if (ctrlId==IDC_STATIC || ctrlId==IDC_CHK_CODE || ctrlId==IDC_CHK_ACNO)
	{
		if (!m_brBk.m_hObject) m_brBk.CreateSolidBrush(m_crBodyBk);

		pDC->SetBkMode(OPAQUE);
		pDC->SetBkColor(m_crBodyBk);
		return (HBRUSH)m_brBk.m_hObject;
	}
	
	return hbr;
}

void CMapWnd::OnDestroy() 
{
	SaveConfig();

	if (m_btnConf) { m_btnConf->DestroyWindow(); }//delete m_btnConf; }
	if (m_btnSend) { m_btnSend->DestroyWindow(); }//delete m_btnSend; }
	if (m_chkCode) { m_chkCode->DestroyWindow(); }//delete m_chkCode; }
	if (m_chkAcno) { m_chkAcno->DestroyWindow(); }//delete m_chkAcno; }
	if (m_btnHist) { m_btnHist->DestroyWindow(); }//delete m_btnHist; }
	if (m_pHistory) { m_pHistory->DestroyWindow(); }// delete m_pHistory; }
	if (m_edPass) { m_edPass->DestroyWindow(); }//delete m_edPass; }
	for(int n=0; n<2; ++n)
	{
		m_stTMado[n]->DestroyWindow(); //delete m_stTMado[n];
		m_stTMasu[n]->DestroyWindow(); //delete m_stTMasu[n];
		m_stTotal[n]->DestroyWindow(); //delete m_stTotal[n];
	}
	CWnd::OnDestroy();	
}

void CMapWnd::CreateStatic( CStatic *pSt, LPCTSTR lpszCaption )
{
	pSt->Create(lpszCaption, SS_CENTER|WS_VISIBLE|WS_CHILD, CRect(0, 0, 0, 0), this, IDC_STATIC);
	pSt->SetFont(m_pFont);
}

void CMapWnd::OnGroupChange()
{
	if (!m_edPass) return;

	CString pswd;
	m_edPass->GetWindowText(pswd);
	if (pswd.IsEmpty()) 
	{
		return;
	}

// 	OutputDebugString("[KSJ] 8710 pswd[" + pswd + "] passCCx[" + Variant(passCCx, "") + "]");

CString slog;
slog.Format("[8710][%s]  [%s]", pswd, Variant(passCCx, ""));

OutputDebugString(slog);
//	if (pswd!=Variant(passCCx, ""))
CString stmp;
stmp.Format(Variant(passCCx, ""));
stmp.TrimRight();
pswd.TrimRight();
	if (pswd.Compare(stmp) != 0)
	{
		MessageBox("접속비밀번호가 올바르지 않습니다.","IBK투자증권");
		m_edPass->SetSel(0, -1);
		m_edPass->SetFocus();
		return;
	}

	m_edPass->EnableWindow(FALSE);

	const int nIndex = m_cbGroup.GetCurSel();
	if (nIndex<0) return;

	CString gpnm;
	m_cbGroup.GetWindowText(gpnm);

	CGroup *group = NULL;
	for(int n=0; n<m_groups.GetSize(); ++n)
	{
		if (m_groups[n].gpnm == gpnm)
		{
			group = &m_groups[n];
			break;
		}
	}
	if (group==NULL) return;

	m_cbGroup.EnableWindow(FALSE);
	m_tree.DeleteAllItems();
	m_anodes.RemoveAll();
	m_cnodes.RemoveAll();
	m_req_acno.RemoveAll();
	for(int n=0; n<group->Accounts.GetSize(); ++n)
	{
		CAccount *pa = &group->Accounts[n];
		HTREEITEM node = m_tree.InsertItem(pa->GetMaskAcno(), TVI_ROOT, TVI_LAST);

		m_anodes.SetAt(pa->acno, node);
		m_req_acno.Add(*pa);

		const int iRow = m_tree.GetRowOfItem(node);
		m_tree.SetItemText(node, pa->acnm, 1 );

		for(int ii=0; ii<GRID_COL_SIZE; ++ii)
			m_tree.SetItemBkColor(node, ii, GetIndexColor(78));

		m_tree.Expand(node, TVE_EXPAND);
	}

	m_chegdata.RemoveAll();
	req_scheg();
}

void CMapWnd::InitTree()
{
	m_tree.SetFont(m_pFont, FALSE);
	m_tree.SetBkColor(m_crBodyBk);
	m_tree.InsertColumn(0, "계좌번호/종목명", TVCFMT_CENTER, m_conf_colsize[0]);
	m_tree.InsertColumn(1, "계좌명/유형" , TVCFMT_LEFT, m_conf_colsize[1]);
	m_tree.InsertColumn(2, "주문수량" , TVCFMT_RIGHT, m_conf_colsize[2]);
	m_tree.InsertColumn(3, "체결량" , TVCFMT_RIGHT, m_conf_colsize[3]);
	m_tree.InsertColumn(4, "미체결량" , TVCFMT_RIGHT, m_conf_colsize[4]);
	m_tree.InsertColumn(5, "체결평균가" , TVCFMT_RIGHT, m_conf_colsize[5]);
	m_tree.InsertColumn(6, "체결금액(천원)" , TVCFMT_RIGHT, m_conf_colsize[6]);
}

void CMapWnd::req_scheg(LPCSTR nkey)
{
	if (m_req_acno.GetSize()<=0) return;

	CAccount *pa = &m_req_acno.GetAt(0);
	vector<char> buff(L_ledgerH+sizeof(struct piboscheg_mid));
	
	struct _ledgerH *ledger = (struct _ledgerH *)&buff[0];
	struct piboscheg_mid *mid = (struct piboscheg_mid *)&buff[L_ledgerH];

	GetLedger(ledger);
	if (nkey==NULL)
	{
		ledger->fkey[0] = 'C';
	}
	else
	{
		ledger->fkey[0] = '7';
		memcpy(ledger->nkey, nkey, strlen(nkey));
	}

	//2013.12.23 KSJ 일방향암호화 추가
	CopyMemory(&ledger->hsiz, "44", sizeof(ledger->hsiz));
	CString strPass;
	strPass = GetEncPassword(pa->pass);

	CopyMemory(&ledger->epwd, strPass, strPass.GetLength());
	sprintf(mid->pswd, "HEAD");


	mid->gubn[0] = '>';	//2015.05.28 KSJ 구분값 추가 
	sprintf(mid->accn, pa->acno);
	sprintf(mid->date, "%.*s", sizeof(mid->date), "");
	sprintf(mid->cod2, "%.*s", sizeof(mid->cod2), "");
	mid->dsgb[0] = '0';
	mid->otim[0] = '0';
	mid->sygb[0] = '0';
	mid->dllf[0] = '1';
	mid->dlgb[0] = '0';
	mid->sort[0] = '1';

	pa->load = TRUE;
	SendTR("pihoschg", US_ENC, &buff[0], buff.size(), TK_PIBOCHEG);
}

void CMapWnd::res_scheg( LPCSTR data, int dlen )
{
	struct _ledgerH *ledger = (struct _ledgerH *)data;
	struct piboscheg_mod *mod = (struct piboscheg_mod *)(data+L_ledgerH);

	CString acno(mod->accn, sizeof(mod->accn));
	const int nrec = Str2Int(mod->nrec, sizeof(mod->nrec));

//	TRACE("Account - %s\n", acno);
	for(int ii=0; ii<nrec; ++ii)
	{
		struct piboscheg_grid *grid = &(mod->rec[ii]);
	
		ChegData cd;

		CString code(grid->cod2, sizeof(grid->cod2)); code.TrimRight();
		CString hnam(grid->hnam, sizeof(grid->hnam)); hnam.TrimRight();
		CString gubn(grid->odgb, sizeof(grid->odgb)); gubn.TrimRight();
		CString stat(grid->stat, sizeof(grid->stat)); stat.TrimRight();

		m_name2codes.SetAt(hnam, code);

		sprintf(cd.acno, acno);
		sprintf(cd.code, code);
		sprintf(cd.hnam, hnam);
		sprintf(cd.gubn, gubn);
		sprintf(cd.stat, stat);
		cd.juno = Str2Int(grid->juno, sizeof(grid->juno));
		cd.ojno = Str2Int(grid->ojno, sizeof(grid->ojno));
		cd.jprc = Str2Int(grid->jprc, sizeof(grid->jprc));
		cd.jqty = Str2Int(grid->jqty, sizeof(grid->jqty));
		cd.cprc = Str2Int(grid->dprc, sizeof(grid->dprc));
		cd.cqty = Str2Int(grid->dqty, sizeof(grid->dqty));
		cd.xqty = Str2Int(grid->wqty, sizeof(grid->wqty));
		
// 		TRACE("[%s] %s, %s, %s, %s, %s, %d, %d, %d, %d, %d, %d, %d\n", 
// 			cd.getKey(), acno, code, hnam, gubn, stat, cd.juno, cd.ojno, cd.jprc, cd.jqty, cd.cprc, cd.cqty, cd.xqty);

		m_chegdata.SetAt(cd.getKey(), cd);
	}

	if (ledger->next[0]=='Y')
	{
		CString nkey(ledger->nkey, sizeof(ledger->nkey));
		nkey.TrimLeft(); nkey.TrimRight();
		req_scheg(nkey);
	}
	else
	{
		Account2Tree(acno);
		if (m_req_acno.GetSize()>0)
			m_req_acno.RemoveAt(0);

		if (m_req_acno.GetSize()==0)	m_cbGroup.EnableWindow(TRUE);
		else							req_scheg();
		
	}
}

void CMapWnd::Account2Tree( LPCSTR acno )
{
	int iVal=0;
	CString key, nkey;
	ChegData cdat;
	CSortStringArray akey;
	CMap<CString, LPCSTR, int, int> jqty, cqty, xqty, pprc;
	CMap<CString, LPCSTR, double, double> pamt;
	int s_jqty=0, d_jqty = 0, s_cqty = 0, d_cqty = 0, s_xqty = 0, d_xqty = 0;
	double s_pamt = 0.0, d_pamt = 0.0, s_tpamt = 0.0, d_tpamt = 0.0;

	s_jqty = d_jqty = s_cqty = d_cqty = s_xqty = d_xqty = 0;
	s_pamt = d_pamt = s_tpamt = d_tpamt = 0.0;

	{
		POSITION pos = m_chegdata.GetStartPosition();
		while(pos!=NULL)
		{
			m_chegdata.GetNextAssoc(pos, key, cdat);
			
			if      (cdat.gubn[0]=='+')	s_tpamt += (cdat.cqty * cdat.cprc);
			else if (cdat.gubn[0]=='-') d_tpamt += (cdat.cqty * cdat.cprc);

			if (memcmp(acno, cdat.acno, sizeof(cdat.acno))!=0) continue;

			CString stat = cdat.stat;

			nkey.Format("%s|%s|%s", cdat.code, cdat.hnam, cdat.gubn);

			if (cdat.gubn[0]=='+')
			{
				s_jqty += (cdat.cqty + cdat.xqty);
				s_cqty += cdat.cqty;
				s_xqty += cdat.xqty;
				s_pamt += (cdat.cqty * cdat.cprc);
			}
			else if (cdat.gubn[0]=='-')
			{
				d_jqty += (cdat.cqty + cdat.xqty);
				d_cqty += cdat.cqty;
				d_xqty += cdat.xqty;
				d_pamt += (cdat.cqty * cdat.cprc);
			}

			if (jqty.Lookup(nkey, iVal))
			{
				if (stat.Find("체결")>=0 || stat.Find("접수")>=0)
				{
					jqty[nkey] += cdat.cqty + cdat.xqty;
					cqty[nkey] += cdat.cqty;
					xqty[nkey] += cdat.xqty;
					pamt[nkey] += cdat.cprc * cdat.cqty;
				}
			}
			else
			{
				if (stat.Find("체결")>=0 || stat.Find("접수")>=0)
				{
					jqty[nkey] = cdat.cqty + cdat.xqty;
					cqty[nkey] = cdat.cqty;
					xqty[nkey] = cdat.xqty;
					pamt[nkey] = cdat.cprc * cdat.cqty;
					akey.Add(nkey);
				}		
			}
		}
	}

	m_stTMado[1]->SetText( Double2CommaStr(d_tpamt) );
	m_stTMasu[1]->SetText( Double2CommaStr(s_tpamt) );
	m_stTotal[1]->SetText( Double2CommaStr(d_tpamt+s_tpamt) );

	HTREEITEM node;
	if (m_anodes.Lookup(acno, node))
	{
		akey.Sort();

		{
			CString strVal;

			strVal.Format("%s(%s)", Int2CommaStr(d_jqty), Int2CommaStr(s_jqty));
			m_tree.SetItemText(node, strVal, 2);

			strVal.Format("%s(%s)", Int2CommaStr(d_cqty), Int2CommaStr(s_cqty));
			m_tree.SetItemText(node, strVal, 3);

			strVal.Format("%s(%s)", Int2CommaStr(d_xqty), Int2CommaStr(s_xqty));
			m_tree.SetItemText(node, strVal, 4);

			strVal.Format("%s(%s)", Int2CommaStr((int)(d_pamt/1000)), Int2CommaStr((int)(s_pamt/1000)));
			m_tree.SetItemText(node, strVal, 6);
		}

		for(int ii=0; ii<akey.GetSize(); ++ii)
		{
			LPCSTR st = (LPCSTR)akey[ii];
			LPCSTR ed = st + akey[ii].GetLength();

			vector<StringProxy> vFields;
			ParseString(st, ed, '|', vFields);

			if (vFields.size()!=3) continue;

			CString code = vFields[0].ToCStr();
			CString hnam = vFields[1].ToCStr();
			CString gubn = vFields[2].ToCStr();

			COLORREF rgb = m_crBlack;
			if (gubn.GetAt(0)=='+') 
			{
				rgb = m_crPlus;
				gubn = gubn.Mid(1);
			}
			else if (gubn.GetAt(0)=='-')
			{
				rgb = m_crMinus;
				gubn = gubn.Mid(1);
			}

			HTREEITEM cnode = m_tree.InsertItem(hnam, node, TVI_LAST);
			m_tree.SetItemText(cnode, gubn, 1);
			m_tree.SetItemText(cnode, Int2CommaStr(jqty[akey[ii]]), 2);
			m_tree.SetItemText(cnode, Int2CommaStr(cqty[akey[ii]]), 3);
			m_tree.SetItemText(cnode, Int2CommaStr(xqty[akey[ii]]), 4);
			if ((int)(cqty[akey[ii]]) == 0)
				m_tree.SetItemText(cnode, "0", 5);
			else
				m_tree.SetItemText(cnode, Int2CommaStr((int)(pamt[akey[ii]]/(double)cqty[akey[ii]])), 5);
			m_tree.SetItemText(cnode, Int2CommaStr((int)(pamt[akey[ii]]/1000)), 6);
			
			for(int col=1; col<GRID_COL_SIZE; ++col)
				m_tree.SetItemTextColor(cnode, col, rgb);

			char ckey[128];
			sprintf(ckey, "%s%s%s", acno, (LPCTSTR)code, (LPCTSTR)gubn);
			m_cnodes.SetAt(ckey, cnode);
		}
	}
}

void CMapWnd::proc_notice( LPCSTR data, int dlen )
{
	vector<StringProxy> vFields;
	ParseNotice(data, data+dlen, vFields);

	// key\tval 형식으로 오기때문에 무조건 짝수갯수로 온다.
	if (vFields.size()%2!=0) return;

	CMap<int,int,CString,LPCSTR> rdat;
	for(size_t n=0; n<vFields.size(); n+=2)
	{
		int key = _ttoi(vFields[n].ToCStr());
		CString val = vFields[n+1].ToCStr(); val.TrimRight();
		rdat.SetAt( key, val );
	}

	LPCSTR  acno = rdat[901];
	LPCSTR  code = rdat[907]; if (strlen(code)==7) ++code;	// A,J떼어내기	
	LPCSTR  hnam = rdat[908];
	LPCSTR  jtim = rdat[923];
	CString gubn = rdat[924];
	CString stat = rdat[988];
	CString cdgb = rdat[925];
	const int juno = _ttoi(rdat[904]);
	const int ogno = _ttoi(rdat[905]);
	const int jprc = _ttoi(rdat[910]);
	const int jqty = _ttoi(rdat[931]);
	const int cprc = _ttoi(rdat[916]);
	const int cqty = _ttoi(rdat[931]);
	const int xqty = _ttoi(rdat[921]);
	char   akey[32];
	sprintf(akey, "%s%10d", acno, juno);	//2015.05.28 KSJ %05d -> %10d

	{
		HTREEITEM hTmp;
		if (!m_anodes.Lookup(acno, hTmp)) return;
	}
	

	m_name2codes.SetAt(hnam, code);

	ChegData cdat;
	const BOOL bFind = m_chegdata.Lookup(akey, cdat);
	
	if (bFind && stat=="체결")
	{
		ChegData *pcd = &m_chegdata[akey];
		pcd->cqty += cqty;	// 체결 수량
		pcd->cprc = cprc;	// 체결 가격 
		pcd->xqty = xqty;	// 미체결 수량
		sprintf(pcd->stat, stat);
//		TRACE(pcd->toString());

		const COLORREF crText = gubn.Find("매수")>=0 ? m_crPlus : m_crMinus;

		CString bmsg;
		bmsg.Format("%.2s:%.2s:%.2s %s[%s] %s(%s) %s(%s) %s %d주", 
			jtim, jtim+2, jtim+4,
			stat, gubn.Mid(1), GetName(acno), GetMaskAcno(acno), hnam, code, cdgb, cqty);
		m_stHist->SetFgColor( crText );
		m_stHist->SetText(bmsg);
		if (m_pHistory->GetSafeHwnd())
		{
// 			OutputDebugString(bmsg);
// 			OutputDebugString("\n");000
			m_pHistory->AddData(bmsg, crText);
		}
	}
	else if (bFind && stat.Find("거부")!=-1)
	{
		sprintf(akey, "%s%10d", acno, juno);	//2015.05.28 KSJ %05d -> %10d
		if (m_chegdata.Lookup(akey, cdat))
		{
			ChegData *pcd = &m_chegdata[akey];
			pcd->xqty -= jqty;
			pcd->jqty -= jqty;
		}
	}
	else if (!bFind && stat=="취소확인")
	{
		sprintf(akey, "%s%10d", acno, ogno);	//2015.05.28 KSJ %05d -> %10d
		if (m_chegdata.Lookup(akey, cdat))
		{
			ChegData *pcd = &m_chegdata[akey];
			pcd->xqty -= jqty;
		}
	}
	else if (!bFind && (stat=="접수" || stat=="정정확인"))
	{
		sprintf(cdat.acno, acno);
		sprintf(cdat.code, code);
		sprintf(cdat.hnam, hnam);
		sprintf(cdat.gubn, gubn);
		sprintf(cdat.stat, stat);
		cdat.juno = juno;
		cdat.ojno = ogno;
		cdat.jprc = jprc;
		cdat.jqty = jqty;
		cdat.cprc = 0;
		cdat.cqty = 0;
		cdat.xqty = xqty;

		m_chegdata.SetAt(cdat.getKey(), cdat);

		sprintf(akey, "%s%10d", acno, ogno);	//2015.05.28 KSJ %05d -> %10d
		if (m_chegdata.Lookup(akey, cdat))
		{
			ChegData *pcd = &m_chegdata[akey];
			pcd->xqty -= jqty;
		}
	}

//	TRACE("UpdateTree (%s) (%s) (%s) (%s)\n", acno, code, gubn, stat);
	UpdateTree(acno, code, hnam, gubn);

	if (stat == "체결")  //20230228
		OnGroupChange();
}

void CMapWnd::UpdateTree( LPCSTR acno, LPCSTR code, LPCSTR hnam, LPCSTR gubn )
{
	if (!m_tree.GetSafeHwnd()) return;
	HTREEITEM anode{}, cnode{};

	if (!m_anodes.Lookup(acno, anode)) return;

	CString ckey;
	int clen = strlen(code);
	const int glen = strlen(gubn);
	if (clen==7)
	{
		++code;	// A, J는 없앤다.
		--clen;	// 길이는 줄이고~
	}
	ckey.Format("%s%s%s", acno, code, gubn+1);	// 부호없애기

	int s_jqty=0, d_jqty=0, s_cqty=0, d_cqty=0, s_xqty=0, d_xqty=0;
	double s_pamt=0.0, d_pamt=0.0, s_tpamt=0.0, d_tpamt=0.0;

	if (!m_cnodes.Lookup(ckey, cnode)) 
	{
		cnode = m_tree.InsertItem(hnam, anode, TVI_FIRST);
		m_cnodes.SetAt(ckey, cnode);
	}
	
	ChegData cdat;
	int jqty=0, cqty=0, xqty=0, pamt=0;
	{
		POSITION pos = m_chegdata.GetStartPosition();
		while(pos!=NULL)
		{
			m_chegdata.GetNextAssoc(pos, ckey, cdat);

			if      (cdat.gubn[0]=='+')	s_tpamt += (cdat.cqty * cdat.cprc);
			else if (cdat.gubn[0]=='-') d_tpamt += (cdat.cqty * cdat.cprc);

			if (memcmp(acno, cdat.acno, sizeof(cdat.acno))!=0) continue;
			
			//OutputDebugString(cdat.toString());
			if (cdat.gubn[0]=='+')
			{
				s_jqty += (cdat.cqty + cdat.xqty);
				s_cqty += cdat.cqty;
				s_xqty += cdat.xqty;
				s_pamt += (cdat.cqty * cdat.cprc);
			}
			else if (cdat.gubn[0]=='-')
			{
				d_jqty += (cdat.cqty + cdat.xqty);
				d_cqty += cdat.cqty;
				d_xqty += cdat.xqty;
				d_pamt += (cdat.cqty * cdat.cprc);

//				TRACE("d_jqty(%d) d_cqty(%d) d_xqty(%d)\n", d_jqty, d_cqty, d_xqty);
			}

			if (memcmp(code, cdat.code, clen)!=0) continue;
			if (memcmp(gubn, cdat.gubn, glen)!=0) continue;

			CString stat = cdat.stat;
			if (stat.Find("취소")==-1)
			{
				jqty += cdat.cqty + cdat.xqty;
				cqty += cdat.cqty;
				xqty += cdat.xqty;
				pamt += cdat.cprc * cdat.cqty;
			}
		}
	}
	
	m_stTMado[1]->SetText( Double2CommaStr(d_tpamt) );
	m_stTMasu[1]->SetText( Double2CommaStr(s_tpamt) );
	m_stTotal[1]->SetText( Double2CommaStr(d_tpamt+s_tpamt) );

	COLORREF rgb = m_crBlack;
	if (gubn[0]=='+') 
	{
		rgb = m_crPlus;
		++gubn;
	}
	else if (gubn[0]=='-')
	{
		rgb = m_crMinus;
		++gubn;
	}

	{
		CString strVal;
		
// 		strVal.Format("%d %d %d %d %d %d %.f %.f\n", d_jqty, s_jqty, d_cqty, s_cqty, d_xqty, s_xqty, d_pamt, s_pamt);
// 		OutputDebugString(strVal);

		strVal.Format("%s(%s)", Int2CommaStr(d_jqty), Int2CommaStr(s_jqty));
		m_tree.SetItemText(anode, strVal, 2);
		
		strVal.Format("%s(%s)", Int2CommaStr(d_cqty), Int2CommaStr(s_cqty));
		m_tree.SetItemText(anode, strVal, 3);
		
		strVal.Format("%s(%s)", Int2CommaStr(d_xqty), Int2CommaStr(s_xqty));
		m_tree.SetItemText(anode, strVal, 4);
		
		strVal.Format("%s(%s)", Int2CommaStr((int)(d_pamt/1000.0)), Int2CommaStr((int)(s_pamt/1000.0)));
		m_tree.SetItemText(anode, strVal, 6);
	}

	m_tree.SetItemText(cnode, gubn, 1);
	m_tree.SetItemText(cnode, Int2CommaStr(jqty), 2);
	m_tree.SetItemText(cnode, Int2CommaStr(cqty), 3);
	m_tree.SetItemText(cnode, Int2CommaStr(xqty), 4);
	
	if((int)cqty == 0)
		m_tree.SetItemText(cnode, "0", 5);
	else
		m_tree.SetItemText(cnode, Int2CommaStr((int)(pamt/(double)cqty)), 5);

	m_tree.SetItemText(cnode, Int2CommaStr((int)(pamt/1000.0)), 6);
		
	for(int col=1; col<GRID_COL_SIZE; ++col)
		m_tree.SetItemTextColor(cnode, col, rgb);
}

void CMapWnd::OnCellDblClick( NMHDR *pHDR, LRESULT *pResult )
{
	NM_TREEVIEW *pData = (NM_TREEVIEW*)pHDR;
	
	CString code, hnam, acno;

	hnam = m_tree.GetItemText(pData->itemNew.hItem);
	acno = m_tree.GetItemText(m_tree.GetParentItem(pData->itemNew.hItem));
	acno.Replace("-", "");

	if (m_name2codes.Lookup(hnam, code))
	{
		CString mapn;
#if 0
		mapn.Format("IB871001 /S/p5/t/dAN1A\t%s\ncod2\t%s\nDDAT\t1\n", acno, code);
		SetPopup(mapn, TRUE);
#else
		mapn.Format("IB110200 /S/t/dAN1A\t%s\ncod2\t%s\nDDAT\t1\n", acno, code);
		SetView(mapn);
#endif
	}
}

CString CMapWnd::GetPassword( LPCSTR acno )
{
	// CArray<CGroup, CGroup&> m_groups;
	for(int n=0; n<m_groups.GetSize(); ++n)
	{
		CGroup *pg = &m_groups[n];
		for(int i=0; i<pg->Accounts.GetSize(); ++i)
		{
			if (pg->Accounts[i].acno==acno)
				return pg->Accounts[i].pass;
		}
	}
	return "";
}

void CMapWnd::OnCellClick( NMHDR *pHDR, LRESULT *pResult )
{
	NM_TREEVIEW *pData = (NM_TREEVIEW*)pHDR;
	
	CString code, hnam;
	hnam = m_tree.GetItemText(pData->itemNew.hItem);
	if (m_name2codes.Lookup(hnam, code))
	{
		if (!m_bRelayCode) return;
		CString trig;
		trig.Format("1301\t%s", code);
		SetTrigger(trig);
	}
	else	// 계좌 트리거
	{
		if (!m_bRelayAcno) return;
		CString acno, acnm, trig;
		acno = hnam;
		acno.Replace("-","");

		if (acno.GetLength()!=11) return;

		trig.Format("AN1A\t%s", acno);
		SetTrigger(trig);
	}
}

std::shared_ptr<CfxImgButton> CMapWnd::CreateImgButton( LPCSTR caption, int nID, CBitmap **bmp )
{
	std::shared_ptr<CfxImgButton> pBtn = std::make_shared<CfxImgButton>(m_pFont);
	if (pBtn->Create(caption, CRect(0, 0, 0, 0), this, nID, FALSE, TRUE))
	{
		pBtn->SetImgBitmap(	bmp[0]->operator HBITMAP(), bmp[1]->operator HBITMAP(), bmp[2]->operator HBITMAP() );
		return pBtn;
	}
	else return nullptr;
}

void CMapWnd::OnClickSave()
{
	SetModal("IB0000A1 /S/s/p5/t");
	req_group();
}

void CMapWnd::OnClickConf()
{

	SetModal("IB0000A2 /S/s/p5/t");
	req_group();

	return;
	

	HTREEITEM node = m_tree.InsertItem("0000000000", TVI_ROOT, TVI_LAST);

	//m_anodes.SetAt(pa->acno, node);
	//m_req_acno.Add(*pa);

	const int iRow = m_tree.GetRowOfItem(node);
	m_tree.SetItemText(node, "0000000000", 1);

	for (int ii = 0; ii < GRID_COL_SIZE; ++ii)
		m_tree.SetItemBkColor(node, ii, GetIndexColor(78));

	m_tree.Expand(node, TVE_EXPAND);







}

void CMapWnd::OnClickSend()
{
	OnGroupChange();
}

CString CMapWnd::GetName( LPCSTR acno )
{
	for(int n=0; n<m_groups.GetSize(); ++n)
	{
		CGroup *pg = &m_groups[n];
		for(int i=0; i<pg->Accounts.GetSize(); ++i)
		{
			if (pg->Accounts[i].acno==acno)
				return pg->Accounts[i].acnm;
		}
	}
	return "";
}

CString CMapWnd::GetMaskAcno( LPCSTR acno )
{
	for(int n=0; n<m_groups.GetSize(); ++n)
	{
		CGroup *pg = &m_groups[n];
		for(int i=0; i<pg->Accounts.GetSize(); ++i)
		{
			if (pg->Accounts[i].acno==acno)
				return pg->Accounts[i].GetMaskAcno();
		}
	}
	return "";
}

void CMapWnd::OnClickHist()
{
	if (!m_pHistory) return;
	if (m_pHistory->m_arList.GetSize()==0) return;

	CRect rc;
	m_stHist->GetWindowRect(&rc);
	rc.bottom = rc.top;
	rc.top = rc.bottom - (min(10, m_pHistory->m_arList.GetSize())*HISTORY_ITEM_HEIGHT)+2;
	rc.right = rc.left + HISTORY_WIDTH;

	m_pHistory->SetWindowPos(&wndTopMost, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam==IDC_ED_PASSWD && m_edPass)
	{
		NMHDR *phdr = (NMHDR*)lParam;
		if (phdr->code==ENM_VKRETURN)
		{
			OnGroupChange();
		}
	}
	return CWnd::OnNotify(wParam, lParam, pResult);
}

LRESULT CMapWnd::OnInitFocus( WPARAM wParam, LPARAM lParam )
{
	if (m_edPass)
		if (m_edPass->GetSafeHwnd())
			m_edPass->SetFocus();
	return 0;
}

void CMapWnd::SaveConfig()
{
	LPCSTR szApp = "IB871000";
	CString val, val2, path;
	
	path.Format("%s\\User\\%s\\u_config.ini", m_home, m_name);

	CRect rc;
	GetClientRect(&rc);

	val.Format("%d", m_bRelayAcno); WritePrivateProfileString(szApp, "relay_acno", val, path);
	val.Format("%d", m_bRelayCode); WritePrivateProfileString(szApp, "relay_code", val, path);

	val.Format("%d", rc.Width());	WritePrivateProfileString(szApp, "width", val, path);
	val.Format("%d", rc.Height());	WritePrivateProfileString(szApp, "height", val, path);

	for(int n=0; n<GRID_COL_SIZE; ++n)
	{
		val2.Format("COL%d_WIDTH", n);
		val.Format("%d", m_tree.GetColumnWidth(n));
		WritePrivateProfileString(szApp, val2, val, path);
	}
}

void CMapWnd::LoadConfig()
{
	LPCSTR szApp = "IB871000";
	CString val, val2, path;
	
	path.Format("%s\\User\\%s\\u_config.ini", m_home, m_name);

	m_bRelayCode = GetPrivateProfileInt(szApp, "relay_code", 1, path);
	m_bRelayAcno = GetPrivateProfileInt(szApp, "relay_acno", 1, path);

	const int def_col_size[] = {120, 70, 60, 60, 60, 70, 80};

	m_conf_width = GetPrivateProfileInt(szApp, "width", -1, path);
	m_conf_height = GetPrivateProfileInt(szApp, "height", -1, path);

	for(int n=0; n<GRID_COL_SIZE; ++n)
	{
		val2.Format("COL%d_WIDTH", n);
		m_conf_colsize[n] = GetPrivateProfileInt(szApp, val2, def_col_size[n], path);
	}
}

LRESULT CMapWnd::OnInitSize( WPARAM wParam, LPARAM lParam )
{
	if (m_conf_width>0 && m_conf_height>0)
		m_pParent->PostMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(m_conf_width, m_conf_height));
	return 0;
}

void CMapWnd::OnRelayAcno()
{
	if (!m_chkAcno) return;
	m_bRelayAcno = (m_chkAcno->GetCheck()==1) ? TRUE : FALSE;
}

void CMapWnd::OnRelayCode()
{
	if (!m_chkCode) return;
	m_bRelayCode = (m_chkCode->GetCheck()==1) ? TRUE : FALSE;
}

CString CMapWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_home, _T("\\dev\\CX_SHA256.DLL"));
	CString strRetrun;
	
	if(m_hiSha256 == NULL)
	{
		m_hiSha256 = LoadLibrary(dllPath);
		
		if (!m_hiSha256)
		{
			TRACE("CX_SHA256 컨트롤 생성 실패1");
			return "";
		}
	}
	
	if(m_hiSha256)
	{
		typedef long (WINAPI *GETSHAFUNC)(char*, int);	
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(m_hiSha256, "axEncrypt");
		
		if (func)
		{
			strRetrun =  (char*)func(sPswd.GetBuffer(sPswd.GetLength()),sPswd.GetLength());
		}
		
	}	
	return strRetrun;
}