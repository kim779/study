#if !defined(AFX_IGFXOUTBARCTRL_H__DFETFFA4_11B7_11D2_8437_0000B43382FE__INCLUDED_)
#define AFX_IGFXOUTBARCTRL_H__DFETFFA4_11B7_11D2_8437_0000B43382FE__INCLUDED_

// #include "../Common_ST/IGfxOutBarCtrl.h"

enum {  fSmallIcon = 1 << 0, fLargeIcon = 1 << 1, fEditGroups = 1 << 2, fEditItems = 1 << 3,
fRemoveGroups = 1 << 4, fRemoveItems = 1 << 5, fAddGroups = 1 << 6, 
fDragItems = 1 << 7, fAnimation = 1 << 8, fSelHighlight = 1 << 9 };


interface IGfxOutBarCtrl
{
	STDMETHOD_(CWnd*, GetCoutBarCtrl)()  PURE;
	STDMETHOD_(BOOL, CreateOutBar)(DWORD dwStyle, const RECT & rect, CWnd * pParentWnd, UINT nID, const DWORD dwFlag) PURE;
	STDMETHOD_(void, Setcr3dFace)(COLORREF) PURE;
	STDMETHOD_(void, Setcr3dUnFace)(COLORREF) PURE;
	STDMETHOD_(CImageList*, SetImageList)(CImageList* pImageList, int nImageList) PURE;
	STDMETHOD_(void, SetFolderImageList)(CImageList * pImageList, int nNormal, int nDown, int nOver) PURE;
};

interface IGfxOutBarCtrl2 : public IGfxOutBarCtrl
{
	STDMETHOD_(int, AddFolderBar)(LPCSTR szFolderName, CWnd* pChild) PURE;
	STDMETHOD_(void, SetSelFolder)(int nIndex) PURE;
};

#endif // !defined(AFX_IGFXOUTBARCTRL_H__DFETFFA4_11B7_11D2_8437_0000B43382FE__INCLUDED_)