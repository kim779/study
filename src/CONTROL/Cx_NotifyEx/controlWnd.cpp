#include "stdafx.h"
#include "Cx_NotifyEx.h"
#include "controlWnd.h"
#include "../../h/fxCommonx.h"
#include "../../h/ledger.h"
#include "libDefine.h"
#include "OpSensCal.h"

#define	INSERT_TEST		// 신규종목 체결시 insert TR수정

//#define	_FILE_DEBUG		// time check 하기 위한 debug
#ifdef	_FILE_DEBUG
#include <fstream.h>
#include "Windows.h"
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef	_FILE_DEBUG
	ofstream dFile("C:\\NotifyEx.trc",ios::app);
#endif

void CMchg::SetMchg(CString gubnS, int amt)
{
	if (gubnS.Find("매도") != -1)
		m_md += amt;
	else if (gubnS.Find("매수") != -1)
		m_ms += amt;
}

void CMchg::GetMchg(int& md, int& ms)
{
	md = m_md;	ms = m_ms;
}

void CMchg::SetMchg(CString gubnS, CString stateS, int amt)
{
	bool bMd = false;
	int saveAmt = 0;
	if (gubnS.Find("매도") != -1)
	{
		bMd = true;
		saveAmt = m_md;
	}
	else if (gubnS.Find("매수") != -1)
	{
		bMd = false;
		saveAmt = m_ms;
	}
	
	if (!stateS.Compare("체결"))
		saveAmt -= amt;
	else if (!stateS.Compare("정정확인"))
		saveAmt -= amt;
	else if (!stateS.Compare("취소확인"))
		saveAmt -= amt * 2;
	else if (!stateS.Compare("거부"))
		saveAmt -= amt;
	if (stateS.Find("접수", 0) != -1)
		saveAmt += amt;

	if (bMd)
		m_md = saveAmt;
	else 
		m_ms = saveAmt;
}

//================================================================================
CControlWnd::CControlWnd()
{
	EnableAutomation();
	deleteCodeMap();
	deleteMchgMap();
	m_arField.RemoveAll();
	m_nPosition = FPOS_NOT;
	m_pOpSensCal = NULL;
	m_bAlert = false;
	m_bEnableSend = true;
	m_oopCode = "";

#ifdef	_FILE_DEBUG
	dFile	<< getTime(false) << "======> NOTIFYEX Start  <<======" << endl;
#endif
}

CControlWnd::~CControlWnd()
{
#ifdef	_FILE_DEBUG
	dFile	<< getTime(false) << "======> NOTIFYEX end  <<======" << endl;
#endif
	m_bAlert = false;
	deleteCodeMap();
	deleteMchgMap();
	m_arField.RemoveAll();
	if (m_pOpSensCal)
	{
		delete m_pOpSensCal;	m_pOpSensCal = NULL;
	}
}

