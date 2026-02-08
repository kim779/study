share// CodeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../IB202700.h"
#include "IHCodeCtrl.h"
#include "../common.h"
#include "../../../h/jmcode.h"
#include <imm.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CListWnd
CIHSubList::CIHSubList(class CIHCodeCtrl *pCodeCtrl) 
{ 
	m_pCodeCtrl = pCodeCtrl; 
}

CIHSubList::~CIHSubList() 
{ 
}

LRESULT CIHSubList::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_LBUTTONUP)
	{
		m_pCodeCtrl->LButtonUp();
	}

	return CWnd::WindowProc(message, wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////
// CIHCodeCombo

CIHCodeCombo::CIHCodeCombo(class CIHCodeCtrl *pParent)
{
	m_pParent = pParent;
	m_DataMode = DM_NONE;
	m_hList = nullptr;
}

CIHCodeCombo::~CIHCodeCombo()
{
}


BEGIN_MESSAGE_MAP(CIHCodeCombo, CComboBox)
	//{{AFX_MSG_MAP(CIHCodeCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIHCodeCombo message handlers

void CIHCodeCombo::OnSelchange() 
{
	const int	index = GetCurSel();
	_JCode	jCode;
	if (index >= 0)
	{
		if (IsCodeListMode())
		{
			jCode = m_pJCode.GetAt(index);
			m_pParent->m_pEdit->ListCode(jCode.Code.Mid(1));
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

int CIHCodeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	SetHorizontalExtent(140);
	SetDroppedWidth(140);

	m_pSubList = std::make_unique<CIHSubList>(m_pParent);

	return 0;
}

void CIHCodeCombo::OnDestroy() 
{
	if (m_pSubList->m_hWnd)
		m_pSubList->UnsubclassWindow();
//	delete m_pSubList;
	m_pJCode.RemoveAll();
	m_pHCode.RemoveAll();
	CComboBox::OnDestroy();
}

HBRUSH CIHCodeCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	switch (nCtlColor)
	{
	case CTLCOLOR_LISTBOX:
		{
			if (m_hList == nullptr)
			{
				m_hList = pWnd->m_hWnd;
				m_pSubList->SubclassWindow(m_hList);
			}
		}
		break;
	}

	return hbr;
}

void CIHCodeCombo::SearchCode(CString code)
{
	if (code.IsEmpty())
	{
		SetTopIndex(0);
		SetCurSel(0);
		return;
	}

	code.MakeUpper();

	_JCode	jCode;
	const int	len = code.GetLength();

	if (IsCodeListMode())
	{
		const int	size = m_pJCode.GetSize();
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
		const int	size = m_pHCode.GetSize();
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

CString CIHCodeCombo::GetSelectCode()
{
	const int	index = GetCurSel();
	if (index < 0)
		return "";

	if (IsCodeListMode())
	{
		_JCode	jCode = m_pJCode.GetAt(index);
		m_sCode = jCode.Code.Mid(1);
	}
	else if (IsHistoryMode())
	{
		_JCode	jCode = m_pHCode.GetAt(index);
		m_sCode = jCode.Code.Mid(1);
	}

	return m_sCode;
}

void CIHCodeCombo::Key(int key)
{
	const int	curIndex = GetCurSel();

	if (curIndex < 0)	return;

	CRect	cRC;
	int	count = 9;
	if (m_pSubList)
	{
		m_pSubList->GetClientRect(&cRC);
		const int	iHeight = GetItemHeight(curIndex);
		count = cRC.Height() / iHeight;
	}

	int	topIndex = GetTopIndex();
	const int	endIndex = topIndex+count-1;
	int	index = curIndex;
	bool	bSetTopIndex = false;
	switch (key)
	{
	case VK_UP:	// up arrow
		index = max(0, index-1);
		if (index < topIndex)	topIndex = index;
		break;
	case VK_DOWN:	// down arrow
		index = min(GetCount()-1, index+1);
		if (index > endIndex)	topIndex += 1;
		bSetTopIndex = true;
		break;
	case VK_PRIOR:	// pgdn
		index = max(0, index-count);
		topIndex = index;
		break;
	case VK_NEXT:	// pgup
		index = min(GetCount()-1, index+count);
		topIndex = min(GetCount()-1 - (count-1), index - (count-1));
		bSetTopIndex = true;
		break;
	case VK_HOME:	// home
		index = 0;
		topIndex = index;
		break;
	case VK_END:	// end
		index = GetCount()-1;
		topIndex = min(GetCount()-1 - (count-1), index - (count-1));
		break;
	default:
		return;
	}

	if (index >= 0 && index < GetCount())
	{
		SetCurSel(index);
		if (bSetTopIndex)	SetTopIndex(topIndex);
	}
}

void CIHCodeCombo::InitCodeList()
{
	ResetContent();

	_JCode	jCode;
	CString	Str;
	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);
		if (!jCode.Code.GetLength())
			continue;
		Str.Format("%s  %s", jCode.Code.Mid(1), jCode.Name);
		AddString(Str);
	}
	m_DataMode = DM_CODELIST;
}

void CIHCodeCombo::InitHistory(CString history)
{
	ResetContent();

	m_pHCode.RemoveAll();
	_JCode	jCode;
	CString	Str;
	int	index{};
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

	for (int ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		Str.Format("%s  %s", jCode.Code, jCode.Name);
		AddString(Str);
	}
	m_DataMode = DM_HISTORY;
}

bool CIHCodeCombo::IsHistoryMode() 
{ 
	return (m_DataMode == DM_HISTORY)? true:false; 
}

bool CIHCodeCombo::IsCodeListMode()
{
	return (m_DataMode == DM_CODELIST)? true:false; 
}

bool CIHCodeCombo::JCodeLoad(CString tabPath)
{
	DllInvoker dll("AxisCode");
	if (!dll.IsLoaded())
		return false;
	auto map = (std::map<CString, struct hjcodex*>*)dll.Function<char* WINAPI(int)>("getArray")(1);
	if (map->size() <= 0)
		return false;

	for_each(map->begin(), map->end(), [&](const auto item) {
		switch (item.second->kosd)
		{
		case jmKOSPI:
		case jmKOSDAQ:
		case jm3RD:
		default:
			if (item.second->ssgb == jmELW || (item.second->ssgb != jmSINJU && item.second->ssgb != jmSINJS))
			{
				struct	_JCode	jCode;
				jCode.Code = CString(item.second->code, HCodeLen).Trim();
				jCode.Name = CString(item.second->hnam, HNameLen).Mid(1).Trim();

				m_pJCode.Add(jCode);
			}
			break;
		}
		});
	m_pJCode.QuickSort();
	return true;
}

bool CIHCodeCombo::ELWCodeLoad(CString tabPath)
{
	DllInvoker dll("AxisCode");
	if (!dll.IsLoaded())
		return false;
	auto map = (std::map<CString, struct hjcodex*>*)dll.Function<char* WINAPI(int)>("getArray")(1);
	if (map->size() <= 0)
		return false;

	for_each(map->begin(), map->end(), [&](const auto item) {
		switch (item.second->ssgb)
		{
		case jmELW:
		default:
			if (item.second->ssgb == jmELW)
			{
				struct	_JCode	jCode;
				jCode.Code = CString(item.second->code, HCodeLen).Mid(1).Trim();
				jCode.Name = CString(item.second->hnam, HNameLen).Mid(1).Trim();
			}
			break;
		}
		});
	m_pJCode.QuickSort();
	return true;
}

bool CIHCodeCombo::FCodeLoad(CString tabPath)
{
//	if (m_pJCode.GetSize() > 0)
//		return true;

	char	buf[1024]{};
	int	res{};
	DWORD	dw{};
	int	bufsize{};
	struct	fjcode* pFjCode{};
	
	CString path = tabPath + "\\" + FJCODE;
	HANDLE fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 0, nullptr,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}
	
	struct	_JCode	fCode;
	bufsize = sizeof(struct fjcode);
	for (int ii = 0; ; ii++)
	{
		res = ReadFile(fh, buf, bufsize, &dw, nullptr);
		if (bufsize != (int)dw)
			break;

		pFjCode = (struct fjcode *)buf;

		fCode.Code = CString(pFjCode->cod2, FCodeLen);
		fCode.Code.TrimRight();
		fCode.Name = CString(pFjCode->hnam, FNameLen);
		fCode.Name.TrimRight();
		m_pJCode.Add(fCode);
	}

	CloseHandle(fh);

	m_pJCode.QuickSort();
	return true;
}

bool CIHCodeCombo::OCodeLoad(CString tabPath)
{
//	if (m_pJCode.GetSize() > 0)
//		return true;

	char	buf[1024 * 8]{};
	DWORD	dw{};
	int	bufsize{};
	struct	ojcode* pOjCode{};
	
	CString	path = tabPath + "\\" + OJCODE;
	HANDLE	fh = CreateFile(path.operator LPCTSTR(), GENERIC_READ, 
		0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (fh == INVALID_HANDLE_VALUE)
	{
		CString	msg; msg.Format("[%s] file open error!!", path);
		AfxMessageBox(msg); CloseHandle(fh);
		return false;
	}

	bufsize = sizeof(struct ojcodh);
	ZeroMemory(buf, sizeof(buf));
	ReadFile(fh, buf, bufsize, &dw, nullptr);
	if (bufsize != (int)dw)
	{
		CloseHandle(fh);
		return false;
	}
	
	bufsize = sizeof(struct ojcode);
	ZeroMemory(buf, sizeof(buf));
	int	AtmIdx = -1;
	CString	tmp;
	int	res{};
	
	for (int ii = 0; ; ii++)
	{
		ReadFile(fh, buf, bufsize, &dw, nullptr);
		if (bufsize != (int)dw)
			break;

		pOjCode = (struct ojcode *)buf;
		
		tmp = CString(pOjCode->price, OPriceLen);
		oCode.Price.Format("%.2f", float(atoi(tmp) / 100.0));
		oCode.Atm = pOjCode->atmg;
		if (pOjCode->atmg == 1)
			AtmIdx = ii;
		
		for (int jj = 0; jj < 4; jj++)
		{
			oCode.Call[jj].Exist = pOjCode->call[3-jj].yorn;
			oCode.Call[jj].Code = CString(pOjCode->call[3-jj].cod2, OCodeLen);
			oCode.Call[jj].Name = CString(pOjCode->call[3-jj].hnam, ONameLen);
			res = oCode.Call[jj].Name.Find('\0');
			if (res != -1)
				oCode.Call[jj].Name = oCode.Call[jj].Name.Left(res);
			oCode.Call[jj].Name.TrimRight();
			
			oCode.Put[jj].Exist = pOjCode->put[jj].yorn;
			oCode.Put[jj].Code = CString(pOjCode->put[jj].cod2, OCodeLen);
			oCode.Put[jj].Name = CString(pOjCode->put[jj].hnam, ONameLen);
			res = oCode.Put[jj].Name.Find('\0');
			if (res != -1)
				oCode.Put[jj].Name = oCode.Put[jj].Name.Left(res);
			oCode.Put[jj].Name.TrimRight();

			struct _JCode tOCode;
			if (oCode.Call[jj].Exist == '1')
			{
				tOCode.Code = oCode.Call[jj].Code;
				tOCode.Name = oCode.Call[jj].Name;
				m_pJCode.InsertAt(0, tOCode);
			}
			if (oCode.Put[jj].Exist == '1')
			{
				tOCode.Code = oCode.Put[jj].Code;
				tOCode.Name = oCode.Put[jj].Name;
				m_pJCode.InsertAt(0, tOCode);
			}
		}
	}

	CloseHandle(fh);
	m_pJCode.QuickSort();
	return true;
}

bool CIHCodeCombo::IsValidCode(CString sCode)
{
	if (sCode.IsEmpty())
		return false;
	
// updateX_20060209
	CString	Acode, Jcode;
	Acode = "A" + sCode; Jcode = "J" + sCode;

	_JCode	jCode;
	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);

		if (!Acode.Compare(jCode.Code) || !Jcode.Compare(jCode.Code))
			return true;
	}

	return false;
}
void CIHCodeCombo::RemoveCode()
{
	m_pJCode.RemoveAll();
}

