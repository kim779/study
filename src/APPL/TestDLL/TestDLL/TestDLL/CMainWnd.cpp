// CMainWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDLL.h"
#include "CMainWnd.h"
#include "AccountCtrl.h"
#include "axisfire.h"

#include "UserDefine.h"

#include "ContainerWnd.h"

#include "MapWnd.h"

// CMainWnd

IMPLEMENT_DYNAMIC(CMainWnd, CWnd)

CMainWnd::CMainWnd(CWnd* pwndParent)
{
	m_pwndParent = pwndParent;
}

CMainWnd::~CMainWnd()
{
	/*if(m_pwndParent)
		delete m_pwndParent;*/
	if (m_pMarketpicker)
	{
		m_pMarketpicker->DestroyWindow();
		delete m_pMarketpicker;
	}
}


BEGIN_MESSAGE_MAP(CMainWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_BN_CLICKED(ID_CTRL_SEND, OnButtonSend)
	ON_BN_CLICKED(ID_CTRL_SYMSEND, OnButtonSymbol)
	ON_BN_CLICKED(ID_CTRL_SONAQ388, OnButtonSonaq388)
	ON_BN_CLICKED(ID_CTRL_SONAQ428, OnButtonSonaq428)
	ON_BN_CLICKED(ID_CTRL_SONAQ429, OnButtonSonaq429)
	ON_BN_CLICKED(ID_CTRL_POOP0200, OnButtonpoop0200)


	ON_BN_CLICKED(ID_CTRL_REGWND, OnButtonRegWnd)
	ON_BN_CLICKED(ID_CTRL_BROADCASTWND, OnButtonBroadcastWnd)
	ON_BN_CLICKED(ID_CTRL_UNREGWND, OnButtonUnRegWnd)
	ON_BN_CLICKED(ID_CTRL_WNDCNT, OnButtonGetWndCnt)  
	ON_BN_CLICKED(ID_CTRL_BTNTOPDLG, OnButtonTopDlg)

	ON_BN_CLICKED(ID_CTRL_TRDLIST, OnButtonTrdList)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_USER + 1, OnAccMessage)
	ON_MESSAGE(WM_USER + 2, OnMaketPickerMessage)
END_MESSAGE_MAP()




// CMainWnd 메시지 처리기


COLORREF CMainWnd::GetAxColor(UINT nIndex)
{
	if (nIndex & 0x02000000)
		return nIndex;

	return m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(getPALETTE, 0), (LPARAM)nIndex);
}


int CMainWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	InitCtrl();

	m_slog.Format("[orderable] orderCC 가 0 이면 직원 = [%d]  orderCCx 가 true 주문가능= [%d] ", 
		(long)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0),
		(long)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0)
		);
	OutputDebugString(m_slog);
	/*
		if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0L))
		{
			m_bEditMode = TRUE;

			if (!(long)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCCx), 0L))
			{
				m_bOrderEnable = FALSE;
			}
		}

		if (!m_bOrderEnable && bOrderCheck)
			return FALSE;
	*/
	return 0;
}
#include "AxStd.hpp"
void CMainWnd::InitCtrl()
{
	const int cx = HORI_GAP + HORI_GAP;
	int cy = VERT_GAP + VERT_GAP;
	int ileft = cx;
	int itop = cy;

	m_strHome = (char*)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);


	CRect cRc;
	cRc.SetRect(cx, cy, cx + ACC_WIDTH, cy + CTRL_HEIGHT);

	m_pAccount = new CAccountCtrl(m_pwndParent, this, m_strHome);
	m_pAccount->CreateEx(WS_EX_TOPMOST, NULL, "ACCN", WS_VISIBLE | WS_CHILD | WS_TABSTOP, cRc, this, 0);
	m_pAccount->createAccountCtrl("AN1A", TK_ACCOUNT, GetAxColor(7));

	m_rectAccName.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + 88, cRc.bottom);
	cRc.SetRect(m_rectAccName.right + GAP, cRc.top, m_rectAccName.right + GAP + 80, cRc.bottom);
	m_pPass = new CEdit;
	m_pPass->Create(WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | ES_PASSWORD | WS_TABSTOP, cRc, this, ID_CTRL_PASS);
	m_pPass->SetLimitText(8);
	

	const int ibtnw2 = 36;
	const int ibtngap = 1;
	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + ibtnw2, cRc.bottom);
	m_pBtnSend = new CButton;
	m_pBtnSend->Create(_T("AN1A"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_SEND);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	cRc.right += cRc.Width();
	//종목심볼 
	CString	text;
	CWnd* (APIENTRY * axCreate)(CWnd*, void*) = NULL;

	text.Format("%s/%s/cx_symbol.dll", m_strHome, "dev");
	m_hiSymbol = AfxLoadLibrary(text);
	if (m_hiSymbol != NULL)
	{
		axCreate = (CWnd * (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
		if (axCreate != NULL)
		{
			axCreate = (CWnd * (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, _T("axCreate"));
			if (axCreate != NULL)
			{
				struct	_param symbolparam;
				symbolparam.key = 0;
				symbolparam.name = _T("17413");
				symbolparam.rect = cRc;
				symbolparam.fonts = "굴림체";
				symbolparam.point = 9;
				symbolparam.style = 1;
				symbolparam.tRGB = 69;
				symbolparam.pRGB = 90;
				symbolparam.options = _T("/a89/b91/c92/d69/i99/s1003");

				m_pwndSymbol = (*axCreate)(m_pwndParent, &symbolparam);
				m_pwndSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
				if (m_pwndSymbol == NULL)
				{
					AfxFreeLibrary(m_hiSymbol);
					m_hiSymbol = NULL;
				}
			}
		}
	}

	cRc.SetRect(cRc.right + GAP, cRc.top, cRc.right + GAP + ibtnw2, cRc.bottom);
	m_pBtnSymbol = new CButton;
	m_pBtnSymbol->Create(_T("심볼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_SYMSEND);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	cRc.right += 50;
	m_pBtnSonaq388 = new CButton;
	m_pBtnSonaq388->Create(_T("sonaq388"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_SONAQ388);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	//cRc.right += 50;
	m_pBtnSonaq388 = new CButton;
	m_pBtnSonaq388->Create(_T("sonaq428"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_SONAQ428);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	//cRc.right += 50;
	m_pBtnSonaq388 = new CButton;
	m_pBtnSonaq388->Create(_T("sonaq429"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_SONAQ429);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	//cRc.right += 50;
	m_pBtnpoop0200 = new CButton;
	m_pBtnpoop0200->Create(_T("poop0200"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_POOP0200);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	cRc.right -= 20;
	m_pBtnBal = new CButton;
	m_pBtnBal->Create(_T("매매내역"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_TRDLIST);

	m_pWFileSync = std::make_unique< CWC_FileSync>();
	m_pWFileSync->Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_TABSTOP | WS_VSCROLL, CRect(0, 0, 0, 0), this, 9988);

	//cx_marketpicker
	cRc.OffsetRect(cRc.Width() + GAP, 0);
	cRc.right -= cRc.Width() / 2;
	m_pMarketpicker = (CContainerWnd*) new CContainerWnd;
	m_pMarketpicker->SetParent(m_pwndParent);
	m_pMarketpicker->Create(NULL, NULL, WS_CHILDWINDOW | WS_VISIBLE, cRc, this, -1);
	m_pMarketpicker->CreateControl(m_strHome, "CX_MarketPicker", "", cRc, "/k2/t3600");


	//
	//int ileft = cx;
	//int itop = cy;

	cRc.left = ileft;
	cRc.top = cRc.bottom + GAP;;
	cRc.bottom = cRc.top +20;
	cRc.right = cRc.left + 100;;

	m_pBtnWndRegister = new CButton;
	m_pBtnWndRegister->Create(_T("Regsterwnd"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
	cRc, this, ID_CTRL_REGWND);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	m_pBtnBroadCasting = new CButton;
	m_pBtnBroadCasting->Create(_T("Broadcastwnd"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_UNREGWND);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	m_pBtnWndUnRegister = new CButton;
	m_pBtnWndUnRegister->Create(_T("UnRegsterwnd"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_BROADCASTWND);

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	m_pBtnWndCnt = new CButton;
	m_pBtnWndCnt->Create(_T("wndcnt"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_WNDCNT);

	cRc.OffsetRect( cRc.Width() + GAP, 0);
	CRect tmprec;
	tmprec = cRc;
	tmprec.bottom += cRc.Width() - cRc.Height();
	CMapWnd* m_pWnd{};
	m_pWnd = new CMapWnd(m_pwndParent);
	m_pWnd->Create(nullptr, "subRTSmap", WS_CHILD | WS_VISIBLE, tmprec, this, 100);
	m_pWnd->ChangeMap("IB36011B", "1301\t005930");

	cRc.OffsetRect(cRc.Width() + GAP, 0);
	m_pBtnTopDlg = new CButton;
	m_pBtnTopDlg->Create(_T("TOPDLG"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		cRc, this, ID_CTRL_BTNTOPDLG);
}

void CMainWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rc;
	GetClientRect(rc);
	dc.FillSolidRect(rc, RGB(255, 255, 255));
	dc.Rectangle(m_rectAccName);


	dc.DrawText(m_strAccName, m_rectAccName, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

	
}


void CMainWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}

LRESULT CMainWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	
	switch (LOBYTE(LOWORD(wParam)))
	{
		case formDLL:
		{
			m_slog.Format("[formDLL][%s]", (char*)lParam);
			AfxMessageBox(m_slog);
	
		}
		break;
		case DLL_TRIGGER:
		{
			m_slog.Format("[DLL_TRIGGER][%s]", (char*)lParam);
		
		}
		break;
		case DLL_ALERTx:
		{
			CString strCode;
			struct _alertR* alertR;
			alertR = (struct _alertR*)lParam;
			strCode = alertR->code;
		}
		break;
		case DLL_OUB:
		{
			int key = HIBYTE(LOWORD(wParam));

			m_strLog.Format("\r\n[%-30s]<%d> key = [%d] [%50s]", __FUNCTION__, __LINE__, key, (char*)lParam);
			OutputDebugString(m_strLog);
			//신규 추가가 필요한 부분 
			if ((key >= TRKEY_MEMO_POPUPOOP && key <= TRKEY_MEMO_CHECK))
			{
				m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
				break;
			}

			switch (key)
			{
				case 255:
				case 254:
						m_pAccount->m_pAccountCtrl->SendMessage(WM_USER, wParam, lParam);
				break;
				case TK_CHECK207:  //계좌비번 검증
				{
 					struct _ledgerH ledger;
					CString strTemp = (char*)lParam;
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					CString strLedger = CString((char*)&ledger, L_ledgerH);
					CString strErrCode = CString((char*)&ledger.emsg, 4);
					CString strErrText = CString((char*)&ledger.emsg, 98);

					lParam += L_ledgerH;

					struct _chkPwdMod* mod = (struct _chkPwdMod*)lParam;
					m_strLog.Format("계좌비번검증 결과=[%c]", mod->zAvalYn);
					AfxMessageBox(m_strLog);
				}
				break;
				case TK_SYMBOL:
				{
					m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
				}
				break;
				case TK_SONAQ388:
				{
					struct _ledgerH ledger;
					CString strTemp = (char*)lParam;
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					CString strLedger = CString((char*)&ledger, L_ledgerH);
					CString strErrCode = CString((char*)&ledger.emsg, 4);
					CString strErrText = CString((char*)&ledger.emsg, 98);

					lParam += L_ledgerH;

					CString strdata;
					strdata.Format("[%s][%s][%s]", strErrCode, strErrText, (char*)lParam);
					AfxMessageBox(strdata);
				}
				break;
				case TK_SONAQ428:
				{
					struct _ledgerH ledger;
					CString strTemp = (char*)lParam;
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					CString strLedger = CString((char*)&ledger, L_ledgerH);
					CString strErrCode = CString((char*)&ledger.emsg, 4);
					CString strErrText = CString((char*)&ledger.emsg, 98);

					lParam += L_ledgerH;

					CString strdata;
					strdata.Format("[%s][%s][%s]", strErrCode, strErrText, (char*)lParam);
					AfxMessageBox(strdata);
				}
				break;
				case TK_SONAQ429:
				{
					struct _ledgerH ledger;
					CString strTemp = (char*)lParam;
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					CString strLedger = CString((char*)&ledger, L_ledgerH);
					CString strErrCode = CString((char*)&ledger.emsg, 4);
					CString strErrText = CString((char*)&ledger.emsg, 98);

					lParam += L_ledgerH;

					CString strdata;
					strdata.Format("[%s][%s][%s]", strErrCode, strErrText, (char*)lParam);
					AfxMessageBox(strdata);
				}
				break;
				case TK_TRDLIST:  //매매내역조회
				{
					struct _ledgerH ledger;
					CString strTemp = (char*)lParam;
					CopyMemory(&ledger, (void*)lParam, L_ledgerH);
					CString strLedger = CString((char*)&ledger, L_ledgerH);
					CString strErrCode = CString((char*)&ledger.emsg, 4);
					CString strErrText = CString((char*)&ledger.emsg, 98);
					CString strbNext = CString((char*)&ledger.next, 1);
					CString strNextkey = CString((char*)&ledger.nkey, 18);

					//연속조회 여부 구분자를 확인해서 연속조회인 경우
					//내려받은 ledger 값을 그대로 활용하되 nkey를 내려받은 값으로 변경해서 재조회 한다
					if (strbNext == "Y")
					{
						AfxMessageBox(strErrText + "\r\n" + "연속여부 =" +  strbNext + "  \r\n 연속키 =  " + strNextkey);
			
						CString strNext = CString((char*)&ledger.nkey, sizeof(ledger.nkey));
						CopyMemory(&ledger.nkey, (LPCTSTR)strNext, sizeof(ledger.nkey));
						ledger.fkey[0] = '7';

						
						CString strUser(_T(""));
						CString strSendData;
						strSendData = CString((char*)&ledger, L_ledgerH);

						CString acc, pass;
						acc = m_pAccount->GetAccNo();
						m_pPass->GetWindowText(pass);
					//	strSendData = LedgerTR(acc.Left(3), "", "SONAQ338", GetEncPassword(pass), 1);

						struct  s_mid {
							char in[5];
							char acctNo[20];				//계좌번호
							char password[8];			//비밀번호
							char qryTp[1];					//조회구분
							char tax[1];						//수수료적용
							char stday[8];					//시작일
							char edday[8];					//종료일
							char code[12];					//종목코드
							char gubn[1];					//구분
							char ordmd[2];					//주문매체 01 ->HTS
							char srhgubn[1];				//조회처리구분
						};

						s_mid mid{};
						memset(&mid, ' ', sizeof(struct  s_mid));

						CString stemp;
						stemp = "00001";
						memcpy((char*)mid.in, stemp, stemp.GetLength());

						stemp = acc;
						memcpy((char*)mid.acctNo, stemp, stemp.GetLength());

						stemp = "HEAD";
						memcpy((char*)mid.password, stemp, stemp.GetLength());

						stemp = "1";
						memcpy((char*)mid.qryTp, stemp, stemp.GetLength());

						stemp = "20230601";
						memcpy((char*)mid.stday, stemp, stemp.GetLength());

						stemp = "20240724";
						memcpy((char*)mid.edday, stemp, stemp.GetLength());

						//stemp = "";
						//memcpy((char*)mid.code, stemp, stemp.GetLength());

						stemp = "0";
						memcpy((char*)mid.gubn, stemp, stemp.GetLength());

						stemp = "01";
						memcpy((char*)mid.ordmd, stemp, stemp.GetLength());

						stemp = "0";
						memcpy((char*)mid.srhgubn, stemp, stemp.GetLength());

						strSendData += CString((char*)&mid, sizeof(struct  s_mid));
						SendTR("pibopbxq", strSendData, TK_TRDLIST, US_KEY);
					}
				

				}
				break;
			}
		}
		break;
		case DLL_GUIDE:
		{
			CString ss;
			ss.Format("%d==%s", HIBYTE(LOWORD(wParam)), lParam);
			//AfxMessageBox(ss);
		}
		return true;
		break;
	}
	return 0;
}

LRESULT CMainWnd::OnMaketPickerMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case 100: //계좌변경시 수신
		{
			CString data = (char*)lParam;
			AfxMessageBox(data);
		}
		break;
	}
	return 0;
}

LRESULT CMainWnd::OnAccMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
		case 100: //계좌변경시 수신
		{
			CString data = (char*)lParam;
			CString acc = Parser(data, '\t');
			m_strAccName = Parser(data, '\t');
			m_strPswd = Parser(data, '\t');
			m_pPass->SetWindowText(m_strPswd);
			InvalidateRect(m_rectAccName);
		}
		break;
	}
	return 0;
}

void CMainWnd::OnButtonSend()
{//계좌 비번 검증 
	CString acc, pass;
	acc = m_pAccount->GetAccNo();
	m_pPass->GetWindowText(pass);

	CString strData = LedgerTR(acc.Left(3), "", "SACMT238", GetEncPassword(pass));

	struct _chkPwdMid mid;
	FillMemory(&mid, L_chkPwdMid, ' ');
	CString acnt = acc;

	memset(&mid, ' ', sizeof(mid));
	memcpy(mid.in, "00001", 5);
	memcpy(mid.acctNo, acnt, acnt.GetLength());
	memcpy(mid.password, "HEAD", 4);	//2013.13.23 KSJ 일방향암호화 추가

	strData += CString((char*)&mid, L_chkPwdMid);

	int nTrKey = 0;

	nTrKey = TK_CHECK207;
	SendTR("pibopbxq", strData, nTrKey, US_PASS);
}

void CMainWnd::OnButtonSymbol()
{
	CString strText;
	strText.Format("1301%c%s\t17413\t", 0x7f, "005930");
	SendTR("POOPPOOP", strText, TK_SYMBOL);
}

LRESULT CMainWnd::SendTR(CString strCode, CString strData, int iKey, int iStat)
{
	char* pcDataBuffer = new char[L_userTH + strData.GetLength()];
	memset(pcDataBuffer, ' ', L_userTH + strData.GetLength());
	struct	_userTH* puserTH;
	puserTH = (struct _userTH*)pcDataBuffer;

	memcpy(puserTH->trc, strCode.operator LPCTSTR(), strCode.GetLength());
	puserTH->key = iKey;
	puserTH->stat = iStat;

	CopyMemory(&pcDataBuffer[L_userTH], strData.operator LPCTSTR(), strData.GetLength());

	//const LRESULT lResult = m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)pcDataBuffer);
	const LRESULT lResult = m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(invokeNTXMarketTRx, strData.GetLength()), (LPARAM)pcDataBuffer);
	
	delete[] pcDataBuffer;

	return lResult;
}

CString CMainWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), m_strHome, _T("\\dev\\CX_SHA256.DLL"));
	CString strRetrun;

	if (m_hiSha256 == NULL)
	{
		m_hiSha256 = LoadLibrary(dllPath);

		if (!m_hiSha256)
		{
			TRACE("CX_SHA256 컨트롤 생성 실패1");
			return "";
		}
	}

	if (m_hiSha256)
	{
		typedef long (WINAPI* GETSHAFUNC)(char*, int);
		GETSHAFUNC func = (GETSHAFUNC)GetProcAddress(m_hiSha256, "axEncrypt");

		if (func)
		{
			strRetrun = (char*)func(sPswd.GetBuffer(sPswd.GetLength()), sPswd.GetLength());
		}


	}

	return strRetrun;
}

CString CMainWnd::LedgerTR(CString sGubn, CString sMaxRow, CString sSvcn, CString sPswd, int rcnt)
{
	CString strUser(_T(""));

	CString strReturn;

	char* pData = (char*)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		strUser = pData;
	        
	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pwndParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	if (!sSvcn.IsEmpty())
		CopyMemory(&ledger.svcd, (LPCTSTR)sSvcn, sizeof(ledger.svcd));

	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, sGubn, sGubn.GetLength());
	if (rcnt)
	{
		CString stmp;
		stmp.Format("%04d", rcnt);
		CopyMemory(&ledger.rcnt, stmp, stmp.GetLength());
	}
	else
		CopyMemory(&ledger.rcnt, _T("0001"), sizeof(ledger.rcnt));

	//2013.12.23 KSJ 일방향암호화 추가
	if (!sPswd.IsEmpty())
	{
		CopyMemory(&ledger.hsiz, "44", sizeof(ledger.hsiz));
		CopyMemory(&ledger.epwd, sPswd, sPswd.GetLength());
	}

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';

	return CString((char*)&ledger, L_ledgerH);
}

void CMainWnd::OnButtonSonaq388()
{
	CString anfname;
	anfname = "c:\\uni.ini";

	for(int ii = 0 ; ii < 5 ; ii++)
		m_pWFileSync->synWritePrivateProfileString("ANSI", "ANSI", "test", anfname);

	return;


	CString strUser(_T(""));
	CString strSendData;

	char* pData = (char*)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0L);
	if ((long)pData > 1)
		strUser = pData;

	struct _ledgerH ledger;

	FillMemory(&ledger, L_ledgerH, ' ');
	m_pwndParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	CString acc, pass;
	acc = m_pAccount->GetAccNo();
	m_pPass->GetWindowText(pass);
	strSendData = LedgerTR(acc.Left(3), "", "SONAQ338", GetEncPassword(pass));

	struct  s_mid {
		char in[5];
		char acctNo[20];				//계좌번호
		char password[8];				//비밀번호
		char Qrytp[1];					//조회구분
		char CmsnAmtAppTo[1]; //수수료적용구분
		char SrtDt[8];					//시작일
		char EndDt[8];					//종료일
		char IsuNo[12];				//종목번호
		char Tp[1];						//구분
		char CommdaCode[2];    //통신매체코드
		char QryTrxTp[1];            //조회처리구분
	};

	s_mid mid{};
	memset(&mid, ' ', sizeof(struct  s_mid));

	CString stemp;
	stemp = "00001";
	memcpy((char*)mid.in, stemp, stemp.GetLength());

	stemp = acc;
	memcpy((char*)mid.acctNo, stemp, stemp.GetLength());

	stemp = "HEAD";
	memcpy((char*)mid.password, stemp, stemp.GetLength());

	stemp = "1";
	memcpy((char*)mid.Qrytp, stemp, stemp.GetLength());

	stemp = "1";
	memcpy((char*)mid.CmsnAmtAppTo, stemp, stemp.GetLength());

	stemp = "20240101";
	memcpy((char*)mid.SrtDt, stemp, stemp.GetLength());

	stemp = "20240515";
	memcpy((char*)mid.EndDt, stemp, stemp.GetLength());

	stemp = "A005930";
	memcpy((char*)mid.IsuNo, stemp, stemp.GetLength());

	stemp = "0";
	memcpy((char*)mid.Tp, stemp, stemp.GetLength());

	stemp = "1";
	memcpy((char*)mid.CommdaCode, stemp, stemp.GetLength());

	stemp = "1";
	memcpy((char*)mid.QryTrxTp, stemp, stemp.GetLength());

	strSendData += CString((char*)&mid, sizeof(struct  s_mid));
	SendTR("pibopbxq", strSendData, TK_SONAQ388, US_KEY);
}

void CMainWnd::OnButtonSonaq428()
{
	CString strUser(_T(""));
	CString strSendData;

	CString acc, pass;
	acc = m_pAccount->GetAccNo();
	m_pPass->GetWindowText(pass);
	strSendData = LedgerTR(acc.Left(3), "", "SONAQ428", GetEncPassword(pass));

	struct  s_mid {
		char in[5];
		char acctNo[20];				//계좌번호
		char password[8];			//비밀번호
		char stday[8];					//시작일
		char edday[8]; //종료일
	};

	s_mid mid{};
	memset(&mid, ' ', sizeof(struct  s_mid));

	CString stemp;
	stemp = "00001";
	memcpy((char*)mid.in, stemp, stemp.GetLength());

	stemp = acc;
	memcpy((char*)mid.acctNo, stemp, stemp.GetLength());

	stemp = "HEAD";
	memcpy((char*)mid.password, stemp, stemp.GetLength());

	stemp = "20240101";
	memcpy((char*)mid.stday, stemp, stemp.GetLength());

	stemp = "20240528";
	memcpy((char*)mid.edday, stemp, stemp.GetLength());

	strSendData += CString((char*)&mid, sizeof(struct  s_mid));
	SendTR("pibopbxq", strSendData, TK_SONAQ428, US_KEY);
}

void CMainWnd::OnButtonSonaq429()
{
	CString strUser(_T(""));
	CString strSendData;

	CString acc, pass;
	acc = m_pAccount->GetAccNo();
	m_pPass->GetWindowText(pass);
	strSendData = LedgerTR(acc.Left(3), "", "SONAQ429", GetEncPassword(pass));

	struct  s_mid {
		char in[5];
		char acctNo[20];				//계좌번호
		char password[8];			//비밀번호
		char scode[12];					//종목코드

	};

	s_mid mid{};
	memset(&mid, ' ', sizeof(struct  s_mid));

	CString stemp;
	stemp = "00001";
	memcpy((char*)mid.in, stemp, stemp.GetLength());

	stemp = acc;
	memcpy((char*)mid.acctNo, stemp, stemp.GetLength());

	stemp = "HEAD";
	memcpy((char*)mid.password, stemp, stemp.GetLength());

	stemp = "A005930";
	memcpy((char*)mid.scode, stemp, stemp.GetLength());


	strSendData += CString((char*)&mid, sizeof(struct  s_mid));
	SendTR("pibopbxq", strSendData, TK_SONAQ429, US_KEY);
}

void CMainWnd::OnButtonpoop0200()
{
	CString strSend, stmp;
	stmp.Format("200311%c%d%c", 0x7f, 10, 0x09);
	strSend += stmp;

	stmp.Format("200312%c%0d%c", 0x7f, 1, 0x09);
	strSend += stmp;

	stmp.Format("200409%c%d%c", 0x7f, 0, 0x09);
	strSend += stmp;

	stmp.Format("200410%c%d%c", 0x7f, 0, 0x09);
	strSend += stmp;

	stmp.Format("200424%c%d%c", 0x7f, 0, 0x09);
	strSend += stmp;

	stmp.Format("200423%c%d%c", 0x7f, 0, 0x09);
	strSend += stmp;

	stmp.Format("200301%cF %c200301%c", 0x7f,  0x09, 0x09);
	strSend += stmp;

	stmp.Format("200302%c0 %c200302%c", 0x7f, 0x09, 0x09);
	strSend += stmp;

	stmp.Format("200303%c1 %c200303%c", 0x7f, 0x09, 0x09);
	strSend += stmp;

	stmp.Format("200304%c1  %c200304%c", 0x7f, 0x09, 0x09);
	strSend += stmp;

	stmp.Format("200305%c13 %c200305%c", 0x7f, 0x09, 0x09);
	strSend += stmp;

	stmp.Format("200313%c0 %c200313%c", 0x7f, 0x09, 0x09);
	strSend += stmp;

	stmp.Format("200315%c0  %c200315%c", 0x7f, 0x09, 0x09);
	strSend += stmp;

	stmp.Format("200306%c%c207300%c", 0x09, 0x24, 0xfe);
	strSend += stmp;

	stmp.Format("2020001222033", 0x09, 0x24, 0xfe);
	strSend += stmp;

	CString sdata, strResult{};
	sdata = "200311*10^200312*01^200409*0^200410*0^200424*0^200423*0^200301*F ^200301^200302* ^200302^200303* ^200303^200304*   ^200304^200305*   ^200305^200313*0^200313^200315*0  ^200315^200306^$207300*202000120                00000                                                                                1021&001&1301&1304&2023&2024&2033&2027&2101&2041&2025&2026&2061&2106&^200331*0000^200332*1^200901*001^c_upgb*001^c_upgb^200334*00^200333*1^200022^200307^200308^";

for (int ii = 0; ii < sdata.GetLength(); ii++)
{
	stmp = sdata.GetAt(ii);
	if (stmp == "^")
		stmp = "\t";
	else 	if (stmp == "&")
		stmp.Format("%c", 0x0A);
	else 	if (stmp == "*")
		stmp.Format("%c", 0x7f);

		strResult += stmp;
}


SendTR("poop0200", strResult, TK_POOP0200, US_KEY);
}
#include "History.h"
void CMainWnd::OnButtonTrdList()
{
	CString sClassName = AfxRegisterWndClass(0);
	CHistory* pWnd{};
	pWnd = new CHistory(nullptr, "000070  삼양홀딩스	005360  모나미	009520  포스코엠텍	032830  삼성생명	256840  한국비엔씨	030960  양지사	071970  HD현대마린엔진	035720  카카오	042700  한미반도체	047040  대우건설	000020  동화약품	024110  기업은행	");
	CPoint pt{};
	GetCursorPos(&pt);
	if (!((CHistory*)pWnd)->CreateEx(WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_DLGMODALFRAME, nullptr, NULL,
		WS_POPUP | WS_BORDER | WS_VISIBLE, CRect(pt.x, pt.y, pt.x + 300, pt.y + 300),
		this, NULL, NULL))
		//if (!((CHistory*)m_child)->Create( WS_VISIBLE | WS_CLIPSIBLINGS, wRc, m_view, 223)) //test codelist
	{
		return;
	}
	//pWnd->CenterWindow();
	return;
	//CString stmp;
	//stmp = (char*)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);
	//stmp = (char*)m_pwndParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC), 0);

	//return;
	CString strUser(_T(""));
	CString strSendData;

	CString acc, pass;
	acc = m_pAccount->GetAccNo();
	m_pPass->GetWindowText(pass);
	strSendData = LedgerTR(acc.Left(3), "", "SONAQ338", GetEncPassword(pass), 10);

	struct  s_mid {
		char in[5];
		char acctNo[20];				//계좌번호
		char password[8];			//비밀번호
		char qryTp[1];					//조회구분
		char tax[1];						//수수료적용
		char stday[8];					//시작일
		char edday[8];					//종료일
		char code[12];					//종목코드
		char gubn[1];					//구분
		char ordmd[2];					//주문매체 01 ->HTS
		char srhgubn[1];				//조회처리구분
	};

	s_mid mid{};
	memset(&mid, ' ', sizeof(struct  s_mid));

	CString stemp;
	stemp = "00001";
	memcpy((char*)mid.in, stemp, stemp.GetLength());

	stemp = acc;
	memcpy((char*)mid.acctNo, stemp, stemp.GetLength());

	stemp = "HEAD";
	memcpy((char*)mid.password, stemp, stemp.GetLength());

	stemp = "1";
	memcpy((char*)mid.qryTp, stemp, stemp.GetLength());

	stemp = "20230601";
	memcpy((char*)mid.stday, stemp, stemp.GetLength());

	stemp = "20240724";
	memcpy((char*)mid.edday, stemp, stemp.GetLength());

	//stemp = "";
	//memcpy((char*)mid.code, stemp, stemp.GetLength());

	stemp = "0";
	memcpy((char*)mid.gubn, stemp, stemp.GetLength());

	stemp = "01";
	memcpy((char*)mid.ordmd, stemp, stemp.GetLength());

	stemp = "0";
	memcpy((char*)mid.srhgubn, stemp, stemp.GetLength());

	strSendData += CString((char*)&mid, sizeof(struct  s_mid));
	SendTR("pibopbxq", strSendData, TK_TRDLIST, US_KEY);
}


#define MMSG_SHARED_REGWND		0x10
#define MMSG_SHARED_BROADCAST		0x11
#define MMSG_SHARED_CTRLDESTROY		0x12
#define MMSG_SHARED_GETHANDLECNT		0x13
void CMainWnd::OnButtonRegWnd()
{
	AfxGetMainWnd()->SendMessage(WM_USER, MMSG_SHARED_REGWND, (LPARAM)(LPCSTR)this);
}

void CMainWnd::OnButtonBroadcastWnd()
{
	AfxGetMainWnd()->SendMessage(WM_USER, MMSG_SHARED_CTRLDESTROY, (LPARAM)(LPCSTR)this);
}

void CMainWnd::OnButtonUnRegWnd()
{
	CString sVal;
	sVal = "test 브로드 \t !@# \t 다시";
	AfxGetMainWnd()->SendMessage(WM_USER, MMSG_SHARED_BROADCAST, (LPARAM)(LPCSTR)sVal);
	
}

void CMainWnd::OnButtonGetWndCnt()
{
	int ret = (int)AfxGetMainWnd()->SendMessage(WM_USER, MMSG_SHARED_GETHANDLECNT, (LPARAM)(LPCSTR)this);
	CString sVal;
	sVal.Format("%d 개", ret);
	AfxMessageBox(sVal);
}

#include "CDlg_Top.h"
void CMainWnd::OnButtonTopDlg()
{
	CDlg_Top dlg;
	dlg.m_path = m_strHome;
	dlg.DoModal();
}