// MapWnd.cpp : implementation file

#include "stdafx.h"
#include "IBXXXX01.h"
#include "MapWnd.h"
#include "DefineAll.h"

#include "../../h/fxCommonx.h"
#include "../../h/jmcode.h"
#include "../../h/axisvar.h"

#define	UPMAXSIZE	1024*15

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#define	_FILE_DEBUG
#ifdef	_FILE_DEBUG
#include <fstream.h>
#endif

CMapWnd::CMapWnd()
{
	m_sCurrKey = "";
	m_system = 'D';
	m_arKey.RemoveAll();
	m_hcnt = 0;

	m_hcB = nullptr;
	m_hcode.RemoveAll();
	m_hname.RemoveAll();

	m_hiSha256 = NULL;
}

CMapWnd::~CMapWnd()
{
//	if (m_hcB) delete[] m_hcB;

	m_hcode.RemoveAll();
	m_hname.RemoveAll();
}

BEGIN_MESSAGE_MAP(CMapWnd, CWnd)
	//{{AFX_MSG_MAP(CMapWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
	ON_MESSAGE(WM_REMAIN, OnRequestOfCtrl)
	ON_MESSAGE(WM_LAWCHEG, OnRequestLawCheg)
	ON_MESSAGE(WM_REMAIN_CLOSE, OnCloseOfCtrl)
	ON_MESSAGE(WM_ALLACCNREMAIN, OnReceiveRemainData)
END_MESSAGE_MAP()

#ifdef	_FILE_DEBUG
	ofstream dFile("C:\\IBXXXX01.trc",ios::app);
#endif

long CMapWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			CString sErrCode = "", sErrText = "", sLedger, sData;
			long	ledgerLength = 0;
			char	nextKey{};

			struct _extTHx* extTHx = (struct _extTHx*)lParam;

			char* pBytes = extTHx->data;
			if (extTHx->size == 0)
			{
				return 0;
			}

			struct _ledgerH ledger;
			CopyMemory(&ledger, (void*)pBytes, L_ledgerH);
			sLedger = CString((char*)&ledger, L_ledgerH);
			sErrCode = CString((char*)&ledger.emsg, 4);
			sErrText = CString((char*)&ledger.emsg, 98);
			nextKey = ledger.next[0];
			ledgerLength = L_ledgerH;
			
			pBytes += ledgerLength;
			
			if (extTHx->key == KEY_NCONT)	// 체결내역
			{
				if (sErrCode.GetAt(0) != _T('0'))
					return 0;
				parsingNContData(pBytes);
				if (nextKey == 'Y')
					sendNContTR(sLedger);
			}else// if (key == KEY_HREMAIN || key == KEY_FREMAIN)	// 잔고
			{
				//if (sErrCode == "    ") sErrText = "코스콤 TR 오류 입니다. 잠시후 다시 시도 하시기바랍니다.";
				sErrCode.TrimLeft(); sErrCode.TrimRight();
				//TRACE("sErrCode: ["+sErrCode+"]\n");
				if (sErrCode == "")
				{
					sErrCode = "9506";
					sData = "ERR\t";
				}
				else sData.Format("ERR\t%s", sErrText);
				
				//TRACE("["+sData+"]\n");
				
				if (sErrCode.GetAt(0) != _T('0'))	// 오류
				{
					//if (sErrCode != "1511")
					{
						CString trkey, keyHandle; trkey.Format("%d",extTHx->key);
						removeAccn(trkey);	// 20070824 memory leak 해결
						keyHandle = GetHandleByKey(trkey);
						
						detachCB(trkey, sData);
						//TRACE("trkey: "+trkey+"\n");						
						m_AccnKey.RemoveKey(keyHandle);
						//CString tmp; tmp.Format("m_AccnKey: %d\n", m_AccnKey.GetCount());
						//TRACE(tmp);
						
					}
					return 0;
					
				}
			
				CString sAccn;
				if(1)
					sAccn = parsingRemainData(pBytes, extTHx->size - L_ledgerH, sErrCode, extTHx->key);  //dll_oubx
				else
					sAccn = parsingRemainData(pBytes, extTHx->size - L_ledgerH, sData, extTHx->key);  //dll_oubx

				sAccn.TrimLeft(); sAccn.TrimRight();
				if (sAccn!="")
				{
					CString trkey; trkey.Format("%d",extTHx->key);
					sendToControl(trkey);
				}
			}
		}
		break;
	case DLL_OUB:
		{
			if (!HIWORD(wParam))
				break;

			CAccn *pAccn = NULL;
			const int key = HIBYTE(LOWORD(wParam));
			// 20070824 신규종목 체결시 선물옵션조회TR 없어짐
			if (key == KEY_SCURR)	// 현물 현재가
			{
				CString sData = CString((char*)lParam, HIWORD(wParam));
				if (!m_AccnMap.Lookup(m_sCurrAccn, (void*&)pAccn))
					return -1;
					
				CRemain *pRemain = NULL;
				if (!pAccn->m_CodeMap.Lookup(m_sCurrKey, (void*&)pRemain))
					return -1;
				if (!pRemain)	return -1;
				
				pRemain->m_curr = parser(sData, "\t");
				pRemain->CalPgsonik("", "");
#ifdef	_FILE_DEBUG
//	dFile	<< "[notice insert]" << m_sCurrAccn  << "\tU\t" << pRemain->TotalData()<< endl;
	//TRACE("[notice insert]%s\n", CString(m_sCurrAccn + "\tU\t"+ pRemain->TotalData()));
#endif
				m_ShMemory->SendTrigger(CString(m_sCurrAccn + "\tI\t" + pRemain->TotalData()));

				return 0;
			}

			// 잔고 or 체결내역(선물)
			CString sErrCode = "", sErrText = "", sLedger, sData;
			long	ledgerLength = 0;
			char	nextKey;

/* @@Zeta	Ledger 변경
			if (m_system == 'D')
			{
				struct _ledgerDaetooH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerDaetooH);
				sLedger = CString((char*)&ledger, L_ledgerDaetooH);
				sErrText = CString((char*)&ledger.msg, 130);
				sErrCode = CString((char*)&ledger.ecode, 6);
				nextKey = ledger.contf[0];
				ledgerLength = L_ledgerDaetooH;
			}
			else
			{
				struct _ledgerHanaH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerHanaH);
				sLedger = CString((char*)&ledger, L_ledgerHanaH);
				sErrText = CString((char*)&ledger.msg, 130);
				sErrCode = CString((char*)&ledger.ecode, 6);
				nextKey = ledger.contf[0]; 
				ledgerLength = L_ledgerHanaH;
			}
*/
			//CString pbdata; pbdata = (char*)lParam;
			//TRACE("PB DATA["+pbdata+"]\n");
			//if (HIWORD(wParam)<340) return 0;
			struct _ledgerH ledger;
			CopyMemory(&ledger, (void*)lParam, L_ledgerH);
			sLedger = CString((char*)&ledger, L_ledgerH);
			sErrCode = CString((char*)&ledger.emsg, 4);
			sErrText = CString((char*)&ledger.emsg, 98);
			nextKey = ledger.next[0];
			ledgerLength = L_ledgerH;

			lParam += ledgerLength;
	
			
			if (key == KEY_NCONT)	// 체결내역
			{
				if (sErrCode.GetAt(0) != _T('0'))
					return 0;
				parsingNContData((char*)lParam);
				if (nextKey == 'Y')
					sendNContTR(sLedger);
			}else// if (key == KEY_HREMAIN || key == KEY_FREMAIN)	// 잔고
			{
				//if (sErrCode == "    ") sErrText = "코스콤 TR 오류 입니다. 잠시후 다시 시도 하시기바랍니다.";
				sErrCode.TrimLeft(); sErrCode.TrimRight();
				//TRACE("sErrCode: ["+sErrCode+"]\n");
				if (sErrCode == "")
				{
					sErrCode = "9506";
					sData = "ERR\t";
				}
				else sData.Format("ERR\t%s", sErrText);
				
				//TRACE("["+sData+"]\n");
				
				if (sErrCode.GetAt(0) != _T('0'))	// 오류
				{
					//if (sErrCode != "1511")
					{
						CString trkey, keyHandle; trkey.Format("%d",key);
						removeAccn(trkey);	// 20070824 memory leak 해결
						keyHandle = GetHandleByKey(trkey);
						
						detachCB(trkey, sData);
						//TRACE("trkey: "+trkey+"\n");						
						m_AccnKey.RemoveKey(keyHandle);
						//CString tmp; tmp.Format("m_AccnKey: %d\n", m_AccnKey.GetCount());
						//TRACE(tmp);
						
					}
					return 0;
					
				}
				//CString atmp; atmp.Format("Received key[%d]\n",key);
				//TRACE(atmp);				
				CString sAccn = parsingRemainData((char*)lParam, HIWORD(wParam), sData, key);    //dll_oub
				sAccn.TrimLeft(); sAccn.TrimRight();
				if (sAccn!="")
				{
					CString trkey; trkey.Format("%d",key);
					sendToControl(trkey);
				}
			}
		}
		break;
	//case DLL_ALERT:
	//	m_sync.Lock();
	//	parsingRTS((CString)(char*)lParam);
	//	m_sync.Unlock();
	//	break;
	case DLL_ALERTx:
		m_sync.Lock();
		parsingRTSx((struct _alertR *)lParam);
		m_sync.Unlock();
		break;
	case DLL_SETPAL:
		break;
	case DLL_TRIGGER:
		break;
	case DLL_DOMINO:
		break;
	case DLL_NOTICE:
		{
//			TRACE("START - %x %x\n", m_hWnd, ::GetParent(m_hWnd));
			m_sync.Lock();
			parsingNotice((char*)lParam);
			m_sync.Unlock();
			SendLawNotice((char*)lParam);		
//			TRACE("END - %x %x\n", m_hWnd, ::GetParent(m_hWnd));
		}
		break;
	case DLL_GUIDE:
		return true;
		break;
	}

	return 0;
}