void CControlWnd::OnFinalRelease()
{
	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "Flag", m_flag, OnFlagChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "DataList", m_dataList, OnDataListChanged, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "SendX", SendX, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "Clear", Clear, VT_EMPTY, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// {820F1EA2-4F03-4B49-983F-32EA870CEAE6}
static const IID IID_IControlWnd =
{ 0x820f1ea2, 0x4f03, 0x4b49, { 0x98, 0x3f, 0x32, 0xea, 0x87, 0xc, 0xea, 0xe6 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUB:
		{
			// 잔고조회 ->  미체결내역 -> 민감도 
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "OnMessage Oub key => [" << m_sendKey << "] len[" << HIWORD(wParam) << "]" << endl;
#endif
			if (!HIWORD(wParam))	break;

			switch (m_sendKey)
			{
			case KEY_FREMAIN:
			case KEY_CONT:
				break;
			case KEY_MINGAM:
				m_Section.Lock();
				oubMingam((char*)lParam, HIWORD(wParam));
				sendToMap(SM_ALL);
				m_bAlert = true;
				m_bEnableSend = true;
				m_Section.Unlock();
				return 0;
			case KEY_FCURR:
				m_Section.Lock();
				oubFCurr((char*)lParam, HIWORD(wParam));
				m_Section.Unlock();
				return 0;
			default:
				return 0;
			}

			CString sErrCode = "", sErrText = "", sLedger, sData;
			long	ledgerLength = 0;
			char	nextKey;

			if (variant(systemCC, "") == "D")
			{
		/*		struct _ledgerDaetooH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerDaetooH);
				sLedger = CString((char*)&ledger, L_ledgerDaetooH);
				sErrText = CString((char*)&ledger.msg, 130);
				sErrCode = CString((char*)&ledger.ecode, 6);
				nextKey = ledger.contf[0];
				ledgerLength = L_ledgerDaetooH;*/
			}
			else
			{
			/*	struct _ledgerHanaH ledger;
				CopyMemory(&ledger, (void*)lParam, L_ledgerHanaH);
				sLedger = CString((char*)&ledger, L_ledgerHanaH);
				sErrText = CString((char*)&ledger.msg, 130);
				sErrCode = CString((char*)&ledger.ecode, 6);
				nextKey = ledger.contf[0]; 
				ledgerLength = L_ledgerHanaH;*/
			}
			lParam += ledgerLength;

			sData = "guide\t" + sErrText;
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)(LPCTSTR)sData);
			if (sErrCode.Compare("000000"))		// 오류가 있는 경우
			{
				sendToMap(SM_ERROR);
				m_bAlert = true;
				m_bEnableSend = true;
				return 0;
			}

			switch (m_sendKey)
			{
			case KEY_FREMAIN:
				m_Section.Lock();
				if (oubRemain((char*)lParam, sErrText))
				{
					m_Section.Unlock();
					queryCont();	//queryMingam();
				}
				else
				{
					sendToMap(SM_ALL);
					m_bAlert = true;
					m_bEnableSend = true;
					m_Section.Unlock();
				}
				break;
			case KEY_CONT:
				m_Section.Lock();
				oubCont((char*)lParam, sErrText);
				m_Section.Unlock();
				if (nextKey == '1')
				{
					queryCont(sLedger);
				}
				else
				{
					queryMingam();

					//sendToMap(SM_ALL);
					//m_bAlert = true;
					//m_bEnableSend = true;
				}
				break;
			}
		}
		break;
	case DLL_ALERT:		// if selfflash, code + data else only data.
		m_Section.Lock();
		alert((char *)lParam);
		m_Section.Unlock();
		break;
	case DLL_NOTICE:
		m_Section.Lock();
		notice((char *)lParam);
		m_Section.Unlock();
		break;
	case DLL_INB:		// if control with input attribute, Write value in m_sCtrlData.
		break;
	case DLL_TRIGGER:	// Remain Data Send to Map!
		break;
	case DLL_DOMINO:
		break;
	case DLL_SETFONT:
		break;
	}
	return 0;
}


void CControlWnd::SetParam(_param *pParam)
{
	m_Param.key     = pParam->key;
	m_Param.name    = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect    = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts   = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point   = pParam->point;
	m_Param.style   = pParam->style;
	m_Param.tRGB    = pParam->tRGB;
	m_Param.pRGB    = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());

	CString datS;
	CString tmpS = m_Param.options;
	tmpS = optionParser(tmpS, "/edit");
	m_arField.RemoveAll();
	while (!tmpS.IsEmpty())
	{
		datS = _Parser(tmpS, "|");
		m_arField.Add(atoi(datS));
	}
}

void CControlWnd::SendX(LPCTSTR accnS, LPCTSTR passS, LPCTSTR gubnS) 
{
	if (!m_bEnableSend)
	{
		AfxMessageBox("잔고조회중입니다. 잔고조회 버튼을 다시 선택하세요");
		return;
	}
	m_bEnableSend = false;
	m_bAlert = false;
	//deleteCodeMap();
	//deleteMchgMap();
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "잔고조회 =>" << accnS << endl;
#endif
	m_nPosition = atoi(gubnS);
	queryJango(accnS, passS);
}


int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CString path = variant(homeCC, "");
	CString name = variant(nameCC, "");

	m_pOpSensCal = new COpSensCal();
	m_pOpSensCal->CreateOpSensCal(this, path);
	m_pOpSensCal->SetUserName(name);
	m_pOpSensCal->ReadCfg();

	return 0;
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

CString CControlWnd::variant(int comm, CString data)
{
	return VariantA(m_pParent, comm, data);
}

CString CControlWnd::optionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	_Parser(tmp, sKey);
	tmp = _Parser(tmp, "/");
	return tmp;
}

