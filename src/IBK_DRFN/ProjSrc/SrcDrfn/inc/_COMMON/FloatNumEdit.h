#if !defined(AFX_FLOATNUMEDIT_H__40B926E2_898B_41F5_B44D_6F020D0DAAB7__INCLUDED_)
#define AFX_FLOATNUMEDIT_H__40B926E2_898B_41F5_B44D_6F020D0DAAB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FloatNumEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFloatNumEdit window

class CFloatNumEdit : public CEdit
{
protected:
	CString		m_strPrvText;
	double		m_dMin;
	double		m_dMax;
	BOOL		m_bFloat;

public:

public:

public:

	//{{AFX_VIRTUAL(CFloatNumEdit)
	//}}AFX_VIRTUAL

public:
	CFloatNumEdit();
	virtual ~CFloatNumEdit();

	void		CheckNumeric		( CString & strNum );
	BOOL		EnableFloat			( BOOL bFloat );
	void		SetMaxMinVal		( double dMin, double dMax );

protected:
	//{{AFX_MSG(CFloatNumEdit)
	afx_msg void OnChange();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FLOATNUMEDIT_H__40B926E2_898B_41F5_B44D_6F020D0DAAB7__INCLUDED_)
