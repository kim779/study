// .ControlWnd.cpp : implementation file
//

#include "stdafx.h"
#include "CX_GROUPLIST.h"
#include "ControlWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CControlWnd

CControlWnd::CControlWnd(struct _param* pParam, CWnd* pWizard)
{
	EnableAutomation();

	m_pWizard = pWizard;
	m_param.key = pParam->key;
	m_param.name = CString(pParam->name, pParam->name.GetLength());
	m_param.rect = CRect(pParam->rect.left, pParam->rect.top, pParam->rect.right, pParam->rect.bottom);
	m_param.fonts = CString(pParam->fonts, pParam->fonts.GetLength());
	m_param.point = pParam->point;
	m_param.style = pParam->style;
	m_param.tRGB = pParam->tRGB;
	m_param.pRGB = pParam->pRGB;
	m_param.options = CString(pParam->options, pParam->options.GetLength());
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
	ON_WM_SIZE()	
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER,OnUser)
	ON_MESSAGE(WM_ITEM_CLICK,OnItemClick)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CControlWnd, CWnd)
	//{{AFX_DISPATCH_MAP(CControlWnd)
	DISP_FUNCTION(CControlWnd, "SetProperties", SetProperties, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "GetProperties", GetProperties, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "AddRow", AddRow, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "AddAlert", AddAlert, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "MakeList", MakeList, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "ReadyList", ReadyList, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CControlWnd, "Visible", Visible, VT_EMPTY, VTS_BOOL)	
	DISP_FUNCTION(CControlWnd, "RemoveAlert", RemoveAlert, VT_EMPTY, VTS_BSTR)
	DISP_FUNCTION(CControlWnd, "ExpandTree", ExpandTree, VT_EMPTY, VTS_BOOL)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IControlWnd to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {47D4D8D4-8A48-4DCC-88EB-D22EB2B0FFE8}
static const IID IID_IControlWnd =
{ 0x47d4d8d4, 0x8a48, 0x4dcc, { 0x88, 0xeb, 0xd2, 0x2e, 0xb2, 0xb0, 0xff, 0xe8 } };

BEGIN_INTERFACE_MAP(CControlWnd, CWnd)
	INTERFACE_PART(CControlWnd, IID_IControlWnd, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CControlWnd message handlers
LONG CControlWnd::OnUser(WPARAM wParam, LPARAM lParam)
{
	switch (LOBYTE(LOWORD(wParam)))
	{
	case DLL_INB:
	case DLL_OUB:
	default:
		break;
	}

	return TRUE;
}

LONG CControlWnd::OnItemClick(WPARAM wParam, LPARAM lParam)
{
	CString sdata;
	CString strCode = m_list->GetSelectedCode();

	sdata.Format("1301\t%s", strCode.Left(1));
	m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, triggerCC), (LPARAM)sdata.GetString());

	return TRUE;
}

void CControlWnd::SetProperties(LPCTSTR str) 
{
	// TODO: Add your dispatch handler code here
}

BSTR CControlWnd::GetProperties() 
{
	CString strResult;
	// TODO: Add your dispatch handler code here

	return strResult.AllocSysString();
}

int CControlWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)  
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)  
		return -1;
	
	// TODO: Add your specialized creation code here 
	CRect rc;
	GetClientRect(&rc);

	m_list = std::make_unique<CGroupListCtrl>();	
	if (!m_list->Create(WS_CHILD|WS_VISIBLE|LVS_REPORT|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES, rc, this, 1090))
		return -1; 	

	m_list->SetExtendedStyle(m_list->GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);	
	m_list->InsertColumn(0, "계좌번호",   LVCFMT_CENTER, 120);
	m_list->InsertColumn(1, "계좌명",     LVCFMT_CENTER, 200);
	m_list->InsertColumn(2, "종목번호",   LVCFMT_CENTER, 90);
	m_list->InsertColumn(3, "종목명",     LVCFMT_CENTER, 120);
	m_list->InsertColumn(4, "주문유형명", LVCFMT_CENTER, 100);
	m_list->InsertColumn(5, "주문량",     LVCFMT_RIGHT,  100);
	m_list->InsertColumn(6, "체결량",     LVCFMT_RIGHT,  100);
	m_list->InsertColumn(7, "미체결량",   LVCFMT_RIGHT,  100);
	m_list->InsertColumn(8, "체결평균가", LVCFMT_RIGHT,  110);
	m_list->InsertColumn(9, "체결금액",   LVCFMT_RIGHT,  200);
	
	return TRUE; 
} 
 
