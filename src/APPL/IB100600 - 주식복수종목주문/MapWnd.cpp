// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB100600.h"
#include "MapWnd.h"
#include "Define.h"
#include "../../control/fx_misc/misctype.h"
#include "../../H/interMSG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd* pParent)
{
	m_lSizeRate[0] = 0.16;
	m_lSizeRate[1] = 0.07;
	m_bInit = FALSE;
	m_iOldSize[0] = m_iOldSize[1] = 0;
	m_iCurFont = 9;
	m_bExpendClick = FALSE;
	m_iWndWidth = SZ_MAP_WIDTH;
	m_iWndHeight = SZ_MAP_HEIGHT;
	m_iExpendWidth = SZ_EXPAND_WIDTH;

	m_pCmdWnd = NULL;
	m_hiSha256 = NULL;

	m_pParent = pParent;
	m_pTab = nullptr;
	m_bExpand = FALSE;
	m_pExpandWnd = nullptr;
	m_pGuide = nullptr;
	m_pModCanWnd = nullptr;
	m_pOrderWnd = nullptr;
	m_pExpand = nullptr;

//	m_iOldSize[0] = SZ_MAP_WIDTH ;
//	m_iOldSize[1] = SZ_MAP_HEIGHT ;
}

CMapWnd::~CMapWnd()
{
//	_SAFEDESTROY(m_pTab);
//	_SAFEDESTROY(m_pExpandWnd);
//	_SAFEDESTROY(m_pGuide);
//	_SAFEDESTROY(m_pModCanWnd);
//	_SAFEDESTROY(m_pOrderWnd);
//	_SAFEDESTROY(m_pExpand);
//	_SAFEDESTROY(m_pWarnWnd);
	
	if (m_hiSha256)
	{
		AfxFreeLibrary(m_hiSha256);
		m_hiSha256 = NULL;
	}
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnTabChange)
	ON_BN_CLICKED(IDC_EXPANDBTN, OnExpand)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Variant(titleCC, "[1006] 주식복수종목주문");
	m_strRoot = Variant(homeCC);
	m_pDefNormalFont = m_pNormalFont = GetAxFont("굴림체", 9, false, 0);
	m_pBoldFont = GetAxFont("굴림체", 9, false, 1);

	CRect rcClient;
	GetClientRect(&rcClient);
	rcClient.DeflateRect(FRAME_SPACE, FRAME_SPACE, FRAME_SPACE, INNER_FRAME_SPACE);

	CRect rcCtrl;
	rcCtrl.SetRect(OUTER_FRAME_SPACE, OUTER_FRAME_SPACE,
		rcClient.Width() - SZ_EXPAND_WIDTH - 2 - SZ_FORM_MARGIN - SZ_CTRL_1BUTTON - SZ_CTRL_HGAP,
		SZ_FORM_MARGIN + SZ_TAB_HEIGHT);

	rcCtrl.SetRect(OUTER_FRAME_SPACE, OUTER_FRAME_SPACE,
		rcClient.Width() - OUTER_FRAME_SPACE - 20,
		SZ_FORM_MARGIN + SZ_TAB_HEIGHT);
	m_pTab = std::make_unique<CfxTab>();
	m_pTab->Create(rcCtrl, this, IDC_TAB);

	rcCtrl.left = rcClient.Width() - SZ_FORM_MARGIN - SZ_CTRL_1BUTTON;
	rcCtrl.right = rcCtrl.left + SZ_CTRL_1BUTTON;

	CRect rc = rcCtrl;
	rc.top = rc.bottom - SZ_CTRL_HEIGHT - 1;
	m_pExpand = std::make_unique<CfxImgButton>();
	m_pExpand->Create("\n호가", rc, this, IDC_EXPANDBTN, FALSE, TRUE);

	rcCtrl.left = OUTER_FRAME_SPACE;
	rcCtrl.right = rcClient.Width() - SZ_EXPAND_WIDTH - 2;
	rcCtrl.top = rcCtrl.bottom;
	//	rcCtrl.bottom = rcClient.Height() - SZ_FORM_MARGIN - SZ_CTRL_VGAP - SZ_CTRL_HEIGHT;
	//	rcCtrl.bottom = rcClient.bottom - GUIDE_HEIGHT - INNER_FRAME_SPACE/*bottom*/ - INNER_FRAME_SPACE/*사이3*/;  //test
	rcCtrl.bottom = rcClient.bottom - GUIDE_HEIGHT - INNER_FRAME_SPACE/*bottom*/ - INNER_FRAME_SPACE/*사이3*/ - STATIC_HEIGHT;

	m_pOrderWnd = std::make_unique<COrderWnd>(m_pParent);
	m_pOrderWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_ORDER);

	rcCtrl.left = rcCtrl.right + INNER_FRAME_SPACE;
	rcCtrl.right = rcCtrl.left + SZ_EXPAND_WIDTH;

	rcCtrl.right = rcClient.right - INNER_FRAME_SPACE;
	m_pExpandWnd = std::make_unique<CExpandWnd>(m_pParent);
	m_pExpandWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_EXPAND);
	m_iDefExpendHeight = rcCtrl.Height();
	m_iDefExpendWidth = rcCtrl.Width();

	//참고사항 윈도우 추가
	rcCtrl.top = rcCtrl.bottom + INNER_FRAME_SPACE * 2;
	rcCtrl.left = FRAME_SPACE;
	rcCtrl.right = rcClient.right;
	rcCtrl.bottom = rcCtrl.top + STATIC_HEIGHT;

	m_pWarnWnd = std::make_unique<CWarningWnd>(m_pParent);
	m_pWarnWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_WARNWND);

	rcCtrl.top = rcCtrl.bottom + 2;
	rcCtrl.bottom = rcCtrl.top + GUIDE_HEIGHT;
	rcCtrl.right = rcClient.right;
	m_pGuide = std::make_unique<CfxStatic>();
	m_pGuide->Create(_T(""), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_GUIDE);

	SetTimer(TT_INIT, 50, NULL);
	m_bInit = TRUE;
	return 0;
}

