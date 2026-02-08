// CodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "axisgcom.h"
#include "CodeCtrl.h"

#include "../../h/jmcode.h"
#include <imm.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListWnd

CSubList::CSubList(class CCodeCtrl *pCodeCtrl) 
{ 
	m_pCodeCtrl = pCodeCtrl; 
}

CSubList::~CSubList() 
{ 
}

LRESULT CSubList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONUP)
	{
		m_pCodeCtrl->LButtonUp();
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo

CCodeCombo::CCodeCombo(class CCodeCtrl *pParent, int iTextHeight, int TextGap)
{
	m_pParent = pParent;
	m_pApp = (CAxisGComApp *)AfxGetApp();
	m_DataMode = DM_NONE;

	m_hList = NULL;
	m_TextHeight = iTextHeight;
	m_TextGap = TextGap;
}

CCodeCombo::~CCodeCombo()
{
}


BEGIN_MESSAGE_MAP(CCodeCombo, CComboBox)
	//{{AFX_MSG_MAP(CCodeCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeCombo message handlers

void CCodeCombo::OnSelchange() 
{
	int	index = GetCurSel();
	_JCode	jCode;
	if (index >= 0)
	{
		if (IsCodeListMode())
		{
			jCode = m_pJCode.GetAt(index);
			//**m_pParent->m_pEdit->ListCode(jCode.Code.Mid(1));
			m_pParent->m_pEdit->ListCode(jCode.Code.GetAt(0) == 'A' ? jCode.Code.Mid(1) : jCode.Code); // 2006.11.16 lsh (윗줄수정)	
			m_pParent->m_pEdit->SetFocus();
		}
		else if (IsHistoryMode())
		{
			jCode = m_pHCode.GetAt(index);
			m_pParent->m_pEdit->ListCode(jCode.Code);
			m_pParent->m_pEdit->SetFocus();
		}
	}
}

int CCodeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetHorizontalExtent(200);
	SetDroppedWidth(200);

	m_pSubList = new CSubList(m_pParent);

	return 0;
}

void CCodeCombo::OnDestroy() 
{
	if (m_pSubList->m_hWnd)
		m_pSubList->UnsubclassWindow();
	delete m_pSubList;

	struct upcode	*pObject = NULL;
	int	nSize = m_pUCode.GetSize();
	for (int ii = 0 ; ii < nSize ; ii++ )
	{
		pObject = m_pUCode.GetAt(ii);
		delete pObject;
		pObject = NULL;
	}
	m_pUCode.RemoveAll();

	CComboBox::OnDestroy();
}

HBRUSH CCodeCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		{
			if (m_hList == NULL)
			{
				m_hList = pWnd->m_hWnd;
				m_pSubList->SubclassWindow(m_hList);
			}
		}
		break;
	}

	return hbr;
}

void CCodeCombo::SearchCode(CString code)
{
	if (code.IsEmpty())
	{
		SetTopIndex(0);
		SetCurSel(0);
		return;
	}

	code.MakeUpper();

	_JCode	jCode;
	int	len = code.GetLength();

	if (IsCodeListMode())
	{
		int	size = m_pJCode.GetSize();
		for (int ii = 0; ii < size; ii++)
		{
			jCode = m_pJCode.GetAt(ii);
			if (jCode.Name.GetLength() < len)
				continue;
			
			if (jCode.Name.Left(len) == code)
			{
				SetTopIndex(ii);
				SetCurSel(ii);
				return;
			}
		}
	}
	else if (IsHistoryMode())
	{
		int	size = m_pHCode.GetSize();
		for (int ii = 0; ii < size; ii++)
		{
			jCode = m_pHCode.GetAt(ii);
			if (jCode.Code.GetLength() < len)
				continue;
			
			if (jCode.Code.Left(len) == code)
			{
				SetTopIndex(ii);
				SetCurSel(ii);
				return;
			}
		}
	}

	if (GetCurSel() < 0)
	{
		SetTopIndex(0);
		SetCurSel(0);
	}
}

