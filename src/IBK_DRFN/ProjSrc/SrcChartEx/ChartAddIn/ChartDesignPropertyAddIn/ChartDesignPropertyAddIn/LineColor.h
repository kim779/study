// LineColor.h: interface for the CLineColor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINECOLOR_H__D950E9BF_EF77_4906_888E_085400238B78__INCLUDED_)
#define AFX_LINECOLOR_H__D950E9BF_EF77_4906_888E_085400238B78__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

class CLineColor  
{
public:
	CString MakeFromSixLineColorStringToOneString(const int nGraphCount);
	void SetLineColor(const int nLineIndex, const CString& strColorValue);

	CLineColor();
	virtual ~CLineColor();

private:
	CList<CString, CString> m_pOriginalColorStringList;
	CList<CString, CString> m_pColorStringList;
};

class CGraphSetting2Data
{
public:
	CGraphSetting2Data(const bool bHighNow = false, const bool bLowHigh = false, 
		const bool bLowNow = false, const bool bPrevNow = false, const int nCurDataType = 0);

	bool IsHighNow() const;
	bool IsLowHigh() const;
	bool IsLowNow() const;
	bool IsPrevNow() const;
	int	GetCurDataType() const;

	void SetData(const bool bHighNow, const bool bLowHigh, const bool bLowNow, const bool bPrevNow, const int nCurDataType);
	void SetHighNow(const bool bHighNow);
	void SetLowHigh(const bool bLowHigh);
	void SetLowNow(const bool bLowNow);
	void SetPrevNow(const bool bPrevNow);
	void SetCurDataType(const int nCurDataType);

private:
	bool m_bHighNow;
	bool m_bLowHigh;
	bool m_bLowNow;
	bool m_bPrevNow;
	int m_nCurDataType;
};

#endif // !defined(AFX_LINECOLOR_H__D950E9BF_EF77_4906_888E_085400238B78__INCLUDED_)
