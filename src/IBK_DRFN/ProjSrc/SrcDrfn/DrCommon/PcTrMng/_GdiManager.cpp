// PcTrMng.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "PcTrMng.h"

/////////////////////////////////////////////////////////////////////////////
// CGDIManager

CGDIFontInfo::CGDIFontInfo()
{
	m_nRefCount = 0;
	m_pKey = NULL;
	m_pFont = NULL;
}

CGDIFontInfo::CGDIFontInfo(LONG Bold, LONG Size, char* szFontName)
{
	m_nRefCount = 0;
	m_pKey = new CFontKey(Bold, Size, szFontName);
	if (m_pKey == NULL) AfxMessageBox("CGDIFontInfo::FontKey생성Error");
	m_pFont = NULL;
}

CGDIFontInfo::~CGDIFontInfo()
{
	if (m_pFont)
	{
		m_pFont->DeleteObject();
		delete m_pFont;
	}

	if (m_pKey)
		delete m_pKey;
}

BOOL CGDIFontInfo::CreateFont()
{
	m_pFont = new CFont; 
	
	LOGFONT logFont;
//	lfHeight = -MulDiv(PointSize, GetDeviceCaps(hDC, LOGPIXELSY), 72); 
//	logFont.lfHeight = 16;
//	logFont.lfHeight = m_pKey->m_nSize;
	logFont.lfHeight = m_pKey->GetSize();
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;	
//	logFont.lfWeight = m_pKey->m_nBold ? FW_BOLD | FW_NORMAL;//FW_BOLD;//FW_THIN
//	if (m_pKey->m_nBold)
//	if (m_pKey->GetBold())
//		logFont.lfWeight = FW_BOLD;
//	else 
//		logFont.lfWeight = FW_NORMAL;
	logFont.lfWeight = m_pKey->GetBold();
	logFont.lfItalic = FALSE;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	logFont.lfQuality = DEFAULT_QUALITY;
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
//	strcpy(logFont.lfFaceName, m_pKey->m_szFnName);
//	strcpy(logFont.lfFaceName, m_pKey->GetName());
	CString tmp = m_pKey->GetName();
	tmp.TrimLeft(); tmp.TrimRight();
	strcpy(logFont.lfFaceName, (LPTSTR)(LPCTSTR)tmp);

	return (m_pFont->CreateFontIndirect(&logFont)!=NULL);
}

CFont* CGDIFontInfo::AddRef()
{
	if (m_nRefCount > 0)
	{
		++m_nRefCount;
		return m_pFont;
	}
	else 
	{
		CreateFont();
		++m_nRefCount;
		return m_pFont;
	}
}

LONG CGDIFontInfo::ReleaseRef()
{
	--m_nRefCount;
	if(m_nRefCount==0)
	{
		SafeDelete(m_pFont);
	}
	return m_nRefCount;
}

CFontKey* CGDIFontInfo::GetKey()
{
	return m_pKey;
}

CFont* CGDIFontInfo::GetFont()
{
	return m_pFont;
}

LONG CGDIFontInfo::GetRefCount()
{
	return m_nRefCount;
}

BOOL CGDIManager::CompareKey(CFontKey& Key, CFontKey *pKey)
{
	char *a1 = Key.GetName();
	char *a2 = pKey->GetName();
	// return (strcmp(Key.m_szFnName, pKey->m_szFnName)==0);
	return (strcmp(a1, a2)==0);
	// return Key.Compare(pKey);
}


/////////////////////////////////////////////////////////////////////////////
// CGDIManager

CGDIManager::CGDIManager()
{
	MakeNull(m_pBaseNormalFont);
	MakeNull(m_pOldBaseNormalFont);
	MakeNull(m_pBaseBoldFont);
	MakeNull(m_pOldBaseBoldFont);
}

CGDIManager::~CGDIManager()
{
	RemoveAllFont();
	RemoveAllBitmap();
	RemoveAllPen();
	RemoveAllBrush();
	RemoveAllImageList();
}

