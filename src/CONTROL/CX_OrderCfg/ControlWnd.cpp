// ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_OrderCfg.h"
#include "ControlWnd.h"
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
#include "../../h/Ledger.h"
#include <math.h>
#include <AtlBase.h>
#include "CfgWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define KEY_HOGA	1
#define	KEY_SONBQ101	2

const	int	CURR = 0;
const	int	SELL1 = 1,  SELL2 = 2,  SELL3 = 3,  SELL4 = 4,  SELL5 = 5;
const	int	BUY1 = 6,   BUY2 = 7,   BUY3 = 8,   BUY4 = 9,   BUY5 = 10;
const	int	SELL6 = 11, SELL7 = 12, SELL8 = 13, SELL9 = 14, SELL10 = 15, MAXL = 16;
const	int	BUY6 = 17, BUY7 = 18, BUY8 = 19, BUY9 = 20, BUY10 = 21, MINL = 22;
const	int	ORDUNIT = 23;

const	char*	spriceSymbol[CNT_STOCK] = { "2023",					// "현재가",
				"3051", "3052", "3053", "3054", "3055",			// "1차매도가", "2차매도가", "3차매도가", "4차매도가", "5차매도가",
				"3071", "3072", "3073", "3074", "3075",			// "1차매수가", "2차매수가", "3차매수가", "4차매수가", "5차매수가",
				"3056", "3057", "3058", "3059", "3060", "2302",		// "6차매도가", "7차매도가", "8차매도가", "9차매도가", "10차매도가", "상한가",
				"3076", "3077", "3078", "3079", "3080", "2303",		// "6차매수가", "7차매수가", "8차매수가", "9차매수가", "10차매수가", "하한가",
				"1377" };						// 주문수량단위

const	char*	fpriceSymbol[CNT_FUTURE] = {"1023",				// "현재가",
				"2051", "2052", "2053", "2054", "2055",		// "1차매도가", "2차매도가", "3차매도가", "4차매도가", "5차매도가",
				"2071", "2072", "2073", "2074", "2075" };	// "1차매수가", "2차매수가", "3차매수가", "4차매수가", "5차매수가",

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd()
{
	EnableAutomation();

	m_pParent	= nullptr;
	m_cfg		= nullptr;
	m_sTxCode	= "POOPPOOP";
	m_nKind		= 1;		// 1.주식, 2:선물옵션, 3:KOPEX
	m_sPushPrefix	= "";
	m_mmgb		= "00";
	m_orderGB	= "0";		// 0.매도 1.매수, 2.정정, 3.취소
	m_price		= "0";
	m_rtsCd		= "";
	m_keepMM	= TRUE;
	m_keepQty	= TRUE;
	m_keepPrc	= TRUE;

	m_bDoAlert	= false;
	m_bQueryQty	= false;

	m_bNewUnit	= FALSE;

	for (int ii = 0; ii < 24; ii++)
		m_sise[ii] = "";
}

CControlWnd::~CControlWnd()
{
}

