#pragma once

// Class  : CPropertyGridButtonProperty
// Desc   : PropertyGrid에서 Button을 표현하는 Class

class AFX_EXT_CLASS CPropertyGridButtonProperty : public CMFCPropertyGridProperty
{
	DECLARE_DYNAMIC(CPropertyGridButtonProperty)

public:
	CPropertyGridButtonProperty(bool full, int type, const CString& strName, const _variant_t& varValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);
	virtual ~CPropertyGridButtonProperty();

protected:
	CWnd*	m_parent;
	bool	m_full;		// full-rect button
	int	m_type;

	int	m_fN;
	struct	_mapH*	m_mapH;
	class	CFormItem* m_formItem;
	CString	m_infos;
	CFontDialog*	m_fontD;

// Overrides
public:
	void	SetInfo(CWnd* parent, int fN, struct _mapH* mapH, CFormItem* formItem, CString info = _T(""));
	virtual	void AdjustButtonRect();
	virtual void OnClickButton(CPoint point);
	virtual void OnDrawValue(CDC* pDC, CRect rect);
};


