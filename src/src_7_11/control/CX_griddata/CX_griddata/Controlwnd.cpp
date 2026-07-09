// Controlwnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "CX_griddata.h"
#include "Controlwnd.h"
#include "../../../h/axisfire.h"

#define TM_ALERTFT 7777
#define TM_ALERTONE 7776
#define TM_ALERTALL 7775
#define TM_RESET 6666
#define TM_SEARCHLIST 5555
#define TM_ONETOONE 9999
// CControlwnd

//IMPLEMENT_DYNAMIC(CControlwnd, CWnd)

#pragma warning(disable:4996)
CControlwnd::CControlwnd()
{
	m_binter = false;
	m_bsuik = true;
	m_bmyhoga = false;
	m_bsync = true;
	m_bServerError = false;
	EnableAutomation();
}

CControlwnd::~CControlwnd()
{
}

void CControlwnd::OnFinalRelease()
{
	// 자동화 개체에 대한 마지막 참조가 해제되면
	// OnFinalRelease가 호출됩니다. 기본 클래스에서 자동으로 개체를 삭제합니다.
	// 기본 클래스를 호출하기 전에 개체에 필요한 추가 정리 작업을
	// 추가하십시오.

	CWnd::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CControlwnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlwnd, CWnd)
	DISP_FUNCTION_ID(CControlwnd, "SendData", dispidSendData, SendData, VT_EMPTY, VTS_NONE)
	DISP_PROPERTY_EX_ID(CControlwnd, "JCode", dispidJCode, GetJCode, SetJCode, VT_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetMapName", dispidSetMapName, SetMapName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "GetOubData", dispidGetOubData, GetOubData, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "GetAlertData", dispidGetAlertData, GetAlertData, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "SetSearchDate", dispidSetSearchDate, SetSearchDate, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetMandate", dispidSetMandate, SetMandate, VT_EMPTY, VTS_BSTR VTS_BSTR)
	//DISP_FUNCTION_ID(CControlwnd, "GetGroupCombo", dispidGetGroupCombo, GetGroupCombo, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "GetGroupCombo", dispidGetGroupCombo, GetGroupCombo, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetGroupByName", dispidSetGroupByName, SetGroupByName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetbInter", dispidSetbInter, SetbInter, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "Setsuik", dispidSetsuik, Setsuik, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "Setmyhoga", dispidSetmyhoga, Setmyhoga, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "Setcodetype", dispidSetcodetype, Setcodetype, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "Sethogatype", dispidSethogatype, Sethogatype, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "GetRealDate", dispidGetRealDate, GetRealDate, VT_BSTR, VTS_NONE)
	DISP_FUNCTION_ID(CControlwnd, "GetCodeInGroup", dispidGetCodeInGroup, GetCodeInGroup, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "MessangerMemoPop", dispidMessangerMemoPop, MessangerMemoPop, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "SetSync", dispidSetSync, SetSync, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION_ID(CControlwnd, "GetGrouptype", dispidGetGrouptype, GetGrouptype, VT_BSTR, VTS_BSTR)
END_DISPATCH_MAP()

// 참고: IID_IControlwnd에 대한 지원을 추가하여
//  VBA에서 형식 안전 바인딩을 지원합니다. 
//  이 IID는 .IDL 파일에 있는 dispinterface의 GUID와 일치해야 합니다.

// {ED3FB8E8-9CDF-4E46-8570-7E5E46469023}
static const IID IID_IControlwnd =
{ 0xED3FB8E8, 0x9CDF, 0x4E46, { 0x85, 0x70, 0x7E, 0x5E, 0x46, 0x46, 0x90, 0x23 } };

BEGIN_INTERFACE_MAP(CControlwnd, CWnd)
	INTERFACE_PART(CControlwnd, IID_IControlwnd, Dispatch)
END_INTERFACE_MAP()


// CControlwnd 메시지 처리기입니다.

CString CControlwnd::Variant(int nComm, CString strData /* = _T("") */)
{
	CString strValue;
	char* pValue = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nComm), (LPARAM)(LPCTSTR)strData);

	if ((long)pValue > 1)
		strValue = pValue;
	else
		return _T("");

	return strValue;
}

CString CControlwnd::GetSymbolFromFile()
{
	CString strResult;

	CString struser, strpath;
	CString strFilePath(_T(""));
	CString sSymData(_T(""));

	strpath = Variant(homeCC);
	strFilePath.Format(_T("%s\\tab\\axis.ini"), strpath);

	GetPrivateProfileString(_T("gridsym"), m_strmap, _T(""), sSymData.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);

	return sSymData;
}

bool CControlwnd::CheckMessangerLogin()
{
	CString	file, key, tmpS;
	char	buf[1024];
	file.Format("%s\\%s\\%s\\%s", m_sRoot, "user",  m_sUserName, "usersetup.ini");
	GetPrivateProfileString("messanger", "connect", "", buf, sizeof(buf), file);
	
	tmpS.Format(_T("%s"), buf);
	tmpS.TrimLeft();
	tmpS.TrimRight();

	if(tmpS.IsEmpty())
		return false;
	else
	{
		if(tmpS == "N" || tmpS == "1" || tmpS == "2")
			return false;
	}

	return true;
}

void CControlwnd::ShellexcuteLogin()
{
	AfxGetMainWnd()->SendMessage(WM_USER+500, 9894, (LPARAM)0);
}

void CControlwnd::JustLogin()
{
	AfxGetMainWnd()->SendMessage(WM_USER+500, 9893, (LPARAM)0);
}

long CControlwnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	CString tmpS;
	switch(LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
		{
			return 0;
		}
	case DLL_OUB:
		{
			char* pbuf= (char*)lParam;
			struct  _ledgerH* pledger = (struct  _ledgerH*)pbuf;
			CString strkey;
			strkey = CString(pledger->fnam, 30).TrimRight();

			if(strkey  == "selectAttGrpList")
				oubSearchList(pbuf);
			else if(strkey == "selectUsrNm")
			{
				char* pdata = new char[sizeof(struct mod)];
				memset(pdata, 0, sizeof(struct  mod));
				memcpy(pdata, (char*)&pbuf[sizeof(struct  _ledgerH)], sizeof(struct mod));
				struct  mod* pmod = (struct mod*)pdata;

				CString temps, senddata;
				temps =  CString(pmod->name, 30).TrimRight();
				temps.TrimLeft();
				m_strMsgName = temps;
				senddata.Format(_T("%s(%s)"), temps, m_strMsgUser);

				WCHAR* data = new WCHAR[senddata.GetLength() + 2];
				memset(data, 0, senddata.GetLength()*2 + 2);
				MultiByteToWideChar(CP_ACP, 0, (LPSTR)senddata.operator LPCSTR(), -1, data, senddata.GetLength()*2);
		
				COPYDATASTRUCT tip;
				tip.cbData = wcslen(data)*2 + 2;
				tip.dwData = 38495;
				tip.lpData = data;

				HWND hwn = ::FindWindow("TfrmMain", NULL);
				if(hwn == NULL)
				{
					ShellexcuteLogin();
					m_imsgcnt = 0;
					SetTimer(TM_ONETOONE, 2000, NULL);
				}
				else
				{
					if(!CheckMessangerLogin())
						JustLogin();
					Sleep(200);
					::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
					::PostMessage(hwn, WM_USER + 1003, 0 , 0);	
				}
			}
		}
		break;
	case DLL_ALERTx:
		{	
			m_sync.Lock();
			struct _alertR* alertR;			
			alertR = (struct _alertR *)lParam;
		
			DWORD* data;
			CString stmp;
			int type = 0;

			for (int ii = 0; ii < alertR->size; ii++)	// 여러 건에 대한 처리
			{
				data = (DWORD *)alertR->ptr[ii];
				if(data[101] != NULL)
				{
					stmp.Format(_T("%s"), (char*)data[101]);
					type = atoi(stmp);

					struct _hogast* phogast;
					phogast = new struct _hogast;
					memset(phogast, 0 , sizeof(struct _hogast));
					char *pdata = (char*)lParam;

					memcpy(phogast->gubn, (char*)data[101], 2);
					memcpy(phogast->code, (char*)data[102], 12);
					memcpy(phogast->name, (char*)data[103], 40);
					memcpy(phogast->ckind, (char*)data[104], 4);
					memcpy(phogast->mandate, (char*)data[105], 10);
					memcpy(phogast->leftdate, (char*)data[106], 6);
					memcpy(phogast->min, (char*)data[107], 10);
					memcpy(phogast->suiktype, (char*)data[108], 2);

					memcpy(phogast->mdcur, (char*)data[109], 10);
					memcpy(phogast->mdcurtime, (char*)data[110], 5);
					memcpy(phogast->mdcurmcont, (char*)data[111], 10);
					memcpy(phogast->mdjprc, (char*)data[112], 10);
					memcpy(phogast->mdjcont, (char*)data[113], 10);
					memcpy(phogast->mdhggoga, (char*)data[114], 10);
					memcpy(phogast->mdhggogatime, (char*)data[115], 5);
					memcpy(phogast->mdhgjuga, (char*)data[116], 10);
					memcpy(phogast->mdhgjugatime, (char*)data[117], 5);

					memcpy(phogast->mscur, (char*)data[118], 10);
					memcpy(phogast->mscurtime, (char*)data[119], 5);
					memcpy(phogast->mscurmcont, (char*)data[120], 10);
					memcpy(phogast->msjprc, (char*)data[121], 10);
					memcpy(phogast->msjcont, (char*)data[122], 10);
					memcpy(phogast->mshggoga, (char*)data[123], 10);
					memcpy(phogast->mshggogatime, (char*)data[124], 5);
					memcpy(phogast->mshgjuga, (char*)data[125], 10);
					memcpy(phogast->mshgjugatime, (char*)data[126], 5);

					memcpy(phogast->fncur, (char*)data[127], 10);
					memcpy(phogast->fncurtime, (char*)data[128], 5);
					memcpy(phogast->fncurmcont, (char*)data[129], 10);
					memcpy(phogast->fnjprc, (char*)data[130], 10);
					memcpy(phogast->fnjcont, (char*)data[131], 10);
					memcpy(phogast->fnhggoga, (char*)data[132], 10);
					memcpy(phogast->fnhggogatime, (char*)data[133], 5);
					memcpy(phogast->fnhgjuga, (char*)data[134], 10);
					memcpy(phogast->fnhgjugatime, (char*)data[135], 5);

					SendjongcodeRTS(phogast);
				}
				else if(data[201] != NULL)
				{
					stmp.Format(_T("%s"), (char*)data[201]);
					type = atoi(stmp);

					struct _balst* pbalst;
					pbalst = new struct _balst;
					memset(pbalst, 0 , sizeof(struct _balst));
					char *pdata = (char*)lParam;

					memcpy(pbalst->gubn, (char*)data[201], 2);
					memcpy(pbalst->junum, (char*)data[202], 7);
					memcpy(pbalst->date, (char*)data[203], 10);
					memcpy(pbalst->time, (char*)data[204], 8);
					memcpy(pbalst->code, (char*)data[205], 12);
					memcpy(pbalst->name, (char*)data[206], 40);
					memcpy(pbalst->ckind, (char*)data[207], 4);
					memcpy(pbalst->mandate, (char*)data[208], 10);
					memcpy(pbalst->leftdate, (char*)data[209], 6);
					memcpy(pbalst->min, (char*)data[210], 7);
					memcpy(pbalst->suiktype, (char*)data[211], 2);
					memcpy(pbalst->hogatypecode, (char*)data[212], 2);
					memcpy(pbalst->hogatype, (char*)data[213], 6);
					memcpy(pbalst->hoga, (char*)data[214], 7);
					memcpy(pbalst->contradict, (char*)data[215], 7);
					memcpy(pbalst->volume, (char*)data[216], 6);
					memcpy(pbalst->payday, (char*)data[217], 8);
					memcpy(pbalst->linegubn, (char*)data[218], 12);
					memcpy(pbalst->hightradegubn, (char*)data[219], 1);
					memcpy(pbalst->sendid, (char*)data[220], 30);
					memcpy(pbalst->comcode, (char*)data[221], 6);
						
					SendallhogaRTS(pbalst);
				}
				else if(data[301] != NULL)
				{
					stmp.Format(_T("%s"), (char*)data[301]);
					type = atoi(stmp);

					struct _ftchest* pftst;
					pftst = new struct _ftchest;
					memset(pftst, 0 , sizeof(struct _ftchest));
					char *pdata = (char*)lParam;

					memcpy(pftst->date, (char*)data[301], 10);
					memcpy(pftst->time, (char*)data[302], 8);
					memcpy(pftst->comcode, (char*)data[303], 6);
					memcpy(pftst->code, (char*)data[304], 12);
					memcpy(pftst->name, (char*)data[305], 40);
					memcpy(pftst->ckind, (char*)data[306], 4);
					memcpy(pftst->mandate, (char*)data[307], 10);
					memcpy(pftst->suikrate, (char*)data[308], 7);
					memcpy(pftst->contrasuik, (char*)data[309], 7);
					memcpy(pftst->prc, (char*)data[310], 7);
					memcpy(pftst->contraprc, (char*)data[311], 7);
					memcpy(pftst->volume, (char*)data[312], 6);
					memcpy(pftst->payday, (char*)data[313], 10);
					memcpy(pftst->min, (char*)data[314], 7);
					memcpy(pftst->minprc, (char*)data[315], 7);

					Send15CheRTS(pftst);
				}
			}

			m_sync.Unlock();
			
			return 0;
		}
		break;
	case DLL_ALERT:
		return 0;
		{
		
		}
		break;
	case DLL_SETFONT:
	case DLL_SETFONTx:
	//	m_Param.point = HIWORD(wParam);
	//	Invalidate(FALSE);
		break;
	}
	return 0;	
}

