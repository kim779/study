/******************************************************************************
 * NAME	: AxMapForm.cpp							 
 * DESC	: FormView를 상속받은 map dll화면을 위한 base 클래스
 * REV. : VER	DATE	DESCRIPTION
 * 	  ----	------	-------------------------------------------------------
 *	  1.00	2004.08.10	Initial version.
 *        1.01  2004.09.17      Keep Child Focus logic
 *****************************************************************************/
// MapForm.cpp : implementation file
//

#include "stdafx.h"
#include "axMapForm.h"
#include "../../h/axisfire.h"

//#include "libDefine.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static		int g_nHookCnt = 0;
HHOOK		g_hKeyboardHook;
HINSTANCE	g_hInstance;

extern "C" __declspec(dllexport)
LRESULT CALLBACK KeyHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode < 0 || nCode != HC_ACTION)
		return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);

	if ((!(lParam & (1<<31)) && !(lParam & (1<<30))) && 
		!(GetKeyState(VK_CONTROL) & 0x8000) && GetFocus())
	{
		HWND hWnd = GetParent((HWND)GetFocus());

		if (hWnd)
		{
			char nameBuf[21];
			GetWindowText(hWnd, nameBuf, 20);
			nameBuf[20] = '\0';
			
			if (!strcmp(nameBuf, "FormView"))
			{
				if (hWnd)
					PostMessage(hWnd, WM_KEYMSG, wParam, lParam);
			}
			else
			{
				hWnd = GetParent(hWnd);
				if (hWnd)
				{
					GetWindowText(hWnd, nameBuf, 20);
					nameBuf[20] = '\0';
					if (!strcmp(nameBuf, "FormView"))
					{
						if (hWnd)
							PostMessage(hWnd, WM_KEYMSG, wParam, lParam);
					}
				}
			}
		}
	}

	return CallNextHookEx(g_hKeyboardHook, nCode, wParam, lParam);
}


IMPLEMENT_DYNCREATE(CAxMapForm, CFormView)

CAxMapForm::CAxMapForm(UINT nIDTemplate)
	: CFormView(nIDTemplate)
{
	m_bEnter       = false;
	m_pFocusWnd    = NULL;
}

CAxMapForm::~CAxMapForm()
{
	m_axOrderMap.RemoveAll();
}

void CAxMapForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAxMapForm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAxMapForm, CFormView)
	//{{AFX_MSG_MAP(CAxMapForm)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_KEYMSG, OnTabNext)
	ON_MESSAGE(WM_AMTPOPUPMSG, OnAmountPopup)
END_MESSAGE_MAP()


#ifdef _DEBUG
void CAxMapForm::AssertValid() const
{
	CFormView::AssertValid();
}

void CAxMapForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif 

void CAxMapForm::CreateMap(CWnd *pParent, CSize szForm)
{
	g_nHookCnt++;
	if (g_nHookCnt == 1)
	{
		g_hInstance = AfxGetInstanceHandle();
		TRACE0("MapForm Keyboard Hook Start!!\n");
		g_hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD, KeyHookProc, g_hInstance, GetCurrentThreadId());
	}
	CWnd wnd;
	wnd.Attach(pParent->m_hWnd);
	Create(NULL, NULL, WS_CHILD | WS_VISIBLE/* | WS_CLIPCHILDREN*/, CRect(0, 0, szForm.cx, szForm.cy), &wnd, 100, NULL);
	wnd.Detach();
	SetScrollSizes(MM_TEXT, CSize(1, 1));
	UpdateData(FALSE);
	OnInitialUpdate();
	SetFocus();
	SetWindowText("FormView");
	UpdateWindow();
}

void CAxMapForm::PostNcDestroy()
{

}

void CAxMapForm::OnDestroy() 
{
	g_nHookCnt--;
	if (!g_nHookCnt)
	{
		TRACE0("MapForm Keyboard Hook Terminate!!\n");
		UnhookWindowsHookEx(g_hKeyboardHook);
	}
	CFormView::OnDestroy();
}

void CAxMapForm::OnSetFocus(CWnd* pOldWnd) 
{
	// I don't know why releasing keyboard hook as reactive.
	CFormView::OnSetFocus(pOldWnd);
	if (m_pFocusWnd && ::IsWindow(m_pFocusWnd->m_hWnd))
		m_pFocusWnd->SetFocus();
}


