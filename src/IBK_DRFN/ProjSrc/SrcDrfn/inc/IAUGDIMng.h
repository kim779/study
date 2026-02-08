#if !defined(AFX_INTERFACE_OF_GDI_H__320E_1464_3_BY_JUNOK_LEE_B1A__INCLUDED_)
#define AFX_INTERFACE_OF_GDI_H__320E_1464_3_BY_JUNOK_LEE_B1A__INCLUDED_

// #include "../../_Include/IGDIMng.h"
#include "IBaseDefine.h"

// Resource Manager(IAUGDIManager)
//	int nID = 4;
//	IAUGDIManager* pRscMng = (IAUGDIManager*)SendMessage(RMSG_INTERFACE, nID, 0);
//	IAUGDIManager* pMng = (IAUGDIManager*)AfxGetPctrInterface(nID);

class CFontKey : public CObject  
{
private:
//public:
	LONG m_nBold;
	LONG m_nSize;
	//CString m_szFontName;
	char m_szFnName[20];
	
public:

	LONG GetBold() {	return m_nBold; }
	LONG GetSize() {	return m_nSize;	}
	char* GetName(){	return m_szFnName;	}
	void SetData(LONG nFontBold, LONG nFontSize, char* szFontName)
	{
		m_nBold = nFontBold;
		m_nSize = nFontSize;
		
//		sprintf(m_szFnName, "%-19.19s", szFontName);
		CString szFont=szFontName;
		szFont.TrimRight();
		sprintf(m_szFnName, (LPSTR)(LPCSTR)szFont);
	}
	
//	BOOL Compare(CFontKey* pTarget)
//	{
//		if(m_nBold!=pTarget->GetBold()) return FALSE;
//		if(m_nSize!=pTarget->GetSize()) return FALSE;
//		return (m_szFontName.Compare(pTarget->GetName()));
//	}

public:
	CFontKey()
	{
		m_nBold = 0;
		m_nSize = 12;
		sprintf(m_szFnName, _T("돋움체"));
	}
	
	CFontKey(LONG Bold, LONG Size, char* szFontName)
	{
		m_nBold = Bold;
		m_nSize = Size;

//		sprintf(m_szFnName, "%-19.19s", szFontName);
		CString szFont=szFontName;
		szFont.TrimRight();
		sprintf(m_szFnName, (LPSTR)(LPCSTR)szFont);
	}

	virtual ~CFontKey()	{}
};

//class CResourceManager
//{
//public:
//	CResourceManager() {}
//	virtual ~CResourceManager() {}
//
//};

class CResourceManager  
{
public:
	CResourceManager(){};
	virtual ~CResourceManager(){};

	virtual LONG UnAdviseFont(CFontKey &key){return 0;};
	virtual LONG AdviseFont(CFontKey &key){return 0;};
	virtual LONG GetBaseFont(LONG lStyle){return NULL;};
	virtual BOOL CreateBaseFont(char* sFontName, int nSize){return 0;};
	virtual BOOL DeleteBaseFont(){return 0;};
	virtual LONG AdviseResource(UINT nResource, long lData) {return 0;};
	virtual LONG UnAdviseResource(UINT nResource, long lData){return 0;};
};