void CMapWnd::LoadRoundBitmap()
{
	CString strHome = GetAxRoot();
	CString sFile;
	sFile.Format("%s\\tab\\PALETTE.INI", strHome);

	CString sPalette = _T("");
	int readL = 0; 
	char readB[1024]; 
	readL = GetPrivateProfileString(_T("General"), _T("Palette"), _T("Blue"), readB, sizeof(readB), sFile);
	if (readL > 0)	sPalette.Format("%s", readB);

	CString sImage;
	sImage.Format("%s\\Image\\axpanel%s1_",  strHome, sPalette);
	for(int i=0; i<4; i++)
	{
		if(m_hRoundBitmap[i])
			DeleteObject(m_hRoundBitmap[i]);
	}

	CIB100600App* pApp = (CIB100600App*)AfxGetApp(); 
	m_hRoundBitmap[0] = (HBITMAP)::LoadImage( pApp->m_hInstance , sImage+"lt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[1] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"rt.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[2] = (HBITMAP)::LoadImage( pApp->m_hInstance, sImage+"lb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );
	m_hRoundBitmap[3] = (HBITMAP)::LoadImage(pApp->m_hInstance, sImage+"rb.bmp",
											IMAGE_BITMAP,0,0,LR_LOADFROMFILE );

}

void CMapWnd::Init()
{
	m_pTab->InsertItem(0, _T("일괄주문"));
	m_pTab->InsertItem(1, _T("정정/취소"));

	m_pTab->LoadTabImage(m_strRoot, false);
	m_pTab->SetStyle(tsIMAGE, false);
	m_pTab->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
					GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));

	m_pTab->SetFont(m_pNormalFont);

	CString strPath = "";
	strPath.Format("%s\\Image\\", GetAxRoot());
/*
	m_hRoundBitmap[0] = GetAxBitmap(strPath+_T("axpanel0_lt.bmp"))->operator HBITMAP();
	m_hRoundBitmap[1] = GetAxBitmap(strPath+_T("axpanel0_rt.bmp"))->operator HBITMAP();
	m_hRoundBitmap[2] = GetAxBitmap(strPath+_T("axpanel0_lb.bmp"))->operator HBITMAP();
	m_hRoundBitmap[3] = GetAxBitmap(strPath+_T("axpanel0_rb.bmp"))->operator HBITMAP();
	*/
	LoadRoundBitmap();
	m_hGuideBitmap = GetAxBitmap(strPath+_T("알림_ic.bmp"))->operator HBITMAP();

	HBITMAP hbmp	= GetAxBitmap(strPath + "투자매매종합100.bmp")->operator HBITMAP();
	HBITMAP hbmp_dn	= GetAxBitmap(strPath + "투자매매종합100_dn.bmp")->operator HBITMAP();
	HBITMAP hbmp_hv = GetAxBitmap(strPath + "투자매매종합100_en.bmp")->operator HBITMAP();

	m_pExpand->SetImgBitmap(hbmp, hbmp_dn, hbmp_hv);
	m_pExpand->SetFont(m_pNormalFont);

	m_pOrderWnd->SetFont(m_pNormalFont);
	m_pOrderWnd->Init();

	m_pExpandWnd->SetFont(m_pNormalFont);
	m_pExpandWnd->Init();

	ShowHideBodyWnd(m_pTab->GetCurSel());

	m_pGuide->SetStyle(m_pGuide->GetStyle() | ssBORDER);
	m_pGuide->SetBkColor(GetIndexColor(GUIDE_BACK_INDEX));
	m_pGuide->SetFgColor(GetIndexColor(GUIDE_TXT_INDEX));
	m_pGuide->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pGuide->SetFont(m_pNormalFont);
	m_pGuide->SetAlignment(alLEFT);

	m_pGuide->SetCornerRound(m_hRoundBitmap);
	m_pGuide->SetBitmap(m_hGuideBitmap);

	m_pOrderWnd->SetInputFieldFocus();
	ShowWindow(SW_SHOW);

	m_pWarnWnd->Init();
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient, rcbox, wrc;
	GetClientRect(&rcClient);
	dc.FillSolidRect(&rcClient, GetIndexColor(BACK_INDEX));


	m_pOrderWnd->GetWindowRect(&wrc);
	ScreenToClient(wrc);

	rcbox = rcClient;
	rcbox.DeflateRect(FRAME_SPACE,FRAME_SPACE,FRAME_SPACE,INNER_FRAME_SPACE);
	rcbox.bottom = wrc.bottom + INNER_FRAME_SPACE;
	

	dc.FillSolidRect(&rcbox, GetIndexColor(CONTENTS_BACK_INDEX));
	DrawCornerRoundBitmap(&dc, rcbox, 0);

	rcbox.left = wrc.right;
	rcbox.right = wrc.right + 3;
	rcbox.top = wrc.top;
	rcbox.bottom =  rcbox.top+m_pOrderWnd->m_iCmdH;
	dc.FillSolidRect(&rcbox, GetIndexColor(CMD_BACK_INDEX));
}

