// TreeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "IB202700.h"
#include "TreeWnd.h"

#include "InputPswd.h"
#include "MainWnd.h"

#include "../../h/group.h"
#include "../../h/ledger.h"
#include "qsort.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
/////////////////////////////////////////////////////////////////////////////
// CTreeWnd
#define	LEDGER_ECODE(param)	(m_gubn=='H'?((_ledgerHanaH*)param)->ecode:((_ledgerDaetooH*)param)->ecode)
#define	LEDGER_CONF(param)	(m_gubn=='H'?((_ledgerHanaH*)param)->contf[0]:((_ledgerDaetooH*)param)->contf[0])
//#define	L_ledgerH		(m_gubn=='H'?L_ledgerHanaH:L_ledgerDaetooH)

constexpr struct _exKospi {
	char* code;
	char* name;
} exKospi[] = {
	{"101", "KOSPI200"},	{"401", "KOSPI100"},
	{"402", "KOSPI50"},	//{"403", "KOSPI IT"},
};
constexpr int cnt_exKospi = sizeof(exKospi) / sizeof(struct _exKospi);

constexpr struct _exKosdaq {
	char* code;
	char* name;
} exKosdaq[] = {{"", ""}
	//{"301", "KOSDAQ50"},			// 20070608
//	{"242", "프리미어"},
	/*{"303", "KOSTAR"},*/
};

constexpr int cnt_exKosdaq = sizeof(exKosdaq) / sizeof(struct _exKosdaq);

constexpr char* xxKOSPI[] = {
     "",
     "KGG01P", //코스피         1
     "KGZ01P", //코스피 대형주	2	
     "KGZ02P", //코스피 중형주	3
     "KGZ03P", //코스피 소형주	4
     "KGS01P", //음식료품	5
     "KGS02P", //섬유, 의복	6
     "KGS03P", //종이, 목재	7
     "KGS04P", //화학		8
     "KGS05P", //의약품		9	
     "KGS06P", //비금속광물	10
     "KGS07P", //철강및금속	11
     "KGS08P", //기계		12
     "KGS09P", //전기, 전자	13
     "KGS19P", //의료정밀	14
     "KGS10P", //운수장비	15
     "KGS11P", //유통업		16
     "KGS20P", //전기가스업	17
     "KGS12P", //건설업		18	
     "KGS13P", //운수창고	19
     "KGS21P", //통신업		20
     "KGS14P", //금융업		21
     "KGS15P", //은행		22
     "",
     "KGS16P", //증권		24
     "KGS17P", //보험		25
     "KGS22P", //서비스업	26
     "KGS18P"  //제조업		27
};

constexpr char* xxKOSDAQ[] = { "", "", "", "", "", "", 

     "QGS02P",    // 제조 6
     "QGS03P",    // 건설 7
     "QGS04P",	  // 유통 8
     "",
     "QGS31P",	  // 운송 10 
     "QGS05P",    // 금융 11
     "QGS06P",    // 통신방송 12
     "QGS07P",	  // IT S / W & SVC  13
     "QGS08P",	  // IT H / W  14      
     "QGS09P",    // 음식료·담배  15 
     "QGS10P",	  // 섬유·의류  16
     "QGS11P",    // 종이 목재   17
     "QGS12P",	  // 출판·매체복제  18
     "QGS13P",	  // 화학         19
     "QGS33P",	  // 제약         20
     "QGS14P",    // 비금속	  21
     "QGS15P",	  // 금 속        22
     "QGS16P",    // 기계·장비   23
     "QGS17P",	  // 일반전기전자 24
     "QGS18P",	  // 의료·정밀기기   25
     "QGS19P",	//   운송장비·부품   26
     "QGS20P",  //   기타 제조        27
     "QGS21P",  //   통신서비스       28
     "QGS22P",  //   방송서비스       29
     "QGS23P",	//   인터넷	      30
     "QGS24P",  //   디지털컨텐츠     31
     "QGS25P",	//	소프트웨어    32
     "QGS26P",   //   컴퓨터서비스    33
     "QGS27P",	//   통신장비	      34
     "QGS28P",  //	정보기기      35
     "QGS29P",	//	반도체        36
     "QGS30P",        //IT부품        37
     "QGS87P",	//         
     "",
     "",	//		     39
     "",	//		     40
     "QGS34P",	//오락·문화         41
     "QGG03P",  //코스닥 우량기업부  42
     "QGG04P",  //코스닥 벤처기업부  43
     "QGG05P",  //코스닥 중견기업부  44
     "QGG06P"    //코스닥 기술성장기업부 45 

};

#define TM_BALANCE100 9898

CTreeWnd::CTreeWnd(CWnd* pMainWnd)
{
	m_pMainWnd = pMainWnd;
	m_selectTree = -1;
	loadingHJcode();
}

CTreeWnd::~CTreeWnd()
{
	m_ilTree.DeleteImageList(); 

	_ETFitem.clear();
	_ETFtema.clear();
	_ETFfore.clear();
	_ETFcode.clear();
}


BEGIN_MESSAGE_MAP(CTreeWnd, CTreeCtrl)
	//{{AFX_MSG_MAP(CTreeWnd)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CLICK, OnClick)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreeWnd message handlers

int CTreeWnd::GetChildCount(HTREEITEM hItem)
{
	if (!this->ItemHasChildren(hItem))
		return 0;

	HTREEITEM	hChild = GetChildItem(hItem);
	int		ncnt = 0;

	while (hChild)
	{
		ncnt++;
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}

	return ncnt;
}

CString CTreeWnd::CutCount(CString text)
{
	const int nPos = text.Find("(");
	if (nPos >= 0)
	{
		text = text.Left(nPos);
		text.TrimRight();
	}

	return text;
}

BOOL CTreeWnd::IsFolder(HTREEITEM hItem)
{
	return this->ItemHasChildren(hItem);
}

BOOL CTreeWnd::IsItem(HTREEITEM hItem)
{
	return !this->ItemHasChildren(hItem);
}


HTREEITEM CTreeWnd::FindChild(HTREEITEM hItem, CString text)
{
	HTREEITEM	hRet = nullptr;
	HTREEITEM	hChild = GetChildItem(hItem);
	CString		str = _T("");

	while (hChild)
	{
		str = GetItemText(hChild);

		if (str.CompareNoCase(text) == 0)
		{
			hRet = hChild;
			break;
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
	return hRet;
}

int CTreeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	init();
	return 0;
}

void CTreeWnd::loadingHJcode()
{
	if (m_hjcode.GetSize() > 0) 
		return;
	
	DllInvoker dll("AxisCode");
	if (!dll.IsLoaded())
		return;

	auto map = (std::map<CString, struct hjcodex*>*)dll.Function<char* WINAPI(int)>("getArray")(1);
	if (map->size() <= 0)
		return;

	AxStd::async( [this, &map](){
		for_each(map->begin(), map->end(), [&](const auto item) {
			const auto& codeinfo = item.second;	
			struct _shjcode hjcode;
			hjcode.name = CString(codeinfo->hnam, HNameLen).Trim().MakeUpper();	
			hjcode.symb = CString(codeinfo->symb, HSymbolLen).Trim();
			hjcode.ecng = codeinfo->ecng;
			hjcode.size = codeinfo->size;
			hjcode.ucdm = CString(codeinfo->acdm, 6).Trim();
			hjcode.ucds = CString(codeinfo->acds, 6).Trim();
			hjcode.acdl = CString(codeinfo->acdl, 6).Trim();
			hjcode.jjug = codeinfo->jjug;
			hjcode.kpgb = codeinfo->kpgb;
			hjcode.ucmd = codeinfo->ucmd;
			hjcode.kosd = (char)codeinfo->kosd;
			hjcode.ssgb = codeinfo->ssgb;
			hjcode.wsgb = codeinfo->wsgb;
			hjcode.jsiz = codeinfo->jsiz;
			hjcode.itgb = codeinfo->itgb;
			hjcode.star = codeinfo->star;
			hjcode.unio = codeinfo->unio;
			hjcode.prmr = codeinfo->kqpg;
			hjcode.nxtf = codeinfo->nxtf;
				
			switch (codeinfo->ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
				hjcode.code = CString(codeinfo->code, HCodeLen);
				break;
			default:
				hjcode.code = CString(codeinfo->code, HCodeLen).Mid(1);
				break;
			}
			m_hjcode.Add(hjcode);	
		});
	});	
}


void CTreeWnd::loadingFJcode()
{
	if (m_fjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode;
	struct	_sfjcode s_fjcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, FJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox("선물 파일을 읽을 수 없습니다.");
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct fjcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&fjcode, sizeof(struct fjcode));

		s_fjcode.code = CString(fjcode.cod2, FCodeLen);
		s_fjcode.name = CString(fjcode.hnam, FNameLen);

		m_fjcode.Add(s_fjcode);
	}

	fileH.Close();
}

void CTreeWnd::loadingOJcode()
{
	if (m_ojcode.GetSize() > 0) return;	

	CFile	fileH;
	CString	filePath;

	struct  ojcodh  ojcodH;
	struct  ojcode  ojcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, OJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox("옵션 파일을 읽을 수 없습니다.");
		return;
	}

	fileH.Read(&ojcodH, sizeof(struct ojcodh));

	const int countC = gsl::narrow_cast<int>((fileH.GetLength() - sizeof(struct ojcodh)) / sizeof(struct ojcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&ojcode, sizeof(struct ojcode));
		m_ojcode.Add(ojcode);
	}

	fileH.Close();
}

void CTreeWnd::loadingPJcode()
{
	if (m_pjcode.GetSize() > 0) return;	

	CFile	fileH;
	CString	filePath;
	struct  pjcode  pjcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, PJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox("현물옵션 파일을 읽을 수 없습니다.");
		return;
	}

	CString		snam, tjgb;
	m_mapPJCODE.RemoveAll();

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct pjcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&pjcode, sizeof(struct pjcode));
		m_pjcode.Add(pjcode);
		tjgb.Format("%.*s", sizeof(pjcode.tjgb), pjcode.tjgb);
		snam.Format("%s", pjcode.snam);
		snam.TrimRight();
		if (!m_mapPJCODE.IsExist(snam))
			m_mapPJCODE.SetData(snam, tjgb);
	}

	fileH.Close();
}

void CTreeWnd::loadingUPcode()
{
	if (m_upcode.GetSize() > 0) return;	

	CFile	fileH;
	CString	filePath;
	struct  upcode  upcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, UPCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox("업종코드 파일을 읽을 수 없습니다.");
		return;
	}

	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct upcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&upcode, sizeof(struct upcode));
		m_upcode.Add(upcode);
	}

	fileH.Close();
}

void CTreeWnd::loadingCFcode(bool bCurrency)//2012.10.04 KSJ 금리(6x), 통화(7x)
{	
	if(bCurrency)
	{
		if (m_cccode.GetSize() > 0) return;	
		
		CFile	fileH;
		CString	filePath, tjgb;
		struct  ccode  ccode;
		
		filePath.Format("%s/%s/%s", m_root, TABDIR, CFCODE);
		if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
		{
			AfxMessageBox("금리/통화선물코드 파일을 읽을 수 없습니다.");
			return;
		}
		
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct ccode));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&ccode, sizeof(struct ccode));

			tjgb = CString(ccode.tjgb, 2);

			if(tjgb.GetAt(0) == '7')
				m_cccode.Add(ccode);
		}
		
		fileH.Close();
	}
	else
	{
		if (m_cicode.GetSize() > 0) return;	
		
		CFile	fileH;
		CString	filePath, tjgb;
		struct  ccode  ccode;
		
		filePath.Format("%s/%s/%s", m_root, TABDIR, CFCODE);
		if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
		{
			AfxMessageBox("금리/통화선물코드 파일을 읽을 수 없습니다.");
			return;
		}
		
		const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct ccode));
		for (int ii = 0; ii < countC; ii++)
		{
			fileH.Read(&ccode, sizeof(struct ccode));

			tjgb = CString(ccode.tjgb, 2);
			
			if(tjgb.GetAt(0) == '6')
				m_cicode.Add(ccode);
		}
		
		fileH.Close();
	}
}

void CTreeWnd::loadingSFcode()	//2012.10.04 KSJ 주식선물
{
	if (m_sfcode.GetSize() > 0) return;	
	
	CFile	fileH;
	CString	filePath, strCode;
	struct  sfcode  sfcode;
	
	filePath.Format("%s/%s/%s", m_root, TABDIR, SFCODEDAT);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		AfxMessageBox("주식선물코드 파일을 읽을 수 없습니다.");
		return;
	}
	
	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct sfcode));
	for (int ii = 0; ii < countC; ii++)
	{
		fileH.Read(&sfcode, sizeof(struct sfcode));
		
		strCode = CString(sfcode.codx, 8);

		if(strCode.GetAt(0) == '1' || strCode.GetAt(0) == 'A')  //파생상품 코드개편
			m_sfcode.Add(sfcode);
	}
	
	fileH.Close();
}

void CTreeWnd::getELWBaseAsset(UINT selItem, HTREEITEM parent)
{
	struct mid
	{
		char    flag[1];
		char    ascf[1];            // 0: 상위 1: 하위  
		char    kcod[1];            //권리유형 code    
		char    gcod[12];           // 기초자산 code    
		char    bcod[12];           // 발행사 code      
		char    jjis[1];            // 잔존일수        
		char    sort[2];            // Sorting 구분     
		char    rtsf[9];            /// 1. RTS  0: NOT
		char    fill[1];            // dummy          
		char    gvol[1];            // 1. gvol > 0   
		char    gsrt[1];            // 1: 그리드소팅  
	};
	
	//선택한 Key값을 기준으로 코드값 가져오기
	int readL = 0; 
	char readB[256]{};
	CString	filePath, string, string_code, string_name, skey, kcod;
	
	filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWUD.CBS");

	//아이템 클릭시만 Send
	UINT	sItem = GetItemData(parent);
	struct _treeID*	treeID = CAST_TREEID(sItem);

	switch (treeID->depth)
	{
	case TD_ROOT:
		return;
	case TD_MID:
		break;
	case TD_ITEM:
		break;
	}

	//콜인지 풋인지 확인
	if(GetItemText(parent) == "콜")
	{
		kcod = _T("1");
	}
	else if(GetItemText(parent) == "풋")
	{
		kcod = _T("2");
	}
	else
	{
		kcod = _T(" ");
	}
	
	for(int ii=0 ; ii<200 ; ii++)
	{
		skey.Format("%d", ii);
		readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
		if(readL <= 0) break;
		if(ii == selItem)
		{
			string = CString(readB, readL);
			string.TrimRight();
			string_code = string.Left(12);
			break;
		}	
	}	
	
	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');	
	CopyMemory(&mid.flag, _T("1"), sizeof(mid.flag));
	CopyMemory(&mid.ascf, _T("0"), sizeof(mid.ascf));
	CopyMemory(&mid.kcod, kcod, sizeof(mid.kcod));
	CopyMemory(&mid.gcod, string_code, sizeof(mid.gcod));
	CopyMemory(&mid.bcod, _T("A99999999999"), sizeof(mid.bcod));
	CopyMemory(&mid.jjis, _T("0"), sizeof(mid.jjis));
	CopyMemory(&mid.sort, _T("01"), sizeof(mid.sort));
[[gsl::suppress(6385)]]
	CopyMemory(&mid.rtsf, _T("01"), sizeof(mid.rtsf));
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));
	
	CString strSendData = CString((char*)&mid, sizeof(mid));
	CSendData sData;
	
	sData.SetData("PIBO2805", TRKEY_ELWBASEASSET, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}

void CTreeWnd::clickRecommand(BOOL bRead)
{
	struct  mid 
	{
		char    gubn[ 1];   /*  구분('0':헤더, '1':본문)    */
				/*  '2': 종목코드리스트         */
	};

	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');	

	CopyMemory(&mid.gubn, _T("2"), sizeof(mid.gubn));

	CString strSendData = CString((char*)&mid, sizeof(mid));
	CSendData sData;

  	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));	

	if(bRead)
	{
 		pWnd->SendMessage(WM_MANAGE, MK_SAVERECOMMANDGROUP);
	
		sData.SetData("pihorsdt", TRKEY_RECOMMAND, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
		m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	}	
}