void CControlWnd::AddRow(LPCTSTR str) 
{ 
	sItem info;

	GetString(str, &info);
	AddList(&info);
}

void CControlWnd::MakeList() 
{	
	for (int ii = 0; ii < m_list->GetItemCount(); ii++)
	{
		sGLI*	pg = (sGLI*)m_list->GetItemData(ii);
		if (pg->type != TYPE_GROUP)
			continue;

		UpdateGroup(pg);
		m_list->SortGroup(pg);
	}
}

void CControlWnd::AddList(sItem* p)
{
	CString gid;
	int	item_id = 0;
	sGLI*	pg = m_list->FindGroup(p->accn);

	if (pg == nullptr) // 그룹이 없을때 그룹 먼저 추가 
	{	
		item_id = m_list->InsertGroup("", p->accn);

		m_list->SetItemText(item_id, 0, MakeAccnString(p->accn));
		m_list->SetItemText(item_id, 1, p->accnName);

		//g_log.write(p->accn);
		//g_log.write("InsertGroup", item_id);
		
		gid = p->accn;
	}
	else
		gid = pg->id;

	COLORREF clr = 0;
	if (p->juName.Find("매도") >= 0)
		clr = RGB(0,0,255);
	else
		clr = RGB(255,0,0);

	// 아이템 추가 
	item_id = m_list->InsertItem("", gid, p->code, clr, p->jucode);

	//g_log.write("InsertItem new item", new_item);

	m_list->SetItemText(item_id, 2, p->code);
	m_list->SetItemText(item_id, 3, p->codeName);
	m_list->SetItemText(item_id, 4, p->juName);
	m_list->SetItemText(item_id, 5, MakeMoneyString(p->juNumber));
	m_list->SetItemText(item_id, 6, MakeMoneyString(p->chNumber));
	m_list->SetItemText(item_id, 7, MakeMoneyString(p->noChNumber));
	m_list->SetItemText(item_id, 8, MakeMoneyString(p->chPrice));
	m_list->SetItemText(item_id, 9, MakeMoneyString(p->chAmount));		
}

void CControlWnd::UpdateGroup(sGLI* pg)
{
	__int64 iJuNumber_a   = 0;		// 주문량		(매수)
	__int64 iChNumber_a   = 0;		// 체결량		(매수)
	__int64 iNoChNumber_a = 0;		// 미체결량		(매수)
	__int64 iChAmount_a   = 0;		// 거래대금		(매수)

	__int64 iJuNumber_b   = 0;		// 주문량		(매도)
	__int64 iChNumber_b   = 0;		// 체결량		(매도)
	__int64 iNoChNumber_b = 0;		// 미체결량		(매도)
	__int64 iChAmount_b   = 0;		// 거래대금		(매도)
 
	CList <sGLI*,sGLI*> list;

	m_list->GetGroupGLI(pg, list);

	CString	str;
	BOOL	bGubn = FALSE;
	const	int	nItem = m_list->GetItemIdx(pg);
	POSITION pos = list.GetHeadPosition();
	while (pos)
	{	
	
		sGLI* p = (sGLI*) list.GetNext(pos);
		/*if (p->type == TYPE_GROUP)
			continue;

		if (p->gid != pg->id)
			continue;*/

		//int idx = m_list->GetItemIdx(p);

		if (p->text.GetAt(4).Find("매수")  > 0)
			bGubn = TRUE;
		else
			bGubn = FALSE;
		
		str = p->text.GetAt(5);
		str.Replace(",","");
		
		if (bGubn)
			iJuNumber_a += _atoi64(str);
		else
			iJuNumber_b += _atoi64(str);


		str = p->text.GetAt(6);
		str.Replace(",","");

		if (bGubn)
			iChNumber_a += _atoi64(str);
		else 
			iChNumber_b += _atoi64(str);

		str = p->text.GetAt(7);
		str.Replace(",","");
		if (bGubn)
			iNoChNumber_a += _atoi64(str);
		else
			iNoChNumber_b += _atoi64(str);

		str = p->text.GetAt(9);
		str.Replace(",","");
		if (bGubn)
			iChAmount_a += _atoi64(str);	
		else
			iChAmount_b += _atoi64(str);	
	}
	
	str.Format("%s(%s)", MakeMoneyString(iJuNumber_b), MakeMoneyString(iJuNumber_a));
	m_list->SetItemText(nItem, 5, str);		//  주문수량 

	str.Format("%s(%s)", MakeMoneyString(iChNumber_b), MakeMoneyString(iChNumber_a));
	m_list->SetItemText(nItem, 6, str);		//  체결수량 

	str.Format("%s(%s)", MakeMoneyString(iNoChNumber_b), MakeMoneyString(iNoChNumber_a));
	m_list->SetItemText(nItem, 7, str);		//  미체결량 

	str.Format("%s(%s)", MakeMoneyString(iChAmount_b), MakeMoneyString(iChAmount_a));
	m_list->SetItemText(nItem, 9, str);		//  거래대금  
}