CString CCodeCombo::GetSelectCode(CString sCode)
{
	int	index = GetCurSel();
	int	dCount = GetCount();
	if (dCount <= 0 || index < 0)
		return sCode;

	if (IsCodeListMode())
	{
		_JCode	jCode = m_pJCode.GetAt(index);
		//**m_sCode = jCode.Code.Mid(1);
		m_sCode = jCode.Code.GetAt(0) == 'A' ? jCode.Code.Mid(1) : jCode.Code; // 2006.11.16 lsh (윗줄수정)
	}
	else if (IsHistoryMode())
	{
		_JCode	jCode = m_pHCode.GetAt(index);
		//**m_sCode = jCode.Code.Mid(1);
		m_sCode = jCode.Code.GetAt(0) == 'A' ? jCode.Code.Mid(1) : jCode.Code; // 2006.11.16 lsh (윗줄수정)
	}

	return m_sCode;
}

void CCodeCombo::Key(int key)
{
	int	index = GetCurSel();

	if (index < 0)
		return;

	switch (key)
	{
	case VK_UP:
		index = max(0, index-1);
		break;
	case VK_DOWN:
		index = min(GetCount()-1, index+1);
		break;
	case VK_PRIOR:
		index = max(0, index-12);
		break;
	case VK_NEXT:
		index = min(GetCount()-1, index+12);
		break;
	case VK_HOME:
		index = 0;
		break;
	case VK_END:
		index = GetCount()-1;
		break;
	default:
		return;
	}

	if (index >= 0 && index < GetCount())
	{
		SetCurSel(index);
	}
}

void CCodeCombo::InitCodeList()
{
	ResetContent();

	_JCode	jCode;
	CString	string;
	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);
		//**Str.Format("%s  %s", jCode.Code.Mid(1), jCode.Name);
		string.Format(_T("%s  %s"), jCode.Code.GetAt(0) == 'A' ? LPCTSTR(jCode.Code.Mid(1)) : LPCTSTR(jCode.Code),
					LPCTSTR(jCode.Name)); // 2006.11.16 lsh (윗줄수정)
		AddString(string);
	}
	m_DataMode = DM_CODELIST;
}

void CCodeCombo::InitHistory(CString history, bool bForeign)
{
	ResetContent();

	m_pHCode.RemoveAll();
	_JCode	jCode;
	CString	Str;
	int	index;

	if (bForeign)
	{
		for (;;)
		{
			index = history.Find("\t");
			if (index <= 0)
				break;

			Str = history.Left(index);
			history = history.Mid(index + 1);

			index = Str.Find(" ");
			if (index > 0)
			{
				jCode.Code = Str.Left(index);
				jCode.Code = jCode.Code.Mid(1);
				jCode.Name = Str.Mid(index + 1);
				m_pHCode.Add(jCode);
			}
		}
	}
	else
	{
		for (;;)
		{
			index = history.Find("\t");
			if (index <= 0)
				break;

			Str = history.Left(index);
			history = history.Mid(index + 1);

			index = Str.Find(" ");
			if (index > 0)
			{
				jCode.Code = Str.Left(index);
				jCode.Name = Str.Mid(index + 1);
				m_pHCode.Add(jCode);
			}
		}
	}

	for (int ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		Str.Format(_T("%s  %s"), LPCTSTR(jCode.Code), LPCTSTR(jCode.Name));
		AddString(Str);
	}

	m_DataMode = DM_HISTORY;
}

void CCodeCombo::InitIndexList(int kkind)
{
	ResetContent();

	m_pHCode.RemoveAll();
	_JCode	jCode;
	CString	Str;

	struct	upcode*	pObject = NULL;
	int	ii = 0, size = m_pUCode.GetSize();
	switch (kkind)
	{
	case upKospi:
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upKOSPI)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		break;
	case upKos200:
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upKOSPI200)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		break;
	case upKos100:
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if ((pObject->jgub == upKOSPIx) && 
				(pObject->ucod == 1 || pObject->ucod == 2))
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		break;
	case upKosdaq:
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upKOSDAQ)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		break;
	case upKq50:
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upKOSDAQ50 && pObject->ucod == 1)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
				break;
			}
		}
		break;
	case upKoStar:		// Added by schbang 2006.02.15
		/**
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upKOSTAR)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		**/
		break;
