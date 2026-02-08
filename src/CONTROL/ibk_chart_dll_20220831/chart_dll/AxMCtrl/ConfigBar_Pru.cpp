// ConfigBar_Pru.cpp : implementation file
//

#include "stdafx.h"
#include "ConfigBar_Pru.h"
#include "ConfigBar_TR.h"

//#include "../../../../h/axisvar.h"
//#include "../../../../h/axisfire.h"
//#include "../../../../h/axisgwin.h"
//#include "../../../../h/jmcode.h"
//#include "../../../../h/interSt.h"
//#include "../../../../h/ledger.h"
#include "../../h/axisvar.h"
#include "../../h/axisfire.h"
#include "../../h/axisgwin.h"
#include "../../h/jmcode.h"
#include "../../h/interSt.h"
#include "../../h/ledger.h"

#include "qsort.h"
#include <afxtempl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const char* __menuNames[] = {
	"즐겨찾기", "추세지표", "모멘텀지표", "거래량지표", "시장강도지표", "채널지표", "기타지표", NULL
};

CHARTKIND	__item[] = {
	{"가격 이평", GK_PMA, JP_TREND},	
	{"그물차트", GK_NET, JP_TREND}, 
	{"A/D Line",	GK_ADLINE, JP_TREND},	
	{"ADX",	GK_ADX, JP_TREND},	
	{"CCI",	GK_CCI, JP_TREND},	
	{"DMI",	GK_DMI, JP_TREND},
	{"Demark", GK_DEMARK, JP_TREND},	
	{"MACD", GK_MACD, JP_TREND},	
	{"MACD Oscillator", GK_MACDOS, JP_TREND}, 
	{"Pivot Line", GK_PIVOT, JP_TREND}, 
	{"Price Oscillator", GK_PMAO, JP_TREND},	
	{"RSI", GK_RSI, JP_TREND},
	{"RSI(가중치)", GK_RSIWEIGHT, JP_TREND},	// 20090923 : ADD : RSI Weight 지표 추가
	{"TRIX", GK_TRIX, JP_TREND}, 
	{"William's %R", GK_WPR, JP_TREND},
	{"Parabolic", GK_PARB, JP_TREND},

	{"이격도", GK_GAP, JP_MOMENTUM},	
	{"모멘텀", GK_MOMENTUM, JP_MOMENTUM}, 
	{"ABRatio", GK_ABRATIO, JP_MOMENTUM},
	{"Band Width", GK_BANDWIDTH, JP_MOMENTUM},
	{"Chaikin's Oscillator", GK_CO, JP_MOMENTUM}, 
	{"Chaikin's Volatility", GK_CV, JP_MOMENTUM}, 
	{"Ease of Movement", GK_EMV, JP_MOMENTUM},	
	{"LFI", GK_LFI, JP_MOMENTUM},
	{"Net Change Oscillator", GK_NCO, JP_MOMENTUM},
	{"Price Rate-of-Change", GK_PROC, JP_MOMENTUM},
	{"Sigma", GK_SIGMA, JP_MOMENTUM}, 
	{"SONAR", GK_SONA, JP_MOMENTUM},	
	{"SONA Momentum", GK_SONAMOMENTUM, JP_MOMENTUM}, 
	{"Slow Stochastics", GK_STOCHS, JP_MOMENTUM},
	{"Fast Stochastics", GK_STOCHF, JP_MOMENTUM},

	{"거래량", GK_VOL, JP_VOLUME},	
	{"거래량 이평", GK_VMA, JP_VOLUME},	
	{"회전율", GK_RRATE, JP_VOLUME},
	{"거래대금", GK_AMT, JP_VOLUME}, 
	{"거래대금 이평", GK_AMA, JP_VOLUME},		
	{"ADR", GK_ADR, JP_VOLUME},
	{"Money Flow Index", GK_MFI, JP_VOLUME},	
	{"OBV",	GK_OBV, JP_VOLUME},	
	{"Volume Oscillator", GK_VMAO, JP_VOLUME},	
	{"VR", GK_VR, JP_VOLUME},
	{"Volume Rate-of-Change", GK_VROC, JP_VOLUME},

	{"심리도", GK_PSY, JP_MARKET},
	{"신심리도", GK_NPSY, JP_MARKET},		

	{"Bollinger Band", GK_BOLB, JP_CHANNEL}, 
	{"Envelope", GK_ENVL, JP_CHANNEL},

	{"외국인보유(수량)", GK_FRGNHAVE, JP_ETC}, 
	{"외국인보유(비율)", GK_FRGNRATE, JP_ETC},
	{"개인순매수(수량)", GK_PSNLPBUY, JP_ETC}, 
	{"외국인순매수(수량)", GK_FRGNPBUY, JP_ETC},
	{"기관순매수(수량)", GK_ORGNPBUY, JP_ETC}, 
	{"개인누적순매수(수량)", GK_PSNLTBUY, JP_ETC},
	{"외국인누적순매수(수량)", GK_FRGNTBUY, JP_ETC}, 
	{"기관누적순매수(수량)", GK_ORGNTBUY, JP_ETC},

};

struct _infop {
	char* code;
	char* name;
} infop[] = {
	{"101", "KOSPI200"},	{"102", "KP200제조"},   {"103", "KP200전기통신"},	
	{"104", "KP200건설"},	{"105", "KP200유통"},	{"106", "KP200금융"},
	{"401", "KOSPI100"},	{"402", "KOSPI50"}
//	,{"403", "KOSPI IT"},	modify 20070801
//	,{"888", "바스켓"}
};

struct _infod {
	char* code;
	char* name;
} infod[] = {
	//{"301", "KOSDAQ50"},	{"302", "KOSDAQ IT"},	modify 20070801
	{"303", "KOSTAR"}
};

 
void ListInsertItem(CListCtrl* list, CString code, CString name, DWORD data);


CConfigBarPru::CConfigBarPru(CWnd* view, CWnd* parent, CFont* font, LPCTSTR info)
	:CAxMConfigBar(view, parent, font, info)
{
	m_root =	 (LPCTSTR)view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, homeCC));
	m_userName = (LPCTSTR)view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, nameCC));
	m_userID =   (LPCTSTR)view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC));
	
	m_tabFolder.Format("%s\\%s\\", m_root, TABDIR);
	m_userFolder.Format("%s\\%s\\", m_root, USRDIR);
	m_gexFolder.Format("%s\\%s\\", m_root, GEXDIR);
	m_imgFolder.Format("%s\\%s\\", m_root, IMAGEDIR);
	
	m_group = new CAxTargetGroupPru(parent, view, m_tabFolder, m_userFolder + m_userName + "\\");
	m_group->SetBar(this);
	m_trSender = NULL;

	m_requestKey = NULL;
	m_requestHaveItem = NULL;
}

CConfigBarPru::~CConfigBarPru()
{
	delete m_group;
}

BEGIN_MESSAGE_MAP(CConfigBarPru, CAxMConfigBar)
//{{AFX_MSG_MAP(CGrpView)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_USER, OnUserMessage)
END_MESSAGE_MAP()


void CConfigBarPru::GetMenunameBookmark(CString& menuname) 
{
	menuname = __menuNames[0];
}

void CConfigBarPru::GetMenunames(CStringList& strList)
{
	for (int i = 1; __menuNames[i]; i++) {
		strList.AddTail(CString(__menuNames[i]));
	}
}

void CConfigBarPru::GetChartKindList(CChartKindList& chartList)
{
	for (int i = 0; i < ARRAY_SIZE(__item); i++) {
		chartList.AddTail(&__item[i]);
	}
}

CString CConfigBarPru::GetRoot()
{
	return m_root;
}

CString CConfigBarPru::GetImagePath()
{
	return m_imgFolder;
}

CString CConfigBarPru::GetChartPath()
{
	return m_gexFolder;
}

CString CConfigBarPru::GetTabPath()
{
	return m_tabFolder;
}

CString CConfigBarPru::GetUserPath()
{
	return m_userFolder;
}

CString CConfigBarPru::GetUserName()
{
	return m_userName;
}

CString CConfigBarPru::GetUserID()
{
	return m_userID;
}


HTREEITEM CConfigBarPru::SetGroupList(CTreeCtrl* tree, LPCTSTR selectKind)
{
	m_group->SetTargetGroup(tree);
	return NULL;
}

void CConfigBarPru::SetGroupItem(CTreeCtrl* tree, HTREEITEM hItem)
{
	if (tree->ItemHasChildren(hItem)) return;
	
	DWORD param = tree->GetItemData(hItem);
	CListCtrl* list = (CListCtrl*)GetWindowLong(tree->m_hWnd, GWL_USERDATA);

	//KIND kind;
	CAxTargetItem::KeyToKind(param, m_lastKind);
	tree->SelectItem(hItem);
	//**if (list != m_listKind || m_lastKind.kind == CAxTargetGroupPru::groupUpDownRank) 
		m_group->SetTargetItem(tree, list, m_lastKind);
	//**else
		ProcessTargetItem();

	m_btnCheckAll->SetDown(TRUE);
	SetListKindItemCheck(TRUE);
}

