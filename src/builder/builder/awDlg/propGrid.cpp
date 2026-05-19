// propGrid.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "propGrid.h"
#include "PropertyGridButtonProperty.h"


// CPropGrid

IMPLEMENT_DYNAMIC(CPropGrid, CMFCPropertyGridCtrl)

CPropGrid::CPropGrid()
{
	m_fN = -1;
	m_datas.RemoveAll();
	m_propMap.RemoveAll();
}

CPropGrid::~CPropGrid()
{
	int	key;
	CPtrArray* arr;

//	RemoveAll();
	for (POSITION pos = m_propMap.GetStartPosition(); pos; )
	{
		m_propMap.GetNextAssoc(pos, key, arr);
		for (int ii = 0; ii < arr->GetCount(); ii++)
			delete (Cproperty *)arr->GetAt(ii);
		arr->RemoveAll();
		delete arr;
	}
	m_propMap.RemoveAll();
	m_datas.RemoveAll();
}


BEGIN_MESSAGE_MAP(CPropGrid, CMFCPropertyGridCtrl)
	ON_MESSAGE(WM_USER, OnMessage)
END_MESSAGE_MAP()



// CPropGrid 메시지 처리기입니다.

void CPropGrid::OnPropertyChanged(CMFCPropertyGridProperty* pProp) const
{
	SendMessage(WM_USER, 100, (LPARAM)pProp);

	return CMFCPropertyGridCtrl::OnPropertyChanged(pProp);
}

LRESULT CPropGrid::OnMessage(WPARAM wp, LPARAM lp)
{
	CMFCPropertyGridProperty* pProp = (CMFCPropertyGridProperty*)lp;
	for (int ii = 0; ii < GetPropertyCount(); ii++)
	{
		if (pProp == GetProperty(ii))
		{
			CString	data;
			Cproperty* prop = (Cproperty *)pProp->GetData();
			_variant_t var = pProp->GetValue();
			if (var.vt == VT_BSTR)
			{
				data = var.bstrVal;
				setValue(prop, ii, data);
			}
			break;
		}
	}
	return 0;
}

void CPropGrid::Initialize(CString data)
{
	m_datas.RemoveAll();
	for ( ; !data.IsEmpty(); )
		m_datas.Add(parse(data, _T("\t")));
}

void CPropGrid::SetControl(CString ctrl, int fN)
{
	m_control = ctrl;
	m_fN = fN;
}

void CPropGrid::SetPropInfo(int kind, CString info)
{
	CString	tmps;
	Cproperty*	prop;
	CPtrArray*	arr;

	if (!m_propMap.Lookup(kind, arr))
		arr = new CPtrArray();
	else
	{
		for (int ii = 0; ii < arr->GetSize(); ii++)
			delete (Cproperty *)arr->GetAt(ii);
	}
	arr->RemoveAll();

	for ( ; true; )
	{
		tmps = parse(info, _T("\t"));
		if (tmps.IsEmpty())
			break;

		prop = new Cproperty();
		prop->m_head = parse(tmps, _T("|"));
		prop->m_type = atoi(parse(tmps, _T("|")));
		prop->m_val1 = parse(tmps, _T("|"));
		prop->m_val2 = parse(tmps, _T("|"));
		prop->m_keys = parse(tmps, _T("|"));
		prop->m_desc = parse(tmps, _T("|"));

		arr->Add(prop);
	}
	m_propMap.SetAt(kind, arr);
}