/*	case upSector:		// Added by schbang 2006.02.15
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upSECTOR)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		break;
*/
	case upKrx100:		// Added by schbang 2006.02.20
		/**
		for (; ii < size; ii++)
		{
			pObject = m_pUCode.GetAt(ii);
			if (pObject->jgub == upKRX100)
			{
				jCode.Code.Format("%02d", pObject->ucod);
				jCode.Name.Format("%.*s", UNameLen, pObject->hnam);
				jCode.Name.TrimRight(" ");
				m_pHCode.Add(jCode);
			}
		}
		**/
		break;
	}

	for (ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		Str.Format(_T("%s  %s"), LPCTSTR(jCode.Code), LPCTSTR(jCode.Name));
		AddString(Str);
	}
	m_DataMode = DM_HISTORY;
}

bool CCodeCombo::IsHistoryMode() 
{ 
	return (m_DataMode == DM_HISTORY)? true:false; 
}

bool CCodeCombo::IsCodeListMode()
{
	return (m_DataMode == DM_CODELIST)? true:false; 
}

bool CCodeCombo::JCodeLoad(CString tabPath)
{
	if (m_pJCode.GetSize() > 0)
		return true;

	char	buf[1024];
	int	bufsize;
	DWORD	dw;
	int	res;
	struct  hjcode  *pHJCode;

	CString path = tabPath + HJCODE;
  	HANDLE fh = CreateFile(path, GENERIC_READ, 0, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (fh == INVALID_HANDLE_VALUE)
	{
		AfxGetMainWnd()->MessageBox("종목 화일을 읽을 수 없습니다.", COMPANYNAME);
		CloseHandle(fh);
		return false;
	}

	struct	_JCode	jCode;
//	char fname;
	bufsize = sizeof(struct hjcode);
	for (int ii = 0; ;ii++)
	{
		ZeroMemory(buf, sizeof(buf));
		res = ReadFile(fh, buf, bufsize, &dw, NULL);
		if (!res || (bufsize != (int)dw))
			break;

		pHJCode = (struct hjcode *)buf;

		jCode.Name = CString(pHJCode->hnam, HNameLen);
		jCode.Name.MakeUpper();
		jCode.Name.TrimRight();

//		fname = jCode.Name.GetAt(0);
//		if (isalpha(fname) || (fname & 0x80))
//			jCode.Name = jCode.Name;
//		else
			jCode.Name = jCode.Name.Mid(1);
		switch ((char)pHJCode->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
			switch (pHJCode->ssgb)
			{
			case jmSINJU:	case jmSINJS:	case jmELW:	case jmHYFND:
				continue;
			default:
				break;
			}

			jCode.Code = CString(pHJCode->code, HCodeLen);
			res = jCode.Code.Find('\0');
			if (res != -1)
				jCode.Code = jCode.Code.Left(res);
			jCode.Code.TrimRight();
			if (jCode.Code.GetLength() > sz_JCODE + 1)
				continue;

			break;
		}

		m_pJCode.Add(jCode);
	}
  	CloseHandle(fh);

	m_pJCode.QuickSort();

	return true;
}

bool CCodeCombo::UCodeLoad(CString tabPath)
{
	if (m_pUCode.GetSize() > 0)
		return true;

	CString path = tabPath + UPCODE;
	ULONGLONG nFSize = 0;
	UINT	nSize = sizeof(struct upcode);
	CFile	file;
	struct  upcode  UPCodh;
	struct	upcode*	pObject = NULL;

	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
	{	
		AfxGetMainWnd()->MessageBox(_T("업종 화일을 읽을 수 없습니다."), COMPANYNAME);
		return false;
	}

	nFSize = file.GetLength() / nSize;

	for (int ii = 0; ii < nFSize; ii++)
	{
		file.Read(&UPCodh, nSize);
		pObject = new struct upcode;
		memcpy(pObject, &UPCodh, nSize);
		m_pUCode.Add(pObject);
	}

	file.Close();

	return true;
}

bool CCodeCombo::IsListShow()
{
	if (!m_pSubList || !m_pSubList->GetSafeHwnd())
		return false;

	int	state = m_pSubList->IsWindowVisible();
	return (state == TRUE)? true:false;
}

// for IBK
/*
void CCodeCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	BOOL bFocus=(lpDrawItemStruct->itemAction & ODA_FOCUS);
	BOOL bDrawEntire=(lpDrawItemStruct->itemAction & ODA_FOCUS);
	BOOL bSelected = ((lpDrawItemStruct->itemState & ODS_SELECTED));

	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);
	CRect rcItem(lpDrawItemStruct->rcItem);
	CRect rcText;
	CString	str;

	int	top = rcItem.top + (rcItem.Height() - m_TextHeight)/2;
	if (bFocus||bSelected)
	{
		pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&rcItem,&CBrush(::GetSysColor(COLOR_HIGHLIGHT)));
		
		rcItem = lpDrawItemStruct->rcItem;	
		rcItem.SetRect(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom);
		if(lpDrawItemStruct->itemID>=0)
		{
			GetLBText(lpDrawItemStruct->itemID, str);
			pDC->TabbedTextOut(rcItem.left+4, top, str, 0, NULL, rcItem.left);
		}

		rcItem.SetRect(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom);
		pDC->DrawFocusRect(&rcItem);
	}
	else
	{
		pDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		pDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&lpDrawItemStruct->rcItem,&CBrush(::GetSysColor(COLOR_WINDOW)));
		
		rcItem = lpDrawItemStruct->rcItem;	
		rcItem.SetRect(rcItem.left,rcItem.top,rcItem.right,rcItem.bottom);
		if(lpDrawItemStruct->itemID>=0)
		{
			GetLBText(lpDrawItemStruct->itemID, str);
			pDC->TabbedTextOut(rcItem.left+4, top, str, 0, NULL, rcItem.left);
		}
	}
}

void CCodeCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// all items are of fixed size
	// must use CBS_OWNERDRAWVARIABLE for this to work
	lpMeasureItemStruct->itemHeight = m_TextHeight + 4;
}

void CCodeCombo::AddString(LPCTSTR str)
{
	int	index = CComboBox::AddString(str);
	if (index != CB_ERR)
		SetItemHeight(index, m_TextHeight + m_TextGap);
}*/
/////////////////////////////////////////////////////////////////////////////
// CCodeEdit

CCodeEdit::CCodeEdit(CCodeCtrl *pCombo, CWnd* pTarget, int type)
{
	m_Type = type;
	m_pParent = pCombo;
	m_pTarget = pTarget;

	m_Unit = 0;
	m_preCode = _T("");
	m_bInitFocus = true;
	m_bKeyChange = false;
	m_bListKey = false;
	m_SaveMode = -1;
}

CCodeEdit::~CCodeEdit()
{
}


BEGIN_MESSAGE_MAP(CCodeEdit, CEdit)
	//{{AFX_MSG_MAP(CCodeEdit)
	ON_WM_CHAR()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCodeEdit message handlers

void CCodeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString tmpS = _T("");
	GetWindowText(tmpS);
	int len = tmpS.GetLength();

	m_bListKey = false;
	switch (nChar)
	{
	case VK_RETURN:
		if (m_pParent->m_pCombo->IsListShow())
		{
			tmpS = m_pParent->m_pCombo->GetSelectCode(tmpS);
			m_pParent->m_pCombo->ShowDropDown(FALSE);
			SetWindowText(tmpS);
			m_bListKey = true;

			m_preCode = tmpS;
			m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
			SetSel(0, -1);
			
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}

		len = tmpS.GetLength();
		if (len <= 0)
		{
			CEdit::OnChar(nChar, nRepCnt, nFlags);
			return;
		}

		switch (m_Unit)
		{
		case GU_CODE:
			if (!m_pParent->IsAllNumbers(tmpS) || tmpS.GetLength() < 6)
			{
				CEdit::OnChar(nChar, nRepCnt, nFlags);
				return;
			}

			tmpS = m_pParent->m_pCombo->GetSelectCode(tmpS);
			m_pParent->m_pCombo->ShowDropDown(FALSE);
			SetWindowText(tmpS);
			m_bListKey = true;

			m_preCode = tmpS;
			m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
			SetSel(0, -1);
			break;
		case GU_INDEX:
			if (tmpS.GetLength() == sz_INDEX)
			{
				m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
				SetSel(0, -1);
			}
			break;
		case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
			if (tmpS.GetLength() >= 5)
			{
				m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
				SetSel(0, -1);
			}
			break;
		case GU_CFUTURE:
		case GU_FFUTURE:	// 20090918: ADD : 상품 선물
			if (tmpS.GetLength() >= 5)
			{
				m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
				SetSel(0, -1);
			}
			break;
		case GU_FOREIGN:
			if (!tmpS.IsEmpty())
			{
				m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
				SetSel(0, -1);
			}
			break;
		}

		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	case VK_TAB:
		m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, ipTab), m_Type);

		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return;
	}
		
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CCodeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeyChange = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CCodeEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
/*	if (nFlags == 0x8000)
	{
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}
*/

	if (!m_bKeyChange)
	{
		CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
		return;
	}
	m_bKeyChange = false;

	CString	tmpS;
	GetWindowText(tmpS);
	int	len = tmpS.GetLength();

	if (len <= 0)
	{
		m_preCode = tmpS;
		m_bListKey = false;
		return;
	}

	if (tmpS == m_preCode)
	{
		m_bListKey = false;
		return;
	}

	m_preCode = tmpS;

	switch(m_Unit)
	{
	case GU_CODE:
		if (!m_pParent->IsAllNumbers(tmpS))
		{
			m_pParent->CodeListMode();
			m_pParent->m_pCombo->ShowDropDown(TRUE);
			::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
			m_pParent->m_pCombo->SearchCode(tmpS);
			m_bKeyChange = false;

			return;
		}

		if (len >= sz_JCODE && !m_bListKey)
		{
			m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
			SetSel(0, -1);
		}
		break;
	case GU_INDEX:
		if (len == sz_INDEX)
		{
			m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
			SetSel(0, -1);
		}
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		if (false && tmpS.GetLength() >= 5)
		{
			m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)tmpS.operator LPCTSTR());
			SetSel(0, -1);
		}
		break;
	case GU_CFUTURE:
	case GU_FFUTURE:	// 20090918: ADD
		break;
	}

	m_bListKey = false;
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CCodeEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	DWORD	sel = GetSel();

	if (HIWORD(sel) - LOWORD(sel) > 0)
		SetSel(-1, -1);
	else
		SetSel(0, -1);
}