LRESULT CAxMapForm::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_COMMAND && GetFocus() && GetFocus()->GetParent() &&
		GetFocus()->GetParent()->m_hWnd == GetSafeHwnd())	// Set Focus window whenever message WM_COMMAND
		m_pFocusWnd = GetFocus();
//	if (message == WM_COMMAND)
//		OnCmd(message, wParam, lParam);
	return CFormView::WindowProc(message, wParam, lParam);
}

LRESULT CAxMapForm::OnTabNext(WPARAM wParam, LPARAM lParam)
{
	if (wParam == VK_TAB || (m_bEnter && wParam == VK_RETURN))
	{
		CWnd *pWnd = this;
		if (pWnd && ::IsWindow(m_hWnd) && ::IsWindow(::GetFocus()))
		{
			BOOL bShift = (::GetKeyState(VK_SHIFT) & 0x8000);
			HWND hWnd = NULL;
			
			if (m_tabOrder.GetSize())
			{
				int ii = 0;
				for (ii = 0; ii < m_tabOrder.GetSize() && 
					::GetFocus() != m_tabOrder.GetAt(ii); ii++);
				if (ii < m_tabOrder.GetSize())
				{
					ii++;
					if (bShift)
						ii -= 2;
					if (ii < 0)
						ii = m_tabOrder.GetSize() - 1;
					else if (ii >= m_tabOrder.GetSize())
						ii = 0;
					hWnd = m_tabOrder.GetAt(ii);
				}
			}
			else
				hWnd = GetNextDlgTabItem(GetFocus(), bShift)->m_hWnd;

			if (hWnd && ::IsWindow(hWnd))
			{
				::SetFocus(hWnd);
				CWnd wnd;
				if (wnd.FromHandle(hWnd)->IsKindOf(RUNTIME_CLASS(CEdit)))
					((CEdit*)wnd.FromHandle(hWnd))->SetSel(0, -1);
				return TRUE;
			}
		}
	}
	return FALSE;
}

void CAxMapForm::SetView(int type, CString data)
{
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(viewDLL, type), (LPARAM)(LPCTSTR)data);
}

CFont* CAxMapForm::getAxFont(CString fName, int point, bool bItalic, int nBold)
{
	struct _fontR fontR;
	fontR.name   = (LPSTR)fName.operator LPCTSTR();
	fontR.point  = point;
	fontR.italic = bItalic;
	fontR.bold   = nBold;
	return (CFont*)m_pParent->SendMessage(WM_USER, getFONT, (long)&fontR);
}

CBitmap* CAxMapForm::getAxBitmap(CString path)
{
	return (CBitmap*)m_pParent->SendMessage(WM_USER, getBITMAP, (long)path.operator LPCTSTR());
}

CPen* CAxMapForm::getAxPen(COLORREF clr, int width, int style)
{
	struct	_penR penR;
	penR.clr   = clr;
	penR.width = width;
	penR.style = style;
	return (CPen*)m_pParent->SendMessage(WM_USER, getPEN, (long)&penR);
}

CBrush* CAxMapForm::getAxBrush(COLORREF clr)
{
	return (CBrush*)m_pParent->SendMessage(WM_USER, getBRUSH, (long)clr);
}

COLORREF CAxMapForm::GetIndexColor(int index)
{
	return m_pParent->SendMessage(WM_USER, MAKEWORD(getPALETTE, 0), (LPARAM)index);
}

CString CAxMapForm::Variant(int cmd, CString data)
{
	CString retvalue;
	char* dta = (char*)m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)(LPCTSTR)data);

	if ((long)dta > 1)
		retvalue = dta;
	else
		return "";

	return retvalue;
}

int CAxMapForm::Variant(int cmd, int data)
{
	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(variantDLL, cmd), (LPARAM)data);
}