void CConfigBarPru::LoadInterestItems(CAxMTreeCtrl* tree)
{
	CStringArray groups, codes, names;
	CByteArray kinds;
	HTREEITEM hItem;
	CString path = m_userFolder + m_userName + "\\", item;

	CAxTargetGroupPru::GetInterestGroupList(path, groups);

	for (int i = 0; i < groups.GetSize(); i++) 
	{
		if (groups.GetAt(i).IsEmpty()) continue;

		hItem = tree->InsertItem(groups.GetAt(i));

		CAxTargetGroupPru::GetInterestItemList(path, i, codes, names, kinds);

		for (int j = 0; j < codes.GetSize(); j++) 
		{
			item.Format("%-8s %s", codes.GetAt(j), names.GetAt(j));
			tree->SetItemData(tree->InsertItem(item, hItem), kinds.GetAt(j));
		}
	}
}

void CConfigBarPru::RefreshInterestItem()
{
	ASSERT(m_treeInter);
	m_treeInter->DeleteAllItems();
	LoadInterestItems(m_treeInter);
}


void CConfigBarPru::LoadHaveItems(CAxMTreeCtrl* tree)
{
	CStringList accts;

	CAxTargetGroupPru::GetAccountList(m_view, accts, GetUseAlias());
	CString account;

	for (POSITION pos = accts.GetHeadPosition(); pos; ) {
		account = accts.GetNext(pos);

		tree->InsertItem(account);
	}
}

void CConfigBarPru::LoadFeatureItems(CAxMListCtrl* list)
{
	//
}

CString CConfigBarPru::GetTargetItemFileName()
{
	CString path;
	path.Format("%s%s\\targetitem.dat", m_gexFolder, m_userName);
	return path;
}

void CConfigBarPru::OnSelectItem(LPCTSTR kind_cd)
{
	m_parent->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipUnitCode), (LPARAM)kind_cd);
}

void CConfigBarPru::OnTargetItemUserConfig()
{
	HTREEITEM hItem = m_treeKind->GetSelectedItem();
	if (m_treeKind->ItemHasChildren(hItem) == FALSE) {
		LONG key = m_treeKind->GetItemData(hItem);
		CAxTargetItem::KeyToKind(key, m_lastKind);
		if (CAxTargetItem::KindToKey(m_lastKind) == DEF_TARGETGROUP) {
			
			HTREEITEM hItem = m_treeKind->GetSelectedItem();
			SetGroupItem(m_treeKind, hItem);
			ResizeListColumn(m_listKind);
		}
	}
}

CString CConfigBarPru::GetCodeName(LPCTSTR code)
{
	return m_group->GetCodeName(code);
}

void CConfigBarPru::SendRequestFromGroup(CString strCode, char* strData, int nSize, char stat)
{
	m_trSender = m_group;

	TRINFO trInfo;
	ZeroMemory(&trInfo, sizeof(trInfo));
	trInfo.sender = this;
	trInfo.trCode = strCode;
	trInfo.data = (LPCTSTR)strData;
	trInfo.size = nSize;
	trInfo.stat = stat;

	m_parent->SendMessage(WM_VIEW_SENDTR, 0, (LPARAM)&trInfo); 
	
	//**((CGrpView*)m_parent)->SendTR(this, strCode, strData, nSize, stat);
}

LRESULT CConfigBarPru::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	if (m_trSender == m_group) 
	{
		m_group->RecvData(wParam, lParam);
		m_btnCheckAll->SetDown(TRUE);
		SetListKindItemCheck(TRUE);			
	}
	else if (m_trSender == this) 
	{
		switch (m_requestKey) 
		{
			case RKEY_FEATUREITEM:
				RecvFeatureData(wParam, lParam);
				break;
			case RKEY_HAVEITEM:
				RecvHaveItemData(wParam, lParam);
				break;
		}
	}

	m_trSender = FALSE;
	m_requestKey = NULL;
	m_requestHaveItem = NULL;
	return 0L;
}

void CConfigBarPru::RequestFeatureItems()
{
	FEATUREMID mid;
	
	memset(&mid, '0', sizeof(mid));
	strncpy(mid.fday, CTime::GetCurrentTime().Format("%Y%m%d"), ARRAY_SIZE(mid.fday));
	
	m_trSender = this;
	m_requestKey = RKEY_FEATUREITEM;

	TRINFO trInfo;
	ZeroMemory(&trInfo, sizeof(trInfo));
	trInfo.sender = this;
	trInfo.trCode = FEATURE_TRCODE;
	trInfo.data = (LPCTSTR)&mid;
	trInfo.size = sizeof(mid);
	trInfo.stat = 0;

	m_parent->SendMessage(WM_VIEW_SENDTR, 0, (LPARAM)&trInfo); 
}

// 지표 - 보유종목
void CConfigBarPru::RequestHaveItem(HTREEITEM hItem)
{
	if (hItem == NULL)
		return;
	
	CString acct = m_treeHave->GetItemText(hItem);
	int find = acct.Find(' ');
	if (find > 0)
		acct = acct.Left(find);

	acct.Replace("-", "");
	if (acct.GetLength() != 11) 
	{
		if (!acct.IsEmpty())
			SelectItem(0, acct);
		return;
	}

	ClearAllChildItem(m_treeHave, hItem);
	CString pass;
	CAxTargetGroupPru::GetAccountPassword(m_view, acct, pass);

//	pass = "0423";
	if (pass.IsEmpty()) 
	{
		if (CAxTargetGroupPru::IsStaff(m_view))	// 직원일때만 체크
		{
			ClearAllChildItem(m_treeHave, hItem);
			const char* msg = "계좌비밀번호 저장 후 사용하십시오.";

			AfxGetMainWnd()->MessageBox(msg, COMPANYNAME);
			return;
		}
		pass = "    ";
	}

//	CString enc;
//	CAxTargetGroupPru::EncrytoAccountPassword(m_view, acct, pass, enc);

	CString	svcn = "";
	CString	sGubn = "";
	if (IsFutureAccount(acct))
	{
		svcn = "SONBQ021";
		sGubn.Format("%c", mkty_FUTURE);
	}
	else
	{
		svcn = "SONAQ052";
		sGubn.Format("%c", mkty_STOCK);
	}

	CString sData = ledgerTR(acct, svcn, sGubn);

	TRINFO trInfo;
	ZeroMemory(&trInfo, sizeof(trInfo));
	if (IsFutureAccount(acct))
	{
		struct f_mid mid;
		
		FillMemory(&mid, L_fmid, ' ');
		CopyMemory(&mid.accn, acct, sizeof(mid.accn));
		CopyMemory(&mid.pswd, pass, pass.GetLength());

		sData += CString((char*)&mid, L_fmid);	
		trInfo.trCode = ACC_FJANGO;
		trInfo.data = (LPCTSTR)sData.operator LPCTSTR(); 
		trInfo.size = sData.GetLength();
	}
	else
	{
		struct s_mid mid;
		
		FillMemory(&mid, L_smid, ' ');
		CopyMemory(&mid.accn, acct, sizeof(mid.accn));
		CopyMemory(&mid.pswd, pass, pass.GetLength());
		mid.sygb[0] = '1';
		mid.fees[0] = '0';

		sData += CString((char*)&mid, L_smid);
		trInfo.trCode = ACC_SJANGO;
		trInfo.data = (LPCTSTR)sData.operator LPCTSTR(); 
		trInfo.size = sData.GetLength();
	}

	m_trSender = this;
	m_requestKey = RKEY_HAVEITEM;
	trInfo.sender = this;
	trInfo.stat = 0;

	m_parent->SendMessage(WM_VIEW_SENDTR, 0, (LPARAM)&trInfo);

	m_requestHaveItem = hItem;
}

bool CConfigBarPru::IsFutureAccount(CString sAccount)
{
	if (sAccount.GetLength() < 5)
		return false;

	CString	sGubn = sAccount.Mid(3, 2);
	if (sGubn == "20" || sGubn == "21") // 선물계좌이면
		return true;

	return false;
}

CString CConfigBarPru::ledgerTR(CString sAccn, CString sSvcn, CString sGubn)
{
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	m_view->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CString	strUser = (LPCTSTR)m_view->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0);
	
	CopyMemory(&ledger.svcd, sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, sAccn.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = sGubn.GetAt(0);
	ledger.odrf[0] = odty_QUERY;

	return CString((char*)&ledger, L_ledgerH);
}

void CConfigBarPru::ClearAllChildItem(CTreeCtrl* tree, HTREEITEM hItem)
{
	HTREEITEM hChild = tree->GetChildItem(hItem);
	HTREEITEM hNext;
	while (hChild) 
	{
		hNext = tree->GetNextSiblingItem(hChild);
		tree->DeleteItem(hChild);
		hChild = hNext;
	}
}