// 잔고조회
LRESULT CMapWnd::OnRequestOfCtrl(WPARAM wParam, LPARAM lParam)
{
	CString sTemp = (char*)lParam,
		sPswd, sAccn, sCredit;
	bool bFuture = (parser(sTemp, "\t")=="1" ? true : false);
	sAccn = parser(sTemp, "\t");
	sPswd = parser(sTemp, "\t");

	CString creS = parser(sTemp, "\t");
	const bool bCredit = (atoi(creS) == 1 ? true : false);

	//buffet add
	CString sFee = parser(sTemp, "\t");
	const int dFee = atoi(sFee);

	CString sMass = parser(sTemp, "\t");
	const double dMass = atof(sMass);

	CString sSave = parser(sTemp, "\t");
	const double dSave = atof(sSave);

	CString sCalcType = parser(sTemp, "\t");
	const int dCalcType = atoi(sCalcType);

	//2015.04.22 KSJ 매체구분 추가
	CString sMcgb = parser(sTemp, "\t");
	const int iMcgb = atoi(sMcgb);

	//nxt bal
	CString sFiller = parser(sTemp, "\t");

	//end buffet

	m_slog.Format("[JANGO][%s]<%d> sFee=[%s] sMass=[%s] sSave=[%s]",__FUNCTION__, __LINE__, sFee, sMass, sSave);
	Output_DebugString(m_slog);

	sendRemainTR(sAccn, sPswd, bFuture, bCredit, dFee, dMass, dSave, dCalcType, iMcgb, (HWND)wParam, sFiller);

	return 0;
}

// 법인영업팀 체결조회
LRESULT CMapWnd::OnRequestLawCheg(WPARAM wParam, LPARAM lParam)
{
	CString sTemp = (char*)lParam,
		sPswd, sAccn, sCredit;
	//AfxMessageBox("TEXT: "+sTemp);
	sendLawCheg("","","","", (HWND)wParam);


	return 0;

	//sendRemainTR(sAccn, sPswd, bFuture, bCredit, (HWND)wParam);
}

// 그룹계좌인경우 사용하는데 프로그램 상 문제 있는듯 함
LRESULT CMapWnd::OnReceiveRemainData(WPARAM wParam, LPARAM lParam)
{
	CString sTemp = (char*)lParam,
		sAccn, keyS, creS, sData;
	bool bFuture = (parser(sTemp, "\t")=="1"?true:false);
	sAccn = parser(sTemp, "\t");
	keyS = parser(sTemp, "\t");
	creS = parser(sTemp, "\t");
	const bool bCredit = (atoi(creS) == 1 ? true : false);
	sData = parser(sTemp, "\t");

//TRACE("그룹 sAccn[%s] keyS[%s] sData[%s]\n", sAccn, keyS, sData);

	CAccn * pAccn = nullptr;

	if (!m_AccnMap.Lookup(sAccn, (void*&)pAccn))
	{
		auto& tAccn = _vList.emplace_back(std::make_unique<CAccn>());
		pAccn = tAccn.get();

		pAccn->m_bFuture = bFuture;
		pAccn->m_pswd = "";
		pAccn->m_bCredit = bCredit;

		m_AccnMap.SetAt(sAccn, pAccn);
	}

	if (!pAccn->m_pswd.IsEmpty())
		return 0;

	if (bFuture)	// 선물
	{
		gsl::owner <CRemainFuture*> pfRemain = nullptr;

		if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pfRemain))
		{
			[[gsl::suppress(26409)]]
			pfRemain = new CRemainFuture(this);
			pfRemain->m_system = m_system;
		}

		pfRemain->ParsingAllaccnData(sData);
		pAccn->m_CodeMap.SetAt(keyS, pfRemain);
	}
	else	// 주식
	{
		CString codeS, sygbS, dateS, jggbS, lookupS;

		jggbS = keyS.Left(LEN_JGGB);
		codeS = keyS.Mid(LEN_JGGB, LEN_ACODE);
		dateS = keyS.Mid(LEN_JGGB + LEN_ACODE, LEN_DATE);
		sygbS = keyS.Mid(LEN_JGGB + LEN_ACODE + LEN_DATE, LEN_SYGB);
		
		keyS = getKey(bFuture, codeS, dateS, sygbS, jggbS);  //OnReceiveRemainData  안쓰는?
		bool b_etf = false;
		struct	hjcodex*	hjc;

		lookupS = codeS;
		int ifind = lookupS.Find('.');
		if (ifind != -1)
			lookupS = codeS.Mid(ifind + 1);

		if (m_hname.Lookup(lookupS.Mid(1,7), (void *&)hjc))   //nxt ?  OnReceiveRemainData가 사용되지는 않는다
		{
				if (hjc->ssgb == jmETF)	// ETF
				{
					b_etf = true;
				}
		}

		gsl::owner <CRemain *> pRemain = nullptr;
		if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
		{
			pRemain = new CRemain(this);
			pRemain->m_system = m_system;
			pRemain->m_betf   = b_etf;
			pRemain->m_b3RDCode = Is3RDCode(codeS);	//2015.04.22 KSJ K-OTC인지 체크 
			pRemain->m_bKONEXCode = IsKONEXCode(codeS);	
			int itype = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)codeS.Right(6).operator LPCTSTR());
			pRemain->m_iCodetype = itype;
			m_slog.Format("[JANGO][%s]<%d> codeS=[%s] iCodetype=[%d] ", __FUNCTION__, __LINE__, codeS, itype);
			Output_DebugString(m_slog);
			if (itype == singjongGoodType)
				pRemain->m_bSJSuikCode = true;
		}
		pRemain->ParsingAllaccnData(sData);
		pAccn->m_CodeMap.SetAt(keyS, pRemain);
	}
	return 0;
}

#define DF_SHARED
//=================================================================================================
BOOL CMapWnd::CreateMap(CWnd *pParent)
{
#ifdef	_FILE_DEBUG
	dFile	<< ">>>>>>>>>>>>>>>>>실시간 잔고 시작 start>>>>>>>>>>>>>>>>" << endl << endl;
#endif
	const BOOL bCreate = Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, CRect(0, 0, MAP_WIDTH, MAP_HEIGHT), pParent, 100);
	LoadCode();
	Set3RDCode();	//2014.10.06 KSJ K-OTC 종목 세팅
	CString path = variant(homeCC, "");
	
	CString systemS = variant(systemCC, "");
	if (systemS.IsEmpty())
		m_system = 'D';
	else
	{
		m_system = systemS.GetAt(0);
	}
	variant(titleCC, "Untitled");
	path.Replace("\\", "_");

//	const DWORD processID = GetCurrentProcessId();
// 	CString sSHMENAME;
// 	sSHMENAME.Format("%s%s%d",path,"_remainCtrl_",processID);
	//m_ShMemory->remainSHMEMNAME = sSHMENAME;

#ifdef DF_SHARED
	DWORD processID = GetCurrentProcessId();
	CString sSHMENAME;
	sSHMENAME.Format("%s%s%d", path, "_remainCtrl_", processID);
	m_ShMemory = std::make_unique<CShMemory>();
	m_ShMemory->remainSHMEMNAME = sSHMENAME;
#else
	m_ShMemory = std::make_unique<CShMemory>();
	m_ShMemory->remainSHMEMNAME = path + "_remainCtrl_";
#endif

	m_ShMemory->InitSharedMemory(this->m_hWnd);
	m_ShMemory->AddHandle(this->m_hWnd);

	CString s;
	s.Format("JANGO INIT [%d] [%s]\n",m_ShMemory->GetHandleCount(),m_ShMemory->remainSHMEMNAME);
	Output_DebugString(s);
#ifdef	_FILE_DEBUG
	dFile	<< ">>>>>>>>>>>>>>>>>실시간 잔고 시작 end >>>>>>>>>>>>>>>>" << endl << endl;
#endif
	return bCreate;
}


void CMapWnd::OnPaint() 
{
	CPaintDC dc(this); 
	CRect clipRC;
	dc.GetClipBox(&clipRC);
	dc.FillSolidRect(clipRC, GetSysColor(COLOR_WINDOW));
}


void CMapWnd::OnDestroy() 
{
	m_arKey.RemoveAll();
	m_AccnKey.RemoveAll();
	m_multicb.RemoveAll();///buffet

	POSITION	accPos;		//codePos
	CString		sAccn;		// sKey

	for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
	{
		CAccn	*pAccn = NULL;
		m_AccnMap.GetNextAssoc(accPos, sAccn, (void*&)pAccn);
		removeRemain(pAccn);
		//delete pAccn;	
		pAccn = NULL;
	}
	m_AccnMap.RemoveAll();

	m_ShMemory->Close();
	m_ShMemory = nullptr;

	CWnd::OnDestroy();
}

BOOL CMapWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
	return CWnd::OnEraseBkgnd(pDC);
}

