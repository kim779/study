// PropertiesBar.h: interface for the CPropertiesBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPERTIESBAR_H__E34AC39A_9DCB_4A27_9CF8_1D673AB95501__INCLUDED_)
#define AFX_PROPERTIESBAR_H__E34AC39A_9DCB_4A27_9CF8_1D673AB95501__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "stdAfx.h"
#include "ListGrid.h"
#include "mapform.h"
#include "awCommon/FormItem.h"

class CPropertiesBar : public CSizingControlBarG  
{
public:
	CPropertiesBar();
	virtual ~CPropertiesBar();
public:
	CListGrid	m_listGrid;
protected:
	CComboBoxEx	m_ctrllistCBO;
	CStatic		m_descStatic;
	CFont		m_font;
	CFormItem	*m_pFormItem;
	struct	_mapH*		m_mapH;
	CMap	<int, int, CString, CString>	m_propInfoArr;

public:
	int AddCBOString(CString sStr, DWORD data);
	void EnableProperty(bool bEnable);
	int getPropertyInt(int idx, bool bGroupCheck);
	void	ReplaceComboString(CString sItem, int data);
	void	parseProperties(CString data);
	void	LoadPropertyFile();
	void	Refresh(int row);
	void	setGroup(CString sGroup);
	void	killFocus();
	void	SetKeyIn(char nChar);
	CString	getPropertyStr(int idx, bool bGroupCheck = true);
	CString	Parser(CString &srcstr, CString substr);
	void	changePro(int idx, CString info);
	void	Initialize(struct _mapH* mapH);
	void	OnSelEndOk();
	void	SelectControl(int selctl);
	void	ResetCombo(int type = 1);
	int	AddComboString(CString sItem, int data);

protected:
	CString	compositInArr(CStringArray &formArr);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertiesBar)
	//}}AFX_VIRTUAL

		// Generated message map functions
protected:
	//{{AFX_MSG(CPropertiesBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	long	OnMessage(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_PROPERTIESBAR_H__E34AC39A_9DCB_4A27_9CF8_1D673AB95501__INCLUDED_)
