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

// CPropGrid

class CPropGrid : public CMFCPropertyGridCtrl
{
	DECLARE_DYNAMIC(CPropGrid)

public:
	CPropGrid();
	virtual ~CPropGrid();

	void	Initialize(CString data);
	void	SetControl(CString ctrl, int fN);
	void	SetPropInfo(int kind, CString info);
	void	SetProperties(int kind);
	CString	GetData();
	CString	GetValues();
	CString	ConvertData(CString data);
	void	Clear();

protected:
	CString	m_control;
	int	m_fN;
	CString	m_param;
	CStringArray	m_datas;
	CMap	< int, int, CPtrArray*, CPtrArray* > m_propMap;

	CString	getValue(Cproperty* prop, int index);
	void	setValue(Cproperty* prop, int index, CString data);
	CString	parse(CString& src, CString sub);

	int	fillCombo(CMFCPropertyGridProperty* combo, Cproperty* prop, CString data);
	CString	getReservedVal(CString str);
	CString	getComboData(CString items, CString data);

	DECLARE_MESSAGE_MAP()

public:
	virtual void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const;
	afx_msg	LRESULT OnMessage(WPARAM wp, LPARAM lp);
};