void CControlWnd::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CControlWnd, CWnd)
	//{{AFX_MSG_MAP(CControlWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_PROPERTY_NOTIFY(CControlWnd, "ItemCd", m_itemCd, OnItemCdChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "TimeOutOrder", m_timeOutOrder, OnTimeOutOrderChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CControlWnd, "OrderGB", m_orderGB, OnOrderGBChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "PushPrefix", m_pushPrefix, OnPushPrefixChanged, VT_BSTR)
	DISP_PROPERTY_NOTIFY(CControlWnd, "KeepMM", m_keepMM, OnKeepMMChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CControlWnd, "KeepQty", m_keepQty, OnKeepQtyChanged, VT_BOOL)
	DISP_PROPERTY_NOTIFY(CControlWnd, "KeepPrc", m_keepPrc, OnKeepPrcChanged, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "NoFee", _GetNoFee, _SetNoFee, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabAccount", _GetTabAccount, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabButton", _GetTabButton, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabCode", _GetTabCode, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabMMGB", _GetTabMMGB, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabPassword", _GetTabPassword, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabPrc", _GetTabPrc, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "TabQty", _GetTabQty, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "EnterOrder", _GetEnterOrder, SetEnterOrder, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "MMGB", _GetMMGB, _SetMMGB, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "OrderConfirm", _GetOrderConfirm, _SetOrderConfirm, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "Price", _GetPrice, _SetPrice, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "Qty", _GetQty, _SetQty, VT_BSTR)
	DISP_PROPERTY_EX(CControlWnd, "TabCondition", _GetTabCondition, SetNotSupported, VT_BOOL)
	DISP_PROPERTY_EX(CControlWnd, "OrdUnit", _GetOrdUnit, _SetOrdUnit, VT_BSTR)
	DISP_FUNCTION(CControlWnd, "ShowDlg", _ShowDlg, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "LoadInfo", _LoadInfo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "Pop", _Pop, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "Push", _Push, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "ReadCfg", _ReadCfg, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "WriteCfg", _WriteCfg, VT_EMPTY, VTS_BSTR VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "ClearPush", _ClearPush, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "QueryHoga", _QueryHoga, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SaveCfg", _SaveCfg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "LoadDefaultCfg", _LoadDefaultCfg, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "QueryQty", _QueryQty, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "SetPrivateCfgName", _SetPrivateCfgName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "SetPublicCfgName", _SetPublicCfgName, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "MoveTick", _MoveTick, VT_BSTR, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CControlWnd, "SetDoQty", _SetDoQty, VT_EMPTY, VTS_BOOL)
	DISP_FUNCTION(CControlWnd, "QueryPrcAbl", QueryPrcAbl, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_BSTR)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {1B2C8910-9301-467F-A9CC-794A9A1D25A7}
static const IID IID_IControlWnd =
{ 0x1b2c8910, 0x9301, 0x467f, { 0xa9, 0xcc, 0x79, 0x4a, 0x9a, 0x1d, 0x25, 0xa7 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void CControlWnd::OnDestroy() 
{
	m_bDoAlert = false;
	if (m_cfg)
		m_cfg.reset();

	CWnd::OnDestroy();
}

void CControlWnd::CreateCtrl(CWnd* pParent, struct _param* pParam)
{
	m_pParent = pParent;

	m_Param.key = pParam->key;
	m_Param.name = CString(pParam->name, pParam->name.GetLength());
	m_Param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_Param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_Param.point = pParam->point;
	m_Param.style = pParam->style;
	m_Param.tRGB = pParam->tRGB;
	m_Param.pRGB = pParam->pRGB;
	m_Param.options = CString(pParam->options, pParam->options.GetLength());
	
	Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPSIBLINGS, m_Param.rect, m_pParent, 100);

	CString str, options, sInfoSection, sCfgDlg;
	options = m_Param.options;

	str = OptionParser(options, "/x"); str.TrimRight();
	if (str.GetLength() > 0) m_sTxCode = str;

	sInfoSection = OptionParser(options, "/r");
	if (sInfoSection.IsEmpty()) sInfoSection = "DH010100";
	sCfgDlg = OptionParser(options, "/s");
	if (sCfgDlg.IsEmpty()) sCfgDlg = "DH0101C0";

	// Create Cfg
	struct	_param	param;
	CString	home, path;
	
	home = Variant(homeCC, "");
	path.Format(_T("%s/%s/axis.ini"), home, TABDIR);
	m_bNewUnit = GetPrivateProfileInt("K2023SyStem", "Newhgunit", 0, path);

	path.Format("%s/%s/%s", home, DEVDIR, "CX_CFG.DLL");
	param.name	= "Cfg";
	param.fonts	= "굴림체";
	param.point	= 9;
	param.style	= 0;
	param.tRGB	= 63;
	param.pRGB	= 62;
	param.options = "/fORDCFG.INI/mDH010100/cCOMMON/dDH1101C0";
	GetClientRect(&param.rect);

	m_cfg = std::make_unique<CCfg>();
	m_cfg->CreateCtrl(m_pParent, &param);
	m_cfg->SetMap(sInfoSection, "Common", sCfgDlg);

	m_sMmObj	= OptionParser(options, "/m");
	m_sPrcObj	= OptionParser(options, "/p");
	m_sQtyObj	= OptionParser(options, "/q");

	m_nKind		= atoi(OptionParser(options, "/k"));
}

CString CControlWnd::Variant(int comm, CString data)
{
	CString retvalue;
	const	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, comm), (LPARAM)data.GetString());

	if ((long)dta > 1)
		retvalue = dta;

	return retvalue;
}

CString CControlWnd::OptionParser(CString sOption, CString sKey)
{
	CString tmp = sOption;
	Parser(tmp, sKey);
	tmp = Parser(tmp, "/");
	return tmp;
}

CString CControlWnd::Parser(CString &srcstr, CString substr)
{
	if (srcstr.Find(substr) == -1)
	{
		CString temp = srcstr;
		srcstr.Empty();
		return temp;
	}

	CString  temp = srcstr.Left(srcstr.Find(substr));
	srcstr = srcstr.Mid(srcstr.Find(substr) + substr.GetLength());
	return temp;
}

void CControlWnd::sendTR(CString name, CString data, BYTE type, char key, BYTE interKey)
{
	int	idx = 0;
	struct	_userTH	udat{};
	const	int	datl = data.GetLength();
	std::unique_ptr<char[]> pWb = std::make_unique<char[]>(datl + L_userTH + 128);

	// Key & control name
	if ((type & US_KEY) == US_KEY)
		pWb[idx++] = interKey;
	CopyMemory(&pWb[idx], (char*)m_Param.name.GetString(), m_Param.name.GetLength());
	idx += m_Param.name.GetLength();
	pWb[idx++] = '\t';

	// userTH
	CopyMemory(udat.trc, (char*)name.GetString(), sizeof(udat.trc));
	udat.key = key;
	udat.stat = type;
	CopyMemory(&pWb[idx], &udat, L_userTH);
	idx += L_userTH;

	// data
	CopyMemory(&pWb[idx], (char*)data.GetString(), datl);

	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, datl), (LPARAM)pWb.get());
	pWb.reset();
}