/////////////////////////////////////////////////////////////////////////////
// CIHCodeEdit

CIHCodeEdit::CIHCodeEdit(CIHCodeCtrl *pCombo, CWnd* pTarget)
{
	m_pParent = pCombo;
	m_pTarget = pTarget;

	m_Unit        = GU_NONE;
	m_preCode     = _T("");
	m_bInitFocus  = true;
	m_bKeydown    = false;
	m_bValidCheck = true;
}

CIHCodeEdit::~CIHCodeEdit()
{
}


BEGIN_MESSAGE_MAP(CIHCodeEdit, CEdit)
	//{{AFX_MSG_MAP(CIHCodeEdit)
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIHCodeEdit message handlers

void CIHCodeEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
//	TRACE("killfocus\n");
	GetParent()->SendMessage(WM_MANAGE, MK_KILLFOCUS);
		
}

void CIHCodeEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int	len{};
	CString tmpS = _T("");
	GetWindowText(tmpS);

	if (tmpS.IsEmpty() && nChar >= '0' && nChar <= '9')
		ChangeHangulMode(this->m_hWnd, false);

	switch (nChar)
	{
	case VK_RETURN:
		len = tmpS.GetLength();
		if (len <= 0)
		{
			if (len == 0)
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
			return;
		}

		if (m_Unit == GU_CODE || m_Unit == GU_ELWCODE)
		{
			if (!isHexNumeric(tmpS))
			{
				CString	tmpS = m_pParent->m_pCombo->GetSelectCode();
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
				SetWindowText(tmpS);					
			}
			else
			{
				m_pParent->m_pCombo->m_DataMode = 0;
// updateX_20060205
				if (len < 6)
				{
					CString	string; string.Format("%06d", atoi(tmpS)); SetWindowText(string); SetSel(0, -1);
				}
			}
			m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
			return;
		}
		else if (m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION)
		{
			if (m_Unit == GU_FUTURE && (len == 5 && (tmpS.GetAt(0) == '1' || tmpS.GetAt(0) == 'A')))  //파생상품 코드개편
			{
				tmpS += "000";
				SetWindowText(tmpS);
				SetSel(0, -1);
				break;
			}

			GetWindowText(tmpS);
			if (!tmpS.IsEmpty())
			{
				if (!isHexNumeric(tmpS))
				{
					CString	tmpS = m_pParent->m_pCombo->GetSelectCode();
					if (m_pParent->m_pCombo->GetDroppedState())
						m_pParent->m_pCombo->ShowDropDown(FALSE);
					SetWindowText(tmpS);					
				}
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
			}
		}
		else
		{
			m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
		}
		break;
	case VK_TAB:
		//m_pTarget->SendMessage(GEV_INPUT, MAKEWPARAM(inputChange, ipTab), m_Type);
		break;
	}
	CEdit::OnChar(nChar, nRepCnt, nFlags);
}

void CIHCodeEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_bKeydown = true;
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIHCodeEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
	//TRACE("%ld", nFlags);
	/*if (!m_bKeydown)
		return;
	m_bKeydown = false;*/
	if (nFlags == 0x8000)
		return;
	int len{};
	if (nChar != VK_RETURN)
	{
		CString	sTmp = _T("");
		GetWindowText(sTmp);
		
		len = sTmp.GetLength();

		if (len <= 0)
		{
			m_preCode = sTmp;
			return;
		}
		
		if (sTmp == m_preCode)
			return;
		
		m_preCode = sTmp;
	
		switch (m_Unit)
		{
		case GU_CODE:
		case GU_ELWCODE:
			if (!isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
			if ((len == sz_JCODE && m_bValidCheck 
				&& (m_pParent->m_pCombo->IsValidCode(sTmp)) || (!m_bValidCheck && len >= sz_JCODE)))
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			else if (len >= sz_OPTION)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		case GU_FUTURE: 
			if (!isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
			if (len == 5 && (sTmp.GetAt(0) == '1' || sTmp.GetAt(0) == 'A'))   //파생상품 코드개편
			{
				sTmp += "000";
				SetWindowText(sTmp);
				SetSel(0, -1);
				len = 8;
			}
			if (len == sz_FCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		case GU_OPTION:
			if (!isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
		case GU_POPTION:
			if (len == sz_OPTION)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		case GU_FOCODE:
			if (!isHexNumeric(sTmp))
			{
				m_pParent->CodeListMode();
				if (!m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(TRUE);
				::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
				m_pParent->m_pCombo->SearchCode(sTmp);
				
				return;
			}
			if (len == sz_FOCODE)
			{
				GetParent()->SetWindowText(sTmp);
				m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
				SetSel(0, -1);
				if (m_pParent->m_pCombo->GetDroppedState())
					m_pParent->m_pCombo->ShowDropDown(FALSE);
			}
			break;
		}
	}
}

void CIHCodeEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);
	
	SetSel(0, -1);
}

BOOL CIHCodeEdit::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	const int key = (zDelta > 0)? VK_UP:VK_DOWN;
	if (m_pParent->m_pCombo->GetDroppedState() && 
		(m_Unit == GU_CODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_POPTION || m_Unit == GU_ELWCODE))
	{
		m_pParent->m_pCombo->Key(key);
	}
	
	return CEdit::OnMouseWheel(nFlags, zDelta, pt);
}

void CIHCodeEdit::OnSetFocus(CWnd* pOldWnd) 
{
	if (m_bInitFocus)
	{
		//m_bInitFocus = false;
		ChangeHangulMode(this->m_hWnd);
	}

	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SETFOCUS), (LPARAM)GetParent()->GetSafeHwnd());
	

	SetSel(0, -1);

	CEdit::OnSetFocus(pOldWnd);
}

