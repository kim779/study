#ifndef __RESOURCEHELPER_H__
#define __RESOURCEHELPER_H__

#include "useMaindefine.h"
class CNPalette;

class CResourceHelper
{
public:
	CResourceHelper();
	~CResourceHelper();
private:
	HICON		m_hIcon;
#ifdef DF_USE_CPLUS17
	std::unique_ptr<CNPalette> m_palette;
#else
	CNPalette*	m_palette;
#endif
	//** for old version _axisRes compatible...
private:
	CFont	m_font;
	CFont	m_fontB;
	CBitmap	m_icon;
public:
	CFont*		GetFont();
	CFont*		GetBoldFont();
	CBitmap*	GetSiteIcon();
	CBitmap*	GetTabImg(int index);
	COLORREF	GetColor(int index);

	void		LoadIcon();

	void		ChangeRES(CString skinName = _T(""), BOOL bReload = FALSE);

	//** additional helper interface
private:	
#ifdef DF_USE_CPLUS17
	std::map<int, std::shared_ptr< CBitmap>> m_tabBitmaps;
#else
	CMap <int, int, CBitmap*, CBitmap*> m_tabBitmaps;
#endif
	
	CString		m_skinName;
#ifdef DF_USE_CPLUS17
	std::shared_ptr<CBitmap> LoadFileBitmap(const char* bmpName);
	BOOL		ReloadFileBitmap(const char* bmpName, std::shared_ptr<CBitmap> pBitmap);
#else
	CBitmap*	LoadFileBitmap(const char* bmpName);
#endif
	BOOL		ReloadFileBitmap(const char* bmpName, CBitmap* pBitmap);

	void		LoadTabImages();
	void		ClearTabImages();
	void		ReloadTabImages();
public:
	HICON		GetIcon();
};

#endif // __RESOURCEHELPER_H__