void CControlWnd::queryJango(CString accnS /*=""*/, CString passS /*=""*/)
{
	m_bAlert = false;
	deleteCodeMap();
	deleteMchgMap();

	CString ledgerS = getLedger("8949", "SONBQ021", "2");

	if (!accnS.IsEmpty() && !passS.IsEmpty())
	{
		m_accnS = accnS;
		m_passS = passS;
	}
	else
	{
		accnS = m_accnS;
		passS = m_passS;
	}

	CString passX;
	passX.Format("%s\t%s", passS, accnS.Left(8));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	passS = passX;

	CString sData = ledgerS;
	struct f_mid mid;
	
	FillMemory(&mid, L_fmid, ' ');
	CopyMemory(&mid.accn, accnS, sizeof(mid.accn));
	CopyMemory(&mid.cogb, accnS.Mid(8, 2), sizeof(mid.cogb));
	CopyMemory(&mid.pswd, passS, passS.GetLength());
	//CopyMemory(&mid.date, "", sizeof(mid.date));
	sData += CString((char*)&mid, L_fmid);	
	sendTR("PIBOFJGO", sData, KEY_FREMAIN, 0);
}

void CControlWnd::queryCont(CString ledgerS /*=""*/)
{
	CString sData = "";
	if (!ledgerS.IsEmpty())
	{
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "미체결내역Cont send =>" << endl;
#endif
		sData = ledgerS;
	}
	else 
	{
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "미체결내역First send =>" << endl;
#endif
		sData = getLedger("8949", "", "1");
	}

	struct f_cmid cmid;
	
	FillMemory(&cmid, L_fcmid, ' ');
	CopyMemory(&cmid.accn, m_accnS, sizeof(cmid.accn));
	CopyMemory(&cmid.cogb, m_accnS.Mid(8, 2), sizeof(cmid.cogb));

	CString passX;
	passX.Format("%s\t%s", m_passS, m_accnS.Left(8));
	passX = (char *)m_pParent->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	CopyMemory(&cmid.pswd, passX, passX.GetLength());

	CopyMemory(&cmid.gubn, "2", sizeof(cmid.gubn));
	CopyMemory(&cmid.sort, "1", sizeof(cmid.sort));
	sData += CString((char*)&cmid, L_fcmid);
	sendTR("PIBOFCHG", sData, KEY_CONT, 0);
}

void CControlWnd::queryMingam()
{
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "민감data Send =>" << endl;
#endif
	struct _mmid mmid;
	CString tmpS;
	int ii = 0;
	CInfo *info = NULL;
	CString codeS;

	memset(&mmid, ' ' , L_MMID);
	for (POSITION pos = m_mapCode.GetStartPosition(); pos != NULL; )
	{
		m_mapCode.GetNextAssoc(pos, codeS, info);
		memcpy(&mmid.codes[ii], codeS, codeS.GetLength());
		ii++;
	}

	tmpS.Format("%3d", ii);
	memcpy(&mmid.crec, tmpS, tmpS.GetLength());
	sendTR("PIBO3101", CString((char *)&mmid, L_MMID), KEY_MINGAM, 0);
}

CString CControlWnd::getLedger(CString sTran, CString sSvcn, CString sGubn)
{
	CString strUser;
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	strUser = variant(userCC, "");

	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CopyMemory(&ledger.svcd, sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = sGubn.GetAt(0);
	ledger.odrf[0] = '1';

	return CString((char*)&ledger, L_ledgerH);
}

BOOL CControlWnd::sendTR(CString sTR, CString sData, int nKey, int nStat)
{
	CString trData = "";
	
	struct _userTH user;
	strncpy(&user.trc[0], sTR, 8);
	user.key  = m_Param.key;
	user.stat = nStat | US_KEY;
	
	trData.Empty();
	trData += (BYTE)nKey;
	trData += m_Param.name;
	trData += "\t";
	trData += CString((char *)&user, L_userTH);
	trData += sData;

	m_sendKey = nKey;
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "sendTR key => [" << m_Param.key << "] m_sendKey[" << m_sendKey << "]" << endl;
#endif

TRACE("send key[%d]\n", m_sendKey);
	BOOL res = m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.GetLength() - L_userTH - m_Param.name.GetLength() -1), 
		(LPARAM)(const char*)trData); 
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "sendTR res[" << res << "] " << endl;
#endif

	return res;
}