void CTreeWnd::getELWIssueSec(UINT selItem, HTREEITEM parent)
{
	struct mid
	{
		char    flag[1];
		char    ascf[1];            // 0: 상위 1: 하위  
		char    kcod[1];            //권리유형 code    
		char    gcod[12];           // 기초자산 code    
		char    bcod[12];           // 발행사 code      
		char    jjis[1];            // 잔존일수        
		char    sort[2];            // Sorting 구분     
		char    rtsf[9];            /// 1. RTS  0: NOT
		char    fill[1];            // dummy          
		char    gvol[1];            // 1. gvol > 0   
		char    gsrt[1];            // 1: 그리드소팅  
	};

	//아이템 클릭시만 Send
	UINT	sItem = GetItemData(parent);
	struct _treeID*	treeID = CAST_TREEID(sItem);
	
	switch (treeID->depth)
	{
	case TD_ROOT:
		return;
	case TD_MID:
		break;
	case TD_ITEM:
		break;
	}

	//선택한 Key값을 기준으로 코드값 가져오기
	int readL = 0; 
	char readB[256]{};
	CString	filePath, string, string_code, string_name, skey, kcod;

	if(GetItemText(parent) == "콜")
	{
		kcod = _T("1");
	}
	else if(GetItemText(parent) == "풋")
	{
		kcod = _T("2");
	}
	else
	{
		kcod = _T(" ");
	}

	
	filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWLP.CBS");
	
	for(int ii=0 ; ii<200 ; ii++)
	{
		skey.Format("%d", ii);
		readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
		if(readL <= 0) break;
		
		if(ii == selItem)
		{
			string = CString(readB, readL);
			string.TrimRight();
			string_code = string.Left(12);
			break;
		}
	}	

	struct mid mid;
	FillMemory(&mid, sizeof(mid), ' ');

	CopyMemory(&mid.flag, _T("1"), sizeof(mid.flag));
	CopyMemory(&mid.ascf, _T("0"), sizeof(mid.ascf));
	CopyMemory(&mid.kcod, kcod, sizeof(mid.kcod));
	CopyMemory(&mid.gcod, _T("A99999999999"), sizeof(mid.gcod));
	CopyMemory(&mid.bcod, string_code, sizeof(mid.bcod));
	CopyMemory(&mid.jjis, _T("0"), sizeof(mid.jjis));
	CopyMemory(&mid.sort, _T("01"), sizeof(mid.sort));
[[gsl::suppress(6385)]]
	CopyMemory(&mid.rtsf, _T("01"), sizeof(mid.rtsf));
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));

	CString strSendData = CString((char*)&mid, sizeof(mid));
	CSendData sData;

	sData.SetData("PIBO2805", TRKEY_ELWISSUESEC, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);

}

void CTreeWnd::loadingELWBase()
{
	if (m_elwbase.GetSize() > 0)
		return;

	int	codeN{};
	CFile	file;
	_elwdata elwdata;
	_sfjcode	sjcode;
	struct  elwcode  ELWCode;

	CString path = m_root + "\\tab\\elwcod.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))	return;

	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));

	bool	bFound{};
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));

		elwdata.code = CString(ELWCode.codx, sizeof(ELWCode.codx));
		elwdata.code.TrimRight();	
		elwdata.name.Format("%s", CString(ELWCode.hnam, ELWHNameLen).Mid(1));
		elwdata.name.TrimRight(); elwdata.name.TrimLeft();
		elwdata.ktype = CString(ELWCode.krye, 2);
		elwdata.pcode = CString(ELWCode.bgce, 12);
		elwdata.pname = CString(ELWCode.hbge, 20);
		elwdata.mjmt  = CString(ELWCode.mjmt, 8);
		elwdata.bkoba = (ELWCode.koba[0]==2) ? TRUE : FALSE;
		
		for (int jj = 0; jj < 5; jj ++)
		{
			elwdata.bcode[jj] = CString((char*)ELWCode.gcjs[jj], 12);
			elwdata.bcode[jj].TrimRight();
		}
		
		m_ELWdata.Add(elwdata);

		for (int jj = 0; jj < 5; jj ++)
		{
			CString bcode = CString((char*)ELWCode.gcjs[jj], 12);
			bcode.TrimRight();
			if (bcode == "") break;

			bcode = bcode.Mid(1);
			bFound = false;
			for (int kk = 0; kk < m_elwbase.GetSize(); kk++)
			{
				if (bcode.CompareNoCase(m_elwbase.GetAt(kk).code) == 0)
				{
					bFound = true;
					break;
				}
			}

			if (!bFound)
			{
				CString	bname = GetCodeName(bcode);
				if (!bname.IsEmpty())	
					;
				else if (bcode.CompareNoCase("OSPI200") == 0)
					continue;
				else if (bcode.CompareNoCase("ASKET") == 0)
					break;

				sjcode.code = bcode;
				sjcode.name = bname;
				sjcode.name.TrimRight();

				m_elwbase.Add(sjcode);
			}
		}
	}

	file.Close();
}

void CTreeWnd::loadingInterest(HTREEITEM hParent)
{
	HTREEITEM	hChild = GetChildItem(hParent);
	HTREEITEM	hTemp = 0;
		
	while (hChild)
	{
		hTemp = GetNextItem(hChild, TVGN_NEXT);
		DeleteItem(hChild);
		hChild = hTemp;
	}
	
	SetItemDataX(hParent, MAKE_TREEID(xINTEREST));
}



void CTreeWnd::init()
{
	m_root.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETROOT));
	m_user.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETUSER));
	m_id.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	m_pass.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_VARIANT, passCCx)));
	m_sDeptCode.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_VARIANT, deptCC)));

	CString	gubn = _T("");
	gubn.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_VARIANT, systemCC)));

	if (!gubn.IsEmpty())
		m_gubn = gubn.GetAt(0);
	else
		m_gubn = ' ';
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));

	if (pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		m_bCustomer = true;


	CString accInfo = Variant(accountCC,"");
	m_accn.load_acc(accInfo);
	loadingETFcode();
	loadingELWBase();

	initTree();
//	settingReminAcc();
	selectKospi();
}

void CTreeWnd::settingReminAcc()
{
	HTREEITEM root_Item{};
	CString root_name, child_name;
	CStringArray* pArr = m_accn.get_account();

	//HTREEITEM hItem;
	root_Item = GetRootItem();

	while(root_Item)
	{
		root_name = GetItemText(root_Item);

		// 관심종목그룹 Add
		
		if(root_name == "보유종목")
		{
			for(int i=0 ; i<pArr->GetSize() ; i++)
			{
				CString strAccn = pArr->GetAt(i);	
				CString strText = strAccn.Left(3) + "-" + strAccn.Mid(3,2) + "-" + strAccn.Right(6);
				CString strT = strAccn.Mid(3,1);
				
				if( strT != "5" && strT != "0" )
				{
					SetItemData(InsertItem(strText, 0, 1, root_Item, TVI_LAST), MAKE_TREEID(xREMAIN));
				}
			}
			break;
		}
		root_Item = GetNextSiblingItem(root_Item);
	}


}

void CTreeWnd::selectKospi()
{
	HTREEITEM root_Item{}, child_Item{};
	CString root_name, child_name;
	bool check = false;
	root_Item = GetRootItem();

	while(root_Item)
	{
		root_name = GetItemText(root_Item);
		
		if(root_name == "관심그룹")
		{
			child_Item = GetChildItem(root_Item);			
			//관심 종목이 있으면 관심종목 보여주기
			if(child_Item)
			{
				SelectItem(child_Item);
//				parent = GetParentItem(hItem);
//				loadingInterest(parent);
//				selectinterest(parent, val);
				check = true;
				break;
			}
		}
		root_Item = GetNextSiblingItem(root_Item);
	}
}



LONG CTreeWnd::OnManage(WPARAM wParam, LPARAM lParam)
{
	LONG	ret = 0;
	const int	kind = HIWORD(wParam);
	switch (LOWORD(wParam))
	{
	case MK_RECVDATA:
		RecvOper(kind, (CRecvData*)lParam);
		break;
	case MK_GETDATATITLE:
		{
			m_szRET.Format("%s", GetDataTitle((DWORD)lParam));
			
// 			int index = m_szRET.Find("보유종목");
// 			if(index > 0)
// 			{
// 				m_szRET += " (※현금/대용 잔고만 확인 가능하십니다.)";
// 			}

			ret = (LONG)(char*)m_szRET.operator LPCTSTR();	
		}	
		break;
	case MK_SELGROUP:
		if (kind == MO_SET)
		{
			HTREEITEM hItem = nullptr;
			if (m_mapParam.Lookup((DWORD)lParam, hItem))
			{
				SelectItem(hItem);
				m_selItem = hItem;
			}
			else
				m_selItem = nullptr;
		}		
		break;
	case MK_SELECTTREE:
		{
			m_selectTree = (int)lParam;
		}
		break;
	case MK_CALLINTEREST:
		{
			HTREEITEM	hItem = nullptr;
			if(m_mapParam.Lookup((DWORD)lParam, hItem))
			{
				SendInterest(hItem);
			}
			
		}
		break;
	case MK_TREEDATA:
		{
			class CGridData* sdata = (class CGridData*)lParam;
			GetData(*sdata, kind);		//2
		}
		break;
	case MK_SETUPOK:
		{
			HTREEITEM	hItem = GetSelectedItem();
			DWORD		val{};
			if (hItem)
			{
				val = GetItemData(hItem);
				
				if (CAST_TREEID(val)->kind == xINTEREST && CAST_TREEID(val)->depth == TD_ITEM)
				{					
					HTREEITEM	hParent = GetParentItem(hItem);
					loadingInterest(hParent);
					selectinterest(hParent, val);
					break;
				}
			}
			
			m_mapParam.Lookup(MAKE_TREEID(xINTEREST), hItem);
			loadingInterest(hItem);
		}
		break;
	case MK_SENDREMAIN:
		{
			clickRemain((int)lParam);
		}
		break;
	case MK_RECOMMAND:
		{
			clickRecommand(TRUE);
			sendUpjong();
		}
		break;
	case MK_READONLYRECOM:
		{
			clickRecommand(FALSE);
			sendUpjong();
		}
		break;
	case MK_TREERESEND:
		{
			ReSend();
		}
		break;
	case MK_GETTREEITEM:
		{
			HTREEITEM	hItem = GetSelectedItem();

			if(hItem != nullptr)
			{
				const UINT	selItem = GetItemData(hItem);	
				ret = selItem;
			}
			else
			{
				ret = -1;
			}

		}
		break;
	case MK_ISSUE:
		{
			queryNews(lParam);
		}
		break;
	}
	return ret;
}

