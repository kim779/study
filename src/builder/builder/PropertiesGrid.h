#pragma once

class Cproperty
{
public:
	CString	m_head;
	int	m_type;
	CString	m_val1;
	CString	m_val2;
	CString	m_keys;
	CString	m_desc;
};

// CPropertiesGrid

class CPropertiesGrid : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CPropertiesGrid)

public:
	CPropertiesGrid();
	virtual ~CPropertiesGrid();

	int	m_fN;		// selected form index

protected:
	int	m_kind;
	class	mapForm*	m_mapH;

	CMap	< int, int, CPtrArray*, CPtrArray* > m_propMap;
	CMap	< int, int, CPtrArray*, CPtrArray* > m_propCtmObj;
	CMapStringToPtr		m_fieldMap;

public:
	void	Initialize(class mapForm* mapForm);
	bool	SetIndex(int fN);
	void	SetPropInfo(int kind, CString info, bool bCtmObj = false);
	void	SetProperties(int kind, int fN = -1, int cellN = -1);
	void	Refresh(int index);

	void	Clear();

protected:
	CString	getValue(Cproperty* prop, int fN, int col = -1);
	void	setValue(Cproperty* prop, int fN, int col, CString data);

	int	getKey(CString keys);
	CString	getInitSymbol(int fN, CString syms);

	int	fillCombo(CMFCPropertyGridProperty* combo, CString str, CString data);
	CString	getComboData(CString items, CString data);
	void	SortIoOrder(class CformProp* Prop);

	DECLARE_MESSAGE_MAP()

private:
	void	checkGridMultiLine(int fN);
	int	hex2Int(CString value);
	CString	parse(CString& src, CString sub);

public:
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	afx_msg	LRESULT OnMessage(WPARAM wp, LPARAM lp);
};


