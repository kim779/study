#if !defined(AFX_ACCCOMBO_H__BB928142_D1CE_40EB_977A_B5567E2FCDD4__INCLUDED_)
#define AFX_ACCCOMBO_H__BB928142_D1CE_40EB_977A_B5567E2FCDD4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AccCombo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAccCombo window
#include "CheckComboBox.h"

class CAccCombo : public CComboBox
{
// Construction
public:
	CAccCombo();

// Attributes
public:
	inline BOOL GetGroup() { return m_bGroup; };
	inline BOOL GetGroupList() { return m_bGroupList; };
	inline int GetGroupKey() { return m_nGroupKey; };
	inline void SetGroup(BOOL bGroup) { m_bGroup = bGroup; };
	inline void SetGroupList(BOOL bGroupList) { m_bGroupList = bGroupList; };
	inline void SetGroupKey(int nGroupKey) { m_nGroupKey = nGroupKey; };

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAccCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAccCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CAccCombo)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bGroup;
	BOOL m_bGroupList;
	int	m_nGroupKey;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ACCCOMBO_H__BB928142_D1CE_40EB_977A_B5567E2FCDD4__INCLUDED_)