#ifdef DF_NEWTREE
void CTreeWnd::initTree()
{
	HTREEITEM hRoot{}, hItem = nullptr;
	UINT	kind = 0;

	//////////////////////////
			//icon
	CBitmap bm[2];
	bm[0].LoadBitmap(IDB_BITMAP1); ;//< = 32 * 32, 256 비트맵
	bm[1].LoadBitmap(IDB_BITMAP2);

	m_ilTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 1, 2);
	m_ilTree.Add(&bm[0], 0xFF00FF); ; //< = 비트맵에서 투명처리할꺼
	m_ilTree.Add(&bm[1], 0xFF00FF);

	// updateX_20111229
	bm[0].DeleteObject();
	bm[1].DeleteObject();

	SetImageList(&m_ilTree, TVSIL_NORMAL);
	m_ilTree.Detach();
	/////////////////////////
	constexpr UINT mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;

	// 관심종목그룹 Add
	hRoot = InsertItem(_T("관심그룹"), 0, 1);
	_treeInter = hRoot;
	SetItemData(hRoot, kind = MAKE_TREEID(xINTEREST));
	SetItemDataX(hRoot, kind);

	// 관심종목그룹 Add
	hRoot = InsertItem(_T("보유종목"), 0, 1);
	SetItemData(hRoot, kind = MAKE_TREEID(xREMAIN));
	SetItemDataX(hRoot, kind);

	hRoot = InsertItem(_T("이슈종목"), 0, 1);
	SetItemData(hRoot, MAKE_TREEID(xISSUE));

	//주식종목
	hRoot = InsertItem(_T("주식종목"), 0, 1);
	SetItemData(InsertItem(_T("전종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xALLCODE));
	SetItemData(InsertItem(_T("코스피"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSPI));
	SetItemData(InsertItem(_T("코스닥"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSDAQ));
	SetItemData(InsertItem(_T("N X T"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xNXT));
	SetItemData(InsertItem(_T("코스피200"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOPSI200));
	SetItemData(InsertItem(_T("KRX100"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKRX100));
	SetItemData(InsertItem(_T("코스피업종"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSPIUP));
	SetItemData(InsertItem(_T("코스닥업종"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSDAQUP));
	SetItemData(InsertItem(_T("코넥스"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKONEX));		//2013.06.25 KSJ 코넥스 추가
	SetItemData(InsertItem(_T("K-OTC"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xFREECODE));
	SetItemData(InsertItem(_T("ETF"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xETFCODE, 0, 0, 0, TD_MID));
	SetItemData(InsertItem(_T("ETN"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xETNCODE));
	SetItemData(InsertItem(_T("상장형수익증권"),0, 1, hRoot, TVI_LAST), MAKE_TREEID(xGOODSTOCK));		//25.06.12 상장형수익증권 추가
	SetItemData(InsertItem(_T("리츠"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xRITS));
	SetItemData(InsertItem(_T("스팩"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xSPACCODE));	//2015.11.03 KSJ spac 추가
	SetItemData(InsertItem(_T("섹터종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xTHEMACODE));
	SetItemData(InsertItem(_T("그룹사별"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xGROUPCODE));

	HTREEITEM hChild = GetChildItem(hRoot);
	CString child_name;
	while (hChild)
	{
		child_name = GetItemText(hChild);

		if (child_name == "ETF")
		{
			SetItemDataX(hChild, MAKE_TREEID(xETFCODE));
		}
		else if (child_name == "코스피업종")
		{
			SetItemDataUJ(hChild, MAKE_TREEID(xKOSPIUP));
		}
		else if (child_name == "섹터종목")
		{
			SetItemDataX(hChild, MAKE_TREEID(xTHEMACODE));
		}
		else if (child_name == "그룹사별")
		{
			SetItemDataX(hChild, MAKE_TREEID(xGROUPCODE));
		}
		else if (child_name == "코스닥업종")
		{
			SetItemDataUJ(hChild, MAKE_TREEID(xKOSDAQUP));
		}

		hChild = GetNextSiblingItem(hChild);
	}

	//ELW
	hRoot = InsertItem(_T("ELW"), 0, 1);
	SetItemData(InsertItem(_T("전종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWCODE));
	SetItemData(InsertItem(_T("기초자산종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWASSET));
	SetItemData(InsertItem(_T("발행회사"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWISSUE));

	SetItemData(InsertItem(_T("신주인수권"), 0, 1), MAKE_TREEID(xSINJUCODE));

	//신종증권
	hRoot = InsertItem(_T("신종증권"), 0, 1);
	SetItemData(InsertItem(_T("전체"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xALLNEWST));
	SetItemData(InsertItem(_T("신탁수익증권"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xSINTAKST));
	SetItemData(InsertItem(_T("투자계약증권"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xTUSACONST));

	hChild = GetChildItem(hRoot);
	
	while (hChild)
	{
		child_name = GetItemText(hChild);

		if (child_name == "발행회사")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWISSUE));
		}
		else if (child_name == "기초자산종목")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWASSET));
		}
		else if (child_name == "전종목")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWCODE));
		}

		hChild = GetNextSiblingItem(hChild);
	}

	//2012.10.04 KSJ 선물추가
	SetItemData(InsertItem(_T("코스피선물"), 0, 1), MAKE_TREEID(xKOSPIFUTURE));
	
	hRoot = InsertItem(_T("코스피옵션"), 0, 1);
	SetItemData(InsertItem(_T("콜옵션"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xCALLCODE));
	SetItemData(InsertItem(_T("풋옵션"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xPUTCODE));

	SetItemData(InsertItem(_T("주식선물"), 0, 1), MAKE_TREEID(xSTOCKFUTURE));
	SetItemData(InsertItem(_T("금리선물"), 0, 1), MAKE_TREEID(xINTERESTRATEFUTURE));
	SetItemData(InsertItem(_T("통화선물"), 0, 1), MAKE_TREEID(xCURRENCYFUTURE));

	hRoot = InsertItem(_T("업종지수"), 0, 1);
	SetItemData(InsertItem(_T("코스피"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xKOSPIJISU));
	SetItemData(InsertItem(_T("코스닥"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xKOSDAQJISU));
}
#else
void CTreeWnd::initTree()
{
	HTREEITEM hRoot{}, hItem = nullptr;
	UINT	kind = 0;

//////////////////////////
		//icon
	CBitmap bm[2];
	bm[0].LoadBitmap(IDB_BITMAP1);   ;//< = 32 * 32, 256 비트맵
	bm[1].LoadBitmap(IDB_BITMAP2);

	m_ilTree.Create(16, 16, ILC_COLOR8 | ILC_MASK, 1, 2);
	m_ilTree.Add( &bm[0], 0xFF00FF );  ; //< = 비트맵에서 투명처리할꺼
	m_ilTree.Add( &bm[1], 0xFF00FF );

	// updateX_20111229
	bm[0].DeleteObject();
	bm[1].DeleteObject();

	SetImageList(&m_ilTree, TVSIL_NORMAL);
	m_ilTree.Detach();
/////////////////////////
	constexpr UINT mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;

	// 관심종목그룹 Add
//	hRoot = InsertItem(mask, _T("관심그룹"), 0, 1, 0, 0, 0, nullptr, nullptr);
	hRoot = InsertItem(_T("관심그룹"), 0, 1);
	_treeInter = hRoot;
	SetItemData(hRoot, kind = MAKE_TREEID(xINTEREST));
	SetItemDataX(hRoot, kind);
	
	// 관심종목그룹 Add
//	hRoot = InsertItem(mask, _T("보유종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	hRoot = InsertItem(_T("보유종목"), 0, 1);
	SetItemData(hRoot, kind = MAKE_TREEID(xREMAIN));
	SetItemDataX(hRoot, kind);
	
//	hRoot = InsertItem(mask, _T("이슈종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	hRoot = InsertItem(_T("이슈종목"), 0, 1);
	SetItemData(hRoot, MAKE_TREEID(xISSUE));
	
	//주식종목
	hRoot = InsertItem(_T("주식종목"), 0, 1);
	SetItemData(InsertItem(_T("전종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xALLCODE));
	SetItemData(InsertItem(_T("거래소"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSPI));
	SetItemData(InsertItem(_T("코스닥"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSDAQ));
	SetItemData(InsertItem(_T("코넥스"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKONEX));	//2013.06.25 KSJ 코넥스 추가
	SetItemData(InsertItem(_T("K-OTC"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xFREECODE));	
	//SetItemData(InsertItem(_T("ETF"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xETFCODE));

	SetItemData(InsertItem(_T("ETF"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xETFCODE,0,0,0,TD_MID));
	SetItemData(InsertItem(_T("ETN"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xETNCODE));
        SetItemData(InsertItem(_T("상장형수익증권"),0, 1, hRoot, TVI_LAST), MAKE_TREEID(xGOODSTOCK));		//25.06.12 상장형수익증권 추가

	SetItemData(InsertItem(_T("스팩"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xSPACCODE));	//2015.11.03 KSJ spac 추가

	HTREEITEM hChild = GetChildItem(hRoot);
	CString child_name;
	while(hChild)
	{
		child_name = GetItemText(hChild);
		
		if(child_name == "ETF")
		{
			SetItemDataX(hChild, MAKE_TREEID(xETFCODE));
		}
		
		hChild = GetNextSiblingItem(hChild);
	}

	//ELW
	hRoot = InsertItem(_T("ELW"), 0, 1); 
	SetItemData(InsertItem(_T("전종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWCODE));
	//SetItemData(InsertItem(_T("기초자산종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWBASE));
	SetItemData(InsertItem(_T("기초자산종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWASSET));
	SetItemData(InsertItem(_T("발행회사"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWISSUE));

	hChild = GetChildItem(hRoot);
	//CString child_name;
	while(hChild)
	{
		child_name = GetItemText(hChild);

		if(child_name == "발행회사")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWISSUE));
		}
		else if(child_name == "기초자산종목")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWASSET));
		}
		else if(child_name == "전종목")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWCODE));
		}

		hChild = GetNextSiblingItem(hChild);
	}

	//2012.10.04 KSJ 선물추가
// 	SetItemData(InsertItem(mask, _T("선물"), 0, 1, 0, 0, 0, nullptr, nullptr), MAKE_TREEID(xFUTURE));

	SetItemData(InsertItem(_T("KOSPI선물"), 0, 1), MAKE_TREEID(xKOSPIFUTURE));
	SetItemData(InsertItem(_T("주식선물"), 0, 1), MAKE_TREEID(xSTOCKFUTURE));
	SetItemData(InsertItem(_T("금리선물"), 0, 1), MAKE_TREEID(xINTERESTRATEFUTURE));
	SetItemData(InsertItem(_T("통화선물"), 0, 1), MAKE_TREEID(xCURRENCYFUTURE));
	// KSJ

	hRoot = InsertItem(_T("KOSPI옵션"), 0, 1);
	SetItemData(InsertItem(_T("콜옵션"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xCALLCODE));
	SetItemData(InsertItem(_T("풋옵션"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xPUTCODE));

	hRoot = InsertItem(_T("KOSPI업종"), 0, 1);
	SetItemDataUJ(hRoot, MAKE_TREEID(xKOSPIUP));

	hRoot = InsertItem(_T("KOSDAQ업종"), 0, 1);
	SetItemDataUJ(hRoot, MAKE_TREEID(xKOSDAQUP));

	SetItemData(InsertItem(_T("KOSPI200"), 0, 1), MAKE_TREEID(xKOPSI200));
	SetItemData(InsertItem(_T("KRX100"), 0, 1), MAKE_TREEID(xKRX100));
	SetItemData(InsertItem(_T("신주인수권"), 0, 1), MAKE_TREEID(xSINJUCODE));

	hRoot = InsertItem(_T("업종지수"), 0, 1);
	SetItemData(InsertItem(_T("거래소"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xKOSPIJISU));
	SetItemData(InsertItem(_T("코스닥"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xKOSDAQJISU));
	
	// 섹터종목
	hRoot = InsertItem(_T("섹터종목"), 0, 1);
	SetItemDataX(hRoot, MAKE_TREEID(xTHEMACODE));

	// 그룹사별
	hRoot = InsertItem(_T("그룹사별"), 0, 1);
	SetItemDataX(hRoot, MAKE_TREEID(xGROUPCODE));

	// IBK추천종목
// 	hRoot = InsertItem(mask, _T("IBKS추천종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
// 	SetItemData(hRoot, MAKE_TREEID(xRECOMMAND));

	// EPB 추천종목 2016.04.08 KSJ
// 	hRoot = InsertItem(mask, _T("온라인 PB 관심종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
// 	SetItemData(hRoot, MAKE_TREEID(xEPBCODE));
}
#endif

void CTreeWnd::SetItemDataUJ(HTREEITEM hItem, UINT gubn)
{
	SetItemDataX(hItem, gubn);

	if (CAST_TREEID(gubn)->kind == xKOSPIUP)
		SetItemDataX(hItem, xKOSPIex);
	else if (CAST_TREEID(gubn)->kind == xKOSDAQUP)
		SetItemDataX(hItem, xKOSDAQex);
}

void CTreeWnd::SetItemDataX(HTREEITEM hItem, UINT gubn)
{
	struct _treeID*	treeID = CAST_TREEID(gubn);
	UINT	kind = 0;
	switch (treeID->kind)
	{
	case xTHEMACODE:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, stringx, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");			

			for (int ii = 0; ii < 500; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(SEC_FOLDER, skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL); stringx = IH::Parser(string, ";"); stringx.TrimRight();
				kind = MAKE_TREEID(treeID->kind, 0, 3, atoi(string), TD_ITEM);
				SetItemData(InsertItem(stringx, 0, 1, hItem), kind);
			}
		}
		break;

	case xGROUPCODE:
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, stringx, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "gropcd.ini");

			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(SEC_FOLDER, skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;
				
				string = CString(readB, readL);	
				stringx = IH::Parser(string, ";"); 
				stringx.TrimRight();
				kind = MAKE_TREEID(treeID->kind, 0, 3, atoi(string), TD_ITEM);
				SetItemData(InsertItem(stringx, 0, 1, hItem), kind);
			}
		}
		break;

	case xELWISSUE:		//ELW발행회사별 트리에 추가
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey;
			
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWLP.CBS");

			SetItemData(InsertItem(_T("콜"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xELWISSUE, 0, 1, 1, TD_MID));
			SetItemData(InsertItem(_T("풋"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xELWISSUE, 0, 1, 2, TD_MID));

			HTREEITEM hChild = GetChildItem(hItem);
			CString child_name;
			while(hChild)
			{
				child_name = GetItemText(hChild);
				
				if(child_name == "콜")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						IH::Parser(string, string_code); 
						
						if(ii>0)	//전체는 표시안함
						{
							kind = MAKE_TREEID(treeID->kind, 0, 12, ii, TD_ITEM);
							SetItemData(InsertItem(string, 0, 1, hChild, TVI_LAST), kind);
						}
					}					
				}
				else if(child_name == "풋")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						IH::Parser(string, string_code); 
						
						if(ii>0)	//전체는 표시안함
						{
							kind = MAKE_TREEID(treeID->kind, 0, 12, ii, TD_ITEM);
							SetItemData(InsertItem(string, 0, 1, hChild, TVI_LAST), kind);
						}
					}			
				}
				
				hChild = GetNextSiblingItem(hChild);
			}
		}
		break;

	case xELWCODE:
		{
			SetItemData(InsertItem(_T("콜"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xELWCODE, 0, 1, 1, TD_MID));
			SetItemData(InsertItem(_T("풋"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xELWCODE, 0, 1, 2, TD_MID));
			
			HTREEITEM hChild = GetChildItem(hItem);
			CString child_name;
			while(hChild)
			{
				child_name = GetItemText(hChild);
				
				if(child_name == "콜")
				{
					SetItemData(InsertItem(_T("일반ELW"), 0, 1, hChild, TVI_LAST), MAKE_TREEID(xELWGEN, 0, 1, 1, TD_ITEM));
					SetItemData(InsertItem(_T("조기종료ELW"), 0, 1, hChild, TVI_LAST), MAKE_TREEID(xELWEARLY, 0, 1, 2, TD_MID));					
				}
				else if(child_name == "풋")
				{
					SetItemData(InsertItem(_T("일반ELW"), 0, 1, hChild, TVI_LAST), MAKE_TREEID(xELWGEN, 0, 1, 1, TD_ITEM));
					SetItemData(InsertItem(_T("조기종료ELW"), 0, 1, hChild, TVI_LAST), MAKE_TREEID(xELWEARLY, 0, 1, 2, TD_MID));			
				}
				
				hChild = GetNextSiblingItem(hChild);
			}
		}
		break;
	case xETFCODE:		//ETF 트리 추가
		{
			CString	filePath, string, string_code, string_name, skey;
			HTREEITEM hMid = TVI_LAST;
			
			SetItemData(InsertItem(_T("전체종목"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xETFCODE, 0, 1, 1, TD_ITEM));
			SetItemData(InsertItem(_T("운용사별"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xETFCOMPANY, 0, 1, 2, TD_MID));
			SetItemData(InsertItem(_T("기초자산별"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xETFTHEMA, 0, 1, 3, TD_MID));	//2015.11.04 KSJ 테마별 --> 기초자산별로 변경
			SetItemData(InsertItem(_T("국내/외"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xETFDOMINTER, 0, 1, 4, TD_MID));

			_efopitem* efopitem{};
			_efoptema* efoptema{};
			_efopfore* efopfore{};

			CString type, opcd, hnam;
 			CString tmcd, frcd, frnm, code, etfm, etfl;

			HTREEITEM hChild = GetChildItem(hItem);
			CString child_name;
			while(hChild)
			{
				child_name = GetItemText(hChild);
				
				if(child_name == "운용사별")
				{
					int ii = 0;
					for_each(_ETFitem.begin(), _ETFitem.end(), [&](auto& efopitem){
						hnam = CString(efopitem->hnam, sizeof(efopitem->hnam)).Trim();					
						kind = MAKE_TREEID(xETFCOMPANY, 0, 12, ii++, TD_ITEM);
						SetItemData(InsertItem(hnam, 0, 1, hChild, TVI_LAST), kind);
					});			

				}
				else if(child_name == "기초자산별")	//2015.11.04 KSJ 테마별 --> 기초자산별로 변경
				{
					int ii = 0;
					for_each(_ETFtema.begin(), _ETFtema.end(), [&](auto& efoptema){
						hnam = CString(efoptema->tmnm, sizeof(efoptema->tmnm)).Trim();						
						kind = MAKE_TREEID(xETFTHEMA, 0, 12, ii++, TD_ITEM);
						SetItemData(InsertItem(hnam, 0, 1, hChild, TVI_LAST), kind);
					});			
				}
				else if(child_name == "국내/외")
				{
					int ii = 0;
					for_each(_ETFfore.begin(), _ETFfore.end(), [&](auto& efopfore){
						frnm = CString(efopfore->frnm, sizeof(efopfore->frnm)).Trim();						
						kind = MAKE_TREEID(xETFDOMINTER, 0, 12, ii++, TD_ITEM);
						SetItemData(InsertItem(frnm, 0, 1, hChild, TVI_LAST), kind);
					});


				}
				
				hChild = GetNextSiblingItem(hChild);
			}
			
		}
		break;

	case xELWASSET:		//ELW기초자산별 트리에 추가
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey;
			HTREEITEM hMid = TVI_LAST;
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWUD.CBS");
			
			SetItemData(InsertItem(_T("콜"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xELWASSET, 0, 1, 1, TD_MID));
			SetItemData(InsertItem(_T("풋"), 0, 1, hItem, TVI_LAST), MAKE_TREEID(xELWASSET, 0, 1, 2, TD_MID));

			HTREEITEM hChild = GetChildItem(hItem);
			CString child_name;
			while(hChild)
			{
				child_name = GetItemText(hChild);
				
				if(child_name == "콜")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						IH::Parser(string, string_code); 
						
						if(ii>0) //전체는 표시안함
						{
							kind = MAKE_TREEID(treeID->kind, 0, 12, ii, TD_ITEM);
							SetItemData(InsertItem(string, 0, 1, hChild, TVI_LAST), kind);
						}

					}					
				}
				else if(child_name == "풋")
				{
					for(int ii=0 ; ii<200 ; ii++)
					{
						skey.Format("%d", ii);
						readL = GetPrivateProfileString("Items", skey, "", readB, sizeof(readB), filePath);
						if(readL <= 0) break;
						
						string = CString(readB, readL);
						string.TrimRight();
						string_code = string.Left(12);
						IH::Parser(string, string_code); 
						
						if(ii>0) //전체는 표시안함
						{
							kind = MAKE_TREEID(treeID->kind, 0, 12, ii, TD_ITEM);
							SetItemData(InsertItem(string, 0, 1, hChild, TVI_LAST), kind);
						}
					}			
				}
				
				hChild = GetNextSiblingItem(hChild);
			}
		}
		break;

	case xINTEREST:
		{
			//struct _treeID* treeID = CAST_TREEID(gubn);
			//int readL = 0; 
			//char readB[1024]{};	//2015.06.16 KSJ 그룹읽어오는 갯수 늘림
			//CString	filePath, string, gno, gname;

			//filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);

			//readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), filePath);
			//if (readL <= 0)	return;

			//string = CString(readB, readL);

			//while (!string.IsEmpty())
			//{
			//	gno = IH::Parser(string, ";");
			//	if (gno.IsEmpty() || gno.GetLength() != 2)
			//		break;

			//	readL = GetPrivateProfileString(SEC_GROUPNAME, gno, "", readB, sizeof(readB), filePath);
			//	gname = CString(readB, readL);
			//	kind = MAKE_TREEID(treeID->kind, 0, 2, atoi(gno), TD_ITEM);
			//	if (//ExistFile(atoi(gno)))
			//	SetItemData(InsertItem(gname, 0, 1, hItem), kind);
			//}
		}
		break;
	case xREMAIN:
		{
			CStringArray* pArr = m_accn.get_account();
			
			for(int i=0 ; i<pArr->GetSize() ; i++)
			{
				CString strAccn = pArr->GetAt(i);

				CString strText = strAccn.Left(3) + "-" + strAccn.Mid(3,2) + "-" + strAccn.Right(6);
				CString strT = strAccn.Mid(3,1);
				
				kind = MAKE_TREEID(treeID->kind, 0, 2, i, TD_ITEM);

				if( strT != "5" && strT != "0" )
				{
					SetItemData(InsertItem(strText, 0, 1, hItem), kind);
				}
			}
		}
		break;
	case xKOSPIex:
		{
			for (int ii = 0; ii < cnt_exKospi; ii++)
			{
				kind = MAKE_TREEID(treeID->kind, 0, 3, atoi(exKospi[ii].code), TD_ITEM);
				CString sname = exKospi[ii].name;
				sname.Replace("KOSPI", "코스피");
				SetItemData(InsertItem(sname, 0, 1, hItem), kind);
			}
		}
		break;

	case xKOSDAQex:
		{
		//2016.05.16 KSJ KOSTAR 지수 삭제
		}
		break;

	case xKOSPIUP:

	case xKOSDAQUP:
		{
			CString name;
			struct	upcode	upcode;
			const int jgub = (treeID->kind == xKOSPIUP) ? upKOSPI : upKOSDAQ;
			if (m_upcode.GetSize() == 0) loadingUPcode();
			for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
			{
				upcode = m_upcode.GetAt(ii);
				if (upcode.jgub == jgub)
				{
					name = CString(upcode.hnam, UNameLen);
					name.TrimLeft(); name.TrimRight();
					kind = MAKE_TREEID(treeID->kind, 0, 2, (UINT)upcode.ucod, TD_ITEM);
					SetItemData(InsertItem(name, 0, 1, hItem), kind);
				}
			}
		}
		break;	

	case xPCALL:

	case xPPUT:
		{
			int	ii = 0, nCount = m_mapPJCODE.GetCount();
			CString	key = _T(""), value = _T("");
			if (nCount == 0)
			{
				loadingPJcode();
				nCount = m_mapPJCODE.GetCount();
			}
			for ( ii = 0 ; ii < nCount ; ii++ )
			{
				key = m_mapPJCODE.GetKey(ii);
				value = m_mapPJCODE.GetValue(ii);
				kind = MAKE_TREEID(treeID->kind, 0, 2, atoi(value), TD_ITEM);
				SetItemData(InsertItem(_T(key), 0, 1, hItem), kind);
			}
		}
		break;
	}
}

void CTreeWnd::loadingETFcode()
{
	CString ETFile;
	char* buf{};
	CString data, line;	
	CString gubn;
	
	ETFile.Format("%s/%s/efcode.dat", m_root, TABDIR);
	CStdioFile file(ETFile, CFile::modeRead|CFile::shareDenyNone);	
	while(file.ReadString(line))
	{
		gubn = line.Left(1);	
		buf = line.GetBuffer(line.GetLength());
		line.ReleaseBuffer();	//2011.12.06 KSJ
		if(strcmp(gubn, "1") == 0)
		{
			auto& efop_item = _ETFitem.emplace_back(std::make_unique<struct _efopitem>());
			_efopitem *efopitem = (struct _efopitem*)buf;
			CopyMemory(efop_item.get(), efopitem, sizeof(struct _efopitem));	
		}
		else if(strcmp(gubn, "2") == 0)
		{
			auto& efop_tema = _ETFtema.emplace_back(std::make_unique<struct _efoptema>());
			_efoptema *efoptema = (struct _efoptema*)buf;
			CopyMemory(efop_tema.get(), efoptema, sizeof(struct _efoptema));	
		}
		else if(strcmp(gubn, "3") == 0)
		{
			auto& efop_fore = _ETFfore.emplace_back(std::make_unique<struct _efopfore>());
			_efopfore *efopfore = (struct _efopfore*)buf;
			CopyMemory(efop_fore.get(), efopfore, sizeof(struct _efopfore));	
		}
		else if(strcmp(gubn, "4") == 0)
		{
			auto& efop_code = _ETFcode.emplace_back(std::make_unique<struct _efopcode>());
			_efopcode *efopcode = (struct _efopcode*)buf;
			CopyMemory(efop_code.get(), efopcode, sizeof(struct _efopcode));;	
		}
	
	}
	file.Close();
}


BOOL CTreeWnd::CheckStock(UINT kind, _shjcode* hjcode)
{
	BOOL	ret = FALSE;
	struct _treeID* treeID = CAST_TREEID(kind);
	const UINT	value = treeID->value;
	switch (treeID->kind)
	{
	case xTUSACONST:  //20240422 투자계약증권
	case xSINTAKST:  //20240422 신탁수익증권
	case xALLNEWST:  //20240422 신종증권 전체
	{
		//switch (hjcode->kosd)
		//{
			/*	case jmKOSPI:
				case jmKOSDAQ:
				case jm3RD:*/
		//default:
			if (treeID->kind == xALLNEWST)
			{
				if (hjcode->ssgb == jmTJCONJS || hjcode->ssgb == jmSINSUJS)
				{
					/*	if (hjcode.ssgb == jmTJCONJS || hjcode.ssgb == jmSINSUJS)
						{
							CString stmp;
							stmp.Format("\r\n [axiscode][%-30s]  ssgb = [%d]  kpgb=[%d] jchk=[%d] sCode=[%s]",
								__FUNCTION__, hjcode->ssgb, hjcode->kpgb, hjcode->jchk, hjcode->code);
							OutputDebugString(stmp);
						}*/
					ret = TRUE;
				}
			}
			else if (treeID->kind == xSINTAKST)
			{
				if (hjcode->ssgb == jmSINSUJS)
					ret = TRUE;
			}
			else if (treeID->kind == xTUSACONST)
			{
				if (hjcode->ssgb == jmTJCONJS)
					ret = TRUE;
			}
			//break;
		//}
	}
	break;
	case xRITS:  //20240422 리츠 추가 
	{
		switch (hjcode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode->ssgb)
			{
			case jmREITS:
				ret = TRUE;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
	break;
	case xALLCODE:
		switch (hjcode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode->ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				break;
			default:
				ret = TRUE;
			}
			break;
		default:
			break;
		}
		break;
	case xKOSPI:
		switch (hjcode->kosd)
		{
		case jmKOSPI:
			switch (hjcode->ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				break;
			default:
				ret = TRUE;
				break;
			}
			break;
		default:			
			break;
		}
		break;
	case xKOSDAQ:
		switch (hjcode->kosd)
		{
		case jmKOSDAQ:
			switch (hjcode->ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			case jmHYFND:
			case jmELW:
				break;
			default:
				ret = TRUE;
				break;
			}
			break;
		default:
			break;
		}
		break;
	case xKOPSI200:
		switch (hjcode->kosd)
		{
		case jmKOSPI:
			if (hjcode->kpgb >= 1)
				ret = TRUE;
			break;
		default:
			break;;
		}
		break;
	case xKOSDAQ50:
		switch (hjcode->kosd)
		{
		case jmKOSDAQ:
			if (hjcode->kpgb >= 1)
				ret = TRUE;
			break;
		default:
			break;
		}
		break;
	case xKRX100:
		if (hjcode->unio == 1)			
			ret = TRUE;
		break;
	case xFREECODE:
		if (hjcode->kosd == jm3RD)
			ret = TRUE;
		break;
	case xETFCODE:
		switch (hjcode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			ret = TRUE;
			if (hjcode->ssgb != jmETF)
				ret = FALSE;			
			break;
		default:
			break;
		}
		break;
	case xELWCODE:
		switch (hjcode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			ret = TRUE;
			if (hjcode->ssgb != jmELW)
				ret = FALSE;
			break;
		default:
			break;
		}
		break;
	case xSINJUCODE:
		switch (hjcode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (hjcode->ssgb)
			{
			case jmSINJU:
			case jmSINJS:
			//case jmHYFND:	//2015.03.30 KSJ 장내수익증권종목 신주인수권 리스트에서 삭제
				ret = TRUE;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
		break;
	case xKOSPIUP:
		{
			if (hjcode->kosd != jmKOSPI)
				break;

			switch (hjcode->ssgb)
			{
			case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
				break;
			default:
				if (value == 1)
				{
					ret = TRUE;
				}
				else if (value < 5)
				{
					if ((UINT)hjcode->size == value)
						ret = TRUE;
				}
				else if (value < 27)
				{
					const CString xxx = xxKOSPI[value];
					if (hjcode->ucds.CompareNoCase(xxx) == 0 || hjcode->ucdm.CompareNoCase(xxx) == 0)
						ret = TRUE;
				}
				else if (value == 27)
				{
					if ((UINT)hjcode->jjug == value)
						ret = TRUE;
				}
				else if (value < 41)
				{
					if ((UINT)hjcode->jsiz == value)
						ret = TRUE;
				}
				else if (value == 41)
				{
					if ((UINT)hjcode->jsiz >= value + 38 && (UINT)hjcode->jsiz <= value + 41)
						ret = TRUE;
				}
				else if (value == 42)
				{
					if ((UINT)hjcode->wsgb == 1)
						ret = TRUE;
				}
				break;
			}				
		}
		break;
	case xKOSDAQUP:
		{
			if (hjcode->kosd != jmKOSDAQ)
				break;

			switch (hjcode->ssgb)
			{
			case jmSINJU: case jmSINJS: case jmHYFND: case jmELW:
				break;
			default:
				if (value == 1)
				{
					ret = TRUE;
				}
				else if (value == 2)
				{
					if ((UINT)hjcode->jjug == value)
						ret = TRUE;
				}
				else if (value < 5)
				{
					if ((UINT)hjcode->size == value)
						ret = TRUE;
				}
				else if (value == 5)
				{
					if ((UINT)hjcode->size == 4 && (UINT)hjcode->jjug == 2)
						ret = TRUE;
				}
				else if (value < 38 || value == 41)
				{
					const CString xxx = xxKOSDAQ[value];
					if (hjcode->ucds.CompareNoCase(xxx) == 0 || hjcode->ucdm.CompareNoCase(xxx) == 0)
						ret = TRUE;
				}
				else if (value < 41)
				{
					if ((UINT)hjcode->jsiz == value - 37)
						ret = TRUE;
				}
				else if (value == 42)		//프리미어 지수 
				{
					if ((UINT)hjcode->prmr == value - 41)
						ret = TRUE;
				}
				else if (value > 42)
				{
					CString xxx = xxKOSDAQ[value];
					if (hjcode->acdl.CompareNoCase(xxx) == 0)
						ret = TRUE;
				}
				break;	
			}
		}
		break;
	case xKOSPIex:
		{
			if (hjcode->kosd != jmKOSPI)
				break;

			switch (value)
			{
			case 101: // KOSPI200
				if ((int)hjcode->kpgb == 3 || (int)hjcode->kpgb == 2 || (int)hjcode->kpgb == 1)
					ret = TRUE;
				break;
			case 401: // KOSPI100
				if ((int)hjcode->kpgb == 3 || (int)hjcode->kpgb == 2)
					ret = TRUE;
				break;
			case 402: // KOSPI50
				if ((int)hjcode->kpgb == 3)
					ret = TRUE;
				break;
			case 403: // KOSPI IT
				if ((int)hjcode->itgb == 1)
					ret = TRUE;
				break;
			}
		}
		break;
	case xKOSDAQex:
		{
			if (hjcode->kosd != jmKOSDAQ)
				break;

			switch (value)
			{
			case 301: // KOSDAQ50
				if ((int)hjcode->kpgb == 1)
					ret = TRUE;
				break;
			case 302: // KOSDAQ IT
				if ((int)hjcode->itgb == 1)
					ret = TRUE;
				break;
			case 303: // KOSTAR
				if ((int)hjcode->star == 1)
					ret = TRUE;
				break;				
			}
		}
		break;
	case xKONEX:	//2013.06.25 KSJ 코넥스 추가
		switch (hjcode->ssgb)
		{
		case jmKONEX:
			ret = TRUE;
			break;
		}
		break;
	case xETNCODE:	//2014.11.06 KSJ ETN 추가
		switch (hjcode->ssgb)
		{
		case jmETN:
			ret = TRUE;
			break;
		}
		break;
	case xSPACCODE:	//2015.11.03 KSJ spac 추가
		switch (hjcode->ssgb)
		{
		case jmSPAC:
			ret = TRUE;
			break;
		}
		break;
	case xGOODSTOCK:
		switch (hjcode->ssgb)
		{
		case jmGOODSTOCK:
			ret = TRUE;
			break;
		}
		break;
	case xNXT:
	{
		if (hjcode->nxtf == 'Y')
			ret = TRUE;
		break;
	}	
	break;
	default:
		break;
	}
	
	return ret;
}

BOOL CTreeWnd::CheckSearch(_shjcode* hjcode, int opt)
{
	if (opt & OPT_KOSDAQ)
	{
		if (hjcode->kosd == jmKOSDAQ)
			return FALSE;
	}

	if (opt & OPT_KOSPI)
	{
		if (hjcode->kosd == jmKOSPI)
			return FALSE;
	}

	if (hjcode->jchk & opt)
		return FALSE;

	return TRUE;
}

void CTreeWnd::GetStock(class CGridData& sdata, int max, int opt)
{
	_shjcode   hjcode; 
	CString code = _T(""), name = _T("");

// 	CString strTemp;
// 	int nCount = 0;
	
	if (m_hjcode.GetSize() == 0) loadingHJcode();
	
	for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
	{

		hjcode = m_hjcode.GetAt(ii);

		if (!CheckSearch(&hjcode, opt))
			continue;
		
		if (!CheckStock(sdata.GetKind(), &hjcode))
			continue;

		code = CString(hjcode.code, HCodeLen);
		sdata.m_arDatas.Add(MakePacket(code));
		if (sdata.GetCount() == max)
			break;
	}	
}

void CTreeWnd::GetFuture(class CGridData& sdata, int max)
{
	_sfjcode   fjcode;
	CString code = _T(""), name = _T("");
	
	if (m_fjcode.GetSize() == 0) loadingFJcode();

	for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
	{
		fjcode = m_fjcode.GetAt(ii);

		code = CString(fjcode.code, FCodeLen);
		sdata.m_arDatas.Add(MakePacket(code));
		if (sdata.GetCount() == max)
			break;
	}	
}

//2012.10.04 금리통화
/*
void CTreeWnd::GetCFuture(class CGridData& sdata, int max, bool bCurrency)
{
	ccode   ccode;
	CString code = _T(""), name = _T(""), tjgb = _T("");
	
	if(bCurrency)
	{
		if (m_cccode.GetSize() == 0) loadingCFcode(bCurrency);
		
		for (int ii = 0; ii < m_cccode.GetSize(); ii++)
		{
			ccode = m_cccode.GetAt(ii);
			
			code = CString(ccode.codx, FCodeLen);

			tjgb = CString(ccode.tjgb, 2);
			if(tjgb.GetAt(0) == '7')
				sdata.m_arDatas.Add(MakePacket(code));

			if (sdata.GetCount() == max)
				break;
		}
	}
	else
	{
		if (m_cicode.GetSize() == 0) loadingCFcode(bCurrency);
		
		for (int ii = 0; ii < m_cicode.GetSize(); ii++)
		{
			ccode = m_cicode.GetAt(ii);
			
			code = CString(ccode.codx, FCodeLen);
			
			tjgb = CString(ccode.tjgb, 2);
			if(tjgb.GetAt(0) == '6')
				sdata.m_arDatas.Add(MakePacket(code));

			if (sdata.GetCount() == max)
				break;
		}	
	}
}*/
// KSJ

//2015.02.24 KSJ 최근월물별로 정렬하도록 수정
void CTreeWnd::GetCFuture(class CGridData& sdata, int max, bool bCurrency)
{
	ccode   ccode;
	CString code = _T(""), name = _T(""), tjgb = _T("");

	int nSize = 3;
	int jj = 0;
	
	if(bCurrency)
	{
		nSize = 15;

		if (m_cccode.GetSize() == 0) loadingCFcode(bCurrency);

		for(;jj < nSize;)
		{
			for (int ii = 0; ii < m_cccode.GetSize()/nSize; ii++)
			{
				ccode = m_cccode.GetAt(jj + nSize*ii);
				
				code = CString(ccode.codx, FCodeLen);
				
				tjgb = CString(ccode.tjgb, 2);
				if(tjgb.GetAt(0) == '7')
					sdata.m_arDatas.Add(MakePacket(code));
				
				if (sdata.GetCount() == max)
					return;
			}
			jj++;
		}
	}
	else
	{
		if (m_cicode.GetSize() == 0) loadingCFcode(bCurrency);

		for(;jj < nSize;)
		{
			for (int ii = 0; ii < m_cicode.GetSize()/nSize; ii++)
			{
				ccode = m_cicode.GetAt(jj + nSize*ii);
				
				code = CString(ccode.codx, FCodeLen);
				
				tjgb = CString(ccode.tjgb, 2);
				if(tjgb.GetAt(0) == '6')
					sdata.m_arDatas.Add(MakePacket(code));
				
				if (sdata.GetCount() == max)
					return;
			}	
			jj++;
		}
	}
}

//2012.10.04 주식선물
/*
void CTreeWnd::GetSFuture(class CGridData& sdata, int max)
{
	sfcode   sfcode;
	CString code = _T(""), name = _T("");
	
	if (m_sfcode.GetSize() == 0) loadingSFcode();
	
	for (int ii = 0; ii < m_sfcode.GetSize(); ii++)
	{
		sfcode = m_sfcode.GetAt(ii);
		
		code = CString(sfcode.codx, FCodeLen);
		sdata.m_arDatas.Add(MakePacket(code));
		if (sdata.GetCount() == max)
			break;
	}	
}
*/
// KSJ

//2015.02.24 KSJ 최근월물별로 정렬하도록 수정
void CTreeWnd::GetSFuture(class CGridData& sdata, int max)
{
	sfcode   sfcode;
	CString code = _T(""), name = _T("");
	
	if (m_sfcode.GetSize() == 0) loadingSFcode();
	
	int jj = 0;
	constexpr int nSize = 9;	// 코드에 스프레드는 빠져있다..	2016.07.08 주식선물 월물 늘어남에 따라 변경

	for(;jj < nSize;)
	{
		for (int ii = 0; ii < m_sfcode.GetSize()/nSize; ii++)
		{
			sfcode = m_sfcode.GetAt(jj + nSize*ii);
			
			code = CString(sfcode.codx, FCodeLen);
			sdata.m_arDatas.Add(MakePacket(code));
			if (sdata.GetCount() == max)	return;
		}	
		jj++;
	}
}


//2015.02.24 KSJ 최근월물별로 정렬하도록 수정
void CTreeWnd::GetOption(class CGridData& sdata, int max)
{
	struct ojcode	ojcode;
	CString code = _T(""), name = _T("");
	if (m_ojcode.GetSize() == 0) loadingOJcode();
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());

	bool bCall = false;

	if (treeID.kind == xCALLCODE)
	{
		bCall = true;
	}


	int jj = 0;

	if(bCall)
	{
		jj = STANDARDNUM -1;
	}

	//m_ojcode 행사가 갯수

	for( ;jj >= 0 && jj < STANDARDNUM; )
	{
		for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
		{
			ojcode = m_ojcode.GetAt(ii);
			
			if (treeID.kind == xCALLCODE)
			{
				if (ojcode.call[jj].yorn != '1')
					continue;
				
				code = CString(ojcode.call[jj].cod2, OCodeLen -1 );				
			}
			else
			{
				if (ojcode.put[jj].yorn != '1')
					continue;
				
				code = CString(ojcode.put[jj].cod2, OCodeLen -1 );			
			}
			
			sdata.m_arDatas.Add(MakePacket(code));
			if (sdata.GetCount() == max)		return;
		}
		
		if(bCall)
		{
			jj--;

		}
		else
		{
			jj++;
		}
	}
}

void CTreeWnd::GetJisu(class CGridData& sdata, int max)
{
	struct upcode	upcode;
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());
	
	const BOOL	bKospi = (treeID.kind == xKOSPIJISU) ? TRUE : FALSE;
	CString code = _T(""), name = _T("");
	const int	ncnt = (bKospi) ? cnt_exKospi : cnt_exKosdaq;
	if (m_upcode.GetSize() == 0) loadingUPcode();

	for (int ii = 0; ii < m_upcode.GetSize(); ii++)
	{
		upcode = m_upcode.GetAt(ii);

		if (bKospi)
		{
			if (upcode.jgub != upKOSPI)
				continue;
		}
		else
		{
			if (upcode.jgub != upKOSDAQ)
				continue;
		}

		code.Format("%01d%02d", (int)upcode.jgub, (int)upcode.ucod);
		sdata.m_arDatas.Add(MakePacket(code));
		if (sdata.GetCount() == max)
			break;
	}

	if (sdata.GetCount() != max)
	{

		for (int ii = 0; ii < ncnt ; ii++)
		{
			if (bKospi)
			{
				code = exKospi[ii].code;
			}

			sdata.m_arDatas.Add(MakePacket(code));

			if (sdata.GetCount() == max)
				break;
		}
	}
}

void CTreeWnd::GetPjcode(class CGridData& sdata, int max)
{
	struct	pjcode pjcode;
	
	CString code = _T(""), name = _T(""), tjgb = _T(""), gubn = _T("3"), seltjgb = _T("");
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());
	
	seltjgb.Format("%0*d", treeID.degree, treeID.value);

	if (treeID.kind == xPCALL)
		gubn = '2';

	if (m_pjcode.GetSize() == 0) loadingPJcode();
	for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
	{
		pjcode = m_pjcode.GetAt(ii);

		code = CString(pjcode.codx, PCodeLen);
		tjgb = CString(pjcode.tjgb, 2);

		if (code.GetAt(0) == gubn && tjgb == seltjgb)
		{
			sdata.m_arDatas.Add(MakePacket(code));
			if (sdata.GetCount() == max)
				break;
		}
	}		
}

void CTreeWnd::GetThema(class CGridData& sdata, int max)
{
	struct _treeID treeID = CONVERT_TREEID(sdata.GetKind());
	CString	skey; 
	skey.Format("%0*d", treeID.degree, treeID.value);

	char	readB[512];
	UINT	readL;
	CString	filePath, code, name, string = _T("");
	filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");

	readL = GetPrivateProfileString(_T("TMCODE"), skey, "", readB, sizeof(readB), filePath);
	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = IH::Parser(string, ";");
		sdata.m_arDatas.Add(MakePacket(code));
		if (sdata.GetCount() == max)
			break;
	}	
}

void CTreeWnd::GetELWBase(class CGridData& sdata, int max)
{
	_sfjcode   sfjcode; 
	CString code = _T(""), name = _T("");
	 
	for (int ii = 0; ii < m_elwbase.GetSize(); ii++)
	{
		sfjcode = m_elwbase.GetAt(ii);

		sdata.m_arDatas.Add(sfjcode.code);
		if (sdata.GetCount() == max)
			break;
	}	
}

CString CTreeWnd::GetCodeName(CString code)
{
	const int codeL = code.GetLength();

	if (codeL == 6)					// kospi code
	{
		_shjcode   hjcode; 

		if (m_hjcode.GetSize() == 0) loadingHJcode();

		for (int ii = 0; ii < m_hjcode.GetSize(); ii++)
		{
			hjcode = m_hjcode.GetAt(ii);
			if (code != hjcode.code)
				continue;

			return hjcode.name;
		}

	}
	else if (codeL <= 3)				// upjong code
	{
		struct	upcode	upcode;
		if (m_upcode.GetSize() == 0) loadingUPcode();
		for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
		{
			upcode = m_upcode.GetAt(ii);
			if (atoi(code) != upcode.ucod)
				continue;

			return CString(upcode.hnam, UNameLen);
		}
	}
	else if (code[0] == '1' || code[0] == '4' || code[0] == 'A' || code[0] == 'D')	// future code //파생상품 코드개편
	{
		_sfjcode   fjcode;
		if (m_fjcode.GetSize() == 0) loadingFJcode();
		for (int ii = 0; ii < m_fjcode.GetSize(); ii++)
		{
			fjcode = m_fjcode.GetAt(ii);
			if (code != fjcode.code)
				continue;

			return fjcode.name;
		}

	}
	else if (code[0] == '2' || code[0] == '3' || code[0] == 'B' || code[0] == 'C') //파생상품 코드개편
	{
		if (code[1] == '0')			// future option code
		{
			struct ojcode	ojcode;
			if (m_ojcode.GetSize() == 0) loadingOJcode();
			for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
			{
				ojcode = m_ojcode.GetAt(ii);

				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.call[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.call[jj].cod2, OCodeLen))
						continue;

					return CString(ojcode.call[jj].hnam, ONameLen);
				}
				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.put[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.put[jj].cod2, OCodeLen))
						continue;

					return CString(ojcode.put[jj].hnam, ONameLen);
				}
			}
		}
		else					// 
		{
			struct	pjcode pjcode;
			if (m_pjcode.GetSize() == 0) loadingPJcode();
			for (int ii = 0; ii < m_pjcode.GetSize(); ii++)
			{
				pjcode = m_pjcode.GetAt(ii);
				if (code != CString(pjcode.codx, PCodeLen))
					continue;

				return CString(pjcode.hnam, sizeof(pjcode.hnam));
			}
		}
	}

	return _T("");
}

void CTreeWnd::sendUpjong()
{
	class CGridData sdata;

	CString upjn;

	upjn = "001";
	sdata.m_arDatas.Add(MakePacket(upjn, "", "", "", ""));

	upjn = "201";
	sdata.m_arDatas.Add(MakePacket(upjn, "", "", "", ""));
	
	m_pMainWnd->SendMessage(WM_MANAGE, MK_UPJONGDATA, (LPARAM)&sdata);
}

BOOL CTreeWnd::SendOper(UINT kind, int opt)
{
	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	pWnd->SendMessage(WM_MANAGE, MK_CLEARSECONDSEND, 0);
	const int max = pWnd->SendMessage(WM_MANAGE, MK_GETMAX);

	class CGridData	sdata;
	sdata.SetKind(kind);
	const bool ret = GetData(sdata, max, opt);
	if (ret)
		SendTreeData(sdata);

	return TRUE;
}


void  _intersx::copy(void* item)
{
	const auto tt = (struct _jinfo*)item;
	gubn = tt->gubn[0];
	code = CString(tt->code, 12).Trim();
	xprc = CString(tt->xprc, 10).Trim();
	xnum = CString(tt->xnum, 10).Trim();
}

void CTreeWnd::receiveOub(CString& data, int keys)
{
	CString strtmp;
	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	pWnd->SendMessage(WM_MANAGE, MK_CLEARSECONDSEND, 0);
	const int max = pWnd->SendMessage(WM_MANAGE, MK_GETMAX);

//XMSG("종목그룹상세");
	_vInters.clear();
	const CString skey       = data.Left(2);
	const CString sGroupName = data.Mid(2, 20);
	const CString sCount     = data.Mid(22, 4);
	const int     nRow 	 = min(atoi(sCount), 100);
	data = data.Mid(26);

//AxStd::_Msg("그룹키   [%s]", skey);
//AxStd::_Msg("그룹명   [%s]", sGroupName);
//AxStd::_Msg("종목개수 [%s]", sCount);

	const int index = atoi(skey) - 1;	
	if (index < 0)
		return;
			
	auto& sdata = _arrInter.at(index);
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());

	CString	code, amount, price, name, bookmark, stmp, bookrgb, MemoYN;
	const gsl::span<struct _jinfo_MEMO> codelist((struct _jinfo_MEMO*)data.GetString(), nRow);


	int ii = 0;
	for_each(codelist.begin(), codelist.end(), [&](auto item) {
		code = CString(item.code, codelen).Trim();
		name = GetCodeName(CString(item.code, codelen).Trim());
		
		//test mod 서버에서 받은 북마크
		if (CString(item.gubn, 1).Trim() == "9")
			bookmark = "1";
	
		if (code.IsEmpty())
			code = "          ";

		
		if (code[0] == 'm') 
		{	//서버에 내려받는 종목코드 정보가 책갈피일때
			//struct _jinfo_MEMO st_jinfo;
			name = CString(item.xprc, 10).Trim();
			if (name.Find("0  ") == 0)
				name.Empty();
		
			amount.Empty();
			price.Empty();
		}
		else
		{
			amount = CString(item.xnum, xnumlen).Trim();
			price = CString(item.xprc, pricelen).Trim();
		}
	
		bookrgb = CString(item.bookrgb, 2).Trim();
		MemoYN  = CString(item.mflg, 1).Trim();

		CString sCode = "A" + code;	
		if (MemoYN.CompareNoCase("Y") == 0)
			_memo[sCode.GetString()] = 1;

		sdata.m_arDatas.Add(MakePacket(code, amount, price, name, bookmark, "", "", "", "", bookrgb, MemoYN));
		if (sdata.GetCount() == max)
			return;		
		ii++;
	});
	SendTreeData(sdata);
}


void CTreeWnd::GetInterest(class CGridData& sdata, int max)
{
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());	
	if (IS_WITHOUTx(0, _arrInter.size(), treeID.value))
		return;

	auto& item = _arrInter.at(treeID.value - 1);
	item = sdata;	
	((CMainWnd*)m_pMainWnd)->Request_GroupCode(treeID.value);
	return;	
}

void CTreeWnd::GetGroup(class CGridData& sdata, int max)
{
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());
	CString	skey; skey.Format("%0*d", treeID.degree, treeID.value);

	char	readB[512];
	UINT	readL;
	CString	filePath, code, name, string = _T("");
	
	filePath.Format("%s/%s/%s", m_root, TABDIR, "gropcd.ini");
	readL = GetPrivateProfileString(_T("GRCODE"), skey, "", readB, sizeof(readB), filePath);

	if (readL > 0) string = CString(readB, readL);

	while (!string.IsEmpty())
	{
		code = IH::Parser(string, ";");
		sdata.m_arDatas.Add(MakePacket(code));
		if (sdata.GetCount() == max)
			break;
	}
}

void CTreeWnd::SendTreeData(const class CGridData& sdata)
{
	m_pMainWnd->SendMessage(WM_MANAGE, MK_TREEDATA, (LPARAM)&sdata);	//3
}

BOOL CTreeWnd::SetItemData(HTREEITEM hItem, DWORD dwData)
{	
	m_mapParam.SetAt(dwData, hItem);	
	return CTreeCtrl::SetItemData(hItem, dwData);
}

BOOL CTreeWnd::DeleteItem(HTREEITEM hItem)
{
	constexpr DWORD	dwData = 0;
	m_mapParam.RemoveKey(dwData);
	return CTreeCtrl::DeleteItem(hItem);
}

BOOL CTreeWnd::DeleteAllItems()
{
	m_mapParam.RemoveAll();
	return CTreeCtrl::DeleteAllItems();
}

CString CTreeWnd::MakePacket(CString& code, CString amount, CString price, CString name, CString bookmark, CString futureGubn, CString creditPrc, CString maeipPrc, CString date, CString bookrgb, CString MemoYN)
{
	code.Trim();
	if (code.IsEmpty())
		return _T("");

	// MODIFY PSH 20070914
	/*
	if (!amount.IsEmpty())
		code += ("\t" + amount);
	*/
	code += "\t";
	if (!amount.IsEmpty())
		code += amount;
	// END MODIFY

	// MODIFY PSH 20070914
	/*ㅍ
	if (!price.IsEmpty())
		code += ("\t" + price);
	*/
	code += "\t";
	if (!price.IsEmpty())
		code += price;
	// END MODIFY

	// ADD PSH 20070914
	code += "\t";
	if (!name.IsEmpty())
		code += name;

	// 북마크 추가
	code += "\t";
	if (!bookmark.IsEmpty())
		code += bookmark[0] == '1' ? "1":"0";	//2015.12.21 KSJ 1이면 1을 넣어준다.

	// 선물/옵션 잔고 조회시 구분값 추가
	code += "\t";
	if (!futureGubn.IsEmpty())
		code += futureGubn;

	code += "\t";
	if (!creditPrc.IsEmpty())
		code += creditPrc;

	code += "\t";
	if (!maeipPrc.IsEmpty())
		code += maeipPrc;
	// END ADD

	//2016.04.15 KSJ epb추천일자 추가
	code += "\t";
	if (!date.IsEmpty())
		code += date;

	code += "\t";   //test IO
	if (!bookrgb.IsEmpty())
		code += bookrgb;

	code += "\t";
	if (!MemoYN.IsEmpty())
		code += MemoYN;
	return code;
}


CString CTreeWnd::GetDataTitle(DWORD dwData)
{
	HTREEITEM	hItem = nullptr;
	HTREEITEM	hParent = nullptr, hPParent = nullptr;
	m_mapParam.Lookup(dwData, hItem);
	CString	szMe = GetItemText(hItem);
	CString	szParent = _T(""), szPParent = _T(""), szRET = _T("");

	hParent = GetParentItem(hItem);
	if (hParent)
	{	
		szParent = GetItemText(hParent);
		hPParent = GetParentItem(hParent);
		
		if (hPParent)
		{
			szPParent = GetItemText(hPParent);
		}
	}

	if (!szPParent.IsEmpty())
		szRET += szPParent;

	if (!szParent.IsEmpty())
	{
		if (!szRET.IsEmpty())
			szRET = szRET + " - ";
		szRET += szParent;
	}

	if (!szMe.IsEmpty())
	{
		if (!szRET.IsEmpty())
		{
			szRET = "[" + szRET + "] - ";
		}
		szRET += (szMe + " ");	
	}
	return szRET;
}

void CTreeWnd::GetRemain(class CGridData& sdata, int max)
{
	CString	szRET = "good";
	if (!szRET.IsEmpty())
	{
		sdata.m_arDatas.Add("good");
	}
}

void CTreeWnd::DataReloadELWAll(class CGridData& sdata,CString str,CString type)
{
	_elwdata elwdata;
	_elwdata elw;
	CString code, name, ktype, pcode, pname, mjmt;
	int bkoba{};
//	int row = 0;
	CPtrArray arCode;
	CPtrArray gbCode;
	
	if(str.Compare("일반ELW") == 0)
		bkoba = 0;
	else
		bkoba = 1;
	
	const int ncnt = m_ELWdata.GetSize();
	for(int ii = 0; ii < ncnt; ii++)
	{
		elwdata = m_ELWdata.GetAt(ii);
		
		code = CString(elwdata.code, 12);
		name = CString(elwdata.name, ELWHNameLen);
		
		code.Trim();
		name.Trim();
		
		if(bkoba == elwdata.bkoba && elwdata.ktype == type)
		{
			sdata.m_arDatas.Add(MakePacket(code));
			if (sdata.GetCount() == 100)
					break;
		}	
	}
}

void CTreeWnd::DataGubn(class CGridData& sdata,HTREEITEM hItem)
{
	CString		gubn;
	CString		data;
		
	HTREEITEM hParentItem;
	
	hParentItem = GetParentItem(hItem);

	gubn = GetItemText(hParentItem);
	data = GetItemText(hItem);

	if(gubn.Compare("운용사별") == 0)
	{
		DataReloadItem(sdata,data);
	}
	else if(gubn.Compare("기초자산별") == 0)	//2015.11.04 KSJ 테마별 --> 기초자산별로 변경
	{
		DataReloadTema(sdata,data);
	}
	else if(gubn.Compare("국내/외") == 0)
	{
		DataReloadFore(sdata,data);
	}
	else if(gubn.Compare("콜") == 0)
	{
		DataReloadELWAll(sdata,data,"01");
	}
	else if(gubn.Compare("풋") == 0)
	{
		DataReloadELWAll(sdata,data,"02");
	}

//	m_search = TRUE;

 }

void CTreeWnd::DataReloadItem(class CGridData& sdata,CString data)
{
	_efopitem* efopitem{};

	CString type, opcd, hnam;
	CString tmcd, frcd, frnm, code, etfm, etfl;
	
	auto ft = std::find_if(_ETFitem.begin(), _ETFitem.end(), [&](auto& efopitem){
		if (CString(efopitem->hnam, sizeof(efopitem->hnam)).Trim().CompareNoCase(data) == 0)
			return true;	
		return false;
	});
	if (ft != _ETFitem.end())
	{
		data = CString((*ft)->opcd, sizeof((*ft)->opcd)).Trim();
		for(const auto& efopcode : _ETFcode)
		{
			code = CString(efopcode->code, sizeof(efopcode->code)).Trim();		
			opcd = CString(efopcode->opcd, sizeof(efopcode->opcd)).Trim();	
			if(data.Compare(opcd) == 0)
			{
				sdata.m_arDatas.Add(MakePacket(code));
				if (sdata.GetCount() == 100)
					break;
			}
		}
	}
}

void CTreeWnd::DataReloadTema(class CGridData& sdata,CString data)
{	
	CString code, etfm;
	auto ft = std::find_if(_ETFtema.begin(), _ETFtema.end(), [&](auto& efoptema){
		if (CString(efoptema->tmnm, sizeof(efoptema->tmnm)).Trim().CompareNoCase(data) == 0)
			return true;	
		return false;
	});

	if (ft != _ETFtema.end())
	{
		data = CString((*ft)->tmcd, sizeof((*ft)->tmcd)).Trim();
		for(auto& efopcode : _ETFcode)
		{
			code = CString(efopcode->code, sizeof(efopcode->code)).Trim();		
			etfm = CString(efopcode->etfm, sizeof(efopcode->etfm)).Trim();	
			if(data.Compare(etfm) == 0)
			{
				sdata.m_arDatas.Add(MakePacket(code));
				if (sdata.GetCount() == 100)
					break;
			}
		}
	}
}

void CTreeWnd::DataReloadFore(class CGridData& sdata, CString data)
{
	CString code, etfl;		
	auto ft = std::find_if(_ETFfore.begin(), _ETFfore.end(), [&](auto& efopfore){
		if (CString(efopfore->frnm, sizeof(efopfore->frnm)).Trim().CompareNoCase(data) == 0)
			return true;	
		return false;
	});
	
	if (ft != _ETFfore.end())
	{
		data = CString((*ft)->frcd, sizeof((*ft)->frcd)).Trim();
		for (auto& efopcode : _ETFcode)
		{
			code = CString(efopcode->code, sizeof(efopcode->code)).Trim();
			etfl = CString(efopcode->etfl, sizeof(efopcode->etfl)).Trim();
			if (data.Compare(etfl) == 0)
			{
				sdata.m_arDatas.Add(MakePacket(code));
				if (sdata.GetCount() == 100)
					break;
			}
		}
	}
}


bool CTreeWnd::GetData(class CGridData& sdata, int max, int opt)
{
	struct _treeID treeID = CONVERT_TREEID(sdata.GetKind());
	switch (treeID.kind)
	{
	case xETFCOMPANY:
	case xETFTHEMA:
	case xETFDOMINTER:
	case xELWGEN:
	case xELWEARLY:
		DataGubn(sdata,m_selItem);
		break;
	case xALLCODE:			// all_coode
	case xKOSPI:
	case xKOSDAQ:
	case xKOPSI200:
	case xKOSDAQ50:
	case xKRX100:
	case xFREECODE:
	case xETFCODE:
	case xELWCODE:
	case xSINJUCODE:
	case xKONEX:		//2013.06.25 KSJ 코넥스 추가
	case xETNCODE:		//2014.11.06 KSJ ETN 추가
	case xSPACCODE:		//2015.11.03 KSJ spac 추가
	case xRITS: //20240422 리츠
	case xALLNEWST: //20240422 신종증권
	case xGOODSTOCK:
	case xNXT:
		{
			if(treeID.kind == xSPACCODE)	opt = 0;	//2015.11.20 KSJ 스팩일때는 옵션을 0으로 
				GetStock(sdata, max, opt);
		}
		break;	
	case xELWBASE:
		GetELWBase(sdata, max);
		break;
	case xKOSPIFUTURE:
		GetFuture(sdata, max);		
		break;
	case xSTOCKFUTURE:
		GetSFuture(sdata, max);		
		break;
	case xINTERESTRATEFUTURE:
		GetCFuture(sdata, max, false);		
		break;
	case xCURRENCYFUTURE:
		GetCFuture(sdata, max, true);		
		break;
	case xFUTURE:
		GetFuture(sdata, max);		
		break;
	case xCALLCODE:
	case xPUTCODE:
		GetOption(sdata, max);
		break;		
	case xKOSPIJISU:
	case xKOSDAQJISU:
		GetJisu(sdata, max);
		break;	
	case xREMAIN:
		GetRemain(sdata, max);
		break;
	case xISSUE:
		sdata.m_arDatas.Add("news");
		break;	
	case xELWASSET:
//		sdata.m_arDatas.Add();
		break;
	case xPCALL:
	case xPPUT:
		GetPjcode(sdata, max);
		break;
	case xKOSPIUP:
	case xKOSDAQUP:
		GetStock(sdata, max, opt);
		break;
	case xINTEREST:
		GetInterest(sdata, max);
		return false;
		break;
	case xKOSPIex:
	case xKOSDAQex:
		GetStock(sdata, max, opt);
		break;		
	case xTHEMACODE:
		GetThema(sdata, max);
		break;
	case xGROUPCODE:
		GetGroup(sdata, max);
		break;
	default:
		break;
	}
	return true;
}

LRESULT CTreeWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

void CTreeWnd::queryGroup()
{
	struct _gmid gmid {};
	gmid.xflg = _T('1');
	sprintf(gmid.usrid, _T("%-12s"), m_id.GetString());
	sprintf(gmid.grec, _T("0000"));

	gmid.sflag = _T('1');
	CString strData((char*)&gmid, L_gmid);
	CSendData sData;
	sData.SetData("PIDOSETA", TRKEY_GROUPACCNLIST, (LPSTR)(LPCTSTR)strData, strData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}

CString CTreeWnd::getLedger(CString strTR, char chType, CString strSvcn, CString strLastKey /* = _T("") */)
{
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	switch (chType)
	{
	case 'I':
		chType = '2';	break;
	case 'U':
		chType = '3';	break;
	case 'D':
		chType = '4';	break;
	case 'Q':
	default:
		chType = '1';
	}

	CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	if (!strSvcn.IsEmpty())
		CopyMemory(&ledger.svcd, (LPCTSTR)strSvcn, sizeof(ledger.svcd));

	if (!strLastKey.IsEmpty())
		CopyMemory(&ledger.next, strLastKey, strLastKey.GetLength());

	return CString((char*)&ledger, L_ledgerH);

	return _T("");
}



void CTreeWnd::queryAcc()
{

	if (m_id.IsEmpty() || m_pass.IsEmpty())
			return;

	queryGroup();

}

void CTreeWnd::queryStaffAcc(CString ledgerS)
{
	CString sData = ledgerS;

	CString passX = "TRUE";
	
	if (m_gubn == 'D')
		sData += m_id + "\t";
	else
		sData += m_sDeptCode + "\t" + m_id + "\t\t" + passX + "\t";

	CSendData	sdata;
	sdata.SetData("PIBOTUXQ", TRKEY_ACCSTAFF, (char*)sData.operator LPCTSTR(), sData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);	
}


CString CTreeWnd::GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	return sRtn;
}


void CTreeWnd::oubGroup(CRecvData* data)
{
	char*	buf = data->data;
	_gmod *gmod = (_gmod *)buf;
	
	if (gmod->okgb != 'Y')
		return;
	int grec = atoi(CString(gmod->grec, sizeof(gmod->grec)));
	if (grec == 0)
	{
//		EnableWindow(TRUE);
		Invalidate(FALSE);
		return;
	}
	
	CString tmpS, accnNUM, accnNAME;
	buf += L_gmod;
	HTREEITEM	hRoot = nullptr, hMid = TVI_FIRST, hItem = nullptr;
	m_mapParam.Lookup(MAKE_TREEID(xREMAIN), hRoot);
	CString	sztmp;
	for (int ii = 0; ii < grec; ii++)
	{
		_group *group = (_group *)buf;

		sztmp.Format("%.*s", sizeof(group->gnam), group->gnam);
		sztmp.TrimRight();
		hMid = InsertItem(sztmp, hRoot, hMid);
		SetItemData(hMid, MAKE_TREEID(xREMAIN, 0, 0, ii, TD_MID)); 		
		buf += L_group;

		sztmp.Format("%.*s", sizeof(group->nrec), group->nrec);
		sztmp.TrimLeft(), sztmp.TrimRight();
		const int	acCnt = atoi(sztmp);		

		_sfjcode	acInfo;
		CQArray <_sfjcode, _sfjcode>	arAccn;
		arAccn.RemoveAll();
		for (int jj = 0; jj < acCnt; jj++)
		{
			_accn *accn = (_accn *)buf;			
			acInfo.code = GetString(accn->accn, sizeof(accn->accn));
			acInfo.name = GetString(accn->acnm, sizeof(accn->acnm));
			buf += L_accn;

			//종합계좌는 추가 안하기
			CString check_ac = acInfo.code.Mid(3,2);
			
			if(check_ac != "00")
			{
				arAccn.Add(acInfo);
			}
			
		}
		arAccn.QuickSort();

		for (int jj = 0; jj < arAccn.GetSize(); jj++)
		{
			acInfo = arAccn.GetAt(jj);
			sztmp = makeAccn(acInfo.code, acInfo.name);
			hItem = InsertItem(sztmp, hMid);
			SetItemData(hItem, MAKE_TREEID(xREMAIN, ii + 1, 0, jj, TD_ITEM));			
		}
	}

//	EnableWindow(TRUE);
	Invalidate(FALSE);
}



CString CTreeWnd::Variant(int comm, CString param /* = _T("" */)
{
	CString	ret;
	ret.Format("%s", (char*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_VARIANT, comm), (LPARAM)(char*)param.operator LPCTSTR()));
	return ret;
}

void CTreeWnd::RecvOper(int kind, CRecvData* data)
{
	switch (kind)
	{
	//
	case TRKEY_GROUPACCNLIST:
		oubGroup(data);
		break;
	case TRKEY_ACCCUSTOM:
		oubAccUser(data->data);
		break;
	case TRKEY_ACCSTAFF:
		oubAccList(data->data, data->size);
		break;
	case TRKEY_REMAIN:
		oubRemain(data->data, data->size);
		break;
	case TRKEY_NEWS:
		parsingNews(data);
		break;
	case TRKEY_ELWISSUESEC:
		setELWIssueSec(data);
		break;
	case TRKEY_ELWBASEASSET:
		setELWBaseAsset(data);
		break;
	case TRKEY_RECOMMAND:
		parsingRecommand(data);
		break;
	}

}

void CTreeWnd::oubAccUser(char* buf)
{
	struct _acSet* acSet  = (struct _acSet*) buf;

	// guide출력
/*	CString	msg = CString(acSet->emsg, sizeof(acSet->emsg));
	msg.TrimRight();	
	m_result.SetWindowText(msg);	*/

	int nRec = atoi(CString(acSet->nrec, sizeof(acSet->nrec)));

	CString accNum, accName, sztmp;
	buf += L_acSet;
	HTREEITEM	hRoot = nullptr, hItem = nullptr;
	m_mapParam.Lookup(xREMAIN, hRoot);
	for (int ii = 0; ii < nRec ; ii++)
	{
		_acInfo *acInfo = (_acInfo *)buf;
		buf += L_acInfo;

		accNum = GetString(acInfo->accn, sizeof(acInfo->accn));
		accName = GetString(acInfo->acnm, sizeof(acInfo->acnm));
		
		if (!IsStockAcc(accNum) || accName.IsEmpty() || accNum.IsEmpty())
			continue;
		
		CString	str = makeAccn(accNum, accName);
		hItem = InsertItem(str, hRoot, TVI_SORT);
		SetItemData(hItem, MAKE_TREEID(xREMAIN, 0, 1, ii, TD_ITEM));		
	}	

	queryGroup();	
}

void CTreeWnd::oubAccList(char* buf, int len)
{
/*@@Zeta
	if (strncmp(LEDGER_ECODE(buf), "000000", 6))
		return;

	bool bNext = false;
	CString sData = _T("");
	if (LEDGER_CONF(buf) == '1')
	{
		sData = CString(buf, L_ledgerH);
		bNext = true;
	}
	
	buf += L_ledgerH;

	oubAccListEx(buf, len-L_ledgerH);
	if (bNext)			// 다음 데이타 조회
	{
		queryStaffAcc(sData);
		return;
	}
*/
	queryGroup();

}

void CTreeWnd::oubAccListEx(char* buf, int len)
{
	CString datS = CString(buf, len);
	
	CString		infoS;
	HTREEITEM	hRoot = nullptr, hItem = nullptr;
	m_mapParam.Lookup(xREMAIN, hRoot);

	_sfjcode	acInfo;
	CQArray <_sfjcode, _sfjcode>	arAccn;
	arAccn.RemoveAll();
	while (!datS.IsEmpty())
	{
		infoS = IH::Parser(datS, "\n");
		if (infoS.IsEmpty())
			break;

		if (m_gubn == 'D')
		{
			acInfo.code = IH::Parser(infoS, "\t");			// 계좌번호
			acInfo.name = IH::Parser(infoS, "\t");			// 고객명
		}
		else
		{
			IH::Parser(infoS, "\t");				// 관리자
			acInfo.code = IH::Parser(infoS, "\t");			// 계좌번호
			acInfo.name = IH::Parser(infoS, "\t");			// 고객명
		}
		

		if (!IsStockAcc(acInfo.code) || acInfo.name.IsEmpty() || acInfo.code.IsEmpty())
			continue;

		arAccn.Add(acInfo);
	}
}

void CTreeWnd::queryNews(int index)
{
	struct  mid {
	    char    func[1];        /* '1':최근100종목      */
	    char    date[8];        /* 날짜                 */
	} mid;

	memset(&mid, ' ', sizeof(mid));
	mid.func[0] = '1';
	CString senddata = CString((char*)&mid, sizeof(mid));
	CSendData	sdata;


	char	key{};
	_trkey* trkey = (struct _trkey*)&key;
	
	if(index > -1)
	{
		trkey->kind = TRKEY_NEWS;
		trkey->group = index;
		
		sdata.SetData("PIBO2022", key, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");	//2012.07.17 KSJ pibo --> pibf 2013.08.05 원복
	}
	else
	{
		sdata.SetData("PIBO2022", TRKEY_NEWS, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");	//2012.07.17 KSJ pibo --> pibf 2013.08.05 원복
	}
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
}

//081219 :: 추가모듈 
void CTreeWnd::setELWIssueSec(CRecvData* data)
{
	struct  grid 
	{
		char    seqn[4];
		char    cod2[7];            /* RTS COD      */
		char    code[6];            /* 종목코드     */
		char    hnam[20];           /* 종목명       */
		char    curr[8];            /* 현재가       */
		char    diff[7];            /* 대비         */
		char    rate[6];            /* 등락율       */
		char    mdga[8];            /* 매도호가     */
		char    msga[8];            /* 매수호가     */
		char    cvol[9];            /* 체결량       */
		char    gvol[9];            /* 거래량       */
		char    gamt[12];           /* 거래대금     */
		char    dvol[8];            /* 순매도잔량   */
		char    svol[8];            /* 순매수잔량   */
		char    irga[7];            /* 이론가       */
		char    grat[7];            /* 괴리율       */
		char    prty[7];            /* 패리티       */
		char    gerr[7];            /* 기어링비율   */
		char    brer[7];            /* 손익분기율   */
		char    cfpv[7];            /* 자본지지점   */
		char    gcjs[18];           /* 기초자산     */
		char    bhsa[20];           /* 발행사       */
		char    jjis[6];            /* 잔존일수     */
		char    hsgg[9];            /* 환산가       */
		char    hsga[9];            /* 행사가격     */
		char    jhre[9];            /* 전환비율     */
		char    delt[9];            /* 델타         */
		char    eger[9];            /* e*기어링     */
		char	cpvr[9];			/* 틱환산		*/
	};

	typedef struct  {		/* GRID form output format  */
		char    aflg[1];		/* add action to top or bottom  */
		/* '0':replace '1':top, '2':bottom */
		char    sdir[1];		/* sort direction       */
		/* '1':asc, '2':desc    */
		char    scol[16];		/* sorting column symbol    */
		char    xpos[1];		/* continuous status        */
		/* 0x40: default(normal)    */
		/* 0x01: PgUp, ScrUp enable */
		/* 0x02: PgDn, ScrDn enable */
		/* 0x04: no local sorting   */
		char    page[4];		/* # of page            */
		char    save[80];		/* grid inout save field    */
	} GRID_O;			/* grid header */

	struct mod
	{
		char flag[1]{};
		GRID_O  grid_o;
		char nrec[4]{};				//gr7id row 개수
		struct grid grid[1];
	}*mod{};

	char* buf = (char*)data->data;
	mod = (struct mod*)buf;

	CString oflag, nrec, code;
	GRID_O m_grido{};
	int ncnt = 0;
	const int nsize = sizeof(struct grid);
	
	oflag.Format("%.*s", sizeof(mod->flag), mod->flag);
	CopyMemory(&m_grido, &mod->grid_o, sizeof(GRID_O));
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);

	CGridData sdata;
	const UINT kind = MAKE_TREEID(xELWISSUE);
	sdata.SetKind(kind);

	for(int ii=0 ; ii<ncnt ; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);
			sdata.m_arDatas.Add(code);
		}
	}

	SendTreeData(sdata);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

//081219 :: 추가모듈 
void CTreeWnd::setELWBaseAsset(CRecvData* data)
{
	struct  grid 
	{
		char    seqn[4];
		char    cod2[7];            /* RTS COD      */
		char    code[6];            /* 종목코드     */
		char    hnam[20];           /* 종목명       */
		char    curr[8];            /* 현재가       */
		char    diff[7];            /* 대비         */
		char    rate[6];            /* 등락율       */
		char    mdga[8];            /* 매도호가     */
		char    msga[8];            /* 매수호가     */
		char    cvol[9];            /* 체결량       */
		char    gvol[9];            /* 거래량       */
		char    gamt[12];           /* 거래대금     */
		char    dvol[8];            /* 순매도잔량   */
		char    svol[8];            /* 순매수잔량   */
		char    irga[7];            /* 이론가       */
		char    grat[7];            /* 괴리율       */
		char    prty[7];            /* 패리티       */
		char    gerr[7];            /* 기어링비율   */
		char    brer[7];            /* 손익분기율   */
		char    cfpv[7];            /* 자본지지점   */
		char    gcjs[18];           /* 기초자산     */
		char    bhsa[20];           /* 발행사       */
		char    jjis[6];            /* 잔존일수     */
		char    hsgg[9];            /* 환산가       */
		char    hsga[9];            /* 행사가격     */
		char    jhre[9];            /* 전환비율     */
		char    delt[9];            /* 델타         */
		char    eger[9];            /* e*기어링     */
		char	cpvr[9];			/* 틱환산		*/
	};
	
	typedef struct  {		/* GRID form output format  */
		char    aflg[1];		/* add action to top or bottom  */
		/* '0':replace '1':top, '2':bottom */
		char    sdir[1];		/* sort direction       */
		/* '1':asc, '2':desc    */
		char    scol[16];		/* sorting column symbol    */
		char    xpos[1];		/* continuous status        */
		/* 0x40: default(normal)    */
		/* 0x01: PgUp, ScrUp enable */
		/* 0x02: PgDn, ScrDn enable */
		/* 0x04: no local sorting   */
		char    page[4];		/* # of page            */
		char    save[80];		/* grid inout save field    */
	} GRID_O;			/* grid header */
	
	struct mod
	{
		char flag[1]{};
		GRID_O  grid_o{};
		char nrec[4]{};				//gr7id row 개수
		struct grid grid[1];
	}*mod{};
	
	char* buf = (char*)data->data;
	mod = (struct mod*)buf;
	
	CString oflag, nrec, code;
	GRID_O m_grido{};
	int ncnt = 0;
	constexpr int nsize = sizeof(struct grid);
	
	oflag.Format("%.*s", sizeof(mod->flag), mod->flag);
	CopyMemory(&m_grido, &mod->grid_o, sizeof(GRID_O));
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);
	
	CGridData sdata;
	const UINT kind = MAKE_TREEID(xELWASSET);
	sdata.SetKind(kind);
	
	for(int ii=0 ; ii<ncnt ; ii++)
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'J')
				code = code.Mid(1);
			sdata.m_arDatas.Add(code);
		}
	}
	
	SendTreeData(sdata);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

void CTreeWnd::parsingRecommand(CRecvData* data)
{
	char* buf = (char*)data->data;
	int count = atoi(CString(buf, 4));
	const gsl::span<char, 12> spancode((buf + 4), count);


	CGridData sdata;
	const UINT kind = MAKE_TREEID(xRECOMMAND);
	sdata.SetKind(kind);

	for_each(spancode.begin(), spancode.end(), [&sdata](CString code){
		code.Trim();
		if (!code.IsEmpty())
		{
			sdata.m_arDatas.Add(code);
		}
	});

	SendTreeData(sdata);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

void CTreeWnd::parsingNews(CRecvData* data)
{
	struct grid
	{
		char cod2[12];
	};

	struct  mod 
	{
	    char    nrec[4];
	    struct grid	grid[1];
	}*mod{};

	char*	buf = data->data;
	mod = (struct mod*)buf;
	CString	nrec, code;
	int	ncnt = 0;
	constexpr int	nsize = sizeof(struct grid);
	nrec.Format("%.*s", sizeof(mod->nrec), mod->nrec);
	ncnt = atoi(nrec);

	CGridData	sdata;
	const UINT	kind = MAKE_TREEID(xISSUE);
	sdata.SetKind(kind);

	for ( int ii = 0 ; ii < ncnt ; ii++ )
	{
		code.Format("%.*s", sizeof(mod->grid[ii].cod2), mod->grid[ii].cod2);
		code.TrimRight();
		if (!code.IsEmpty())
		{
			if (code.GetAt(0) == 'A' || code.GetAt(0) == 'Q')	//2016.07.04 KSJ 이슈종목일때 ETN뉴스도 표시				
				code = code.Mid(1);
			sdata.m_arDatas.Add(code);
		}
	}
	
	SendTreeData(sdata);
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

BOOL CTreeWnd::IsStockAcc(CString sAcc)
{
	sAcc.Remove('-');
	if (sAcc.GetLength() >= 10)
		if (sAcc.Mid(8, 2) == "11" || sAcc.Mid(8, 1) == "6")
			return TRUE;
	return FALSE;
}

void CTreeWnd::saveGroupwhenClose(int index)
{
	CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE, (LPARAM)index);
}

void CTreeWnd::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	CRect rt;
	GetClientRect(&rt);

	CPoint	pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	HTREEITEM	hItem = HitTest(pt);

	if (hItem == nullptr)
		return;

	m_selItem = hItem;
	
	CString	text = GetItemText(hItem);


	CRect	rect, chRect;
	GetItemRect(hItem, rect, TRUE);
	chRect = CRect(rect.left - 16, rect.top, rect.right, rect.bottom);	//2012.08.29 KSJ 트리의 폴더를 클릭해도 조회되도록 수정함.

	if (!chRect.PtInRect(pt))
		return;
	
	UINT	selItem = GetItemData(hItem);
	struct _treeID*	treeID = CAST_TREEID(selItem);


	//전에 관심종목을 클릭한 상태에서만 저장여부 판단
	if(m_selectTreetype == xINTEREST || m_selectTreetype == NULL)
	{
		if(m_selectTree != -1)
		{
			saveGroupwhenClose(m_selectTree);
		}
	}
	
	m_selectTree = treeID->value - 1;
	m_selectTreetype = treeID->kind;

	CString string;
	//int rlt;
	string = "";
	
	//일단 선택된 그룹 영역의 추천종목클릭인지에 대한 변수 초기화
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));	
	

	//정렬기능 초기화
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));	
	pWnd->SendMessage(WM_MANAGE, MK_SETARRANGENOACTIVE);

	if(treeID->kind == xREMAIN)
	{
		if (this->ItemHasChildren(hItem))
		{
			
		}
		else
		{
			//보유계좌에 계좌 안보일 경우, 그룹계좌 등록 유도 
			switch (treeID->depth)
			{
			case TD_ROOT:
/*
				rlt = AfxMessageBox("그룹계좌등록이 안되어 있습니다. 등록하시겠습니까?", IDOK, 0);

				if(rlt == IDOK)
				{	
					string.Format("%s /p5/S/d", "IB0000A2");
					m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)string.operator LPCTSTR());					
					MessageBox("그룹계좌등록작업을 마치신 후 관심종목화면을 종료하시고 다시 열어주세요", "IBK투자증권");
				}				
*/				
				return;
			case TD_MID:
				break;
			case TD_ITEM:
				break;
			}	
		}
	}

	if (treeID->kind == xINTEREST && treeID->depth == TD_ROOT)
		return;
	 
	switch (treeID->kind)
	{
	case xISSUE:
		queryNews();
		break;
	case xREMAIN:
		{
			clickRemain(selItem);
		}
		break;
	case xELWISSUE:
		getELWIssueSec(treeID->value, this->GetParentItem(hItem));
		
		break;
	case xELWASSET:
		getELWBaseAsset(treeID->value, this->GetParentItem(hItem));
		break;
	default:
		{
			int	opt = 0;
			
			switch (treeID->kind)
			{
			case xINTEREST:
				saveGroupIndex(treeID->value);
				break;
			case xREMAIN:
				{
					m_kind = selItem;
				}
				break;
// 			case xETFCODE:
// 				{
// 					UINT	sItem = GetItemData(this->GetParentItem(hItem));
// 					struct _treeID*	treeID = CAST_TREEID(sItem);
// 					
// 					switch (treeID->depth)
// 					{
// 					case TD_ROOT:
// 						return;
// 					case TD_MID:
// 						break;
// 					case TD_ITEM:
// 						break;
// 					}
// 				}	
// 				break;
			default:
				opt = (int)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SEARCHOPT, MO_GET));
				break;
			}

			const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
			
			//MO_VISIBLE모드일때는 항상 맨 앞을 선택하도록 한다
			if(nOver == MO_VISIBLE)
			{
				pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
				pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)0);
			}	

			if (SendOper(selItem, opt))
			{
				const CWnd* pwnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				pwnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)selItem);	
			}
		}		

		break;
	}

}

void CTreeWnd::ReSend()
{
	HTREEITEM	hItem = GetSelectedItem();
	
	if (hItem == nullptr)
		return;
	
	CString	text = GetItemText(hItem);
	UINT	selItem = GetItemData(hItem);
	struct _treeID*	treeID = CAST_TREEID(selItem);

	//전에 관심종목을 클릭한 상태에서만 저장여부 판단
	if(m_selectTreetype == xINTEREST || m_selectTreetype == NULL)
	{
		if(m_selectTree != -1)
		{
			saveGroupwhenClose(m_selectTree);
		}
	}
	
	m_selectTree = treeID->value - 1;
	m_selectTreetype = treeID->kind;

	CString string;
	//int rlt;
	string = "";
	
	//일단 선택된 그룹 영역의 추천종목클릭인지에 대한 변수 초기화
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));	

	//정렬기능 초기화
	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));	
	pWnd->SendMessage(WM_MANAGE, MK_SETARRANGENOACTIVE);

	if(treeID->kind == xREMAIN)
	{
		if (this->ItemHasChildren(hItem))
		{
			
		}
		else
		{
			//보유계좌에 계좌 안보일 경우, 그룹계좌 등록 유도 
			switch (treeID->depth)
			{
			case TD_ROOT:
/*
				rlt = AfxMessageBox("그룹계좌등록이 안되어 있습니다. 등록하시겠습니까?", IDOK, 0);

				if(rlt == IDOK)
				{	
					string.Format("%s /p5/S/d", "IB0000A2");
					m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_OPENSCR, typePOPUP), (LPARAM)string.operator LPCTSTR());					
					MessageBox("그룹계좌등록작업을 마치신 후 관심종목화면을 종료하시고 다시 열어주세요", "IBK투자증권");
				}				
*/				
				return;
			case TD_MID:
				break;
			case TD_ITEM:
				break;
			}	
		}

	}

	if (treeID->kind == xINTEREST && treeID->depth == TD_ROOT)
		return;
	
	switch (treeID->kind)
	{
	case xISSUE:
		queryNews();
		break;
	case xREMAIN:
		{
			clickRemain(selItem);
		}
		break;
	case xELWISSUE:
		getELWIssueSec(treeID->value, this->GetParentItem(hItem));
		
		break;
	case xELWASSET:
		getELWBaseAsset(treeID->value, this->GetParentItem(hItem));
		break;
	default:
		{
			int	opt = 0;
			
			switch (treeID->kind)
			{
			case xINTEREST:
				saveGroupIndex(treeID->value);
				break;
			case xREMAIN:
				{
					m_kind = selItem;
				}
				break;
			default:
				opt = (int)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SEARCHOPT, MO_GET));
				break;
			}
	
			if (SendOper(selItem, opt))
			{
				const CWnd* pwnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				pwnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)selItem);	
			}
		}		

		break;
	}
}


BOOL CTreeWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    const int nLen = str.GetLength();
    for( int i=0 ; i < nLen ; ++i)
    {
        if( isdigit(str.GetAt(i)) == FALSE)
        {
            bRet = FALSE;
            break;
        }
    }
    return bRet;
}


void CTreeWnd::clickRemain(UINT	selItem)
{
	m_kind = selItem;
	m_arJango.RemoveAll();

	HTREEITEM hItem = nullptr;
	
	m_mapParam.Lookup(m_kind, hItem);
	CString strAccount(_T(""));
	
	if (hItem == nullptr)
		return;
	
	struct _treeID*	treeID = CAST_TREEID(m_kind);
	
	strAccount = CString(GetItemText(hItem));
	
	if(strAccount.GetLength() != 13)
		return;
	
	CString tempAccount = strAccount;

	strAccount.Remove(_T('-'));
	strAccount.TrimLeft();
	strAccount.TrimRight();

	CString pswd, acctdata, strMember;
	bool bIsMember;

	acctdata = Variant(accountCC, "");
	strMember = Variant(userCC, "");
	bIsMember = FALSE;
	
	//직원 고객 구분
	if(IsNumber(strMember))
	{
		pswd = m_accn.loadPass(acctdata,strAccount);

		if(pswd == "")
		{
			CInputPswd dlg(nullptr, tempAccount);
			
			if(dlg.DoModal() == IDOK)
			{
				pswd = dlg.m_strPassword;
			}
			else
			{
				return;
			}
		}
	}
	else
	{
		pswd = "9999";
	}
	
	if(strAccount.Mid(3, 1) == "2")
	{
		queryFutureRemain2(strAccount, pswd);
	}
	else
	{
		queryRemain2(strAccount, pswd);
	}
	
}

void CTreeWnd::selectinterest(HTREEITEM hParent, DWORD val)
{
	HTREEITEM	hChild = GetChildItem(hParent);
	DWORD		param = 0;
	
	while (hChild)
	{
		param = GetItemData(hChild);
		if (param == val)
		{
			SelectItem(hChild);
			break;
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
}

void CTreeWnd::SendInterest(HTREEITEM hItem)
{
	const int opt = 0;
	const UINT	selItem = GetItemData(hItem);
	
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int	nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);	
	
	//MO_VISIBLE모드일때는 항상 맨 앞을 선택하도록 한다
	if(nOver == MO_VISIBLE)
	{
		pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)0);
	}	
	
	if (SendOper(selItem, opt))
	{
		const CWnd* pwnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		pwnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)selItem);	
	}
}

