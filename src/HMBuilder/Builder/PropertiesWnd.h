
#pragma once

#include "PropertiesGrid.h"

class CPropertiesToolBar : public CMFCToolBar
{
public:
	virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
	{
		CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
	}

	virtual BOOL AllowShowOnList() const { return FALSE; }
};

class CPropertiesWnd : public CDockablePane
{
// 생성입니다.
public:
	CPropertiesWnd();
	virtual ~CPropertiesWnd();

// 특성입니다.
public:
	void SetVSDotNetLook(BOOL bSet)
	{
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont			m_font;
	CComboBoxEx		m_wndCtrlCombo;
	CPropertiesToolBar	m_wndToolBar;
	CPropertiesGrid		m_wndPropList;

	class	CFormItem*	m_formItem;
	struct	_mapH*		m_mapH;
	CMap	<int, int, CString, CString>	m_infoMap;

	bool			m_bLayout;


// 구현입니다.
public:
	void	Initialize(struct _mapH* mapH, CFormItem* formItem);
	void	EnableProperty(BOOL bEnable);
	int	AddControl(CString sStr, DWORD data);
	void	ReplaceControl(CString sItem, int data);
	void	SelectControl(int selctl, bool bLayout);
	void	ResetControl(int type = 1);

	void	SetGroup(CString sGroup);
	void	Refresh(int row);

protected:
	void	adjustLayout();

	void	initPropList();
	void	loadPropertyFile();
	void	parseProperties(CString data);
	void	setPropListFont();
	int	getPropertyInt(int idx, bool bGroupCheck);
	CString	getPropertyStr(int idx, bool bGroupCheck = true);
	void	changeProperty(int idx, CString info);
	CString	compositeInArr(CStringArray &formArr);	// block 설정된 객체들의 property 합성
	CString	parse(CString &srcstr, CString substr);

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSortProperties();
	afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	afx_msg	void OnSelCombo();
	afx_msg	LRESULT OnMessage(WPARAM wp, LPARAM lp);

	DECLARE_MESSAGE_MAP()
};