CFont* CMapWnd::GetAxFont(CString strName, int nPt, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name = (LPSTR)(LPCTSTR)strName;
	fontR.point = nPt;
	fontR.italic = bItalic;
	fontR.bold = nBold;

	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CString CMapWnd::Variant(int nComm, CString strData)
{
	CString strRet(_T(""));

	char* pData = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pData > 1)
		strRet = pData;

	return strRet;
}

COLORREF CMapWnd::GetIndexColor(UINT nIndex)
{
	if (!m_pParent)
		return 0;

	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)nIndex);
}

void CMapWnd::ShowHideBodyWnd(int nIndex)
{
	switch (nIndex)
	{
	case 0:
		if (m_pOrderWnd)
			m_pOrderWnd->ShowWindow(SW_SHOW);
		if (m_pModCanWnd)
			m_pModCanWnd->ShowWindow(SW_HIDE);
		
		if (m_pModCanWnd)
		{
			
			if (m_pModCanWnd->m_pAccount->GetAccNo() == m_pOrderWnd->m_pAccount->GetAccNo()) 
			{
				CString pwd;
				m_pModCanWnd->m_pPassword->GetWindowText(pwd);
				m_pOrderWnd->m_pPassword->SetWindowText(pwd);
			}
		}

		break;
	case 1:
		if (m_pOrderWnd)
			m_pOrderWnd->ShowWindow(SW_HIDE);
		if (m_pModCanWnd)
			m_pModCanWnd->ShowWindow(SW_SHOW);

		if (m_pOrderWnd)
		{
			//AfxMessageBox(m_pOrderWnd->m_pAccount->GetAccNo());
			CString pwd;
			pwd = m_pModCanWnd->m_pAccount->GetAccNo();
			pwd.TrimRight();
			if ((pwd == "") || (pwd == m_pOrderWnd->m_pAccount->GetAccNo())) 
			{
				
				m_pOrderWnd->m_pPassword->GetWindowText(pwd);
				m_pModCanWnd->m_pPassword->SetWindowText(pwd);
			}
		}
		break;
	}
	
}

