#include "stdafx.h"
#include "IBKS_Sample.h"
#include "IBKS_SampleDlg.h"
#include "Open_API_OUT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CIBKS_SampleDlg::CIBKS_SampleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIBKS_SampleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIBKS_SampleDlg)
	m_CertPW = _T("");
	m_UserID = _T("");
	m_UserPW = _T("");
	m_odrAccn = _T("");
	m_odrPswd = _T("");
	m_odrCode = _T("");
	m_odrHogb = _T("");
	m_odrJprc = _T("");
	m_odrJqty = _T("");
	m_odrMmgb = _T("");
	m_odrOgno = _T("0");
	m_siseCode = _T("");
	m_jngoAccn = _T("");
	m_jngoPswd = _T("");
//	m_ServerIP = _T("211.255.204.137");
	m_ServerIP = _T("172.16.202.106");
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bGreeks1 = FALSE;
	m_bGreeks2 = FALSE;
}

void CIBKS_SampleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIBKS_SampleDlg)
	DDX_Control(pDX, IDC_COMBO2, m_cbJType);
	DDX_Control(pDX, IDC_JNGO_ACCN, m_cbAccn2);
	DDX_Control(pDX, IDC_JNGOLIST, m_lbxJngo);
	DDX_Control(pDX, IDC_CHEGLIST, m_cbCheg);
	DDX_Control(pDX, IDC_ODR_RD, m_edRdResult);
	DDX_Control(pDX, IDC_ODR_RESULT, m_edResult);
	DDX_Control(pDX, IDC_ODR_MMGB, m_cbMmgb);
	DDX_Control(pDX, IDC_ODR_HOGB, m_cbHogb);
	DDX_Control(pDX, IDC_ODR_ACCN, m_cbAccn);
	DDX_Text(pDX, IDC_CERTPW, m_CertPW);
	DDV_MaxChars(pDX, m_CertPW, 30);
	DDX_Text(pDX, IDC_USERID, m_UserID);
	DDV_MaxChars(pDX, m_UserID, 12);
	DDX_Text(pDX, IDC_USERPW, m_UserPW);
	DDV_MaxChars(pDX, m_UserPW, 12);
	DDX_Control(pDX, IDC_CONN, m_axIBKS);
	DDX_CBString(pDX, IDC_ODR_ACCN, m_odrAccn);
	DDX_Text(pDX, IDC_ODR_ACPW, m_odrPswd);
	DDX_Text(pDX, IDC_ODR_CODE, m_odrCode);
	DDV_MaxChars(pDX, m_odrCode, 8);
	DDX_CBString(pDX, IDC_ODR_HOGB, m_odrHogb);
	DDX_Text(pDX, IDC_ODR_JPRC, m_odrJprc);
	DDX_Text(pDX, IDC_ODR_JQTY, m_odrJqty);
	DDX_CBString(pDX, IDC_ODR_MMGB, m_odrMmgb);
	DDX_Text(pDX, IDC_ODR_OGNO, m_odrOgno);
	DDX_Text(pDX, IDC_SISE_CODE, m_siseCode);
	DDV_MaxChars(pDX, m_siseCode, 8);
	DDX_CBString(pDX, IDC_JNGO_ACCN, m_jngoAccn);
	DDX_Text(pDX, IDC_JNGO_ACPW, m_jngoPswd);
	DDX_Text(pDX, IDC_EDIT1, m_ServerIP);
	DDV_MaxChars(pDX, m_ServerIP, 20);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIBKS_SampleDlg, CDialog)
	//{{AFX_MSG_MAP(CIBKS_SampleDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, OnBtnConnect)
	ON_BN_CLICKED(IDC_ODR_SEND, OnOdrSend)
	ON_BN_CLICKED(IDC_SISE_SEND, OnSiseSend)
	ON_BN_CLICKED(IDC_JNGO_SEND, OnJngoSend)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, OnBtnDisconnect)
	ON_BN_CLICKED(IDC_JNGO_SEND2, OnJngoSend2)
	ON_BN_CLICKED(IDC_SISE_SEND2, OnSiseSend2)
	ON_BN_CLICKED(IDC_SISE_SEND3, OnSiseSend3)
	ON_BN_CLICKED(IDC_SISE_SEND4, OnSiseSend4)
	ON_BN_CLICKED(IDC_SISE_SEND5, OnSiseSend5)
	ON_BN_CLICKED(IDC_GREEKS, OnGreeks)
	ON_BN_CLICKED(IDC_SONIK, OnSonIk)
	ON_BN_CLICKED(IDC_MON_SISE, OnMonSise)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OOP, &CIBKS_SampleDlg::OnBnClickedBtnOop)
	ON_BN_CLICKED(IDC_TEST, &CIBKS_SampleDlg::OnBnClickedTest)
END_MESSAGE_MAP()

BOOL CIBKS_SampleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_cbJType.SetCurSel(0);
	// 반드시 초기화 후 사용합니다.
	if (!m_axIBKS.Initialize())
	{
		CString stmp;
		stmp.Format("%d", GetLastError());
		MessageBox(m_axIBKS.GetLastErrMsg(), "초기화오류");
		OnCancel();
	}
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CIBKS_SampleDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CIBKS_SampleDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIBKS_SampleDlg::OnBtnConnect() 
{
	UpdateData(TRUE);

	if (m_UserID.IsEmpty())
	{
		MessageBox("아이디를 입력하세요", "IBK투자증권");
		GetDlgItem(IDC_USERID)->SetFocus();
		return;
	}

	if (m_UserPW.IsEmpty())
	{
		MessageBox("접속비번을 입력하세요", "IBK투자증권");
		GetDlgItem(IDC_USERPW)->SetFocus();
		return;
	}

	if (m_CertPW.IsEmpty())
	{
		MessageBox("인증서비번을 입력하세요", "IBK투자증권");
		GetDlgItem(IDC_CERTPW)->SetFocus();
		return;
	}

	// 테스트서버 : 211.255.204.98

	if(m_ServerIP.IsEmpty())
	{
		if (!m_axIBKS.Login(m_UserID, m_UserPW, m_CertPW, "", 15201))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "로그인 에러", MB_OK|MB_ICONERROR);
		}
	}
	else
	{
		if (!m_axIBKS.Login(m_UserID, m_UserPW, m_CertPW, m_ServerIP, 15201))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "로그인 에러", MB_OK|MB_ICONERROR);
		}
	}

}