// 기존 리소스매니저와의 호환을 위해서 이렇게 작업함.
//DECLARE_INTERFACE(IGDIManager)
class IAUGDIManager : public CResourceManager
{
public:
/*	// ---------------------------------------------------------------------
	// 기존에 사용하고 있던 CResourceManager
	// 이 메시지를 보는 사람은 CResourceManager 이 부분은 사용하지 말것.
	// ---------------------------------------------------------------------
	// CResourceManager .....start
	STDMETHOD_(LONG, AdviseFont)(CFontKey &key) PURE;
	STDMETHOD_(LONG, UnAdviseFont)(CFontKey &key) PURE;
	STDMETHOD_(LONG, GetBaseFont)(LONG lStyle) PURE;
	STDMETHOD_(BOOL, CreateBaseFont)(char* sFontName, int nSize) PURE;
	STDMETHOD_(BOOL, DeleteBaseFont)() PURE;
//	STDMETHOD_(LONG, AdviseResource)(UINT nResource, long lData) PURE;
//	STDMETHOD_(LONG, UnAdviseResource)(UINT nResource, long lData) PURE;
	// CResourceManager .....end
*/
	// ---------------------------------------------------------------------
	// Font GDI-Object
	// ---------------------------------------------------------------------
	// Input Param: LOGFONT 정보
//	STDMETHOD_(CFont*, CreateFontIndirect)(const LOGFONT* lpLogFont ) PURE;
//	STDMETHOD_(HFONT,  CreateFontIndirectHandle)(const LOGFONT* lpLogFont ) PURE;

	// Input Param: 폰트명+Point크기+Weight+CDC
//	STDMETHOD_(CFont*, CreatePointFont)( int nPointSize, LPCTSTR lpszFaceName, CDC* pDC = NULL ) PURE;
//	STDMETHOD_(HFONT, CreatePointFontHandle)( int nPointSize, LPCTSTR lpszFaceName, CDC* pDC = NULL ) PURE;

	// Input Param: Height+Width+
//	STDMETHOD_(CFont*, CreateFont)(int nHeight, int nWidth, int nEscapement, int nOrientation,
//						int nWeight, BYTE byItalic, BYTE byUnderLine, BYTE byStrikeOut,
//						BYTE byCharSet, BYTE byOutputPrecision, BYTE byClipPrecision,
//						BYTE byQuality, BYTE byPitchAndFamily, LPCTSTR lpszFontName) PURE;
//
//	STDMETHOD_(HFONT, CreateFontHandle)(int nHeight, int nWidth, int nEscapement, int nOrientation,
//						int nWeight, BYTE byItalic, BYTE byUnderLine, BYTE byStrikeOut,
//						BYTE byCharSet, BYTE byOutputPrecision, BYTE byClipPrecision,
//						BYTE byQuality, BYTE byPitchAndFamily, LPCTSTR lpszFontName) PURE;
//
//	STDMETHOD_(CFont*, CreatePointFontIndirect)( const LOGFONT* lpLogFont, CDC* pDC = NULL ) PURE;
//	STDMETHOD_(HFONT, CreatePointFontIndirectHandle)( const LOGFONT* lpLogFont, CDC* pDC = NULL ) PURE;

	// Input Param: 폰트명+Height+Weight
//	STDMETHOD_(CFont*, CreateFont)(LPCTSTR lpszFontName, int nHeight, int nWeight) PURE;
//	STDMETHOD_(HFONT, CreateFontHandle)(LPCTSTR lpszFontName, int nHeight, int nWeight) PURE;

	// Input Param: 사용한 CFont* 반환
	// Reference가 0일때 삭제된다.
//	STDMETHOD_(BOOL, DeleteFont)(CFont* pObject) PURE;
//	STDMETHOD_(BOOL, DeleteFont)(HFONT  hObject) PURE;

	// ---------------------------------------------------------------------
	// Image GDI-Object
	// BITMAP/CURSOR/ICON 
	// ---------------------------------------------------------------------
	// Input Param: 이미지파일명
	STDMETHOD_(HANDLE, AdviseBitmap)(LPCTSTR lpszResourceName) PURE;
	STDMETHOD_(HANDLE, AdviseCursor)(LPCTSTR lpszResourceName) PURE;
	STDMETHOD_(HANDLE, AdviseIcon)(LPCTSTR lpszResourceName) PURE;
//	STDMETHOD_(CBitmap*, GetBitmap)(LPCTSTR lpszResourceName) PURE;
//	STDMETHOD_(HBITMAP, GetBitmapHandle)(LPCTSTR lpszResourceName) PURE;