void CControlWnd::alert(CString rtsS)
{

//TRACE("alert[%.200s]\n", rtsS);
	CString codeS = _Parser(rtsS, "\t");

	// 20080111 RTS 수정
	CInfo *info = NULL;
	if (!m_mapCode.Lookup(codeS, info))
	{
		if (m_mgBase.krts != codeS && m_mgBase.k200rts != codeS)
			return;
	}

	CMapStringToString	mapRts;
	CString symS, datS;
	CString currS, diffS;
	while (!rtsS.IsEmpty())
	{
		symS = _Parser(rtsS, "\t");
		datS = _Parser(rtsS, "\t");
		if (datS.IsEmpty())
			continue;
		mapRts.SetAt(symS, datS);
	}
	if (!mapRts.Lookup("023", currS))
		return;
	mapRts.Lookup("024", diffS);



	mapRts.RemoveAll();
	
	
	if (m_mapCode.Lookup(codeS, info))
	{
		if (info->CalPgsonik(currS, diffS))
		{
			info->CalMingam(&m_mgBase, false);
			sendToMap(SM_UPDATE, codeS);
			return;
		}
	}

	bool bCalMin = false;
	if (m_mgBase.krts == codeS && m_mgBase.kcur != currS)		// 선물 최근월물
	{
		m_mgBase.kcur = currS;
		bCalMin = true;
	}
	else if (m_mgBase.k200rts == codeS && (m_mgBase.k200cur != currS))// K200
	{
		m_mgBase.k200cur = currS;
		bCalMin = true;
	}
	if (!bCalMin)
		return;
	

	for (POSITION pos = m_mapCode.GetStartPosition(); pos != NULL; )
	{
		m_mapCode.GetNextAssoc(pos, codeS, info);
		info->CalMingam(&m_mgBase, false);
		sendToMap(SM_UPDATE, codeS);
	}

}

bool CControlWnd::notice(CString noticeS)
{
TRACE("notice notice[%s]\n", noticeS);

	CString sKeys = _Parser(noticeS, "\n"), sAcc = "", sTemp = "", sOrder = "", sJCode = "",
		sSendData = "";
	bool	bNewCode = false;

	while (!sKeys.IsEmpty())
	{
		CString sKey = _Parser(sKeys, "\t");
		if (atoi(sKey) == 901)
		{				//	계좌번호
			sAcc = _Parser(sKeys, "\r");
			if (sAcc.GetLength() > 10)
			{
				sTemp = sAcc;
				sAcc = _Parser(sTemp, "\t");
				_Parser(sTemp, "\t");	// 902를 제거
			}
			break;
		}
	}
	
	sKeys = noticeS;
	while (!sKeys.IsEmpty())
	{
		if (atoi(_Parser(sKeys, "\t")) == 907)
		{
			sJCode = _Parser(sKeys, "\t");
			break;
		}
		_Parser(sKeys, "\t");
	}

	if (sAcc.IsEmpty() || sJCode.IsEmpty() || sAcc != m_accnS)
		return false;

	if (!m_bAlert)
	{
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "조회중 실시간 체결[" << noticeS << "]" << endl;
#endif
		return false;
	}

#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "실시간 체결[" << sJCode << "]" << noticeS << endl;
#endif
	CString codeS = sJCode;
	bool bNew = false;
	CInfo *info = NULL;

	CMapStringToString	mapNotice;
	mapNotice.RemoveAll();
	while(!noticeS.IsEmpty())
	{
		CString sSym = _Parser(noticeS, "\t"), 
			sDat = _Parser(noticeS, "\t");
		sDat.TrimLeft();	
		sDat.TrimRight();
		mapNotice.SetAt(sSym, sDat);
	}
	
	CString stateS, gubnS, amtS;
	mapNotice.Lookup(RT_STATE, stateS);					// 주문상태
	mapNotice.Lookup(RT_GUBN, gubnS);					// 주문구분
	if (gubnS.Find("매도") != -1)	gubnS = "-매도";
	else				gubnS = "+매수";
	mapNotice.Lookup(RT_CAMT, amtS);					// 주문수량

	CMchg *mchg = NULL;
	if (m_mapMchg.Lookup(codeS, mchg))
	{
		mchg->SetMchg(gubnS, stateS, atoi(amtS));
	}
	else
	{
		mchg = new CMchg();
		mchg->SetMchg(gubnS, stateS, atoi(amtS));
		m_mapMchg.SetAt(codeS, mchg);
	}

	if (!m_mapCode.Lookup(codeS, info))
	{
		bNew = true;
		info = new CInfo(this);

		int md = 0, ms = 0;
		if (m_mapMchg.Lookup(codeS, mchg))
		{
			mchg->GetMchg(md, ms);
			info->SetNoContNew(md, ms);
		}
	}


	int type = info->NoticeData(mapNotice, sJCode, bNew);

	
	switch (type)
	{
	case SM_UPDATE:	// update
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "update  : " << sJCode << endl;
#endif
		sendToMap(type, sJCode);
		break;
	case SM_DELETE:	// delete
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "delete : " << sJCode << endl;
#endif
		if (info)
		{
			delete info;	info = NULL;
			m_mapCode.RemoveKey(sJCode);
		}
		sendToMap(type, sJCode);
		break;
	case SM_INSERT:	// oop send(insert) : 새로운 종목에 대한 현재가 정보 등 필요
		{
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "insert 시세조회 :" << sJCode << endl;
#endif
			if (!checkFuPosition(codeS, info->sGubn))
			{
				delete info;	info = NULL;
				mapNotice.RemoveAll();
				return true;
			}
			
			
#ifdef	INSERT_TEST
			info->SetOpSensCal(m_pOpSensCal);
			m_mapCode.SetAt(codeS, info);

			CString currS = "", diffS = "", yydtS = "", dddtS = "";

			mapNotice.Lookup(RT_CCURR, currS);
			mapNotice.Lookup(RT_CDIFF, diffS);
			mapNotice.Lookup(RT_CYYDT, yydtS);
			mapNotice.Lookup(RT_CDDDT, dddtS);
//TRACE("notice code[%s] 현재가[%s] 대비[%s]\n", codeS, currS, diffS);
			if (atof(currS) == 0.0)	// 현재가 없으면 체결가로
			{
				mapNotice.Lookup(RT_CPRICE, currS);
				currS.Format("%.2f", atof(currS)/(float)100.0);
//TRACE("notice 체결가로 code[%s] 현재가[%s]\n", codeS, currS);
			}
			
			info->sCurr = currS;
			info->sDiff = diffS;
			info->sJjTrade = yydtS;
			info->sJjCalendar = dddtS;
			info->CalPgsonik();
			info->CalMingam(&m_mgBase);

			sendToMap(SM_INSERT, codeS);

#else
			m_bAlert = false;
			info->SetOpSensCal(m_pOpSensCal);
			m_mapCode.SetAt(codeS, info);
			m_oopCode = codeS;
			SetTimer(TM_EVENT, TM_GAP, NULL);
#endif			
			
			/*
			CString sData, sSym = "4";
			if (sJCode.GetAt(0) == '1' || sJCode.GetAt(0) == 'A')	//  20230125 파생상품 코드 개편	'1', 'A' : 선물
				sSym = "3";
			// 종목코드 : 현재가 + 대비 + 영업일 + 달력일
			sData.Format("%s0301%c%s\t%s1023\t%s1024\t%s0021\t%s0370\t%s0314\t", sSym, 0x7f, sJCode, sSym, sSym, sSym, sSym, sSym);
			sendTR("POOPPOOP", sData, KEY_FCURR, US_OOP|US_PASS);
			*/
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "end notice insert" << codeS << "]" << endl;	
#endif		
		}
		break;
	case SM_NOT:
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "not : " << sJCode << endl;
#endif
		if (bNew && info)
		{
			delete info;	info = NULL;
		}
		break;
	}
	mapNotice.RemoveAll();
	return true;
}