CString CControlWnd::MakeAccnString(CString str)
{
	CString strRet;

	strRet.Format("%s-%s-%s", str.Mid(0,3), str.Mid(3,2), str.Mid(5,6));
	return strRet;
}

CString CControlWnd::MakeMoneyString(__int64 dw)
{
	CString str;

	str.Format("%I64d",dw);
	return MakeMoneyString(str);
}

CString CControlWnd::MakeMoneyString(CString str)
{
	int	iLen = str.GetLength() / 3;

	if (iLen == 0)
		return str;
 
	if ((str.GetLength()%3) == 0)
		iLen--;

	const	int	iOffset = str.GetLength() - iLen*3; 
	CString strRet = str.Mid(0,iOffset);
	for (int ii = 0; ii < iLen; ii++)	
	{
		strRet += ",";
		strRet += str.Mid(iOffset+3*ii,3);
	}	

	return strRet;
}

void CControlWnd::SetList(sItem* p, int nItem)
{
	/*CString str;

	str = m_list->GetItemText(nItem, 5).Replace(",","");
	__int64 iJuNumber = _atoi64(str);

	str = m_list->GetItemText(nItem, 6).Replace(",","");
	__int64 iChNumber = _atoi64(str);

	str = m_list->GetItemText(nItem, 7).Replace(",","");
	__int64 iNoChNumber = _atoi64(str);

	str = m_list->GetItemText(nItem, 8).Replace(",","");
	__int64 iChPrice = _atoi64(str);

	str = m_list->GetItemText(nItem, 9).Replace(",","");
	__int64 iChAmount = _atoi64(str);

	iJuNumber += _atoi64(p->juNumber);
	iChNumber += _atoi64(p->chNumber);
	//iNoChNumber += atoi(p->noChNumber);
	iChPrice = _atoi64(p->chPrice);
	iChAmount += _atoi64(p->chAmount);

	iNoChNumber = iJuNumber - iChNumber;*/

	m_list->SetItemText(nItem, 2, p->code);
	m_list->SetItemText(nItem, 3, p->codeName);
	m_list->SetItemText(nItem, 4, p->juName);	
	m_list->SetItemText(nItem, 5, MakeMoneyString(p->juNumber));	
	m_list->SetItemText(nItem, 6, MakeMoneyString(p->chNumber));	
	m_list->SetItemText(nItem, 7, MakeMoneyString(p->noChNumber));	
	m_list->SetItemText(nItem, 8, MakeMoneyString(p->chPrice));	
	m_list->SetItemText(nItem, 9, MakeMoneyString(p->chAmount));	

	/*str.Format("%I64d",iJuNumber);
	m_list->SetItemText(nItem, 5, MakeMoneyString(str));

	str.Format("%I64d",iChNumber);
	m_list->SetItemText(nItem, 6, MakeMoneyString(str));

	str.Format("%I64d",iNoChNumber);
	m_list->SetItemText(nItem, 7, MakeMoneyString(str));

	str.Format("%I64d",iChPrice);
	m_list->SetItemText(nItem, 8, MakeMoneyString(str));

	str.Format("%I64d",iChAmount);
	m_list->SetItemText(nItem, 9, MakeMoneyString(str));	*/	
}