LRESULT CIHCodeEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
		case VK_RETURN:
			if (m_pParent->m_pCombo->GetDroppedState() &&
				(m_Unit == GU_CODE || m_Unit == GU_FUTURE || m_Unit == GU_OPTION || m_Unit == GU_FOCODE || m_Unit == GU_ELWCODE))
			{
				m_pParent->m_pCombo->Key(wParam);
				return TRUE;
			}
			break;
		}
	}
	
	return CEdit::WindowProc(message, wParam, lParam);
}

void CIHCodeEdit::ListCode(CString code)
{
	SetWindowText("");
	SetWindowText(code);
	GetParent()->SetWindowText(code);
	m_pTarget->SendMessage(WM_COMMAND, MAKEWPARAM(GetParent()->GetDlgCtrlID(), CBN_SELENDOK), (LPARAM)GetParent()->GetSafeHwnd());
}

void CIHCodeEdit::ChangeHangulMode(HWND hWnd, bool bHangle)
{
	DWORD	dwConversion{}, dwSentence{};

	HIMC hImc = ImmGetContext(hWnd);
	if (ImmGetConversionStatus(hImc, &dwConversion, &dwSentence))
	{
		if (bHangle)
			ImmSetConversionStatus(hImc, dwConversion | IME_CMODE_HANGEUL, dwSentence);
		else
			ImmSetConversionStatus(hImc, dwConversion & ~IME_CMODE_HANGEUL, dwSentence);
	}
}