CWnd* CAxMapForm::CreateMapObject(CString sMapName, int &nObjKey, CPoint point)
{
	CWnd *pObjWnd = NULL;
	if (!sMapName.IsEmpty())
	{
		CRect   rc;
		CString sStr;
		long    nSzMap = 0, nType;

		GetClientRect(&rc);

		sStr = CString(sMapName, 8);
		nSzMap = m_pParent->SendMessage(WM_USER, sizeMAP, (long)sStr.operator LPCTSTR());

		nType = m_pParent->SendMessage(WM_USER, typeDLL, (long)sStr.operator LPCTSTR());

		if (nType != vtypeERR)
		{
			pObjWnd = new CWnd();
			pObjWnd->Create(NULL, NULL, WS_CHILD, CRect(0, 0, LOWORD(nSzMap), HIWORD(nSzMap)), this, 0);
			nObjKey = m_pParent->SendMessage(WM_USER, MAKEWPARAM(attachMAP, nType), (long)pObjWnd);
			sStr = sMapName;
			m_pParent->SendMessage(WM_USER, MAKEWPARAM(loadMAP, nObjKey), (long)sStr.operator LPCTSTR());
			pObjWnd->SetWindowPos(NULL, point.x, point.y, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
		}
	}
	return pObjWnd;
}

void CAxMapForm::DeleteMapObject(CWnd *pObjWnd, int nObjKey)
{
	if (!pObjWnd || !::IsWindow(pObjWnd->m_hWnd))
		return;
	m_pParent->SendMessage(WM_USER, MAKEWPARAM(detachMAP, nObjKey));
	pObjWnd->DestroyWindow();
	delete pObjWnd;
	pObjWnd = NULL;
}

BOOL CAxMapForm::SendTR(CString name, BYTE type, CString data, int key)
{
	CString trData = "";
	
	struct _userTH udat;
	
	strcpy(udat.trc, name);
	udat.key = key;
	udat.stat = type;

	trData += CString((char *)&udat, L_userTH);
	trData += data;

	return m_pParent->SendMessage(WM_USER, MAKEWPARAM(invokeTRx, data.GetLength()), (LPARAM)trData.operator LPCTSTR());
}


//
// account
//

int CAxMapForm::SetAccountCBO(CComboBox *pCombo, CString sAccKey)
{
	CString tmp = _T(""), sSel;
	tmp = Variant(accountCCx, sAccKey);
	sSel = Parser(tmp, "\t");

	while(!tmp.IsEmpty())
	{
		CString sAcc = Parser(tmp, "\t"), sDisp = Parser(tmp, "\n");
		pCombo->SetItemData(pCombo->AddString(sDisp), m_accountArr.Add(sAcc));
		if (!sSel.IsEmpty() && !sAcc.CompareNoCase(sSel))
			pCombo->SelectString(0, sDisp);
	}
	if (sSel.IsEmpty())
		pCombo->SetCurSel(0);
	return pCombo->GetCurSel();
}

CString CAxMapForm::GetAccount(int nIdx)
{
	if (nIdx >= m_accountArr.GetSize())
		return "";
	return  m_accountArr.GetAt(nIdx);
}

//
// utilities
//

CString CAxMapForm::Parser(CString &srcstr, CString substr)
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

CRect CAxMapForm::adjustRect(CRect screenRect)
{
	int nDx = 0, nDy = 0;
	if (screenRect.right > GetSystemMetrics(SM_CXFULLSCREEN))
		nDx = GetSystemMetrics(SM_CXFULLSCREEN) - screenRect.right;
	if (screenRect.bottom > GetSystemMetrics(SM_CYFULLSCREEN))
		nDy = GetSystemMetrics(SM_CYFULLSCREEN) - screenRect.bottom;
	screenRect.OffsetRect(nDx, nDy);
	return screenRect;
}

// suppress 1: + and -,   2: + only
CString CAxMapForm::getFormatData(CString src, bool bComma, int nSuppress)
{
	CString sRtn = src;
	if (bComma)
		sRtn = SetComma(sRtn);
	if ((nSuppress & 1) && !sRtn.IsEmpty() && sRtn.FindOneOf("+-") == 0)
		sRtn = sRtn.Mid(1);
	if ((nSuppress & 2) && !sRtn.IsEmpty() && sRtn.GetAt(0) == '+')
		sRtn = sRtn.Mid(1);
	if (nSuppress & 4)
	{
		sRtn.TrimLeft();
		sRtn.TrimLeft('0');
	}
	sRtn.TrimLeft();
	return sRtn;
}

CString CAxMapForm::SetComma(CString src)
{
	CString temp, floatStr;
	int	ii, jj = -1;

	src.TrimLeft();
	if (src.Find('.') >= 0)
	{
		floatStr = src.Mid(src.Find('.'));
		src = src.Left(src.Find('.'));
	}
	else
		floatStr.Empty();
	
	temp = src;
	src.Empty();
	
	for (ii = temp.GetLength() - 1; ii >= 0; ii--)
	{
		jj++;
		if (jj == 3)
		{
			if (temp.GetAt(0) != '-' && temp.GetAt(0) != '+')
				src = ',' + src;
			else if (ii != 0)
				src = ',' + src;
			
			jj = 0;
		}
		src = temp.GetAt(ii) + src;
	}
	src += floatStr;
	
	return src;
}

//
//	CGrpWnd관련
//


//
//	CDC 관련 함수
//

void CAxMapForm::drawContrast(CDC *pDC, CRect cellRc, char cStyle, COLORREF clr)
{
	CRect	pRc = cellRc;
	float	hh = (float)cellRc.Height();
	CSize	size;
	CPoint	pts[7];
	CPen	*pOldPen;
	CBrush	*pOldBrush;
	
	pOldPen = (CPen *) pDC->SelectObject(getAxPen(clr, 1, PS_SOLID));

	pOldBrush = (CBrush *) pDC->SelectObject(getAxBrush(clr));

	pRc.top = (cellRc.top + cellRc.Height() / 2) - (int)(hh / 2);
	pRc.bottom = (int)(pRc.top + hh);
	
	size = pDC->GetOutputTextExtent("A");
	if (size.cx > 10)
		pRc.left = (int)(pRc.right - (size.cx * 0.8));
	else
		pRc.left = pRc.right - size.cx;
	
	hh = (float)(int)(pRc.Height()+1)/2;
	hh = (float)(int)((hh+1)/2) * 2 - 1;
	int	ww = pRc.Width();
	if (ww >= 4)
		ww = (ww / 2 * 2);
	pRc.right = pRc.left + ww;
	int	rr = ww % 3;
	ww /= 3;
	if (rr >= 2 && pRc.Width() > 3)
		ww++;
	
	if (pRc.left >= cellRc.left && pRc.right <= cellRc.right)
	{
		pDC->SetPolyFillMode(ALTERNATE);
		switch(cStyle)
		{
		case '1':	// arrow up
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.top;
			pts[3].y = pRc.bottom;
			pts[4].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left + 1;
			pts[6].x = pRc.right - 1;
			pts[2].x = pRc.left + ww;
			pts[3].x = pRc.left + ww;
			pts[4].x = pRc.right - ww;
			pts[5].x = pRc.right - ww;
			pDC->Polygon(pts, 7);
			break;
		case '4':	// arrow down
			pts[1].y = pRc.top + (int)hh;
			pts[2].y = pRc.top + (int)hh;
			pts[5].y = pRc.top + (int)hh;
			pts[6].y = pRc.top + (int)hh;
			pts[0].y = pRc.bottom;
			pts[3].y = pRc.top;
			pts[4].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right - 1;
			pts[6].x = pRc.left + 1;
			pts[2].x = pRc.right - ww;
			pts[3].x = pRc.right - ww;
			pts[4].x = pRc.left + ww;
			pts[5].x = pRc.left + ww;
			pDC->Polygon(pts, 7);
			break;
		case '2':	// triangle up
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.top;
			pts[1].y = pRc.bottom;
			pts[2].y = pRc.bottom;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.left;
			pts[2].x = pRc.right;
			pDC->Polygon(pts, 3);
			break;
		case '3':	// triangle down
			pRc.top += 2;
			pRc.bottom -= 2;
			if (!(pRc.Height() % 2))
				pRc.top++;
			pts[0].y = pRc.bottom;
			pts[1].y = pRc.top;
			pts[2].y = pRc.top;
			
			pts[0].x = pRc.left + (pRc.Width()/2);
			pts[1].x = pRc.right;
			pts[2].x = pRc.left;
			pDC->Polygon(pts, 3);
			break;
		}
	}
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldBrush);
}