LONG CGDIManager::AdviseFont(CFontKey &key)
{
	CGDIFontInfo* pFontKey = NULL;

	POSITION pos = m_ListFont.GetHeadPosition();
	for (;pos;)
	{
		pFontKey = m_ListFont.GetNext(pos);
		if (CompareKey(key, pFontKey->GetKey()))
		{
			return (LONG)(pFontKey->AddRef());
		}
	}

	pFontKey = new CGDIFontInfo(key.GetBold(), key.GetSize(), key.GetName());
	m_ListFont.AddTail(pFontKey);

	return (LONG)(pFontKey->AddRef());
}

LONG CGDIManager::UnAdviseFont(CFontKey &key)
{
	CGDIFontInfo* pFontKey = NULL;
	POSITION oldPos = NULL;

	POSITION pos = m_ListFont.GetHeadPosition();
	for (;pos;)
	{
		oldPos = pos;
		pFontKey = m_ListFont.GetNext(pos);

		if (CompareKey(key, pFontKey->GetKey()))
		{
			if (pFontKey->ReleaseRef() == 0)
			{
				delete pFontKey;
				m_ListFont.RemoveAt(oldPos);
			}
		}
	}

	return 0;
}

LONG CGDIManager::GetBaseFont(LONG lStyle)
{
	if (lStyle == 1)
	{
		return (LONG)m_pBaseNormalFont;
	}
	else if (lStyle == 2)
	{
		return (LONG)m_pBaseBoldFont;
	}
	return NULL;
}

BOOL CGDIManager::CreateBaseFont(char* sFontName, int nSize)
{
	//폰트 기본 정보 생성
	LOGFONT logFont;
	logFont.lfHeight = nSize;
	logFont.lfWidth = 0;
	logFont.lfEscapement = 0;
	logFont.lfOrientation = 0;	

	logFont.lfWeight = FW_NORMAL;
	logFont.lfItalic = FALSE;
	logFont.lfUnderline = FALSE;
	logFont.lfStrikeOut = FALSE;
	logFont.lfCharSet = DEFAULT_CHARSET;
	logFont.lfOutPrecision = OUT_CHARACTER_PRECIS;
	logFont.lfClipPrecision = CLIP_CHARACTER_PRECIS;
	logFont.lfQuality = DEFAULT_QUALITY;
	logFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
	CString tmp = sFontName;
	tmp.TrimLeft(); tmp.TrimRight();
	strcpy(logFont.lfFaceName, (LPTSTR)(LPCTSTR)tmp);

	//Noral Base Font 생성
	m_pBaseNormalFont = new CFont;
	m_pBaseNormalFont->CreateFontIndirect(&logFont);
	if(m_pOldBaseNormalFont != NULL)
	{
		m_pOldBaseNormalFont->DeleteObject();
		delete m_pOldBaseNormalFont;
	}
	m_pOldBaseNormalFont = m_pBaseNormalFont;

	//Bold Base Font 생성
/*
	logFont.lfWeight = FW_BOLD;
	if (m_pBaseBoldFont == NULL)
		m_pBaseBoldFont = new CFont;
	else
		m_pBaseBoldFont->DeleteObject();
	m_pBaseBoldFont->CreateFontIndirect(&logFont);
*/
	logFont.lfWeight = FW_BOLD;
	m_pBaseBoldFont = new CFont;
	m_pBaseBoldFont->CreateFontIndirect(&logFont);
	if(m_pOldBaseBoldFont != NULL)
	{
		m_pOldBaseBoldFont->DeleteObject();
		delete m_pOldBaseBoldFont;
	}
	m_pOldBaseBoldFont = m_pBaseBoldFont;
	
	if (m_pBaseNormalFont == NULL || m_pBaseBoldFont == NULL)
		return FALSE;
	return TRUE;
}

BOOL CGDIManager::DeleteBaseFont()
{
	if(m_pBaseNormalFont)
	{
		m_pBaseNormalFont->DeleteObject();
		delete m_pBaseNormalFont;
	}
	if(m_pBaseBoldFont)
	{
		m_pBaseBoldFont->DeleteObject();
		delete m_pBaseBoldFont;
	}
	
	if(m_pOldBaseNormalFont != NULL && m_pOldBaseNormalFont != m_pBaseNormalFont)
	{
		m_pOldBaseNormalFont->DeleteObject();
		delete m_pOldBaseNormalFont;
	}
	if(m_pOldBaseBoldFont != NULL && m_pOldBaseBoldFont != m_pBaseBoldFont)
	{
		m_pOldBaseBoldFont->DeleteObject();
		delete m_pOldBaseBoldFont;
	}
	return TRUE;
}