//관심그룹 index 저장
void CTreeWnd::saveGroupIndex(int index)
{
	CString	str, m_fileCFG;

	str.Format("%02d", index);

	const int nKey = m_pMainWnd->SendMessage(WM_MANAGE,MK_GETKEY);

	CString strCfg;
	strCfg.Format("intercfg%d.ini",nKey);
	
	m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, strCfg);

	WritePrivateProfileString(SEC_MAIN, KEY_SELECTGROUP, str.operator LPCTSTR(), m_fileCFG);
}


void CTreeWnd::queryFutureRemain(CString strAccount)
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, FutureService, sizeof(ledger.svcd));
	CString strUser = CString((char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CString strBrno = (char*)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
	CopyMemory(&ledger.brno, strBrno.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
	
	CString strLedger = CString((char*)&ledger, L_ledgerH);

	//Input data Setting
	struct future_mid mid;
	ZeroMemory(&mid, L_future_mid);
	CopyMemory(&mid.nrec, _T("00001"), sizeof(mid.nrec));
	CopyMemory(&mid.accn, (LPCTSTR)strAccount, sizeof(mid.accn));
	CopyMemory(&mid.fill, _T("         "), sizeof(mid.fill));
	CopyMemory(&mid.pass, _T("9999    "), sizeof(mid.pass));

	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_future_mid);
	strSendData = strLedger + strMid;
	
	CSendData sData;
	sData.SetData("piboPBxQ", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}

void CTreeWnd::queryFutureRemain2(CString strAccount, CString sPswd)
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, FutureService, sizeof(ledger.svcd));
	CString strUser = CString((char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CString strBrno = (char*)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
	CopyMemory(&ledger.brno, strBrno.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '2';
	ledger.odrf[0] = '1';
	
	CString strLedger = CString((char*)&ledger, L_ledgerH);

	struct f_mid mid;
	
	FillMemory(&mid, L_fmid, ' ');
	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
	CopyMemory(&mid.date, "", sizeof(mid.date));


	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_fmid);
	strSendData = strLedger + strMid;
	
	CSendData sData;
	sData.SetData("PIBOFJGO", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}



void CTreeWnd::queryRemain2(CString strAccount, CString sPswd)
{
	//int skey=-1;

	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');
	
	CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, StockService, sizeof(ledger.svcd));
	CString strUser = CString((char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CString strBrno = (char*)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
	CopyMemory(&ledger.brno, strBrno.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
	
	CString strLedger = CString((char*)&ledger, L_ledgerH);

	struct s_mid mid;
	FillMemory(&mid, L_smid, ' ');
	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
	mid.allf[0] = '1';
	
	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_smid);
	strSendData = strLedger + strMid;
	
	CSendData sData;
//	sData.SetData("PIBOSJGO", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	sData.SetData("PIBOSJG2", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}


void CTreeWnd::queryRemain(CString strAccount)
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	
	CopyMemory(&ledger.svcd, StockService, sizeof(ledger.svcd));
	CString strUser = CString((char*)m_pMainWnd->SendMessage(WM_MANAGE, MK_GETID));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	
	CString strBrno = (char*)pView->SendMessage(WM_USER, MAKEWPARAM(variantDLL, deptCC), 0L);
	CopyMemory(&ledger.brno, strBrno.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));
	
	ledger.fkey[0] = 'C';
	ledger.mkty[0] = '1';
	ledger.odrf[0] = '1';
	
	CString strLedger = CString((char*)&ledger, L_ledgerH);

	//Input data Setting
	struct jango_mid mid;
	ZeroMemory(&mid, L_jango_mid);
	CopyMemory(&mid.nrec, _T("00001"), sizeof(mid.nrec));
	CopyMemory(&mid.accn, (LPCTSTR)strAccount, sizeof(mid.accn));
	CopyMemory(&mid.fill, _T("         "), sizeof(mid.fill));
	CopyMemory(&mid.pass, _T("9999    "), sizeof(mid.pass));
	mid.zQryTp = '0';
	mid.zD2balBase = '1';

	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_fmid);
	strSendData = strLedger + strMid;

	CSendData sData;
	sData.SetData("piboPBxQ", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}



BOOL CTreeWnd::sendTR(CString sTR, CString sData, int nKey, int nStat, CString Acnt)
{
	CString sTRData = "";
	CString sKey = "";
	//int ikey = -1;
	struct _userTH user;
	strncpy(&user.trc[0], sTR, 8);
	

	user.key  = nKey;//ikey;
	
	user.stat = nStat;
	
	sTRData  = CString((char*)&user, sizeof(_userTH));
	sTRData += sData;
	
	return m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, sData.GetLength()), 
		(LPARAM)(const char*)sTRData); 
}

void CTreeWnd::oubRemain(char* buf, int len)
{
	struct _ledgerH ledger;
	CopyMemory(&ledger, (void*)buf, L_ledgerH);
	CString strLedger = CString((char*)&ledger, L_ledgerH);
	CString strErrCode = CString((char*)&ledger.emsg, 4);
	CString strErrText = CString((char*)&ledger.emsg, 98);
	CString strServiceName = CString((char*)&ledger.svcd, 8);

	CGridData	sdata;

	sdata.SetKind(m_kind);

	CString strMsg = "ERR\t" + strErrText;

	if (strErrCode.GetAt(0) != _T('0'))	// 오류
	{
		strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
		MessageBox(strMsg, "IBK투자증권");
//		return;
	}
	else
	{

		buf += L_ledgerH;
		
		if(strServiceName == FutureService)		//선물잔고
		{
			settingDataFuture2(buf, sdata);	
		}
		else			//현물잔고
		{
			settingDataStock2(buf, sdata);
		}

		SendTreeData(sdata);
		
		
		CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_kind);
	}

}
/*
void CTreeWnd::settingDataStock(char* buf, class CGridData& sdata)
{
	//struct jango_mod mod;
	std::unique_ptr<struct jango_mod> mod = std::make_unique<struct jango_mod>();
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod->nrec, sizeof(mod->nrec)));
	
	CString qty, AvrUprc, loanDtlcode, creditPrc, maeipPrc;
	int iQty{}, icreditPrc{}, iMaeipPrc{};
	double dPrice{};
	CString result_qty, result_Price, result_CPrice, result_MPrice;
	
	struct out_rec* rec{};
	for (int i = 0 ; i< nRec ; i++)
	{
	//	struct out_rec rec = mod.grid[i];   
		rec = &mod->grid[i];
		CString strCode(rec->code, sizeof(rec->code));
		strCode.TrimRight();
		
		if (!strCode.IsEmpty())
		{
			if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J') || strCode.GetAt(0) == _T('Q'))  //202004 ETN종목 추가
			{
				if(strlen(strCode) == 7)
					strCode.Delete(0);
			}
			qty = CString(rec->dealbaseQty, sizeof(rec->dealbaseQty));
			AvrUprc = CString(rec->AvrUprc, sizeof(rec->AvrUprc));
			loanDtlcode = CString(rec->loanDtlcode, sizeof(rec->loanDtlcode));
			creditPrc = CString(rec->creditAmt, sizeof(rec->creditAmt));
			maeipPrc = CString(rec->buyAmt, sizeof(rec->buyAmt));
			
			//			if(loanDtlcode != "80")		//신용은 표기하지 않음
			{
				if(qty.Left(1) == '+')
					qty.Delete(0);
				
				if(AvrUprc.Left(1) == '+')
					AvrUprc.Delete(0);
				
				if(creditPrc.Left(1) == '+')
					creditPrc.Delete(0);
				
				if(maeipPrc.Left(1) == '+')
					maeipPrc.Delete(0);
				
				iQty = atoi(qty);
				dPrice = atof(AvrUprc);
				iMaeipPrc = atoi(maeipPrc);
				icreditPrc = atoi(creditPrc);
				
				result_qty.Format("%d", iQty);
				result_Price.Format("%.2f", dPrice);
				result_CPrice.Format("%d", icreditPrc);
				result_MPrice.Format("%d", iMaeipPrc);
				
				//패킷생성(종목번호, 잔고, 매입단가)
				sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", "", result_CPrice, result_MPrice));
			}
			
		}
	}
	
	if(rec != nullptr)
		rec = nullptr;
}
*/
void CTreeWnd::settingDataStock2(char* buf, class CGridData& sdata)
{
	std::unique_ptr<struct s_mod > mod = std::make_unique<struct s_mod >();
	int sizeJango = sizeof(struct s_mod);
	const int sBuf = strlen(buf);
	int nRec = 0;

	CopyMemory(mod.get(), (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod->nrec, sizeof(mod->nrec)));

	CString qty, AvrUprc, loanDtlcode, creditPrc, maeipPrc;
	int iQty{}, icreditPrc{}, iMaeipPrc{};
	double dPrice{};
	CString result_qty, result_Price, result_CPrice, result_MPrice;

	//modi  보유종목100 이상일때 가이드
	if (nRec > 100)
	{
		nRec = 100;
		SetTimer(TM_BALANCE100, 1000, nullptr);
	}

	struct s_rec* rec = nullptr;
	for (int i = 0 ; i<nRec ; i++)
	{
		rec = &mod->rec[i];
		CString strCode(rec->cod2, sizeof(rec->cod2));
		strCode.TrimRight();
		
		if (!strCode.IsEmpty())
		{
			if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J') || strCode.GetAt(0) == _T('Q'))  //202004 ETN추가
			{
				if(strlen(strCode) == 7)
					strCode.Delete(0);
			}
			
			qty = CString(rec->jqty, sizeof(rec->jqty));
			AvrUprc = CString(rec->pamt, sizeof(rec->pamt));
			loanDtlcode = CString(rec->sycd, sizeof(rec->sycd));
			creditPrc = CString(rec->samt, sizeof(rec->samt));
			maeipPrc = CString(rec->mamt, sizeof(rec->mamt));
			
			//if(loanDtlcode != "80")		//신용은 표기하지 않음
			{
				if(qty.Left(1) == '+')
					qty.Delete(0);
				
				if(AvrUprc.Left(1) == '+')
					AvrUprc.Delete(0);
				
				if(creditPrc.Left(1) == '+')
					creditPrc.Delete(0);
				
				if(maeipPrc.Left(1) == '+')
					maeipPrc.Delete(0);
				
				iQty = atoi(qty);
				dPrice = atof(AvrUprc);
				iMaeipPrc = atoi(maeipPrc);
				icreditPrc = atoi(creditPrc);
				
				result_qty.Format("%d", iQty);
				result_Price.Format("%.0f", dPrice);
				result_CPrice.Format("%d", icreditPrc);
				result_MPrice.Format("%d", iMaeipPrc);
				
				//패킷생성(종목번호, 잔고, 매입단가)
				sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", "", result_CPrice, result_MPrice));
			}
			
		}
	}
	
	if(rec != nullptr)
		rec = nullptr;
}

