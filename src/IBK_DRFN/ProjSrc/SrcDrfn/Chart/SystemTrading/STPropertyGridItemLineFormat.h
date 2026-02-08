// STPropertyGridItemLineFormat.h: interface for the CSTPropertyGridItemLineFormat class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STPROPERTYGRIDITEMLINEFORMAT_H__8AEF2FAF_DE57_468E_848D_CECDBFBBA21A__INCLUDED_)
#define AFX_STPROPERTYGRIDITEMLINEFORMAT_H__8AEF2FAF_DE57_468E_848D_CECDBFBBA21A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSTPropertyGridItemLineFormat : public CXTPPropertyGridItem  
{
public:
	CSTPropertyGridItemLineFormat(CString strCaption, int nIndex);
	virtual ~CSTPropertyGridItemLineFormat();

protected:
	virtual BOOL OnDrawItemValue(CDC& dc, CRect rcValue);

	int		GetPenStyle();
	void	SetPenStyle(int nPenStyle);
	int		m_nIndex;

};

#endif // !defined(AFX_STPROPERTYGRIDITEMLINEFORMAT_H__8AEF2FAF_DE57_468E_848D_CECDBFBBA21A__INCLUDED_)
