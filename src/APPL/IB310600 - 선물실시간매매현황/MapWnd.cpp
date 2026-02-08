// MapWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB310600.h"
#include "MapWnd.h"

#include "SplitterCtrl.h"

#include "AccountCtrl.h"
#include "ScreenWnd.h"
#include "GuideWnd.h"
#include "../../h/ledger.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMapWnd

CMapWnd::CMapWnd(CWnd *pParent)
	:m_pWizard(pParent)
{
	m_pPanelCtrl = NULL;
	m_pStAccName = NULL;

	m_pAccount = NULL;
	m_pEditPass = NULL;

	m_pBtnInquiry = NULL;

	m_pScreenWnd = NULL;	
	m_pGuideWnd = NULL;

	
	m_bBtnState[0] = FALSE;
	m_bBtnState[1] = FALSE;
	m_bBtnState[2] = FALSE;
	
	m_bPopUp = FALSE;

	m_nGuideCnt = 0;
	m_strGuide = _T("");
	
	m_nKey = 0;
	m_nOrder = 0;
}

CMapWnd::~CMapWnd()
{
	SAFE_DELETE(m_pPanelCtrl);

	SAFE_DELETE(m_pAccount);
	SAFE_DELETE(m_pEditPass);
	SAFE_DELETE(m_pStAccName);

	SAFE_DELETE(m_pBtnInquiry);

	SAFE_DELETE(m_pScreenWnd);
	SAFE_DELETE(m_pGuideWnd);
}


BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnMessage1)
	ON_MESSAGE(WM_BTNMSG, OnMsgBtn)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMapWnd message handlers

int CMapWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	Variant(titleCC, S_TITLE);

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(MAP_WIDTH, MAP_HEIGHT));
	m_nOrder = (long)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L);	//0:직원 1:고객
	
	m_nMapHeight = MAP_HEIGHT;
	m_strRoot = Variant(homeCC);
	m_strUser = Variant(userCC);
	m_strPal = GetPalette();
	m_pFont = GetAxFont("굴림", 9, false, 0);
	m_pBFont = GetAxFont("굴림", 9, false, FW_BOLD);

	SetBitmap();

	CreateControl();

	m_pAccount->SetFocus();

	return 0;
}

void CMapWnd::CreateControl()
{
	CRect mapRc, cRc;
	CRect panelRc, screenRc, guideRc;

	GetClientRect(mapRc);

	panelRc.SetRect(mapRc.left, mapRc.top, mapRc.right, mapRc.top + PANEL_HEIGHT);
	m_topRc = panelRc;
	guideRc.SetRect(mapRc.left, mapRc.bottom - GUIDE_HEIGHT, mapRc.right, mapRc.bottom);
	screenRc.SetRect(mapRc.left, panelRc.bottom, mapRc.right, guideRc.top);

	cRc.SetRect(panelRc.left + 62, panelRc.top + GAP_TOP + GAP_CTRLT, 
				panelRc.left + 62 + ACC_WIDTH, panelRc.top + GAP_TOP + GAP_CTRLT + CTRL_HEIGHT);
	m_pAccount = std::make_unique<CAccountCtrl>(m_pWizard, this, m_strRoot);
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE|WS_CHILD|WS_TABSTOP, cRc, this, 0);
	m_pAccount->createAccountCtrl(CONTROL_ACCN, KEY_ACCOUNT, GetIndexColor(7));

	cRc.left = cRc.right + 3;
	cRc.right = cRc.left + 65;
	m_pStAccName = std::make_unique <CfxStatic>(m_pFont);
	m_pStAccName->Create(_T(""), WS_CHILD|WS_VISIBLE, cRc, this, IDC_STATIC_ACCNAME);
	m_pStAccName->SetStyle(ssBORDER);
	m_pStAccName->SetBorderColor(GetIndexColor(105));
	m_pStAccName->SetBkColor(GetIndexColor(49));
	m_pStAccName->SetFgColor(GetIndexColor(clText));
	m_pStAccName->SetAlignment(alLEFT);

	cRc.left = cRc.right + 62;
	cRc.right = cRc.left + 62;
	m_pEditPass = std::make_unique <CfxEdit>(m_pFont);
	m_pEditPass->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|ES_PASSWORD, cRc, this, IDC_EDIT_PASS);
	((CEdit*)m_pEditPass.get())->LimitText(8);

	if(m_nOrder)
	{
		m_pEditPass->EnableWindow(FALSE); 
		m_pEditPass->SetBkColor(GetSysColor(COLOR_GRAYTEXT), false);
	}

	cRc.right = mapRc.right - GAP_CTRLS - GAP_SIDE;
	cRc.left = cRc.right - BTN2_WIDTH - 1;
	cRc.top = mapRc.top + GAP_TOP + GAP_CTRLT;
	cRc.bottom = cRc.top + CTRL_HEIGHT;
	m_pBtnInquiry = std::make_unique <CfxImgButton>();
	m_pBtnInquiry->Create("\n조회", cRc, this, IDC_BTN_INQUIRY);
	m_pBtnInquiry->SetWindowText("조회");
	m_pBtnInquiry->SetTextColor(GetIndexColor(clBtnText));
	m_pBtnInquiry->SetFont(m_pBFont);
	m_pBtnInquiry->SetDrawFocus();
	m_pBtnInquiry->SetImgBitmap(m_hBmpBtnInquiry, m_hBmpBtnInquiry_dn, m_hBmpBtnInquiry_hv);

	m_pGuideWnd = std::make_unique <CGuideWnd>(this);
	m_pGuideWnd->Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, guideRc, this, IDC_WND_GUIDE);

	m_pScreenWnd = std::make_unique <CScreenWnd>(this);
	m_pScreenWnd->Create(NULL, "", WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, screenRc, this, IDC_WND_SCREEN);
}

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString strBuffer = (char*)lParam;
	const int nKey = HIBYTE(LOWORD(wParam));

	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		if(nKey == 254 || nKey == 255)
			m_pAccount->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
		else if (nKey == KEY_CHKPASS)
			ParsingPass(strBuffer);
		break;
	case DLL_TRIGGER:
		Trigger(strBuffer);
		break;
	case DLL_GUIDE:
		m_pGuideWnd->SetGuide(strBuffer);
		return 1;
	case DLL_SETPAL:
		m_strPal = GetPalette();
		SetBitmap();

		m_pScreenWnd->SetPal();
		m_pGuideWnd->SetPalette(m_strPal);
		m_pGuideWnd->SetBitmap();
		m_pGuideWnd->Invalidate();
		Invalidate();
		break;
	case DLL_SETFONT:
		break;
	default:
		break;
	}
	return 0;
}

