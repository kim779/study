// TreeWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_INTERGRID.h"
#include "TreeWnd.h"
#include "sharemsg.h"
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

struct _exKospi {
	char* code;
	char* name;
} exKospi[] = {
	{"101", "KOSPI200"},	{"401", "KOSPI100"},
	{"402", "KOSPI50"},	//{"403", "KOSPI IT"},
};
const int cnt_exKospi = sizeof(exKospi) / sizeof(struct _exKospi);

struct _exKosdaq {
	char* code;
	char* name;
} exKosdaq[] = {
	//{"301", "KOSDAQ50"},			// 20070608
//	{"242", "프리미어"},
	{"303", "KOSTAR"},
};



#pragma warning(disable : 6387)

const int cnt_exKosdaq = sizeof(exKosdaq) / sizeof(struct _exKosdaq);
#define TM_BALANCE100 9898
CTreeWnd::CTreeWnd(CWnd* pMainWnd)
{
	m_pMainWnd = pMainWnd;
	
	m_hjcode.RemoveAll();
	m_fjcode.RemoveAll();
	m_ojcode.RemoveAll();
	m_pjcode.RemoveAll();
	m_upcode.RemoveAll();
	m_elwbase.RemoveAll();
	m_bCustomer = false;
	m_bOper = false;
	m_selectTree = -1;
	m_selectTreetype = 0;
	m_pageView = -1;
}

CTreeWnd::~CTreeWnd()
{
	m_ilTree.DeleteImageList(); 

	m_accnT.RemoveAll();
	m_pSData.Reset();
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
//	queryAcc();	
	return 0;
}

void CTreeWnd::loadingHJcode()
{
//	if (m_hjcode.GetSize() > 0) return;
//
//	CFile	fileH;
//	CString	filePath;
//	struct  hjcode  hjcode {};
//	struct	_shjcode s_hjcode;
//	
//	filePath.Format("%s/%s/%s", m_root, TABDIR, HJCODE);
//  	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
//	{
//		AfxMessageBox("종목 파일을 읽을 수가 없습니다.");
//		return;
//	}
//
//	const int countC = gsl::narrow_cast<int>(fileH.GetLength() / sizeof(struct hjcode));
//
//	for (int ii = 0; ii < countC; ii++)
//	{
//		fileH.Read(&hjcode, sizeof(struct hjcode));
//
//		s_hjcode.name = CString(hjcode.hnam, HNameLen);
//		const int idx = s_hjcode.name.Find('\0');
//		if (idx != -1) s_hjcode.name = s_hjcode.name.Left(idx);
//		s_hjcode.name.MakeUpper(); s_hjcode.name.TrimRight();
//
//		s_hjcode.symb = CString(hjcode.symb, HSymbolLen);
//		s_hjcode.symb.TrimRight();
//
//		s_hjcode.ecng = hjcode.ecng;
//		s_hjcode.size = hjcode.size;
//		s_hjcode.ucdm = hjcode.ucdm;
//		s_hjcode.ucds = hjcode.ucds;
//		s_hjcode.jjug = hjcode.jjug;
//		s_hjcode.kpgb = hjcode.kpgb;
//		s_hjcode.ucmd = hjcode.ucmd;
//		s_hjcode.kosd = (char)hjcode.kosd;
//		s_hjcode.ssgb = hjcode.ssgb;
//		s_hjcode.wsgb = hjcode.wsgb;
//		s_hjcode.jsiz = hjcode.jsiz;
//		s_hjcode.itgb = hjcode.itgb;
//		s_hjcode.star = hjcode.star;
//		s_hjcode.unio = hjcode.unio;
//		s_hjcode.jchk = hjcode.jchk;
//		s_hjcode.prmr = hjcode.prmr;
//
//		switch (hjcode.ssgb)
//		{
//		case jmSINJU:
//		case jmSINJS:
//		case jmHYFND:
//			s_hjcode.code = CString(hjcode.code, HCodeLen);
//			break;
//		default:
//			s_hjcode.code = CString(hjcode.code, HCodeLen).Mid(1);
//			break;
//		}
//
//		switch (s_hjcode.ssgb)
//		{
//		case jmHYFND:
////			continue;
//		default:
//			m_hjcode.Add(s_hjcode);
//			break;
//		}
//      	}
//
//	fileH.Close();
}


void CTreeWnd::loadingFJcode()
{
	if (m_fjcode.GetSize() > 0) return;

	CFile	fileH;
	CString	filePath;
	struct  fjcode  fjcode;
	struct	_sfjcode s_fjcode;

	filePath.Format("%s/%s/%s", m_root, TABDIR, FJCODE);
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
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
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
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
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
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
	if (!fileH.Open(filePath, CFile::modeRead|CFile::typeBinary))
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
	const UINT	sItem = GetItemData(parent);
	const struct _treeID*	treeID = CAST_TREEID(sItem);

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
	CopyMemory(&mid.rtsf, _T("01"), 2);
	CopyMemory(&mid.fill, _T(" "), sizeof(mid.fill));
	CopyMemory(&mid.gvol, _T(" "), sizeof(mid.gvol));
	CopyMemory(&mid.gsrt, _T(" "), sizeof(mid.gsrt));
	
	CString strSendData = CString((char*)&mid, sizeof(mid));
	CSendData sData;
	
	sData.SetData("PIBO2805", TRKEY_ELWBASEASSET, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
}

void CTreeWnd::clickRecommand()
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

	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
 	pWnd->SendMessage(WM_MANAGE, MK_CHANGERECOMMANDFIELD);

	sData.SetData("pihorsdt", TRKEY_RECOMMAND, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);
	
	
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
	const UINT	sItem = GetItemData(parent);
	const struct _treeID*	treeID = CAST_TREEID(sItem);
	
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
	CString	filePath, string, string_code, string_name, skey;

	
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
	CopyMemory(&mid.kcod, _T(" "), sizeof(mid.kcod));
	CopyMemory(&mid.gcod, _T("A99999999999"), sizeof(mid.gcod));
	CopyMemory(&mid.bcod, string_code, sizeof(mid.bcod));
	CopyMemory(&mid.jjis, _T("0"), sizeof(mid.jjis));
	CopyMemory(&mid.sort, _T("01"), sizeof(mid.sort));
	CopyMemory(&mid.rtsf, _T("01"), 2);
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
	_sfjcode	sjcode;
	struct  elwcode  ELWCode;

	CString path = m_root + "\\tab\\elwcod.dat";
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))	return;

	codeN = gsl::narrow_cast<int>(file.GetLength() / sizeof(struct elwcode));

	bool	bFound{};
	for (int ii = 0; ii < codeN; ii++)
	{
		file.Read(&ELWCode, sizeof(struct elwcode));

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
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	if (pWnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0))
		m_bCustomer = true;

	CString accInfo = Variant(accountCC,"");
	m_accn.load_acc(accInfo);
	
	const CStringArray* pArr = m_accn.get_account();

	for(int i=0;i<pArr->GetSize();i++)
	{
		CString strAccn = pArr->GetAt(i);
		CString strT = strAccn.Mid(3,1);

		if( strT != "5" && strT != "0" )
		{
			m_accnT.Add(strAccn);
		}
	}

	initTree();