void CMapWnd::OnTabChange(NMHDR * pHdr, LRESULT * result)
{
	if (!(pHdr->idFrom == (UINT)IDC_TAB)) return;
	ChangeSelectTab( m_pTab->GetCurSel() );
	/*
	if (m_pTab->GetCurSel() == 1 && !m_pModCanWnd)
		CreateModCanWnd();

	if (pHdr->idFrom == IDC_TAB)
		ShowHideBodyWnd(m_pTab->GetCurSel());

	CString sval;
	sval.Format("%d",  m_pTab->GetCurSel() );
	SaveBuffer(_T("SelTab"), sval);
	*/
	return;
}

void CMapWnd::ChangeSelectTab(int itab)
{
	if (m_pTab->GetCurSel() == 1 && !m_pModCanWnd)
		CreateModCanWnd();

	ShowHideBodyWnd(m_pTab->GetCurSel());
	CString sval;
	sval.Format("%d",  m_pTab->GetCurSel() );
	SaveBuffer(_T("SelTab"), sval);

}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			if (m_pGuide)
				m_pGuide->SetText(_T(""));

			const int key = HIBYTE(LOWORD(wParam));
			const int len = HIWORD(wParam);
			CString strTemp;
			struct _ledgerH ledger;
			CString strLedger(_T(""));
			CString strErrCode(_T(""));
			CString strErrText(_T(""));
			CString strMsg(_T(""));
			int nNextKey = 0;

			CString stmp;
			stmp.Format("[memo][IB100600]  key =[%s]   len=[%d]\r\n", GetMemoTrKeyType(key), HIWORD(wParam));
			OutputDebugString(stmp);

			if ((key >= TRKEY_MEMO_POPUPOOP && key <= TRKEY_MEMO_CHECK) || key == 0)
			{
				if (m_pModCanWnd)
					m_pModCanWnd->ParsingSymbol(wParam, lParam);
				break;
			}

			switch (key)
			{
			case KEY_CHKPASSWORD:
				{
					
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					strErrCode = CString((char*)&ledger.ecod, 4);
					strErrText = CString((char*)&ledger.emsg, 98);
					strErrText.TrimRight(" "); 
					
					if(strErrCode.Left(1) != "0")
					{
						SetGuide(strErrText);
						MessageBox(strErrCode + ":" + strErrText);
					}
					else 
						SetTimer(TT_CHKPWD, 100, NULL);

				}
				return 0;

			case KEY_ACCOUNT:
			case KEY_ACCNAME:
			case 250:
				if (m_pTab->GetCurSel() == 0)
					((CAccountCtrl*)m_pOrderWnd->GetAccountCtrl())->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
				else
					((CAccountCtrl*)m_pModCanWnd->GetAccountCtrl())->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
				break;

			case KEY_SREMAIN:
			case KEY_SORDER:
			case KEY_NONCONT:
			case KEY_MODCANSORDER:
				strTemp = (char*)lParam;
				CopyMemory(&ledger, (void*)lParam, L_ledgerH);
				strLedger = CString((char*)&ledger, L_ledgerH);
				strErrCode = CString((char*)&ledger.emsg, 4);
				strErrText = CString((char*)&ledger.emsg, 98);
				nNextKey = ledger.next[0];
				
				lParam += L_ledgerH;
				strMsg = "ERR\t" + strErrText;

				if (strErrCode.GetAt(0) != _T('0'))	// 오류
				{
					strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
					strMsg.TrimRight();

					// 기업은행임직원 주문제한[9997] - 2011.05.19 by warship
					if (strErrCode == _T("1801") || strErrCode == _T("3669") || strErrCode == _T("9997"))
					{
						MessageBox(strMsg);
						if (key == KEY_SREMAIN || key == KEY_SORDER)
							m_pOrderWnd->ClearOrderGrid();
						else if (key == KEY_NONCONT || key == KEY_MODCANSORDER)
							m_pModCanWnd->ClearModCanGrid();
					}
					else if (strErrCode == "3735" || strErrCode == "4727")  //최선집행미등록 고객
					{
						CString strData;
						strData = _T("IB999963 /S /t");
						m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, typeVIEW), (LPARAM)(LPCTSTR)strData);
					}
					else
						SetGuide(strMsg);

					return 0;
				}

				if (key == KEY_SREMAIN)
					m_pOrderWnd->ParsingSRemain((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_SORDER)
					m_pOrderWnd->ParsingSOrder((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_NONCONT)
					m_pModCanWnd->ParsingNonCont((char*)lParam, HIWORD(wParam), strMsg);
				else if (key == KEY_MODCANSORDER)
					m_pModCanWnd->ParsingSOrder((char*)lParam, HIWORD(wParam), strMsg);

				break;

			case KEY_SHOGA:
				m_pOrderWnd->ParsingSCHoga((char*)lParam, TRUE);
				break;

			case KEY_SCURR:
				m_pOrderWnd->ParsingSCHoga((char*)lParam, FALSE);
				break;

			case KEY_SYMBOL:
				if (m_pModCanWnd)
					m_pModCanWnd->ParsingSymbol(wParam, lParam);
				break;

			case KEY_MODCANSHOGA:
				m_pModCanWnd->ParsingSCHoga((char*)lParam, TRUE);
				break;

			case KEY_MODCANSCURR:
				m_pModCanWnd->ParsingSCHoga((char*)lParam, FALSE);
				break;

			case KEY_SHOGA_STDPRC:
				m_pOrderWnd->ParsingSCHoga((char*)lParam, TRUE);
				break;
				
			case KEY_SCURR_STDPRC:
				m_pOrderWnd->ParsingSCHoga((char*)lParam, FALSE);
				break;
								
			case KEY_MODCANSHOGA_STDPRC:
				m_pModCanWnd->ParsingSCHoga((char*)lParam, TRUE);
				break;
				
			case KEY_MODCANSCURR_STDPRC:
				m_pModCanWnd->ParsingSCHoga((char*)lParam, FALSE);
				break;

			}
		}
		break;

	case DLL_TRIGGER:
		{
			CString strTrigger = (char*)lParam;
			CString strData(_T(""));
			CString strSymbol(_T(""));

			while (!strTrigger.IsEmpty())
			{
				strData = Parser(strTrigger, '\n');
				strSymbol = Parser(strData, '\t');

				if (!strSymbol.CompareNoCase("CODX") || !strSymbol.CompareNoCase("1301"))
				{
					if (m_pModCanWnd)
						m_pModCanWnd->SetCode(strData);
				}
				else if (!strSymbol.CompareNoCase("AN10"))
				{
					if (m_pOrderWnd)
						m_pOrderWnd->SetAccount(strData);

					if (m_pModCanWnd)
						m_pModCanWnd->SetAccount(strData);
				}
			}
		}
		break;

	case DLL_DOMINO:
		{
/*
			CString strDomino = (char*)lParam;

			while (!strDomino.IsEmpty())
			{
				CString strData = Parser(strDomino, _T('\n'));
				CString strSymbol = Parser(strData, _T('\t'));
				if (!strSymbol.CompareNoCase(_T("1301")))
				{
					if (m_pModCanWnd)
						m_pModCanWnd->SetCode(strData);
				}
			}			
*/
		}
	case DLL_SETPAL:
		SetPallete();
		break;

	case DLL_GUIDE:
		SetGuide((char*)lParam);
		return 1;

	case DLL_SETFONT:break;
	case DLL_SETFONTx:
		ChangeFont( HIWORD(wParam), CString((char*)lParam) );
		break;

		//2016.02.16 KSJ 종목명컨트롤에 실시간 전달
	case DLL_ALERTx:
		if (m_pModCanWnd)
			m_pModCanWnd->SendMessage(WM_USER+1, wParam, lParam);

		break;
	}


	return 0;
}

