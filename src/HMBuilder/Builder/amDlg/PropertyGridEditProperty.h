#pragma once

// Class : CPropertyGridEditProperty
// Desc  : PropertyGrid에서 Edit을 표현하는 Class 

class AFX_EXT_CLASS CPropertyGridEditProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CPropertyGridEditProperty)

public:
	CPropertyGridEditProperty(int nLimit, DWORD dwStyle, const CString& strName, const _variant_t& varValue, LPCTSTR lpszDescr, DWORD dwData);
	virtual ~CPropertyGridEditProperty();

protected:
	int	m_limit;
	DWORD	m_style;

	bool	m_bColor;

public:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
	void SetColor(bool bflag){ m_bColor = bflag; }
};