BEGIN_EVENTSINK_MAP(CIBKS_SampleDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CIBKS_SampleDlg)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 1 /* OnLogin */, OnEvtLogin, VTS_BOOL VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 2 /* OnContract */, OnEvtContract, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 3 /* OnRealData */, OnEvtRealData, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 4 /* OnRecvData */, OnEvtRecvData, VTS_I4 VTS_I4 VTS_I4 VTS_BOOL VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 5 /* OnError */, OnEvtError, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 6 /* OnClose */, OnEvtClose, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 7 /* OnAlert */, OnEvtAlert, VTS_I4 VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 9 /* OnVerUpdate */, OnEvtVerUpdate, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 10 /* OnUpdateStart */, OnEvtUpdateStart, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 11 /* OnUpdating */, OnEvtUpdating, VTS_BSTR VTS_I4)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 12 /* OnUpdateEnd */, OnEvtUpdateEnd, VTS_NONE)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 13 /* OnGuideMsg */, OnEvtGuideMsg, VTS_I4 VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 14 /* OnFBalance */, OnEvtFBalance, VTS_BSTR)
	ON_EVENT(CIBKS_SampleDlg, IDC_CONN, 15 /* OnSBalance */, OnEvtSBalance, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CIBKS_SampleDlg::OnEvtLogin(BOOL bLogin, LPCTSTR msg) 
{
	MessageBox(msg, "로그인 결과", bLogin ? MB_OK : MB_OK|MB_ICONERROR);

	if (bLogin)
	{
		// 계좌번호 로드
		m_cbAccn.ResetContent();
		m_cbAccn2.ResetContent();
		{
			// 계좌번호\t계좌명\n계좌번호\t계좌명 형식
			CString strSt = m_axIBKS.GetAccounts();
			LPCSTR ed = (LPCSTR)strSt + strSt.GetLength();
			
			vector<StringProxy> vAccn;
			ParseString((LPCSTR)strSt, ed, '\n', vAccn);
			for(int n=0; n<vAccn.size(); ++n)
			{
				m_cbAccn.AddString( vAccn[n].ToCStr() );
				m_cbAccn2.AddString( vAccn[n].ToCStr() );
			}

			if (!vAccn.empty())
			{
				m_cbAccn.SetCurSel(0);
				m_cbAccn2.SetCurSel(0);
			}
		}

		// 구분값 세팅
		m_cbHogb.ResetContent();
		{
			m_cbHogb.AddString("00 지정가");		m_cbHogb.SetItemData(0, 0);
			m_cbHogb.AddString("03 시장가");		m_cbHogb.SetItemData(1, 3);
			m_cbHogb.AddString("05 조건부");		m_cbHogb.SetItemData(2, 5);
			m_cbHogb.AddString("06 최유리");		m_cbHogb.SetItemData(3, 6);
			m_cbHogb.AddString("10 지정가(IOC)");	m_cbHogb.SetItemData(4,10);
			m_cbHogb.AddString("20 지정가(FOK)");	m_cbHogb.SetItemData(5,20);
			m_cbHogb.SetCurSel(0);
		}

		// 매매구분 세팅
		m_cbMmgb.ResetContent();
		{
			m_cbMmgb.AddString("1 매도");	m_cbMmgb.SetItemData(0, 1);
			m_cbMmgb.AddString("2 매수");	m_cbMmgb.SetItemData(1, 2);
			m_cbMmgb.AddString("3 정정");	m_cbMmgb.SetItemData(2, 3);
			m_cbMmgb.AddString("4 취소");	m_cbMmgb.SetItemData(3, 4);
			m_cbMmgb.SetCurSel(0);
		}
	}
}

void CIBKS_SampleDlg::OnEvtContract(LPCTSTR data) 
{
	LPCTSTR pos;
	LPCTSTR st = data;
	LPCTSTR ed = data + strlen(data);
	op_notice_spliter op;
	
	// 데이터의 자세한 형식은 메뉴얼을 참조하세요 \r \t \n 구분자.
	StringProxy key, val;
	CMapStringToString map;
	while(st<ed)
	{
		pos = find_if(st, ed, op);
		key = StringProxy(st, pos);
		st = ++pos;

		pos = find_if(st, ed, op);
		val = StringProxy(st, pos);
		st = ++pos;
		
		map[key.ToCStr()] = val.ToCStr();
	}

	// 결과메세지 출력
	CString strRet, strText;

// 	strRet.Format("구분[%s] 계좌[%s] 종목[%s] 매매[%s] 가격[%s] 구분[%s] 주문번호[%s] 원주문[%s] 사용자정의ID[%s]\r\n",
// 		map["988"], map["901"], map["907"], map["912"], map["916"], map["925"], map["904"], map["905"], map["974"]);

	strRet.Format("구분[%s] 계좌[%s] 종목[%s] 매매[%s] 가격[%s] 구분[%s] 주문번호[%s] 원주문[%s] 주문수량[%s] 체결수량[%s] 미체결수량[%s]\r\n",
		map["988"], map["901"], map["907"], map["912"], map["916"], map["925"], map["904"], map["905"], map["974"], map["909"], map["931"], map["921"]);

	m_edRdResult.GetWindowText(strText);
	m_edRdResult.SetWindowText(strRet + strText);
	m_edResult.SetWindowText(data);
}

void CIBKS_SampleDlg::OnEvtRealData(LPCTSTR data) 
{
	LPCTSTR pos;
	LPCTSTR st = data;
	LPCTSTR ed = data + strlen(data);
	StringProxy code, key, val;
	CMapStringToString map;

	CString strCode;
	// 데이터의 자세한 형식은 메뉴얼을 참조하세요 \t 구분자.

	pos = find(st, ed, '\t');
	code = StringProxy(st, pos);
	st = ++pos;

	strCode = code.ToCStr();

	if(strCode.GetAt(0) == 'A')
	{
		strCode.Delete(0);
	}
	
	// 현재 조회된 종목만 시세처리한다.

	if (strCode != m_siseCode)return;

	while(st<ed)
	{
		pos = find(st, ed, '\t');
		key = StringProxy(st, pos);
		st = ++pos;
		
		pos = find(st, ed, '\t');
		val = StringProxy(st, pos);
		st = ++pos;
		
		map[key.ToCStr()] = val.ToCStr();
	}

	{
		CString key, val;

		if (map.Lookup("034", val))	// 체결시간이 있으면 체결데이터
		{
			GetDlgItem(IDC_CURR)->SetWindowText( map["023"] );	// 현재가
			GetDlgItem(IDC_DIFF)->SetWindowText( map["024"] );	// 전일대비
			GetDlgItem(IDC_RATE)->SetWindowText( map["033"] );	// 등락률
			GetDlgItem(IDC_GVOL)->SetWindowText( map["027"] );	// 거래량
			GetDlgItem(IDC_MGJV)->SetWindowText( map["201"] );	// 미결약정
			GetDlgItem(IDC_SIGA)->SetWindowText( map["029"] );	// 시가
			GetDlgItem(IDC_KOGA)->SetWindowText( map["030"] );	// 고가
			GetDlgItem(IDC_JEGA)->SetWindowText( map["031"] );	// 저가
			//전일종가 = 현재가 - 전일대비
			CString strJongga, strCurr;

			strCurr = map["023"];

			OutputDebugString("[KSJ] Real diff[" + map["024"] + "]");

			if(strCurr.GetAt(0) == '-')
			{
				strCurr.Delete(0);

				strJongga.Format("%8.2lf", atof(strCurr) - atof(map["024"]));

				strJongga.TrimLeft(); strJongga.TrimRight();
				strJongga.Insert(0, "-");

				GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// 전일종가
			}
			else
			{
				strJongga.Format("%8.2lf", atof(strCurr) - atof(map["024"]));
				GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// 전일종가
			}

			// 체결량에 부호가 안오면 첫번째 숫자는 체결강도 표시이므로 버리세요.
			LPCSTR cvol = map["032"];
			if (cvol[0]!='+' && cvol[0]!='-') ++cvol;

			// 체결내역 갱신 ( 체결시간/체결가/체결량/K200지수 )
			CString line;
			line.Format("%s | %6s | %5s | %6s", val, map["023"], cvol, map["002"]);
			m_cbCheg.InsertString(0, line);
		}
		if (map.Lookup("040", val))	// 호가시간이 있으면 호가데이터
		{
			GetDlgItem(IDC_MDVOL)->SetWindowText( map["101"] );	// 매도잔량
			GetDlgItem(IDC_MSVOL)->SetWindowText( map["106"] );	// 매수잔량
		}
	}
}

void CIBKS_SampleDlg::OnEvtRecvData(long key, long dptr, long size, BOOL bNext, LPCTSTR nkey) 
{
	CString emsg;
	if (key == TK_TR1231)
	{
		struct tr1231_mod* pmod;
		CString sRes;
		sRes.Format("예수금 [%s]", pmod->lDps);
		AfxMessageBox(sRes);
		return;
	}

	if (key == 49)
	{
		CString str;
		str.Format("%s", (char*)dptr);
		AfxMessageBox(str);
		return;
	}

	if (key==TK_TR3201)
	{
		struct tr3201_mod *mod = (struct tr3201_mod*)dptr;
		emsg.Format("주문번호[%.6s] 원주문번호[%.6s] 결과[%.80s]", mod->jmno, mod->ojno, mod->emsg);
		m_edResult.SetWindowText(emsg);
	}
	else if (key==TK_TR3001)
	{
		struct tr3001_mod *mod = (struct tr3001_mod*)dptr;
		
		GetDlgItem(IDC_CURR)->SetWindowText( STR(mod->curr) );
		GetDlgItem(IDC_DIFF)->SetWindowText( STR(mod->diff) );
		GetDlgItem(IDC_RATE)->SetWindowText( STR(mod->rate) );
		GetDlgItem(IDC_GVOL)->SetWindowText( STR(mod->gvol) );
		GetDlgItem(IDC_MGJV)->SetWindowText( STR(mod->mgjv) );
		GetDlgItem(IDC_MDVOL)->SetWindowText( STR(mod->dvol) );
		GetDlgItem(IDC_MSVOL)->SetWindowText( STR(mod->svol) );
		GetDlgItem(IDC_SIGA)->SetWindowText( STR(mod->siga) );
		GetDlgItem(IDC_KOGA)->SetWindowText( STR(mod->koga) );
		GetDlgItem(IDC_JEGA)->SetWindowText( STR(mod->jega) );

		//전일종가 = 현재가 - 전일대비
		CString strJongga, strCurr, strDiff;
		
		strCurr = STR(mod->curr);
		strDiff = STR(mod->diff);
		
		OutputDebugString("[KSJ] 3001 diff[" + strDiff + "]");
		if(strCurr.GetAt(0) == '-')
		{
			strCurr.Delete(0);
			
			strJongga.Format("%8.2lf", atof(strCurr) - atof(STR(mod->diff)));
			
			strJongga.TrimLeft(); strJongga.TrimRight();
			strJongga.Insert(0, "-");
			
			GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// 전일종가
		}
		else
		{
			strJongga.Format("%8.2lf", atof(strCurr) - atof(STR(mod->diff)));
			GetDlgItem(IDC_JONGGA)->SetWindowText( strJongga );	// 전일종가
		}
	}
	else if (key==TK_TR3221)
	{
		struct tr3221_mod *mod = (struct tr3221_mod*)dptr;
		
		int nrec = _ttoi( STR(mod->nrec) );
		for(int n=0; n<nrec; ++n)
		{
			CString code = STR(mod->grid[n].cod2);	code.TrimRight();
			CString dsgb = STR(mod->grid[n].dsgb);	dsgb.TrimRight();
			int jqty = _ttoi(STR(mod->grid[n].jqty));	// 보유수량
			int xqty = _ttoi(STR(mod->grid[n].xqty));	// 청산가능수량
			double pamt = atof(STR(mod->grid[n].pamt));	// 평균가
			double tamt = atof(STR(mod->grid[n].tamt)); // 평가손익
			double srat = atof(STR(mod->grid[n].srat));	// 수익률
			
			CString line;
			line.Format("종목(%s) 매매(%s) 잔고(%d) 청산(%d) 평균가(%.2f) 손익(%.f) 수익률(%.2f)",
				code, dsgb, jqty, xqty, pamt, tamt, srat);
			m_lbxJngo.AddString(line);
			
			if (bNext)
			{
				if (!m_axIBKS.TR3221(TK_TR3221, m_jngoAccn, m_jngoPswd, nkey))
				{
					MessageBox(m_axIBKS.GetLastErrMsg(), "잔고연속조회오류", MB_OK|MB_ICONERROR);
				}
			}
		}
		
	}
	else if (key==TK_TR1201)
	{
		struct tr1201_mod *mod = (struct tr1201_mod*)dptr;
		emsg.Format("주문번호[%.6s] 원주문번호[%.6s] 결과[%.80s]", mod->jmno, mod->ojno, mod->emsg);
		m_edResult.SetWindowText(emsg);
	}
	else if (key==TK_TR1001)
	{
		struct tr1001_mod *mod = (struct tr1001_mod*)dptr;
		
		GetDlgItem(IDC_CURR)->SetWindowText( STR(mod->curr) );
		GetDlgItem(IDC_DIFF)->SetWindowText( STR(mod->diff) );
		GetDlgItem(IDC_RATE)->SetWindowText( STR(mod->rate) );
		GetDlgItem(IDC_GVOL)->SetWindowText( STR(mod->gvol) );
// 		GetDlgItem(IDC_MGJV)->SetWindowText( STR(mod->mgjv) );
		GetDlgItem(IDC_MDVOL)->SetWindowText( STR(mod->dvol) );
		GetDlgItem(IDC_MSVOL)->SetWindowText( STR(mod->svol) );
		GetDlgItem(IDC_SIGA)->SetWindowText( STR(mod->siga) );
		GetDlgItem(IDC_KOGA)->SetWindowText( STR(mod->koga) );
		GetDlgItem(IDC_JEGA)->SetWindowText( STR(mod->jega) );

		CString strDiff;
		strDiff = STR(mod->diff);
		OutputDebugString("[KSJ] 1001 diff[" + strDiff + "]");
		emsg.Format("1001 [%s]", mod->cod2);
		m_edResult.SetWindowText(emsg);

	}
	else if (key==TK_TR1221)
	{
		struct tr1221_mod *mod = (struct tr1221_mod*)dptr;
		
		int nrec = _ttoi( STR(mod->nrec) );
		for(int n=0; n<nrec; ++n)
		{
			CString code = STR(mod->grid[n].cod2);	code.TrimRight();
// 			CString dsgb = STR(mod->grid[n].dsgb);	dsgb.TrimRight();
			int jqty = _ttoi(STR(mod->grid[n].jqty));	// 보유수량
			int xqty = _ttoi(STR(mod->grid[n].xqty));	// 청산가능수량
			double pamt = atof(STR(mod->grid[n].pamt));	// 평균가
			double tamt = atof(STR(mod->grid[n].tamt)); // 평가손익
			double srat = atof(STR(mod->grid[n].srat));	// 수익률
			
			CString line;
			line.Format("종목(%s) 잔고(%d) 청산(%d) 평균가(%.2f) 손익(%.f) 수익률(%.2f)",
				code, jqty, xqty, pamt, tamt, srat);
			m_lbxJngo.AddString(line);
			
			if (bNext)
			{
				if (!m_axIBKS.TR1221(TK_TR1221, m_jngoAccn, m_jngoPswd, 0, nkey))
				{
					MessageBox(m_axIBKS.GetLastErrMsg(), "잔고연속조회오류", MB_OK|MB_ICONERROR);
				}
			}
		}
	}
	else if (key == TK_TR1802)
	{
		struct tr1802_mod *mod = (struct tr1802_mod*)dptr;
		emsg.Format("선옵[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1801)
	{
		struct tr1801_mod *mod = (struct tr1801_mod*)dptr;
		emsg.Format("주식[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1803)
	{
		struct tr1803_mod *mod = (struct tr1803_mod*)dptr;
		emsg.Format("업종별 등락[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1804)
	{
		struct tr1804_mod *mod = (struct tr1804_mod*)dptr;
		emsg.Format("지수조회[%s]", mod->nrec);
		m_edResult.SetWindowText(emsg);
	}
	else if(key == TK_GETCODE)
	{
		struct hjcode *mod = (struct hjcode*)dptr;
		emsg.Format("마스터[%s]", mod->codx);
		m_edRdResult.SetWindowText(emsg);
	}
	else if (key == TK_TR1211)
	{
		struct tr1211_mod *mod = (struct tr1211_mod*)dptr;
		emsg.Format("체결내역조회[%s]", mod->acno);
		m_edResult.SetWindowText(emsg);
	}
	else if (key == TK_GREEKS2) //선물 현재가(기초자산을 선물최근월물로 했을경우)
	{
		m_bGreeks2 = TRUE;

		CString strResult, strTemp;
	
		struct tr3001_mod *mod = (struct tr3001_mod*)dptr;
		
		m_strGreeks2 = STR(mod->curr);

		if(m_bGreeks1 && m_bGreeks2)
		{
			//종목코드\t잔존일수\t행사가\tcd금리\t현재가\t기초자산
			strTemp = m_strGreeksCode + "\t" + m_strGreeks1 + "\t" + m_strGreeks2;
			strResult = m_axIBKS.GetGreeksData(strTemp);
			m_edResult.SetWindowText(strResult);

			m_bGreeks1 = m_bGreeks2 = FALSE;
		}
	}
	else if (key == TK_GREEKS1) // 옵션 잔존일수, 행사가, cd금리, 현재가
	{
		m_bGreeks1 = TRUE;
		
		CString strResult, strTemp, strDays;
		strTemp = (char*)dptr;
	
		strDays = strTemp.Left(strTemp.Find("\t") + 1); strDays.TrimRight();

		m_strGreeks1.Format("%f", atof(strDays)/365.0);	//달력일수기준 잔존일수
		//m_strGreeks1.Format("%f", atof(strDays)/250.0);	//영업일수기준 잔존일수

		m_strGreeks1 += strTemp.Right(strTemp.GetLength() - strDays.GetLength());

		if(m_bGreeks1 && m_bGreeks2)
		{
			//종목코드\t잔존일수\t행사가\tcd금리\t현재가\t기초자산
			strTemp = m_strGreeksCode + "\t" + m_strGreeks1 + "\t" + m_strGreeks2;
			strResult = m_axIBKS.GetGreeksData(strTemp);
			m_edResult.SetWindowText(strResult);

			m_bGreeks1 = m_bGreeks2 = FALSE;
		}
	}
	else if (key == TK_TR3232)	//선옵 당일손익조회
	{
		struct tr3232_mod *mod = (struct tr3232_mod*)dptr;
		emsg.Format("선옵 당일손익조회[%s]", mod->zOut);
		m_edResult.SetWindowText(emsg);

	}
	else if (key == TK_TR3411)	//선옵 월물별 시세조회
	{
		struct tr3411_mod *mod = (struct tr3411_mod*)dptr;
		emsg.Format("선옵 월물별 시세조회[%s]", mod->atmg);
		m_edResult.SetWindowText(emsg);	
	}
}

void CIBKS_SampleDlg::OnEvtError(LPCTSTR msg) 
{
	MessageBox(msg, "오류발생", MB_OK|MB_ICONERROR);
}

void CIBKS_SampleDlg::OnEvtGuideMsg(long key, LPCTSTR sMsg) 
{
	CString strTemp;
	strTemp.Format("[%d]%s", key, sMsg);
	MessageBox(strTemp, "코스콤", MB_OK);
}

void CIBKS_SampleDlg::OnEvtClose() 
{
	MessageBox("접속이 종료되었습니다.", "접속", MB_OK|MB_ICONINFORMATION);
}

void CIBKS_SampleDlg::OnEvtAlert(long code, LPCTSTR msg) 
{
	CString strMsg;
	strMsg.Format("[%04d] %s", code, msg);
	MessageBox(strMsg, "긴급전달메세지", MB_OK|MB_ICONINFORMATION);
}

/*
  선옵 실시간 잔고(tab구분자로 문자열 전달, "\n"으로 종목구분)
  레이아웃(14) : Flag종목코드\t종목명\t구분\t잔고수량\t청산가능수량\t평균가\t현재가\t
  평균가대비\t평가금액\t평가손익\t수익률\t매입금액\t전일대비\n

  Flag : "A"(전체 "A"\t총갯수\t), "I"(삽입 "I"\t종목코드), 
       "U"(갱신 "U"\t종목코드), "D"(삭제 "D"\t종목코드), "E"(에러 "E"\t에러메세지)
*/
void CIBKS_SampleDlg::OnEvtFBalance(LPCTSTR data) 
{
	CString strData, strLine, strField, flag, strFormat;
	CStringArray arrLine;

	CStringArray arrField;
	arrField.Add("종목코드"); arrField.Add("종목명"); arrField.Add("구분"); arrField.Add("잔고수량"); arrField.Add("청산가능수량");
	arrField.Add("평균가"); arrField.Add("현재가"); arrField.Add("평균가대비"); arrField.Add("평가금액"); arrField.Add("평가손익");
	arrField.Add("수익률"); arrField.Add("매입금액"); arrField.Add("전일대비");

	strData = data;
	
	int i = 0;

	CString strTemp, strOrgData = data;
	strOrgData.Replace("\t", "??");
	strTemp.Format("[KSJ] Data[%s]", strOrgData);
	OutputDebugString(strTemp);

	flag = strData.Left(strData.Find("\t") + 1); flag.TrimRight();
	strData = strData.Right(strData.GetLength() - flag.GetLength() -1);
	

	if(flag == "A")	// 전체일때는 갯수가 하나 더 붙는다.
		strData = strData.Right(strData.GetLength() - strData.Find("\t") -1);
	else if(flag =="U" || flag == "I")	//삽입이나 갱신일때는 종목코드가 연속으로 온다.
		strData = strData.Right(strData.GetLength() - 8);

	//종목별
	while(AfxExtractSubString(strLine, strData, i++, '\n'))
	{
		arrLine.RemoveAll();
		int n = 0;

		if(strLine.Find("\t") == -1)
			continue;

		//Field를 Array에 저장
		while(AfxExtractSubString(strField, strLine, n++, '\t'))
		{
			arrLine.Add(strField);
		}

		CString line;

		int nSize = min(arrField.GetSize(), arrLine.GetSize());

		//리스트에 들어갈 문자열 가공
		for(int ii = 0; ii < nSize; ii++)
		{
			line += arrField.GetAt(ii) + "(" + arrLine.GetAt(ii) + ")\t";
		}

		//Flag에 따라 구분하여 리스트에서 관리
		if(flag == "A" || flag =="I")
		{
			m_lbxJngo.AddString(line);
		}
		else if(flag == "U")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");
			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
				m_lbxJngo.InsertString(nIndex, line);
			}
		}
		else if(flag == "D")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");
			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
			}
		}
		else
		{

		}
	}

	CreateHorizontalScroll();
}

/*
		주식 실시간 잔고(tab구분자로 문자열 전달, "\n"으로 종목구분)
		레이아웃(22) : Flag종목코드\t종목명\t잔고구분\t결제잔고\t주문잔량\t매입평균가\t
		매도가능잔고\t현재가\t평가금액\t평가손익\t평가수익률\t매입금액\t
		대출일\t신용구분\t신용구분내용\t신용구분약자\t신용대출금액\t
		잔고구분\tBEP단가\t비용\t등락률\n

		Flag : "A"(전체 "A"\t총갯수\t), 
		"I"(삽입 "I"\t잔고구분(2)+종목코드(11)+대출일(8)+신용구분(2)), 
		"U"(갱신 "U"\t잔고구분(2)+종목코드(11)+대출일(8)+신용구분(2)),
		"D"(삭제 "D"\t잔고구분(2)+종목코드(11)+대출일(8)+신용구분(2)), 
		"E"(에러 "E"\t에러메세지)*/
void CIBKS_SampleDlg::OnEvtSBalance(LPCTSTR data) 
{
	CString strData, strLine, strField, flag, strFormat;
	CStringArray arrLine;
	
	CStringArray arrField;
	arrField.Add("종목코드"); arrField.Add("종목명"); arrField.Add("잔고구분"); arrField.Add("결제잔고"); arrField.Add("주문잔량"); arrField.Add("매입평균가");
	arrField.Add("매도가능잔고"); arrField.Add("현재가"); arrField.Add("평가금액"); arrField.Add("평가손익"); arrField.Add("평가수익률");
	arrField.Add("매입금액"); arrField.Add("대출일"); arrField.Add("신용구분"); arrField.Add("신용구분내용"); arrField.Add("신용구분약자");
	arrField.Add("신용대출금액"); arrField.Add("잔고구분"); arrField.Add("BEP단가"); arrField.Add("비용"); arrField.Add("등락률");
	
	strData = data;
	
	CString strTemp, strOrgData = data;
	strOrgData.Replace("\t", "??");
	strTemp.Format("[KSJ] Data[%s]", strOrgData);
	OutputDebugString(strTemp);

	int i = 0;
	
	flag = strData.Left(strData.Find("\t") + 1); flag.TrimRight();
	strData = strData.Right(strData.GetLength() - flag.GetLength() -1);
	
	strTemp.Format("[KSJ] flag[%s] strData[%s]", flag, strData);
	OutputDebugString(strTemp);

	//종목별	
	if(flag == "A")	// 전체일때는 갯수가 하나 더 붙는다.
		strData = strData.Right(strData.GetLength() - strData.Find("\t") -1);
	else if(flag =="U" || flag == "I" || flag == "D")	//삽입이나 갱신일때는 종목코드가 연속으로 온다.
		strData = strData.Right(strData.GetLength() - 24);	//잔고구분(2)+종목코드(11)+대출일(8)+신용구분(2)(23자리)
	
	strTemp.Format("[KSJ] flag[%s] strData[%s]", flag, strData);
	OutputDebugString(strTemp);

	while(AfxExtractSubString(strLine, strData, i++, '\n'))
	{
		arrLine.RemoveAll();
		int n = 0;
		
		if(strLine.Find("\t") == -1)
			continue;
		
		//Field를 Array에 저장
		while(AfxExtractSubString(strField, strLine, n++, '\t'))
		{
			arrLine.Add(strField);
		}
		
		CString line;
		
		int nSize = min(arrField.GetSize(), arrLine.GetSize());
		
		//리스트에 들어갈 문자열 가공
		for(int ii = 0; ii < nSize; ii++)
		{
			line += arrField.GetAt(ii) + "(" + arrLine.GetAt(ii) + ")\t";
		}
		
		//Flag에 따라 구분하여 리스트에서 관리
		if(flag == "A" || flag =="I")
		{
			m_lbxJngo.AddString(line);
		}
		else if(flag == "U")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");

			strTemp.Format("[KSJ] %s(%s) ", arrField.GetAt(0), arrLine.GetAt(0));
			OutputDebugString(strTemp);

			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
				m_lbxJngo.InsertString(nIndex, line);
			}
		}
		else if(flag == "D")
		{
			int nIndex = m_lbxJngo.FindString(-1, arrField.GetAt(0) + "(" + arrLine.GetAt(0) + ")");
			if(nIndex != LB_ERR)
			{
				m_lbxJngo.DeleteString(nIndex);
			}
		}
		else
		{
			
		}
	}

	CreateHorizontalScroll();
}

BOOL CIBKS_SampleDlg::PreTranslateMessage(MSG* pMsg) 
{
	// 엔터로 인한 종료 금지
	if (pMsg->message==WM_RBUTTONUP)
	{
		MessageBox("XXX");
		if (pMsg->wParam==VK_RETURN || pMsg->wParam==VK_ESCAPE) 
			return TRUE;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CIBKS_SampleDlg::ParseString( LPCSTR st, LPCSTR ed, CHAR del, vector<StringProxy> &retval )
{
	retval.clear();
	LPCSTR pos;
	while(st<ed)
	{
		pos = find(st, ed, del);
		retval.push_back( StringProxy(st, pos) );
		st = pos + 1;
	}
}

void CIBKS_SampleDlg::ParseString( StringProxy &sp, CHAR del, vector<StringProxy> &retval )
{
	ParseString(sp.first, sp.second, del, retval);
}

void CIBKS_SampleDlg::OnOdrSend() 
{
	UpdateData(TRUE);

	// 주문시 주문 가격은 실제가격X100 한 값을 정수로 넘기세요
	// ex) 288.05 -> 28805
	DWORD mmgb = m_cbMmgb.GetItemData( m_cbMmgb.GetCurSel() );
	DWORD hogb = m_cbHogb.GetItemData( m_cbHogb.GetCurSel() );
	m_odrAccn = m_odrAccn.Left(11);

	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR3201(TK_TR3201, mmgb, m_odrAccn, m_odrPswd, _ttoi(m_odrOgno), m_odrCode, _ttoi(m_odrJqty), _ttoi(m_odrJprc), hogb, 0))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "주문오류", MB_OK|MB_ICONERROR);
		}
	}
	else
	{
		if (!m_axIBKS.TR1201(TK_TR1201, mmgb, m_odrAccn, m_odrPswd, _ttoi(m_odrOgno), m_odrCode, _ttoi(m_odrJqty), _ttoi(m_odrJprc), hogb, 0))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "주문오류", MB_OK|MB_ICONERROR);
		}
	}
}