CBitmap* CMapWnd::GetAxBitmap(CString strPath)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)(LPCTSTR)strPath);
}

void CMapWnd::SetPallete()
{
	LoadRoundBitmap();
	m_pTab->SetColor(GetIndexColor(CLR_TAB_FORE), GetIndexColor(CONTENTS_BACK_INDEX), 
					GetIndexColor(CLR_TAB_SELFORE), GetIndexColor(CONTENTS_BACK_INDEX), GetIndexColor(CONTENTS_BACK_INDEX));

	m_pTab->LoadTabImage(m_strRoot, false);
	m_pTab->SetBorderColor(GetIndexColor(TAB_BORDER_INDEX));
	m_pExpandWnd->SetPallete();

	if (m_pModCanWnd)
		m_pModCanWnd->SetPallete();

	if (m_pOrderWnd)
		m_pOrderWnd->SetPallete();

	m_pGuide->SetBkColor(GetIndexColor(GUIDE_BACK_INDEX));
	m_pGuide->SetFgColor(GetIndexColor(GUIDE_TXT_INDEX));
	m_pGuide->SetBorderColor(GetIndexColor(CLR_GUIDE_LINE));
	m_pGuide->SetCornerRound(m_hRoundBitmap);


	m_pGuide->Invalidate();

	Invalidate();
}

