// GDIpPrinterView.h : interface of the CGDIpPrinterView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIPPRINTERVIEW_H__C01867AF_0282_42E8_94BD_3A0D73484EA6__INCLUDED_)
#define AFX_GDIPPRINTERVIEW_H__C01867AF_0282_42E8_94BD_3A0D73484EA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGDIpPrinterView : public CView
{
protected: // create from serialization only
	CGDIpPrinterView();
	DECLARE_DYNCREATE(CGDIpPrinterView)

// Attributes
public:
	CGDIpPrinterDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGDIpPrinterView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	static float CreateFontSize(HDC hDC, int points, double dpiRatio);
	void DrawStuff(HDC hDC, CRect rect, Graphics& graphics, double dpiRatio);
	virtual ~CGDIpPrinterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGDIpPrinterView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in GDIpPrinterView.cpp
inline CGDIpPrinterDoc* CGDIpPrinterView::GetDocument()
   { return (CGDIpPrinterDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GDIPPRINTERVIEW_H__C01867AF_0282_42E8_94BD_3A0D73484EA6__INCLUDED_)