long CControlWnd::OnMessage(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_OUBx:
		{
			CString	sval;
			struct _extTHx* exth = (struct _extTHx*)lParam;
			if (exth->key == KEY_SONBQ101)
			{
				struct _ledgerH* ledger = (struct _ledgerH*)exth->data;
				_SONBQ101_mod* mod = (_SONBQ101_mod*)(exth->data + L_ledgerH);

				CString ecod = CString(ledger->ecod, sizeof(ledger->ecod));
				CString emsg = CString(ledger->emsg, sizeof(ledger->emsg));

				if (atoi(ecod) < 1000)
				{
					sval.Format("%.16s", mod->lOrdAbleQty);
					SendMapVal(m_sQtyObj, sval);
				}
				else
					SendMapVal(m_sQtyObj, "0");
			}
			else
			{
				CString val, data((char*)exth->data, exth->size);
				const	int	pos = data.Find("\t");
				if (pos < 0)
					return 0;

				val = data.Left(pos);
				if (val.Find(m_itemCd) >= 0)
				{
					processHogaOUB(exth->data);
				}
			}
		}
		break;
	case DLL_OUB:	// NOT USED. User DLL_OUBx Only !!
		{
			CString sval;
			LPCSTR data = (LPCSTR)lParam;
			struct _ledgerH *ledger = (struct _ledgerH *)data;
			
			if (CString(ledger->svcd, 8) == "SONBQ101")
			{	
				_SONBQ101_mod *mod = (_SONBQ101_mod*)(data+L_ledgerH);
				
				CString ecod = CString(ledger->ecod, sizeof(ledger->ecod));
				CString emsg = CString(ledger->emsg, sizeof(ledger->emsg));
				
				if (atoi(ecod) < 1000)
				{
					sval.Format("%.16s", mod->lOrdAbleQty) ;
					SendMapVal(m_sQtyObj, sval);
				}
				else
					SendMapVal(m_sQtyObj, "0");
			}
			else
			{
				CString val, data((char*)lParam, HIWORD(wParam));
				const	int	pos = data.Find("\t");
				if (pos < 0)
					return 0;
				
				val = data.Left(pos);
				if (val.Find(m_itemCd) >= 0)
				{
					processHogaOUB((char*)lParam);	
				}
			}
		}
		break;
	case DLL_ALERTx:
		processAlertx((struct _alertR*)lParam);
		break;
	case DLL_ALERT:
	//	processAlert((char*)lParam);
		break;
	case variantDLL:
	case viewDLL:
		return m_pParent->SendMessage(WM_USER, wParam, lParam);
	default:
		break;
	}

	return 0;
}

void CControlWnd::DoQueryHoga()
{
	m_bDoAlert = false;
	m_rtsCd = "";
	m_itemCd.Trim();
	if (m_itemCd.IsEmpty())
		return;

	CString sTmp, sQry, sQueryField;
	if (m_nKind == 1) // 주식
	{
		sQry.Format("1301%c%s\t1021\t", 0x7f, m_itemCd);
		for (int ii = 0; ii < CNT_STOCK; ii++)
		{
			sTmp.Format("%s\t", spriceSymbol[ii]);
			sQry = sQry + sTmp;
		}
	}
	else if (m_nKind == 2) // 선물옵션
	{
		if (m_itemCd.IsEmpty())
			return;

		char	prefix = '\0';
		if (m_itemCd[0] == '1' || m_itemCd[0] == 'A')	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
			prefix = '3';
		else if (m_itemCd[1] == '0')	// 옵션
			prefix = '4';
		else				// 주식옵션
			prefix = '7';

		sQry.Format("%c%c301%c%s\t%c0021\t", prefix, '0', 0x7f, m_itemCd, prefix);
		for (int ii = 0; ii < CNT_FUTURE; ii++)
		{
			sTmp.Format("%c%s\t", prefix, fpriceSymbol[ii]);
			sQry = sQry + sTmp;
		}
	}
	else // KOFEX
	{
		sQry.Format("%c%c301%c%s\t%c0021\t", '5', '1', 0x7f, m_itemCd, '5');
		for (int ii = 0; ii < CNT_FUTURE; ii++)
		{
			sTmp.Format("%c%s\t", '5', fpriceSymbol[ii]);
			sQry = sQry + sTmp;
		}
	}

	sendTR(m_sTxCode, sQry, US_OOP | US_PASS | US_KEY, m_Param.key, KEY_HOGA);
}

void CControlWnd::processHogaOUB(char* stream)
{
	CString sTmp, sStream = CString(stream);
	int	pos = sStream.Find('\t');

	if (pos == -1)
		return;

	m_rtsCd = sStream.Left(pos++);
	sStream = sStream.Mid(pos);

	int	ii = 0;
	const	int	nLoop = (m_nKind == 1) ? CNT_STOCK : CNT_FUTURE;
	while (TRUE)
	{
		if (sStream.IsEmpty() || ii == nLoop)
			break;

		pos = sStream.Find('\t');
		if (pos == -1)
		{
			sTmp = sStream;
			sStream.Empty();
		} 
		else
		{
			sTmp = sStream.Left(pos++);
			sStream = sStream.Mid(pos);
		}
		sTmp.TrimLeft();
		sTmp.Replace("+", "");
		sTmp.Replace("-", "");
		m_sise[ii] = sTmp;

		ii++;
	}
	m_bDoAlert = true;

	ReturnHoga();
	if (m_bQueryQty)
	{
		m_bQueryQty = false;
		_QueryQty();
	}
}