LONG CGDIManager::AdviseResource(UINT nResource, long lData)
{
	return NULL;
}

LONG CGDIManager::UnAdviseResource(UINT nResource, long lData)
{
	return NULL;
}

// CResourceManager .....end

// ---------------------------------------------------------------------
// Font GDI-Object
// ---------------------------------------------------------------------
// Input Param: LOGFONT 정보
//CFont* CGDIManager::CreateFontIndirect(const LOGFONT* lpLogFont )
//{
//	return NULL;
/*
// 이전에 존재하는 font인가 찾아본다. 존재하지 않는다면 새로 create
	CFont* pFont = m_ListFont.GetFont(lf);
	if (!pFont)
	{
		///// 새 font create
		pFont = new CFont();
		pFont->CreateFontIndirect(&lf);

		m_ListFont.AddPointer(pFont);
	}

	return pFont;
*/
//}

//HFONT  CGDIManager::CreateFontIndirectHandle(const LOGFONT* lpLogFont )
//{
//	CFont *pFont = CreateFontIndirect(lpLogFont);
//	if(pFont)
//		return (HFONT)pFont->GetSafeHandle();
//	return NULL;
//}

// Input Param: 폰트명+Point크기+Weight+CDC
//	virtual CFont* GetFont(LPCTSTR lpszFontName, int nPointSize, int nWeight = FW_NORMAL, CDC* pDC);
//	virtual HFONT GetFontHandle(LPCTSTR lpszFontName, int nPointSize, int nWeight = FW_NORMAL, CDC* pDC);
//CFont* CGDIManager::CreatePointFont( int nPointSize, LPCTSTR lpszFaceName, CDC* pDC )
//{
//	ASSERT(AfxIsValidString(lpszFaceName));
//	return NULL;

/*

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfCharSet	= DEFAULT_CHARSET;
	lf.lfHeight		= nPointSize;
	lf.lfWeight		= nWeight;

	if (_tcslen(lpszFontName) < LF_FACESIZE)
		_tcscpy(lf.lfFaceName, lpszFontName);
	else
		_tcscpy(lf.lfFaceName, _T(""));


	HDC hDC;
	if (pDC != NULL)
	{
		ASSERT_VALID(pDC);
		ASSERT(pDC->m_hAttribDC != NULL);
		hDC = pDC->m_hAttribDC;
	}
	else
		hDC = ::GetDC(NULL);


	///// convert nPointSize to logical units based on pDC
	POINT pt;
	pt.y = ::GetDeviceCaps(hDC, LOGPIXELSY) * lf.lfHeight;
	pt.y /= 720;								// 72 points/inch, 10 decipoints/point
	::DPtoLP(hDC, &pt, 1);
	POINT ptOrg = { 0, 0 };
	::DPtoLP(hDC, &ptOrg, 1);
	lf.lfHeight = -abs(pt.y - ptOrg.y);

	if (pDC == NULL)
		ReleaseDC(NULL, hDC);


	///// 이전에 존재하는 font인가 찾아본다. 존재하지 않는다면 새로 create
	CFont* pFont = m_ListFont.GetFont(lf);
	if (!pFont)
	{
		///// 새 font create
		pFont = new CFont();
		pFont->CreateFontIndirect(&lf);

		m_ListFont.AddPointer(pFont);
	}

	return pFont;
*/
//}
//
//HFONT CGDIManager::CreatePointFontHandle( int nPointSize, LPCTSTR lpszFaceName, CDC* pDC )
//{
//	return NULL;
//}
//
//// Input Param: Height+Width+
//CFont* CGDIManager::CreateFont(int nHeight, int nWidth, int nEscapement, int nOrientation,
//			int nWeight, BYTE byItalic, BYTE byUnderLine, BYTE byStrikeOut,
//			BYTE byCharSet, BYTE byOutputPrecision, BYTE byClipPrecision,
//			BYTE byQuality, BYTE byPitchAndFamily, LPCTSTR lpszFontName)
//{
//	return NULL;
/*
	ASSERT(AfxIsValidString(lpszFontName));


	LOGFONT lf;

	///// LOGFONT 정보를 채운다.
	lf.lfHeight				= nHeight;
	lf.lfWidth				= nWidth;
	lf.lfEscapement			= nEscapement;
	lf.lfOrientation		= nOrientation;
	lf.lfWeight				= nWeight;
	lf.lfItalic				= byItalic;
	lf.lfUnderline			= byUnderLine;
	lf.lfStrikeOut			= byStrikeOut;
	lf.lfCharSet			= byCharSet;
	lf.lfOutPrecision		= byOutputPrecision;
	lf.lfClipPrecision		= byClipPrecision;
	lf.lfQuality			= byQuality;
	lf.lfPitchAndFamily		= byPitchAndFamily;
	if (_tcslen(lpszFontName) < LF_FACESIZE)
		_tcscpy(lf.lfFaceName, lpszFontName);
	else
		_tcscpy(lf.lfFaceName, _T(""));



	///// 이전에 존재하는 font인가 찾아본다. 존재하지 않는다면 새로 create
	CFont* pFont = m_ListFont.GetFont(lf);
	if (!pFont)
	{
		///// 새 font create
		pFont = new CFont();
		pFont->CreateFontIndirect(&lf);

		m_ListFont.AddPointer(pFont);
	}

	return pFont;
*/
//}
//
//HFONT CGDIManager::CreateFontHandle(int nHeight, int nWidth, int nEscapement, int nOrientation,
//			int nWeight, BYTE byItalic, BYTE byUnderLine, BYTE byStrikeOut,
//			BYTE byCharSet, BYTE byOutputPrecision, BYTE byClipPrecision,
//			BYTE byQuality, BYTE byPitchAndFamily, LPCTSTR lpszFontName)
//{
//	return NULL;
//}
//
//CFont* CGDIManager::CreatePointFontIndirect( const LOGFONT* lpLogFont, CDC* pDC )
//{
//	return NULL;
//}
//
//HFONT CGDIManager::CreatePointFontIndirectHandle( const LOGFONT* lpLogFont, CDC* pDC )
//{
//	return NULL;
//}