bool CControlWnd::oubRemain(char* pData, CString msgS)
{
TRACE("strlen[%d] L_fmod[%d] L_frec[%d]\n", strlen(pData), L_fmod, L_frec);

	struct	f_mod	*mod = (struct f_mod *)pData;
	CString accnS = _GetString(mod->accn, sizeof(mod->accn));
	if (accnS != m_accnS)	return false;
	int nrec = atoi(_GetString(mod->nrec, sizeof(mod->nrec)));

	int strL = strlen(pData);
	int chkL = L_fmod + L_frec * nrec;
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "잔고조회 receive => strlen[" << strL << "] chkL[" << chkL << "] nrec[" << nrec << "] fmod[" << L_fmod << "] L_frec[ " << L_frec << "]" << endl;
#endif
	if (strL != chkL)
		return false;

	if (nrec < 0)	return false;
	pData += L_fmod;

	CInfo *info = NULL;
	CString codeS, gubnS;
	for (int ii = 0; ii < nrec; ii++)
	{
		struct f_rec *rec = (struct f_rec *)pData;
		codeS = _GetString(rec->cod2, sizeof(rec->cod2));
		gubnS = _GetString(rec->dsgb, sizeof(rec->dsgb));

		if (!checkFuPosition(codeS, gubnS))
		{
			pData += L_frec;
			continue;
		}

		info = new CInfo(this);
		info->SetOpSensCal(m_pOpSensCal);
		info->RemainParsing(rec);

		m_mapCode.SetAt(codeS, info);
		m_arCode.Add(codeS);
TRACE("잔고 [%s]\n", codeS);
		pData += L_frec;
	}
	return true;
}