void CIBKS_SampleDlg::OnSiseSend() 
{
	UpdateData(TRUE);

	m_cbCheg.ResetContent();
	GetDlgItem(IDC_CURR)->SetWindowText( "" );
	GetDlgItem(IDC_DIFF)->SetWindowText( "" );
	GetDlgItem(IDC_RATE)->SetWindowText( "" );
	GetDlgItem(IDC_GVOL)->SetWindowText( "" );
	GetDlgItem(IDC_MGJV)->SetWindowText( "" );

	if(m_cbJType.GetCurSel() == 0)
	{
		if (m_siseCode.GetLength()!=8)
		{
			MessageBox("올바른 종목코드를 입력하세요", "종목오류", MB_OK|MB_ICONERROR);
			return;
		}
		
		if (!m_axIBKS.TR3001(TK_TR3001, m_siseCode))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "시세조회오류", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (m_siseCode.GetLength()!=6)
		{
			MessageBox("올바른 종목코드를 입력하세요", "종목오류", MB_OK|MB_ICONERROR);
			return;
		}
		
		if (!m_axIBKS.TR1001(TK_TR1001, m_siseCode))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "시세조회오류", MB_OK|MB_ICONERROR);
			return;
		}
	}
}

LPCSTR CIBKS_SampleDlg::GetString( LPCSTR dat, int len )
{
	// No thread safe!!
	static char buff[1024];
	sprintf(buff, "%.*s", len, dat);
	return buff;
}