// Input Param: 폰트명+Height+Weight
//CFont* CGDIManager::CreateFont(LPCTSTR lpszFontName, int nHeight, int nWeight)
//{
//	return NULL;
//}
//
//HFONT CGDIManager::CreateFontHandle(LPCTSTR lpszFontName, int nHeight, int nWeight)
//{
//	return NULL;
//}

// Input Param: 사용한 CFont* 반환
// Reference가 0일때 삭제된다.
//BOOL CGDIManager::DeleteFont(CFont* pObject)
//{
//	return NULL;
//}
//
//BOOL CGDIManager::DeleteFont(HFONT  hObject)
//{
//	return NULL;
//}

// ---------------------------------------------------------------------
// Image GDI-Object
// CBitmap 에서 제공하는 함수 제공
// ---------------------------------------------------------------------
// Input Param: 이미지파일명
HANDLE CGDIManager::AdviseBitmap(LPCTSTR lpszResourceName)
{
	// 이전에 존재하는 Bitmap인가 찾아본다. 존재하지 않는다면 새로 create
	CGDIBitmapInfo* pInfo = FindBitmap(lpszResourceName);
	if(pInfo) 
	{
		pInfo->AddCount();
		return pInfo->GetHandle();
	}
	else
	{
		pInfo = new CGDIBitmapInfo(lpszResourceName);
		if(pInfo->GetCount())
		{
			m_ListBitmap.AddTail(pInfo);
			return pInfo->GetHandle();
		}
	}
	return NULL;
}

HANDLE CGDIManager::AdviseCursor(LPCTSTR lpszResourceName)
{
	// 이전에 존재하는 Bitmap인가 찾아본다. 존재하지 않는다면 새로 create
	CGDIBitmapInfo* pInfo = FindBitmap(lpszResourceName);
	if(pInfo) 
	{
		pInfo->AddCount();
		return pInfo->GetHandle();
	}
	else
	{
		pInfo = new CGDIBitmapInfo(lpszResourceName, IMAGE_CURSOR);
		if(pInfo->GetCount())
		{
			m_ListBitmap.AddTail(pInfo);
			return pInfo->GetHandle();
		}
	}
	return NULL;
}