	// Resource ID
//	STDMETHOD_(CBitmap*, GetBitmap)(UINT nIDResource) PURE;
//	STDMETHOD_(HBITMAP, GetBitmapHandle)(UINT nIDResource) PURE;
//
//	STDMETHOD_(CBitmap*, LoadOEMBitmap)( UINT nIDBitmap ) PURE;
//	STDMETHOD_(HBITMAP, LoadOEMBitmapHandle)( UINT nIDBitmap ) PURE;
//
//	STDMETHOD_(CBitmap*, LoadMappedBitmap)(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0) PURE;
//	STDMETHOD_(HBITMAP,  LoadMappedBitmapHandle)(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0) PURE;
//
//	STDMETHOD_(CBitmap*, CreateBitmap)( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits ) PURE;
//	STDMETHOD_(HBITMAP, CreateBitmapHandle)( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits ) PURE;
//
//	STDMETHOD_(CBitmap*, CreateBitmapIndirect)( LPBITMAP lpBitmap ) PURE;
//	STDMETHOD_(HBITMAP, CreateBitmapIndirectHandle)( LPBITMAP lpBitmap ) PURE;
//	
//	STDMETHOD_(CBitmap*, CreateCompatibleBitmap)( CDC* pDC, int nWidth, int nHeight ) PURE;
//	STDMETHOD_(HBITMAP, CreateCompatibleBitmapHandle)( CDC* pDC, int nWidth, int nHeight ) PURE;
//
//	STDMETHOD_(CBitmap*, CreateDiscardableBitmap)( CDC* pDC, int nWidth, int nHeight ) PURE;
//	STDMETHOD_(HBITMAP, CreateDiscardableBitmapHandle)( CDC* pDC, int nWidth, int nHeight ) PURE;

	// Input Param: 사용한 CBitmap* 반환
//	STDMETHOD_(BOOL, DeleteBitmap)(CBitmap* pObject) PURE;
//	STDMETHOD_(BOOL, DeleteBitmap)(HBITMAP  hObject) PURE;
	STDMETHOD_(BOOL, UnadviseBitmap)(HANDLE  hObject) PURE;
	STDMETHOD_(BOOL, UnadviseCursor)(HANDLE  hObject) PURE;
	STDMETHOD_(BOOL, UnadviseIcon)(HANDLE  hObject) PURE;

	//2005.11.16 by LYH
	STDMETHOD_(CImageList*, AdviseImageList)(LPCTSTR lpszResourceName, int nWidth, int nHeight) PURE;
	STDMETHOD_(BOOL, UnadviseImageList)(LPCTSTR lpszResourceName) PURE;
	//end
	// ---------------------------------------------------------------------
	// Pen GDI-Object
	// ---------------------------------------------------------------------
	// Pen를 얻는다.
	STDMETHOD_(CPen*, AdvisePen)( int nPenStyle, int nWidth, COLORREF crColor ) PURE;
	STDMETHOD_(HPEN, AdvisePenHandle)( int nPenStyle, int nWidth, COLORREF crColor ) PURE;

	STDMETHOD_(CPen*, AdvisePen)( int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL ) PURE;
	STDMETHOD_(HPEN, AdvisePenHandle)( int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL ) PURE;

	STDMETHOD_(CPen*, AdvisePenIndirect)( LPLOGPEN lpLogPen ) PURE;
	STDMETHOD_(HPEN, AdvisePenIndirectHandle)( LPLOGPEN lpLogPen ) PURE;

