#include "stdafx.h"
#include "ExCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CExComboBox::CExComboBox()
{
	m_Ptr.RemoveAll();
}

CExComboBox::~CExComboBox()
{
	for (int ii = 0; ii < m_Ptr.GetSize(); ii++)
	{
		_ComboData* pData = (_ComboData *)m_Ptr.GetAt(ii);
		delete pData;
	}
	m_Ptr.RemoveAll();
}


BEGIN_MESSAGE_MAP(CExComboBox, CComboBox)
	//{{AFX_MSG_MAP(CExComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CExComboBox::SetItemDataEx(int index, CString str)
{
	_ComboData* pData = new _ComboData(str);
	m_Ptr.Add(pData);
	SetItemData(index, (DWORD)m_Ptr[m_Ptr.GetSize()-1]);
}

void CExComboBox::GetItemDataEx(int index, CString& str)
{
	const _ComboData* pData = (_ComboData *)GetItemData(index);
	str = pData->m_sData;
}