HANDLE CGDIManager::AdviseIcon(LPCTSTR lpszResourceName)
{
	// 이전에 존재하는 Bitmap인가 찾아본다. 존재하지 않는다면 새로 create
	CGDIBitmapInfo* pInfo = FindBitmap(lpszResourceName);
	if(pInfo) 
	{
		pInfo->AddCount();
		return pInfo->GetHandle();
	}
	else
	{
		pInfo = new CGDIBitmapInfo(lpszResourceName, IMAGE_ICON);
		if(pInfo->GetCount())
		{
			m_ListBitmap.AddTail(pInfo);
			return pInfo->GetHandle();
		}
	}
	return NULL;
}


//HBITMAP CGDIManager::GetBitmapHandle(LPCTSTR lpszResourceName)
//{
//	CBitmap* pObject = GetBitmap(lpszResourceName);
//	if(pObject)
//		return (HBITMAP)pObject->GetSafeHandle();
//	return NULL;
//}

// Resource ID
//CBitmap* CGDIManager::GetBitmap(UINT nIDResource)
//{
//	return NULL;
//}
//HBITMAP CGDIManager::GetBitmapHandle(UINT nIDResource)
//{
//	return NULL;
//}
//CBitmap* CGDIManager::LoadOEMBitmap( UINT nIDBitmap )
//{
//	return NULL;
//}
//HBITMAP CGDIManager::LoadOEMBitmapHandle( UINT nIDBitmap )
//{
//	return NULL;
//}
//CBitmap* CGDIManager::LoadMappedBitmap(UINT nIDBitmap, UINT nFlags, LPCOLORMAP lpColorMap, int nMapSize)
//{
//	return NULL;
//}
//HBITMAP  CGDIManager::LoadMappedBitmapHandle(UINT nIDBitmap, UINT nFlags, LPCOLORMAP lpColorMap, int nMapSize)
//{
//	return NULL;
//}
//CBitmap* CGDIManager::CreateBitmap( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits )
//{
//	return NULL;
//}
//HBITMAP CGDIManager::CreateBitmapHandle( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits )
//{
//	return NULL;
//}
//CBitmap* CGDIManager::CreateBitmapIndirect( LPBITMAP lpBitmap )
//{
//	return NULL;
//}
//HBITMAP CGDIManager::CreateBitmapIndirectHandle( LPBITMAP lpBitmap )
//{
//	return NULL;
//}
//CBitmap* CGDIManager::CreateCompatibleBitmap( CDC* pDC, int nWidth, int nHeight )
//{
//	return NULL;
//}
//HBITMAP CGDIManager::CreateCompatibleBitmapHandle( CDC* pDC, int nWidth, int nHeight )
//{
//	return NULL;
//}
//CBitmap* CGDIManager::CreateDiscardableBitmap( CDC* pDC, int nWidth, int nHeight )
//{
//	return NULL;
//}
//HBITMAP CGDIManager::CreateDiscardableBitmapHandle( CDC* pDC, int nWidth, int nHeight )
//{
//	return NULL;
//}


// Input Param: 사용한 CBitmap* 반환
//BOOL CGDIManager::DeleteBitmap(CBitmap* pObject)
//{
//	if(!pObject) return TRUE;
//
//	HBITMAP hObject = (HBITMAP)pObject->GetSafeHandle();
//	return DeleteBitmap(hObject);
//}

BOOL CGDIManager::UnadviseBitmap(HANDLE hObject)
{
	CGDIBitmapInfo* pInfo = NULL;
	POSITION pOld;
	for(POSITION pos=m_ListBitmap.GetHeadPosition(); pos;)
	{
		pOld = pos;
		pInfo = m_ListBitmap.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->GetHandle()==hObject)
			{
				if(pInfo->ReleaseCount()==0)
				{
					delete pInfo;
					m_ListBitmap.RemoveAt(pOld);
				}
				return TRUE;
			}
		}
	}
	return TRUE;
}

BOOL CGDIManager::UnadviseCursor(HANDLE  hObject)
{
	return UnadviseBitmap(hObject);
}

BOOL CGDIManager::UnadviseIcon(HANDLE  hObject)
{
	return UnadviseBitmap(hObject);
}


// ---------------------------------------------------------------------
// Pen GDI-Object
// ---------------------------------------------------------------------
// Pen를 얻는다.
CPen* CGDIManager::AdvisePen( int nPenStyle, int nWidth, COLORREF crColor )
{
	return NULL;
}