//=================================================================================================
void CMapWnd::sendRemainTR(CString sAccn, CString sPswd, bool bFuture, bool bCredit, int dFee, double dMass,double dSave, int dCalcType, int iMcgb,HWND sHwnd, CString sdata)	//2015.04.22 KSJ 매체구분 추가
{
	sPswd.TrimRight();
	sAccn.TrimRight();	sAccn.TrimLeft();//2014.10.10 KSJ 계좌번호 트림하는 거 주석처리되어 있어서 주식선물잔고가 이상하게 나왔던거 같아서 다시 주석 해제함.

	if(sAccn.IsEmpty())	return;	//2015.03.06 KSJ 계좌번호가 없으면 TR올리지 않음.
	/*
	if (sPswd.IsEmpty() && (sAccn != "           "))
	{
		AfxMessageBox("비밀번호를 입력하세요");
		return;
	}
	*/
	CAccn *pAccn = NULL;
	//std::shared_ptr<CAccn> pAccn = nullptr;
	CString sData;
	int skey=-1;
	CString AccHandleKey, sHandle; sHandle.Format("%d", (UINT)sHwnd);
	if (m_AccnKey.Lookup(sHandle,AccHandleKey))
	{
		skey = atoi(AccHandleKey);
		
		if (!m_AccnMap.Lookup(AccHandleKey, (void*&)pAccn))	// 20070824
		{
			auto& tAccn = _vList.emplace_back(std::make_unique<CAccn>());
			pAccn = tAccn.get();

			pAccn->m_accn = sAccn;
			pAccn->m_bFuture = bFuture;
			pAccn->m_pswd = sPswd;
			pAccn->m_bCredit = bCredit;
			pAccn->m_dFee = dFee;//buffet
			pAccn->m_dMass = dMass;
			pAccn->m_dSave = dSave;
			pAccn->m_dCalcType = dCalcType;
			pAccn->m_iMcgb = iMcgb;	//2015.04.22 KSJ 매체구분 추가
			if (sdata == "1")  
				pAccn->m_iMarket = MK_KRX;
			else if (sdata == "2")
				pAccn->m_iMarket = MK_NXT;
			else if (sdata == "3")
				pAccn->m_iMarket = MK_TOT;
			else
				pAccn->m_iMarket = MK_KRX;

			if(bFuture)			pAccn->m_bReceive = false;	//2015.03.04 KSJ 리시브 받지 않으면 잔고화면에 추가하지않는다.
			
			const int kNum = GetNewAccHandleKey();
			AccHandleKey.Format("%d",kNum);

			m_slog.Format("[NXT][BALANCE][%s]<%d> m_AccnKey EXIST  m_AccnMap NO  sHandle[%s] AccHandleKey[%s] acc=[%s] market=[%d]",
				__FUNCTION__, __LINE__, sHandle, AccHandleKey, sAccn, pAccn->m_iMarket);
			Output_DebugString(m_slog);

			m_AccnMap.SetAt(AccHandleKey, pAccn);
			//skey = kNum;
			//CString tmp_data;	tmp_data.Format("%d", m_hcnt);
			//m_AccnKey.SetAt(sHandle, tmp_data);
		}
		else
		{
			removeRemain(pAccn);
			pAccn->m_accn = sAccn;
			pAccn->m_pswd = sPswd;
			pAccn->m_dFee = dFee;//buffet
			pAccn->m_dMass = dMass;
			pAccn->m_dSave = dSave;
			pAccn->m_dCalcType = dCalcType;
			pAccn->m_iMcgb = iMcgb;	//2015.04.22 KSJ 매체구분 추가
			if (sdata == "1")  //NXT bal
				pAccn->m_iMarket = MK_KRX;
			else if (sdata == "2")
				pAccn->m_iMarket = MK_NXT;
			else if (sdata == "3")
				pAccn->m_iMarket = MK_TOT;
			else
				pAccn->m_iMarket = MK_KRX;

			m_slog.Format("[NXT][BALANCE][%s]<%d> m_AccnKey EXIST  m_AccnMap EXIST   sHandle[%s] AccHandleKey[%s] acc=[%s] market=[%d]",
				__FUNCTION__, __LINE__, sHandle, AccHandleKey, sAccn, pAccn->m_iMarket);
			Output_DebugString(m_slog);
		}
	}
	else
	{
		auto& tAccn = _vList.emplace_back(std::make_unique<CAccn>());
		pAccn = tAccn.get();

		pAccn->m_accn = sAccn;
		pAccn->m_bFuture = bFuture;
		pAccn->m_pswd = sPswd;
		pAccn->m_bCredit = bCredit;

		pAccn->m_dFee = dFee;//buffet
		pAccn->m_dMass = dMass;
		pAccn->m_dSave = dSave;
		pAccn->m_dCalcType = dCalcType;
		pAccn->m_iMcgb = iMcgb;	//2015.04.22 KSJ 매체구분 추가

		const int kNum = GetNewAccHandleKey();
		AccHandleKey.Format("%d",kNum);
		m_AccnMap.SetAt(AccHandleKey, pAccn);
		m_AccnKey.SetAt(sHandle, AccHandleKey);
		skey = kNum;

		if (sdata == "1")  //NXT bal
			pAccn->m_iMarket = MK_KRX;
		else if (sdata == "2")
			pAccn->m_iMarket = MK_NXT;
		else if (sdata == "3")
			pAccn->m_iMarket = MK_TOT;
		else
			pAccn->m_iMarket = MK_KRX;

		m_slog.Format("[NXT][BALANCE][%s]<%d> m_AccnKey NO    sHandle[%s] AccHandleKey[%s] acc=[%s] market=[%d]",
			__FUNCTION__, __LINE__, sHandle, AccHandleKey, sAccn, pAccn->m_iMarket);
		Output_DebugString(m_slog);

	}
	attachCB((HWND)sHwnd, AccHandleKey);

	m_sCurrAccn = sAccn;

	sData  = ledgerTR("2700", bFuture ? sv_FJGO : sv_SJGO, bFuture ? "2" : "1");

	CString passX;
	passX.Format("%s\t%s", sPswd, sAccn.Left(8));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	sPswd = passX;
	CString text, tmps;
	
	text = "";
	tmps.Format("%d",(UINT)sHwnd);
		
	if (bFuture)
	{
		struct f_mid mid;
		
		FillMemory(&mid, L_fmid, ' ');
		CopyMemory(&mid.accn, sAccn, sizeof(mid.accn));
		CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
// 		CopyMemory(&mid.pswd, "HEAD", 4);			//2014.10.20 KSJ 일방향암호화
		CopyMemory(&mid.date, "", sizeof(mid.date));
		sData += CString((char*)&mid, L_fmid);	
		sendTR("PIBOFJGO", sData, skey, 0, sAccn);

		CRemainFuture *pRemain = NULL;
		pRemain = new CRemainFuture(this);
		pRemain->m_system = m_system;

// 		CString strTemp;
// 		strTemp.Format("MAIN sendRemainTR accn[%s] AccHandleKey[%s] sHandle[%s]", sAccn, AccHandleKey, sHandle);
// 		pRemain->testSaveFile(strTemp);
	}
	else
	{
		struct s_mid mid;
		
		FillMemory(&mid, L_smid, ' ');
		CopyMemory(&mid.accn, sAccn, sizeof(mid.accn));
		CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
// 		CopyMemory(&mid.pswd, "HEAD", 4);			//2014.10.20 KSJ 일방향암호화

		if (bCredit)
			mid.allf[0] = '1';
		else
			mid.allf[0] = '0';


		if (sdata == "1")  //NXT bal
			mid.sjgb[0] = '1';
		else if (sdata == "2")
			mid.sjgb[0] = '2';
		else if (sdata == "3")
			mid.sjgb[0] = '3';
		else
			mid.sjgb[0] = '1';

		sData += CString((char*)&mid, L_smid);

		sendTR("PIBOSJG2", sData, skey, 0, sAccn);

		CRemain *pRemain = NULL;
		pRemain = new CRemain(this);
		pRemain->m_system = m_system;

#ifdef	_FILE_DEBUG
	dFile	<< "잔고조회 =>" << sAccn << endl;
#endif
	}
}

CString CMapWnd::parsingRemainData(char* pData, int len, CString sErrmsg, int trkey)
{
	CString sAccn = CString((char*)pData, 11),
		sCode, sAllCode, keyS, sygbS, dateS, jggbS, lookupS;
	int nrec = 0;
	int sizeL = 0;
	int checkL = 0;

	if (sAccn.IsEmpty())	return "";

	CAccn *pAccn = NULL;
	sAccn.Format("%d",trkey);
	
	//CString tmp; tmp.Format("trkey: %d, (%d)\n",trkey,m_AccnMap.GetCount());
	//TRACE(tmp);
	if (!m_AccnMap.Lookup(sAccn, (void*&)pAccn))	// 20070824
		return "";

	const bool bFuture = pAccn->m_bFuture;
	pAccn->m_errmsg = sErrmsg;
	
	if (bFuture)
	{
		pAccn->m_bReceive = true;	//2015.03.04 KSJ Receive받았다는 표시해줌

		struct f_mod	fmod;
		CopyMemory(&fmod, pData, L_fmod);
		nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
		pData += L_fmod;
		sizeL += L_fmod;	checkL = L_fmod + L_frec * nrec;
	}
	else
	{
		struct s_mod	smod;
		CopyMemory(&smod, pData, L_smod);
		nrec = atoi(CString(smod.nrec, sizeof(smod.nrec)));
		pData += L_smod;
		sizeL += L_smod;	checkL = L_smod + L_srec * nrec;
	}

#ifdef	_FILE_DEBUG
	dFile	<< "잔고갯수 : " << nrec << endl;
#endif
	m_arKey.RemoveAll();
	pAccn->m_CodeMap.RemoveAll();	//2014.11.03 KSJ RTS들어올때마다 초기화 시켜준다.(잘못된게 들어올수도 있으므로)

	for (int ii = 0; ii < nrec; ii++)
	{
		if (bFuture)
		{
			sCode = CString(pData, 8);
			keyS = getKey(true, sCode, "", "", "", trkey);   //parsingRemainData 선물
			CRemainFuture *pRemain = NULL;
			//2014.11.03 KSJ 주석처리함.
// 			if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
// 			{
				pRemain = new CRemainFuture(this);
				pRemain->m_system = m_system;
// 			}
			//TRACE("["+CString(pData)+"]\n");
			pRemain->ParsingRemainData(pData);
			pAccn->m_CodeMap.SetAt(keyS, pRemain);
			pData += L_frec;	sizeL += L_frec;
		}
		else
		{
			struct s_rec *srec = (struct s_rec *)pData;

			sCode = CString(srec->cod2, sizeof(srec->cod2));	// code
			sCode.TrimRight();
			dateS = CString(srec->sydt, sizeof(srec->sydt));	// 대출일
			dateS.TrimRight();
			sygbS = CString(srec->sycd, sizeof(srec->sycd));	// 신용구분
			sygbS.TrimRight();
			jggbS = CString(srec->jggb, sizeof(srec->jggb));
			jggbS.TrimRight();
			keyS = getKey(false, sCode, dateS, sygbS, jggbS, trkey);  //parsingRemainData 주식
//TRACE("key[%s][%s]\n", keyS, CString(srec->hnam, sizeof(srec->hnam)));

			bool b_etf = false;
			struct	hjcodex*	hjc;

			lookupS = sCode;
			int ifind = lookupS.Find('.');
			if (ifind != -1)
				lookupS = sCode.Mid(ifind + 1);
			
			if (m_hname.Lookup(lookupS.Mid(1,7), (void *&)hjc))   //nxt ? 잔고에 NXT종목이면 N.A005930 이라서 . 이 있으면 그다음부터 한다.  종목마스터는 N. 로 안주니까..
			{
					if (hjc->ssgb == jmETF)	// ETF
					{
						b_etf = true;
					}
			}

			CRemain *pRemain = NULL;
			if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				pRemain = new CRemain(this);
				pRemain->m_system = m_system;
				pRemain->m_betf = b_etf;
			}
			m_arKey.Add(keyS);
			pRemain->m_dFee = pAccn->m_dFee;//buffet
			pRemain->m_dMass = pAccn->m_dMass;
			pRemain->m_dSave = pAccn->m_dSave;
			pRemain->m_dCalcType = pAccn->m_dCalcType;
			pRemain->m_iMcgb = pAccn->m_iMcgb;	//2015.04.22 KSJ 매체구분 추가
			pRemain->m_b3RDCode = Is3RDCode(sCode);	//2015.04.22 KSJ K-OTC인지 체크
			pRemain->m_bKONEXCode = IsKONEXCode(sCode);
			//if (!pRemain->m_b3RDCode && !pRemain->m_bKONEXCode && (sCode.Left(1) == "A" || sCode.Left(3) == "M.A" || sCode.Left(3) == "N.A"))
			int itype = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)sCode.Right(6).operator LPCTSTR());
			pRemain->m_iCodetype = itype;
			m_slog.Format("[JANGO][%s]<%d> sCode=[%s] iCodetype=[%d] ", __FUNCTION__, __LINE__, sCode, itype); 
			Output_DebugString(m_slog);
			if (itype == singjongGoodType)
				pRemain->m_bSJSuikCode = true;
			
			

			pRemain->ParsingRemainData(pData);
		//	Sleep(200);
			pAccn->m_CodeMap.SetAt(keyS, pRemain);