void CConfigBarPru::RecvFeatureData(WPARAM wParam, LPARAM lParam)
{
	LPFEATUREMOD mod = (LPFEATUREMOD)lParam;

	int index = 0, count = atoi(CString(mod->nrec, ARRAY_SIZE(mod->nrec)));

	CString code, title, name;

	for (int i = 0; i < count; i++) 
	{
		code = CString(mod->grid[i].code, ARRAY_SIZE(mod->grid[i].code));
		title = CString(mod->grid[i].titl, ARRAY_SIZE(mod->grid[i].titl));

		title.TrimRight();


		name = GetCodeName(code);
		name.TrimRight();
		if (!name.IsEmpty()) 
		{
			m_listFeature->InsertItem(index, code);
			m_listFeature->SetItemText(index++, 1, name);
		}
		else 
		{
			//**Debug("Can not find name. [%s]", code);
		}
	}

	ResizeListColumn(m_listFeature);
}

void CConfigBarPru::RecvHaveItemData(WPARAM wParam, LPARAM lParam)
{
	if (m_requestHaveItem != m_treeHave->GetSelectedItem()) return;

	int	len = HIWORD(wParam);
	struct _ledgerH *ledger = (_ledgerH*)lParam;	

	CString	sErrCode = CString((char*)ledger->emsg, 4);
	CString	sErrText = CString((char*)ledger->emsg, 94);
	if (sErrCode.GetAt(0) != _T('0'))	// 오류
		return;

	char	*pJango = (char*)(lParam + L_ledgerH);
	CString sAccn = CString((char*)pJango, 11), sCode, sName, str;
	int nrec = 0;
	int sizeL = 0;
	int checkL = 0;

	if (sAccn.IsEmpty())	return;

	if (IsFutureAccount(sAccn))
	{
		struct f_mod	fmod;
		CopyMemory(&fmod, pJango, L_fmod);
		nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
		pJango += L_fmod;
		sizeL += L_fmod;	
		checkL = L_fmod + L_frec * nrec;

		for (int ii = 0; ii < nrec; ii++)
		{
			struct f_rec *frec = (struct f_rec *)pJango;

			sCode = CString(frec->cod2, sizeof(frec->cod2));
			sCode.TrimRight();
			sName = CString(frec->hnam, sizeof(frec->hnam));
			sName.TrimRight();

			str.Format("%-8s %s", sCode, sName);
			m_treeHave->InsertItem(str, m_requestHaveItem);

			pJango += L_frec;	sizeL += L_frec;
			if (sizeL > len || sizeL > checkL)
				break;
		}
	}
	else
	{
		struct s_mod	smod;
		CopyMemory(&smod, pJango, L_smod);
		nrec = atoi(CString(smod.nrec, sizeof(smod.nrec)));
		pJango += L_smod;
		sizeL += L_smod;	
		checkL = L_smod + L_srec * nrec;

		for (int ii = 0; ii < nrec; ii++)
		{
			struct s_rec *srec = (struct s_rec *)pJango;

			sCode = CString(srec->cod2, sizeof(srec->cod2));
			sCode.TrimRight();
			if (!sCode.IsEmpty() && sCode.GetAt(0) == 'A' || sCode.GetAt(0) == 'J')
				sCode = sCode.Mid(1);

			sName = CString(srec->hnam, sizeof(srec->hnam));
			sName.TrimRight();

			CString	sGubn = CString(srec->jgyh, sizeof(srec->jgyh));
			CString	sCredit = _T("");
			switch (atoi(sGubn))
			{
			case 1:		
				sCredit = _T("(신용)");	
				break;
			case 2:		case 3:		case 4:		case 5:
				sCredit = _T("(대용)");	
				break;
			default:
				break;
			}

			str.Format("%-8s %s%s", sCode, sName, sCredit);
			m_treeHave->InsertItem(str, m_requestHaveItem);

			pJango += L_srec;	sizeL += L_srec;
			if (sizeL > len || sizeL > checkL)
				break;
		}
	}

	m_treeHave->Expand(m_requestHaveItem, TVE_EXPAND);
	m_treeHave->EnsureVisible(m_requestHaveItem);
}

BOOL CConfigBarPru::GetUseAlias()
{
	CString fileName;
	fileName.Format("%s\\%s\\%s.ini", m_userFolder, m_userName, m_userName);
	int useAlias = GetPrivateProfileInt("Account", "UseAlias", 0, fileName);

	return useAlias;
}


/************************************************************************/
/* CAxTargetGroupPru                                                    */
/************************************************************************/
CAxTargetGroupPru::CAxTargetGroupPru(CWnd* wizard, CWnd* parent, LPCTSTR lpszTabFolder, LPCTSTR lpszUserFolder)
:CAxTargetGroup(wizard, parent, lpszTabFolder, lpszUserFolder)
{
	m_list = NULL;
	m_listTree = NULL;
	m_asyncList = m_list;

	m_lastTRCode.Empty();
}

CAxTargetGroupPru::~CAxTargetGroupPru()
{
	ClearStockOptions();
}

void CAxTargetGroupPru::Setting(CTreeCtrl* tree)
{
	TARGETGROUP tgs[] = {
		{ "사용자설정", groupUserSelect},
		{ "전종목", groupAllItem },			{ "KOSPI200", groupKospi200 },			{ "KOSTAR", groupKostar },
		{ "ETF", groupETF },				{ "프리보드", groupFreeboard },			{ "ELW", groupELW },
		{ "선물", groupFuture },			{ "KOSPI옵션", groupOption },			/*{ "개별주식옵션", groupStockOption },*/
		{ "업종지수", groupIndex },			{ "업종", groupIndexItem },				{ "계열사", groupCompany },
		/*{ "등락순위별", groupUpDownRank },*/  { "보유종목", groupHoldItem },			/**{ "최근조회", groupHistory },**/
		{ "관심종목그룹", groupInterestItem}, {"테마리스트", groupThemeItem }
	};
	
	tree->DeleteAllItems();
	HTREEITEM hItem;

	int start = tree == m_bar->m_treeKind ? groupUserSelect: groupAllItem;
	
	for (int i = start; i < ARRAY_SIZE(tgs); i++) {
		ZeroMemory(&m_kind, sizeof(KIND));
		hItem = tree->InsertItem(tgs[i].name);
		m_kind.kind = tgs[i].group;
		tree->SetItemData(hItem, CAxTargetItem::KindToKey(m_kind));
		SetSubItem(tree, hItem, tgs[i].group);
	}
	ZeroMemory(&m_kind, sizeof(KIND));
}

#define GOTO(a, b)  { case a: b(); break; }
void CAxTargetGroupPru::SetSubItem(CTreeCtrl* tree, HTREEITEM hItem, int group)
{
	m_tree = tree;
	m_item = hItem;
	switch (group) {
		GOTO(groupELW,			SetgroupELW);
		GOTO(groupOption,		SetgroupOption);
		GOTO(groupStockOption,	SetgroupStockOption);
		GOTO(groupIndex,		SetgroupIndex);
		GOTO(groupIndexItem,	SetgroupIndexItem);
		GOTO(groupCompany,		SetgroupCompany);
		GOTO(groupUpDownRank,	SetgroupUpDownRank);
		GOTO(groupHoldItem,		SetgroupHoldItem);
		GOTO(groupInterestItem, SetgroupInterestItem);
		GOTO(groupThemeItem,	SetgroupThemeItem);
	}
	m_tree = NULL;
	m_item = NULL;
}

#define ADD_MY_ARRAY_TO_TREE() {\
		for (int i = 0; i < ARRAY_SIZE(tgs); i++) {\
			m_kind.index = tgs[i].group;\
			m_tree->SetItemData(m_tree->InsertItem(tgs[i].name, m_item), CAxTargetItem::KindToKey(m_kind));\
		}\
		m_kind.index = 0; \
	}

void CAxTargetGroupPru::SetgroupELW()
{
	TARGETGROUP tgs[] = {
		{ "발행사별", groupELW_List },			{ "기초자산별", groupELW_Asset }
	};

	ADD_MY_ARRAY_TO_TREE();

	HTREEITEM hItemList = m_tree->GetChildItem(m_item);
	HTREEITEM hItemAsset = m_tree->GetNextSiblingItem(hItemList);
	SetTreeItem_ELW(hItemList, hItemAsset);
}

void CAxTargetGroupPru::SetgroupOption()
{
	TARGETGROUP tgs[] = {
		{ "콜옵션", groupCallOption },			{ "풋옵션", groupPutOption }
	};

	ADD_MY_ARRAY_TO_TREE();
}