BOOL CCodeEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	int	key = (zDelta > 0)? VK_UP:VK_DOWN;
	if (m_Unit == GU_CODE)
	{
		m_pParent->m_pCombo->Key(key);
	}
	
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CCodeEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);

	if (m_bInitFocus)
	{
		m_bInitFocus = false;

		if (m_Unit == GU_CODE || m_Unit == GU_INDEX)
			ChangeHangulMode(this->m_hWnd, &m_SaveMode);
	}
	else if (m_SaveMode != -1)
	{
		ChangeHangulMode(this->m_hWnd, &m_SaveMode);
	}

	SetSel(0, -1);
}


void CCodeEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	if (m_Unit == GU_CODE || m_Unit == GU_INDEX)
	{
		DWORD	dwConversion, dwSentence;

		HIMC hImc = ImmGetContext(this->m_hWnd);
		if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
		{
			m_SaveMode = dwConversion;
		}		
	}
}

LRESULT CCodeEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
		case VK_HOME:
		case VK_END:
			if (m_pParent->m_pCombo->IsListShow())
			{
				m_pParent->m_pCombo->Key(wParam);
				return TRUE;
			}
			break;
		}
	}
	
	return CEdit::WindowProc(message, wParam, lParam);
}

void CCodeEdit::SetUnit(int unit)
{
	m_Unit = unit; 
	switch (unit)
	{
	case GU_INDEX:
		SetLimitText(sz_INDEX);
		break;
	default:
		SetLimitText(16);
		break;
	}
}