#ifdef	_FILE_DEBUG
	dFile	<< "잔고 " << ii << " => " << keyS << endl;
#endif
			pData += L_srec;	sizeL += L_srec;
		}	
		if (sizeL > len || sizeL > checkL)
			break;
	}
//TRACE("len[%d] sizeL[%d] checkL[%d] \n", len, sizeL, checkL);
	if (bFuture)
		sendNContTR();
	return sAccn;
}

// 잔고 데이타 조회 내용
void CMapWnd::sendToControl(CString sAccn)
{
	CAccn	*pAccn = NULL;
	if (!m_AccnMap.Lookup(sAccn, (void*&)pAccn))	// 20070824
		return;
	if (pAccn)	sendToControl(sAccn, pAccn);
}

void CMapWnd::sendToControl(CString sAccn, CAccn *pAccn)
{
	CString sData, sCode;
	POSITION pos{};
	CString keyS;
	if (pAccn->m_bFuture)
	{
		for (pos = pAccn->m_CodeMap.GetStartPosition(); pos != NULL;)
		{
			CRemainFuture *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(pos, keyS, (void*&)pRemain);
			if (pRemain)	sData += pRemain->TotalData() + "\n";
		}
	}
	else
	{
#ifdef	_FILE_DEBUG
		dFile	<< "*잔고내역 화면으로 보내기->>>>> "<< endl;
#endif
		CRemain *pRemain = NULL;
		for (int ii = 0; ii < m_arKey.GetSize(); ii++)
		{
			keyS = m_arKey.GetAt(ii);
			if (pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				sData += pRemain->TotalData() + "\n";
//TRACE("key[%s]total[%.400s]\n",keyS, pRemain->TotalData());
#ifdef	_FILE_DEBUG
	//TRACE("key[%s]total[%.400s]\n",keyS, pRemain->TotalData());
	dFile	<< "잔고 " << keyS << " :: " << pRemain->TotalData() << endl;
#endif
			}
		}
#ifdef	_FILE_DEBUG
		dFile	<< "===================== "<< endl;
#endif
		/*
		for (pos = pAccn->m_CodeMap.GetStartPosition(); pos != NULL;)
		{
			CRemain *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(pos, keyS, (void*&)pRemain);

			if (pRemain)
			{
				sData += pRemain->TotalData() + "\n";
TRACE("total[%.400s]\n",pRemain->TotalData());
			}

		}	
		*/
	}

	detachCB(sAccn, sData, pAccn->m_errmsg);
}

// 잔고조회요청시(요청한 HWND값 가지고 있음)
void CMapWnd::attachCB(HWND hWnd, CString accn)
{
	CString	tmps, text;

	m_sync.Lock();
	const BOOL ret = m_multicb.Lookup(accn, text);

	if ( ret == TRUE)
	{
		RemoveCB(accn);
	}

	tmps.Format("%ld\t", (ULONG)hWnd);
//	text += tmps;
	text = tmps;//DOUBLED HWND BLOCK

	m_multicb.SetAt(accn, text);

	m_sync.Unlock();
}

// 잔고조회후(요청한 HWND값 없앰)
void CMapWnd::detachCB(CString accn, CString sData, CString emsg)
{
	int	idx=0;
	HWND	hWnd=0;
	CString	tmps, text;

	m_sync.Lock();
	//TRACE("accn: "+accn+"\n");
	if (m_multicb.Lookup(accn, text))
	{
		for (;;)
		{
			idx = text.Find('\t');
			if (idx == -1)
				break;
			tmps = text.Left(idx++);
			text = text.Mid(idx);
			hWnd = (HWND)atol(tmps);
			// 오류메세지
			if (!emsg.IsEmpty())	::SendMessage(hWnd, WM_REMAIN, 0, (LPARAM)(LPCTSTR)emsg);
			// 잔고데이타
			::SendMessage(hWnd, WM_REMAIN, 0, (LPARAM)(LPCTSTR)sData);
		}
//		m_multicb.RemoveKey(accn);//block buffet
	}
	/*
	if (!m_multicb.Lookup(accn, text))
	{
		//asdfasdf
		AfxMessageBox("1");
	}
	else
	{
		if (text == "") AfxMessageBox("2");
		else AfxMessageBox("text: "+text);
	}
	*/
	m_sync.Unlock();
}

void CMapWnd::sendNContTR(CString sLedger)
{
/* @@ Zeta  뭐야.. 이 코드...
	CString sData = ledgerTR("2700", "", "1");
	if (!sLedger.IsEmpty())
		sData = sLedger;
*/
	struct _ledgerH ledger;
	
	if (sLedger.IsEmpty())
	{
		CopyMemory(&ledger, ledgerTR("2700", "", "1"), L_ledgerH);
	}
	else
	{
		CopyMemory(&ledger, (LPCTSTR)sLedger, L_ledgerH);
		CString strNext = CString((char*)&ledger.nkey, sizeof(ledger.nkey));
		CopyMemory(&ledger, ledgerTR("2700", "", "1"), L_ledgerH);
		CopyMemory(&ledger.nkey, (LPCTSTR)strNext, sizeof(ledger.nkey));
		ledger.fkey[0] = '7';
	}

	CString sData = CString((char*)&ledger, L_ledgerH);

	struct f_cmid cmid;
	
	FillMemory(&cmid, L_fcmid, ' ');
	CopyMemory(&cmid.accn, m_sCurrAccn, sizeof(cmid.accn));

	CString passX;
	passX.Format("%s\t%s", m_sCurrPswd, m_sCurrAccn.Left(8));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	CopyMemory(&cmid.pswd, passX, passX.GetLength());

	CopyMemory(&cmid.gubn, "2", sizeof(cmid.gubn));
	CopyMemory(&cmid.sort, "1", sizeof(cmid.sort));
	sData += CString((char*)&cmid, L_fcmid);
	sendTR("PIHOFCHG", sData, KEY_NCONT, 0,cmid.accn);	
}

void CMapWnd::parsingNContData(char *pData)
{
	CString sCode, sGubn, sAmt, keyS;
	int nrec = 0;
	
	CAccn *pAccn = NULL;
	if (!m_AccnMap.Lookup(m_sCurrAccn, (void*&)pAccn))
		return;
	
	struct f_cmod	fmod;
	CopyMemory(&fmod, pData, L_fcmod);
	nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
	pData += L_fcmod;	
	
	for (int ii = 0; ii < nrec; ii++)
	{
		struct f_crec frec;
		CopyMemory(&frec, pData, L_fcrec);
		sCode = CString(frec.cod2, sizeof(frec.cod2));
		keyS = getKey(true, sCode, "", "", "");  //parsingNContData
		CRemainFuture *pRemain;
		if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
		{
			pData += L_fcrec;
			continue;
		}
		
		sGubn = CString(frec.odgb, sizeof(frec.odgb));
		sAmt =  CString(frec.wqty, sizeof(frec.wqty));
		pRemain->SetNoCont(sGubn.Left(5), atoi(sAmt));
		
		pData += L_fcrec;
	}
}

void  CMapWnd::parsingRTSx(struct _alertR* alertR) {
	DWORD* data{};

	CString sCode = alertR->code, sSym, sData, sCurr, sDiff = "", sRate = "", sAccn;

	if (sCode.GetLength() < 6)
		return;

	data = (DWORD*)alertR->ptr[0];

	if (!data) return;

	if (!data[23])
		return;

	sCurr = (char*)data[23];

	if (data[24]) {
		sDiff = (char*)data[24];
	}

	if (data[33]) {
		sRate = (char*)data[33];
	}

	if (sRate == "0.00") sRate = " ";

	if (!m_AccnMap.GetCount())
		return;

	POSITION AccnPos{};

	for (AccnPos = m_AccnMap.GetStartPosition(); AccnPos != NULL;)
	{
		CAccn* pAccn = NULL;
		m_AccnMap.GetNextAssoc(AccnPos, sAccn, (void*&)pAccn);
		if (pAccn->m_bFuture == (sCode.GetLength() == 8 ? false : true))
			continue;

		if (pAccn->m_bFuture)
		{
			CRemainFuture* pRemain;
			CString keyS = getKey(true, sCode, "", "", "");			// key 선물
			if (pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				if (!pRemain->CalPgsonik(sCurr, sDiff, sRate))
					continue;

				m_ShMemory->SendTrigger(pAccn->m_accn + "\tU\t" + pRemain->TotalData());
			}
		}
		else
		{
			CRemain* pRemain{};
			CString keyS;
			CString keyCodeS;
			for (POSITION codePos = pAccn->m_CodeMap.GetStartPosition(); codePos != NULL;)
			{
				pAccn->m_CodeMap.GetNextAssoc(codePos, keyS, (void*&)pRemain);
				keyCodeS = keyS.Mid(2, LEN_ACODE);  //NXT  KRX  00A005930  NXT N.A005930   
				keyCodeS.TrimRight();

				if (keyCodeS == sCode)
				{
					if (sCurr == pRemain->m_curr)
						continue;
					if (!pRemain->CalPgsonik(sCurr, sRate))
					{
						continue;
					}
#ifdef	_FILE_DEBUG
					//	dFile	<< "[sise update]" << sAccn  << "\tU\t" << pRemain->TotalData() << endl;
#endif
					HWND hwnd = GetRightHwnd(keyS);
					m_ShMemory->SendTrigger(pAccn->m_accn + "\tU\t" + pRemain->TotalData(), hwnd);
				}
				else
				{

				}
			}
		}
	}


}