//종목호가
void CControlwnd::SendjongcodeRTS(struct _hogast* pdata)
{
	CString strresult;
	if(m_kind == 0)  //종목별 최근호가
	{
		strresult.Format(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),				 
																			   "102",CString(pdata->code, 12).TrimRight(),  //종목코드																		   
																			   "103",CString(pdata->name, 40).TrimRight(),  //종목명	
																			   "104",CString(pdata->ckind, 4).TrimRight(),  //채권종류
																			   "105",CString(pdata->mandate, 10).TrimRight(), //만기일
																			   "106",CString(pdata->leftdate, 6).TrimRight(),  //잔존일																			   
																			   "107",CString(pdata->min, 10).TrimRight(),  //민평 
																			   "108",CString(pdata->suiktype, 2).TrimRight(),  //수익율유형

																			   "109",CString(pdata->mdcur, 10).TrimRight(),  //매도호가현재가 
																			   "110",CString(pdata->mdcurtime, 5).TrimRight(),  //매도호가현재가 업데이트시간
																			   "111",CString(pdata->mdcurmcont, 10).TrimRight(),  //매도호가현재가 민평대비 
																			   "112",CString(pdata->mdjprc, 10).TrimRight(),  //매도호가 직전가
																			   "113",CString(pdata->mdjcont, 10).TrimRight(),  //매도호가 직전가대비
																			   "114",CString(pdata->mdhggoga, 10).TrimRight(),  //매도호가 고가
																			   "115",CString(pdata->mdhggogatime, 5).TrimRight(),  //매도호가 고가 업데이트 시간
																			   "116",CString(pdata->mdhgjuga, 10).TrimRight(),  //매도호가 저가
																			   "117",CString(pdata->mdhgjugatime, 5).TrimRight(),  //매도호가 저가 업데이트 시간

																			   "118",CString(pdata->mscur, 10).TrimRight(),  //매수호가현재가 
																			   "119",CString(pdata->mscurtime, 5).TrimRight(),  //매수호가현재가 업데이트시간
																			   "120",CString(pdata->mscurmcont, 10).TrimRight(),  //매수호가현재가 민평대비 
																			   "121",CString(pdata->msjprc, 10).TrimRight(),  //매수호가 직전가
																			   "122",CString(pdata->msjcont, 10).TrimRight(),  //매수호가 직전가대비
																			   "123",CString(pdata->mshggoga, 10).TrimRight(),  //매수호가 고가
																			   "124",CString(pdata->mshggogatime, 5).TrimRight(),  //매수호가 고가 업데이트 시간
																			   "125",CString(pdata->mshgjuga, 10).TrimRight(),  //매수호가 저가
																			   "126",CString(pdata->mshgjugatime, 5).TrimRight(),  //매수호가 저가 업데이트 시간

																			   "127",CString(pdata->fncur, 10).TrimRight(),  //확정호가현재가 
																			   "128",CString(pdata->fncurtime, 5).TrimRight(),  //확정호가현재가 업데이트시간
																			   "129",CString(pdata->fncurmcont, 10).TrimRight(),  //확정호가현재가 민평대비 
																			   "130",CString(pdata->fnjprc, 10).TrimRight(),  //확정호가 직전가
																			   "131",CString(pdata->fnjcont, 10).TrimRight(),  //확정호가 직전가대비
																			   "132",CString(pdata->fnhggoga, 10).TrimRight(),  //확정호가 고가
																			   "133",CString(pdata->fnhggogatime, 5).TrimRight(),  //확정호가 고가 업데이트 시간
																			   "134",CString(pdata->fnhgjuga, 10).TrimRight(),  //확정호가 저가
																			   "135",CString(pdata->fnhgjugatime, 5).TrimRight()  //확정가 저가 업데이트 시간																		 
																			   ); 
		m_strRealData = strresult;	
		m_arr.Add(m_strRealData);

		SetTimer(TM_ALERTONE, 30,0);
	}
	else if(m_kind == 1)		//전체호가
	{
		
	}
	else if(m_kind == 3)  //종목별현재 
	{
		
	}
	else if(m_kind == 4)  //개별종목
	{
		
	}
}