void CMapWnd::Procedure(CString strProc)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(GetMajor(), GetMinor())), (LPARAM)(LPCTSTR)strProc);
}

int CMapWnd::GetMajor()
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, majorCC), 0L);
}

int CMapWnd::GetMinor()
{
	return m_pExpandWnd->GetMapKey();
}

BOOL CMapWnd::SendTR(CString strTR, CString strData, int nKey, int nStat)
{
	CString strTRData = "";
	
	struct _userTH user;
	strncpy(&user.trc[0], strTR, 8);
	user.key  = nKey;
	user.stat = nStat;
	
	strTRData = CString((char*)&user, sizeof(_userTH));
	strTRData += strData;
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), 
		(LPARAM)(const char*)strTRData); 
/*




	CString strData(_T(""));
	CString strTRData = (_T(""));
	
	// Data 구성
	struct _mid mid;
	// ...
	strData = CString((char*)&mid, sizeof(_mid));
	
	// User Header 구성
	struct _userTH user;
	strncpy(&user.trc[0], "PIBOXXXX", 8);
	user.key  = TRKEY;
	user.stat = 0;
	
	strTRData = CString((char*)&user, sizeof(_userTH));
	strTRData += strData;
	
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), 
		(LPARAM)(const char*)strTRData); 
*/
}

void CMapWnd::SetGuide(CString strMsg)
{
	if( strMsg.Find("그룹계좌") > -1 && strMsg.Find("존재") > -1 )
		return;

	m_pGuide->SetText(strMsg);
	SetTimer(TT_GUIDE, 5000, NULL);
}

void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TT_CHKPWD)
	{
		KillTimer(TT_CHKPWD);
		if(m_pCmdWnd)
			m_pCmdWnd->PostMessage(WM_USER+1, KEY_CHKPASSWORD, 1);
		m_pCmdWnd = NULL;
	}

	if (nIDEvent == TT_GUIDE)
	{
		m_pGuide->SetText(_T(""));
		KillTimer(TT_GUIDE);
	}

	if(nIDEvent == TT_INIT)
	{
		KillTimer(TT_INIT);
		CString sval = GetBuffer(_T("SelTab"));
		if(!sval.IsEmpty())
		{
			const int itab = atoi(sval);
			if(itab)
			{
				m_pTab->SetCurSel(itab);
				ChangeSelectTab( itab);
			}
		}
	}
	
	CWnd::OnTimer(nIDEvent);
}

CString CMapWnd::Parser(CString &srcstr, CString substr)
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

void CMapWnd::OnExpand()
{
	m_bExpendClick = TRUE;
	m_bExpand = !m_bExpand;

	int nWidth = m_iWndWidth;
	const int nHeight = m_iWndHeight;
	if (m_bExpand)
		m_pExpandWnd->ShowWindow(SW_SHOW);
	else
	{
		nWidth -= m_iExpendWidth;
		m_pExpandWnd->ShowWindow(SW_HIDE);
	}

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, NULL), MAKELPARAM(nWidth, nHeight));
	MoveWindow(0, 0, nWidth, nHeight);

	CRect rcClient, wrc, tabrc;
	GetClientRect(&rcClient);

	m_pTab->GetWindowRect(&tabrc);
	ScreenToClient(&tabrc);
	
	m_pExpand->GetWindowRect(&wrc);
	ScreenToClient(wrc);
	const int itop = tabrc.bottom - wrc.Height() -1 ;
	m_pExpand->MoveWindow(rcClient.Width() - SZ_FORM_MARGIN - wrc.Width() , 
								/*SZ_FORM_MARGIN,*/itop,
								wrc.Width(), wrc.Height());

//	m_pExpand->MoveWindow(rcClient.Width() - SZ_FORM_MARGIN - SZ_CTRL_1BUTTON, SZ_FORM_MARGIN,
//								SZ_CTRL_1BUTTON, SZ_CTRL_1BUTTON);
}

void CMapWnd::CreateModCanWnd()
{
	//	XMSG('가나다라마바사');
	CRect rcCtrl;
	m_pOrderWnd->GetClientRect(&rcCtrl);
	m_pOrderWnd->ClientToScreen(&rcCtrl);
	ScreenToClient(&rcCtrl);

	m_pModCanWnd = std::make_unique<CModCanWnd>(m_pParent);
	m_pModCanWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, rcCtrl, this, IDC_MODCAN);

	m_pModCanWnd->SetFont(m_pNormalFont);
	m_pModCanWnd->Init();
	m_pModCanWnd->SetPallete();

	if(m_iCurFont != 9)
		m_pModCanWnd->ChangeSize(m_lcalcRate, m_iCurFont, m_lszFontName);
}

void CMapWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapWnd::ChangeFont(int ifontsixze, LPCTSTR lszFont)
{
	m_lszFontName = lszFont;
	if(!m_bInit) return;
	if(m_iCurFont == ifontsixze) return;

	m_pNormalFont = GetAxFont(lszFont, ifontsixze, false, 0);
	m_pBoldFont = GetAxFont(lszFont, ifontsixze, false, 1);
	if(!m_pNormalFont) return;

	const double ldif = float(ifontsixze - 9);
	const int iWndWidth = SZ_MAP_WIDTH + (int)(m_lSizeRate[0] * (float)SZ_MAP_WIDTH * ldif);
	const int iWndHeight = SZ_MAP_HEIGHT +(int) (m_lSizeRate[1] * (float)SZ_MAP_HEIGHT * ldif);

	m_iCurFont = ifontsixze;
	this->SetFont(m_pNormalFont);
	m_iWndWidth = iWndWidth;
	m_iWndHeight = iWndHeight;

	m_pTab->SetFont(m_pNormalFont);
	m_pExpand->SetFont(m_pNormalFont);
	m_pGuide->SetFont(m_pNormalFont);

	SetWindowPos(NULL, 0, 0, iWndWidth, iWndHeight, SWP_NOZORDER | SWP_NOMOVE);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(iWndWidth, iWndHeight) );
}

void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	

	if(m_bExpendClick)
	{
		m_bExpendClick = FALSE;
		return;
	}

	if(cx == 0 || cy == 0) return;
	if(!m_bInit) return;
	
	// TODO: Add your message handler code here
	if(m_iOldSize[0] == 0 && m_iOldSize[1] == 0)
	{
		m_iOldSize[0] = cx ;
		m_iOldSize[1] = cy ;
		return;
	}
//	if(m_iOldSize[0] == cx && m_iOldSize[1] == cy) return;
	double lcalcrate[2] = {( (float)cx - (float)m_iOldSize[0] ) / (float)m_iOldSize[0], 
							( (float)cy - (float)m_iOldSize[1] ) /(float)m_iOldSize[1]};
	
	m_lcalcRate[0] = lcalcrate[0];
	m_lcalcRate[1] = lcalcrate[1];

	m_iOldSize[0] = cx ;
	m_iOldSize[1] = cy ;

	CRect wrc, rc, trc;
	int iw=0, ih=0, itop=0;
	GetClientRect(&rc);

	m_pGuide->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	ih = wrc.Height() ;
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	itop = cy - ih - INNER_FRAME_SPACE;
	m_pGuide->MoveWindow(FRAME_SPACE, itop, cx-FRAME_SPACE, ih );
	const int iguideh = ih;
	

	m_pTab->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);

	m_pTab->GetItemRect(0, trc);
	CSize tabsize = CSize(trc.right, trc.bottom);
	tabsize.cx = tabsize.cx + (int)(lcalcrate[0] * (float)tabsize.cx );
	tabsize.cy = tabsize.cy + (int)(lcalcrate[1] * (float)tabsize.cy );
	if(m_iCurFont == 9)	tabsize.cy = SZ_TAB_HEIGHT;
	m_pTab->SetItemSize(tabsize);
	ih = tabsize.cy+1;

	wrc.SetRect(wrc.left, wrc.top, wrc.left + iw, wrc.top+ih-1);
	m_pTab->MoveWindow(wrc );
	itop = wrc.bottom;
	tabsize.cy = wrc.Height();
	const int icontenth = rc.Height() - ih - wrc.Height() - INNER_FRAME_SPACE-INNER_FRAME_SPACE;
	itop = wrc.top+ih-1;

	m_pExpand->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	int ileft = cx - iw - FRAME_SPACE - INNER_FRAME_SPACE ;
	
	m_pExpand->MoveWindow(ileft, itop-ih, iw, ih );

	m_pOrderWnd->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	iw = wrc.Width(), ih = wrc.Height() ;

	const int igap = 3;
	iw = iw + (int)(lcalcrate[0] * (float)iw );
	ih = ih + (int)(lcalcrate[1] * (float)ih);
	m_pOrderWnd->MoveWindow(wrc.left, itop, iw, icontenth);
	m_pOrderWnd->ChangeSize(lcalcrate, m_iCurFont, m_lszFontName);
	const int igridw = wrc.left+iw;

	ileft = iw + wrc.left+ INNER_FRAME_SPACE;
	if(m_pModCanWnd)
	{
		m_pModCanWnd->MoveWindow(wrc.left, itop, iw, icontenth);
		m_pModCanWnd->ChangeSize(lcalcrate, m_iCurFont, m_lszFontName);
	}
	
	ih = rc.Height() - iguideh  - itop- INNER_FRAME_SPACE/*bottom*/- INNER_FRAME_SPACE/*사이3*/- INNER_FRAME_SPACE;
	iw = rc.Width() - igridw - INNER_FRAME_SPACE - INNER_FRAME_SPACE;
	wrc.SetRect(ileft, itop, iw, ih);

	m_pExpandWnd->MoveWindow(ileft, itop, iw, ih);
	m_pExpandWnd->GetWindowRect(&wrc);
	ScreenToClient(&wrc);
	m_iExpendWidth = wrc.Width();

	
	itop = itop + ih + 2;
}