bool CControlWnd::oubCont(char* pData, CString msgS)
{
	struct f_cmod *fmod = (struct f_cmod *)pData;
	int nrec = atoi(_GetString(fmod->nrec, sizeof(fmod->nrec)));

	int strL = strlen(pData);
	int chkL = L_fcmod + L_fcrec * nrec; 

#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "미체결내역 receive => strlen[" << strL << "] chkL[" << chkL << "] nrec[" << nrec << "] L_fcmod[" << L_fcmod << "] L_fcrec[ " << L_fcrec << "]" << endl;
#endif

	if (strL != chkL)
		return false;

	pData += L_fcmod;	
	
	CMchg *mchg = NULL;
	CInfo *pInfo = NULL;
	CString codeS, gubnS, amtS;
	for (int ii = 0; ii < nrec; ii++)
	{
		struct f_crec *frec = (struct f_crec *)pData;

		codeS = _GetString(frec->cod2, sizeof(frec->cod2));
		gubnS = _GetString(frec->odgb, sizeof(frec->odgb));
		amtS =  _GetString(frec->wqty, sizeof(frec->wqty));
		if (m_mapCode.Lookup(codeS, pInfo))
			pInfo->SetNoCont(gubnS, amtS);

		if (m_mapMchg.Lookup(codeS, mchg))
		{
			mchg->SetMchg(gubnS, atoi(amtS));
		}
		else
		{
			mchg = new CMchg();
			mchg->SetMchg(gubnS, atoi(amtS));
			m_mapMchg.SetAt(codeS, mchg);
		}

		pData += L_fcrec;
	}
	return true;
}

void CControlWnd::oubFCurr(char *pData, int len)
{
	CString tmpS = CString(pData,len);
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "insert data receive =>" << tmpS << endl;
#endif
	CInfo *info = NULL;

	CString currS = _Parser(tmpS, "\t");
	CString diffS = _Parser(tmpS, "\t");
	CString codeS = _Parser(tmpS, "\t");
	if (!m_mapCode.Lookup(codeS, info))
	{
		m_bAlert = true;
		return;
	}

	info->sCurr = currS;
	info->sDiff = diffS;
	info->sJjTrade = _Parser(tmpS, "\t");
	info->sJjCalendar = _Parser(tmpS, "\t");
	info->CalPgsonik();
	info->CalMingam(&m_mgBase);
	
	m_bAlert = true;
	sendToMap(SM_INSERT, codeS);
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "insert data receive2 end" << endl;
#endif
	
}

void CControlWnd::oubMingam(char *pData, int len)
{
	CString tmpS;
	_mmod *mmod = (struct _mmod *)pData;
	int nrec = atoi(_GetString(mmod->grec, sizeof(mmod->grec)));
	
	int strL = strlen(pData);
	int chkL = L_MMOD + L_MGMOD * nrec;
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "민감data receive => strlen[" << strL << "] chkL[" << chkL << "] nrec[" << nrec << "] L_MMOD[" << L_MMOD << "] L_MGMOD[ " << L_MGMOD << "]" << endl;
#endif

	if (strL != chkL)
		return;

	m_mgBase.krts = _GetString(mmod->frts, sizeof(mmod->frts));		// 최근월물 선물 code
	m_mgBase.kcur = _GetString(mmod->fcur, sizeof(mmod->fcur));		// 최근월물 선물 현재가

	m_mgBase.k200rts = _GetString(mmod->k200rts, sizeof(mmod->k200rts));	// KOSPI 200 code
	m_mgBase.k200cur = _GetString(mmod->k200cur, sizeof(mmod->k200cur));	// KOSPI 200 현재가

	tmpS = _GetString(mmod->cdgl, sizeof(mmod->cdgl));			// CD 금리
	m_mgBase.cdgl.Format("%.2f", atof(tmpS)/1000);

	m_mgBase.time = _GetString(mmod->k200tim, sizeof(mmod->k200tim));	// KOSPI 200 Time

	m_mgBase.hv[0] = _GetString(mmod->hv[0], sizeof(mmod->hv[0]));		//역사적변동성 (90)
	m_mgBase.hv[1] = _GetString(mmod->hv[1], sizeof(mmod->hv[1]));		//역사적변동성 (60)
	m_mgBase.hv[2] = _GetString(mmod->hv[2], sizeof(mmod->hv[2]));		//역사적변동성 (30)
	m_mgBase.hv[3] = _GetString(mmod->hv[3], sizeof(mmod->hv[3]));		//역사적변동성 (21)

TRACE("k200[%s] 선물[%s] cd[%s] hv0[%s] hv1[%s] hv2[%s] hv3[%s]\n", m_mgBase.k200cur, m_mgBase.kcur, m_mgBase.cdgl, m_mgBase.hv[0], m_mgBase.hv[1], m_mgBase.hv[2], m_mgBase.hv[3]);
	if (m_pOpSensCal)
	{
		m_pOpSensCal->SetCd(m_mgBase.cdgl);				// CD금리
		m_pOpSensCal->SetHv(m_mgBase.hv[0], m_mgBase.hv[1], m_mgBase.hv[2], m_mgBase.hv[3]);// 역사적변동성
	}

	CInfo *pInfo = NULL;
	CString codeS;

	

	pData += L_MMOD;
	for (int ii = 0; ii < nrec; ii++)
	{
		_mgmod* mgmod = (struct _mgmod *)pData;
		codeS = _GetString(mgmod->ccod, sizeof(mgmod->ccod));		// 종목코드
		if (!m_mapCode.Lookup(codeS, pInfo))
		{
			pData += L_MGMOD;
			continue;
		}
		pInfo->sJjTrade = _GetString(mgmod->jjil[0], sizeof(mgmod->jjil[0]));	// 거래일수
		pInfo->sJjCalendar = _GetString(mgmod->jjil[1], sizeof(mgmod->jjil[1]));// 달력일수
		tmpS = _GetString(mgmod->bdjs, sizeof(mgmod->bdjs));			// 배당지수
		pInfo->sBdjs.Format("%.6f", atof(tmpS)/1000000);

		pInfo->CalMingam(&m_mgBase);
		pData += L_MGMOD;
	}
}