void CPropGrid::SetProperties(int kind)
{
	CPtrArray*	arr;

	Clear();
	if (!m_propMap.Lookup(kind, arr))
		return;

	Cproperty*	prop;
	CString		tmps;

	if (m_datas.IsEmpty())
		m_datas.SetSize(arr->GetSize());
	for (int ii = 0; ii < arr->GetSize(); ii++)
	{
		prop = (Cproperty *)arr->GetAt(ii);

		switch (prop->m_type)
		{
		case 1:	// edit
			CMFCPropertyGridProperty*	pEdit;

			pEdit = new CMFCPropertyGridProperty(prop->m_head, (_variant_t)getValue(prop, ii), prop->m_desc, (DWORD_PTR)prop);
			if (!prop->m_val1.IsEmpty() && prop->m_val1.GetAt(0) == _T('1'))
				pEdit->AllowEdit(FALSE);
			else
				pEdit->AllowEdit(TRUE);
			AddProperty(pEdit);
			break;

		case 2:	// edit + button
			CPropertyGridButtonProperty*	pButton;

			pButton = new CPropertyGridButtonProperty(false, atoi(prop->m_val2), prop->m_head, (_variant_t)getValue(prop, ii), prop->m_desc, (DWORD_PTR)prop);
			pButton->SetInfo(this, m_fN, NULL, NULL);
			pButton->AllowEdit(TRUE);
			AddProperty(pButton);
			break;
		
		case 3:	// combo
			CMFCPropertyGridProperty*	pCombo;

			tmps = getValue(prop, ii);
			pCombo = new CMFCPropertyGridProperty(prop->m_head, (_variant_t)_T(""), prop->m_desc, (DWORD_PTR)prop);
			fillCombo(pCombo, prop, tmps);
			pCombo->AllowEdit(FALSE);
			AddProperty(pCombo);
			break;

		case 4:	// button
			CPropertyGridButtonProperty*	pfButton;

			pfButton = new CPropertyGridButtonProperty(true, atoi(prop->m_val2), prop->m_head, (_variant_t)getValue(prop, ii), prop->m_desc, (DWORD_PTR)prop);
			pfButton->SetInfo(this, m_fN, NULL, NULL);
			pfButton->AllowEdit(FALSE);
			AddProperty(pfButton);
			break;
		}
	}
}

CString CPropGrid::GetData()
{
	CMFCPropertyGridProperty* pProp;
	Cproperty*	prop;
	CString tmps, data = _T("");
	int	count = (int)m_datas.GetSize();

	for (int ii = 0; ii < GetPropertyCount(); ii++)
	{
		pProp = GetProperty(ii);
		prop = (Cproperty *)pProp->GetData();

		_variant_t var = pProp->GetValue();
		if (var.vt == VT_BSTR)
			tmps = var.bstrVal;
		else
			tmps = (ii < count) ? m_datas.GetAt(ii) : _T("");
		data += tmps;
		data += _T("\t");
	}
	return data;
}

CString CPropGrid::GetValues()
{
	CMFCPropertyGridProperty* pProp;
	Cproperty*	prop;
	CString tmps, data = _T("");
	int	count = (int)m_datas.GetSize();

	for (int ii = 0; ii < GetPropertyCount(); ii++)
	{
		pProp = GetProperty(ii);
		prop = (Cproperty *)pProp->GetData();

		data += _T("/");
		data += prop->m_keys;
		_variant_t var = pProp->GetValue();
		if (var.vt == VT_BSTR)
			tmps = var.bstrVal;
		else
			tmps = (ii < count) ? m_datas.GetAt(ii) : _T("");
	//	tmps.Replace(_T("/"), _T("*sl"));
		if (prop->m_type == 3)	// combo
			tmps = getComboData(prop->m_val1, tmps);
		data += tmps;
	}
	return data;
}

void CPropGrid::Clear()
{
	RemoveAll();
	RedrawWindow();
}

CString CPropGrid::ConvertData(CString data)
{
	Cproperty*	prop;
	CMFCPropertyGridProperty* pProp;
	int	pos;
	CString tmps, text = _T("");

	for (int ii = 0; ii < GetPropertyCount(); ii++)
	{
		pProp = GetProperty(ii);
		prop = (Cproperty *)pProp->GetData();

		tmps = _T("/") + prop->m_keys;
		pos = data.Find(tmps);
		if (pos == -1)
		{
			if (prop->m_type == 3 && !prop->m_val1.CompareNoCase(_T("$bool")))
				text += _T("False\t");
			else
				text += _T("\t");
		}
		else
		{
			CString	str;

			if (prop->m_type == 3)
			{
				if (!prop->m_val1.CompareNoCase(_T("$bool")))
					str = _T("True");
				else
				{
					CString combos = prop->m_val1;
					str = data.Mid(pos + tmps.GetLength());
					str = parse(str, _T("/"));
					if (combos.GetLength() > 0 && combos.GetAt(0) == _T('$'))
						combos = getReservedVal(combos);
					str += _T('-');
					pos = combos.Find(str);
					if (pos >= 0)
					{
						pos += str.GetLength();
						str = combos.Mid(pos, combos.Find(_T(";"), pos) - pos);
					}
				}
				text += str;
				text += _T("\t");
			}
			else
			{
				str = data.Mid(pos + tmps.GetLength());
				text += parse(str, _T("/"));
				text += _T("\t");
			}
		}
	}
	return text;
}