void CMapWnd::parsingRTS(CString sRtsData)
{
	CString sCode = parser(sRtsData, "\t"), sSym, sData, sCurr, sDiff = "", sRate = "", sAccn;
	
	CString tot_str(_T(""));
	
	while(true)
	{
		if (sRtsData.Find("\n")!=-1)
		{
			tot_str = parser(sRtsData, "\n");
			tot_str.TrimLeft(); tot_str.TrimRight();
			if (tot_str != "") sRtsData = tot_str;
			if (tot_str == "") break;
		} else break;	

	}
			
	/*
	if (sCode == "A000660")
	{	
		/*
		if (sRtsData.Find("\n")!=-1)
		{
			sRtsData.TrimLeft(); sRtsData.TrimRight();
			//MessageBox("Next Line Included");
			TRACE("sRtsData: %s]\n", sRtsData);
		}
	
		
		//TRACE("sRtsData: 067250]\n");
		TRACE("sRtsData: %s]\n",sRtsData);
	}
	*/

	CMapStringToString symMap;
	POSITION AccnPos{};

	if (sCode.GetLength() < 6)
		return;

	while (!sRtsData.IsEmpty())
	{
		sSym = parser(sRtsData, "\t");
		sData = parser(sRtsData, "\t");
		if (sData.IsEmpty())
			continue;
		symMap.SetAt(sSym, sData);
		//if (sSym == "024") break;
	}

/*
	if(Is3RDCode(sCode))	//2014.10.06 KSJ K-OTC일때는 가중평균가로 평가손익 계산
	{
		if (!symMap.Lookup("039", sCurr))
			return;
	}

	else*/ 

	if (!symMap.Lookup("023", sCurr))
		return;
	symMap.Lookup("024", sDiff);
	symMap.Lookup("033", sRate);
	
	if (sRate=="0.00") sRate = " ";
	
	//if (sCode == "A000660")  TRACE("sCode[%s] currS[%s] diff[%s]\n", sCode, sCurr, sDiff);
	/*
	//if (sCode == "A000100")
	{
		CWnd* dd = FindWindow(NULL, "test form");
		if(dd)
		{
			int isCurr, isDiff;
			CString tsCurr, tsDiff;
			tsCurr = sCurr; tsDiff = sCode;
			tsDiff.Delete(0,1);

			tsCurr.Remove(_T(','));
			
			dd->SendMessage(WM_USER,atoi(tsDiff),atoi(tsCurr));
		}
	}
	*/
	if (!m_AccnMap.GetCount())
		return;

	//int nCount = 0;

	for (AccnPos = m_AccnMap.GetStartPosition(); AccnPos != NULL;)
	{
		CAccn	*pAccn = NULL;
		m_AccnMap.GetNextAssoc(AccnPos, sAccn, (void*&)pAccn);
		if (pAccn->m_bFuture == (sCode.GetLength()==8 ? false : true))
			continue;

		if (pAccn->m_bFuture)
		{
			CRemainFuture *pRemain;
			CString keyS = getKey(true, sCode, "", "", "");			// key 선물
			if (pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
			{
				if (!pRemain->CalPgsonik(sCurr, sDiff, sRate))
					continue;
				
				m_ShMemory->SendTrigger(pAccn->m_accn + "\tU\t" + pRemain->TotalData());
			}
		}
		else
		{
			CRemain* pRemain{};
			CString keyS;
			CString keyCodeS;
			for (POSITION codePos = pAccn->m_CodeMap.GetStartPosition(); codePos != NULL;)
			{
				pAccn->m_CodeMap.GetNextAssoc(codePos, keyS, (void*&)pRemain);
//				keyCodeS = keyS.Left(LEN_ACODE);
				keyCodeS = keyS.Mid(2, LEN_ACODE);     //parsingRTS(...) 는 사용되지 않는다
				keyCodeS.TrimRight();
				
				if (keyCodeS == sCode)
				{
//TRACE("keyS[%s] codeS[%s] curr[%s]\n", keyS, keyCodeS, sCurr);
// 					CString strTemp;
// 					strTemp.Format("[KSJ][parsingRTS] nCount[%d] sAccn[%s] pAccn->m_accn[%s] keyS[%s] sCode[%s] scurr[%s] data[%s]", nCount++, sAccn, pAccn->m_accn, keyS, sCode, sCurr, pRemain->TotalData());
// 					pRemain->testSaveFile(strTemp);

					if(sCurr == pRemain->m_curr)	
						continue;

					if (!pRemain->CalPgsonik(sCurr, sRate))
					{
						//TRACE("keyS[%s] codeS[%s] curr[%s]\n", keyS, keyCodeS, sCurr);
						continue;
					}
#ifdef	_FILE_DEBUG
//	dFile	<< "[sise update]" << sAccn  << "\tU\t" << pRemain->TotalData() << endl;
#endif
					HWND hwnd = GetRightHwnd(keyS);
					//TRACE(pAccn->m_accn  + "\tU\t" + pRemain->TotalData()+"\n");
					m_ShMemory->SendTrigger(pAccn->m_accn  + "\tU\t" + pRemain->TotalData(),hwnd);
				}
				else
				{
					
				}
			}
		}
	}
}

HWND CMapWnd::GetRightHwnd(CString key)
{
	CString sCode,text,tmps;
	HWND	hWnd{};
	int	idx=0;
	const int i = key.Find("|");

	sCode = key.Mid(i+1);	

	m_sync.Lock();

	if (m_multicb.Lookup(sCode, text))
	{
		for (;;)
		{
			idx = text.Find('\t');
			if (idx == -1)
				break;
			tmps = text.Left(idx++);
			text = text.Mid(idx);
			hWnd = (HWND)atol(tmps);
			//TRACE("__FINDING__[%ld][%s]\n",hWnd,sCode);
		}
	}
	m_sync.Unlock();

	return hWnd;
}

CString CMapWnd::parsingNotice(CString str)
{ 
//TRACE("notice[%.400s]\n", str);
	CString dataS = parser(str, "\n"), sAcc = "", sTemp = "", sOrder = "", sJCode = "", sDate = "", keyS = "", 
		sSygb = "", sSymt = "",	sSendData = "", sJGType = "", lookupS = "";
	bool	bNewCode = false;

	m_slog.Format("[jango][%s]<%d> 체결시작 str=[%s] ", __FUNCTION__, __LINE__, str);
	Output_DebugString(m_slog);

	while(!dataS.IsEmpty())
	{
		CString sKey = parser(dataS, "\t");
		if (atoi(sKey) == 901)
		{				//	계좌번호
			sAcc = parser(dataS, "\r");
			if (sAcc.GetLength() > 11)
			{
				sTemp = sAcc;
				sAcc = parser(sTemp, "\t");
				parser(sTemp, "\t");	// 902를 제거
			}

			sAcc.TrimLeft(); sAcc.TrimRight();	//2015.02.05 KSJ 계좌번호 trim 해준다.
			break;
		}
	}
	
	dataS = str;
	
	CMapStringToString	mapNotice;
	mapNotice.RemoveAll();

	CString sSym, sDat, sJggb, smkgb;
	while (!dataS.IsEmpty())
	{
		sSym = parser(dataS, "\t"); 
		sDat = parser(dataS, "\t");
		sDat.TrimLeft();	sDat.TrimRight();
		mapNotice.SetAt(sSym, sDat);

		m_slog.Format("[jango][%s]<%d> 체결 심볼 sSym=[%s] sDat=[%s]", __FUNCTION__, __LINE__, sSym, sDat);
		Output_DebugString(m_slog);

	}

	mapNotice.Lookup(RT_JCODE, sJCode);		// 종목코드
	sJCode.TrimLeft(); sJCode.TrimRight();	//2014.11.03 KSJ 종목코드 trim해준다.
	mapNotice.Lookup(RT_CTDATE, sDate);		// 신용대출일
	mapNotice.Lookup(RT_CREDIT, sSygb);		// 신용구분
	mapNotice.Lookup(RT_JGTYPE, sJggb);
	mapNotice.Lookup(RT_MKGB, smkgb);

	sSygb.Format("%02d", atoi(sSygb));
	sJggb.Format("%02d", atoi(sJggb));
	mapNotice.SetAt(RT_CREDIT, sSygb);
	mapNotice.SetAt(RT_JGTYPE, sJggb);
	
	//

	mapNotice.Lookup(RT_CTSYMT, sSymt);		// 신용대출금액
//TRACE("종목코드[%s] 신용날짜[%s] 신용구분[%s] 대출금[%s]\n", sJCode, sDate, sSygb, sSymt);

	bool m_bSpread = false;
	if ((sJCode.GetLength()==8) && (sJCode.Mid(0,1)=="4"))	m_bSpread = true;
	
	if ((sJCode[0] == 'J')&&(sJCode.GetLength() == 9)) //신주인수원인경우 
	{
		mapNotice.RemoveAll();
		return "";
	}
	if (sAcc.IsEmpty() || sJCode.IsEmpty())
	{
		mapNotice.RemoveAll();
		return "";
	}

	//if (smkgb == "2")  //nxt
	//{
	//	sJCode = "N." + sJCode;
	//}
	//else if (smkgb == "3") //통합
	//{
	//	sJCode = "M." + sJCode;
	//}

	//asdfasdf
	CAccn *pAccn = NULL;
	bool bFind = false;
	POSITION	accPos;		//codePos
	CString sTRKey;
	for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
	{
		m_AccnMap.GetNextAssoc(accPos, sTRKey, (void*&)pAccn);
		//TRACE("pAccn->m_accn: [%s], sAcc: [%s]", pAccn->m_accn, sAcc);
		if (pAccn->m_accn == sAcc)
		{
			//AfxMessageBox("bFind=true");
			bFind = true;
			break;
		}
	}
	if (!bFind)
	{
		mapNotice.RemoveAll();
		return "";
	}

	if (pAccn->m_bFuture)	// 선물
	{
		if(!pAccn->m_bReceive)	return "";	//2015.03.04 KSJ Receive받지 않으면 안태워준다.

		CRemainFuture* pRemain{};
		int type = 0;
		if (m_bSpread)
		{
			m_ShMemory->SendTrigger(sAcc + "\tU\t" + sJCode+"\t ");
		}

		for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
		{
			m_AccnMap.GetNextAssoc(accPos, sTRKey, (void*&)pAccn);
			if (pAccn->m_accn==sAcc)
			{
 				keyS = getKey(true, sJCode, "", "", "", atoi(sTRKey));		// key 선물

				if (!pAccn->m_bCredit)	// 신용 조회가 아닌경우
				{
					if (atoi(sSygb) > 0 && atoi(sSygb) < 10)
					{
						mapNotice.RemoveAll();
					}
				}
				type = 0;
				bNewCode = false;
				if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
				{
					pRemain = new CRemainFuture(this);
					pRemain->m_system = m_system;
					bNewCode = true;
				}
				
				type = pRemain->CalRemainData(mapNotice, keyS, bNewCode);
				
// 				CString strTemp;
// 				strTemp.Format("[KSJ][parsingNotice] Type[%d] sAcc[%s] pAccn->m_accn[%s] sJCode[%s] keyS[%s] sTRKey[%s] data[%s] ", type, sAcc, pAccn->m_accn, sJCode, keyS, sTRKey, pRemain->TotalData());
// 				pRemain->testSaveFile(strTemp);

				if ((type == 3) || (type==1)) //insert or update
				{
					pAccn->m_CodeMap.SetAt(keyS, pRemain);
				}
				else if (type == 2) //delete
				{
					if (pRemain)
					{
						//새로추가
						pRemain->DestroyWindow();
						delete pRemain;	pRemain = NULL;
					}
					pAccn->m_CodeMap.RemoveKey(keyS);
				}

				if (bNewCode && type == 0 && pRemain)
				{
					pRemain->DestroyWindow();
					delete pRemain;	pRemain = NULL;
				}
			}
		}

		//1: update, 2: delete, 3: OOP Send(insert)
		if (type == 1)
		{
			m_ShMemory->SendTrigger(sAcc + "\tU\t" + pRemain->TotalData());
		}
		else if (type == 2)
		{
			m_ShMemory->SendTrigger(sAcc + "\tD\t" + keyS);
		}
		else if (type == 3)
		{
			CString sData, sSym = "4";
			if (sJCode.GetAt(0) == '1')
			{
				if (sJCode.GetAt(1) == '0')
					sSym = "3";
				else
					sSym = "5";
			}
			
			// 20070823 수정
			CString currS = "", diffS = "";;
			
			mapNotice.Lookup(RT_CCURR, currS);
			mapNotice.Lookup(RT_CDIFF, diffS);
			
			if (atof(currS) == 0.0)	// 현재가 없으면 체결가로
			{
				mapNotice.Lookup(RT_CPRICE, currS);
				currS.Format("%.2f", atof(currS)/(float)100.0);
			}
			
			pRemain->m_curr = currS;
			pRemain->m_diff = diffS;
			pRemain->CalPgsonik();
			m_ShMemory->SendTrigger(sAcc + "\tI\t" + pRemain->TotalData());
		}
	}
	else	// 주식
	{
		CRemain* pRemain{};
		int type = 0;
		for (accPos = m_AccnMap.GetStartPosition(); accPos != NULL;)
		{
			m_AccnMap.GetNextAssoc(accPos, sTRKey, (void*&)pAccn);
			if (pAccn->m_accn==sAcc)
			{
				if (!pAccn->m_bCredit)	// 신용 조회가 아닌경우
				{
					if (atoi(sSygb) > 0 && atoi(sSygb) < 10)
					{
						mapNotice.RemoveAll();
					}
				}
			
				mapNotice.Lookup(RT_JGTYPE, sJGType);	// 잔고구분
				type = 0;
				bNewCode = false;
				CString findJCode = sJCode;
				keyS = getKey(false, findJCode, sDate, sSygb, sJggb, atoi(sTRKey));		// key  NXT
				if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
				{
					m_slog.Format("[jango][%s]<%d> findJCode=[%s]  처음 없음 ", __FUNCTION__, __LINE__, findJCode);
					Output_DebugString(m_slog);
					findJCode = "N." + sJCode;
					keyS = getKey(false, findJCode, sDate, sSygb, sJggb, atoi(sTRKey));		// key NXT
					if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
					{
						m_slog.Format("[jango][%s]<%d> findJCode=[%s]  두번째 없음 ", __FUNCTION__, __LINE__, findJCode);
						Output_DebugString(m_slog);
				
						findJCode = "M." + sJCode;
						keyS = getKey(false, findJCode, sDate, sSygb, sJggb, atoi(sTRKey));		// key NXT
						if (!pAccn->m_CodeMap.Lookup(keyS, (void*&)pRemain))
						{
							m_slog.Format("[jango][%s]<%d> findJCode=[%s]  세번째도 없음 ", __FUNCTION__, __LINE__, findJCode);
							Output_DebugString(m_slog);

							findJCode = sJCode;

							if (smkgb == "2")  //nxt
							{
								findJCode = "N." + sJCode;
							}
							else if (smkgb == "3") //통합
							{
								findJCode = "M." + sJCode;
							}

							keyS = getKey(false, findJCode, sDate, sSygb, sJggb, atoi(sTRKey));    // key NXT
							m_slog.Format("[jango][%s]<%d> 새로운잔고 findJCode=[%s] keyS=[%s]  ", __FUNCTION__, __LINE__, findJCode, keyS);
							Output_DebugString(m_slog);

							bool b_etf = false;
							struct	hjcodex* hjc;

							lookupS = findJCode;
							int ifind = lookupS.Find('.');
							if (ifind != -1)
								lookupS = findJCode.Mid(ifind + 1);

							if (m_hname.Lookup(lookupS.Mid(1, 7), (void*&)hjc))   //nxt ?  체결이   NXT종목이면 N.A005930 이라서 . 이 있으면 그다음부터 한다. 종목마스터는 N. 로 안주니까..
							{
								if (hjc->ssgb == jmETF)	// ETF
								{
									b_etf = true;
								}
							}
							pRemain = new CRemain(this);
							pRemain->m_system = m_system;
							pRemain->m_betf = b_etf;

							pRemain->FillString(sDate, LEN_DATE);
							pRemain->FillString(sSygb, LEN_SYGB);
							pRemain->FillString(sSymt, LEN_SYMT);
							pRemain->FillString(sJGType, LEN_JGGB);
							pRemain->m_date = sDate;
							pRemain->m_creditS = sSygb;
							//@@ Zeta  신용관련 주석처리 되어 있던것 해제 함.
							pRemain->m_symt = sSymt;

							///buffet
							pRemain->m_dFee = pAccn->m_dFee;
							pRemain->m_dMass = pAccn->m_dMass;
							pRemain->m_dSave = pAccn->m_dSave;
							pRemain->m_dCalcType = pAccn->m_dCalcType;
							pRemain->m_iMcgb = pAccn->m_iMcgb;	//2015.04.22 KSJ 매체구분 추가
							pRemain->m_b3RDCode = Is3RDCode(findJCode);	//2015.04.22 KSJ K-OTC인지 체크
							pRemain->m_bKONEXCode = IsKONEXCode(findJCode);
							int itype = m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)findJCode.Right(6).operator LPCTSTR());
							pRemain->m_iCodetype = itype;
							m_slog.Format("[JANGO][%s]<%d> sJCode=[%s] iCodetype=[%d] ", __FUNCTION__, __LINE__, sJCode, itype);
							Output_DebugString(m_slog);
							if (itype == singjongGoodType)
								pRemain->m_bSJSuikCode = true;

							//buffet
							bNewCode = true;
						}
					}
				}

				type = pRemain->CalRemainData(mapNotice, keyS, bNewCode);   //주식
				if ((type == 3) || (type==1)) //insert or update
				{
					pAccn->m_CodeMap.SetAt(keyS, pRemain);
				}else if (type == 2) //delete
				{
					if (pRemain)
					{
						//새로추가
						pRemain->DestroyWindow();
						delete pRemain;	pRemain = NULL;
					}
					pAccn->m_CodeMap.RemoveKey(keyS);
				}

				if (type==1)  //존재하는 잔고 데이터 갱신
				{
					m_ShMemory->SendTrigger(sAcc + "\tU\t" + pRemain->TotalData(), GetRightHwnd(keyS));
				}
				else if (type==2)  //잔고 삭제
				{
					m_ShMemory->SendTrigger(sAcc + "\tD\t"+ keyS, GetRightHwnd(keyS));
				}
				else if (type==3)  //잔고 추가
				{
					CString currS = "";
					CString rateS = "";
					mapNotice.Lookup(RT_CCURR, currS);
					
					if (atof(currS) == 0.0)	// 현재가 없으면 체결가로
					{
						mapNotice.Lookup(RT_CPRICE, currS);
						currS.Format("%.f", atof(currS)/(float)100.0);
					}
					
					pRemain->m_curr = currS;
					
					mapNotice.Lookup(RT_CRATE, rateS);
					pRemain->m_srate = rateS;
					pRemain->CalPgsonik("", "", TRUE);
					m_ShMemory->SendTrigger(sAcc + "\tI\t" + pRemain->TotalData(), GetRightHwnd(keyS));
				}
				
				if (bNewCode && type == 0 && pRemain)
				{
					//새로추가
					pRemain->DestroyWindow();
					delete pRemain;	pRemain = NULL;
				}
			}
		}
	}
	mapNotice.RemoveAll();
	return "";
}

