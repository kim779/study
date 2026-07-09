// CodeCombo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "fx_codectrl.h"
#include "CodeCombo.h"

#include "Controlwnd.h"

#include "../../../h/jmcode.h"
#include "../../../h/axisfire.h"

// CCodeCombo

IMPLEMENT_DYNAMIC(CCodeCombo, CComboBox)

CCodeCombo::CCodeCombo()
{

}

CCodeCombo::CCodeCombo(CControlwnd *pParent)
{
	m_CellHeight = 12;
	m_pParent =pParent;
	m_hList = NULL;
	m_ifind = 0;
}

CCodeCombo::~CCodeCombo()
{
}

void CCodeCombo::ResetCode()
{
	m_pJCode.RemoveAll();
}

BEGIN_MESSAGE_MAP(CCodeCombo, CComboBox)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, &CCodeCombo::OnCbnSelchange)
	ON_WM_CREATE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_DROPDOWN, &CCodeCombo::OnCbnDropdown)
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
END_MESSAGE_MAP()

bool CCodeCombo::IsCodeListMode()
{
	return (m_DataMode == DM_CODELIST)? true:false; 
}

void CCodeCombo::SetFont(CFont* pFont, bool redraw)
{
	m_pFont = pFont;
	CComboBox::SetFont(pFont, FALSE);

	CDC *pDC = GetDC();
	CFont	*sFont = pDC->SelectObject(m_pFont);
	CSize	sz = pDC->GetOutputTextExtent("하");
	m_CellHeight = sz.cy + 4;
	pDC->SelectObject(sFont);
	ReleaseDC(pDC);

	if (m_pSubList)
		m_pSubList->SetListItems(m_pFont, m_CellHeight);
	
	if (redraw) RedrawWindow();
}

void CCodeCombo::OnCbnSelchange()
{
	int	index = GetCurSel();
	_JCode	jCode;
	if (index >= 0)
	{
		if (IsCodeListMode())
		{
			if(m_Unit == 1)
			{

			}
			else if(m_Unit == 0)
			{
				jCode = m_pJCode.GetAt(index);		
				jCode = m_pSearchCode.GetAt(index);	
				m_pParent->m_pEdit->ListCode(jCode.Name);
				m_pParent->m_pEdit->SetSel(jCode.Name.GetLength(), jCode.Name.GetLength());
			}
		}
		else if (IsHistoryMode())
		{
			jCode = m_pHCode.GetAt(index);
			m_pParent->m_pEdit->ListCode(jCode.Name);
			m_pParent->SetEditData(jCode.Name);
		}

		ShowDropDown(FALSE);
		m_pParent->m_pEdit->SetFocus();
		m_pParent->SendCtrlMsg(2);
		SendMessage(WM_KILLFOCUS, 0, 0);
	
		m_pParent->Trigger(jCode.Code);
	}
}

void CCodeCombo::RemoveHistory(CString sSymbol, CString history, CString strcode)
{
	ResetContent();
	ResetString();
	
	_JCode	jCode;
	CString	Str;
	int	index;
	m_pHCode.RemoveAll();
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
			jCode.Name = Str.Mid(index + 1).TrimLeft();
			if(jCode.Code.Find(strcode) >= 0)
				continue;
			m_pHCode.Add(jCode);
		}
	}

	for (int ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		//Str.Format("%s  %s", jCode.Code, jCode.Name);
		Str.Format("%s  %s", jCode.Name, jCode.Code);
		AddString(Str);
	}

	m_DataMode = DM_HISTORY;
	m_sSymbol = sSymbol;
}


void CCodeCombo::InitHistory(CString sSymbol, CString history)
{
	ResetContent();
	ResetString();
	m_pHCode.RemoveAll();
	_JCode	jCode;
	CString	Str;
	int	index;

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
			jCode.Name = Str.Mid(index + 1).TrimLeft();
			if(CheckType(jCode.Code))
				m_pHCode.Add(jCode);
		}
	}

	for (int ii = 0; ii < m_pHCode.GetSize(); ii++)
	{
		jCode = m_pHCode.GetAt(ii);
		//Str.Format("%s  %s", jCode.Code, jCode.Name);
		Str.Format("%s  %s",  jCode.Name, jCode.Code);
		AddString(Str);
	}

	m_DataMode = DM_HISTORY;
	m_sSymbol = sSymbol;
}