void CCodeEdit::ListCode(CString code)
{
	SetWindowText("");
	m_preCode = code;
	SetWindowText(code);
	m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(INPUT_CHANGE, IP_CODE), (long)code.operator LPCTSTR());
}

void CCodeEdit::ChangeHangulMode(HWND hWnd, DWORD *saveMode)
{
	DWORD	dwConversion, dwSentence;

	HIMC hImc = ImmGetContext(hWnd);
	if (*saveMode == -1)
	{
		if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
			ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
	}
	else
	{
		if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
			ImmSetConversionStatus(hImc, *saveMode, dwSentence);
	}
}

void CCodeEdit::SetCode(CString code)
{
	SetWindowText("");
	m_preCode.Format(_T("%s"), code.GetString());
	SetWindowText(code);
}
/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl

CCodeCtrl::CCodeCtrl(CWnd *pParent, CWnd *pView, CFont *pFont, int type, LPCTSTR userPath)
{
	m_pParent = pParent;
	m_pView = pView;
	m_pFont = pFont;
	m_Type = type;

	m_pEdit = NULL;
	m_pCombo = NULL;

	m_bBtnDown = false;
	m_userPath = userPath;
}

CCodeCtrl::~CCodeCtrl()
{
}


BEGIN_MESSAGE_MAP(CCodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CCodeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_ENABLE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCodeCtrl message handlers

int CCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// IBK
	CDC	*pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(m_pFont);
	CSize	sz = pDC->GetOutputTextExtent("한");
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);
	// IBK
	
	m_pEdit = new CCodeEdit(this, m_pParent, m_Type);
	m_pEdit->Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|ES_UPPERCASE, CRect(0,0,10,10), this, 0);
	m_pEdit->SetFont(m_pFont);

	m_pCombo = new CCodeCombo(this, sz.cy, 4);						
	m_pCombo->Create(WS_CHILD|WS_VSCROLL|CBS_DROPDOWN, //|CBS_OWNERDRAWVARIABLE|CBS_HASSTRINGS, // IBK
		CRect(0, 0, 10, 10), this, 0);	
	m_pCombo->SetFont(m_pFont);

	CString	tabPath = (char *)m_pView->SendMessage(GEV_VIEW, MAKEWPARAM(viewInterface, getTabPath), 0);
	m_pCombo->JCodeLoad(tabPath);
	m_pCombo->UCodeLoad(tabPath);
	
	return 0;
}