bool CIHCodeEdit::isNumeric(CString str)
{
	for (int ii = 0; ii < str.GetLength(); ii++)
		if (str.GetAt(ii) < '0' || str.GetAt(ii) > '9')
			return false;
	return true;
}

bool CIHCodeEdit::isHexNumeric(CString str)
{
	if (!str.IsEmpty() && (str.GetAt(0) < '0' || str.GetAt(0) > '9'))
		return false;
	for (int ii = 1; ii < str.GetLength(); ii++)
		if ((str.GetAt(ii) < '0' || str.GetAt(ii) > '9') &&
			(str.GetAt(ii) < 'a' || (str.GetAt(ii) > 'f' && str.GetAt(ii) != 's')) &&
			(str.GetAt(ii) < 'A' || (str.GetAt(ii) > 'F' && str.GetAt(ii) != 'S')))
			return false;
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CIHCodeCtrl

CIHCodeCtrl::CIHCodeCtrl(CWnd* parent, CString tabPath)
{
	m_view    = nullptr;
	m_parent  = parent;
	m_tabPath = tabPath;

	m_pEdit   = nullptr;
	m_pCombo  = nullptr;

	m_btnDown = false;
	m_sHistory = "";
	m_bValidCheck = true;
	m_bEdit = false;
	m_bCombo = false;

	m_Unit = GU_NONE;
}

CIHCodeCtrl::~CIHCodeCtrl()
{
	if (m_pEdit)
	{
		m_pEdit->DestroyWindow();
//		delete m_pEdit; m_pEdit = nullptr;
	}
	if (m_pCombo)
	{
		m_pCombo->DestroyWindow();
//		delete m_pCombo; m_pCombo = nullptr;
	}
}


BEGIN_MESSAGE_MAP(CIHCodeCtrl, CWnd)
	//{{AFX_MSG_MAP(CIHCodeCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_MANAGE, OnManage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CIHCodeCtrl message handlers
BOOL CIHCodeCtrl::Create(CWnd* parent, CRect rect, UINT id)
{
	const UINT style = WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS;
	return CWnd::Create(nullptr, _T(""), style, rect, parent, id);
}

LONG CIHCodeCtrl::OnManage(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case MK_KILLFOCUS:
		{
			CWnd*	pWnd = GetFocus();
			if (pWnd)
			{
				HWND	hWnd = pWnd->m_hWnd;
				if (hWnd != m_pEdit->m_hWnd && hWnd != m_pCombo->GetListHwnd())
					ShowWindow(SW_HIDE);
			}
			else
				ShowWindow(SW_HIDE);
		}
		break;
	}
	return 0;
}

int CIHCodeCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_pEdit = std::make_unique<CIHCodeEdit>(this, m_parent);
	m_pEdit->Create(ES_AUTOHSCROLL|WS_VISIBLE|WS_CHILD|ES_UPPERCASE, CRect(0,0,10,10), this, 1001);
	m_pEdit->m_bValidCheck = m_bValidCheck;
	
	m_pCombo = std::make_unique<CIHCodeCombo>(this);
	m_pCombo->Create(CBS_DROPDOWN|WS_CHILD|WS_VISIBLE|WS_VSCROLL, CRect(0, 0, 0, 140), this, 0);
	m_pCombo->SetItemHeight(-1, 0);
	m_pCombo->SetDroppedWidth(150);

	SetUnit(GU_CODE);
	
	return 0;
}

void CIHCodeCtrl::OnDestroy() 
{
	if (m_pEdit)
	{
		m_pEdit->DestroyWindow();
	}
	if (m_pCombo)
	{
		m_pCombo->DestroyWindow();
	}
	CWnd::OnDestroy();
	

}

void CIHCodeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_btnRect.PtInRect(point))
	{
		m_parent->SendMessage(WM_COMMAND, MAKEWPARAM(GetDlgCtrlID(), BN_CLICKED), (LPARAM)GetSafeHwnd());
		m_btnDown = true;
		InvalidateRect(&m_btnRect);
		RedrawWindow();

		HistoryMode();
		m_pCombo->ShowDropDown(TRUE);
		if (m_pCombo->GetCount() > 0)
			m_pCombo->SetCurSel(0);
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CIHCodeCtrl::LButtonUp()
{
	m_btnDown = false;
	InvalidateRect(&m_btnRect);
}

void CIHCodeCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect	WinRC;
	GetClientRect(&WinRC);
	dc.FillSolidRect(WinRC, GetSysColor(COLOR_WINDOW));
	dc.DrawEdge(&WinRC, EDGE_SUNKEN, BF_RECT);   

	if (m_btnDown)
		dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX|DFCS_PUSHED);
	else
		dc.DrawFrameControl(&m_btnRect, DFC_SCROLL, DFCS_SCROLLCOMBOBOX);
}

void CIHCodeCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	m_pEdit->SetFocus();
	m_pEdit->SetSel(0, -1);
}

void CIHCodeCtrl::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	CRect	rect, editRC, btnRC;
	GetClientRect(&rect);

	rect.DeflateRect(2,2);
	editRC = btnRC = rect;
	btnRC.left = btnRC.right - btnRC.Height() + 1;

	editRC.right = btnRC.left;
	editRC.left += 1;
	editRC.top += 1;

	m_pEdit->MoveWindow(editRC.left, editRC.top, editRC.Width(), editRC.Height());
	m_pEdit->SetWindowPos(&wndTop, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	//m_pCombo->MoveWindow(editRC.left, editRC.top, rect.Width(), 140);
	//m_pCombo->SetItemHeight(-1, 0);

	m_btnRect = btnRC;
}

void CIHCodeCtrl::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	m_pEdit->SetFont(m_pFont); m_pCombo->SetFont(m_pFont);
	if (redraw) RedrawWindow();
}

CString CIHCodeCtrl::GetEditData()
{
	CString	code;
	m_pEdit->GetWindowText(code);

	return code;
}


void CIHCodeCtrl::SetEditData(CString sData)
{
	m_pEdit->SetWindowText(sData);
}


void CIHCodeCtrl::SetUnit(int unit)
{
	if (m_Unit == unit) return;

	m_Unit = unit;

	CString code = GetEditData();
	m_pEdit->SetWindowText("");
	m_pEdit->SetUnit(m_Unit);

	m_pCombo->ResetContent();
	m_pCombo->RemoveCode();
	switch(m_Unit)
	{
	case GU_FUTURE:
	case GU_OPTION:
	case GU_POPTION:
	case GU_FOCODE:
		break;
	case GU_ELWCODE:
		m_pCombo->ELWCodeLoad(m_tabPath);
		break;
	case GU_CODE:
	default:
		m_pCombo->JCodeLoad(m_tabPath);
		break;
	}
}

void CIHCodeCtrl::SetHistory(CString sHistory)
{
	m_sHistory = sHistory; 
}

void CIHCodeCtrl::HistoryMode()
{
	CString	symbol;
	switch (m_Unit)
	{
	case GU_CODE:
		symbol = "1301";
		break;
	case GU_FUTURE:
		symbol = "30301";
		break;
	case GU_OPTION:
		symbol = "40301";
		break;
	case GU_POPTION:
		symbol = "70301";
		break;
	case GU_FOCODE:
		symbol = "50301";
		break;
	case GU_ELWCODE:
		symbol = "1301";
		break;
	default:
		symbol = "";
		break;
	}
	m_pCombo->InitHistory(m_sHistory);
}

void CIHCodeCtrl::CodeListMode()
{
	if (!m_pCombo->IsCodeListMode())
		m_pCombo->InitCodeList();
}

void CIHCodeCtrl::SetValidCheck(bool bValid)
{
	m_bValidCheck =bValid;
}

