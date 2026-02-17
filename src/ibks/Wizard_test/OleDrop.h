// OleDrop.h: interface for the COleDrop class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OLEDROP_H__26CB78DF_725C_4B3D_BA98_8EACC3A8EC8D__INCLUDED_)
#define AFX_OLEDROP_H__26CB78DF_725C_4B3D_BA98_8EACC3A8EC8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COleDrop : public COleDropTarget  
{
public:
	COleDrop();
	virtual ~COleDrop();

// Attributes
public:

private:
	class CWorks*	m_works;

// Operations
public:
	BOOL	Register(class CWorks* works);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COleDrop)
	public:
	virtual DROPEFFECT OnDragEnter(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(CWnd* pWnd, COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(CWnd* pWnd, COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COleDrop)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_OLEDROP_H__26CB78DF_725C_4B3D_BA98_8EACC3A8EC8D__INCLUDED_)