	// 사용이 끝난 brush를 반환
	STDMETHOD_(BOOL, UnadvisePen)(CPen* pObject) PURE;
	STDMETHOD_(BOOL, UnadvisePen)(HPEN  hObject) PURE;

	
	// ---------------------------------------------------------------------
	// Brush GDI-Object
	// ---------------------------------------------------------------------
	STDMETHOD_(CBrush*, AdviseSolidBrush)( COLORREF crColor ) PURE;
	STDMETHOD_(HBRUSH,  AdviseSolidBrushHandle)( COLORREF crColor ) PURE;
	STDMETHOD_(CBrush*, AdviseSolidBrush)( UINT uStyle, COLORREF crColor ) PURE;
	STDMETHOD_(HBRUSH,  AdviseSolidBrushHandle)( UINT uStyle, COLORREF crColor ) PURE;

//	STDMETHOD_(CBrush*, AdviseHatchBrush)( int nIndex, COLORREF crColor ) PURE;
//	STDMETHOD_(HBRUSH,  AdviseHatchBrushHandle)( int nIndex, COLORREF crColor ) PURE;
//
//	STDMETHOD_(CBrush*, AdviseBrushIndirect)( const LOGBRUSH* lpLogBrush ) PURE;
//	STDMETHOD_(HBRUSH,  AdviseBrushIndirectHandle)( const LOGBRUSH* lpLogBrush ) PURE;
//
//	STDMETHOD_(CBrush*, AdvisePatternBrush)( CBitmap* pBitmap ) PURE;
//	STDMETHOD_(HBRUSH,  AdvisePatternBrushHandle)( CBitmap* pBitmap ) PURE;
//
//	STDMETHOD_(CBrush*, AdviseDIBPatternBrush)( HGLOBAL hPackedDIB, UINT nUsage ) PURE;
//	STDMETHOD_(HBRUSH,  AdviseDIBPatternBrushHandle)( HGLOBAL hPackedDIB, UINT nUsage ) PURE;
//
//	STDMETHOD_(CBrush*, AdviseDIBPatternBrush)( const void* lpPackedDIB, UINT nUsage ) PURE;
//	STDMETHOD_(HBRUSH,  AdviseDIBPatternBrushHandle)( const void* lpPackedDIB, UINT nUsage ) PURE;
//
//	STDMETHOD_(CBrush*, AdviseSysColorBrush)( int nIndex ) PURE;
//	STDMETHOD_(HBRUSH,  AdviseSysColorBrushHandle)( int nIndex ) PURE;

	// 사용이 끝난 brush를 반환
	STDMETHOD_(BOOL, UnAdviseBrush)(CBrush* pObject) PURE;
	STDMETHOD_(BOOL, UnAdviseBrushHandle)(HBRUSH  hObject) PURE;

	// ---------------------------------------------------------------------
	// CPalette GDI-Object
	// ---------------------------------------------------------------------
	// Palette 얻는다.
//	STDMETHOD_(CPalette*, GetPalette)(LPLOGPALETTE lpLogPalette) PURE;
//	STDMETHOD_(HPALETTE, GetPaletteHandle)(LPLOGPALETTE lpLogPalette) PURE;
//
//	STDMETHOD_(CPalette*, CreateHalftonePalette)( CDC* pDC ) PURE;
//	STDMETHOD_(HPALETTE, CreateHalftonePaletteHandle)( CDC* pDC ) PURE;
//
//	// 사용이 끝난 Palette 핸들을 반환
//	STDMETHOD_(BOOL, DeletePalette)(CPalette* pObject) PURE;
//	STDMETHOD_(BOOL, DeletePalette)(HPALETTE  hObject) PURE;


	// ---------------------------------------------------------------------
	// CRgn GDI-Object
	// ---------------------------------------------------------------------
	// CRgn은 실제적으로 사용하는 부분이 드물기때문에 
	// 필요한 곳에서 직접 생성해서 사용한다.
};

DECLARE_INTERFACE(IMainResouceManager)
{
	// dwKey는 현재 사용 안함.
	// I-PARAM :
	// O-PARAM : 
	STDMETHOD_(IAUGDIManager*, CreateGDIManager)(long dwKey=0) PURE;
	
	// 
	// I-PARAM :
	// O-PARAM : 
	STDMETHOD_(void, DeleteGDIManager)(IAUGDIManager*) PURE;
};

#endif //AFX_INTERFACE_OF_GDI_H__320E_1464_3_BY_JUNOK_LEE_B1A__INCLUDED_
