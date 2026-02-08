// axisDoc.h : interface of the CAxisDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_AXISDOC_H__90E432E6_0388_40C6_A061_7437F6544F62__INCLUDED_)
#define AFX_AXISDOC_H__90E432E6_0388_40C6_A061_7437F6544F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAxisDoc : public CDocument
{
protected: // create from serialization only
	CAxisDoc();
	DECLARE_DYNCREATE(CAxisDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAxisDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CAxisDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CAxisDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AXISDOC_H__90E432E6_0388_40C6_A061_7437F6544F62__INCLUDED_)