void CMapWnd::Trigger(CString strTrigger)
{
	while (!strTrigger.IsEmpty())
	{
		CString str = Parser(strTrigger, '\n');
		str.TrimRight();
		CString sym = Parser(str, '\t');
		if (!sym.CompareNoCase(CONTROL_ACCN))
			m_pAccount->TriggerAccount(str);
		else if(!sym.CompareNoCase(TRIGGER_GUIDE))
		{
			int nGuide = atoi(Parser(str, '\t'));
			if(nGuide == 1)
			{
				m_pGuideWnd->SetGuide(str);
				m_strGuide = str;
			}

			if(m_nGuideCnt < 3)
				m_nGuideCnt++;
			else
			{
				m_nGuideCnt = 0;
				if(m_strGuide.IsEmpty())
					m_pGuideWnd->SetGuide(str);

				m_strGuide = _T("");
			}
		}
		else if(!sym.CompareNoCase(TRIGGER_RPASS))
		{
			m_bPopUp = TRUE;
			m_nKey = atoi(str);

			CString strAccount = m_pAccount->GetAccNo();
			strAccount.TrimLeft();
			CString strPass = m_pEditPass->GetText();
			
			if(!m_nOrder)
			{
				if(!strAccount.IsEmpty())
					CheckPassWord(strAccount, strPass);

			}
			else OnButtonInquiry();

//			if(!strAccount.IsEmpty())
//				CheckPassWord(strAccount, strPass);
		}
	}
}

long CMapWnd::OnMessage1(WPARAM wParam, LPARAM lParam)
{
	CString strBuffer = (char*)lParam;
	switch (LOBYTE(LOWORD(wParam)))
	{
	case 100:
		SetAccInfo(strBuffer);
		break;
	}

	return 0;
}

long CMapWnd::OnMsgBtn(WPARAM wParam, LPARAM lParam)
{
	CRect mapRc;

	if(HIWORD(wParam) == BN_CLICKED)
	{
		const int id = LOWORD(wParam);
		switch(id)
		{
		case IDC_BTN_UP:
			GetClientRect(mapRc);
			m_pScreenWnd->SetClickBtn(TRUE);
			const int nMiddleHeight = m_pScreenWnd->GetScreenHeight();
			m_nMapHeight = PANEL_HEIGHT + GUIDE_HEIGHT + nMiddleHeight;
			SendSize(mapRc.Width(), m_nMapHeight);
			break;
		}
	}
	
	return 0;
}