// 2006.10.30 매입단가 수수료 미포함 추가
BOOL CMapWnd::getRemainFee()
{
	CString root = variant(homeCC, "");
	CString	name = variant(nameCC, "");
	CString userPath;
	userPath.Format("%s\\user\\%s\\%s", root, name, "ORDCFG.INI");
	BOOL bFee = FALSE;
	bFee = (BOOL)GetPrivateProfileInt("Common", "NoFee", 0, userPath);
	return bFee;
}

CString	CMapWnd::getKey(bool bFuture, CString codeS, CString dateS, CString sygbS, CString sJggb, int trkey)
{
	if (bFuture)	return codeS;
	/*
	if (atoi(sygbS) >= 10 || atoi(sygbS) == 0)	// 신용구분 아니면 0
		sygbS = "  ";
	*/
	CString keyS;
	keyS.Format("%-*s%-*s%-*s%-*s|%d", LEN_JGGB, sJggb, LEN_ACODE, codeS, LEN_DATE, dateS, LEN_SYGB, sygbS, trkey);
	
	return keyS;
}

//=================================================================================================
CString CMapWnd::variant(int comm, CString data)
{
	return VariantA(m_pParent, comm, data);
}

int CMapWnd::openView(int type, CString data)
{
	return openViewA(m_pParent, type, data);
}