HPEN CGDIManager::AdvisePenHandle( int nPenStyle, int nWidth, COLORREF crColor )
{
	return NULL;
}

CPen* CGDIManager::AdvisePen( int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount, const DWORD* lpStyle )
{
	return NULL;
}

HPEN CGDIManager::AdvisePenHandle( int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount, const DWORD* lpStyle )
{
	return NULL;
}

CPen* CGDIManager::AdvisePenIndirect( LPLOGPEN lpLogPen )
{
	return NULL;
}

HPEN CGDIManager::AdvisePenIndirectHandle( LPLOGPEN lpLogPen )
{
	return NULL;
}

// 사용이 끝난 brush를 반환
BOOL CGDIManager::UnadvisePen(CPen* pObject)
{
	return TRUE;
}

BOOL CGDIManager::UnadvisePen(HPEN  hObject)
{
	return TRUE;
}

	
// ---------------------------------------------------------------------
// Brush GDI-Object
// ---------------------------------------------------------------------
CBrush* CGDIManager::AdviseSolidBrush( COLORREF crColor )
{	
	// 이전에 존재하는 Bitmap인가 찾아본다. 존재하지 않는다면 새로 create
	CGDIBrushInfo* pInfo = FindBrush(crColor);
	if(pInfo) 
	{
		pInfo->AddCount();
		return &pInfo->m_Brush;
	}
	else
	{
		pInfo = new CGDIBrushInfo(crColor);
		if(pInfo->GetCount())
		{
			m_ListBrush.AddTail(pInfo);
			return &pInfo->m_Brush;
		}
	}
	return NULL;
}

HBRUSH  CGDIManager::AdviseSolidBrushHandle( COLORREF crColor )
{
	// 이전에 존재하는 Bitmap인가 찾아본다. 존재하지 않는다면 새로 create
	CGDIBrushInfo* pInfo = FindBrush(crColor);
	if(pInfo) 
	{
		pInfo->AddCount();
		return pInfo->GetHandle();
	}
	else
	{
		pInfo = new CGDIBrushInfo(crColor);
		if(pInfo->GetCount())
		{
			m_ListBrush.AddTail(pInfo);
			return pInfo->GetHandle();
		}
	}
	return NULL;
}

CBrush* CGDIManager::AdviseSolidBrush( UINT uStyle, COLORREF crColor )
{
	return NULL;
}

HBRUSH  CGDIManager::AdviseSolidBrushHandle( UINT uStyle, COLORREF crColor )
{
	return NULL;
}

//CBrush* CGDIManager::AdviseHatchBrush( int nIndex, COLORREF crColor )
//{
//	return NULL;
//}
//
//HBRUSH  CGDIManager::AdviseHatchBrushHandle( int nIndex, COLORREF crColor )
//{
//	return NULL;
//}
//
//CBrush* CGDIManager::AdviseBrushIndirect( const LOGBRUSH* lpLogBrush )
//{
//	return NULL;
//}
//
//HBRUSH  CGDIManager::AdviseBrushIndirectHandle( const LOGBRUSH* lpLogBrush )
//{
//	return NULL;
//}
//
//CBrush* CGDIManager::AdvisePatternBrush( CBitmap* pBitmap )
//{
//	return NULL;
//}
//
//HBRUSH  CGDIManager::AdvisePatternBrushHandle( CBitmap* pBitmap )
//{
//	return NULL;
//}
//
//CBrush* CGDIManager::AdviseDIBPatternBrush( HGLOBAL hPackedDIB, UINT nUsage )
//{
//	return NULL;
//}
//
//HBRUSH  CGDIManager::AdviseDIBPatternBrushHandle( HGLOBAL hPackedDIB, UINT nUsage )
//{
//	return NULL;
//}
//
//CBrush* CGDIManager::AdviseDIBPatternBrush( const void* lpPackedDIB, UINT nUsage )
//{
//	return NULL;
//}
//
//HBRUSH  CGDIManager::AdviseDIBPatternBrushHandle( const void* lpPackedDIB, UINT nUsage )
//{
//	return NULL;
//}
//
//CBrush* CGDIManager::AdviseSysColorBrush( int nIndex )
//{
//	return NULL;
//}
//
//HBRUSH  CGDIManager::AdviseSysColorBrushHandle( int nIndex )
//{
//	return NULL;
//}