void CIBKS_SampleDlg::OnJngoSend() 
{
	UpdateData(TRUE);

	m_lbxJngo.ResetContent();
	m_jngoAccn = m_jngoAccn.Left(11);

	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR3221(TK_TR3221, m_jngoAccn, m_jngoPswd, ""))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "잔고조회오류", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (!m_axIBKS.TR1221(TK_TR1221, m_jngoAccn, m_jngoPswd, 0, ""))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "잔고조회오류", MB_OK|MB_ICONERROR);
			return;
		}
	}
}

void CIBKS_SampleDlg::OnBtnDisconnect() 
{
	// TODO: Add your control notification handler code here
	m_axIBKS.Logout();
}

void CIBKS_SampleDlg::OnEvtVerUpdate()
{
	MessageBox("플랫폼 업데이트 완료\n파일복사 후 재기동 하시기 바랍니다.", "플랫폼 업데이트", MB_OK);
}

void CIBKS_SampleDlg::OnEvtUpdateStart()
{
	MessageBox("파일 업데이트", "파일 업데이트 시작", MB_OK);
}

void CIBKS_SampleDlg::OnEvtUpdating(LPCSTR sData, long nProgress)
{
	CString emsg;
	emsg.Format("sData[%s] nProgress[%d]", sData, nProgress);
	m_edResult.SetWindowText(emsg);
}