bool CCodeCombo::CheckType(CString strcode)
{
	CString tmpS, tempS;
	CStringArray arrtmp;
	arrtmp.Copy(m_pParent->m_arrtype);

	if(!m_mapCode.Lookup(strcode, tempS))
		return false;
	tmpS = m_pParent->Parser(tempS, _T("\n"));
	tempS += _T("\t");
	tempS += strcode;
	if(m_pParent->IsCheckType(tempS))
		return true;

	return false;
}

void CCodeCombo::AddString(LPCTSTR lpszString)
{
	m_arString.Add(lpszString);

	CComboBox::AddString(lpszString);
}

bool CCodeCombo::IsHistoryMode() 
{ 
	return (m_DataMode == DM_HISTORY)? true:false; 
}

void CCodeCombo::Key(int key)
{
	CString tmpS;
	int	curIndex = GetCurSel();

	if (curIndex < 0)	return;

	CRect	cRC;
	int	count = 9;

	if (m_pSubList)
	{
		m_pSubList->GetClientRect(&cRC);
		int	iHeight = GetItemHeight(curIndex);
		count = cRC.Height() / iHeight;
	}

	int	topIndex = GetTopIndex();
	int	endIndex = topIndex+count-1;
	int	index = curIndex;
	bool	bSetTopIndex = false;


tmpS.Format(_T("[k=%d] [t=%d] [c=%d] \n"), key, topIndex, curIndex);
//TRACE(tmpS);


	switch (key)
	{
	case VK_UP:	// up arrow
		index = max(0, index-1);

		if (index < topIndex)	
			topIndex = index;

		break;
	case VK_DOWN:	// down arrow
		index = min(GetCount()-1, index+1);

		if (index > endIndex)	
			topIndex += 1;

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

tmpS.Format(_T("-[k=%d] [t=%d] [c=%d] [%s]\n"), key, topIndex, curIndex, bSetTopIndex == true ? "true" : "false");
//TRACE(tmpS);

	if (index >= 0 && index < GetCount())
	{
	//	if (bSetTopIndex)			
			SetTopIndex(topIndex);
			SetCurSel(index);
	}
	
}

void CCodeCombo::InitCodeList()
{
	ResetContent();
	ResetString();

	_JCode	jCode;
	CString	Str;

	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);
		if (!jCode.Code.GetLength())
			continue;

		Str.Format("%s  %s", jCode.Name, jCode.Code);
		AddString(Str);
	}
	m_DataMode = DM_CODELIST;

}

bool CCodeCombo::IsValidCode(CString sCode)
{
	if (sCode.IsEmpty())
		return false;

	_JCode	jCode;

	for (int ii = 0; ii < m_pJCode.GetSize(); ii++)
	{
		jCode = m_pJCode.GetAt(ii);

		if ( jCode.Code == sCode)
			return true;
	}

	return false;
}
// CCodeCombo 메시지 처리기입니다.
CString CCodeCombo::GetSelectCode()
{
	int	index = GetCurSel();
	if (index < 0)
		return "";

	if (IsCodeListMode())
	{
		_JCode	jCode = m_pJCode.GetAt(index);
	//	if (m_Unit == GU_FOREIGN)
	//		m_sCode = jCode.Code;
	//	else
			m_sCode = jCode.Code.Mid(1);
	}
	else if (IsHistoryMode())
	{
		_JCode	jCode = m_pHCode.GetAt(index);
		m_sCode = jCode.Code.Mid(1);
	}

	return m_sCode;
}

void CCodeCombo::SearchNumberCode(CString number)
{
	if (number.IsEmpty())
	{
		SetTopIndex(0);
		SetCurSel(0);
		return;
	}

	CString Str;
	_JCode	jCode;
	
	if (IsCodeListMode())
	{
		ResetContent();
		ResetString();
		int icnt = 0 ;

		for (int ii = m_ifind ; ii < m_pJCode.GetCount(); ii++)
		{
			jCode = m_pJCode.GetAt(ii);

			if (jCode.Name.Find(number) >= 0 )
			{
				icnt++;
				Str.Format("%s  %s", jCode.Name.TrimRight(), jCode.Code.TrimRight());
				AddString(Str);
				if(icnt >= 20)
				{
					m_ifind = ii;
					return;
				}
			}
		}
	}
}

