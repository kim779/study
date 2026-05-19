#pragma once

// Class  : CPropertyGridButtonProperty
// Desc   : PropertyGrid에서 Button을 표현하는 Class

class AFX_EXT_CLASS CPropertyGridButtonProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CPropertyGridButtonProperty)

public:
	CPropertyGridButtonProperty(bool full, int type, const CString& strName, const _variant_t& varValue, LPCTSTR lpszDescr = NULL, DWORD_PTR dwData = 0);
	virtual ~CPropertyGridButtonProperty();

protected:
	CWnd*	m_parent;
	bool	m_full;		// full-rect button
	int	m_type;

	int	m_fN;
	class mapForm*	m_mapH;
	CString	m_infos;

// Overrides
public:
	void	SetInfo(CWnd* parent, int fN, class mapForm* mapForm, CString info = _T(""));
	virtual	void AdjustButtonRect();
	virtual void OnClickButton(CPoint point);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
};