void CIBKS_SampleDlg::OnEvtUpdateEnd()
{
	MessageBox("파일 업데이트", "파일 업데이트 끝", MB_OK);
}

void CIBKS_SampleDlg::OnJngoSend2() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_lbxJngo.ResetContent();
	m_jngoAccn = m_jngoAccn.Left(11);
	
	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR3222(TK_TR3222, m_jngoAccn, m_jngoPswd))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "실시간 잔고조회오류", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (!m_axIBKS.TR1222(TK_TR1222, m_jngoAccn, m_jngoPswd, 2))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "실시간 잔고조회오류", MB_OK|MB_ICONERROR);
			return;
		}
	}	
}

//가로스크롤 생성
void CIBKS_SampleDlg::CreateHorizontalScroll()
{
    CString str; CSize sz; int dx=0;
    CDC *pDC = m_lbxJngo.GetDC();
	
	for(int i = 0; i < m_lbxJngo.GetCount(); i++)
    {
        m_lbxJngo.GetText(i, str);
        sz = pDC->GetTextExtent(str);
		
        if (sz.cx > dx)
			dx = sz.cx;
    }
    m_lbxJngo.ReleaseDC(pDC);
	
    if (m_lbxJngo.GetHorizontalExtent() < dx)
    {
        m_lbxJngo.SetHorizontalExtent(dx);
        ASSERT(m_lbxJngo.GetHorizontalExtent() == dx);
    }
}