double CTreeWnd::Round(double data ) //(반올림)
{
    return (int)(data + 0.5);
}


void CTreeWnd::settingDataFuture2(char* buf, class CGridData& sdata)
{
	struct f_mod mod {};
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	CString qty, AvrUprc, gubn, maeipPrc, pyungaPrc, sonikPrc;
	int iQty = 0;
	
	double dPrice = 0;
	CString result_qty, result_Price, result_maeipPrice, result_gubn;
	double dmaeipPrice{};
	
	struct f_rec* rec = nullptr;
	for (int i = 0 ; i<nRec ; i++)
	{
		rec = (struct f_rec*)&mod.rec[i];
		if (rec)
		{
			CString strCode(rec->cod2, sizeof(rec->cod2));
			strCode.TrimRight();

			if (!strCode.IsEmpty())
			{
				qty = CString(rec->bqty, sizeof(rec->bqty));
				AvrUprc = CString(rec->curr, sizeof(rec->curr));
				gubn = CString(rec->dsgb, sizeof(rec->dsgb));
				maeipPrc = CString(rec->tamt, sizeof(rec->tamt));

				if (qty.Left(1) == '+')
					qty.Delete(0);

				if (AvrUprc.Left(1) == '+')
					AvrUprc.Delete(0);

				if (maeipPrc.Left(1) == '+')
					maeipPrc.Delete(0);

				if (gubn.Left(1) == '+' || gubn.Left(1) == '-')
					gubn.Delete(0);

				gubn.TrimLeft(); gubn.TrimRight();

				if (gubn == "매수")
				{
					result_gubn = "2";
				}
				else
				{
					result_gubn = "1";
				}

				iQty = atoi(qty);
				dPrice = atof(AvrUprc);
				dmaeipPrice = atof(maeipPrc);

				result_qty.Format("%d", iQty);
				result_Price.Format("%.2f", dPrice);
				result_maeipPrice.Format("%.2f", dmaeipPrice);

				//패킷생성(종목번호, 잔고, 매입단가)
				sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", result_gubn, "", result_maeipPrice));

			}
		}
	}

	if(rec != nullptr)
		rec = nullptr;
}