CString CMapWnd::parser(CString &srcstr, CString substr)
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

CString CMapWnd::ledgerTR(CString sTran, CString sSvcn, CString sGubn)
{
	CString strUser;
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	strUser = variant(userCC, "");

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(&ledger.svcd, sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, m_sCurrAccn.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
/*
	if(!sSvcn.IsEmpty())	//2014.09.18 KSJ 서비스번호가 있는경우 암호화해서 넣어준다.
	{
		CopyMemory(&ledger.hsiz, _T("44"), 2);

		if(m_sCurrPswd.GetLength() == 44)		//2014.09.30 KSJ 비번이 44byte면 암호화된 비번이므로
			CopyMemory(&ledger.epwd, m_sCurrPswd, 44);
		else
			CopyMemory(&ledger.epwd, GetEncPassword(m_sCurrPswd), 44);
	}
*/

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = sGubn.GetAt(0);
	ledger.odrf[0] = '1';

	return CString((char*)&ledger, L_ledgerH);
}

BOOL CMapWnd::sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt)
{
	CString sTRData = "";
	CString sKey = "";
//	int ikey = -1;
	struct _userTH user;
	strncpy(&user.trc[0], sTR, 8);
	
	/*
	m_sync.Lock();
	if (m_AccnKey.Lookup(Acnt, sKey))
	{	
		CString tmp;
		tmp.Format("%d",nKey);
		ikey = atoi(sKey);
		TRACE("Key: "+sKey+"<=>"+tmp+"\n");
	}
	m_sync.Unlock();
	*/
	/*
	CString tmp;
	tmp.Format("%d",nKey);
	TRACE("Key: "+tmp+"\n");
	*/
	user.key  = nKey;//ikey;

	user.stat = nStat;
	
	sTRData  = CString((char*)&user, sizeof(_userTH));
	sTRData += sData;
	
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sData.GetLength()), 
		(LPARAM)(const char*)sTRData); 
}


CString CMapWnd::getTime()
{
	const CTime curr = CTime::GetCurrentTime();
	CString timeS;
	timeS.Format("[%02d/%02d %02d:%02d:%02d]", curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());
	return timeS;
}

void CMapWnd::removeAccn(CString accn)
{
	CAccn *pAccn = NULL;
	if (m_AccnMap.Lookup(accn, (void*&)pAccn))
	{
		if (pAccn)
		{
			removeRemain(pAccn);
//			delete pAccn;	
			pAccn = NULL;
		}
	}
	m_AccnMap.RemoveKey(accn);
}

void CMapWnd::removeRemain(CAccn *pAccn)
{
	if (!pAccn)	return;

	POSITION codePos;
	CString	sKey;
	for (codePos = pAccn->m_CodeMap.GetStartPosition(); codePos != NULL;)
	{
/* @@ Zeta 아놔.. 왜 이러니..
		if (pAccn->m_bFuture)
		{
			CRemainFuture *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(codePos, sKey, (void*&)pRemain);
			if (pRemain)
			{
				pRemain->DestroyWindow();
				delete pRemain;	pRemain = NULL;
			}
		}
		else
		{
			CRemain *pRemain = NULL;
			pAccn->m_CodeMap.GetNextAssoc(codePos, sKey, (void*&)pRemain);
			if (pRemain)
			{
				pRemain->DestroyWindow();
				delete pRemain;	pRemain = NULL;
			}
		}
*/

		CWnd* pRemain = NULL;
		pAccn->m_CodeMap.GetNextAssoc(codePos, sKey, (void*&)pRemain);
		
		if (pRemain)
		{
			//TRACE("sKey: "+sKey+"\n");
			pRemain->DestroyWindow();
			delete pRemain;
			pRemain = NULL;
		}
	}

	pAccn->m_CodeMap.RemoveAll();
}

LRESULT CMapWnd::OnCloseOfCtrl(WPARAM wParam, LPARAM lParam)
{
	//return;
	CString accn = (char*) lParam;
	HWND shwnd = (HWND) wParam;
	CString text = "";

	CString tmp;
	tmp.Format("%d", (ULONG)shwnd);
	
	if (m_AccnKey.Lookup(tmp,text))
	{
		CAccn *pAccn = NULL;
		
		if (m_AccnMap.Lookup(text, (void*&)pAccn))	// 20070824
		{
			if (pAccn)
			{
				removeRemain(pAccn);
				//delete pAccn;
			}
			m_AccnMap.RemoveKey(text);
		}
		
		//실시간 잔고 key값으로 초기화함
		//TRACE("삭제된 키값: "+text+"\n");
		m_AccnKey.RemoveKey(tmp);
		
		
		bool bFuture = (accn.Mid(3,1) == "2");
		sendRemainTR("           ","        ",bFuture,true,0,0,0,0,0,shwnd);//buffet
		
		/*
		CString sData, sCode = "A005930";

		if (sCode.Left(1) == "A")
			sCode = sCode.Mid(1, 6);
		
		sData.Format("1301%c%s\t2023\t", 0x7f, sCode);
		sendTR("POOPPOOP", sData, atoi(text), 0, US_OOP);
		*/
		
		
	}
	else
	{
		if (m_LawKey.Lookup(tmp, text))	// 20070824
		{
			m_LawKey.RemoveKey(tmp);
		}
		CString stext;
		
	}

	
	return 0;
}

int CMapWnd::GetNewAccHandleKey()
{
	int itmp = 1;
	POSITION	pos{};		//keyPos
	CString tmp_key, tmp_val, tmp_data;
	bool bfind = false;
	while(true)
	{
		bfind = false;
		for (pos = m_AccnKey.GetStartPosition(); pos != NULL;)
		{
			m_AccnKey.GetNextAssoc(pos,tmp_key,tmp_val);
			tmp_data.Format("%d",itmp);
			if(tmp_val == tmp_data)
			{
				bfind = true;
				break;
			}	
		}
		if (bfind) itmp++;
		else
		{
			m_hcnt = itmp;
			break;
		}	
	}
	//m_hcnt++;
	if (m_hcnt>247)
	{
		MessageBox("할당가능한 실시간 잔고 수를 초과하였습니다.\n 재접속하시면 실시간 잔고할당을 초기화 할수 있습니다.","IBK투자증권");
		return -1;
	}	
	m_hcnt;
	return m_hcnt;
}

int CMapWnd::GetNewLawHandleKey()
{
	int itmp = 1;
	POSITION	pos{};		//keyPos
	CString tmp_key, tmp_val, tmp_data;
	bool bfind = false;
	while(true)
	{
		bfind = false;
		for (pos = m_LawKey.GetStartPosition(); pos != NULL;)
		{
			m_LawKey.GetNextAssoc(pos,tmp_key,tmp_val);
			tmp_data.Format("%d",itmp);
			if(tmp_val == tmp_data)
			{
				bfind = true;
				break;
			}	
		}
		if (bfind) itmp++;
		else
		{
			m_hcnt = itmp;
			break;
		}	
	}
	//m_hcnt++;
	if (m_hcnt>247)
	{
		MessageBox("할당가능한 실시간 잔고 수를 초과하였습니다.\n 재접속하시면 실시간 잔고할당을 초기화 할수 있습니다.","IBK투자증권");
		return -1;
	}	
	m_hcnt;
	return m_hcnt;
}

CString CMapWnd::GetHandleByKey(CString key)
{
	int	idx=0;
	CString	tmps, text;
	tmps = "-1";
	m_sync.Lock();
	if (m_multicb.Lookup(key, text))
	{
		
			idx = text.Find('\t');
			tmps = text.Left(idx++); //핸들
			text = text.Mid(idx);
	}
	m_sync.Unlock();
	return tmps;
}

void CMapWnd::sendLawCheg(CString acc_gb, CString mng_no, CString mk_cd, CString grp_no, HWND sHwnd)
{
	CAccn *pAccn = NULL;
	CString sData;
	int skey=-1;
	CString AccHandleKey, sHandle; sHandle.Format("%d",(ULONG)sHwnd);
	
	if (m_LawKey.Lookup(sHandle,AccHandleKey))
	{
		skey = atoi(AccHandleKey);
	}
	else
	{
		const int kNum = GetNewLawHandleKey();
		AccHandleKey.Format("%d",kNum);
		m_LawKey.SetAt(sHandle, AccHandleKey);
		skey = kNum;
	}
	//TRACE("KEY: %d\n",skey);
	//CString asdf; asdf.Format("KEY: %d", skey);

}