CString CControlwnd::AddSpace(CString strdata, int ilen)
{
	CString strResult;
	strResult = strdata;

	for(int ii = 0 ; ii < ilen - strdata.GetLength() ; ii++)
		strResult += _T(" ");

	return strResult;
}

//15분체결 실시간
void CControlwnd::Send15CheRTS(struct _ftchest* pdata)
{
	CString strresult;
	CString tmpS, temps;

	if(m_kind == 0)  //종목별최근호가  
	{
	}
	else if(m_kind == 1) //전체호가
	{

	}
	else if(m_kind == 2)   //15분체결 
	{
		CString date, time, mandate, paydate, minprc, suik, wonsuik;
		date = CString(pdata->date, 10).TrimRight();
		date.Replace(_T("-"), _T(""));
		time = CString(pdata->time, 8).TrimRight();
		time.Replace(_T(":"), _T(""));
		mandate = CString(pdata->mandate, 10).TrimRight();
		mandate.Replace(_T("-"), _T(""));
		paydate = CString(pdata->payday, 10).TrimRight();
		paydate.Replace(_T("-"), _T(""));

		suik.Format(_T("%s"),pdata->contrasuik);
		suik.TrimRight();
		suik = AddSpace(suik.Left(10), 10);
		OutputDebugString(_T("######") + suik + _T("\n"));

		wonsuik.Format(_T("%s"),pdata->contraprc);
		wonsuik.TrimRight();
		wonsuik = AddSpace(wonsuik.Left(10), 10);
		
		strresult.Format(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s"),					  
																			  "301",date,  //날짜	
																			  "302",time,  //시각	
																			  "303",CString(pdata->comcode, 6).TrimRight(),  //회사코드	
																			  "304",CString(pdata->code, 12).TrimRight(),  //종목코드	
																			  "305",CString(pdata->name, 40).TrimRight(),  //종목명	
																			  "306",CString(pdata->ckind, 4).TrimRight(),  //채권종류																				 																			  																			   pdata->mdcurmcont, //매도직전가민평대비
																			  "307",mandate,  //만기일	
																			  "308",CString(pdata->suikrate, 7).TrimRight(),  //수익률	
																			  "309",suik,  //수익율대비	
																			  "310",CString(pdata->prc, 7).TrimRight(),  //가격	
																			  "311",wonsuik,  //가격대비	
																			  "312",CString(pdata->volume, 6).TrimRight(),  //수량	
																			  "313",paydate,  //결제일																				 																			  																			   pdata->mdcurmcont, //매도직전가민평대비
																			  "314",CString(pdata->min, 7).TrimRight(),  //민평	
																			  "315",CString(pdata->minprc, 7).TrimRight()  //민평가격																				  	
																			   );  
		
		m_strRealData = strresult;
		m_arr.Add(m_strRealData);
		SetTimer(TM_ALERTFT, 30,0);
	}
	else if(m_kind == 3)  //종목별현재
	{

	}
	else if(m_kind == 4)  //개별종목
	{

	}
}

//전체호가 실시간
void CControlwnd::SendallhogaRTS(struct _balst* pdata)
{
	CString strresult;
	CString tmpS, temps, strsun;

   
	if(m_kind == 0 ) //종목별최근호가  
	{

	}
	else if( m_kind == 1) //전체호가
	{
		tmpS.Format(_T("%.8s"), pdata->time);
		tmpS.TrimRight();
		tmpS.Replace(_T(":"), _T(""));

		temps.Format(_T("%.10s"), pdata->date);
		temps.TrimRight();
		temps.Replace(_T("-"), _T(""));

		strsun.Format(_T("%.12s"),pdata->linegubn);
		strsun.TrimLeft();
		strsun.TrimRight();

		if(strsun.Find("선") >= 0)
		{
			strsun = "01";
		}
		else if(strsun.Find("교") >= 0)
		{
			strsun = "02";
		}
		else if(strsun.Find("선/교") >= 0)
		{
			strsun = "03";
		}

		strresult.Format(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s"), 
																			   "202",CString(pdata->junum, 7).TrimRight(),  //주문번호
																			   "203",temps,  //날짜 
																			   "204",tmpS,		//시간	
																			   "205",CString(pdata->code, 12).TrimRight(),  //종목코드 
																			   "206",CString(pdata->name, 40).TrimRight(),  //종목명 
																			   "207",CString(pdata->ckind, 4).TrimRight(),  //채권종류  
																			   "208",CString(pdata->mandate, 10).TrimRight(),  //만기일 
																			   "209",CString(pdata->leftdate, 6).TrimRight(),  //잔존일 
																			   "210",CString(pdata->min, 7).TrimRight(),  //민평 
																			   "211",CString(pdata->suiktype, 2).TrimRight(), //수익율유형  
																			   "212",CString(pdata->hogatypecode, 2).TrimRight(), //호가유형코드 
																			   "213",CString(pdata->hogatype, 6).TrimRight(),  //호가유형
																			   "214",CString(pdata->hoga, 7).TrimRight(),  //수익율이나 가격
																			   "215",CString(pdata->contradict, 7).TrimRight(),  //대비  
																			   "216",CString(pdata->volume, 6).TrimRight(),  //수량 
																			   "217",CString(pdata->payday, 8).TrimRight(),  //결제일   
																			   "218",CString(pdata->linegubn, 12).TrimRight(),  //선/교?  
																			   //"218",strsun,  //선/교?  
																			   "219",CString(pdata->hightradegubn, 1).TrimRight(),  //집중거래호가
																			   "220",CString(pdata->sendid, 30).TrimRight(), //송신자 ID 14
																		       "221",CString(pdata->comcode, 6).TrimRight()	//회사코드				   																																										   
																			   );   
		m_strRealData = strresult;
		m_arr.Add(m_strRealData);

		SetTimer(TM_ALERTALL, 30,0);
	}
	else if(m_kind == 2)  //15분체결
	{
		
	}	
	else if(m_kind == 3)  //종목별현재
	{
	
	}
	else if(m_kind == 4)  //개별종목
	{
		
	}
}