void CTreeWnd::oubRemainEx(char* buf, int len)
{
	CString datS = CString(buf, len);
	CString infoS, code, num, price, sztmp, sJango;
	HTREEITEM	hRoot = nullptr, hItem = nullptr;
	m_mapParam.Lookup(xREMAIN, hRoot);
//	int	ii = 0;

	// 종목\t보유수량\t가격
	while (!datS.IsEmpty())
	{
		infoS = IH::Parser(datS, "\n");
		if (infoS.IsEmpty())
			break;

		sztmp = IH::Parser(infoS, "\t");	// 계좌번호상품번호
		sztmp = IH::Parser(infoS, "\t");	// 계좌명
		code = IH::Parser(infoS, "\t");		// 종목번호

		sJango = "";
		if (code.GetLength() && code.GetAt(0) == 'A')
			sJango = code.Mid(1);
		
		sztmp = IH::Parser(infoS, "\t");	// 종목명
		sztmp = IH::Parser(infoS, "\t");	// 잔고구분	
		sztmp = IH::Parser(infoS, "\t");	// 결제잔고
		sztmp = IH::Parser(infoS, "\t");	// 체결잔고
		
		num = IH::Parser(infoS, "\t");	// 매도가능수량
		price = IH::Parser(infoS, "\t");	// 매입평균단가	
		if (!sJango.IsEmpty())
		{
			price.TrimLeft();	price.TrimRight();
			sztmp.Format("%s\t%s\t%s", sJango, num, price);
			m_arJango.Add(sztmp);

			// 20071228 cej 보유계좌 최대 100까지 보여줌
			if (m_arJango.GetSize()+1 >= MAX_LINE)
				break;
		}

		sztmp = IH::Parser(infoS, "\t");	// 현재가	
		sztmp = IH::Parser(infoS, "\t");	// 평가손익
		sztmp = IH::Parser(infoS, "\t");	// 수익률	
	}
}

