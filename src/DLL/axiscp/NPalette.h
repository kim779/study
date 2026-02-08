// NPalette.h: interface for the CNPalette class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NPALETTE_H__BDEACFEC_FBC4_4EFD_BE46_76A84D6498DF__INCLUDED_)
#define AFX_NPALETTE_H__BDEACFEC_FBC4_4EFD_BE46_76A84D6498DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNPalette  
{
public:
	CNPalette(CString home);
	virtual ~CNPalette();
	
	COLORREF GetColor(int index);
	static	COLORREF GetIndexColor(int index);
	static	CString	 GetTheme();

protected:
	CString	m_home;
};

#endif // !defined(AFX_NPALETTE_H__BDEACFEC_FBC4_4EFD_BE46_76A84D6498DF__INCLUDED_)