void CControlWnd::processAlert(char* stream)
{
	if (!m_bDoAlert)
		return;

	int	pos;
	CString	sSymbol, sData;
	CMapStringToString mapRts;
	CString sBuffer = CString(stream);;

	pos = sBuffer.Find("\t");
	if (pos <= 0)
		return;

	sData = sBuffer.Left(pos++);
	sBuffer = sBuffer.Mid(pos);

	if (m_rtsCd.CompareNoCase(sData) != 0)
		return;

	while (!sBuffer.IsEmpty())
	{
		pos = sBuffer.Find("\t");
		if (pos < 0)
		{
			sSymbol = sBuffer;
			sBuffer.Empty();
		}
		else
		{
			sSymbol = sBuffer.Left(pos++);
			sBuffer = sBuffer.Mid(pos);
		}

		pos = sBuffer.Find("\t");
		if (pos < 0)
		{
			sData = sBuffer;
			sBuffer.Empty();
		}
		else
		{
			sData = sBuffer.Left(pos);
			sBuffer.Mid(pos);
		}

		if (sData.IsEmpty() || sSymbol.IsEmpty())
			continue;

		mapRts.SetAt(sSymbol, sData);
	}

	const	int	nLoop = (m_nKind == 1) ? CNT_STOCK : CNT_FUTURE;
	for (int ii = 0; ii < nLoop; ii++)
	{
		sSymbol = spriceSymbol[ii];
		sSymbol = sSymbol.Mid(1);

		if (mapRts.Lookup(sSymbol, sData))
		{
			sData.TrimLeft();
			sData.Replace("+", "");
			sData.Replace("-", "");
			m_sise[ii] = sData;
		}
	}
}

void CControlWnd::processAlertx(struct _alertR* alertR)
{
	if (!m_bDoAlert)
		return;

	if (m_rtsCd.CompareNoCase(alertR->code) != 0)
		return;

	CString	sym, data;
	CMap <int, int, CString, CString& > mapRts;
	DWORD*	ptr = (DWORD*)alertR->ptr[0];

	for (int ii = 1; ii < 1000; ii++)	// 0 : rts type
	{
		if (!ptr[ii])
			continue;

		data = (char*)ptr[ii];
		mapRts.SetAt(ii, data);
	}

	const	int	nLoop = (m_nKind == 1) ? CNT_STOCK : CNT_FUTURE;
	for (int ii = 0, isym = 0; ii < nLoop; ii++)
	{
		sym  = spriceSymbol[ii];
		isym = atoi(sym.Mid(1));

		if (mapRts.Lookup(isym, data))
		{
			data.TrimLeft();
			data.Replace("+", "");
			data.Replace("-", "");
			m_sise[ii] = data;
		}
	}
}

void CControlWnd::ReturnHoga()
{
	int	idx = -1;

	switch (atoi(m_orderGB))
	{
	case 0:	// 매도 (0값,현재가,매도1~10,매수1~10)
		idx = atoi(m_cfg->ReadCfg("SellPrcGb"));
		break; 
	case 1:	// 매수 (0값,현재가,매도1~10,매수1~10)
		idx = atoi(m_cfg->ReadCfg("BuyPrcGb"));
		break;
	case 2:	// 정정
		break;
	case 3:	// 취소
		break;
	}

	if (idx < 1)  // 호가 초기화 ("0")
	{
		CString val;
		if (m_nKind = 1)
			val = "0";
		else
			val = "0.00";
		SendMapVal(m_sPrcObj, val);
	}
	else
	{
		int	idxVal = -1;

		switch (idx)
		{
		case 1: // 현재가
		case 2: // 매도1호가
		case 3:
		case 4:
		case 5:
		case 6:	// 매도5호가
			idxVal = idx -1;
			break;
		case 7: // 매도6호가
		case 8:
		case 9:
		case 10:
		case 11:// 매도10호가
			idxVal = SELL6 + (idx - 7);
			break;
		case 12:// 매수1호가
		case 13:
		case 14:
		case 15:
		case 16:// 매수5호가
			idxVal = BUY1 + (idx - 12);
			break;
		case 17:// 매수6호가
		case 18:
		case 19:
		case 20:
		case 21:// 매수10호가
			idxVal = BUY6 + (idx - 17);
			break;
		default:break;
		}
		if (idxVal >= 0)
			SendMapVal(m_sPrcObj, m_sise[idxVal]);
	}
}

void CControlWnd::SendMapVal(CString obj, CString val)
{
	if (obj.IsEmpty())
		return;

	CString send;

	send.Format("%s\t%s", obj, val);
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(formDLL, m_Param.key), (LPARAM)send.GetString());
}