bool CControlwnd::Checksearchdate(char* date)  //조회시간체크
{
	CString tmpS;
	tmpS.Format(_T("%s"), date);

	if(atoi(tmpS) >=  atoi(m_ssSearchdate) && atoi(tmpS) <=  atoi(m_seSearchdate))
		return true;

	return false;
}

bool CControlwnd::Checkmandate(char* mandate)   //만기일체크
{
	CString tmpS;
	tmpS.Format(_T("%s"), mandate);

	if(atoi(tmpS) >=  atoi(m_ssmandate) && atoi(tmpS) <=  atoi(m_semandate))
		return true;

	return false;
}

bool CControlwnd::Checkhogatype(char* hogatypecode)  //전체 매도 매수 확정 체크
{
	CString tmpS;
	tmpS.Format(_T("%s"), hogatypecode);

	if(m_shogatype == tmpS)
		return true;

	return false;
}

bool CControlwnd::Checkmyhoga(char* psendid)  //내 호가인지 판별
{
	CString tmpS;
	tmpS.Format(_T("%s"), psendid);
	if(m_sUserID.Find(tmpS) >= 0)
		return true;

	return false;
}

bool CControlwnd::CheckSuikprc(char* type)     //수익률 보여줄지 가격 보여줄지
{   //m_bsuik //true 수익률  false 가격
	//char suiktype[2];			//수익률유형    01수익률  02가격
	CString tmpS;
	tmpS.Format(_T("%s"), type);
	if(m_bsuik && atoi(tmpS) == 1)
		return true;

	if(!m_bsuik && atoi(tmpS) == 2)
		return true;
	 
	return false;
}

bool CControlwnd::Checkintercode(char* pcode)
{
	if(!m_binter)
		return true;

	CString strcode;
	CCodeData codedata;
	strcode.Format(_T("%s"), pcode);
	strcode.TrimRight();

	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(m_mapGroupCodemap.Lookup(m_sGroupName, pmap))
	{
		if(pmap->Lookup(strcode, codedata))
			return true;
	}
	else
		return false;

	return false;
}

bool CControlwnd::Checkcodetype(char* pkind)
{
	if(m_itype == 0)   //전체타입이면 통과
		return true;

	if(m_itype == atoi(pkind))
		return true;

	if(m_itype == 7000)  //국고나 통안이면 
	{
		if(atoi(pkind) == GUKCODE || atoi(pkind) == TONGCODE)
			return true;
	}

	if(m_itype == 8000)  //국고 통안 아니면
	{
		if(atoi(pkind) != GUKCODE && atoi(pkind) != TONGCODE)
			return true;
	}

	return false;
}

CString CControlwnd::Parser(CString &srcstr, CString substr)
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

void CControlwnd::SetParam(_param *pParam)
{
 	m_Param.key   = pParam->key;
	m_Param.name  = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect  = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB  = pParam->tRGB;
	m_Param.pRGB  = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
	CString tmp = m_Param.options;

	Parser(tmp, "/g");
	m_kind = ctype(atoi(tmp));
	m_sUserName = Variant(nameCC, "");
	m_sUserID = Variant(userCC, "");
	m_sRoot = Variant(homeCC, "");
}

void CControlwnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CControlwnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
		case TM_SEARCHLIST:
		{
			KillTimer(TM_SEARCHLIST);
			SearchInterList();
		}
		break;
		case TM_RESET:
		{
			KillTimer(TM_RESET);
			m_bsync = true;
		}
		break;
		case TM_ALERTALL:
			{
				if(m_bsync && m_arr.GetSize() > 0)
				{
					m_strRealData = m_arr.GetAt(0);
					m_bsync = false;
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)(LPCTSTR)m_Param.name);
					OutputDebugString("^^^^^" + m_strRealData);
					m_arr.RemoveAt(0);
					SetTimer(TM_RESET, 5000, NULL);


					if(m_arr.GetSize() == 0)
						KillTimer(TM_ALERTALL);
				}
			}
			break;
		case TM_ALERTONE:
			{
				if(m_bsync && m_arr.GetSize() > 0)
				{
					m_strRealData = m_arr.GetAt(0);
					CString strData;
					strData.Format("realdata\t%s", m_strRealData);
					m_bsync = false;
					m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnDblClk)), (LPARAM)(LPCTSTR)m_Param.name);

					m_arr.RemoveAt(0);
					SetTimer(TM_RESET, 5000, NULL);

				


					if(m_arr.GetSize() == 0)
						KillTimer(TM_ALERTONE);
				}
			}
			break;
		case TM_ALERTFT:
		{
			if(m_bsync && m_arr.GetSize() > 0)
			{
				m_strRealData = m_arr.GetAt(0);
				CString strData;
				strData.Format("realdata\t%s", m_strRealData);
				m_bsync = false;
				m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnChange)), (LPARAM)(LPCTSTR)m_Param.name);
				OutputDebugString("^^^^^" + m_strRealData);
				m_arr.RemoveAt(0);
				SetTimer(TM_RESET, 5000, NULL);


				if(m_arr.GetSize() == 0)
					KillTimer(TM_ALERTFT);
			}
		}
		break;
		case TM_ONETOONE:
		{
			CString senddata;
			senddata.Format(_T("%s(%s )"), m_strMsgUser,m_strMsgName);

			WCHAR* data = new WCHAR[senddata.GetLength()];
			memset(data, 0, senddata.GetLength()*2);
			MultiByteToWideChar(CP_ACP, 0, (LPSTR)senddata.operator LPCSTR(), -1, data, senddata.GetLength()*2);
		
			COPYDATASTRUCT tip;
			tip.cbData = wcslen(data)*2;
			tip.dwData = 38495;
			tip.lpData = data;
			
			HWND hwn = ::FindWindow("TfrmMain", NULL);

			if(CheckMessangerLogin())
			{
				KillTimer(TM_ONETOONE);
				::SendMessage(hwn, WM_COPYDATA, (WPARAM)this->m_hWnd , LPARAM(&tip));	
				::PostMessage(hwn, WM_USER + 1003, 0 , 0);
			}
			else
			{
				m_imsgcnt++;
				if(m_imsgcnt == 3)
					KillTimer(TM_ONETOONE);
			}
		}
		break;
	}
	
	CWnd::OnTimer(nIDEvent);
}


int CControlwnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	LoadGroupOfFile();
	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetTimer(TM_SEARCHLIST, 1000, NULL);
	return 0;
}

void CControlwnd::SendData(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());


}

void CControlwnd::SendTR(CString name, CString data, BYTE type, int key)
{//여기는 오면안되
	CString trData = ""; 
	CString tempdata = "";
	CString pass;
	
	struct _userTH udat;
	
	strcpy_s(udat.trc, name);
	udat.key = key;
	udat.stat = type;

	trData.Empty();
	trData = m_Param.name;
	trData += '\t';
	trData += CString((char *)&udat, L_userTH);
	trData += data;

	m_brts = false;
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, trData.GetLength() - L_userTH - m_Param.name.GetLength()), (LPARAM)trData.operator LPCTSTR());
}

BSTR CControlwnd::GetJCode(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.

	return strResult.AllocSysString();
}


void CControlwnd::SetJCode(LPCTSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	m_strcode.Format(_T("%s"), newVal);
}


void CControlwnd::SetMapName(LPCTSTR strmap)
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//m_strmap = strmap;
}


BSTR CControlwnd::GetOubData(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	
	return strResult.AllocSysString();
}


BSTR CControlwnd::GetAlertData(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;
	
	return strResult.AllocSysString();
}


void CControlwnd::SetSearchDate(LPCTSTR sdate, LPCTSTR edate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
}


void CControlwnd::SetMandate(LPCTSTR sdate, LPCTSTR edate)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_ssmandate.Format(_T("%s"), sdate);
	m_semandate.TrimRight();

	m_semandate.Format(_T("%s"), edate);
	m_semandate.TrimRight();
}

void CControlwnd::LoadGroupOfFile()
{
	CFile file;
	CFileException ex;
	CStringArray* pArr = NULL;
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));
	CString sMsg = _T("");

	strUserPath.Format(_T("%s\\user\\%s"), m_sRoot, m_sUserName);
	strFilePath.Format(_T("%s\\%s"), strUserPath, _T("userinterest.ini"));

	int nGrpCnt = GetPrivateProfileInt(_T("GROUPINFO"), _T("COUNT"), 0, (LPCTSTR)strFilePath);

	m_mapGroupCodemap.RemoveAll();
	CString sGroup(_T(""));
	CString sGrpData(_T(""));
	CString strGroup(_T(""));

	for ( int i = 0 ; i < nGrpCnt ; i ++ )
	{
		sGroup.Format(_T("%d"), i);
		GetPrivateProfileString(_T("GROUPINFO"), _T(sGroup), _T(""), sGrpData.GetBufferSetLength(1024), 1024, (LPCTSTR)strFilePath);
		sGrpData.ReleaseBuffer();

		sGrpData.TrimRight();
		sGrpData.TrimLeft();
		if(sGrpData.IsEmpty())
			continue;

		strGroup = Parser(sGrpData, _T(";"));
		strGroup = sGrpData;
		AddGroup(strGroup);
		LoadFile(strGroup);
	}
}

void CControlwnd::AddGroup(CString strGroup)
{
	CString strIndex(_T(""));
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(!m_mapGroupCodemap.Lookup(strGroup, pmap))
	{
		pmap =  new CMap<CString, LPCTSTR, CCodeData, CCodeData>;
		m_mapGroupCodemap.SetAt(strGroup, pmap);
	}
}

void CControlwnd::LoadFile(CString strgroup)
{
	CString strUserPath(_T(""));
	CString strFilePath(_T(""));

	
	strUserPath.Format("%s\\user\\%s\\%s", m_sRoot, m_sUserName, strgroup);

	CString strData(_T(""));

	CFile file;

	CFileException ex;

	int nTry = 0;
	int nFileSize = 0;
	while(1)
	{
		if (file.Open(strUserPath, CFile::modeRead | CFile::shareDenyWrite, &ex))
		{
			nFileSize = (int)file.GetLength();

			char* buffer = new char[nFileSize + 1];

			if ( nFileSize > 0 )
			{
				ZeroMemory(buffer, nFileSize + 1);

				FillMemory(buffer, nFileSize + 1, 0L);

				file.Read(buffer, nFileSize + 1);

				strData.Format("%s", buffer);
			}

			delete []buffer;

			file.Close();

			break;
		}
		else
		{
			if (nTry >= 10)
				break;

			Sleep(10);

			nTry ++;
		}
	}

	CString sTmp(_T(""));

	int ncnt = 0;
	ncnt = 0;

	CString strCode(_T(""));
	CString strCodeName(_T(""));
	CString strMangi(_T(""));
	CString strsuik(_T(""));
	CString strName(_T(""));
	CString strCodeData(_T(""));
	CString stmp(_T(""));
	CString strCount(_T(""));

	for ( int i = 0 ; i < 80 ; i ++ )
	{
		if (strData.IsEmpty())
			break;

		strCodeData = Parser(strData, _T("\n"));
		strCode = Parser(strCodeData, _T("\t"));
		strName = Parser(strCodeData, _T("\t"));
		strsuik = Parser(strCodeData, _T("\t"));
		strMangi = Parser(strCodeData, _T("\t"));
		strCode.TrimRight();
		strCode.TrimLeft();

		stmp.Format(_T("%s"), strCode);
		strCodeName = _T("");
		AddListOfCodeMap(strgroup, stmp, strName, strsuik, strMangi);
		ncnt++;
	}	
}

void CControlwnd::AddListOfCodeMap(CString strgroup, CString sCode, CString sName, CString strsuik, CString strMangi)
{
	CArray<CCodeData, CCodeData>* ptrArr = NULL;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	CString	 strkey;
	POSITION pos;

	for (pos = m_mapGroupCodemap.GetStartPosition(); pos; )
	{
		m_mapGroupCodemap.GetNextAssoc(pos, strkey, pmap);
		if(strkey == strgroup)
		{
			CCodeData cd;
			cd.strCode = sCode;
			cd.strName = sName;
			cd.strsuik = strsuik;
			cd.strMangi = strMangi;
			pmap->SetAt(sCode, cd);
		}
	}
}

