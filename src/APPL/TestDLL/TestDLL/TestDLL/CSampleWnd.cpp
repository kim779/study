// CSampleWnd.cpp: 구현 파일
//

#include "pch.h"
#include "TestDLL.h"
#include "CSampleWnd.h"
#include "ContainerWnd.h"


#include "axisfire.h"
#include "UserDefine.h"		// TK_SYMBOL
// CSampleWnd

#define KEY_TR_OOP  128

IMPLEMENT_DYNAMIC(CSampleWnd, CWnd)

CString Parse(CString& strSrc, char cDel)
{
	CString strReturn;
	strReturn.Empty();
	if (strSrc.Find(cDel) < 0)
	{
		strReturn = strSrc;
		strSrc.Empty();
	}
	else
	{
		strReturn = strSrc.Left(strSrc.Find(cDel));
		strSrc = strSrc.Mid(strSrc.Find(cDel) + 1);

	}
	return strReturn;
}

CSampleWnd::CSampleWnd(CWnd* pwndParent)
{
	m_pWizard = pwndParent;
}

CSampleWnd::~CSampleWnd()
{
	if (m_pMarketpicker)
	{
		m_pMarketpicker->DestroyWindow();
		delete m_pMarketpicker;
	}
	// cleanup mirrors IB100300(운영)\MapWnd.cpp's own m_pwndSymbol teardown
	if (m_pwndSymbol)
	{
		if (IsWindow(m_pwndSymbol->GetSafeHwnd()))
			m_pwndSymbol->SendMessage(WM_CLOSE);
		m_pwndSymbol->Detach();
		delete m_pwndSymbol;
	}
	if (m_hiSymbol)
	{
		AfxFreeLibrary(m_hiSymbol);
		m_hiSymbol = NULL;
	}
	if (m_pwndCodeCtrl)
	{
		if (IsWindow(m_pwndCodeCtrl->GetSafeHwnd()))
			m_pwndCodeCtrl->SendMessage(WM_CLOSE);
		m_pwndCodeCtrl->Detach();
		delete m_pwndCodeCtrl;
	}
	if (m_hiCodeCtrl)
	{
		AfxFreeLibrary(m_hiCodeCtrl);
		m_hiCodeCtrl = NULL;
	}
}

BEGIN_MESSAGE_MAP(CSampleWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_PAINT()
	
	ON_BN_CLICKED(9999, OnButtonKRX)
	ON_BN_CLICKED(9998, OnButtonNXT)
	ON_BN_CLICKED(9997, OnButtonTOT)
	ON_BN_CLICKED(9996, OnButtonGetMarket)
	ON_BN_CLICKED(9995, OnButtonSendOOP)
	ON_BN_CLICKED(9994, OnButtonSymbol)
	ON_BN_CLICKED(9993, OnButtonCodeGet)
	ON_MESSAGE(WM_USER, OnMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CSampleWnd 메시지 처리기
LRESULT CSampleWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
		case formDLL:
		{
			
		}
		break;
		case DLL_TRIGGER:
		{
m_slog.Format("[TRIGGER] %s", (char*)lParam);
OutputDebugString(m_slog);
			if (m_slog.Find("edMarketTrigger") >= 0)
			{
				CString sTriggerKey, sMarkset;
				sTriggerKey = Parse(m_slog, '\t');
				sMarkset = m_slog;

				m_slog.Format("   메인으로 부터 장구분 메시지 \n트리거 구분자[%s]    \n거래소[%s]", sTriggerKey, sMarkset);
				AfxMessageBox(m_slog);

				if (sMarkset == "KRX")
					m_iMarket = 1;
				if (sMarkset == "NXT")
					m_iMarket = 2;
				if (sMarkset == "통합")
					m_iMarket = 3;
			}
		}
		break;
		case DLL_ALERTx:
		{
		
		}
		break;
		case DLL_OUB:
		{
			int key = HIBYTE(LOWORD(wParam));

			switch (key)
			{
				case KEY_TR_OOP:
				{
					CString sRec{};
					sRec.Format("%s", (char*)lParam);
					sRec.Replace("\t", "\n");
					CString scode;
					scode = Parser(sRec, '\n');
					_variant_t var((LPCTSTR)scode);
					m_pMarketpicker->SetCtrlProperty("sCode", var);
					AfxMessageBox(sRec);
				}
				break;
				case TK_SYMBOL:
					// forward the server response into the cx_symbol control, same as CMainWnd.cpp::OnMessage
					if (m_pwndSymbol)
						m_pwndSymbol->SendMessage(WM_USER, wParam, lParam);
					break;
			}
			break;
		}
		break;
		case DLL_GUIDE:
		{
			CString ss;
			ss.Format("%d==%s", HIBYTE(LOWORD(wParam)), lParam);
		}
		return true;
		break;
		}
	return 0;
}

int CSampleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_strHome = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC), 0);

	int ictrl_width = 18;
	int ictrl_height = 20;
	CRect cRc{};
	cRc.SetRect(10, 10, 10 + ictrl_width, 10 + ictrl_height);
	
	m_pMarketpicker = (CContainerWnd*) new CContainerWnd;
	m_pMarketpicker->SetParent(m_pWizard);
	m_pMarketpicker->Create(NULL, NULL, WS_CHILDWINDOW | WS_VISIBLE, cRc, this, -1);
	m_pMarketpicker->CreateControl(m_strHome, "CX_MarketPicker", "컨트롤이름", cRc, "/k2");



	SetTimer(9898, 100, nullptr);


	cRc.OffsetRect(400, 0);

	const int ibtnw= 200;
	cRc.SetRect(cRc.right + 10, 10, cRc.right + 10 + ibtnw, 10 + ictrl_height);
	m_pBtnSetMarket_KRX  = new CButton;
	if (m_pBtnSetMarket_KRX)
	{
		m_pBtnSetMarket_KRX->Create(_T("KRX->SetCtrlProperty"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cRc, this, 9999);
	}

	cRc.OffsetRect(0, 50);
	m_pBtnSetMarket_NXT = new CButton;
	if (m_pBtnSetMarket_NXT)
	{
		m_pBtnSetMarket_NXT->Create(_T("NXT->SetCtrlProperty"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cRc, this, 9998);
	}

	cRc.OffsetRect(0, 50);
	m_pBtnSetMarket_TOT = new CButton;
	if (m_pBtnSetMarket_TOT)
	{
		m_pBtnSetMarket_TOT->Create(_T(" 통합->SetCtrlProperty"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cRc, this, 9997);
	}

	cRc.OffsetRect(0, 50);
	m_pBtnGSetMarket = new CButton;
	if (m_pBtnGSetMarket)
	{
		m_pBtnGSetMarket->Create(_T("거래소정보->GetCtrlProperty"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cRc, this, 9996);
	}


	cRc.OffsetRect(0, 50);
	m_pBtnTR_OOP = new CButton;
	if (m_pBtnTR_OOP)
	{
		m_pBtnTR_OOP->Create(_T("조회"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			cRc, this, 9995);
	}
	return 0;
}

void CSampleWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	CRect rec;
	GetClientRect(&rec);
	dc.FillSolidRect(rec, RGB(233, 246,253));
}

void CSampleWnd::OnButtonKRX()
{
	CString str = _T("KRX");
	_variant_t var((LPCTSTR)str);
	AfxMessageBox("컨트롤을 KRX로 변경");
	m_pMarketpicker->SetCtrlProperty("sMarket", var);
	m_iMarket = 1;

}

void CSampleWnd::OnButtonNXT()
{
	CString str = _T("NXT");
	_variant_t var((LPCTSTR)str);
	AfxMessageBox("컨트롤을 NXT로 변경");
	m_pMarketpicker->SetCtrlProperty("sMarket", var);
	m_iMarket = 2;
}

void CSampleWnd::OnButtonTOT()
{
	CString str = _T("통합");
	_variant_t var((LPCTSTR)str);
	AfxMessageBox("컨트롤을 통합으로 변경");
	m_pMarketpicker->SetCtrlProperty("sMarket", var);
	m_iMarket = 3;
}

void CSampleWnd::OnButtonGetMarket() 
{
	CString str;
	str = m_pMarketpicker->GetCtrlProperty("sMarket");
	AfxMessageBox(str);
}

void CSampleWnd::OnButtonSendOOP()
{
	CString strText;
	CString str;
	str = m_pMarketpicker->GetCtrlProperty("sMarket");
	strText.Format("1301%c%s\t1777%c%d\t1021\t2023\t3149\t3147\t3146\t3148\t3150\t3181\t", 0x7f, "005930", 0x7f, str == "통합" ? 3 : str == "KRX" ? 1 : 2);
	SendTR("POOPPOOP", strText, KEY_TR_OOP);
}

// 1777(시장구분) 값을 종목코드로 추측하지 않고, 화면에 이미 떠 있는 마켓피커 컨트롤에서
// 그대로 물어본다 - OnButtonKRX/NXT/TOT가 SetCtrlProperty("sMarket",...)로 바꿔주는 바로 그 값.
int CSampleWnd::GetMarketGubn()
{
	if (!m_pMarketpicker)
		return 1;	// KRX (default)

	CString sMarket = m_pMarketpicker->GetCtrlProperty("sMarket");
	if (sMarket == "NXT")
		return 2;
	if (sMarket == "통합")
		return 3;
	return 1;		// KRX
}

void CSampleWnd::OnButtonSymbol()
{
	// same request CMainWnd.cpp::OnButtonSymbol() sends, but now with the 1777 시장구분 field added -
	// response comes back tagged TK_SYMBOL and OnMessage()'s DLL_OUB/TK_SYMBOL case forwards it into m_pwndSymbol.
	CString strText;
	CString str;
	str = m_pMarketpicker->GetCtrlProperty("sMarket");
	strText.Format("1301%c%s\t1777%c%d\t17413\t", 0x7f, "005930", 0x7f, str == "통합" ? 3 : str == "KRX" ? 1 : 2);
	SendTR("POOPPOOP", strText, TK_SYMBOL);
}

// cx_codectrl exposes a COM property "Data" (DISP_PROPERTY_EX in cx_codectrl(운영)\ControlWnd.cpp)
// that holds the currently typed/selected code - same GetIDispatch()+CComDispatchDriver technique
// as ContainerWnd::GetCtrlProperty() uses for the marketpicker's "sMarket" property.
CString CSampleWnd::GetCodeCtrlData()
{
	if (!m_pwndCodeCtrl || !m_pwndCodeCtrl->GetSafeHwnd())
		return "";

	IDispatch* pDisp = m_pwndCodeCtrl->GetIDispatch(FALSE);
	if (!pDisp)
		return "";

	_variant_t var;
	CComDispatchDriver driver(pDisp);
	driver.GetPropertyByName(_bstr_t("Data"), &var);
	return (LPCSTR)(_bstr_t)var;
}

void CSampleWnd::OnButtonCodeGet()
{
	if (!m_pwndCodeCtrl || !m_pwndCodeCtrl->GetSafeHwnd())
	{
		AfxMessageBox("cx_codectrl 컨트롤이 아직 없습니다.");
		return;
	}
	CString sCode = GetCodeCtrlData();
	AfxMessageBox("현재 종목코드: [" + sCode + "]");
}

LRESULT CSampleWnd::SendTR(CString strCode, CString strData, int iKey, int iStat)
{
	char* pcDataBuffer = new char[L_userTH + strData.GetLength()];
	memset(pcDataBuffer, ' ', L_userTH + strData.GetLength());
	struct	_userTH* puserTH;
	puserTH = (struct _userTH*)pcDataBuffer;

	memcpy(puserTH->trc, strCode.operator LPCTSTR(), strCode.GetLength());
	puserTH->key = iKey;
	puserTH->stat = iStat;

	CopyMemory(&pcDataBuffer[L_userTH], strData.operator LPCTSTR(), strData.GetLength());

	const LRESULT lResult = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, strData.GetLength()), (LPARAM)pcDataBuffer);

	delete[] pcDataBuffer;

	return lResult;
}


void CSampleWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case 9898:
		{
			KillTimer(nIDEvent);

			CRect cRc;
			m_pMarketpicker->GetWindowRect(cRc);
			ScreenToClient(cRc);
			cRc.OffsetRect(cRc.Width() + 5, 0);

			// cx_symbol : direct LoadLibrary + axCreate, same pattern as CMainWnd.cpp::InitCtrl()
			// (no CContainerWnd wrapper - matches how CMainWnd/IB100300 do it)
			// size matches the real production usage in IB100300(운영)\MapWnd.cpp::InitSymbol()
			// (width 110 / height CTRL_HEIGHT+1 = 21)
			CString text;
			text.Format("%s\\dev\\cx_symbol.dll", m_strHome);
			m_hiSymbol = AfxLoadLibrary(text);
			if (m_hiSymbol == NULL)
			{
				AfxMessageBox("cx_symbol.dll 로드 실패\n\n확인한 경로: " + text);
			}
			else
			{
				CWnd* (APIENTRY* axCreateSym)(CWnd*, void*) = NULL;
				axCreateSym = (CWnd * (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiSymbol, "axCreate");
				if (axCreateSym == NULL)
				{
					AfxMessageBox("cx_symbol.dll 에서 axCreate 를 찾지 못했습니다.");
					AfxFreeLibrary(m_hiSymbol);
					m_hiSymbol = NULL;
				}
				else
				{
					const int iSymbolWidth = 110;
					const int iSymbolHeight = 21;
					cRc.SetRect(cRc.left, 10, cRc.left + iSymbolWidth, 10 + iSymbolHeight);

					struct _param symbolparam;
					symbolparam.key = 0;
					symbolparam.name = _T("17413");
					symbolparam.rect = cRc;
					symbolparam.fonts = "굴림체";
					symbolparam.point = 9;
					symbolparam.style = 1;
					symbolparam.tRGB = 69;
					symbolparam.pRGB = 90;
					symbolparam.options = _T("/a89/b91/c92/d69/i99/s1003");

					m_pwndSymbol = (*axCreateSym)(m_pWizard, &symbolparam);
					if (m_pwndSymbol == NULL)
					{
						AfxMessageBox("cx_symbol 컨트롤 생성 실패");
						AfxFreeLibrary(m_hiSymbol);
						m_hiSymbol = NULL;
					}
					else
					{
						m_pwndSymbol->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

						// wiring sample: button below cx_symbol, mirrors CMainWnd.cpp's m_pBtnSymbol/OnButtonSymbol()
						CRect rcBtn(cRc.left, cRc.bottom + 5, cRc.left + 60, cRc.bottom + 5 + CTRL_HEIGHT);
						m_pBtnSymbol = new CButton;
						m_pBtnSymbol->Create(_T("심볼"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rcBtn, this, 9994);
					}
				}
			}

			// cx_codectrl : same direct pattern.
			// option string / size copied from the working production usage in IB201600(운영)\CodeCtrl.cpp
			text.Format("%s\\dev\\cx_codectrl.dll", m_strHome);
			m_hiCodeCtrl = AfxLoadLibrary(text);
			if (m_hiCodeCtrl == NULL)
			{
				AfxMessageBox("cx_codectrl.dll 로드 실패\n\n확인한 경로: " + text);
			}
			else
			{
				CWnd* (APIENTRY* axCreateCode)(CWnd*, void*) = NULL;
				axCreateCode = (CWnd * (APIENTRY*)(CWnd*, void*))GetProcAddress(m_hiCodeCtrl, "axCreate");
				if (axCreateCode == NULL)
				{
					AfxMessageBox("cx_codectrl.dll 에서 axCreate 를 찾지 못했습니다.");
					AfxFreeLibrary(m_hiCodeCtrl);
					m_hiCodeCtrl = NULL;
				}
				else
				{
					const int iCodeWidth = 80;
					const int iCodeHeight = 18;
					CRect cRcCode;
					if (m_pwndSymbol)
					{
						m_pwndSymbol->GetWindowRect(cRcCode);
						ScreenToClient(cRcCode);
						cRcCode.SetRect(cRcCode.right + 6, 10, cRcCode.right + 6 + iCodeWidth, 10 + iCodeHeight);
					}
					else
					{
						cRcCode.SetRect(cRc.right + 6, 10, cRc.right + 6 + iCodeWidth, 10 + iCodeHeight);
					}

					struct _param codeparam;
					codeparam.key = 0;
					codeparam.name = _T("CodeCtrl");
					codeparam.rect = cRcCode;
					codeparam.fonts = "굴림체";
					codeparam.point = 9;
					codeparam.style = 0;
					codeparam.tRGB = 63;
					codeparam.pRGB = 90;
					codeparam.options = "/u 4 /k 검색.BMP /l AXCOMBO.BMP /p 3 /s 0 /o True /q False";

					m_pwndCodeCtrl = (*axCreateCode)(m_pWizard, &codeparam);
					if (m_pwndCodeCtrl == NULL)
					{
						AfxMessageBox("cx_codectrl 컨트롤 생성 실패");
						AfxFreeLibrary(m_hiCodeCtrl);
						m_hiCodeCtrl = NULL;
					}
					else
					{
						m_pwndCodeCtrl->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

						// wiring sample: button below cx_codectrl, reads its "Data" COM property
						CRect rcBtn(cRcCode.left, cRcCode.bottom + 5, cRcCode.left + 60, cRcCode.bottom + 5 + CTRL_HEIGHT);
						m_pBtnCodeGet = new CButton;
						m_pBtnCodeGet->Create(_T("코드조회"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rcBtn, this, 9993);
					}
				}
			}
		}
		break;
	}
	CWnd::OnTimer(nIDEvent);
}