void CCodeCtrl::OnDestroy() 
{	
	if (m_pEdit)
	{
		m_pEdit->DestroyWindow();
		delete m_pEdit;
		m_pEdit = NULL;
	}
	if (m_pCombo)
	{
		m_pCombo->DestroyWindow();
		delete m_pCombo;
		m_pCombo = NULL;
	}

	CWnd::OnDestroy();
}

void CCodeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_btnRect.PtInRect(point))
	{
		m_bBtnDown = true;
		InvalidateRect(&m_btnRect);
		CDC *pDC = GetDC();
		Draw(pDC);
		ReleaseDC(pDC);

		if (m_pCombo->IsListShow())
		{
			m_pCombo->ShowDropDown(FALSE);
		}
		else
		{
			HistoryMode();
			m_pCombo->ShowDropDown(TRUE);
		}

		if (m_pCombo->GetCount() > 0)
			m_pCombo->SetCurSel(0);

		CWnd	*pWnd = GetFocus();
		if (pWnd != m_pEdit)
			m_pEdit->SetFocus();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CCodeCtrl::LButtonUp()
{
	m_bBtnDown = false;
	InvalidateRect(&m_btnRect);
}

void CCodeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_pEdit->SetFocus();
}

void CCodeCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	Draw(&dc);
}

void CCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect	rect, btnRC;
	GetClientRect(&rect);

	rect.DeflateRect(2,2);
	m_editRect = btnRC = rect;
	btnRC.left = btnRC.right - btnRC.Height() + 1;

	m_editRect.right = btnRC.left;
	m_editRect.left += 2;
	m_editRect.top += 2;

	m_pEdit->MoveWindow(m_editRect.left, m_editRect.top, m_editRect.Width(), m_editRect.Height());
	m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	m_pCombo->MoveWindow(0, m_editRect.top-2, rect.Width(), 140);
	//m_pCombo->MoveWindow(0, m_editRect.top-2, rect.Width(), m_pCombo->GetDataHeight()*21);

	m_btnRect = btnRC;
}

CString CCodeCtrl::GetEditData()
{
	CString	code;
	m_pEdit->GetWindowText(code);

	return code;
}

void CCodeCtrl::SetEditData(CString sData)
{
	m_pEdit->SetCode(sData);
}