// 사용이 끝난 brush를 반환
BOOL CGDIManager::UnAdviseBrush(CBrush* pObject)
{
	CGDIBrushInfo* pInfo = NULL;
	POSITION pOld;
	for(POSITION pos=m_ListBrush.GetHeadPosition(); pos;)
	{
		pOld = pos;
		pInfo = m_ListBrush.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->GetHandle() == pObject->GetSafeHandle())
			{
				if(pInfo->ReleaseCount()==0)
				{
					delete pInfo;
					m_ListBrush.RemoveAt(pOld);
				}
				return TRUE;
			}
		}
	}
	return TRUE;
}

BOOL CGDIManager::UnAdviseBrushHandle(HBRUSH  hObject)
{
	CGDIBrushInfo* pInfo = NULL;
	POSITION pOld;
	for(POSITION pos=m_ListBrush.GetHeadPosition(); pos;)
	{
		pOld = pos;
		pInfo = m_ListBrush.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->GetHandle() == hObject)
			{
				if(pInfo->ReleaseCount()==0)
				{
					delete pInfo;
					m_ListBrush.RemoveAt(pOld);
				}
				return TRUE;
			}
		}
	}
	return TRUE;
}

// ---------------------------------------------------------------------
// CPalette GDI-Object
// ---------------------------------------------------------------------
// Palette 얻는다.
//CPalette* CGDIManager::GetPalette(LPLOGPALETTE lpLogPalette)
//{
//	return NULL;
//}
//
//HPALETTE CGDIManager::GetPaletteHandle(LPLOGPALETTE lpLogPalette)
//{
//	return NULL;
//}
//
//CPalette* CGDIManager::CreateHalftonePalette( CDC* pDC )
//{
//	return NULL;
//}
//
//HPALETTE CGDIManager::CreateHalftonePaletteHandle( CDC* pDC )
//{
//	return NULL;
//}
//
//// 사용이 끝난 Palette 핸들을 반환
//BOOL CGDIManager::DeletePalette(CPalette* pObject)
//{
//	return TRUE;
//}
//
//BOOL CGDIManager::DeletePalette(HPALETTE  hObject)
//{
//	return TRUE;
//}
//
//----------------------------------------------------------------
// FONT 리소스 관련 start....
//----------------------------------------------------------------
void CGDIManager::RemoveAllFont()
{
	for(POSITION pos=m_ListFont.GetHeadPosition(); pos;)
		delete m_ListFont.GetNext(pos);
	m_ListFont.RemoveAll();
	DeleteBaseFont();
}

//----------------------------------------------------------------
// FONT 리소스 관련 end....
//----------------------------------------------------------------

//----------------------------------------------------------------
// BITMAP 리소스 관련 start....
//----------------------------------------------------------------
void CGDIManager::RemoveAllBitmap()
{
	for(POSITION pos=m_ListBitmap.GetHeadPosition(); pos;)
		delete m_ListBitmap.GetNext(pos);
	m_ListBitmap.RemoveAll();
}

// 관리리스트에 있으면 아이템 리턴. 없으면 NULL리턴.
CGDIBitmapInfo*  CGDIManager::FindBitmap(LPCSTR szResourceName)
{
	CGDIBitmapInfo* pInfo = NULL;
	for(POSITION pos=m_ListBitmap.GetHeadPosition(); pos;)
	{
		pInfo = m_ListBitmap.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->szResourceName.CompareNoCase(szResourceName)==0)
				return pInfo;
		}
	}
	return NULL;
}

//----------------------------------------------------------------
// BITMAP 리소스 관련 end....
//----------------------------------------------------------------

//----------------------------------------------------------------
// PEN 리소스 관련 start....
//----------------------------------------------------------------
void CGDIManager::RemoveAllPen()
{
	for(POSITION pos=m_ListPen.GetHeadPosition(); pos;)
		delete m_ListPen.GetNext(pos);
	m_ListPen.RemoveAll();
}

//----------------------------------------------------------------
// PEN 리소스 관련 end....
//----------------------------------------------------------------