void CCodeCombo::SearchCode(CString code, bool benter)
{
	CString log;
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
		if(benter)
		{
			int	index = GetCurSel();
			_JCode	jCode;
			if (index >= 0)
			{	//SearchNumberCode
				if (IsCodeListMode())
				{
					if(m_Unit == 1)
					{

					}
					else if(m_Unit == 0)
					{
					//	jCode = m_pJCode.GetAt(index);	
						jCode = m_pSearchCode.GetAt(index);	
						
						CString str;
						str.Format(_T("%d %s"), index, jCode.Name);
						m_pParent->m_pEdit->ListCode(jCode.Name);
					}
				}
				else if (IsHistoryMode())
				{
					jCode = m_pHCode.GetAt(index);
					m_pParent->m_pEdit->ListCode(jCode.Name);
					m_pParent->SetEditData(jCode.Name);
				}

				ShowDropDown(FALSE);
				m_pParent->SendCtrlMsg(2);
				SendMessage(WM_KILLFOCUS, 0, 0);
				m_pParent->Trigger(jCode.Code);
				m_pParent->m_pEdit->SetSel(jCode.Name.GetLength(), jCode.Name.GetLength());
				Invalidate();
			}	
			return;
		}

		int	size = m_pJCode.GetSize();
		int icnt = 0;
		CString Str;
		ResetContent();
		ResetString();
		m_pSearchCode.RemoveAll();
		for (int ii = 0; ii < size; ii++)
		{
			jCode = m_pJCode.GetAt(ii);
			if (jCode.Name.Find(code) >= 0)
			{
				Str.Format("%s  %s", jCode.Name, jCode.Code);
				AddString(Str);
				m_pSearchCode.Add(jCode);
				icnt++;
			}

			if(icnt >= 20)
			{
				SetCurSel(0);
				return;	
			}
		}

		SetItemHeight(-1, m_CellHeight - 5);
		ShowDropDown(TRUE);
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

void CCodeCombo::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->CtlType != ODT_COMBOBOX)
		return;

	CDC dc;
	if (!dc.Attach(lpDrawItemStruct->hDC))
		return;

	CFont	*sFont = NULL;
	if (NULL != m_pFont)
		sFont = dc.SelectObject(m_pFont);
	COLORREF sTextColor = dc.GetTextColor();
	COLORREF sBkColor = dc.GetBkColor();
////////////////////////////////////////////
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) && (lpDrawItemStruct->itemState  & ODS_SELECTED))
	{
		dc.SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		dc.SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, ::GetSysColor(COLOR_HIGHLIGHT));
	}
	else
	{
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, sBkColor);
	}
	
	// Draw the text.
	if (lpDrawItemStruct->itemID != -1)
	{
		CString	text;
		//GetLBText(lpDrawItemStruct->itemID, text);
		text = GetText(lpDrawItemStruct->itemID);
		dc.DrawText(text, &lpDrawItemStruct->rcItem, DT_LEFT|DT_SINGLELINE|DT_VCENTER);
	}	

/////////////////////////////////////////////
	dc.SetTextColor(sTextColor);
	dc.SetBkColor(sBkColor);
	if (sFont)
		dc.SelectObject(sFont);

	dc.Detach();
}

CString CCodeCombo::GetText(int index)
{
	if (index >= m_arString.GetSize())
		return _T("");

	return m_arString.GetAt(index);
}

void CCodeCombo::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if (lpMeasureItemStruct->CtlType == ODT_COMBOBOX)
		lpMeasureItemStruct->itemHeight = m_CellHeight;
	// TODO:  지정된 항목의 크기를 확인하는 코드를 추가합니다.
}

void CCodeCombo::RemoveString(int index)
{
	if (IsHistoryMode())
	{
		if (0 <= index && index < m_arString.GetSize())
		{
			m_arString.RemoveAt(index);
			_JCode	jcode = m_pHCode.GetAt(index);
			m_pHCode.RemoveAt(index);
			if (m_pWizard)
			{
			//	if (m_sSymbol == FOREIGN_SYM)
			//		m_pTemp.Format("%s\t%s%s", m_sSymbol, FOREIGN_MARK, jcode.Code);
			//	else
				//m_pTemp.Format("%s\t%s", m_sSymbol, jcode.Code);
				m_pTemp = (char*)m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, historyCC), (long)m_sSymbol.operator LPCTSTR());
				//m_pWizard->SendMessage(WM_USER, MAKEWPARAM(variantDLL, codeCCx), (LPARAM)(LPCTSTR)m_pTemp);
				//RemoveHistory(m_sSymbol, m_pTemp, jcode.Code);
			}
		}
	}
}

