// STPropertyGridItemColor.h: interface for the CSTPropertyGridItemColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STPROPERTYGRIDITEMCOLOR_H__69BF7691_C41D_4BF2_9E81_6BBC9F447F8E__INCLUDED_)
#define AFX_STPROPERTYGRIDITEMCOLOR_H__69BF7691_C41D_4BF2_9E81_6BBC9F447F8E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSTPropertyGridItemColor : public CXTPPropertyGridItemColor  
{
public:
	CSTPropertyGridItemColor(CString strCaption, COLORREF clr = 0, COLORREF* pBindColor = NULL);
	virtual ~CSTPropertyGridItemColor();

	virtual void SetValue(CString strValue);

};

#endif // !defined(AFX_STPROPERTYGRIDITEMCOLOR_H__69BF7691_C41D_4BF2_9E81_6BBC9F447F8E__INCLUDED_)