BOOL CControlWnd::_GetTabAccount()		{	return m_cfg->ReadCfg("TabAccount") != "0";		}
BOOL CControlWnd::_GetTabButton()		{	return m_cfg->ReadCfg("TabButton") != "0";		}
BOOL CControlWnd::_GetTabCode()			{	return m_cfg->ReadCfg("TabCode") != "0";		}
BOOL CControlWnd::_GetTabMMGB()			{	return m_cfg->ReadCfg("TabMMGB") != "0";		}
BOOL CControlWnd::_GetTabPassword()		{	return m_cfg->ReadCfg("TabPassword") != "0";		}
BOOL CControlWnd::_GetTabPrc()			{	return m_cfg->ReadCfg("TabPrc") != "0";			}
BOOL CControlWnd::_GetTabQty()			{	return m_cfg->ReadCfg("TabQty") != "0";			}
BOOL CControlWnd::_GetTabCondition()		{	return m_cfg->ReadCfg("TabCondition") != "0";		}
BOOL CControlWnd::_GetEnterOrder()		{	return m_cfg->ReadCfg("EnterOrder") == "1";		}
void CControlWnd::SetEnterOrder(BOOL bNewValue) {	m_cfg->WriteCfg("EnterOrder", bNewValue ? "1" : "0");	}
void CControlWnd::OnTimeOutOrderChanged()	{}
void CControlWnd::OnPushPrefixChanged()		{}
void CControlWnd::OnKeepMMChanged()		{	m_cfg->WriteCfg("KeepMM", m_keepMM ? "1" : "0");	}
void CControlWnd::OnKeepQtyChanged()		{	m_cfg->WriteCfg("KeepQty", m_keepQty ? "1" : "0");	}
void CControlWnd::OnKeepPrcChanged()		{	m_cfg->WriteCfg("KeepPrc", m_keepPrc ? "1" : "0");	}

void CControlWnd::OnItemCdChanged()		
{ 
	m_bDoAlert = false;
	for (int ii = 0; ii < 24; ii++)
		m_sise[ii] = "";
	/*DoQueryHoga();*/
}

void CControlWnd::OnOrderGBChanged() 
{
	if (m_orderGB.GetLength() < 1)
		m_orderGB = "0";
}

BOOL CControlWnd::_GetOrderConfirm() 
{	
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, confirmCC), 0);
}

void CControlWnd::_SetOrderConfirm(BOOL bNewValue) 
{
}

BOOL CControlWnd::_ShowDlg() 
{
	return m_cfg->ShowDlg();
}

void CControlWnd::_LoadInfo() 
{
	m_cfg->LoadCfg();
	m_keepMM	= atoi(m_cfg->ReadCfg("KeepMM"))  ? TRUE : FALSE;
	m_keepQty	= atoi(m_cfg->ReadCfg("KeepQty")) ? TRUE : FALSE;
	m_keepPrc	= atoi(m_cfg->ReadCfg("KeepPrc")) ? TRUE : FALSE;
}

BSTR CControlWnd::_GetMMGB() 
{
	CString strResult(m_mmgb);
	return strResult.AllocSysString();
}

void CControlWnd::_SetMMGB(LPCTSTR lpszNewValue) 
{
	m_mmgb = lpszNewValue;
}

BSTR CControlWnd::_GetPrice() 
{
	CString strResult(m_price);
	//if (!m_keepPrc)
	//{
	//	switch (m_nKind)
	//	{
	//	case 2: // 선물옵션
	//		strResult = "0.00";
	//		break;
	//	case 3: // KOFEX
	//		strResult = "0.00";
	//		break;
	//	//case 1: // 주식
	//	default:
	//		strResult = "0";
	//		break;
	//	}
	//}
	return strResult.AllocSysString();
}

void CControlWnd::_SetPrice(LPCTSTR lpszNewValue) 
{
	m_price = lpszNewValue;
}

BSTR CControlWnd::_GetQty() 
{
	CString strResult(m_qty);
	//if (!m_keepQty) strResult = "0";

	return strResult.AllocSysString();
}

void CControlWnd::_SetQty(LPCTSTR lpszNewValue) 
{
	m_qty = lpszNewValue;
}

BSTR CControlWnd::_ReadCfg(LPCTSTR Key) 
{
	CString strResult(m_cfg->ReadCfg(Key));
//TRACE("r %s,%s\n", Key, strResult);
	return strResult.AllocSysString();
}

void CControlWnd::_WriteCfg(LPCTSTR Key, LPCTSTR Value) 
{
//TRACE("w %s,%s\n", Key, Value);
	m_cfg->WriteCfg(Key, Value);
}

void CControlWnd::_Pop() 
{
	m_qty = Variant(popCC, m_sPushPrefix + "SBJCQty");
	switch (atoi(m_orderGB))
	{
	case 0: // 매도
		m_price = Variant(popCC, m_sPushPrefix + "SBJPrc");
		m_mmgb	= Variant(popCC, m_sPushPrefix + "SBMMGB");
		break;
	case 1: // 매수
		m_price = Variant(popCC, m_sPushPrefix + "SBJPrc");
		m_mmgb	 = Variant(popCC, m_sPushPrefix + "SBMMGB");
		break;
	case 2: // 정정
		m_price = Variant(popCC, m_sPushPrefix + "SBJPrc");
		if (m_nKind == 1)
			m_mmgb	= Variant(popCC, m_sPushPrefix + "JCMMGB");
		else if (m_nKind == 2)
			m_mmgb	= Variant(popCC, m_sPushPrefix + "SBMMGB");
		else
			m_mmgb	= Variant(popCC, m_sPushPrefix + "KBMMGB");
		break;
	case 3: // 취소
		m_mmgb	 = Variant(popCC, m_sPushPrefix + "JCMMGB");
		break;
	}
}