void CAxTargetGroupPru::SetgroupStockOption()
{
	TARGETGROUP tgs[] = {
		{ "콜옵션", groupStockCallOption },			{ "풋옵션", groupStockPutOption }
	};
	
	HTREEITEM hItem;

	for (int i = 0; i < ARRAY_SIZE(tgs); i++) {
		m_kind.subkind = tgs[i].group;
		hItem = m_tree->InsertItem(tgs[i].name, m_item);
		m_tree->SetItemData(hItem, CAxTargetItem::KindToKey(m_kind));
		SetStockOptionItem(hItem);
	}
	m_kind.subkind = 0;
}

void CAxTargetGroupPru::SetgroupIndex()
{
	TARGETGROUP tgs[] = 
	{
		{ "장내",		groupIndexKospi },			
		{ "코스닥", groupIndexKosdaq },
		{ "섹터지수",	groupIndexSector},
		{ "프리보드지수", groupIndexFreeboard }
	};
	
	ADD_MY_ARRAY_TO_TREE();
}

void CAxTargetGroupPru::SetgroupIndexItem()
{
	TARGETGROUP tgs[] = 
	{
		{ "코스피업종",		groupIndexItemKospi },		
		{ "코스닥업종", groupIndexItemKosdaq }
//		{ "섹터지수",		groupIndexItemSector},		
//		{ "프리보드업종", groupIndexItemFreeboard }
	};

	HTREEITEM hItem, hItemSave;

	for (int i = 0; i < ARRAY_SIZE(tgs); i++) 
	{
		m_kind.subkind = tgs[i].group;
		hItem = m_tree->InsertItem(tgs[i].name, m_item);
		m_tree->SetItemData(hItem, CAxTargetItem::KindToKey(m_kind));

		hItemSave = m_item;
		m_item = hItem;
		switch (m_kind.subkind) 
		{
			case groupIndexItemKospi:
				SetgroupIndexItemKospi(); break;
			case groupIndexItemKosdaq:
				SetgroupIndexItemKosdaq(); break;
//			case groupIndexItemSector:
//				SetgroupIndexItemSector(); break;
//			case groupIndexItemFreeboard:
//				SetgroupIndexItemFreeboard(); break;
		}
		m_item = hItemSave;
	}
	m_kind.subkind = 0;
}

void CAxTargetGroupPru::SetgroupCompany()
{
	SetgroupFromInformationFile(m_tabFolder + GROUP_FILE, 2);
}

void CAxTargetGroupPru::SetgroupUpDownRank()
{
	TARGETGROUP tgs[] = 
	{
		{"상승률상위(거래소)",			3111},
		{"상승률상위(코스닥)",			3211},
		{"하락율상위(거래소)",			3112},
		{"하락율상위(코스닥)",			3212},
		{"보통주/우선주이격율(거래소)",		3113},
		{"보통주/우선주이격율(코스닥)",		3213},
		{"거래량상위(거래소)",			3114},
		{"거래량상위(코스닥)",			3214},
		{"거래대금상위(거래소)",		3115},
		{"거래대금상위(코스닥)",		3215},
		{"시가총액순위(거래소)",		3116},
		{"시가총액순위(코스닥)",		3216},
		{"상한가종목(거래소)",			3101},
		{"상한가종목(코스닥)",			3201},
		{"상승종목(거래소)",			3102},
		{"상승종목(코스닥)",			3202},
		{"보합종목(거래소)",			3103},
		{"보합종목(코스닥)",			3203},
		{"하락종목(거래소)",			3105},
		{"하락종목(코스닥)",			3205},
		{"하한가종목(거래소)",			3104},
		{"하한가종목(코스닥)",			3204},
		{"52주고가종목(거래소)",		3117},
		{"52주고가종목(코스닥)",		3217},
		{"52주저가종목(거래소)",		3118},
		{"52주저가종목(코스닥)",		3218},
		{"매수체결비율상위(거래소)",		3121},
		{"매수체결비율상위(코스닥)",		3221},
		{"매도체결비율상위(거래소)",		3122},
		{"매도체결비율상위(코스닥)",		3222},
		{"5일-20일 골든크로스종목(거래소)",	3131},
		{"5일-20일 골든크로스종목(코스닥)",	3231},
		{"5일-60일 골든크로스종목(거래소)",	3132},
		{"5일-60일 골든크로스종목(코스닥)",	3232},
		{"20일-60일 골든크로스종목(거래소)",	3133},
		{"20일-60일 골든크로스종목(코스닥)",	3233},
		{"5일-20일 데드크로스종목(거래소)",	3135},
		{"5일-20일 데드크로스종목(코스닥)",	3235},
		{"5일-60일 데드크로스종목(거래소)",	3136},
		{"5일-60일 데드크로스종목(코스닥)",	3236},
		{"20일-60일 데드크로스종목(거래소)",	3137},
		{"20일-60일 데드크로스종목(코스닥)",	3237}
	};
	
	ADD_MY_ARRAY_TO_TREE();
}

void CAxTargetGroupPru::SetgroupHoldItem()
{
	CStringList accts;

	GetAccountList(m_parent, accts, m_bar->GetUseAlias());

	int i = 0;
	for (POSITION pos = accts.GetHeadPosition(); pos; ) 
	{
		m_kind.index = ++i;
		m_tree->SetItemData(m_tree->InsertItem(accts.GetNext(pos), m_item), CAxTargetItem::KindToKey(m_kind));
	}
	m_kind.index = 0;
}

#ifndef MAX_GR
#define MAX_GR	100
#endif 

const char* _portfolio = "portfolio.ini";

void CAxTargetGroupPru::GetInterestGroupList(LPCTSTR folder, CStringArray& array)
{
	array.RemoveAll();
	CString key, str, name, fileName;
	char buffer[1024];
	const char* cFolder = "FOLDER";

	fileName = CString(folder) + _portfolio;

	for (int i = 0; i < MAX_GR; i++) 
	{
		key.Format("%02d", i);
		GetPrivateProfileString("GROUPNAME", key, EMPTYSTR, buffer, ARRAY_SIZE(buffer), fileName);
		str = buffer;
		if (!str.IsEmpty()) 
		{
			name = buffer;
			if (IsExistPortfolio(folder, i))
				array.Add(name);
			else
				array.Add(EMPTYSTR);
		}
		else
		{
			array.Add(EMPTYSTR);
		}
	}
}

void CAxTargetGroupPru::GetInterestItemList(LPCTSTR folder, int index, CStringArray& codeArray, CStringArray& nameArray, CByteArray& kindArray)
{
	codeArray.RemoveAll();
	nameArray.RemoveAll();
	kindArray.RemoveAll();

	CString fileName;
	fileName.Format("%sPortFolio.i%02d", folder, index);
	if (!IsExistPortfolio(folder, index)) return;
	
	CFile* pFile;
	
	TRY {
		pFile = new CFile(fileName, CFile::modeRead);
	}
	CATCH(CFileException, e) 
	{
		return;
	}
	END_CATCH;
	
//	_alarms	alarm;
	struct _inters	inter;
	CString code, name;
	while (pFile->Read(&inter, sizeof(inter)) == sizeof(inter)) 
	{
		code = CString(inter.code, sizeof(inter.code)); TRIM(code);
		name = CString(inter.name, sizeof(inter.name)); TRIM(name);
		if (!code.IsEmpty() && code.GetAt(0) == 'm')	// 책갈피 제거
			continue;

		codeArray.Add(code);
		nameArray.Add(name);
		kindArray.Add((BYTE)inter.gubn);	// 0:none, 1:현물, 2:선물, 3:옵션, 4:개별주식옵션, 5:지수

//		for (int jj = 0; jj < inter.a_num; jj++)
//			pFile->Read(&alarm, sizeof(alarm));
	}
	delete pFile;
}

int	CAxTargetGroupPru::ConvertItemKind(DWORD data)
{
	switch (data) 
	{
	case ikStock: return GU_CODE;
	case ikFuture: return GU_FUTURE;
	case ikOption: return GU_OPTION;
	case ikStockOption: return GU_COOPT;
	case ikIndex: return GU_INDEX;
	case ikForeign: return GU_FOREIGN;
	}
	return 0;
}


BOOL CAxTargetGroupPru::IsExistPortfolio(LPCTSTR folder, int index)
{
	CString fileName;
	fileName.Format("%sPortFolio.i%02d", folder, index);
	
	return IsFileExist(fileName);
}

void CAxTargetGroupPru::GetAccountList(CWnd* wnd, CStringList& accts, BOOL useAlias)
{
	int	gubn;
	CString data, acnt, alias, name, str;
	CString	tmp = (LPCTSTR)wnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, accountCC), (LPARAM)"AN10");
//char szInfo[1024] = "00110001232\t\t안수현|||\t\n00110001233\t0423\t안수현|||\t\n00120001232\t0423\t안수현|||\t\n00120001233\t0423\t안수현|||\t\n";

