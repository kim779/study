// NumericalInquiryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ChartAnalysisToolAddIn.h"
#include "NumericalInquiryDlg.h"
#include "../Include_Chart/Dll_Load/IMetaTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryDlg dialog


CNumericalInquiryDlg::CNumericalInquiryDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
	m_dlgBrush.CreateSolidBrush(RGB(230, 230, 230));
	m_pFont = NULL;

	m_hOcxWnd = ( pParent ? pParent->GetSafeHwnd() : NULL);
}

CNumericalInquiryDlg::~CNumericalInquiryDlg()
{
	ML_SET_LANGUAGE_RES();
	if( IsWindow( GetSafeHwnd())) DestroyWindow();

	m_dlgBrush.DeleteObject();
}

// (2006/2/15 - Seung-Won, Bae) for checking Runtime Class ~ Not used yet in 2006/2/15
IMPLEMENT_DYNAMIC( CNumericalInquiryDlg, CDialog)

/////////////////////////////////////////////////////////////////////////////
// CNumericalInquiryDlg message handlers

/*-------------------------------------------------------------------------------
 - Function    :  SetDlgBrush
 - Created at  :  2005-03-16   10:25
 - Author      :  최종찬
 - Description :  수치조회창 색상을 지정한다.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::SetDlgBrush(COLORREF clrDlgBrush)
{
	if(m_dlgBrush.GetSafeHandle())
		m_dlgBrush.DeleteObject();
	m_dlgBrush.CreateSolidBrush(clrDlgBrush);
}


/*-------------------------------------------------------------------------------
 - Function    :  SetResourceFont
 - Created at  :  2005-03-23   09:34
 - Author      :  최종찬
 - Description :  Resource Font를 세팅한다.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::SetResourceFont(CFont* pFont)
{
	m_pFont = pFont;
}



/*-------------------------------------------------------------------------------
 - Function    :  GetMaxColumnString
 - Created at  :  2005-03-23   10:36
 - Author      :  최종찬
 - Parameters  :  pData     - list.
 -                nColIndex - 컬럼 인덱스.
 - Description :  Column의 구분값중에 가장 긴 문자열을 가져온다. (왜냐면 칼럼의 넓이를 구하기 위해)
 -------------------------------------------------------------------------------*/
LPCTSTR CNumericalInquiryDlg::GetMaxColumnString( CInquiryItem *pData, int nColIndex)
{
	CString strMaxColumnString = _T("");

	CString strFirst;
	CRowData *pRowData = NULL;
	POSITION posRow = pData->GetHeadPosition();
	while( posRow)
	{
		pRowData = pData->GetNext( posRow);

		if(nColIndex == 0)
			strFirst = pRowData->m_strName;
		else if(nColIndex == 1)
			strFirst = pRowData->m_strData;
		
		if(strMaxColumnString.GetLength() < strFirst.GetLength())
			strMaxColumnString = strFirst;
	}
	
	return (LPCTSTR)strMaxColumnString;
}


/*-------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-03-28   13:05
 - Author      :  최종찬
 - Description :  수치조회창을 이동시킨다.
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::MoveWindow(LPCRECT lpRect, BOOL bRepaint /* = TRUE*/, CDC* pDC /* = NULL*/)
{
	CDialog::MoveWindow(lpRect, bRepaint);
}


/*-----------------------------------------------------------------------------------------
 - Function    :  MoveWindow
 - Created at  :  2005-04-06   09:59
 - Author      :  최종찬
 - Description :  수치조회창을 이동시킨다.
 -----------------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::MoveWindow(int x, int y, int nWidth, int nHeight, BOOL bRepaint /* = TRUE */)
{
	CDialog::MoveWindow(x, y, nWidth, nHeight, bRepaint);
}


/*-------------------------------------------------------------------------------
 - Function    :  OnMouseMove
 - Created at  :  2005-04-14   10:50
 - Author      :  최종찬
 - Description :  Window와 Simple Window타입(파생클래스에서 가상상속했다.)에서 외엔
				  그닥 필요하지 않는...
 -------------------------------------------------------------------------------*/
void CNumericalInquiryDlg::OnMouseMove(CPoint point)
{
}

/*-------------------------------------------------------------------------------
 - Function    :  ReFormData
 - Created at  :  2005-04-14   10:39
 - Author      :  최종찬
 - Parameters  :  strData - 바뀌기전 데이타.
 - Return Value:  CString - 소수점 자리를 맞춘 데이타.
 - Description :  소수점자릿수를 맞춘다.
 -------------------------------------------------------------------------------*/
CString CNumericalInquiryDlg::ReFormData(const CString& strData, const int nMaxDecimal)
{
	if(nMaxDecimal == 0)
		return strData;
	
	CString strReFormData = strData;

	int nFindIndex = strReFormData.Find(_T("."));
	
	if(nFindIndex == -1)	// 1. 소수점없는 데이타!
	{
		for(int i = 0; i < (nMaxDecimal + 2); i++)
			strReFormData += _T(" ");	// 소수점+소수점이하자릿수만큼 공백을 추가.
	}
	else					// 2. 소수점있는 데이타.
	{
		int nDecimal = strReFormData.GetLength() - nFindIndex;
		// 최대소수점자릿수가 더 많을 경우 많은 만큼 공백 추가.
		if(nDecimal < nMaxDecimal)
		{
			for(int i = 0; i < (nMaxDecimal - nDecimal); i++)
				strReFormData += _T(" ");
		}
	}

	return strReFormData;
}

CString CNumericalInquiryDlg::ReFormData(const CString& strData,double dRatio, const int nMaxDecimal)
{ 
	CString strRatio;
	strRatio.Format("(%.2f%%)",dRatio);

	CString strReFormData = strData + strRatio;
	if(nMaxDecimal == 0)
	{		
		return strReFormData;
	}
		
	int nFindIndex = strData.Find(_T("."));	
	if(nFindIndex == -1)	// 1. 소수점없는 데이타!
	{
		for(int i = 0; i < (nMaxDecimal + 2); i++)
			strReFormData += _T(" ");	// 소수점+소수점이하자릿수만큼 공백을 추가.
	}
	else					// 2. 소수점있는 데이타.
	{
		int nDecimal = strReFormData.GetLength() - nFindIndex;
		// 최대소수점자릿수가 더 많을 경우 많은 만큼 공백 추가.
		if(nDecimal < nMaxDecimal)
		{
			for(int i = 0; i < (nMaxDecimal - nDecimal); i++)
				strReFormData += _T(" ");
		}
	}

	return strReFormData;
}