void CControlWnd::_Push() 
{
	CString sTmp;

	sTmp.Format("%s\t%s", m_sPushPrefix + "SBJCQty", m_qty);
	Variant(pushCC,	sTmp);

	switch (atoi(m_orderGB))
	{
	case 0: // 매도
		sTmp.Format("%s\t%s", m_sPushPrefix + "SBJPrc", m_price);
		Variant(pushCC,	sTmp);
		sTmp.Format("%s\t%s", m_sPushPrefix + "SBMMGB", m_mmgb);
		Variant(pushCC,	sTmp);
		break;
	case 1: // 매수 
		sTmp.Format("%s\t%s", m_sPushPrefix + "SBJPrc", m_price);
		Variant(pushCC,	sTmp);
		sTmp.Format("%s\t%s", m_sPushPrefix + "SBMMGB", m_mmgb);
		Variant(pushCC,	sTmp);
		break;
	case 2: // 정정
		sTmp.Format("%s\t%s", m_sPushPrefix + "SBJPrc", m_price);
		Variant(pushCC,	sTmp);
		if (m_nKind == 1)
			sTmp.Format("%s\t%s", m_sPushPrefix + "JCMMGB", m_mmgb);
		else if (m_nKind == 2)
			sTmp.Format("%s\t%s", m_sPushPrefix + "SBMMGB", m_mmgb);
		else
			sTmp.Format("%s\t%s", m_sPushPrefix + "KBMMGB", m_mmgb);
		Variant(pushCC,	sTmp);
		break;
	case 3: // 취소
		sTmp.Format("%s\t%s", m_sPushPrefix + "JCMMGB", m_mmgb);
		Variant(pushCC,	sTmp);
		break;
	}
}

void CControlWnd::_ClearPush() 
{
	CString	sTmp;

	sTmp.Format("%s\t%s", m_sPushPrefix + "SBJCQty", "");
	Variant(pushCC,	sTmp);

	sTmp.Format("%s\t%s", m_sPushPrefix + "SBJPrc", "");
	Variant(pushCC,	sTmp);

	sTmp.Format("%s\t%s", m_sPushPrefix + "SBMMGB", "");
	Variant(pushCC,	sTmp);

	sTmp.Format("%s\t%s", m_sPushPrefix + "JCMMGB", "");
	Variant(pushCC,	sTmp);

	_LoadInfo();
}

void CControlWnd::_SaveCfg() 
{
	// save private information only, public information is managed by main program(AXIS.EXE)
	m_cfg->SaveCfg(true);
}

void CControlWnd::_LoadDefaultCfg() 
{
	m_cfg->LoadDefaultCfg();
}

void CControlWnd::_QueryHoga() 
{
	if (!m_bDoAlert)
	{
		DoQueryHoga();
		return;
	}
	ReturnHoga();
}

void CControlWnd::_QueryQty() 
{
	if (m_bDoAlert == false)
	{
		m_bQueryQty = true;
		return;
	}

	int	qty = 0;
	CString opt, val;

	opt.Empty();
	switch (atoi(m_orderGB))
	{
	case 0: // 매도
		opt = m_cfg->ReadCfg("SellQtyGB");
		switch (atoi(opt))
		{
		case 0: // 사용자 설정수량
			qty = atoi(m_cfg->ReadCfg("SellFixQty"));
			if (m_nKind == 1) qty = GetAvailQty(qty);
			val.Format("%d", qty);
			SendMapVal(m_sQtyObj, val);
			return;
		case 1: // 가능 수량 or 보유잔고
			return;
		case 2: // 금액 매도
			val = m_cfg->ReadCfg("SellFixPrcQty");
			break;
		}
		break;
	case 1: // 매수 
		opt = m_cfg->ReadCfg("BuyQtyGB");
		switch (atoi(opt))
		{
		case 0: // 사용자 설정수량
			qty = atoi(m_cfg->ReadCfg("BuyFixQty"));
			if (m_nKind == 1) qty = GetAvailQty(qty);
			val.Format("%d", qty);
			SendMapVal(m_sQtyObj, val);
			return;
		case 1: // 미수가능수량
		case 3: // 현금가능수량
			return;
		case 2: // 금액매수
			val = m_cfg->ReadCfg("BuyFixPrcQty");
			break;
		}
		break;
	case 2: // 정정
		SendMapVal(m_sQtyObj, "0");
		return;
	case 3: // 취소
		SendMapVal(m_sQtyObj, "0");
		return;
	}

	if (atoi(opt) == 2) // 금액매수/매도 수량조회
	{
		switch (m_nKind)
		{
		case 1: // 주식
			if (atof(m_price) > 0)
			{
				qty = (int)floor(atof(val) / atof(m_price));
				qty = GetAvailQty(qty);
			}
			else
			{
				qty = 0;
			}
			break;
		case 2: // 선물옵션
			if (atof(m_price) > 0)
			{
				if (!m_itemCd.IsEmpty() && (m_itemCd[0] == '1' || m_itemCd[0] == 'A'))	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
				{
					qty = (int)floor(atof(val) / (atof(m_price) * 500000 * 0.15));
				}
				else 
					qty = (int)floor(atof(val) / (atof(m_price) * 100000));
			} 
			else
			{
				qty = 0;
			}
			break;
		default: //case 3: // KOFEX
			if (atof(m_price) > 0)
				qty = (int)floor(atof(val) / (atof(m_price) * 200000));
			else
				qty = 0;
			break;
		}
		if (m_nKind == 1) qty = GetAvailQty(qty);
		val.Format("%d", qty);
		SendMapVal(m_sQtyObj, val);
		return;
	}
}