//char szInfo[] = "00100000026		정문일||1\n00200000588		정문일||1\n00200000589		정문일||1\n50100000028		정문일||1\n00110000026		정문일\n00151000026	0423	정문일\n00210000588	0423	정문일\n00210000589		정문일\n00211000588		정문일\n00251000588		정문일\n00252000588		정문일";
//tmp = szInfo;
	while (!tmp.IsEmpty()) 
	{
		data = XParse(tmp, "\n");
		acnt = XParse(data, TAB);
		XParse(data, TAB);
		name = XParse(data, "|");
		name.TrimRight();
		alias = XParse(data, "|");
		alias.TrimRight();

		if (useAlias && !alias.IsEmpty())
			name = alias;
		
		if (acnt.GetLength() == 11) 
		{
			gubn = atoi(acnt.Mid(3, 2));
			if (gubn == 10 || gubn == 11 ||				// 주식위탁
				gubn == 30 || gubn == 31 || gubn == 32 ||	// 증권저축
				gubn == 50 || gubn == 51 || gubn == 52)		// 자산관리
			{
				str.Format("%s-%s-%s", acnt.Left(3), acnt.Mid(3, 2), acnt.Mid(5, 6));
				data = str + SPACE;
				data += name;
				accts.AddTail(data);
			}
		}
	}

	tmp = (LPCTSTR)wnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, accountCC) , (LPARAM)"AN20");
	while (!tmp.IsEmpty()) 
	{
		data = XParse(tmp, "\n");
		acnt = XParse(data, TAB);
		XParse(data, TAB);
		name = XParse(data, "|");
		name.TrimRight();
		alias = XParse(data, "|");
		alias.TrimRight();
		
		if (acnt.GetLength() == 11) 
		{
			gubn = atoi(acnt.Mid(3, 2));
			if (gubn == 20 || gubn == 21)				// 선물옵션,옵션매수전용
			{
				str.Format("%s-%s-%s", acnt.Left(3), acnt.Mid(3, 2), acnt.Mid(5, 6));
				data = str + SPACE;
				data += name;
				
				accts.AddTail(data);
			}
		}
	}
}

void CAxTargetGroupPru::GetAccountPassword(CWnd* wnd, LPCTSTR acct, CString& pass)
{
	pass = (LPCTSTR)wnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, passCC), (LPARAM)acct);
}

void CAxTargetGroupPru::EncrytoAccountPassword(CWnd* wnd, LPCTSTR acct, LPCTSTR pass, CString& enc)
{
	CString param;
	param.Format("%s\t%s", pass, acct);
	enc = (LPCTSTR)wnd->SendMessage(WM_USER, MAKEWPARAM(encPASSx, 0), (LPARAM)(LPCTSTR)param);
}

BOOL CAxTargetGroupPru::IsStaff(CWnd* wnd)
{
	return wnd->SendMessage(WM_USER, MAKEWPARAM(variantDLL, orderCC), 0) ? FALSE:TRUE;
}

void CAxTargetGroupPru::SetgroupInterestItem()
{
	CStringArray groups;

	CAxTargetGroupPru::GetInterestGroupList(m_userFolder, groups);

	for (int i = 0; i < groups.GetSize(); i++) 
	{
		m_kind.index = i;
		if (!groups.GetAt(i).IsEmpty())
			m_tree->SetItemData(m_tree->InsertItem(groups.GetAt(i), m_item), CAxTargetItem::KindToKey(m_kind));
	}
	m_kind.index = 0;
}

void CAxTargetGroupPru::SetgroupThemeItem()
{
	SetgroupFromInformationFile(m_tabFolder + THEME_FILE, 2);
}

void CAxTargetGroupPru::SetStockOptionItem(HTREEITEM hItem)
{
	LPTARGETGROUP p;

	for (int i = 0; i < m_stockOptions.GetSize(); i++) 
	{
		p = m_stockOptions.GetAt(i);

		m_kind.index = p->group;
		m_tree->SetItemData(m_tree->InsertItem(p->name, hItem), CAxTargetItem::KindToKey(m_kind));
	}
	m_kind.index = 0;
}

void CAxTargetGroupPru::SetgroupIndexItemKospi()
{
	SetgroupIndexUpjong(upKOSPI, upINTEGRATE);

	for (int i = 0; i < ARRAY_SIZE(infop); i++) 
	{
		m_kind.index = atoi(infop[i].code);
		m_tree->SetItemData(m_tree->InsertItem(infop[i].name, m_item), CAxTargetItem::KindToKey(m_kind));
	}
}

void CAxTargetGroupPru::SetgroupIndexItemKosdaq()
{
	SetgroupIndexUpjong(upKOSDAQ, -1);
	for (int i = 0; i < ARRAY_SIZE(infod); i++) 
	{
		m_kind.index = atoi(infod[i].code);
		m_tree->SetItemData(m_tree->InsertItem(infod[i].name, m_item), CAxTargetItem::KindToKey(m_kind));
	}
}
/*
void CAxTargetGroupPru::SetgroupIndexItemSector()
{
	SetgroupIndexUpjong(upSECTOR, -1);
}
*/
/*
void CAxTargetGroupPru::SetgroupIndexItemFreeboard()
{
	SetgroupIndexUpjong(upFREEBOARD, -1);
}
*/
void CAxTargetGroupPru::SetgroupIndexUpjong(int kind1, int kind2)
{
	struct upcode* up;
	CString code, name;
	m_kind.subkind = kind1;

	for (int i = 0; i < m_upcount; i++) 
	{
		up = &m_upcode[i];

		if (up->jgub == kind1 || up->jgub == kind2) 
		{
			name = CString(up->hnam, 20);
			TRIM(name);

			m_kind.index = up->ucod;
			m_tree->SetItemData(m_tree->InsertItem(name, m_item), CAxTargetItem::KindToKey(m_kind));
		}
	}
	m_kind.index = 0;
}

void CAxTargetGroupPru::SetgroupFromInformationFile(LPCTSTR fileName, int keyLength)
{
	int i = 0;
	CString key, str, name, format;
	char buffer[MAX_PATH];
	const char* cFolder = "FOLDER";

	CQArray<_ThemeData, _ThemeData> pTheme;
	pTheme.RemoveAll();

	_ThemeData	TData;
	format.Format("%%0%dd", keyLength);
	while (TRUE) 
	{
		key.Format(format, i++);
		GetPrivateProfileString(cFolder, key, EMPTYSTR, buffer, ARRAY_SIZE(buffer), fileName);
		str = buffer;

		if (!str.IsEmpty()) 
		{
			TData.Name = XParse(str, ';');
			TData.Name.TrimRight();
			TData.Key = atoi(str);
			pTheme.Add(TData);
		}
		else
			break;
	}

	pTheme.QuickSort();

	for (int ii = 0; ii < pTheme.GetSize(); ii++)
	{
		TData = pTheme.GetAt(ii);
		m_kind.index = TData.Key;
		m_tree->SetItemData(m_tree->InsertItem(TData.Name, m_item, NULL), CAxTargetItem::KindToKey(m_kind));
	}

	pTheme.RemoveAll();
	m_kind.index  = 0;
}

void CAxTargetGroupPru::LoadJcode()
{
	LoadCodeMaster(HJCODE, m_hjcount, (void**)&m_hjcode, sizeof(struct hjcode));
}

void CAxTargetGroupPru::LoadUpcode()
{
	LoadCodeMaster(UPCODE, m_upcount, (void**)&m_upcode, sizeof(struct upcode));
}

void CAxTargetGroupPru::LoadFcode()
{
	LoadCodeMaster(FJCODE, m_fjcount, (void**)&m_fjcode, sizeof(struct fjcode));
}

void CAxTargetGroupPru::LoadOJcode()
{
	LoadCodeMaster(OJCODE, m_ojcount, (void**)&m_ojcode, sizeof(struct ojcode), sizeof(struct ojcodh));
}

void CAxTargetGroupPru::LoadPJcode()
{
	LoadCodeMaster(PJCODE, m_pjcount, (void**)&m_pjcode, sizeof(struct pjcode));

	NormalizeStockOptions();
}

void CAxTargetGroupPru::LoadELWcode()
{
	LoadCodeMaster(ELCODE, m_elwcount, (void**)&m_elwcode, sizeof(struct elwcode));
}

void CAxTargetGroupPru::Setting(CTreeCtrl* tree, CListCtrl* list, KIND kind)
{
	list->DeleteAllItems();

	m_list = list;
	m_listTree = tree;
	memcpy(&m_kind, &kind, sizeof(KIND));

	switch (kind.kind) 
	{
		case groupUserSelect:	SetListUserSelect();	break;
		case groupAllItem:		SetListAllItem();		break;
		case groupKospi200:		SetListKospi200();		break;
		case groupKostar:		SetListKostar();		break;
		case groupETF:			SetListETF();			break;
		case groupFreeboard:	SetListFreeboard();		break;
		case groupELW:			SetListELW();			break;
		case groupFuture:		SetListFuture();		break;
		case groupOption:		SetListOption();		break;
		case groupStockOption:	SetListStockOption();	break;
		case groupIndex:		SetListIndex();			break;
		case groupIndexItem:	SetListIndexItem();		break;
		case groupCompany:		SetListCompany();		break;
		case groupUpDownRank:	SetListUpDownRank();	break;
		case groupHoldItem:		SetListHoldItem();		break;
		case groupHistory:		SetListHistory();		break;
		case groupInterestItem: SetListInterestItem();	break;
		case groupThemeItem:	SetListThemeItem();		break;
	}

	m_asyncList = m_list;
	m_list = NULL;
	m_listTree = NULL;
}