//	settingReminAcc();
	selectKospi();
}



void CTreeWnd::settingReminAcc()
{
	HTREEITEM root_Item{};
	CString root_name, child_name;
	const CStringArray* pArr = m_accn.get_account();

//	HTREEITEM hItem{};
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

	//주식 전종목 보여주기
	if(check == false)
	{

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
			HTREEITEM	hItem = nullptr;
			m_mapParam.Lookup((DWORD)lParam, hItem);
			SelectItem(hItem);			
		}		
		break;
	case MK_SELECTTREE:
		m_selectTree = (int)lParam;
		break;
	case MK_CALLINTEREST:
		{
			//HTREEITEM	hItem = nullptr;
			//m_mapParam.Lookup((DWORD)lParam, hItem);
			SendInterest(lParam);
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
	case MK_VIEWTYPE:
		{
			m_pageView = (int)lParam;
		}
		break;
	case MK_SAVEFOROUB:
		{
			RefreshInterest();
		}
		break;
	case MK_REMAIN:
		{
 			m_nAccountIndex = 0;
			m_accnT.RemoveAll();
			m_pSData.Reset();

			m_accnT.Add(m_strAccount);

			MakeTotalRemain(m_nAccountIndex);
			
		}
		break;
	case MK_ACCOUNT:
		{
			m_strAccount = CString((char*)lParam);
		}
		break;
	}

	return ret;
}

void CTreeWnd::MakeTotalRemain(int nIndex)
{
	CString strAccn;

	if(nIndex < m_accnT.GetSize())
	{
		strAccn = m_accnT.GetAt(nIndex);

		m_strAccount = strAccn;

		((CMainWnd*)m_pMainWnd)->m_strAccount = strAccn;
		CString log;
		log.Format("MAKE LOG : %s",strAccn);
		SendTotalRemain(strAccn);
	}
}