void CControlWnd::Clear() 
{
	deleteCodeMap();
	deleteMchgMap();
}

void CControlWnd::OnFlagChanged() 
{

}

void CControlWnd::OnDataListChanged() 
{

}

void CControlWnd::deleteCodeMap()
{
	CInfo *info = NULL;
	CString codeS;
	for (POSITION pos = m_mapCode.GetStartPosition(); pos != NULL; )
	{
		m_mapCode.GetNextAssoc(pos, codeS, info);
		delete info;	info = NULL;
	}
	m_mapCode.RemoveAll();
	m_arCode.RemoveAll();

	m_mgBase.hv[0] = m_mgBase.hv[1] = m_mgBase.hv[2] = m_mgBase.hv[3] = "";
	m_mgBase.cdgl = m_mgBase.krts = m_mgBase.kcur = "";
	m_mgBase.time = m_mgBase.k200rts = m_mgBase.k200cur = "";
}

void CControlWnd::deleteMchgMap()
{
	CMchg *mchg = NULL;
	CString codeS;
	for (POSITION pos = m_mapMchg.GetStartPosition(); pos != NULL; )
	{
		m_mapMchg.GetNextAssoc(pos, codeS, mchg);
		delete mchg;	mchg = NULL;
	}
	m_mapMchg.RemoveAll();
}

bool CControlWnd::checkFuPosition(CString codeS, CString gubnS)
{
	if (m_nPosition == FPOS_NOT)
		return true;

	if (codeS.IsEmpty())	return false;
	char codeF = codeS.GetAt(0);
	bool bMD = false, bMS = false;
	if (gubnS.Find("매도") != -1)
		bMD = true;
	else if (gubnS.Find("매수") != -1)
		bMS = true;
	else
		return false;

	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
	//                              '2', 'B' : Call option
	//			        '3', 'C' : Put option
	//			        '4', 'D' : 스프레드
	switch (m_nPosition)
	{
	case FPOS_BULL:	// 강세불리:강세포지션(선물매수,콜매수, 풋매도)
		if ((codeF == '1' || codeF == 'A') && bMS)	// 선물매수
			return true;
		else if ((codeF == '2' || codeF == 'B') && bMS)	// 콜매수
			return true;
		else if ((codeF == '3' || codeF == 'C') && bMD)	// 풋매도
			return true;
		break;
	case FPOS_BEAR:	// 약세불리:약세포지션(선물매도, 콜매도, 풋매수)
		if ((codeF == '1' || codeF == 'A') && bMD)	// 선물매도
			return true;
		else if ((codeF == '2' || codeF == 'B') && bMD)	// 콜매도
			return true;
		else if ((codeF == '3' || codeF == 'C') && bMS)	// 풋매수
			return true;
		break;
	}
	return false;
}