LRESULT CAxMapForm::OnAmountPopup(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

//
// 종목관련 함수
//
CString CAxMapForm::GetJName(CString sJCode)
{
	if (sJCode.IsEmpty())
		return "";

	CString sRtn = CString(sJCode, sJCode.GetLength());
	sRtn = (char*)m_pParent->SendMessage(WM_USER, nameDLL, (LPARAM)sRtn.operator LPCTSTR());
	sRtn.Replace("선물", "F");
	sRtn.Replace("스프레드 ", "");
	sRtn.Replace("콜옵션", "C");
	sRtn.Replace("풋옵션", "P");
	return sRtn;
}


int CAxMapForm::GetJKind(CString sCode)
{
	if (sCode.GetLength() != 8)
		return -1;
	switch (sCode.GetAt(0))
	{
	case '1':	// future
		return JK_FUTURE;
	case '4':	// future spread
		return JK_SPREAD;
	case '2':	// call option
		if (sCode.GetAt(1) == '0')
			return JK_CALLOPT;
		else
			return JK_JCALLOPT;
		break;
	case '3':	// put option
		if (sCode.GetAt(1) == '0')
			return JK_PUTOPT;
		else
			return JK_JPUTOPT;
		break;
	}
	return -1;
}

CString CAxMapForm::GetHsga(CString sCode)
{
	if (sCode.GetLength() != 8)
		return "";
	
	CString sRtn;
	sRtn.Format("%4.1f", atof(sCode.Right(3)) + ((sCode.GetAt(7) == '0' || sCode.GetAt(7) == '5')?0:0.5));
	return sRtn;
}

double CAxMapForm::GetPriceUnit(double price, CString sJCode, bool bPlus)
{
	double unit = 0;
	if (GetJKind(sJCode) == JK_FUTURE)
		return 0.05;
	else
	{
		CString sPrice;
		sPrice.Format("%6.2f", price);
		price = atof(sPrice);

		if (price < 3 || (!bPlus && price == 3))
			return 0.01;
		else
			return 0.05;
	}
}

void CAxMapForm::ParsingNotice(CString str)
{
	CString sKeys = Parser(str, "\n"), sAcc = "", sAdmin = "", sOrder = "";
	while(!sKeys.IsEmpty())
	{
		CString sKey = Parser(sKeys, "\t");
		switch(atoi(sKey))
		{
		case 901:	//	계좌번호
			sAcc = Parser(sKeys, "\r");
			if (sAcc.GetLength() > 10)
			{
				sAdmin = sAcc;
				sAcc = Parser(sAdmin, "\t");
				Parser(sAdmin, "\t");	// 902를 제거
			}
			break;
		case 904:	//	주문번호
			sOrder = Parser(sKeys, "\n");
			break;
		}
	}
	if (sAcc.GetLength() >= 10)
		ContractNotify(sAcc.Left(8), sAcc.Mid(8), sAdmin, sOrder, str);
}

void CAxMapForm::ContractNotify(CString sAcc, CString sAccGubn, CString sAdmin, CString sOrder, CString str)
{

}

CString CAxMapForm::GetOrderMapKey()
{
	static int m_nOrderMapKey = 0;
	CString sRtn;
	if (m_nOrderMapKey > 99999999)
		m_nOrderMapKey = 0;
	sRtn.Format("%08d", m_nOrderMapKey);
	m_nOrderMapKey++;
	return sRtn;
}

void CAxMapForm::SetOrderMap(CString sKey, CString sOrderInfo)
{
	m_axOrderMap.SetAt(sKey, sOrderInfo);
}

CString CAxMapForm::GetOrderMap(CString sKey)
{
	CString sOrderInfo = "";
	m_axOrderMap.Lookup(sKey, sOrderInfo);
	return sOrderInfo;
}

void CAxMapForm::RemoveOrderMap(CString sKey)
{
	m_axOrderMap.RemoveKey(sKey);
}

void CAxMapForm::SetAccountInfo(CString str)
{
	CString sAccs, sAccount, sTmp, sTmp2;
	while (!str.IsEmpty())
	{
		CString sAccInfo = Parser(str, "\n");
		sAccs = "\n" + Variant(accountCC, "");
		sAccount = Parser(sAccInfo, "\t");
		sTmp = Parser(sAccs, "\n" + sAccount + "\t");
		Parser(sAccs, "\n");
		if (!sAccs.IsEmpty())
			sTmp += "\n" + sAccs;
		if (!sTmp.IsEmpty())
			sTmp = sAccount + "\t" + sAccInfo + sTmp;
		else
			sTmp = sAccount + "\t" + sAccInfo;

		int nCnt = sTmp.Replace("\n", "\n") + 1, nLen;
		if (sTmp.GetAt(sTmp.GetLength() - 1) == '\n')
			nCnt--;
		sTmp2.Format("%c%c", 0x02, nCnt);
		nLen = sTmp.GetLength() + 2;
		sTmp = sTmp2 + sTmp;
		sTmp.Replace('\n', '\0');
		sTmp.SetAt(1, nCnt);

		m_pParent->SendMessage(WM_USER, MAKEWPARAM(accountDLL, nLen), (LPARAM)(LPSTR)sTmp.operator LPCTSTR());	
	}
}

void CAxMapForm::SetAccountInfo(CString sAccount, CString sPass, CString sAccName)
{
	if (sPass.IsEmpty())
		sPass = Variant(passCC, sAccount);
	if (sAccName.IsEmpty())
		sAccName = Variant(acnameCC, sAccount);
	SetAccountInfo(sAccount + "\t" + sPass + "\t" + sAccName);
}

void CAxMapForm::SetAccountINIT(CString str)
{
	CString sPacket, sName, sData, sTmp;
	int nLen;
	
	if (!str.IsEmpty())
	{
		sPacket = Parser(str, "\n");
		Parser(sPacket, "\t");				// 계좌갯수
		sData  = Parser(sPacket, "\t") + "\t";		// 계좌번호
		sName  = Parser(sPacket, "\t");			// 계좌명
		sData += Parser(sPacket, "\t") + "\t";		// 비밀번호
		sData += sName + "\n";
		
		sTmp.Format("%c%c", 0x02, 1);
		nLen = sData.GetLength() + 2;
		
		sData = sTmp + sData;
		sData.Replace('\n', '\0');
		sData.SetAt(1, 1);
		
		m_pParent->SendMessage(WM_USER, MAKEWPARAM(accountDLL, nLen), (LPARAM)(LPSTR)sData.operator LPCTSTR());	
	}

	while (!str.IsEmpty())
	{
		sPacket = Parser(str, "\n");
		Parser(sPacket, "\t");				// 계좌갯수
		sData  = Parser(sPacket, "\t");			// 계좌번호
		sName  = Parser(sPacket, "\t");			// 계좌명
		sTmp   = Parser(sPacket, "\t");			// 비밀번호
		
		SetAccountInfo(sData, sTmp, sName);
	}
}

void CAxMapForm::OnClose() 
{
	DestroyWindow();	
	CFormView::OnClose();
}


BOOL CAxMapForm::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;	
	return CFormView::OnEraseBkgnd(pDC);
}