CString	CTreeWnd::makeAccn(CString accnnum, CString accnname)
{
	CString	str = _T("");

	if (accnnum.GetLength() == 11)
		str.Format("%s-%s-%s", accnnum.Left(3), accnnum.Mid(3, 2), accnnum.Right(6));
	
	str += (" " + accnname);
	return str;
}

CString	CTreeWnd::GetAccName(CString accn)
{
	const int nPos = accn.Find(" ");
	if (nPos >= 0)
		accn = accn.Mid(nPos + 1);

	return accn;
}

CString CTreeWnd::GetAccNum(CString accn)
{
	const int nPos = accn.Find("-");
	if (nPos >= 0)
		accn = accn.Left(nPos);

	return accn;
}

CString	CTreeWnd::GetAccGubn(CString accn)
{
	const int	nPos = accn.Find("-");
	
	if (nPos >= 0)
		accn = accn.Mid(nPos + 1, 2);

	return accn;
}

void CTreeWnd::GetAccnInGroup(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem)
{
	HTREEITEM	hChild = GetChildItem(hItem);
	CString		str, str2;
	while (hChild)
	{
		str = GetItemText(hChild);
		if (!mapACCN.Lookup(str, str2))
		{
			arACCN.Add(str);
			mapACCN.SetAt(str, "");
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
}

void CTreeWnd::GetAccnTotal(CMapStringToString& mapACCN, CStringArray& arACCN, HTREEITEM hItem)
{
	HTREEITEM	hChild = GetChildItem(hItem);
	CString		str, str2;
	while (hChild)
	{
		if (this->ItemHasChildren(hChild))
			GetAccnInGroup(mapACCN, arACCN, hChild);
		else
		{
			str = GetItemText(hChild);
			if (!mapACCN.Lookup(str, str2))
			{
				arACCN.Add(str);
				mapACCN.SetAt(str, "");
			}
		}
		hChild = GetNextItem(hChild, TVGN_NEXT);
	}
}

void CTreeWnd::makeInterGroup(std::vector<std::pair<CString, CString>>& vGroup)
{	
	HTREEITEM item = GetChildItem(_treeInter);
	while (item != NULL)
	{
		HTREEITEM next = GetNextItem(item, TVGN_NEXT);
		DeleteItem(item);	
		item = next;
	}

	UINT gubn = MAKE_TREEID(xINTEREST);
	struct _treeID* treeID = CAST_TREEID(gubn);
	for_each(vGroup.begin(), vGroup.end(), [&](auto item) {
		const int kind = MAKE_TREEID(treeID->kind, 0, 2, atoi(item.second), TD_ITEM);	
		SetItemData(InsertItem(item.first, 0, 1, _treeInter), kind);
	});
}

void CTreeWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
		case TM_BALANCE100:
		{
			KillTimer(nIDEvent);
			CString sval;
			sval = _T("보유종목은 100개 까지 조회됩니다.");
			m_pMainWnd->SendMessage(WM_MANAGE, MK_SHOWTOOLTIP, (LPARAM)sval.operator LPCSTR());
		}
		break;
	}
	CTreeCtrl::OnTimer(nIDEvent);
}