void CIBKS_SampleDlg::OnSiseSend2() 
{
	// TODO: Add your control notification handler code here
	
	if(m_cbJType.GetCurSel() == 0)
	{
		if (!m_axIBKS.TR1802(TK_TR1802, "101GC000101H3000101H6000101H9000"))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "선옵 복수종목 시세", MB_OK|MB_ICONERROR);
			return;
		}
	}
	else
	{
		if (!m_axIBKS.TR1801(TK_TR1801, "006910078000000660005930"))
		{
			MessageBox(m_axIBKS.GetLastErrMsg(), "주식 복수종목 시세", MB_OK|MB_ICONERROR);
			return;
		}
	}	

}

void CIBKS_SampleDlg::OnSiseSend3() 
{
	// TODO: Add your control notification handler code here
	if (!m_axIBKS.TR1803(TK_TR1803, "0001"))
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "업종별등락현황", MB_OK|MB_ICONERROR);
		return;
	}

}

void CIBKS_SampleDlg::OnSiseSend4() 
{
	// TODO: Add your control notification handler code here
	if (!m_axIBKS.TR1804(TK_TR1804, "0001"))
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "지수조회", MB_OK|MB_ICONERROR);
		return;
	}
}

void CIBKS_SampleDlg::OnSiseSend5() 
{
	UpdateData(TRUE);

	m_jngoAccn = m_jngoAccn.Left(11);

	// TODO: Add your control notification handler code here
	if (!m_axIBKS.TR1211(TK_TR1211, m_jngoAccn, m_jngoPswd, "A000660", 0, 0, 0, 1, ""))
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "체결내역조회", MB_OK|MB_ICONERROR);
		return;
	}
}