void CControlWnd::GetString(CString str, sItem* p)
{	
	int	start = 0;
	int	find = 0;
	CStringArray ar;
	CString data;

	while (TRUE)
	{
		find = str.Find("\t", start);
		if (find < 0)
		{
			data = str.Mid(start, str.GetLength()-start);
			ar.Add(data);
			break;
		}

		data = str.Mid(start, find-start);
		ar.Add(data);
		
		start = find + 1;
	}

	p->brAccn	= ar.GetAt(0);
	p->accn		= ar.GetAt(1);
	p->accnName	= ar.GetAt(2);
	p->code		= ar.GetAt(3);
	p->codeName	= ar.GetAt(4);
	p->gubn		= ar.GetAt(5);
	p->jucode	= ar.GetAt(6);
	p->juName	= ar.GetAt(7);
	p->juNumber	= ar.GetAt(9);
	p->chNumber	= ar.GetAt(11);
	p->chPrice	= ar.GetAt(14);
	p->chAmount	= ar.GetAt(17);
	
	p->brAccn.TrimRight();
	p->accn.TrimRight();
	p->accnName.TrimRight();
	p->code.TrimRight();
	p->codeName.TrimRight();
	p->juName.TrimRight();

	p->juNumber.TrimLeft();
	p->chNumber.TrimLeft();
	p->chPrice.TrimLeft();
	p->chAmount.TrimLeft();

	const	__int64 iJuNumber = _atoi64(p->juNumber);
	const	__int64 iChNumber = _atoi64(p->chNumber);
	const	__int64 iNoChNumber = iJuNumber - iChNumber;
	p->noChNumber.Format("%I64d", iNoChNumber);	
}

void CControlWnd::OnDestroy() 
{
	CWnd::OnDestroy();
}

void CControlWnd::AddAlert(LPCTSTR str) 
{
	BOOL	bSort = FALSE;
	sItem	info;

	GetString(str, &info);	
	sGLI*	p = m_list->FindItem(info.accn, info.code, info.jucode);

	if (p)
	{
		const	int	nItem = m_list->GetItemIdx(p);
		SetList(&info, nItem);		
	}
	else
	{		
		AddList(&info);
		bSort = TRUE;
	}
 
	sGLI* pg = m_list->FindGroup(info.accn);
	if (pg)
	{
		UpdateGroup(pg);
		if (bSort)
			m_list->SortGroup(pg);
	} 
}


void CControlWnd::ReadyList() 
{
	m_list->DeleteAllItems();
}

void CControlWnd::Visible(BOOL bShow) 
{
	ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

void CControlWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	if (m_list->GetSafeHwnd())
	{	
		m_list->SetWindowPos(NULL, 0, 0, cx, cy, SWP_SHOWWINDOW|SWP_NOZORDER);
	}
}


void CControlWnd::RemoveAlert(LPCTSTR str) 
{
	sItem	info;

	GetString(str, &info);	
	sGLI* p = m_list->FindItem(info.accn, info.code, info.jucode);

	m_list->DeleteItem(p);
	if (m_list->GetGroupItemCount(info.accn) == 0)
	{ 
		sGLI* pg = m_list->GetGroupGLI(info.accn);
		m_list->DeleteItem(pg);
	}
}

void CControlWnd::ExpandTree(BOOL bExpand) 
{
	if (m_list->GetSafeHwnd())
	{
		m_list->ExpandAllItems(bExpand);
	}
}
