// ResourceHelper.cpp
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "axis.h"
#include "ResourceHelper.h"
#include "NPalette.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CResourceHelper::CResourceHelper()
{
	m_skinName = "";		

	m_font.CreatePointFont(90, "±¼¸²");
	m_fontB.CreateFont(	   12,                        // nHeight
				   0,                         // nWidth
				   0,                         // nEscapement
				   0,                         // nOrientation
				   FW_SEMIBOLD,	              // nWeight
				   FALSE,                     // bItalic
				   FALSE,                     // bUnderline
				   0,                         // cStrikeOut
				   DEFAULT_CHARSET,              // nCharSet
				   OUT_DEFAULT_PRECIS,        // nOutPrecision
				   CLIP_DEFAULT_PRECIS,       // nClipPrecision
				   DEFAULT_QUALITY,           // nQuality
				   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
				   "±¼¸²");                 // lpszFacename

#ifdef DF_USE_CPLUS17
	m_palette = std::make_unique<CNPalette>();
#else
	m_palette = new CNPalette;
#endif
}

void CResourceHelper::LoadIcon()
{
	if( !Axis::isCustomer )
	{
		m_icon.LoadBitmap(IDB_IBK);
		m_hIcon = (HICON)AfxGetApp()->LoadIcon(IDI_STAFF);
	}
	else
	{		
		m_icon.LoadBitmap(IDB_CUSTOM);
		m_hIcon = (HICON)AfxGetApp()->LoadIcon(IDI_CUSTOMER);
	}
	
}

CResourceHelper::~CResourceHelper()
{
#ifndef DF_USE_CPLUS17
	delete m_palette;
#endif

	ClearTabImages();
}

#ifdef DF_USE_CPLUS17
std::shared_ptr<CBitmap> CResourceHelper::LoadFileBitmap(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;

	fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		std::shared_ptr<CBitmap> bmp = std::make_shared<CBitmap>();
		bmp->Attach(hBitmap);

		return bmp;
	}
	return nullptr;
}
#else
CBitmap* CResourceHelper::LoadFileBitmap(const char* bmpName)
{
	HBITMAP hBitmap;
	CString fileName;

	fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), fileName, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		CBitmap* bmp = new CBitmap;
		bmp->Attach(hBitmap);

		return bmp;
	}
	return NULL;
}
#endif


#ifdef DF_USE_CPLUS17
BOOL CResourceHelper::ReloadFileBitmap(const char* bmpName, CBitmap* pBitmap)
{
	HBITMAP hBitmap{};
	CString fileName;

	pBitmap->DeleteObject();

	fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	HINSTANCE ins = AfxGetInstanceHandle();
	hBitmap = (HBITMAP)::LoadImage(ins, fileName,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		pBitmap->Attach(hBitmap);

		return TRUE;
	}

	return FALSE;
}
#else
BOOL CResourceHelper::ReloadFileBitmap(const char* bmpName,CBitmap* pBitmap)
{
	HBITMAP hBitmap{};
	CString fileName;

	pBitmap->DeleteObject();

	fileName.Format("%s\\image\\%s.bmp", Axis::home, bmpName);
	HINSTANCE ins = AfxGetInstanceHandle();
	hBitmap = (HBITMAP)::LoadImage(ins, fileName, 
			IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
	if (hBitmap)
	{
		pBitmap->Attach(hBitmap);

		return TRUE;
	}

	return FALSE;
}
#endif

CFont* CResourceHelper::GetFont()
{
	return &m_font;
}

CFont* CResourceHelper::GetBoldFont()
{
	return &m_fontB;
}

#ifdef DF_USE_CPLUS17
CBitmap* CResourceHelper::GetTabImg(int index)
{
	CBitmap* bmp{};

	auto ft = m_tabBitmaps.find(index);
	if (ft == m_tabBitmaps.end())
		return nullptr;
	else
		return ft->second.get();
}
#else
CBitmap* CResourceHelper::GetTabImg(int index)
{
	CBitmap* bmp;
	
	if (m_tabBitmaps.Lookup(index, bmp))
		return bmp;
	else
		return NULL;
}
#endif

COLORREF CResourceHelper::GetColor(int index)
{
	if (!m_palette)	return RGB(255, 255, 255);
	return m_palette->GetColor(index);
}

void CResourceHelper::LoadTabImages()
{
	ClearTabImages();

	const char* const tabImageNames[] = {
		"NM_l_%stab", "NM_%stab", "NM_r_%stab", "NM_l_s%stab", "NM_s%stab", "NM_r_s%stab", NULL
	};

#ifdef DF_USE_CPLUS17
	CString fileName;

	for (int i = 0; tabImageNames[i]; i++)
	{
		fileName.Format(tabImageNames[i], Axis::skinName);
		auto bmp = LoadFileBitmap(fileName);
		if (bmp)
			m_tabBitmaps[i] = bmp;
	}
#else
	CString fileName;
	CBitmap* bmp{};

	for (int i = 0; tabImageNames[i]; i++)
	{
		fileName.Format(tabImageNames[i], Axis::skinName);	

		bmp = LoadFileBitmap(fileName);
		if (bmp)
		{
			m_tabBitmaps[i] = bmp;
		}
	}
#endif
}

#ifdef DF_USE_CPLUS17
void CResourceHelper::ReloadTabImages()
{
	const char* const tabImageNames[] = {
		"NM_l_%stab", "NM_%stab", "NM_r_%stab", "NM_l_s%stab", "NM_s%stab", "NM_r_s%stab", NULL
	};

	CString fileName;
	CBitmap* bmp{};

	for (int ii = 0; ii < (int)m_tabBitmaps.size(); ii++)
	{
		std::shared_ptr<CBitmap> ft = m_tabBitmaps[ii];
		CString str;
		str.Format(tabImageNames[ii], Axis::skinName);
		ReloadFileBitmap(str, ft.get());
	}
}
#else
void CResourceHelper::ReloadTabImages()
{
	const char* const tabImageNames[] = {
		"NM_l_%stab", "NM_%stab", "NM_r_%stab", "NM_l_s%stab", "NM_s%stab", "NM_r_s%stab", NULL
	};

	CString fileName;
	CBitmap* bmp{};

	int index = 0;
	
	for (POSITION pos = m_tabBitmaps.GetStartPosition(); pos; ) 
	{
		m_tabBitmaps.GetNextAssoc(pos, index, bmp);

		CString str;
		str.Format(tabImageNames[index],Axis::skinName);
		
		ReloadFileBitmap( str, bmp );
		index++;
	}

}
#endif

#ifdef DF_USE_CPLUS17
void CResourceHelper::ClearTabImages()
{
	m_tabBitmaps.clear();
}
#else
void CResourceHelper::ClearTabImages()
{
	CBitmap* bmp{};
	int index{};
	
	for (POSITION pos = m_tabBitmaps.GetStartPosition(); pos; ) {
		m_tabBitmaps.GetNextAssoc(pos, index, bmp);
		delete bmp;
	}

	m_tabBitmaps.RemoveAll();
}
#endif


void CResourceHelper::ChangeRES(CString skinName,BOOL bReload)
{
	m_skinName = skinName;

	if( bReload )
		ReloadTabImages();
	else
		LoadTabImages();
}

CBitmap* CResourceHelper::GetSiteIcon()
{
	return &m_icon;
}

HICON CResourceHelper::GetIcon()
{
	return m_hIcon;
}