void CMapWnd::OnButtonInquiry()
{
	CString strAccount = m_pAccount->GetAccNo();
	strAccount.TrimLeft();
	CString strPass= m_pEditPass->GetText();

	if (strAccount.IsEmpty())
	{
		m_pAccount->SetFocus();
		return;
	}

	if (m_nOrder == 0)
	{
		if (strPass.IsEmpty() || strPass.GetLength() < 4)
		{
			m_pEditPass->SetFocus();
			return;
		}
	}
	else
	{
		if (strPass.IsEmpty())
			strPass = "9999";
	}

	CString strData = strAccount + "\t" + strPass;

	if(m_bPopUp)
	{
		CString strBuffer;
		strBuffer.Format("%s\t%s\t%s\t", MAP_PROC, PARAM_POPUP, strData);
		Procedure(strBuffer, m_nKey, 0);
		m_bPopUp = FALSE;
	}
	else
		m_pScreenWnd->SetScreenAccount(PARAM_VALUE, strData);
}

void CMapWnd::SetBitmap()
{
	CStringArray strImageArr;
	CString sImageDir;
	//panel
	strImageArr.Add("axpanel" + m_strPal + "0_lt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "0_rt.bmp");
	strImageArr.Add("axpanel" + m_strPal + "0_lb.bmp");
	strImageArr.Add("axpanel" + m_strPal + "0_rb.bmp");

	for(int i = 0; i < 4; i++)
	{	
		m_hBmpRoundPanel[i] = GetAxBitmap(strImageArr.GetAt(i));
	}

	m_hBmpBtnInquiry = GetAxBitmap("조회BTN.bmp");
	m_hBmpBtnInquiry_dn = GetAxBitmap("조회BTN_dn.bmp");
	m_hBmpBtnInquiry_hv = GetAxBitmap("조회BTN_en.bmp");
}

CString CMapWnd::GetPalette()
{
	char buff[10];
	CString strPal;
	CString file;
	file.Format("%s\\%s\\%s", m_strRoot, "tab", "palette.ini");

	GetPrivateProfileString("General", "Palette", "", buff, sizeof(buff), file);
	strPal = buff;

	return strPal;
}

void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CRect cRc;

	CPen	*pOldPen = dc.SelectObject(GetAxPen(GetIndexColor(clPanelLine), 1, PS_SOLID));
	dc.SelectStockObject(NULL_BRUSH);
	
	const int bkMode = dc.SetBkMode(TRANSPARENT);
	CFont *pOldFont = dc.SelectObject(m_pFont);

	cRc = m_topRc;
	DrawOuterSolidRect(&dc, cRc, GAP_SIDE, GAP_TOP, GAP_SIDE, GAP_BLOCK, GetIndexColor(clBack));
	
	cRc.DeflateRect(GAP_SIDE, GAP_TOP, GAP_SIDE, GAP_BLOCK);
	dc.FillSolidRect(cRc, GetIndexColor(clPanel));
	DrawRoundBitmap(&dc, cRc, m_hBmpRoundPanel);

	dc.SetTextColor(GetIndexColor(clText));
	cRc = m_topRc;
	cRc.DeflateRect(GAP_SIDE + GAP_CTRLS, GAP_TOP, 0, GAP_TOP);
	dc.DrawText("계좌번호", cRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	cRc.OffsetRect(270, 0);
	dc.DrawText("비밀번호", cRc, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	
	dc.SelectObject(pOldFont);
	dc.SelectObject(pOldPen);
	dc.SetBkMode(bkMode);
}

//깜박임 제거를 위해 외곽에만 backcolor를 그림
void CMapWnd::DrawOuterSolidRect(CDC *dc, CRect rc, int nLeft, int nTop, int nRight, int nBottom, COLORREF color)
{
	CRect cRc;
	cRc = rc;	cRc.right = nLeft;
	dc->FillSolidRect(cRc, color);

	cRc = rc;	cRc.bottom = nTop;
	dc->FillSolidRect(cRc, color);

	cRc = rc;	cRc.left = rc.right - nRight;
	dc->FillSolidRect(cRc, color);

	cRc = rc;	cRc.top = rc.bottom - nBottom;
	dc->FillSolidRect(cRc, color);
}

void CMapWnd::DrawRoundBitmap(CDC *dc, CRect rc, HBITMAP *hBitmap)
{
	//Round left + top
	CRect cRc;
	//Round left + top
	cRc.SetRect(rc.left, rc.top, 
				rc.left + 5, rc.top + 5);
	DrawBitmap(dc, cRc, hBitmap[0], true);

	//Round right + top
	cRc.SetRect(rc.right - 5, rc.top, 
				rc.right, rc.top + 5);
	DrawBitmap(dc, cRc, hBitmap[1], true);
	
	//Round left + bottom
	cRc.SetRect(rc.left, rc.bottom - 5, 
				rc.left + 5, rc.bottom);
	DrawBitmap(dc, cRc, hBitmap[2], true);

	//Round right + top
	cRc.SetRect(rc.right - 5, rc.bottom - 5, 
				rc.right, rc.bottom);
	DrawBitmap(dc, cRc, hBitmap[3], true);
}

#pragma comment (lib, "Msimg32.lib")
void CMapWnd::DrawBitmap(CDC* pDC, CRect rc, HBITMAP hBitmap, bool bTrans)
{
	CDC		memDC;
	CBitmap* pOldBitmap{}, * pBitmap{};
	
	pBitmap = CBitmap::FromHandle(hBitmap);
	
	memDC.CreateCompatibleDC(pDC);	
	if (memDC.m_hDC != NULL) 
		pOldBitmap = (CBitmap*)memDC.SelectObject(pBitmap);
	
	if (memDC.m_hDC != 0) {
		if (bTrans)
			::TransparentBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, rc.Width(), rc.Height(), 0xFF00FF);
		else
			::BitBlt(pDC->m_hDC, rc.left, rc.top, rc.Width(), rc.Height(), memDC.m_hDC, 0, 0, SRCCOPY);
	}

	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
}

BOOL CMapWnd::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	if(HIWORD(wParam) == BN_CLICKED)
	{
		const int id = LOWORD(wParam);
		CString strAccount;
		CString strPass;
		switch(id)
		{
		case IDC_BTN_INQUIRY:
			strAccount = m_pAccount->GetAccNo();
			strAccount.TrimLeft();
			strPass = m_pEditPass->GetText();
			/*
			if(!strAccount.IsEmpty())
				CheckPassWord(strAccount, strPass);
			*/
			if(!m_nOrder)
			{
				if(!strAccount.IsEmpty())
					CheckPassWord(strAccount, strPass);

			}
			else OnButtonInquiry();
			break;
		}
	}
	return CWnd::OnCommand(wParam, lParam);
}

