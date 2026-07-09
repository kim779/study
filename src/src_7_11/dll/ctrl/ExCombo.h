#if !defined(AFX_EXCOMBO_H__7B3CEBA7_3A37_11D5_941A_00001CD7EE63__INCLUDED_)
#define AFX_EXCOMBO_H__7B3CEBA7_3A37_11D5_941A_00001CD7EE63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif

class _ComboData
{
public:
	_ComboData(CString str)
	{
		m_sData = str;
	}
public:
	CString m_sData;
};

////////////////////////////////////////////////////
class AFX_EXT_CLASS CExComboBox : public CComboBox
{
public:
	CExComboBox();
	virtual ~CExComboBox();
	//{{AFX_VIRTUAL(CExComboBox)
	//}}AFX_VIRTUAL

protected:
	//{{AFX_MSG(CExComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

public:
	void	SetItemDataEx(int index, CString str);
	void	GetItemDataEx(int index, CString& str);
private:
	CPtrArray	m_Ptr;
};

//{{AFX_INSERT_LOCATION}}
#endif // !defined(AFX_EXCOMBO_H__7B3CEBA7_3A37_11D5_941A_00001CD7EE63__INCLUDED_)
