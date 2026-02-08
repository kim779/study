#if !defined(AFX_NUMERICALINQUIRYDLG_H__D4BAD048_DE72_435E_B836_CF3F040452AE__INCLUDED_)
#define AFX_NUMERICALINQUIRYDLG_H__D4BAD048_DE72_435E_B836_CF3F040452AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NumericalInquiryDlg.h : header file
// 수치조회창 Base Dialog.

#include <afxtempl.h>									// for CTypedPtrList
#include "NumericalInquiryDataQueryer.h"				// for CInquiryItem

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryDlg dialog
class CNumericalInquiryDlg : public CDialog
{
	// (2006/2/15 - Seung-Won, Bae) for checking Runtime Class ~ Not used yet in 2006/2/15
	DECLARE_DYNAMIC( CNumericalInquiryDlg)

// Construction
public:
	CNumericalInquiryDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CNumericalInquiryDlg();

// Function
public:
	virtual void SetDlgBrush( COLORREF clrDlgBrush);
	void SetResourceFont( CFont* pFont);
	virtual void MoveWindow(LPCRECT lpRect, BOOL bRepaint = TRUE, CDC* pDC = NULL);
	virtual void MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint = TRUE);
	virtual void OnMouseMove(CPoint point);

/*
	virtual void SetInquiryData(CDC* pDC, CInquiryData *pInquiryData, int nMaxDecimal = 0) = 0;



*/

protected:
	LPCTSTR GetMaxColumnString( CInquiryItem *pData, int nColIndex);
	CString ReFormData(const CString& strData, const int nMaxDecimal);		// 데이타를 소숫점자리에 맞추어 변환한다. 
	CString ReFormData(const CString& strData,double dRatio, const int nMaxDecimal);		// 데이타를 소숫점자리에 맞추어 변환한다. 

// Field
protected:
	CBrush m_dlgBrush;
	CFont* m_pFont;
	HWND	m_hOcxWnd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NUMERICALINQUIRYDLG_H__D4BAD048_DE72_435E_B836_CF3F040452AE__INCLUDED_)