void CAxTargetGroupPru::SetListUserSelect()
{
	CAxTargetItem item(m_bar->GetTargetItemFileName());
	item.LoadItemFromFile();

	int find;
	KIND key;
	CAxTargetItem::KeyToKind(DEF_TARGETGROUP, key);

	CString code, name, items = item.GetItem(key);

	while (TRUE) 
	{
		find = items.Find(';');
		if (find < 1)
			break;

		code = items.Left(find);
		items.Delete(0, find + 1);

		name = m_bar->GetCodeName(code);
		if (!name.IsEmpty()) 
		{
			name.TrimRight();
			m_list->SetItemText(m_list->InsertItem(m_list->GetItemCount(), code), 1, name);
		}
	}

}

void CAxTargetGroupPru::SetListAllItem()
{
	InsertListItem(m_hjcode, sizeof(struct hjcode), m_hjcount, FP_AllItem, II_hjcode);
}

void CAxTargetGroupPru::SetListKospi200()
{
	InsertListItem(m_hjcode, sizeof(struct hjcode), m_hjcount, FP_Kospi200, II_hjcode);
}

void CAxTargetGroupPru::SetListKostar()
{
	InsertListItem(m_hjcode, sizeof(struct hjcode), m_hjcount, FP_Kostar, II_hjcode);
}

void CAxTargetGroupPru::SetListETF()
{
	InsertListItem(m_hjcode, sizeof(struct hjcode), m_hjcount, FP_ETF, II_hjcode);
}

void CAxTargetGroupPru::SetListFreeboard()
{
	InsertListItem(m_hjcode, sizeof(struct hjcode), m_hjcount, FP_Freeboard, II_hjcode);
}

int compare_by_listupOrg(const void * a, const void * b)
{
	struct elwcode *p1, *p2;

	p1 = (struct elwcode*)a;
	p2 = (struct elwcode*)b;

	CString code1, code2;
	code1 = CString(p1->bgce, ARRAY_SIZE(p1->bgce));
	code2 = CString(p2->bgce, ARRAY_SIZE(p2->bgce));

	return code1.Compare(code2);
}

int compare_by_baseAsset(const void* a, const void* b)
{
	struct elwcode *p1, *p2;
	
	p1 = (struct elwcode*)a;
	p2 = (struct elwcode*)b;
	
	CString code1, code2;
	code1 = CString(p1->gcjs[0], ARRAY_SIZE(p1->gcjs[0]));
	code2 = CString(p2->gcjs[0], ARRAY_SIZE(p2->gcjs[0]));
	
	return code1.Compare(code2);
}

void CAxTargetGroupPru::SetListELW()
{
	if (m_kind.subkind == groupELW_List)	// 발행기관별
	{
		LPVOID	node;
		int	recSize = sizeof(struct elwcode);
		BASECode	selElw = m_arrBase.GetAt(m_kind.index);
		for (int i = 0; i < m_elwcount; i++) 
		{
			node = (LPBYTE)m_elwcode + (recSize * i);
			struct elwcode* p = (struct elwcode*)node;
			CString s = CString(p->bgce, ARRAY_SIZE(p->bgce));
			if (s == selElw.code) 
			{
				II_elwcode(node, m_list);
			}
		}
	}
	else									// 기초자산별
	{
		elwcode* node;
		CString s, code;
		BOOL nameCompare = FALSE;
		if (m_kind.index == 0xFFFF)
			code = "KOSPI200";
		else if (m_kind.index == 0xFFFE) 
		{
			nameCompare = TRUE;
			code = "바스켓";
		}
		else
		{
			code = m_arrELW[m_kind.index].code;
		}

		for (int i = 0; i < m_elwcount; i++) 
		{
			node = &m_elwcode[i];
			if (!nameCompare) 
			{
				for (int j = 0; j < 5; j++) 
				{
					s = CString(node->gcjs[j], ARRAY_SIZE(node->gcjs[j]));
					s.TrimRight();
					if (s == code) 
					{ 
						CString code, name;
						code = CString(node->codx, ELWCodeLen);
						name = CString(node->hnam, ELWHNameLen);

						ListInsertItem(m_list, code, name, GU_CODE);
						break;
					}
				}
			}
			else 
			{
				s = CString(node->hnam, ARRAY_SIZE(node->hnam));
				if (s.Find("바스켓") > 0) 
				{
					CString code, name;
					code = CString(node->codx, ELWCodeLen);
					name = CString(node->hnam, ELWHNameLen);

					ListInsertItem(m_list, code, name, GU_CODE);
				}
			}
		}
	}
}

void CAxTargetGroupPru::SetListFuture()
{
	InsertListItem(m_fjcode, sizeof(struct fjcode), m_fjcount, FP_NoCondition, II_fjcode);
}

void CAxTargetGroupPru::SetListOption()
{
	InsertListItem(m_ojcode, sizeof(struct ojcode), m_fjcount, 
		FP_NoCondition,
		m_kind.index == groupCallOption ? II_ojcode_c: II_ojcode_p);
}

void CAxTargetGroupPru::SetListStockOption()
{
	if (m_kind.index == 0) return;

	InsertListItem(m_pjcode, sizeof(struct pjcode), m_pjcount, FP_StockOption, II_pjcode);
}


void CAxTargetGroupPru::SetListIndex()
{
	InsertListItem(m_upcode, sizeof(struct upcode), m_upcount, FP_Index, II_upcode);

	if (m_kind.index == CAxTargetGroupPru::groupIndexKospi) 
	{
		for (int i = 0; i < ARRAY_SIZE(infop); i++) 
		{
			ListInsertItem(m_list, infop[i].code, infop[i].name, GU_INDEX);
		}
	}
	else if (m_kind.index == CAxTargetGroupPru::groupIndexKosdaq) 
	{
		for (int i = 0; i < ARRAY_SIZE(infod); i++) 
		{
			ListInsertItem(m_list, infod[i].code, infod[i].name, GU_INDEX);
		}
	}
}

void CAxTargetGroupPru::SetListIndexItem()
{
	CODEFILTERPROC proc = GetCodeFilterProc(m_kind.kind, m_kind.subkind, m_kind.index);
	if (proc)
		InsertListItem(m_hjcode, sizeof(struct hjcode), m_hjcount, proc, II_hjcode);
}

void CAxTargetGroupPru::SetListCompany()
{
	CString key;
	key.Format("%03d", m_kind.index);
	SetgroupFromInformationFile(m_tabFolder + GROUP_FILE, "GRCODE", key);
}

CString fillSpace(CString input, int len)
{
	CString buf = input;
	int	bufsize = buf.GetLength();
	if (len < bufsize)
	{
		buf = buf.Left(len);
	}
	for (int ii = 0; ii < len - bufsize; ii++)
	{
	//	buf += 0x20;  //vc2019 test
		buf += " ";
	}
	return buf;
}

void CAxTargetGroupPru::SetListUpDownRank()
{
	CString strIndex;
	strIndex.Format("%d", m_kind.index);
	
	rank_mid     mid;
	memset(&mid, '\0', sizeof(&mid));
	CopyMemory(mid.gubn, fillSpace(strIndex.Right(2), 2), 2);
	CopyMemory(mid.mark, fillSpace(strIndex.Mid(1,1), 1), 1);
	
	int size = sizeof(mid);

	m_lastTRCode = RANK_TRCODE;
	m_bar->SendRequestFromGroup(RANK_TRCODE, (LPSTR)&mid, size);
}