//종목코드\t잔존일수\t행사가\tcd금리\t현재가\t기초자산
void CIBKS_SampleDlg::OnGreeks() 
{
	// TODO: Add your control notification handler code here

	CString strData, strResult;
	m_axIBKS.TR3001(TK_GREEKS2, "101H6000" );//선물 현재가(기초자산을 선물최근월물로 했을경우)

	m_strGreeksCode = "201H4260";
	m_axIBKS.TR3002(TK_GREEKS1, m_strGreeksCode, "40314\t40310\t40332\t41023\t" );//잔존일수, 행사가, cd금리, 현재가
}

void CIBKS_SampleDlg::OnSonIk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	
	m_jngoAccn = m_jngoAccn.Left(11);

	if (!m_axIBKS.TR3232(TK_TR3232, m_jngoAccn, m_jngoPswd, 0, 1))//선옵대용포함, 수익률(평균예탁자산기준)
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "선옵 당일손익 조회", MB_OK|MB_ICONERROR);
		return;
	}
}

void CIBKS_SampleDlg::OnMonSise() 
{
	// TODO: Add your control notification handler code here
	
	if (!m_axIBKS.TR3411(TK_TR3411, "201304"))//YYYYMM
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "선옵 월물별 시세 조회", MB_OK|MB_ICONERROR);
		return;
	}
}


void CIBKS_SampleDlg::OnBnClickedBtnOop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (!m_axIBKS.TR1002(49, "005930", "1023\t1024\t1033"))//YYYYMM
	{
		AfxMessageBox("poop 조회 에러");
	}
}


void CIBKS_SampleDlg::OnBnClickedTest()
{ 
	//TR1231(long key, LPCTSTR acno, LPCTSTR pswd, LPCTSTR code, long mmgb, double jprc)

	CString sAcc, sPswd;
	m_cbAccn2.GetWindowText(sAcc);
	sAcc = sAcc.Left(11);
	((CWnd*)GetDlgItem(IDC_JNGO_ACPW))->GetWindowText(sPswd);

	if (!m_axIBKS.TR1231(TK_TR1231, sAcc, sPswd, "A005930", 1,60500 ))//YYYYMM
	{
		MessageBox(m_axIBKS.GetLastErrMsg(), "TK_TR1231");
	}

	/*
	if (!m_axIBKS.TR3201(TK_TR3201, mmgb, m_odrAccn, m_odrPswd, _ttoi(m_odrOgno), m_odrCode, _ttoi(m_odrJqty), _ttoi(m_odrJprc), hogb, 0))
		{
	*/
}