bool CCodeCombo::CodeLoad(CString tabPath)
{
	m_pParent->LoadAuthAg();

	CString path, tmps;
	int codeN;
	struct agcode code;
	AGCODE* acode = NULL;
	CFile file;
	
	path = tabPath + "\\tab\\" + _T("issorgcd.dat");   
	if (!file.Open(path, CFile::modeRead|CFile::typeBinary))
		return false;

	m_pJCode.RemoveAll();
	m_mapCode.RemoveAll();
	int ilen = (int)file.GetLength();
	codeN = (int)file.GetLength() / sizeof(struct agcode);
	char* pdata = NULL;
	CString strtemp, tmpS;
	struct	_JCode	jCode;

	POSITION pos, mappos;
	int cnt = 1;
	CString strkey, key;
	for (pos = m_pParent->m_mapAg.GetStartPosition(); pos; )
	{
		m_pParent->m_mapAg.GetNextAssoc(pos, strkey, key);
		strtemp += strkey;
		strtemp += _T("  ");
	}

	tmps.Format(_T("--------- 룩업세팅 m_mapAg=%d[%s], m_arrtype=%d개,%s m_mapCodeAg=%d개 onlytype=%d\n"), m_pParent->m_mapAg.GetSize(),strtemp, m_pParent->m_arrtype.GetSize(), \
		m_pParent->m_arrtype.GetSize() > 0 ? m_pParent->m_arrtype.GetAt(0):"없음",  m_pParent->m_mapCodeAg.GetSize(), m_pParent->m_bonlyBal);
	OutputDebugString(tmps);

	for (int ii = 0; ii < codeN; ii++)
	{
		acode = new AGCODE;
		memset(acode, 0, L_agcode);
		
		file.Read(&code, sizeof(struct agcode));
	
		jCode.Code = CString(code.code, 6);
		jCode.Code.TrimRight();

		jCode.Name = CString(code.name, 50);
		jCode.Name.TrimRight();
		jCode.Name.TrimLeft();

		jCode.up = CString(code.up, 2);
		jCode.up.TrimRight();
		jCode.up.TrimLeft();

		jCode.bBal = code.gubn;
		jCode.Balcode = CString(code.balcode, 14);
		jCode.Balcode.TrimRight();
		jCode.Balcode.TrimLeft();
		
		if(m_pParent->m_mapAg.GetCount() > 0)
		{
			if(!m_pParent->m_mapAg.Lookup(jCode.up, tmpS))
				continue;
		}

		m_mapCode.SetAt(jCode.Code, jCode.Name + _T("\n") + jCode.up +  _T("\t") + jCode.bBal);

		if(m_pParent->IsCheckType(jCode.up + _T("\t") + jCode.bBal + _T("\t") + jCode.Code ))
			m_pJCode.Add(jCode);
	}

	file.Close();

//	m_pJCode.QuickSort();

	switch (m_Unit)	// ljh
	{
	case 0:
///	case GU_ELWCODE:
//	case GU_FOREIGN:
		m_pParent->CodeListMode();
		break;
	}
	return true;
}



int CCodeCombo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	SetHorizontalExtent(140);
	SetDroppedWidth(140);

	m_pSubList = new CSubList(m_pParent, this);
	return 0;
}


HBRUSH CCodeCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  여기서 DC의 특성을 변경합니다.
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
	// TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
	return hbr;
}


void CCodeCombo::OnDestroy()
{
	//if (m_pSubList->m_hWnd)
	//	m_pSubList->UnsubclassWindow();
	//delete m_pSubList;
	m_pJCode.RemoveAll();
	m_pHCode.RemoveAll();
	
	CComboBox::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CCodeCombo::OnCbnDropdown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


CString CCodeCombo::GetCode(CString strname)
{
	_JCode	jCode;
	for(int ii = 0 ;  ii < m_pJCode.GetCount() ; ii++)
	{
		jCode = m_pJCode.GetAt(ii);
		if(jCode.Name == strname)
			return jCode.Code;
	}
		
	return _T("");
}

void CCodeCombo::OnSetFocus(CWnd* pOldWnd)
{
	CComboBox::OnSetFocus(pOldWnd);

	if (pOldWnd)
		pOldWnd->SendMessage(WM_KILLFOCUS, 0, 0);
}


void CCodeCombo::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CComboBox::OnTimer(nIDEvent);
}