void CMapWnd::SendLawNotice(CString sData)
{
	//TRACE("SendLawNotice: "+sData+"\n");
	CString dataS = parser(sData, "\n"), sAcc = "", sTemp = "", sOrder = "", sJCode = "", sDate = "", keyS = "", sOrdNo = "", sOrdGb = "",
		sSygb = "", sSymt = "",	sSendData = "", sJGType = "", sCprice = "", sCamt = "", sOrdQty = "", sJName = "", sJGB = "", sAccName = "", sMCamt = "", sState = "", sJtype = "";

	CString full_data;
//	const bool	bNewCode = false;
	
	while(!dataS.IsEmpty())
	{
		CString sKey = parser(dataS, "\t");
		if (atoi(sKey) == 901)
		{	//	계좌번호
			sAcc = parser(dataS, "\r");
			if (sAcc.GetLength() > 11)
			{
				sTemp = sAcc;
				sAcc = parser(sTemp, "\t");
				parser(sTemp, "\t");	// 902를 제거
			}
			break;
		}
	}
	/*
	if (sAcc == "00110000449") 
	{
		TRACE("%s\n",sData);
		//TRACE("sState: %s\n",sState);
	}
	*/
	dataS = sData;
	
	CMapStringToString	mapNotice;
	mapNotice.RemoveAll();

	CString sSym, sDat, sJggb;
	while (!dataS.IsEmpty())
	{
		sSym = parser(dataS, "\t"); 
		sDat = parser(dataS, "\t");
		sDat.TrimLeft();	sDat.TrimRight();
		mapNotice.SetAt(sSym, sDat);
	}
	mapNotice.Lookup(RT_ANAME , sAccName);		// 계좌명
	mapNotice.Lookup(RT_JCODE , sJCode );		// 종목코드
	mapNotice.Lookup(RT_JNAME , sJName );		// 종목명
	mapNotice.Lookup(RT_GUBN  , sJGB   );		// 구분
	mapNotice.Lookup(RT_ORDQTY , sOrdQty);		// 주문수량
	mapNotice.Lookup(RT_CAMT  , sCamt  );		// 체결수량
	mapNotice.Lookup(RT_MCAMT , sMCamt );		// 미체결수량
	mapNotice.Lookup(RT_CPRICE, sCprice);		// 체결단가
	mapNotice.Lookup(RT_JOTYPE, sState);		// 주문상태
	mapNotice.Lookup(RT_ORDNO , sOrdNo);		// 주문번호
	mapNotice.Lookup(RT_ORDGB , sOrdGb);		// 주문구분
	mapNotice.Lookup(RT_JTYPE , sJtype);		// 종목시장


	mapNotice.Lookup(RT_CREDIT, sSygb  );		// 신용구분
	mapNotice.Lookup(RT_JGTYPE, sJggb  );		// 잔고코드
	if (sJCode.GetLength()==8) return; //선물옵션 종목코드일때 필터링하기위해 처리
	if (sState=="1") sCamt = "0";  //접수일때 체결수량에 주문수량이 내려와서 "0"으로 초기화시킴
	if (sState=="3") return; //취소접수
	if (sState=="2") return; //정정접수
	if (sState=="12") return; //정정확인
	if ((sState=="14")||(sState=="15")||(sState=="16")) return; //거부
	
	if (sState=="13") //취소확인
	{
		sOrdQty.Format("%d",atoi(sOrdQty)*-1);
			
	}
	if (sJGB == "매수")
	{
		if (sJtype == "KSE")
		{
			sJGB = "2\t02\t(E)현금매수";
		}
		else if (sJtype == "KODAQ")
		{
			sJGB = "2\t02\t(Q)현금매수";
		}
		else
			sJGB = "2\t02\t현금매수";
		
	}
	else
	{
		if (sJtype == "KSE")
		{
			sJGB = "1\t01\t(E)현금매도";
		}
		else if (sJtype == "KODAQ")
		{
			sJGB = "1\t01\t(Q)현금매도";
		}
		else
			sJGB = "1\t01\t현금매도";
		
	}
	//mapNotice.Lookup(RT_CTSYMT, sSymt);		// 신용대출금액
	//TRACE("계좌번호[%s] 계좌명[%s] 종목코드[%s] 종목명[%s] 주문구분[%s] 주문수량[%s] 체결수량[%s] 체결단가[%s] 잔고코드[%s]\n", sAcc, sAccName, sJCode, sJName, sJGB, sOrdQty, sCamt, sMCamt, sCprice, sJggb);

	full_data.Format("|%11s%-12s%c%11s\t%-40s\t%7s\t%-40s\t%-40s\t+\t%15s\t+\t%15s\t+\t+\t%9s\t.00",sAcc,sJCode,sJGB[0],sAcc, sAccName, sJCode, sJName, sJGB, sOrdQty, sCamt, sCprice);

	//계좌번호(11) 종목코드(7) 매매구분(1) 주문수량(16) 체결수량(16) 미체결수량(16) 체결단가(16) 계좌명(40)

	sData.Format("%11s%-12s%c%16s%16s%16s%16s%-40s",sAcc,sJCode,sJGB[0], sOrdQty, sCamt, sMCamt, sCprice, sAccName);
	//TRACE(sData+"\n");
	//AfxMessageBox(sData+full_data);
	

	if (sAcc.IsEmpty() || sJCode.IsEmpty())
	{
		mapNotice.RemoveAll();
		return;
	}
	sAcc.Insert(3,"-");
	sAcc.Insert(6,"-");
	CString sMsg;
	if ((sState == "1")) 
	{
	}
	if (sState.GetLength()==1)  //주문접수
	{
		if (sJGB[0]!='1')
		{
			sMsg.Format("#[매    수] %s %s(%s) %s(%s) %s %s주", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sOrdQty);
		}
		else
		{
			sMsg.Format("#[매    도] %s %s(%s) %s(%s) %s %s주", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sOrdQty);
		}
	}else //주문체결
	{
		if (sJGB[0]!='1')
		{
			if (sState=="13")
			{
				sMsg.Format("#[매수취소] %s %s(%s) %s(%s) %s %s주", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
			else
			{
				sMsg.Format("#[매수체결] %s %s(%s) %s(%s) %s %s주", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
		}
		else
		{
			if (sState=="13")
			{
				sMsg.Format("#[매도취소] %s %s(%s) %s(%s) %s %s주", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
			else
			{
				sMsg.Format("#[매도체결] %s %s(%s) %s(%s) %s %s주", sOrdNo, sAccName, sAcc, sJName, sJCode, sOrdGb, sCamt);
			}
		}
	}

	sData += full_data;
	sData += sMsg;

	// 데이터 전송
	CString tmp_key, tmp_val, tmp_data;
	POSITION	pos{};		//keyPos
	HWND m_sendhwnd=0;
	for (pos = m_LawKey.GetStartPosition(); pos != NULL;)
	{
		m_LawKey.GetNextAssoc(pos,tmp_key,tmp_val);
		m_sendhwnd = (HWND) atoi(tmp_key);
		::SendMessage(m_sendhwnd, WM_USER, MAKEWPARAM(MAKEWORD(DLL_TRIGGER, -1), false), (LPARAM)(LPCTSTR)sData);
	}
}

void CMapWnd::RemoveCB(CString accn)
{
	CString	text;

	m_sync.Lock();

	if (m_multicb.Lookup(accn, text))
	{
		m_multicb.RemoveKey(accn);//buffet block
	}

	m_sync.Unlock();
}

BOOL CMapWnd::LoadCode()
{
	if (m_hcode.GetSize() > 0) 
		return TRUE;
	CString root = variant(homeCC, "");

	CString	string; 
	CFile	file;
	int	pos{}, size{};

	string.Format("%s/%s/%s", root, TABDIR, HJCODEx);
	if (!file.Open(string, CFile::modeRead|CFile::typeBinary))
		return FALSE;

	size = (int)file.GetLength();

	if (m_hcB)
		m_hcB = nullptr;
		//delete[] m_hcB;
	 
	m_hcB = std::make_unique<char[]>(size);
//	m_hcB  = new char[size];
	file.Read(m_hcB.get(), size);
	file.Close();

	size /= sizeof(struct hjcodex);
	if (size <= 0)
	{
//		delete[] m_hcB;
		m_hcB = nullptr;
		return FALSE;
	}

	struct	hjcodex*	hjc = (struct hjcodex *)m_hcB.get();
	for (int ii = 0; ii < size; ii++, hjc++)
	{
		m_hcode.Add(hjc);
		string = CString(hjc->code, HCodeLen);
		if (string.GetAt(0) == 'A')
			string = string.Mid(1);
		pos = string.Find('\0');
		if (pos != -1)
			string = string.Left(pos);
		string.TrimRight();
		m_hname.SetAt(string, hjc);
	}

	return TRUE;
}

bool CMapWnd::Set3RDCode()
{
	if (m_hcode.GetSize() == 0) 
		return false;

	m_3RDcode.RemoveAll();
	m_KONEXcode.RemoveAll();
	struct	hjcodex* hjc=nullptr;

	for (int ii = 0; ii < m_hcode.GetSize(); ii++)
	{
		hjc = (struct hjcodex *)m_hcode.GetAt(ii);
		if (hjc->kosd == jm3RD)
			m_3RDcode.Add(hjc);
		else if(hjc->ssgb == jmKONEX)
			m_KONEXcode.Add(hjc);
	
	}

	return true;
}

bool CMapWnd::IsKONEXCode(CString sCode)
{
	if (m_KONEXcode.GetSize() == 0) 
		return false;
	
	struct	hjcodex* hjc{};
	CString strCode;
	
	for (int ii = 0; ii < m_KONEXcode.GetSize(); ii++)
	{
		hjc = (struct hjcodex *)m_KONEXcode.GetAt(ii);
		strCode.Format("%s", hjc->code); strCode.TrimLeft(); strCode.TrimRight();
		if (!strCode.Compare(sCode))
			return true;
	}
	
	return false;
}

bool CMapWnd::Is3RDCode(CString sCode)
{
	if (m_3RDcode.GetSize() == 0) 
		return false;
	
	struct	hjcodex* hjc{};
	CString strCode;
	
	for (int ii = 0; ii < m_3RDcode.GetSize(); ii++)
	{
		hjc = (struct hjcodex *)m_3RDcode.GetAt(ii);
		strCode.Format("%s", hjc->code); strCode.TrimLeft(); strCode.TrimRight();
		if (!strCode.Compare(sCode))
			return true;
	}

	return false;
}

CString CMapWnd::GetRoot() 
{ 
	return variant(homeCC, ""); 
}

CString CMapWnd::GetUser() 
{ 
	return variant(userCC, "");
}

CString CMapWnd::GetName() 
{ 
	return variant(nameCC, ""); 
}

//2014.09.18 KSJ 일방향 암호화
CString CMapWnd::GetEncPassword(CString sPswd)
{
	CString dllPath;
	dllPath.Format(_T("%s%s"), GetRoot(), _T("\\dev\\CX_SHA256.DLL"));
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