void CCodeCtrl::SetUnit(int unit)
{
	m_Unit = unit;
	m_pEdit->SetUnit(unit);
}

void CCodeCtrl::SetKkind(int kkind)
{
	m_Kkind = kkind;
}

void CCodeCtrl::HistoryMode()
{
	CString	symbol, history;

	switch (m_Unit)
	{
	case GU_CODE:
		symbol = JTRG;
		history = (char *)m_pView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getHistory), (long)symbol.operator LPCTSTR());
		break;
	case GU_INDEX:
		history = GetHistory(ITRG);
		break;
	case GU_FUTURE:	case GU_OPTION:	case GU_COFUT:	case GU_COOPT:
		symbol = FOTRG;
		history = (char *)m_pView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getHistory), (long)symbol.operator LPCTSTR());
		break;
	case GU_FFUTURE:	// 20090916 : ADD : 상품 선물
	case GU_CFUTURE:	// 20090916 : ADD : 채권, 금리 선물
		symbol = CDFTRG;
		history = (char *)m_pView->SendMessage(GEV_VIEW, MAKEWPARAM(viewMain, getHistory), (long)symbol.operator LPCTSTR());
		break;
	case GU_FOREIGN:
		history = GetHistory(STRG);
		break;
	default:
		return;
	}
	m_pCombo->InitHistory(history);
}

void CCodeCtrl::CodeListMode()
{
	if (!m_pCombo->IsCodeListMode())
		m_pCombo->InitCodeList();
}

bool CCodeCtrl::IsAllNumbers(CString sCode)
{
	int	size = sCode.GetLength();
	if (size > 6)
		return false;

	size = (size > 5)? 5:size;
	for (int ii = 0; ii < size; ii++)
	{
		char	code = sCode.GetAt(ii);
		if ('0' > code || code > '9')
			return false;
	}

	return true;
}

void CCodeCtrl::OnEnable(BOOL bEnable) 
{
//	m_pEdit->EnableWindow(bEnable);

	CWnd::OnEnable(bEnable);
}

void CCodeCtrl::Draw(CDC *pDC)
{
/*
	COLORREF bg, fg;
	if (m_pParent != NULL)
	{
		bg = m_cbRGB;
		fg = m_csRGB;
	}
	else
	{
		bg = COLOR_PAL_BG;
		fg = COLOR_PAL_FG;
	}
 
	CRect WinRC, rcBtn, tRc;
	CPoint	pts[3];
	GetClientRect(&WinRC);
	tRc.SetRect(WinRC.left, WinRC.top, m_editRect.left, WinRC.bottom);
	pDC->FillSolidRect(&tRc, RGB(255,255,255));
	tRc.SetRect(m_editRect.right, WinRC.top, WinRC.right, WinRC.bottom);
	pDC->FillSolidRect(&tRc, RGB(255,255,255));
	tRc.SetRect(WinRC.left, WinRC.top, WinRC.right, m_editRect.top);
	pDC->FillSolidRect(&tRc, RGB(255,255,255));
	tRc.SetRect(WinRC.left, m_editRect.bottom, WinRC.right, WinRC.bottom);
	pDC->FillSolidRect(&tRc, RGB(255,255,255));
	pDC->DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   

	rcBtn = WinRC;
	rcBtn.DeflateRect(0, 3, 2, 3);
	rcBtn.left = rcBtn.right - rcBtn.Height();
	tRc = rcBtn;
	tRc.DeflateRect(1, 0);
	
	CPen	cPen(PS_SOLID, 1, bg);
	CBrush	cBrush(bg);
	CPen	*sPen = pDC->SelectObject(&cPen);
	CBrush	*sBrush = pDC->SelectObject(&cBrush);
	pDC->FillRect(tRc, &cBrush);

	pDC->MoveTo(rcBtn.left, rcBtn.top+1);
	pDC->LineTo(rcBtn.left, rcBtn.bottom-1);
	pDC->MoveTo(rcBtn.right-1, rcBtn.top+1);
	pDC->LineTo(rcBtn.right-1, rcBtn.bottom-1);

	CPen	cPen1(PS_SOLID, 1, fg);
	CBrush	cBrush1(fg);
	pDC->SelectObject(&cPen1);
	pDC->SelectObject(&cBrush1);

	tRc = rcBtn;
	tRc.DeflateRect(3, tRc.Height()*2/5);
	tRc.OffsetRect(1, 0);
	tRc.right = tRc.left + (tRc.Width()/2)*2-1;
	tRc.bottom = tRc.top + tRc.Width()/2;
	pts[0].y = tRc.bottom;
	pts[1].y = tRc.top;
	pts[2].y = tRc.top;
	pts[0].x = tRc.left + (tRc.Width()/2);
	pts[1].x = tRc.right-1;
	pts[2].x = tRc.left;
	pDC->Polygon(pts, 3);

	pDC->SelectObject(sPen);
	pDC->SelectObject(sBrush);
*/
	CRect	WinRC, rect;
	GetClientRect(&WinRC);

	rect.SetRect(WinRC.left, WinRC.top, m_editRect.left, WinRC.bottom);
	pDC->FillSolidRect(&rect, RGB(255,255,255));
	rect.SetRect(m_editRect.right, WinRC.top, WinRC.right, WinRC.bottom);
	pDC->FillSolidRect(&rect, RGB(255,255,255));
	rect.SetRect(WinRC.left, WinRC.top, WinRC.right, m_editRect.top);
	pDC->FillSolidRect(&rect, RGB(255,255,255));
	rect.SetRect(WinRC.left, m_editRect.bottom, WinRC.right, WinRC.bottom);
	pDC->FillSolidRect(&rect, RGB(255,255,255));
	
	pDC->DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   

	if (m_bBtnDown)
		pDC->DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX|DFCS_PUSHED);
	else
		pDC->DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
}