BOOL CMapWnd::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_EDIT_PASS)
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		if (pNMHDR->code == ENM_VKRETURN)
		{
			CString strAccount = m_pAccount->GetAccNo();
			strAccount.TrimLeft();
			CString strPass = m_pEditPass->GetText();
			/*
			if(!strAccount.IsEmpty())
				CheckPassWord(strAccount, strPass);
			*/
			if(!m_nOrder)
			{
				if(!strAccount.IsEmpty())
					CheckPassWord(strAccount, strPass);

			}
			else OnButtonInquiry();
		}
	}

	return CWnd::OnNotify(wParam, lParam, pResult);
}


void CMapWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	CRect mapRc;
	CRect panelRc, screenRc, guideRc;

	GetClientRect(mapRc);

	panelRc.SetRect(mapRc.left, mapRc.top, mapRc.right, mapRc.top + PANEL_HEIGHT);
	m_topRc = panelRc;
	guideRc.SetRect(mapRc.left, mapRc.bottom - GUIDE_HEIGHT, mapRc.right, mapRc.bottom);
	screenRc.SetRect(mapRc.left, panelRc.bottom, mapRc.right, guideRc.top);

	CRect cRc;
	cRc.right = mapRc.right - GAP_CTRLS - GAP_SIDE;
	cRc.left = cRc.right - BTN2_WIDTH - 1;
	cRc.top = mapRc.top + GAP_TOP + GAP_CTRLT;
	cRc.bottom = cRc.top + CTRL_HEIGHT;
	m_pBtnInquiry->MoveWindow(cRc);
	m_pScreenWnd->MoveWindow(screenRc);
	m_pGuideWnd->MoveWindow(guideRc);
}


void CMapWnd::SendSize(int nWidth, int nHeight)
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(sizeDLL, 0), MAKELPARAM(nWidth, nHeight));
}