CString CPropGrid::getValue(Cproperty* prop, int index)
{
	if (index >= 0 && index < (int)m_datas.GetSize())
	{
		CString	tmps = m_datas.GetAt(index);
		if (tmps.IsEmpty())
			tmps = prop->m_val2;
		return tmps;
	}
	return _T("");
}

void CPropGrid::setValue(Cproperty* prop, int index, CString data)
{
	if (index < 0 || index >= (int)m_datas.GetSize())
		return;

	m_datas.SetAt(index, data);
}

CString CPropGrid::parse(CString& src, CString sub)
{
	CString	tmps;
	int	pos = src.Find(sub);
	
	if (pos == -1)
	{
		tmps = src;
		src  = _T("");
		return tmps;
	}

	tmps = src.Left(pos);
	src  = src.Mid(pos + sub.GetLength());
	return tmps;
}

int CPropGrid::fillCombo(CMFCPropertyGridProperty* combo, Cproperty* prop, CString data)
{
	if (prop->m_val1.GetLength() <= 0)
		return 0;

	CString str = prop->m_val1;
	int	pos;
	if (str.GetAt(0) == _T('$'))	// reserved items
	{
		CString field = "00$bool\t01$fontStyle\t02$alignment\t03$typeHint\t";

		pos = field.Find(str);
		if (pos == -1)
			return 0;

		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:		// $bool
			str = _T("0-False;1-True;");
			break;
		case 1:		// $fontStyle
			str = _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
			break;
		case 2:		// $alignment
			str = _T("0-Center;1-Left;2-Right;");
			break;
		case 3:		// $typeHint
			str = _T("0-None;1-String;2-Reference;");
			break;
		}
	}

	CString	item, tmps = str;
	for ( ; !tmps.IsEmpty(); )
	{
		item = parse(tmps, _T(";"));
		parse(item, _T("-"));
		combo->AddOption(item);
	}

	combo->SetValue((_variant_t)data);
	return combo->GetOptionCount();
}

CString CPropGrid::getReservedVal(CString str)
{
	CString field = _T("00$bool\t01$fontStyle\t02$alignment\t");
	int	pos = field.Find(str);

	if (pos != -1)
	{
		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:	//	$bool
			return _T("0-False;1-True;");
		case 1:	//	$fontStyle
			return _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
		case 2:	//	$alignment
			return _T("0-Center;1-Left;2-Right;");
		}
	}
	return _T("");
}

CString CPropGrid::getComboData(CString items, CString data)
{
	if (data.GetLength() <= 0)
		return _T("");

	int	pos;
	if (items.GetAt(0) == _T('$'))	// reserved items
	{
		CString field = "00$bool\t01$fontStyle\t02$alignment\t03$typeHint\t";

		pos = field.Find(items);
		if (pos == -1)
			return 0;

		switch (atoi(field.Mid(pos - 2, 2)))
		{
		case 0:		// $bool
			items = _T("0-False;1-True;");
			break;
		case 1:		// $fontStyle
			items = _T("0-None;1-Normal;2-Italic;3-Bold;4-Bold/Italic;");
			break;
		case 2:		// $alignment
			items = _T("0-Center;1-Left;2-Right;");
			break;
		case 3:		// $typeHint
			items = _T("0-None;1-String;2-Reference;");
			break;
		}
	}

	items.Insert(0, _T(';'));
	data += _T(';');
	pos = items.Find(data);
	if (pos != -1)
	{
		data = items.Left(pos);
		pos = data.ReverseFind(_T(';'));
		if (pos != -1)
			data = data.Mid(pos+1);
		pos = data.Find(_T('-'));
		if (pos != -1)
		{
			data = data.Left(pos);
			data.Trim();
			return data;
		}
	}
	return _T("");
}