void CMapWnd::SaveBuffer(CString sKey, CString sData)
{
	CIB100600App* pApp = (CIB100600App*)AfxGetApp();
	pApp->SaveBuffer(sKey, sData);
}

CString CMapWnd::GetBuffer(CString sKey)
{
	CIB100600App* pApp = (CIB100600App*)AfxGetApp();
	return pApp->GetBuffer(sKey);
}


void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap)
{
	HDC hDC = ::CreateCompatibleDC (NULL);
	SelectObject (hDC, hBitmap); //icon
//	::BitBlt( pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(),	hDC, 0, 0, SRCCOPY);
	::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);

	::DeleteDC (hDC);
}

void CMapWnd::DrawCornerRoundBitmap(CDC* pDC, CRect rc, int iType)
{
	const int iPoint = 5;
	int idx = 0;
	if(iType) idx = 4;

	DrawBitmap(pDC, CRect(rc.left, rc.top, rc.left+iPoint, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.top, rc.right, rc.top +iPoint), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.left, rc.bottom-iPoint, rc.left+iPoint, rc.bottom), m_hRoundBitmap[idx++]);
	DrawBitmap(pDC, CRect(rc.right-iPoint, rc.bottom-iPoint, rc.right, rc.bottom), m_hRoundBitmap[idx++]);
}



struct _chkP
{
	char szLedger[L_ledgerH];
	char szEdit1[5];
	char szAcc[20];
	char szPwd[8];
	char szType[1]; 
};

BOOL CMapWnd::CheckPassWord(CString sAcc, CString sPass, CWnd* pCmdWnd) 
{
	m_pCmdWnd = pCmdWnd;
	CString sTRCode = _T("SACMQ901");
	sTRCode = _T("SACMT238");
	sPass.TrimLeft(" "); 
	sPass.TrimRight(" ");
	
	CString sUser = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), NULL);

	struct	_ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

//	CopyMemory(&ledger.size, "000374", sizeof(ledger.size));
	CopyMemory(&ledger.svcd, sTRCode, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)sUser, sUser.GetLength());
	CopyMemory(&ledger.brno, sAcc.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	//2013.12.23 KSJ 일방향암호화 추가
	if(!sPass.IsEmpty())
	{
		CString pswd;
		pswd = GetEncPassword(sPass);
		CopyMemory(&ledger.hsiz, "44", sizeof(ledger.hsiz));
		CopyMemory(&ledger.epwd, pswd, pswd.GetLength());
	}

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
//00
	//sAcc = sAcc.Left(3) + "00" +  sAcc.Mid(5, 6);  //종합잔고로 변경
	for(int i=sAcc.GetLength() ; i<20;i++) sAcc += _T(" ");
	
	sPass = "HEAD";	//2013.12.23 KSJ 일방향암호화
	for(int i=sPass.GetLength() ; i<8;i++) sPass += _T(" ");
	CString sChkPass = _T("00001");
	sChkPass += sAcc;
	sChkPass += sPass;
	//sChkPass += _T("1");

	CString strSendData = CString((char*)&ledger, L_ledgerH);
	strSendData += sChkPass;

	CString sTmp;
	sTmp.Format("[%d]%s",  strSendData.GetLength(), strSendData);

	SendTR(_T("piboPBxQ"), strSendData, KEY_CHKPASSWORD, 0);
	return TRUE;
}

CString CMapWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_strRoot, _T("\\dev\\CX_SHA256.DLL"));
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