CString CControlwnd::GetCombodata(int len, int icnt, CString strgroup)
{
	CString strresult, tmpS;
	if(m_mapGroupKey.Lookup(strgroup, strresult))
		return strresult;
	else 
	{
		tmpS.Format(_T("%d"), icnt);
		strresult.Format(_T("%08d"), icnt);
		strresult = strresult.Right(len);
		return strresult;
	}
}

CString CControlwnd::GetCombodata(int len, int icnt)
{
	CString strresult, tmpS;
	tmpS.Format(_T("%d"), icnt);

	strresult.Format(_T("%08d"), icnt);
	strresult = strresult.Right(len);

	return strresult;
}

BSTR CControlwnd::GetGroupCombo(LPCTSTR cnt)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	int len = 0, icnt = 1;
	CString tmpS;
	CString strResult;
	CString strkey;
	
	tmpS.Format(_T("%s"), cnt);
	len = atoi(tmpS);

	strResult = GetCombodata(len, 0);
	strResult +=  _T("\t");

	//POSITION pos;
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	
	/*
	for (pos = m_mapGroupCodemap.GetStartPosition(); pos; )
	{
		m_mapGroupCodemap.GetNextAssoc(pos, strkey, pmap);
		strResult += GetCombodata(len, icnt);
		strResult +=  strkey;
		strResult += _T("\t");
		icnt++;
	}
	*/

	POSITION pos_group;
	icnt = 0;
	//if(m_mapGroupCodemap.GetSize() == 0)

	CString key, tmps;
	CStringArray arr;
	for (pos_group = m_mapGroupKey.GetStartPosition(); pos_group; )
	{
		m_mapGroupKey.GetNextAssoc(pos_group, key, tmps);
		tmps.Empty();
		tmps += GetCombodata(len, icnt);
		tmps +=  key;
		tmps += _T("\t");
		arr.Add(tmps);
		icnt++;			
	}

	for(int ii = 0 ; ii < arr.GetSize() ; ii++)
		strResult += arr.GetAt(arr.GetSize() - 1 - ii);

	return strResult.AllocSysString();
}

void CControlwnd::SetGroupByName(LPCTSTR group)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	m_sGroupName.Format(_T("%s"), group);
	m_sGroupName.TrimRight();
	//return strResult.AllocSysString();
}

void CControlwnd::SetbInter(LPCTSTR binter)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	CString tmpS;
	tmpS.Format(_T("%s"), binter);
	if(atoi(tmpS) == 0)
		m_binter = false;
	else
		m_binter = true;
}

void CControlwnd::Setsuik(LPCTSTR bsuik)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
}

void CControlwnd::Setmyhoga(LPCTSTR bmyhoga)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
}

void CControlwnd::Setcodetype(LPCTSTR type)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 여기에 디스패치 처리기를 추가합니다.
}

void CControlwnd::Sethogatype(LPCTSTR typehoga)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString tmpS;
	tmpS.Format(_T("%s"), typehoga);

	if(tmpS == _T("전체"))
		m_shogatype == _T("00");
	else if(tmpS == _T("매도"))
		m_shogatype == _T("01");
	else if(tmpS == _T("매수"))
		m_shogatype == _T("02");
	else if(tmpS == _T("확정"))
		m_shogatype == _T("03");
}

BSTR CControlwnd::GetRealDate(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	strResult = m_strRealData;
	m_strRealData.Empty();
	return strResult.AllocSysString();
}

BSTR CControlwnd::GetCodeInGroup(LPCTSTR group)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	POSITION pos;
	CString strResult;
	CString strkey, strdata;
	strkey.Format(_T("%s"), group);
	CMap<CString, LPCTSTR, CCodeData, CCodeData>* pmap = NULL;
	if(m_mapGroupCodemap.Lookup(strkey, pmap))
	{
		for (pos = pmap->GetStartPosition(); pos; )
		{
			CCodeData cd;
			pmap->GetNextAssoc(pos, strkey, cd);
			strResult +=  cd.strCode;
			strResult += _T("\t");
		}
	}

	return strResult.AllocSysString();
}

void CControlwnd::GetledgerH(struct _ledgerH* pLedgerH, int gubn)
{
	//_ledgerH ledger;
	CString strtemp;
	strtemp = "FITS-HTS";
	memcpy(pLedgerH->anam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());  

	if(gubn == 0)
	{
		strtemp = "FITSComAttGrpSO"; 
		memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

		strtemp = "selectAttGrpList";
		memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
	}
	else
	{
		strtemp = "FITSUsrNmSO";
		memcpy(pLedgerH->snam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());

		strtemp = "selectUsrNm";
		memcpy(pLedgerH->fnam, (LPSTR)(LPCTSTR)strtemp, strtemp.GetLength());
	}
}

void CControlwnd::test()
{
	CString strresult;

	for(int ii = 0 ; ii < 50 ; ii++)
	{
		strresult.Format(_T("%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s"), 
																			   "202","test",  //주문번호
																			   "203","test",  //날짜 
																			   "204","test",		//시간	
																			   "205","test",  //종목코드 
																			   "206","test",  //종목명 
																			   "207","test",  //채권종류  
																			   "208","test",  //만기일 
																			   "209","test",  //잔존일 
																			   "210","test",  //민평 
																			   "211","test", //수익율유형  
																			   "212","test", //호가유형코드 
																			   "213","test",  //호가유형
																			   "214","test",  //수익율이나 가격
																			   "215","test",  //대비  
																			   "216","test",  //수량 
																			   "217","test",  //결제일   
																			   "218","test",  //선/교?  
																			   //"218",strsun,  //선/교?  
																			   "219","test",  //집중거래호가
																			   "220","test", //송신자 ID 14
																		       "221","test"	//회사코드				   																																										   
																			   );   
		KillTimer(TM_ALERTALL);
		SetTimer(TM_ALERTALL, 30,0);
		m_arr.Add(strresult);
	}
	//m_arr.Add(m_strRealData);
	
}