void CAxMapForm::ClipChildren(CDC *pDC)
{
	CWnd *pWnd = GetWindow(GW_CHILD);
	pDC->SelectClipRgn(NULL);

	while (pWnd && IsWindow(pWnd->GetSafeHwnd()))
	{
		if (pWnd->IsWindowVisible())
		{
			CRgn clientRgn;
			CRect clientRC;
			pWnd->GetWindowRect(&clientRC);
			ScreenToClient(&clientRC);
			pDC->ExcludeClipRect(&clientRC);
		}
		pWnd = pWnd->GetNextWindow();
	}
}
/*
void CAxMapForm::OnCmd(UINT message, WPARAM wParam, LPARAM lParam)
{
}
*/

CString CAxMapForm::GetString(char *pChar, int nMinLen)
{
	CString sRtn = CString(pChar, min(nMinLen, (int)strlen(pChar)));
	sRtn.TrimRight();
	return sRtn;
}

const int foAcct = 20;
const int opAcct = 21;

BOOL CAxMapForm::IsStockAcc(CString sAcc)
{
	CString acct(sAcc);
	acct.Replace("-", "");
	if (acct.GetLength() < LEN_ACCT)
		return FALSE;

	acct = acct.Mid(acctLeft, acctMid);
	return (atoi(acct) != foAcct && atoi(acct) != opAcct);
}

BOOL CAxMapForm::IsFOptionAcc(CString sAcc)
{
	CString acct(sAcc);
	acct.Replace("-", "");
	if (acct.GetLength() < LEN_ACCT)
		return FALSE;

	acct = acct.Mid(acctLeft, acctMid);
	return (atoi(acct) == foAcct || atoi(acct) == opAcct);
}

void CAxMapForm::SetString(char *pChar, int nMinLen, CString sStr)
{
	CopyMemory(pChar, sStr.operator LPCTSTR(), min(nMinLen, sStr.GetLength() + 1));
}

void CAxMapForm::EnDecrypt(char *pass, int nCnt)
{
	for (int ii = 0; ii < nCnt; ii++)
	{
		switch (ii % 5)
		{
		case 0:		pass[ii] ^= 0x57;	break;
		case 1:		pass[ii] ^= 0x49;	break;
		case 2:		pass[ii] ^= 0x4E;	break;
		case 3:		pass[ii] ^= 0x49;	break;
		case 4:		pass[ii] ^= 0x58;	break;
		}
	}
}