void CMapWnd::Procedure(CString procs, int major, int minor)
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(procDLL, MAKEWORD(major, minor)), 
		(LPARAM)(LPCTSTR)procs);
}

void CMapWnd::SetView(int type, CString data)
{
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

char* CMapWnd::Variant(int key, CString data)
{
	char* dta = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, key), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)	
		return dta;
	else			
		return "";
}

HBITMAP CMapWnd::GetAxBitmap(CString bmps)
{
	CString	path;

	path.Format("%s\\%s\\%s", m_strRoot, "image", bmps);
	return ((CBitmap *)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getBITMAP, 0), (LPARAM)(LPCTSTR)path))->operator HBITMAP();
}

COLORREF CMapWnd::GetIndexColor(int color)
{
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)color);
}

CPen* CMapWnd::GetAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pWizard->SendMessage(WM_USER, getPEN, (long)&penR);
}

CFont* CMapWnd::GetAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pWizard->SendMessage(WM_USER, getFONT, (long)&fontR);
}


void CMapWnd::SetAccInfo(CString strBuffer)
{
	m_strAccount = Parser(strBuffer, "\t");
	m_strAccName = Parser(strBuffer, "\t");
	m_strPass = Parser(strBuffer, "\t");

	if(m_pStAccName)
		m_pStAccName->SetText(m_strAccName);

	if(m_nOrder)
	{
		if(m_strPass.IsEmpty() )
		{
			m_strPass = "9999";
			m_pEditPass->SetText(m_strPass); 
		}
	}

	if(m_strPass.IsEmpty())
		m_pEditPass->SetText("");


	if(m_nOrder == 0)		//직원
	{
		if(m_strPass.IsEmpty())
			m_pEditPass->SetFocus();
		else
		{
			if(m_pEditPass)
				m_pEditPass->SetText(m_strPass);
		}
		m_pScreenWnd->SetScreenAccount(PARAM_RESET, "");
		m_pGuideWnd->SetGuide("");
	}
	else
		OnButtonInquiry();
}

BOOL CMapWnd::CheckPassWord(CString sAcc, CString sPass) 
{
	CString sTRCode = _T("SACMQ901");
	sTRCode = _T("SACMT238");

	struct	_ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');
	m_pWizard->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

//	CopyMemory(&ledger.size, "000374", sizeof(ledger.size));
	CopyMemory(&ledger.svcd, sTRCode, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)m_strUser, m_strUser.GetLength());
	CopyMemory(&ledger.brno, sAcc.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
//00
	sAcc = sAcc.Left(3) + "00" +  sAcc.Mid(5, 6);  //종합잔고로 변경
	for(int i=sAcc.GetLength() ; i<20;i++) sAcc += _T(" ");
	
	for( int i=sPass.GetLength() ; i<8;i++) sPass += _T(" ");
	CString sChkPass = _T("00001");
	sChkPass += sAcc;
	sChkPass += sPass;
	//sChkPass += _T("1");

	CString strSendData = CString((char*)&ledger, L_ledgerH);
	strSendData += sChkPass;

	CString sTmp;
	sTmp.Format("[%d]%s",  strSendData.GetLength(), strSendData);

	SendTR(_T("piboPBxQ"), strSendData, KEY_CHKPASS, 0);
	return TRUE;
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
	return m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), 
		(LPARAM)(const char*)strTRData); 
}

void CMapWnd::ParsingPass(CString strBuffer)
{
	struct _ledgerH ledger;
	CString strLedger(_T(""));
	CString strErrCode(_T(""));
	CString strErrText(_T(""));
	
	CopyMemory(&ledger, strBuffer, L_ledgerH);
	strErrCode = CString((char*)&ledger.ecod, 4);
	strErrText = CString((char*)&ledger.emsg, 98);
	strErrText.TrimRight(" "); 

	if(strErrCode.Left(1) != "0")
	{
		if(!m_bPopUp && m_nOrder == 1)
			SetTimer(TT_CHKPWD, 100, NULL);
		else
		{
			m_pGuideWnd->SetGuide(strErrText);
			MessageBox(strErrCode + ":" + strErrText);
			m_pEditPass->SetText("");
			m_pEditPass->SetFocus();
			m_bPopUp = FALSE;
		}
	}
	else
		SetTimer(TT_CHKPWD, 100, NULL);
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


void CMapWnd::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == TT_CHKPWD)
	{
		KillTimer(TT_CHKPWD);
		OnButtonInquiry();
	}
	
	CWnd::OnTimer(nIDEvent);
}