// 자동 - 보유종목
void CAxTargetGroupPru::SetListHoldItem()
{
	HTREEITEM hItem = m_listTree->GetSelectedItem();
	if (!hItem) return;

	CString acct = m_listTree->GetItemText(hItem);
	int find = acct.Find(' ');
	if (find > 0)
		acct = acct.Left(find);

	acct.Replace("-", "");
	if (acct.GetLength() != 11) 
		return;

	CString pass;
	CAxTargetGroupPru::GetAccountPassword(m_bar->GetView(), acct, pass);

//pass = "0423"; 
	if (pass.IsEmpty()) 
	{
		if (CAxTargetGroupPru::IsStaff(m_bar->GetView()))	// 직원일때만 체크
		{
			const char* msg = "계좌비밀번호 저장 후 사용하십시오.";
			AfxGetMainWnd()->MessageBox(msg, COMPANYNAME);
			return;
		}
		pass = "    ";
	}

//	CString enc;
//	CAxTargetGroupPru::EncrytoAccountPassword(m_bar->GetView(), acct, pass, enc);

	CString	svcn = "";
	CString	sGubn = "";
	if (IsFutureAccount(acct))
	{
		svcn = "SONBQ021";
		sGubn.Format("%c", mkty_FUTURE);
	}
	else
	{
		svcn = "SONAQ052";
		sGubn.Format("%c", mkty_STOCK);
	}

	CString sData = ledgerTR(acct, svcn, sGubn);

	CString	trCode;
	if (IsFutureAccount(acct))
	{
		struct f_mid mid;
		
		FillMemory(&mid, L_fmid, ' ');
		CopyMemory(&mid.accn, acct, sizeof(mid.accn));
		CopyMemory(&mid.pswd, pass, pass.GetLength());

		sData += CString((char*)&mid, L_fmid);	
		trCode = ACC_FJANGO;
	}
	else
	{
		struct s_mid mid;
		
		FillMemory(&mid, L_smid, ' ');
		CopyMemory(&mid.accn, acct, sizeof(mid.accn));
		CopyMemory(&mid.pswd, pass, pass.GetLength());
		mid.sygb[0] = '1';
		mid.fees[0] = '0';

		sData += CString((char*)&mid, L_smid);
		trCode = ACC_SJANGO;
	}

	m_lastTRCode = ACC_TRCODE;
	m_bar->SendRequestFromGroup(trCode, (LPSTR)sData.operator LPCTSTR(), sData.GetLength(), 0);
}

bool CAxTargetGroupPru::IsFutureAccount(CString sAccount)
{
	if (sAccount.GetLength() < 5)
		return false;

	CString	sGubn = sAccount.Mid(3, 2);
	if (sGubn == "20" || sGubn == "21") // 선물계좌이면
		return true;

	return false;
}

CString CAxTargetGroupPru::ledgerTR(CString sAccn, CString sSvcn, CString sGubn)
{
	struct _ledgerH ledger;
	FillMemory(&ledger, L_ledgerH, ' ');

	m_bar->GetView()->SendMessage(WM_USER, ledgerDLL, (LPARAM)&ledger);
	CString	strUser = (LPCTSTR)m_bar->GetView()->SendMessage(WM_USER, MAKEWPARAM(variantDLL, userCC), 0);
	
	CopyMemory(&ledger.svcd, sSvcn, sizeof(ledger.svcd));
	CopyMemory(&ledger.usid, (LPCTSTR)strUser, strUser.GetLength());
	CopyMemory(&ledger.brno, sAccn.Left(3), 3);
	CopyMemory(&ledger.rcnt, _T("0000"), sizeof(ledger.rcnt));

	ledger.fkey[0] = 'C';
	ledger.mkty[0] = sGubn.GetAt(0);
	ledger.odrf[0] = odty_QUERY;

	return CString((char*)&ledger, L_ledgerH);
}


void CAxTargetGroupPru::SetListHistory()
{
}

void CAxTargetGroupPru::SetListInterestItem()
{
	CStringArray codes, names;
	CByteArray	kinds;

	GetInterestItemList(m_userFolder, m_kind.index, codes, names, kinds);

	for (int i = 0; i < codes.GetSize(); i++) 
	{
		m_list->InsertItem(i, codes.GetAt(i));
		m_list->SetItemText(i, 1, names.GetAt(i));
		m_list->SetItemData(i, ConvertItemKind(kinds.GetAt(i)));
	}
}

void CAxTargetGroupPru::SetListThemeItem()
{
	CString key;
	key.Format("%03d", m_kind.index);
	SetgroupFromInformationFile(m_tabFolder + THEME_FILE, "TMCODE", key);
}

void CAxTargetGroupPru::SetgroupFromInformationFile(LPCTSTR fileName, LPCTSTR section, LPCTSTR key)
{
	char buffer[1024];
	GetPrivateProfileString(section, key, EMPTYSTR, buffer, ARRAY_SIZE(buffer), fileName);

	InsertListItem(buffer);
}

void CAxTargetGroupPru::InsertListItem(LPVOID arr, int recSize, int count, CODEFILTERPROC proc, ITEMINSERTPROC itemProc)
{
	LPBYTE node;
	for (int i = 0; i < count; i++) 
	{
		node = (LPBYTE)arr + (recSize * i);
		if (proc(node, m_kind.kind, m_kind.subkind, m_kind.index)) 
		{
			itemProc(node, m_list);
		}
	}
}

void CAxTargetGroupPru::InsertListItem(LPCTSTR lpszBuffer)
{
	CString str(lpszBuffer);

	CString code, name;
	LPVOID p;

	do 
	{
		code = XParse(str, ';');
		name = GetCodeName(code, &p);

		if (p)
			II_pubcode(code, name, m_list, p);
	} while (!str.IsEmpty());
}

#define GETNAME(a) CString(a->hnam, ARRAY_SIZE(a->hnam))

CString CAxTargetGroupPru::GetCodeName(CString code, LPVOID* ptrBuffer)
{
	const int stockCodeLen = 6, upCodeLen = 3, futureCodeLen = 8, optionCodeLen = 8, stockOptionLen = 8;
	int i;

	if (code.GetLength() == stockCodeLen && atoi(code) > 0) 
	{

		if (atoi(code) < 500000) 
		{
			struct hjcode* hj;
			for (i = 0; i < m_hjcount; i++) 
			{
				hj = &m_hjcode[i];
				if (code == CString(&hj->code[1], stockCodeLen)) 
				{
					if (ptrBuffer) *ptrBuffer = hj;
					return CString(&hj->hnam[1], HNameLen - 1);
				}
			}
		}
		else 
		{
			struct elwcode* elw;
			for (i = 0; i < m_elwcount; i++) 
			{
				elw = &m_elwcode[i];
				if (code == CString(elw->codx, stockCodeLen)) 
				{
					if (ptrBuffer) *ptrBuffer = elw;
					return CString(elw->hnam, HNameLen - 1);
				}
			}
		}
	}
	else if (code.GetLength() == upCodeLen && atoi(code) > 0) 
	{
		struct upcode* up;
		CString upcode;
		for (i = 0; i < m_upcount; i++) 
		{
			up = &m_upcode[i];

			upcode.Format("%d%02d", up->jgub, up->ucod);
			if (code == upcode) 
			{
				if (ptrBuffer) *ptrBuffer = up;
				return GETNAME(up);
			}
		}
	}
	else if (code.GetLength() == futureCodeLen && atoi(code.Left(3)) > 0) 
	{
		struct fjcode* fj;
		for (i = 0; i < m_fjcount; i++) 
		{
			fj = &m_fjcode[i];

			if (code == CString(fj->cod2, futureCodeLen)) 
			{
				if (ptrBuffer) *ptrBuffer = fj;
				return GETNAME(fj);
			}
		}

		struct ojcode* oj;
		int j;

		for (i = 0; i < m_ojcount; i++) 
		{
			oj = &m_ojcode[i];

			for (j = 0; j < 11; j++) 
			{
				if (code == CString(oj->call[j].cod2, optionCodeLen)) 
				{
					if (ptrBuffer) *ptrBuffer = oj;
					return CString(oj->call[j].hnam, ARRAY_SIZE(oj->call[j].hnam));
				}
				if (code == CString(oj->put[j].cod2, optionCodeLen)) 
				{
					if (ptrBuffer) *ptrBuffer = oj;
					return CString(oj->put[j].hnam, ARRAY_SIZE(oj->put[j].hnam));
				}
			}
		}

		struct pjcode* pj;
		for (i = 0; i < m_pjcount; i++) 
		{
			pj = &m_pjcode[i];

			if (code == CString(pj->codx, stockOptionLen)) 
			{
				if (ptrBuffer) *ptrBuffer = pj;
				return GETNAME(pj);
			}
		}
	}


	if (ptrBuffer)
		*ptrBuffer = NULL;
	return EMPTYSTR;
}


void CAxTargetGroupPru::NormalizeStockOptions()
{	
	LPTARGETGROUP p;
	int lastItem = 0, item, find;
	struct pjcode* pj;
	CString name;

	for (int i = 0; i < m_pjcount; i++) 
	{
		pj = &m_pjcode[i];
		item = atoi(CString(pj->tjgb, ARRAY_SIZE(pj->tjgb)));
		if (lastItem != item) 
		{
			p = new TARGETGROUP;
			ZeroMemory(p, sizeof(TARGETGROUP));
			name = CString(pj->hnam, ARRAY_SIZE(pj->hnam));
			name.TrimRight();
			
			find = name.Find(SPACE);
			ASSERT(find >= 0);
			name = name.Left(find);

			strcpy(p->name, name);
			p->group = item;
			lastItem = item;

			m_stockOptions.Add(p);
		}
	}
}