void CControlwnd::SetSync(LPCTSTR sync)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strdata;
	strdata.Format(_T("%s"), sync);
	if(strdata == "Y")
		m_bsync = true;
	// TODO: 여기에 디스패치 처리기를 추가합니다.
}

bool CControlwnd::sendTR(CString trC, char* pBytes, int nBytes, int gubn)
{
	int	len = L_userTH + nBytes + m_Param.name.GetLength() + 1 + sizeof(struct  _ledgerH);
	char*	sndB = new char[len];
	ZeroMemory(sndB, sizeof(sndB));
	sprintf(sndB, _T("%s\t"), m_Param.name);
		
	struct _userTH* udat = (struct _userTH*)&sndB[m_Param.name.GetLength() + 1];
	memcpy(udat->trc, trC, trC.GetLength());
	udat->key = m_Param.key;	
	udat->stat = 0;

	struct  _ledgerH ledgerH, *pledgerH;	
	pledgerH = new _ledgerH;
	int nn = sizeof(struct  _ledgerH);
	FillMemory(&ledgerH, sizeof(struct  _ledgerH), ' ');
	GetledgerH(&ledgerH, gubn);
	CopyMemory(&sndB[L_userTH + m_Param.name.GetLength() + 1], &ledgerH, sizeof(struct  _ledgerH));
	CopyMemory(&sndB[L_userTH + m_Param.name.GetLength() + 1 + sizeof(struct  _ledgerH)], pBytes, nBytes);

	int iret = m_pWizard->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, nBytes + sizeof(struct  _ledgerH)), (LPARAM)sndB);
	delete [] sndB;
	if(iret)
		return true;
	else
		return false;
	//return iret;
}

void CControlwnd::MessangerMemoPop(LPCTSTR userid)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CString strdata;
	m_strMsgUser.Empty();
	char  *pbuf = NULL;
	char  *psub = NULL;

	psub = new char[sizeof(struct mid) + 1];
	memset(psub, 0, sizeof(struct mid) + 1);
	
	struct mid* pmid = new struct mid;
	FillMemory(pmid, sizeof(struct  mid), ' ');
	strdata.Format(_T("%s"), userid);
	strdata.TrimRight();
	memcpy(pmid->userid, (char*)strdata.operator LPCSTR(), strdata.GetLength());
	memcpy(psub, pmid, sizeof(struct mid));

	if(sendTR("PIBOPFRM", psub, sizeof(struct mid)))
		m_strMsgUser = userid;
}

void CControlwnd::SearchInterList()
{
	CString strdata;
	
	char  *pbuf = NULL;
	char  *psub = NULL;

	psub = new char[sizeof(struct mid) + 1];
	memset(psub, 0, sizeof(struct mid) + 1);
	
	struct mid* pmid = new struct mid;
	FillMemory(pmid, sizeof(struct  mid), ' ');
	strdata.Format(_T("%s"), m_sUserID.TrimRight());
	strdata.TrimRight();
	memcpy(pmid->userid, (char*)strdata.operator LPCSTR(), strdata.GetLength());
	memcpy(psub, pmid, sizeof(struct mid));

	sendTR("PIBOPFRM", psub, sizeof(struct mid), 0);
}

void CControlwnd::oubSearchList(char* pdata)
{
	//관심그룹 리스트 조회결과
	typedef struct _listmod
	{
		char fill[30];
		char gubun;     //서비스구분 1
		char usrid[30];  //사용자ID
		char succesYN;  //서비스성공여부 
		char attCnt[4];  //관심그룹갯수
		char errMsg[200]; //오류메시지
		char attGrpNm[20]; //관심그룹명 
		char gubn; //공통인지 사용자용인지 구분   C 공통 U 사용자
	}LISTMID;
	
	CString strsuccess, tmpS, temp;

	strsuccess = CString(&pdata[sizeof(struct  _ledgerH) + 61], 1);    //서비스 성공여부
	if(strsuccess != "Y")
	{
	//	AfxMessageBox( CString(&pdata[sizeof(struct  _ledgerH) + 36], 200));
		return;
	}
	

	int cnt = atoi(CString(&pdata[sizeof(struct  _ledgerH) + 62], 4));   //그룹갯수

	if(cnt <= 0)
		return;

	m_mapGroupKey.RemoveAll();

	int ipos = sizeof(struct  _ledgerH) + 1 + 60 + 1+ 4 + 200;
	for(int ii = 0 ; ii < cnt ; ii++)
	{
		temp = CString(&pdata[ipos + (ii * 21)], 21);
		tmpS = temp.Left(20);
		tmpS.TrimRight();
		tmpS.TrimLeft();
		m_mapGroupKey.SetAt(tmpS, temp.Right(1));
	}

	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(eventDLL, MAKEWORD(m_Param.key, evOnClick)), (LPARAM)(LPCTSTR)m_Param.name);
}

BSTR CControlwnd::GetGrouptype(LPCTSTR groupname)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult;

	// TODO: 여기에 디스패치 처리기를 추가합니다.
	if(!m_mapGroupKey.Lookup(groupname, strResult))
		strResult = _T("");
	return strResult.AllocSysString();
}

void CControlwnd::AddComma(char* pcData)
{
	CString strBuffer = CString(pcData);
	strBuffer.TrimLeft();
	strBuffer.TrimRight();

	int iLength = strBuffer.GetLength();
	int iPosPoint = strBuffer.Find('.') + 1;
	int iNaturalLength = iLength - iPosPoint;

	if (iNaturalLength < 4)
	{
		return;
	}


	int iCommaCount = (iNaturalLength - 1) / 3;


	int iDest = 0;
	for (int ii = 0; ii < iNaturalLength; ii++)
	{
		if (	(iNaturalLength - ii) % 3 == 0
			&& ii != 0)
		{
			pcData[iDest++] = ',';
		}

		pcData[iDest++] = strBuffer[ii];
	}

	pcData[iNaturalLength + iCommaCount] = 0;
}