int CControlWnd::GetAvailQty(int qty)
{
	int	nUnit = atoi(m_sise[ORDUNIT]);

	if (nUnit <= 0 || nUnit >= 100)
		nUnit = 1;

	if (nUnit <= 0)
		return qty;

	return (int)floor(qty / nUnit) * nUnit;
}

void CControlWnd::_SetPrivateCfgName(LPCTSTR PrivateCfgName) 
{
	m_cfg->SetPrivateCfgName(PrivateCfgName);
}

void CControlWnd::_SetPublicCfgName(LPCTSTR PublicCfgName) 
{
	m_cfg->SetPublicCfgName(PublicCfgName);
}

BSTR CControlWnd::_MoveTick(LPCTSTR Prc, short Tick) 
{
	CString strResult;
	int	nVal = 0, nDelta = 0;
	double	dVal = 0, dDelta = 0;

	switch (m_nKind)
	{
	case 2: // 선물옵션
		dVal = atof(Prc);
		if (m_itemCd.IsEmpty()) 
		{
			strResult = Prc;
			break;
		}

		if (m_itemCd[0] == '1' || m_itemCd[0] == 'A')	// 20230125 파생상품 코드 개편  '1', 'A' : 선물
		{
			dDelta = 0.05;
		}
		else if (m_itemCd[1] == '0')	// 옵션
		{
			if (dVal < 3)
				dDelta = 0.01;		
			else
			{
				dDelta = 0.05;
				if (Tick < 0 && dVal == 3) dDelta = 0.01;
			}
		}
		else				// 주식옵션
		{
			if (dVal < 1000)
			{
				dDelta = 10;
			}
			else if (dVal < 2000)
			{
				dDelta = 20;
				if (Tick < 0 && dVal == 1000) dDelta = 5;
			}
			else if (dVal < 5000)
			{
				dDelta = 50;
				if (Tick < 0 && dVal == 2000) dDelta = 10;
			}
			else if (dVal < 10000)
			{
				dDelta = 100;
				if (Tick < 0 && dVal == 5000) dDelta = 50;
			}
			else
			{
				dDelta = 200;
			}
		}
		dVal = dVal + dDelta * Tick;
		strResult.Format("%f", dVal);
		break;
	case 3: // KOPEX
		break;
	default: // 주식
		nVal = atoi(Prc);
		if (m_bNewUnit)		// 2023 주식 호가가격단위 개선
		{
			if (nVal < 2000)
			{
				nDelta = 1;
				if (Tick < 0 && nVal <= 1) nDelta = 0;
			}
			if (nVal < 5000)
			{
				nDelta = 5;
				if (Tick < 0 && nVal == 2000) nDelta = 1;
			}
			else if (nVal < 20000)
			{
				nDelta = 10;
				if (Tick < 0 && nVal == 5000) nDelta = 5;
			}
			else if (nVal < 50000)
			{
				nDelta = 50;
				if (Tick < 0 && nVal == 20000) nDelta = 10;
			}
			else if (nVal < 200000)
			{
				nDelta = 100;
				if (Tick < 0 && nVal == 50000) nDelta = 50;
			}
			else if (nVal < 500000)
			{
				nDelta = 500;
				if (Tick < 0 && nVal == 100000) nDelta = 100;
			}
			else
			{
				nDelta = 1000;
				if (Tick < 0 && nVal == 500000) nDelta = 500;
			}
		}
		else	// as-is
		{
			const	bool	bKospi = (m_pParent->SendMessage(WM_USER, MAKEWPARAM(codeTYPE, 0), (LPARAM)m_itemCd.GetString()) == 1) ? true : false;
			if (bKospi)
			{
				if (nVal < 5000)
				{
					nDelta = 5;
					if (Tick < 0 && nVal <= 5) nDelta = 0;
				}
				else if (nVal < 10000)
				{
					nDelta = 10;
					if (Tick < 0 && nVal == 5000) nDelta = 5;
				}
				else if (nVal < 50000)
				{
					nDelta = 50;
					if (Tick < 0 && nVal == 10000) nDelta = 10;
				}
				else if (nVal < 100000)
				{
					nDelta = 100;
					if (Tick < 0 && nVal == 50000) nDelta = 50;
				}
				else if (nVal < 500000)
				{
					nDelta = 500;
					if (Tick < 0 && nVal == 100000) nDelta = 100;
				}
				else
				{
					nDelta = 1000;
					if (Tick < 0 && nVal == 500000) nDelta = 500;
				}
			}
			else
			{
				if (nVal < 5000)
				{
					nDelta = 5;
				}
				else if (nVal < 10000)
				{
					nDelta = 10;
					if (Tick < 0 && nVal == 5000) nDelta = 5;
				}
				else if (nVal < 50000)
				{
					nDelta = 50;
					if (Tick < 0 && nVal == 10000) nDelta = 10;
				}
				else
				{
					nDelta = 100;
					if (Tick < 0 && nVal == 50000) nDelta = 50;
				}
			}
		}
		nVal = nVal + nDelta * Tick;
		strResult.Format("%d", nVal);
		break;
	}

	return strResult.AllocSysString();
}


BOOL CControlWnd::_GetNoFee() 
{	
	if (!m_cfg)
		return FALSE;
	return m_cfg->GetNoFee();
}

void CControlWnd::_SetNoFee(BOOL bNewValue) 
{
}

void CControlWnd::_SetDoQty(BOOL bCheck) 
{
	m_bDoAlert = bCheck ? true : false;
}

