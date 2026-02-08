// STPropertyGridItemLineWidth.h: interface for the CSTPropertyGridItemLineWidth class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STPROPERTYGRIDITEMLINEWIDTH_H__4CB70259_0CBD_430F_8E1A_4F0F3BD47B24__INCLUDED_)
#define AFX_STPROPERTYGRIDITEMLINEWIDTH_H__4CB70259_0CBD_430F_8E1A_4F0F3BD47B24__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSTPropertyGridItemLineWidth : public CXTPPropertyGridItem  
{
public:
	CSTPropertyGridItemLineWidth(CString strCaption, int nWidth);
	virtual ~CSTPropertyGridItemLineWidth();

protected:
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	int		m_nWidth;
};

#endif // !defined(AFX_STPROPERTYGRIDITEMLINEWIDTH_H__4CB70259_0CBD_430F_8E1A_4F0F3BD47B24__INCLUDED_)