void CAxTargetGroupPru::ClearStockOptions()
{
	for (int i = 0; i < m_stockOptions.GetSize(); i++) 
	{
		delete m_stockOptions.GetAt(i);
	}

	m_stockOptions.RemoveAll();
}

void CAxTargetGroupPru::RecvData(WPARAM wParam, LPARAM lParam)
{
	if (m_lastTRCode == RANK_TRCODE) 
	{
		rank_mod* mod = (rank_mod*)lParam;
		int recCount = atoi(CString(mod->nrec, ARRAY_SIZE(mod->nrec)));

		CString code, name;

		for (int i = 0; i < recCount; i++) 
		{
			code = CString(mod->grid[i].code, ARRAY_SIZE(mod->grid[i].code));
			name = CString(mod->grid[i].hnam, ARRAY_SIZE(mod->grid[i].hnam));
			code.TrimRight();
			name.TrimRight();

			m_asyncList->InsertItem(i, code);
			m_asyncList->SetItemText(i, 1, name);
		}
	}
	else if (m_lastTRCode == ACC_TRCODE) 
	{
		int	len = HIWORD(wParam);
		struct _ledgerH *ledger = (_ledgerH*)lParam;	

		CString	sErrCode = CString((char*)ledger->emsg, 4);
		CString	sErrText = CString((char*)ledger->emsg, 94);
		if (sErrCode.GetAt(0) != _T('0'))	// 오류
			return;

		char	*pJango = (char*)(lParam + L_ledgerH);
		CString sAccn = CString((char*)pJango, 11), sCode, sName, str;
		int nrec = 0;
		int sizeL = 0;
		int checkL = 0;

		if (sAccn.IsEmpty())	return;

		if (IsFutureAccount(sAccn))
		{
			struct f_mod	fmod;
			CopyMemory(&fmod, pJango, L_fmod);
			nrec = atoi(CString(fmod.nrec, sizeof(fmod.nrec)));
			pJango += L_fmod;
			sizeL += L_fmod;	
			checkL = L_fmod + L_frec * nrec;

			for (int ii = 0; ii < nrec; ii++)
			{
				struct f_rec *frec = (struct f_rec *)pJango;
				sCode = CString(frec->cod2, sizeof(frec->cod2));	sCode.TrimRight();
				sName = CString(frec->hnam, sizeof(frec->hnam));	sName.TrimRight();
				m_asyncList->SetItemText(m_asyncList->InsertItem(m_asyncList->GetItemCount(), sCode), 1, sName);

				pJango += L_frec;	sizeL += L_frec;
				if (sizeL > len || sizeL > checkL)
					break;
			}
		}
		else
		{
			struct s_mod	smod;
			CopyMemory(&smod, pJango, L_smod);
			nrec = atoi(CString(smod.nrec, sizeof(smod.nrec)));
			pJango += L_smod;
			sizeL += L_smod;	
			checkL = L_smod + L_srec * nrec;

			for (int ii = 0; ii < nrec; ii++)
			{
				struct s_rec *srec = (struct s_rec *)pJango;
				sCode = CString(srec->cod2, sizeof(srec->cod2));	sCode.TrimRight();
				if (!sCode.IsEmpty() && sCode.GetAt(0) == 'A' || sCode.GetAt(0) == 'J')
					sCode = sCode.Mid(1);

				sName = CString(srec->hnam, sizeof(srec->hnam));	
				sName.TrimRight();

				CString	sGubn = CString(srec->jgyh, sizeof(srec->jgyh));
				CString	sCredit = _T("");
				switch (atoi(sGubn))
				{
				case 1:		
					sCredit = _T("(신용)");	
					break;
				case 2:		case 3:		case 4:		case 5:
					sCredit = _T("(대용)");	
					break;
				default:
					break;
				}

				m_asyncList->SetItemText(m_asyncList->InsertItem(m_asyncList->GetItemCount(), sCode), 1, sName + sCredit);

				pJango += L_srec;	sizeL += L_srec;
				if (sizeL > len || sizeL > checkL)
					break;
			}
		}
	}
}

struct _sortStr
{
	CString code;
	CString name;
};

int _sortCode(const void *arg1, const void *arg2)
{
	struct _sortStr* pLVal= (struct _sortStr*)arg1;
	struct _sortStr* pRVal= (struct _sortStr*)arg2;
	return pLVal->code.Compare(pRVal->code);
}

int _sortName(const void *arg1, const void *arg2)
{
	struct _sortStr* pLVal= (struct _sortStr*)arg1;
	struct _sortStr* pRVal= (struct _sortStr*)arg2;
   	return pLVal->name.Compare(pRVal->name);
}

void CAxTargetGroupPru::SetTreeItem_ELW(HTREEITEM hItemList, HTREEITEM hItemAsset)
{
	KIND kindList, kindAsset;
	memcpy(&kindList, &m_kind, sizeof(KIND));
	memcpy(&kindAsset, &m_kind, sizeof(KIND));

	kindList.subkind = groupELW_List;
	kindAsset.subkind = groupELW_Asset;

	struct elwcode* code;

	ELWCode save;

	int i, j, k;
	BOOL bFound;
	CString bcode, tmp;

	struct _sortStr* ss1, *ss2;
	int num1 = 0, num2 = 0;

	ss1 = new _sortStr[m_elwcount];
	ss2 = new _sortStr[m_elwcount];

	for (i = 0; i < m_elwcount; i++) 
	{
		code = &m_elwcode[i];

		save.code = CString(code->codx, ELWCodeLen);
		save.name = CString(code->hnam, ELWHNameLen);
		save.name.TrimRight();

		save.ktype = CString(code->krye, ARRAY_SIZE(code->krye));
		save.pcode = CString(code->bgce, ARRAY_SIZE(code->bgce));
		save.pname = CString(code->hbge, ARRAY_SIZE(code->hbge));
		save.pname.TrimRight();
		save.mjmt  = CString(code->mjmt, ARRAY_SIZE(code->mjmt));

		for (j = 0; j < 5; j++) 
		{
			save.bcode[j] = CString((LPSTR)code->gcjs[j], 12);
			save.bcode[j].TrimRight();
		}

		bFound = FALSE;

		for (k = 0; k < num1; k++) 
		{
			if (!save.pcode.CompareNoCase(ss1[k].code)) 
			{
				bFound = TRUE;
				break;
			}
		}

		if (!bFound) 
		{
			ss1[num1].code = save.pcode;
			ss1[num1].name = save.pname;
			num1++;
		}

		for (j = 0; j < 5; j++) 
		{
			bcode.Format("%s", save.bcode[j]);
			bcode.TrimRight();

			if (bcode.IsEmpty()) 
				break;

			bFound = FALSE;

			for (k = 0; k < num2; k++) 
			{
				if (!bcode.CompareNoCase(ss2[k].code)) 
				{
					bFound = TRUE;
					break;
				}
			}
			if (!bFound) 
			{
				tmp = GetCodeName(bcode.Mid(1));
				tmp.TrimRight();
				if (bcode.GetLength() == 7 && !tmp.IsEmpty())
					ss2[num2].name = tmp;
				else
					continue;
				ss2[num2].code = bcode;
				num2++;
			}
		}
	}

	// 기초자산(이름순)
	m_kind.subkind = groupELW_Asset;
	qsort((void*)ss2, num2, sizeof(struct _sortStr), _sortName);

	for (i = 0; i < num2; i++) 
	{
		m_kind.index = i;
		save.code = ss2[i].code;
		save.name = ss2[i].name;
		m_arrELW.Add(save);

		m_tree->SetItemData(m_tree->InsertItem(ss2[i].name, hItemAsset), CAxTargetItem::KindToKey(m_kind));
	}	

	m_kind.index = 0xFFFF;
	save.code.Format("%03d", ID_KOSPI200);
	save.name = "KOSPI200";
	m_arrELW.Add(save);
	m_tree->SetItemData(m_tree->InsertItem(save.name, hItemAsset), CAxTargetItem::KindToKey(m_kind));

//	m_kind.index = 0xFFFE;
//	save.code.Format("%03d", 888);
//	save.name = "바스켓";
//	m_arrELW.Add(save);
//	m_tree->SetItemData(m_tree->InsertItem(save.name, hItemAsset), CAxTargetItem::KindToKey(m_kind));
//

	// 발행기관(코드순)
	BASECode	base;
	m_arrBase.RemoveAll();

	m_kind.subkind = groupELW_List;
	qsort((void*)ss1, num1, sizeof(struct _sortStr), _sortCode);
	for (i = 0; i < num1; i++) 
	{
		m_kind.index = i;
		base.code = ss1[i].code;
		base.name = ss1[i].name;
		m_arrBase.Add(base);
		m_tree->SetItemData(m_tree->InsertItem(ss1[i].name, hItemList), CAxTargetItem::KindToKey(m_kind));
	}

	delete [] ss1;
	delete [] ss2;
}