BSTR CControlWnd::_GetOrdUnit() 
{
	CString strResult = m_sise[ORDUNIT];
	return strResult.AllocSysString();
}

void CControlWnd::_SetOrdUnit(LPCTSTR lpszNewValue) 
{
	m_sise[ORDUNIT] = lpszNewValue;
}

void CControlWnd::QueryPrcAbl(LPCTSTR sCode, LPCTSTR sAcc, LPCTSTR sPrc, LPCTSTR sMmgb) 
{
	CString scode, sacc, sgubn;

	scode.Format("%s", sCode);	scode.TrimRight();	//종목코드
	sacc.Format("%s", sAcc);	sacc.TrimRight();	//계좌번호
	sgubn.Format("%s", sMmgb);	sgubn.TrimRight();	//매도매수 구분 //1매도, 2매수

	if (scode == "" || sacc == "" ||  sgubn == "")
		return;

	CString	Price, RatVal, Money;
	CString sUser, stmp;
	char	wb[L_userTH + L_ledgerH + L_SONBQ101_mid + 128]{};
	int	idx = 0;

	// key & control name
	wb[idx++] = KEY_SONBQ101;
	CopyMemory(&wb[idx], m_Param.name.GetString(), m_Param.name.GetLength());
	idx += m_Param.name.GetLength();
	wb[idx++] = '\t';

	struct _userTH* udat = (struct _userTH*)&wb[idx];
	struct _ledgerH* ledger = (struct _ledgerH*)&wb[idx + L_userTH];
	SONBQ101_mid* mid_sonbq101 = (SONBQ101_mid*)&wb[idx + L_userTH + L_ledgerH];

	// userTH
	CopyMemory(udat->trc, "piboPBxQ", sizeof(udat->trc));
	udat->stat = US_ENC | US_KEY;
	udat->key = m_Param.key;

	// make data
	stmp = CString(sPrc); stmp.TrimRight();
	if (stmp.IsEmpty())
	{
		Price.Format("+%012.2f", atof("0")); 
		Price.TrimRight();	  //주문가
	}
	else
	{
		if (stmp.Find("-") >= 0)
		{
			Price.Format("%013.2f", atof(sPrc)); 
			Price.TrimRight();
		}
		else
		{
			Price.Format("+%012.2f", atof(sPrc)); 
			Price.TrimRight();	 
		}
	}

	stmp.Format("%s", sMmgb); stmp.TrimRight();
	if (stmp == "1") 
		Money.Format("+%015s", m_cfg->ReadCfg("SellFixPrcQty")); 
	else
		Money.Format("+%015s", m_cfg->ReadCfg("BuyFixPrcQty")); 

	// ledgerH
	FillMemory(ledger, L_ledgerH, ' ');
	m_pParent->SendMessage(WM_USER, ledgerDLL, (LPARAM)ledger);

	sUser = Variant(userCC, "");
	CopyMemory(ledger->svcd, _T("SONBQ101"), sizeof(ledger->svcd));
	CopyMemory(ledger->usid, sUser.GetString(), sUser.GetLength());
	CopyMemory(ledger->rcnt, _T("0000"), sizeof(ledger->rcnt));
	CopyMemory(ledger->fkey, _T("C"), 1);
	CopyMemory(ledger->mkty, _T("2"), 1);
	CopyMemory(ledger->odrf, _T("2"), 1);

	// SONBQ101_mid
	FillMemory(mid_sonbq101, L_SONBQ101_mid, ' ');
	CopyMemory(mid_sonbq101->In, _T("00001"), sizeof(mid_sonbq101->In));
	CopyMemory(mid_sonbq101->zFnoIsuNo, scode.GetString(), scode.GetLength());
	CopyMemory(mid_sonbq101->zQryTp, _T("2"), sizeof(mid_sonbq101->zQryTp));		//조회구분 //1일반 2금액 3비율
	CopyMemory(mid_sonbq101->lOrdAmt, Money.GetString(), sizeof(mid_sonbq101->lOrdAmt));	//주문금액 
	CopyMemory(mid_sonbq101->dOrdPrc, Price.GetString(), sizeof(mid_sonbq101->dOrdPrc));	//주문가	
	
	RatVal.Format("%+019.8f", 0.0);
	CopyMemory(mid_sonbq101->dRatVal, RatVal.GetString(), sizeof(mid_sonbq101->dRatVal));	//비율값 
	
	if (sgubn == "1")									//매매구분 매도 매수
		mid_sonbq101->zBnsTp[0] = '1';
	else
		mid_sonbq101->zBnsTp[0] = '2';
	
	CopyMemory(mid_sonbq101->zFnoOrdprcPtnCode, "00", 2);					//호가유형코드   00 지정가
	
	FillMemory(mid_sonbq101->zAcntNo, sizeof(mid_sonbq101->zAcntNo), ' ');
	CopyMemory(mid_sonbq101->zAcntNo, sacc.GetString(), sacc.GetLength());
	FillMemory(mid_sonbq101->zPwd, sizeof(mid_sonbq101->zPwd), ' ');

	CString spass;
	spass ="";
	CopyMemory(&mid_sonbq101->zPwd, spass.GetString(), spass.GetLength());

	// invoke TR
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, L_SONBQ101_mid + L_ledgerH), (LPARAM)wb);
}