CString CCodeCtrl::GetHistoryFileName(LPCTSTR key)
{
	CString fileName;
	fileName.Format(_T("%s%s_history.dat"), m_userPath.GetString(), key);
	return fileName;
}

CString CCodeCtrl::GetHistory(LPCTSTR key)
{
	CString history = _T("");
	CString fileName = GetHistoryFileName(key);
	CFile* pFile = nullptr;

	TRY {
		pFile = new CFile(fileName, CFile::modeRead);
	}
	CATCH (CFileException, e) {
		return history;
	}
	END_CATCH;

	UINT fileL = (UINT) pFile->GetLength();
	LPSTR buffer = new char[fileL +1];
	pFile->Read(buffer, fileL); buffer[fileL] = 0x00;
	pFile->Close();

	history = buffer;

	delete [] buffer;
	delete pFile;

	return history;
}

#define MAX_HISTORY 20

void CCodeCtrl::AddHistory(LPCTSTR key, LPCTSTR code, LPCTSTR name)
{
	if (!strlen(code) || !strlen(name))
		return;

	CString history = GetHistory(key);
	CString fileName = GetHistoryFileName(key);

	if (!history.IsEmpty()) 
	{
		TRY 
		{
			CFile::Remove(fileName);
		}
		CATCH(CFileException, e) 
		{
		}
		END_CATCH;
	}

	_JCode	jCode;
	CArray<_JCode, _JCode> arr;
	CString	str;
	int	find;

	jCode.Code = code;
	jCode.Name = name;

	arr.Add(jCode);

	while (TRUE) 
	{
		find = history.Find('\t');

		if (find < 1) 
			break;

		str = history.Left(find);
		history.Delete(0, find + 1);

		find = str.Find(' ');
		if (find > 0) 
		{
			jCode.Code = str.Left(find);
			jCode.Name = str.Mid(find + 1);

			if (jCode.Code != code)
				arr.Add(jCode);
		}
	}

	if (arr.GetSize() > MAX_HISTORY) 
	{
		arr.SetSize(MAX_HISTORY);
	}

	history.Empty();

	for (int i = 0; i < arr.GetSize(); i++) 
	{
		str.Format(_T("%s %s\t"), LPCTSTR(arr[i].Code), LPCTSTR(arr[i].Name));
		history += str;
	}

	CFile* pFile;

	TRY 
	{
		pFile = new CFile(fileName, CFile::modeCreate | CFile::modeWrite);
	}
	CATCH (CFileException, e) 
	{
		return;
	}
	END_CATCH;

	pFile->Write((LPCTSTR)history, history.GetLength());

	delete pFile;
}