//----------------------------------------------------------------
// BRUSH 리소스 관련 start....
//----------------------------------------------------------------
void CGDIManager::RemoveAllBrush()
{
	for(POSITION pos=m_ListBrush.GetHeadPosition(); pos;)
		delete m_ListBrush.GetNext(pos);
	m_ListBrush.RemoveAll();
}

// 관리리스트에 있으면 아이템 리턴. 없으면 NULL리턴.
CGDIBrushInfo*  CGDIManager::FindBrush(COLORREF crColor)
{
	CGDIBrushInfo* pInfo = NULL;
	for(POSITION pos=m_ListBrush.GetHeadPosition(); pos;)
	{
		pInfo = m_ListBrush.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->m_crColor == crColor)
				return pInfo;
		}
	}
	return NULL;
}
//----------------------------------------------------------------
// BRUSH 리소스 관련 end....
//----------------------------------------------------------------

//----------------------------------------------------------------
// PALETTE 리소스 관련 start....
//----------------------------------------------------------------

//----------------------------------------------------------------
// PALETTE 리소스 관련 end....
//----------------------------------------------------------------

//----------------------------------------------------------------
// ImageList 리소스 관련 start....
//----------------------------------------------------------------
CImageList* CGDIManager::AdviseImageList(LPCTSTR lpszResourceName, int nWidth, int nHeight)
{
	// 이전에 존재하는 Bitmap인가 찾아본다. 존재하지 않는다면 새로 create
	CGDIImageListInfo* pInfo = FindImageList(lpszResourceName);
	if(pInfo) 
	{
		pInfo->AddCount();
		return pInfo->GetHandle();
	}
	else
	{
		pInfo = new CGDIImageListInfo(lpszResourceName, nWidth, nHeight);
		if(pInfo->GetCount())
		{
			m_ListImageList.AddTail(pInfo);
			return pInfo->GetHandle();
		}
	}
	return NULL;
}
BOOL CGDIManager::UnadviseImageList(LPCTSTR lpszResourceName)
{
	CGDIImageListInfo* pInfo = NULL;
	POSITION pOld;
	for(POSITION pos=m_ListImageList.GetHeadPosition(); pos;)
	{
		pOld = pos;
		pInfo = m_ListImageList.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->m_szResourceName.CompareNoCase(lpszResourceName)==0)
			{
				if(pInfo->ReleaseCount()==0)
				{
					delete pInfo;
					m_ListImageList.RemoveAt(pOld);
				}
				return TRUE;
			}
		}
	}
	return TRUE;
}

void CGDIManager::RemoveAllImageList()
{
	for(POSITION pos=m_ListImageList.GetHeadPosition(); pos;)
		delete m_ListImageList.GetNext(pos);
	m_ListImageList.RemoveAll();
}

CGDIImageListInfo*  CGDIManager::FindImageList(LPCSTR szResourceName)
{
	CGDIImageListInfo* pInfo = NULL;
	for(POSITION pos=m_ListImageList.GetHeadPosition(); pos;)
	{
		pInfo = m_ListImageList.GetNext(pos);
		if(pInfo)
		{
			if(pInfo->m_szResourceName.CompareNoCase(szResourceName)==0)
				return pInfo;
		}
	}
	return NULL;
}
//----------------------------------------------------------------
// ImageList 리소스 관련 end....
//----------------------------------------------------------------

CMainResouceManager::~CMainResouceManager()
{
	RemoveAll();
}

void CMainResouceManager::RemoveAll()
{
	for(POSITION pos=m_List.GetHeadPosition(); pos; )
	{
		IAUGDIManager* pMng = m_List.GetNext(pos);
		if(pMng) delete pMng;
	}
	m_List.RemoveAll();
}

IAUGDIManager* CMainResouceManager::CreateGDIManager(long dwKey)
{
	IAUGDIManager* pMng = new CGDIManager;
	m_List.AddTail(pMng);
	return pMng;
}

void CMainResouceManager::DeleteGDIManager(IAUGDIManager* _pMng)
{
	POSITION pOld = NULL;
	for(POSITION pos=m_List.GetHeadPosition(); pos; )
	{
		pOld = pos;
		IAUGDIManager* pMng = m_List.GetNext(pos);
		if(pMng == _pMng)
		{
			delete pMng;
			m_List.RemoveAt(pOld);
		}
	}
}