void CTreeWnd::initTree()
{
	HTREEITEM hRoot{};// hItem = nullptr;
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
	const UINT mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;

	// 관심종목그룹 Add
	hRoot = InsertItem(mask, _T("관심그룹"), 0, 1, 0, 0, 0, nullptr, nullptr);

	kind = MAKE_TREEID(xINTEREST);
	SetItemData(hRoot, kind = MAKE_TREEID(xINTEREST));
	SetItemDataX(hRoot, kind);
	
	// 관심종목그룹 Add
	hRoot = InsertItem(mask, _T("보유종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(hRoot, kind = MAKE_TREEID(xREMAIN));
	SetItemDataX(hRoot, kind);
	
	hRoot = InsertItem(mask, _T("이슈종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(hRoot, MAKE_TREEID(xISSUE));
	
	//주식종목
	hRoot = InsertItem(mask, _T("주식종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(InsertItem(_T("전종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xALLCODE));
	SetItemData(InsertItem(_T("거래소"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSPI));
	SetItemData(InsertItem(_T("코스닥"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xKOSDAQ));
	SetItemData(InsertItem(_T("K-OTC"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xFREECODE));	
	SetItemData(InsertItem(_T("ETF"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xETFCODE));

	//ELW
	hRoot = InsertItem(mask, _T("ELW"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(InsertItem(_T("전종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWCODE));
	SetItemData(InsertItem(_T("기초자산종목"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWBASE));
	SetItemData(InsertItem(_T("발행회사별"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWISSUE, 0, 0, 0, TD_MID));
	SetItemData(InsertItem(_T("기초자산별"), 0, 1, hRoot, TVI_LAST), MAKE_TREEID(xELWASSET));

	HTREEITEM hChild = GetChildItem(hRoot);
	CString child_name;
	while(hChild)
	{
		child_name = GetItemText(hChild);

		if(child_name == "발행회사별")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWISSUE));
		}
		else if(child_name == "기초자산별")
		{
			SetItemDataX(hChild, MAKE_TREEID(xELWASSET));
		}

		hChild = GetNextSiblingItem(hChild);
	}

	SetItemData(InsertItem(mask, _T("선물"), 0, 1, 0, 0, 0, nullptr, nullptr), MAKE_TREEID(xFUTURE));

	hRoot = InsertItem(mask, _T("KOSPI옵션"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(InsertItem(_T("콜옵션"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xCALLCODE));
	SetItemData(InsertItem(_T("풋옵션"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xPUTCODE));

	hRoot = InsertItem(mask, _T("KOSPI업종"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemDataUJ(hRoot, MAKE_TREEID(xKOSPIUP));

	hRoot = InsertItem(mask, _T("KOSDAQ업종"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemDataUJ(hRoot, MAKE_TREEID(xKOSDAQUP));

	SetItemData(InsertItem(mask, _T("KOSPI200"), 0, 1, 0, 0, 0, nullptr, nullptr), MAKE_TREEID(xKOPSI200));
	SetItemData(InsertItem(mask, _T("KRX100"), 0, 1, 0, 0, 0, nullptr, nullptr), MAKE_TREEID(xKRX100));
	SetItemData(InsertItem(mask, _T("신주인수권"), 0, 1, 0, 0, 0, nullptr, nullptr), MAKE_TREEID(xSINJUCODE));

	hRoot = InsertItem(mask, _T("업종지수"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(InsertItem(_T("거래소"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xKOSPIJISU));
	SetItemData(InsertItem(_T("코스닥"), 0, 1, hRoot, TVI_SORT), MAKE_TREEID(xKOSDAQJISU));
	
	// 섹터종목
	hRoot = InsertItem(mask, _T("섹터종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemDataX(hRoot, MAKE_TREEID(xTHEMACODE));

	// 그룹사별
	hRoot = InsertItem(mask, _T("그룹사별"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemDataX(hRoot, MAKE_TREEID(xGROUPCODE));

	// IBK추천종목
	hRoot = InsertItem(mask, _T("IBK추천종목"), 0, 1, 0, 0, 0, nullptr, nullptr);
	SetItemData(hRoot, MAKE_TREEID(xRECOMMAND));
}

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
	const struct _treeID*	treeID = CAST_TREEID(gubn);
	UINT	kind = 0;
	switch (treeID->kind)
	{
	case xTHEMACODE:
		{
			int readL = 0; char 
			readB[256]{};
			CString	filePath, string, stringx, skey;

			filePath.Format("%s/%s/%s", m_root, TABDIR, "themcd.ini");			

			for (int ii = 0; ii < 200; ii++)
			{
				skey.Format("%02d", ii);
				readL = GetPrivateProfileString(SEC_FOLDER, skey, "", readB, sizeof(readB), filePath);
				if (readL <= 0) break;

				string = CString(readB, readL); stringx = IH::Parser(string, ";"); stringx.TrimRight();
				kind = MAKE_TREEID(treeID->kind, 0, 3, atoi(string), TD_ITEM);
				SetItemData(InsertItem(stringx, 0, 1, hItem, nullptr), kind);
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
				SetItemData(InsertItem(stringx, 0, 1, hItem, nullptr), kind);
			}
		}
		break;

	case xELWISSUE:		//ELW발행회사별 트리에 추가
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey;
			
			filePath.Format("%s/%s/%s", m_root, TABDIR, "ELWLP.CBS");
			
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
					SetItemData(InsertItem(string, 0, 1, hItem, TVI_LAST), kind);
				}
			}
		}
		break;

	case xELWASSET:		//ELW기초자산별 트리에 추가
		{
			int readL = 0; 
			char readB[256]{};
			CString	filePath, string, string_code, string_name, skey;
			//const HTREEITEM hMid = TVI_LAST;
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
			int readL = 0; 
			char readB[1024];	//2015.06.16 KSJ 그룹읽어오는 갯수 늘림
			CString	filePath, string, gno, gname;

			filePath.Format("%s/%s/%s/%s", m_root, USRDIR, m_user, FILE_GROUP);

			readL = GetPrivateProfileString(SEC_GROUPORDER, "00", "", readB, sizeof(readB), filePath);
			if (readL <= 0)	return;

			string = CString(readB, readL);

			while (!string.IsEmpty())
			{
				gno = IH::Parser(string, ";");
				if (gno.IsEmpty() || gno.GetLength() != 2)
					break;

				readL = GetPrivateProfileString(SEC_GROUPNAME, gno, "", readB, sizeof(readB), filePath);
				gname = CString(readB, readL);
				kind = MAKE_TREEID(treeID->kind, 0, 2, atoi(gno), TD_ITEM);
				if (ExistFile(atoi(gno)))
					SetItemData(InsertItem(gname, 0, 1, hItem, nullptr), kind);
			}
		}
		break;
	case xREMAIN:
		{
			const CStringArray* pArr = m_accn.get_account();
			
			for(int i=0 ; i<pArr->GetSize() ; i++)
			{
				CString strAccn = pArr->GetAt(i);

				CString strText = strAccn.Left(3) + "-" + strAccn.Mid(3,2) + "-" + strAccn.Right(6);
				CString strT = strAccn.Mid(3,1);
				
				kind = MAKE_TREEID(treeID->kind, 0, 2, i, TD_ITEM);

				if( strT != "5" && strT != "0" )
				{
					SetItemData(InsertItem(strText, 0, 1, hItem, nullptr), kind);
				}
			}
		}
		break;
	case xKOSPIex:
		{
			for (int ii = 0; ii < cnt_exKospi; ii++)
			{
				kind = MAKE_TREEID(treeID->kind, 0, 3, atoi(exKospi[ii].code), TD_ITEM);
				SetItemData(InsertItem(exKospi[ii].name, 0, 1, hItem, nullptr), kind);
			}
		}
		break;

	case xKOSDAQex:
		{
			for (int ii = 0; ii < cnt_exKosdaq; ii++)
			{								
				kind = MAKE_TREEID(treeID->kind, 0, 3, atoi(exKosdaq[ii].code), TD_ITEM);
				SetItemData(InsertItem(exKosdaq[ii].name, 0, 1, hItem, nullptr), kind);
			}
		}
		break;

	case xKOSPIUP:

	case xKOSDAQUP:
		{
			CString name;
			struct	upcode	upcode;
			const int	jgub = (treeID->kind == xKOSPIUP) ? upKOSPI : upKOSDAQ;
			if (m_upcode.GetSize() == 0) loadingUPcode();
			for (int ii = 0 ; ii < m_upcode.GetSize() ; ii++)
			{
				upcode = m_upcode.GetAt(ii);
				if (upcode.jgub == jgub)
				{
					name = CString(upcode.hnam, UNameLen);
					name.TrimLeft(); name.TrimRight();
					kind = MAKE_TREEID(treeID->kind, 0, 2, (UINT)upcode.ucod, TD_ITEM);
					SetItemData(InsertItem(name, 0, 1, hItem, nullptr), kind);
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
				SetItemData(InsertItem(_T(key), 0, 1, hItem, nullptr), kind);
			}
		}
		break;
	}
}

bool CTreeWnd::ExistFile(int gno)
{
	CString	filePath;

	filePath.Format("%s/%s/%s/portfolio.i%02d", m_root, USRDIR, m_user, gno);

	CFileFind finder;
	if (!finder.FindFile(filePath))
		return false;

	return true;
}

BOOL CTreeWnd::CheckStock(UINT kind, _shjcode* hjcode)
{
	BOOL	ret = FALSE;
	const struct _treeID* treeID = CAST_TREEID(kind);
	const UINT	value = treeID->value;
	switch (treeID->kind)
	{
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
			case jmHYFND:
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
					if ((UINT)hjcode->ucds == value || (UINT)hjcode->ucdm == value)
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
				else if (value < 15 || value == 41)
				{
					if ((UINT)hjcode->ucdm == value)
						ret = TRUE;
				}
				else if (value < 38)
				{
					if ((UINT)hjcode->ucds == value)
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

/*
void CTreeWnd::GetOption(class CGridData& sdata, int max)
{
	struct ojcode	ojcode;
	CString code = _T(""), name = _T("");
	if (m_ojcode.GetSize() == 0) loadingOJcode();
	struct _treeID	treeID = CONVERT_TREEID(sdata.GetKind());
	
	//m_ojcode 행사가 갯수
	for (int ii = 0; ii < m_ojcode.GetSize(); ii++)
	{
		ojcode = m_ojcode.GetAt(ii);

		for (int jj = 0; jj < STANDARDNUM; jj++)
		{
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
	}
}
*/

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
			else
			{
				code = exKosdaq[ii].code;
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
	loadingELWBase();

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
	else if (code[0] == '1' || code[0] == '4'|| code[0] == 'A' || code[0] == 'D')	// future code  //파생상품 코드개편
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
	else if (code[0] == '2' || code[0] == '3' || code[0] == 'B' || code[0] == 'C')  //파생상품 코드개편
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
					if (code != CString(ojcode.call[jj].cod2, OCodeLen -1))
						continue;

					return CString(ojcode.call[jj].hnam, ONameLen);
				}
				for (int jj = 0; jj < STANDARDNUM; jj++)
				{
					if (ojcode.put[jj].yorn != '1')
						continue;
					if (code != CString(ojcode.put[jj].cod2, OCodeLen -1))
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

// void CTreeWnd::sendUpjong()
// {
// 	class CGridData sdata;
// 
// 	CString upjn;
// 
// 	upjn = "001";
// 	sdata.m_arDatas.Add(MakePacket(upjn, "", "", "", ""));
// 
// 	upjn = "201";
// 	sdata.m_arDatas.Add(MakePacket(upjn, "", "", "", ""));
// 	
// 	m_pMainWnd->SendMessage(WM_MANAGE, MK_UPJONGDATA, (LPARAM)&sdata);
// }


void CTreeWnd::GetInterest(class CGridData& sdata, int max)
{
	
}

int CTreeWnd::GetIndexinGroup(int pageView)
{
	int result = -1;

	if(pageView == 1)
		result = 0;
	else if(pageView == 2)
		result = 20;
	else if(pageView == 3)
		result = 40;
	else if(pageView == 4)
		result = 60;
	else if(pageView == 5)
		result = 80;

	return result;
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

BOOL CTreeWnd::SendOper(UINT kind, int opt)
{
	const BOOL ret = TRUE;
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	
	class CGridData	sdata;
	sdata.SetKind(kind);
	const int	max = pWnd->SendMessage(WM_MANAGE, MK_GETMAX);
	GetData(sdata, max, opt);
	
	if (ret)
		SendTreeData(sdata);
	return ret;
}

void CTreeWnd::RefreshInterest()
{
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	
	class CGridData	sdata;
	sdata.SetKind(xINTEREST);
	const int	max = pWnd->SendMessage(WM_MANAGE, MK_GETMAX);
	GetData(sdata, max, 0);

	m_pMainWnd->SendMessage(WM_MANAGE, MK_SAVEFOROUB, (LPARAM)&sdata);
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
	const DWORD	dwData = 0;
	m_mapParam.RemoveKey(dwData);
	return CTreeCtrl::DeleteItem(hItem);
}

BOOL CTreeWnd::DeleteAllItems()
{
	m_mapParam.RemoveAll();
	return CTreeCtrl::DeleteAllItems();
}

// MODIFY PSH 20070914
//CString CTreeWnd::MakePacket(CString& code, CString amount, CString price)
CString CTreeWnd::MakePacket(CString& code, CString amount, CString price, CString name, CString bookmark, CString futureGubn, CString creditPrc, CString maeipPrc, CString bookrgb, CString MemoYN)
// END MODIFY
{
	code.TrimLeft(); 
	code.TrimRight();
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
	/*
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

	code += "\t";  //test IO
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


void CTreeWnd::GetData(class CGridData& sdata, int max, int opt)
{
	//struct _treeID treeID = CONVERT_TREEID(sdata.GetKind());		
	((CMainWnd*)m_pMainWnd)->Request_GroupCode(sdata.GetKind());

}

LRESULT CTreeWnd::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	return CTreeCtrl::WindowProc(message, wParam, lParam);
}

void CTreeWnd::queryGroup()
{
/*
	_gmid	gmid;
	gmid.xflg  = '1';				// 조회
	sprintf(gmid.usrid, "%12s", m_id);
	sprintf(gmid.grec, "0000");
	gmid.sflag = '1';				// 현물 조회

	CString senddata = CString((char*)&gmid, L_gmid);
	CSendData	sdata;
	sdata.SetData("PIIOSETA", TRKEY_GROUPACCNLIST, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);
*/
	struct _gmid gmid;
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

	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
	pView->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);

	if (!strSvcn.IsEmpty())
		CopyMemory(&ledger.svcd, (LPCTSTR)strSvcn, sizeof(ledger.svcd));

	if (!strLastKey.IsEmpty())
		CopyMemory(&ledger.next, strLastKey, strLastKey.GetLength());

	return CString((char*)&ledger, L_ledgerH);
}

void CTreeWnd::queryAcc()
{
//	EnableWindow(FALSE);

/* @@ Zeta : 고객도 Group을 설정 할 수 있도록 수정되었기 때문에 queryGroup으로 변경
	if (m_bCustomer)
	{
		struct _acSet	acSet;
		memset(&acSet, ' ', L_acSet);
		acSet.func[0] = 'Q';
		CopyMemory(acSet.usid, m_id, m_id.GetLength());
		CString senddata = CString((char*)&acSet, L_acSet);
		CSendData	sdata;
		sdata.SetData("piioaccn", TRKEY_ACCCUSTOM, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");
		m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sdata);		
	}
	else
	{
		// DEL PSH 20071206
		
		if (m_id.IsEmpty() || m_pass.IsEmpty())
			return;

		// 20071228 cej 직원은 그룹계좌만 가져오도록 수정
		queryGroup();
		/*
		CString sData = _T("");
		if (m_gubn == 'D')
			sData = getLedger("2057", 'Q', "SI511101_Q");
		else
			sData = getLedger("2057", 'Q', "SI511100_Q");
		
		queryStaffAcc(sData);
		*/
		// END DEL
//	}

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
	char*	buf = (char*)data->m_lParam;
	const _gmod *gmod = (_gmod *)buf;
	
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
			hItem = InsertItem(sztmp, hMid, nullptr);
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
	if(kind == 0)
		kind = 4;

	switch (kind)
	{
	//
	case TRKEY_GROUPACCNLIST:
		oubGroup(data);
		break;
	case TRKEY_ACCCUSTOM:
		oubAccUser((char*)data->m_lParam);
		break;
	case TRKEY_ACCSTAFF:
		oubAccList((char*)data->m_lParam, HIWORD(data->m_wParam));
		break;
	case TRKEY_REMAIN:
		{
			struct	_extTHx* th = (struct _extTHx*)data->m_lParam;
			oubRemain(th->data, th->size);
		}
		
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
	}

}

void CTreeWnd::oubAccUser(char* buf)
{
	const struct _acSet* acSet  = (struct _acSet*) buf;

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
	HTREEITEM	hRoot = nullptr;
	//hItem = nullptr;
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
	/*
	arAccn.QuickSort();

	for (int ii = 0; ii < arAccn.GetSize(); ii++)
	{
		acInfo = arAccn.GetAt(ii);
		infoS = makeAccn(acInfo.code, acInfo.name);
		hItem = InsertItem(infoS, hRoot, nullptr);
		SetItemData(hItem, MAKE_TREEID(xREMAIN, 0, 0, ii++, TD_ITEM));	
	}
	*/
}

void CTreeWnd::queryNews()
{
	struct  mid {
	    char    func[1];        /* '1':최근100종목      */
	    char    date[8];        /* 날짜                 */
	} mid;

	memset(&mid, ' ', sizeof(mid));
	mid.func[0] = '1';
	CString senddata = CString((char*)&mid, sizeof(mid));
	CSendData	sdata;
	sdata.SetData("PIBO2022", TRKEY_NEWS, (char*)senddata.operator LPCTSTR(), senddata.GetLength(), "");	//2012.07.17 KSJ pibo --> pibf 2013.08.05 원복
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
		char flag[1];	
		GRID_O  grid_o;
		char nrec[4];				//gr7id row 개수
		struct grid grid[1];
	}*mod{};

	char* buf = (char*)data->m_lParam;
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
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
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
		char flag[1];	
		GRID_O  grid_o;
		char nrec[4];				//gr7id row 개수
		struct grid grid[1];
	}*mod{};
	
	char* buf = (char*)data->m_lParam;
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
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)kind);
}

void CTreeWnd::parsingNews(CRecvData* data)
{
	struct grid
	{
		char cod2[12]{};
	};

	struct  mod 
	{
	    char    nrec[4]{};
	    struct grid	grid[1]{};
	}*mod{};

	char*	buf = (char*)data->m_lParam;
	mod = (struct mod*)buf;
	CString	nrec, code;
	int	ncnt = 0;
	const int	nsize = sizeof(struct grid);
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
			if (code.GetAt(0) == 'A')
				code = code.Mid(1);
			sdata.m_arDatas.Add(code);
		}
	}
	
	SendTreeData(sdata);
	const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
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
	const CWnd* pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
	pWnd->SendMessage(WM_MANAGE, MK_GROUPSAVE, (LPARAM)index);
}

void CTreeWnd::OnClick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;

	CPoint	pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	HTREEITEM	hItem = HitTest(pt);
	if (hItem == nullptr)
		return;
	
	CString	text = GetItemText(hItem);
	CRect	rect;
	GetItemRect(hItem, rect, TRUE);
	
	if (!rect.PtInRect(pt))
		return;
	
	const UINT	selItem = GetItemData(hItem);
	const struct _treeID*	treeID = CAST_TREEID(selItem);

	//전에 관심종목을 클릭한 상태에서만 저장여부 판단
	if(m_selectTreetype == xINTEREST || m_selectTreetype == 0)
	{
		if(m_selectTree != -1)
		{
			saveGroupwhenClose(m_selectTree);
		}
	}
	
	m_selectTree = treeID->value - 1;
	m_selectTreetype = treeID->kind;

	CString string;
	string = "";

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
	case xRECOMMAND:
		clickRecommand();
		//sendUpjong();
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
					CString log;
					log.Format("OnClick LOG : %d",selItem);
					m_kind = selItem;
				}
				break;
			default:
				opt = (int)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SEARCHOPT, MO_GET));
				break;
			}
	
			if (SendOper(selItem, opt))
			{
				const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
				pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)selItem);	
			}
		}		

		break;
	}

}


BOOL CTreeWnd::IsNumber(CString str)
{
    BOOL bRet = TRUE;
    const  int nLen = str.GetLength();
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

void CTreeWnd::SendTotalRemain(CString strAccount)
{
	m_arJango.RemoveAll();

	if(strAccount.GetLength() != 11)
	{
		return;
	}
	
	CString tempAccount = strAccount;
	
	strAccount.Remove(_T('-'));
	strAccount.TrimLeft();
	strAccount.TrimRight();
	
	CString pswd, acctdata, strMember;
	bool bIsMember;
	
	acctdata = Variant(accountCC, "");
	strMember = Variant(userCC, "");
	bIsMember = FALSE;

	pswd = m_accn.loadPass(acctdata,strAccount);
	
	//직원 고객 구분
	if(IsNumber(strMember))
	{
		pswd = m_accn.loadPass(acctdata,strAccount);
		if(pswd == "")
		{
			pswd = ((CMainWnd*)m_pMainWnd)->m_Pass;
		}
	}
	else
	{
		pswd = "9999";
	}
	if(strAccount.GetLength() > 0 && strAccount.Mid(3, 1) == "2")
	{
		queryFutureRemain2(strAccount, pswd);
	}
	else
	{
		queryRemain2(strAccount, pswd);
	}
}

void CTreeWnd::clickRemain(UINT	selItem)
{
	CString log;
	log.Format("OnClick LOG : %d",selItem);
	m_kind = selItem;
	m_arJango.RemoveAll();

	HTREEITEM hItem = nullptr;
	
	m_mapParam.Lookup(m_kind, hItem);
	CString strAccount(_T(""));
	
	if (hItem == nullptr)
		return;
	
	const struct _treeID*	treeID = CAST_TREEID(m_kind);
	
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

void CTreeWnd::SendInterest(int item)
{
	const int opt = 0;
//	const UINT	selItem = GetItemData(hItem);
	
	CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
	const int nOver = (int)pWnd->SendMessage(WM_MANAGE, MK_INPUTOVER);
	
	//MO_VISIBLE모드일때는 항상 맨 앞을 선택하도록 한다
	if(nOver == MO_VISIBLE)
	{
		pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_GROUP));
		pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)0);
	}	
	
	if (SendOper(item, opt))
	{
		const CWnd* ptoolWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		ptoolWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)item);	
	}
}

//관심그룹 index 저장
void CTreeWnd::saveGroupIndex(int index)
{
//	CString	str, m_fileCFG;

//	str.Format("%02d", index);
//	m_fileCFG.Format("%s\\%s\\%s\\%s", m_root, USRDIR, m_user, FILE_CONFIG);
//	WritePrivateProfileString(SEC_MAIN, KEY_SELECTGROUP, str.operator LPCTSTR(), m_fileCFG);
}


void CTreeWnd::queryFutureRemain(CString strAccount)
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
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
//	CAccn *pAccn = nullptr;
//	CString sPswd = "9999";
	const int skey=-1;
	
	//pAccn = new CAccn;
	//pAccn->m_accn = strAccount;
	//pAccn->m_bFuture = TRUE;
	//pAccn->m_pswd = sPswd;
	//pAccn->m_bCredit = FALSE;
	//pAccn->m_dFee = 0;//buffet
	//pAccn->m_dMass = 0;
	//pAccn->m_dSave = 0;

	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
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
//	CAccn *pAccn = nullptr;
////	CString sPswd = "9999";
//	const int skey=-1;
//	
//	pAccn = new CAccn;
//	pAccn->m_accn = strAccount;
//	pAccn->m_bFuture = FALSE;
//	pAccn->m_pswd = sPswd;
//	pAccn->m_bCredit = TRUE;
//	pAccn->m_dFee = 0;//buffet
//	pAccn->m_dMass = 0;
//	pAccn->m_dSave = 0;
	
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
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
/*	
	CString passX;
	passX.Format("%s\t%s", sPswd, strAccount.Left(8));
	passX = (char *)m_pMainWnd->SendMessage(WM_USER, MAKEWPARAM(encPASSx, modeACN), (LPARAM)(LPCTSTR)passX);
	sPswd = passX;
*/	
	struct s_mid mid;
	
	FillMemory(&mid, L_smid, ' ');
	CopyMemory(&mid.accn, strAccount, sizeof(mid.accn));
	CopyMemory(&mid.pswd, sPswd, sPswd.GetLength());
	mid.allf[0] = '1';
	
	CString strSendData(_T(""));
	CString strMid = CString((char*)&mid, L_smid);
	strSendData = strLedger + strMid;
	
	CSendData sData;
	//sData.SetData("PIBOSJGO", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");
	sData.SetData("PIBOSJG2", TRKEY_REMAIN, (char*)strSendData.operator LPCTSTR(), strSendData.GetLength(), "");  //20200423 잔고조회TR변경
	m_pMainWnd->SendMessage(WM_MANAGE, MK_SENDTR, (LPARAM)&sData);

//	delete pAccn;
}


void CTreeWnd::queryRemain(CString strAccount)
{
	struct _ledgerH ledger;
	
	FillMemory(&ledger, L_ledgerH, ' ');
	
	const CWnd*	pView = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_VIEW));
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

	//CGridData	sdata;

	//sdata.SetKind(m_kind);
	m_pSData.SetKind(m_kind);

	CString strMsg = "ERR\t" + strErrText;
	if (strErrCode.GetAt(0) != _T('0'))	// 오류
	{
		strMsg.Format("[%s]%s", strErrCode, strErrText.Right(strErrText.GetLength() - 4));
		if(strErrCode.Find("4589") >= 0 )	
			MessageBox(strMsg, "IBK투자증권");
	}
	else
	{

		buf += L_ledgerH;
		
		if(strServiceName == FutureService)		//선물잔고
		{
			settingDataFuture2(buf, m_pSData);	
		}
		else			//현물잔고
		{
			settingDataStock2(buf, m_pSData);
		}
		
		const CWnd*	pWnd = (CWnd*)m_pMainWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_GETWND, MO_TOOL));
		pWnd->SendMessage(WM_MANAGE, MAKEWPARAM(MK_SELGROUP, MO_SET), (LPARAM)m_kind);
	}

	m_nAccountIndex++;
	if(m_nAccountIndex == m_accnT.GetSize())
	{
		SendTreeData(m_pSData);
	}
	else
	{
		MakeTotalRemain(m_nAccountIndex);
	}
}

void CTreeWnd::settingDataStock(char* buf, class CGridData& sdata)
{
	struct jango_mod mod {};
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	CString strTotalCode;
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	CString qty, AvrUprc, loanDtlcode, creditPrc, maeipPrc;
	int iQty{}, icreditPrc{}, iMaeipPrc{};
	double dPrice{};
	CString result_qty, result_Price, result_CPrice, result_MPrice;
	
	for (int i = 0 ; i<nRec ; i++)
	{
		const struct out_rec rec = mod.grid[i];
		CString strCode(rec.code, sizeof(rec.code));
		strCode.TrimRight();
		
		if (!strCode.IsEmpty())
		{
			if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J'))
			{
				if(strlen(strCode) == 7)
					strCode.Delete(0);
			}
			qty = CString(rec.dealbaseQty, sizeof(rec.dealbaseQty));
			AvrUprc = CString(rec.AvrUprc, sizeof(rec.AvrUprc));
			loanDtlcode = CString(rec.loanDtlcode, sizeof(rec.loanDtlcode));
			creditPrc = CString(rec.creditAmt, sizeof(rec.creditAmt));
			maeipPrc = CString(rec.buyAmt, sizeof(rec.buyAmt));

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
				
				strTotalCode.Format("%s%s\t",strTotalCode,strCode);
				//패킷생성(종목번호, 잔고, 매입단가)
				sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", "", result_CPrice, result_MPrice));
			}

		}
	}	


	m_pMainWnd->SendMessage(WM_MANAGE,MK_BOUCODE,(LPARAM)strTotalCode.operator LPCTSTR());
}

void CTreeWnd::settingDataStock2(char* buf, class CGridData& sdata)
{
	struct s_mod	mod {};
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	CString strTotalCode;

	strTotalCode = "";
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	CString qty, AvrUprc, loanDtlcode, creditPrc, maeipPrc;
	int iQty{}, icreditPrc{}, iMaeipPrc{};
	double dPrice{};
	CString result_qty, result_Price, result_CPrice, result_MPrice;

	 if (nRec > 100)
	 {
	 	nRec = 100;
// 		SetTimer(TM_BALANCE100, 1000, nullptr);
	 }

	for (int i = 0 ; i<nRec ; i++)
	{
		const struct s_rec rec = mod.rec[i];
		CString strCode(rec.cod2, sizeof(rec.cod2));
		strCode.TrimRight();
		

		if (!strCode.IsEmpty())
		{
		//	if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J')) //test
			if (strCode.GetAt(0) == _T('A') || strCode.GetAt(0) == _T('J') || strCode.GetAt(0) == _T('Q')) //202004 ETN 종목
			{
				if(strlen(strCode) == 7)
					strCode.Delete(0);
			}
			
			qty = CString(rec.jqty, sizeof(rec.jqty));
			AvrUprc = CString(rec.pamt, sizeof(rec.pamt));
			loanDtlcode = CString(rec.sycd, sizeof(rec.sycd));
			creditPrc = CString(rec.samt, sizeof(rec.samt));
			maeipPrc = CString(rec.mamt, sizeof(rec.mamt));
			
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
				result_Price.Format("%.0f", dPrice);
				result_CPrice.Format("%d", icreditPrc);
				result_MPrice.Format("%d", iMaeipPrc);
				
			//	strTotalCode.Format("%s%s\t",strTotalCode,strCode);  //test
				strTotalCode += "\t";
				strTotalCode += strCode;

				//패킷생성(종목번호, 잔고, 매입단가)
				sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", "", result_CPrice, result_MPrice));
			}
			
		}
	}	


	m_pMainWnd->SendMessage(WM_MANAGE,MK_BOUCODE,(LPARAM)strTotalCode.operator LPCTSTR());

}

double CTreeWnd::Round(double data ) //(반올림)
{
    return (int)(data + 0.5);
}

void CTreeWnd::settingDataFuture(char* buf, class CGridData& sdata)
{
	struct future_mod mod {};
	
	int sizeJango = 0;
	int nRec = 0;
	const int sBuf = strlen(buf);
	
	sizeJango = sizeof(mod);
	CopyMemory(&mod, (void*)buf, min(sizeJango, sBuf));
	nRec = atoi((LPCTSTR)CString(mod.nrec, sizeof(mod.nrec)));
	
	CString qty, AvrUprc, gubn, maeipPrc, pyungaPrc, sonikPrc;
	int iQty = 0;

	double dPrice = 0;
	CString result_qty, result_Price, result_maeipPrice;
	double dPyungaPrice{}, dmaeipPrice{}, dsonikPrc{};

	for (int i = 0 ; i<nRec ; i++)
	{
		const struct out_grid rec = mod.grid[i];
		CString strCode(rec.zFnoIsuNo, sizeof(rec.zFnoIsuNo));
		strCode.TrimRight();
		
		if (!strCode.IsEmpty())
		{
			qty = CString(rec.lLqdtOrdAbleQty, sizeof(rec.lLqdtOrdAbleQty));
			AvrUprc = CString(rec.dAvrUprc, sizeof(rec.dAvrUprc));
			gubn = CString(rec.zBnsTp, sizeof(rec.zBnsTp));
			pyungaPrc = CString(rec.lFnoEvalAmt, sizeof(rec.lFnoEvalAmt));
			sonikPrc = CString(rec.lEvalPnl, sizeof(rec.lEvalPnl));
			maeipPrc = CString(rec.lUnsttAmt, sizeof(rec.lUnsttAmt));

			if(qty.Left(1) == '+')
				qty.Delete(0);
			
			if(AvrUprc.Left(1) == '+')
				AvrUprc.Delete(0);
			
			if(pyungaPrc.Left(1) == '+')
				pyungaPrc.Delete(0);

			if(maeipPrc.Left(1) == '+')
				maeipPrc.Delete(0);

			iQty = atoi(qty);
			dPrice = atof(AvrUprc);
			dPyungaPrice = atof(pyungaPrc);
			dsonikPrc = atof(sonikPrc);
			dmaeipPrice = atof(maeipPrc);

			result_qty.Format("%d", iQty);
			result_Price.Format("%.2f", dPrice);
			result_maeipPrice.Format("%.2f", dmaeipPrice);

			//패킷생성(종목번호, 잔고, 매입단가)
			sdata.m_arDatas.Add(MakePacket(strCode, result_qty, result_Price, "", "", gubn, "", result_maeipPrice));
			
		}
	}		
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
	
	for (int i = 0 ; i<nRec ; i++)
	{
		const struct f_rec rec = mod.rec[i];
		CString strCode(rec.cod2, sizeof(rec.cod2));
		strCode.TrimRight();
		
		if (!strCode.IsEmpty())
		{
			qty = CString(rec.bqty, sizeof(rec.bqty));
			AvrUprc = CString(rec.curr, sizeof(rec.curr));
			gubn = CString(rec.dsgb, sizeof(rec.dsgb));
			maeipPrc = CString(rec.tamt, sizeof(rec.tamt));
			
			if(qty.Left(1) == '+')
				qty.Delete(0);
			
			if(AvrUprc.Left(1) == '+')
				AvrUprc.Delete(0);
			
			if(maeipPrc.Left(1) == '+')
				maeipPrc.Delete(0);

			if(gubn.Left(1) == '+' || gubn.Left(1) == '-')
				gubn.Delete(0);
			
			gubn.TrimLeft();gubn.TrimRight();

			if(gubn == "매수")
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

void CTreeWnd::oubRemainEx(char* buf, int len)
{
	CString datS = CString(buf, len);
	CString infoS, code, num, price, sztmp, sJango;
	HTREEITEM	hRoot = nullptr;// , hItem = nullptr;
	m_mapParam.Lookup(xREMAIN, hRoot);

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
	const int nPos = accn.Find("-");
	
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


void CTreeWnd::receiveOub(CString& data, int key)
{	
	CString strtmp;
//m_slog.Format("\r\n[interest][memo_cx_interest] -------------------------------------");
//OutputDebugString(m_slog);
//m_slog.Format("\r\n[interest][memo_cx_interest][%s]", data.Left(22));
//OutputDebugString(m_slog);
	CGridData sdata;
	
	sdata.m_kind = 0;
	const CString skey = data.Left(2);
	const CString sGroupName = data.Mid(2, 20);

	((CMainWnd*)m_pMainWnd)->_groupName = sGroupName;
	const CString sCount = data.Mid(22, 4);
	data = data.Mid(26);

	constexpr UINT newbooksize = sizeof(_bookmarkinfo);

	UINT	fileSize = 0;
	UINT	readL{};
	int     count = 0;
	CFile   fileB;
	CString sBookBuffer;
	CString filePath;
	CString	code, amount, price, name, bookmark, bookrgb, MemoYN;

	filePath.Format("%s/%s/%s/bookmark.i%0*d", m_root, USRDIR, m_user, 2, key);
	if (fileB.Open(filePath, CFile::modeRead | CFile::shareDenyNone))
	{
		fileSize = gsl::narrow_cast<UINT>(fileB.GetLength());
		readL = fileB.Read(sBookBuffer.GetBuffer(fileSize + 1), fileSize);
		sBookBuffer.ReleaseBuffer();
		if (readL != fileSize)
			return;

		// m_slog.Format("\r\n[interest][cx_interest][%s] 파일이 있다", filePath);

		// OutputDebugString(m_slog);
		fileB.Close();
	}
	else
	{
		// m_slog.Format("\r\n[interest][cx_interest][%s] 파일이 없다", filePath);
		// OutputDebugString(m_slog);
	}

	if (fileSize)
		count = fileSize / sizeof(struct _bookmarkinfo);
	const gsl::span<struct _bookmarkinfo> bookspan((struct _bookmarkinfo*)sBookBuffer.GetString(), count);
	const gsl::span<struct _jinfo_MEMO> codelist((struct _jinfo_MEMO*)data.GetString(), atoi(sCount));

	int ii = 0;
	for_each(codelist.begin(), codelist.end(), [&](auto item) {

		if (bookspan.size() > ii)
			bookmark = CString(bookspan.at(ii).bookmark[0], 1);
		else
			bookmark = "0";

		//test mod 서버에서 받은 북마크
		if (CString(item.gubn, 1).Trim() == "9")
			bookmark = "1";

		code = CString(item.code, codelen).Trim();
		name = GetCodeName(CString(item.code, codelen).Trim());
		if (code.IsEmpty())
			code = "emptyrow";
	//		code = "          ";  //test 20230209

		if (code[0] == 'm' &&  bookspan.size() > ii)
		{
			CString bookCode = CString(bookspan.at(ii).code, codelen).Trim();

// m_slog.Format("\r\n[interest][cx_interest] bookCode=[%s] code=[%s] ", bookCode, code);
// OutputDebugString(m_slog);

			if (code.CompareNoCase(bookCode) == 0)
			{
				if (bookspan.size() > ii)
					name = CString(bookspan.at(ii).name, namelen).Trim();
			}
			/*for (auto& bookmark : bookspan)
			{
				strtmp.Format(".12%", bookmark.code);
				strtmp.Trim();
				if (strtmp == code)
				{
					name = CString(bookmark.name, namelen).Trim();
					break;
				}
			}*/
	

			if (name.Find("0  ") == 0)
				name.Empty();
		}
		else if (code[0] == 'm')  //test mod
		{ //북마크 파일이 없는데 서버에 내려받는 종목코드 정보가 책갈피일때
			//struct _jinfo_MEMO st_jinfo;
			name = CString(item.xprc, 10).Trim();
			if (name.Find("0  ") == 0)
				name.Empty();
		}

		amount = CString(item.xnum, xnumlen).Trim();
		price = CString(item.xprc, pricelen).Trim();
		bookrgb = CString(item.bookrgb, 2).Trim();  //test IO
		MemoYN = CString(item.mflg, 1).Trim();  //test memo

// m_slog.Format("\r\n[interest][cx_interest] ii=[%d] g=[%s] c=[%s] n=[%s] bookmark=[%s] p=[%s] a=[%s] bookrgb=[%.2s]", ii, CString(item.gubn, 1).Trim(), code, name, bookmark, price, amount, bookrgb);
// OutputDebugString(m_slog);

		sdata.m_arDatas.Add(MakePacket(code, amount, price, name, bookmark, "", "", "", bookrgb, MemoYN));
		if (sdata.GetCount() == 100)
			return;
		ii++;
	});

	SendTreeData(sdata);
}

void CTreeWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch (nIDEvent)
	{
	case TM_BALANCE100:
	{
		// KillTimer(nIDEvent);
		// CString sval;
		// sval = _T("보유종목은 100개 까지 조회됩니다.");
		// m_pMainWnd->SendMessage(WM_MANAGE, MK_SHOWTOOLTIP, (LPARAM)sval.operator LPCSTR());
	}
	break;
	}
	CTreeCtrl::OnTimer(nIDEvent);
}