void CControlWnd::sendToMap(int smType /*= SM_ALL*/, CString codeS /*= ""*/)
{
	CInfo *info = NULL;
	CString sendS = "", totalS = "", editS = "";
	
	int ii = 0, idx = 0;
	
	switch (smType)
	{
	case SM_ALL:
		{
			// 20080111 sort안되도록 수정
			CString codeS;
			for (int ii = 0; ii < m_arCode.GetSize(); ii++)
			{
				codeS = m_arCode.GetAt(ii);
				if (m_mapCode.Lookup(codeS, info))
				{
					editS = getEditData(info->GetTotalData());
					sendS += editS + "\n";
				}
			}
			/*
			for (POSITION pos = m_mapCode.GetStartPosition(); pos != NULL; )
			{
				m_mapCode.GetNextAssoc(pos, codeS, info);
				editS = getEditData(info->GetTotalData());
				sendS += editS + "\n";
			}
			*/
			m_flag = "A";
			m_dataList.Format("%d\t", m_mapCode.GetCount());
			m_dataList += sendS;
		}
		break;
	case SM_UPDATE:
		if (m_mapCode.Lookup(codeS, info))
		{
			m_flag = "U";
			editS = getEditData(info->GetTotalData());
			m_dataList = codeS + editS;
			if (!m_bAlert)	return;
		}
		break;
	case SM_INSERT:
		if (m_mapCode.Lookup(codeS, info))
		{
			m_flag = "I";
			editS = getEditData(info->GetTotalData());
			m_dataList = codeS + editS;
			if (!m_bAlert)	return;
#ifdef	_FILE_DEBUG
	dFile	<< getTime() << "SM_INSERT[" << codeS << "]" << endl;	
#endif	
		}
		break;
	case SM_DELETE:
		m_flag = "D";
		m_dataList = codeS;
		if (!m_bAlert)	return;
		break;
	case SM_ERROR:
		m_flag = "E";
		m_dataList = "";
		break;
	}
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk/*DblClick*/)),
		(long)m_Param.name.operator LPCTSTR());
}

CString CControlWnd::getEditData(CString totalS)
{
	int idx;
	CString	editS;
	CStringArray	arTotal;

	editS = "";
	arTotal.RemoveAll();
	while (!totalS.IsEmpty())
		arTotal.Add(_Parser(totalS, "\t"));
	
	for (int ii = 0; ii < m_arField.GetSize(); ii++)
	{
		idx = m_arField.GetAt(ii);
		if (idx > arTotal.GetUpperBound())	continue;
		editS += arTotal.GetAt(idx) + "\t";
	}
	arTotal.RemoveAll();
	return editS;
}


void CControlWnd::PostNcDestroy() 
{
	CWnd::PostNcDestroy();
}

CString CControlWnd::getTime(bool bPID /*= true*/)
{
	CTime curr = CTime::GetCurrentTime();
	CString timeS;
	if (bPID)
		timeS.Format("PID[%d][%02d/%02d %02d:%02d:%02d]", this->m_hWnd, curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());
	else
		timeS.Format("[%02d/%02d %02d:%02d:%02d]", curr.GetMonth(), curr.GetDay(), curr.GetHour(), curr.GetMinute(), curr.GetSecond());

	return timeS;
}

void CControlWnd::OnTimer(UINT nIDEvent) 
{
	switch (nIDEvent)
	{
	case TM_EVENT:
		{
			KillTimer(nIDEvent);

			/*
#ifdef	_FILE_DEBUG
			dFile	<< getTime() << "timer조회  => [" << m_oopCode << "]" << endl;	
#endif

			CString sJCode = m_oopCode;
			if (sJCode.IsEmpty())	break;

			m_oopCode = "";
			CString sData, sSym = "4";
			if (sJCode.GetAt(0) == '1' || sJCode.GetAt(0) == 'A')	//  20230125 파생상품 코드 개편	'1', 'A' : 선물
				sSym = "3";
			// 종목코드 : 현재가 + 대비 + 영업일 + 달력일
			sData.Format("%s0301%c%s\t%s1023\t%s1024\t%s0021\t%s0370\t%s0314\t", sSym, 0x7f, sJCode, sSym, sSym, sSym, sSym, sSym);
			sendTR("POOPPOOP", sData, KEY_FCURR, US_OOP|US_PASS);
			*/
		}
		break;
	}

	CWnd::OnTimer(nIDEvent);
}
