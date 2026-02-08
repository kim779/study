// PcTrMng.h : main header file for the PCTRMNG DLL
//
//
// 성공을 주는 말 : 현재하는 일이 나의 미래를 결정한다.
//                  원하는 성공에 맞춰 나의 하는 일을 바꿔봐요^^
// 최초작성자 : 이준옥 2005.11월 어느날.
// 수정자     :
#if !defined(AFX_PCTRMNG_H__DEAC0189_E61B__BY_JUNOK_LEE_INCLUDED_)
#define AFX_PCTRMNG_H__DEAC0189_E61B__BY_JUNOK_LEE_INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// _include 디렉토리의 내용기준으로 알파벳순으로 나열
// 직접적인 연관이 있는 파일은 비슷한 위치로 배열
//{{
//#include "../../inc/IAccountMng.h"
#include "../../Inc/IAUBGMng.h" // No.7
#include "../../inc/IOrderMonitorMng.h"	// No.8

// #include "../../inc/IAUGwanMng.h"			// No.9
#include "./GwanWnd.h"

#include "../../inc/IAUDrdsMng.h"
#include "../../inc/IAUGDIMng.h"
#include "../../inc/IAUHiddenMng.h"
#include "../../inc/IAUOrderMng.h"
#include "../../inc/IAUTrCommMng.h"
#include "./TrComm.h"

#include "../../inc/IDebugMng.h"	//No.14
#include "../../inc/LoadDRDebuger.h"

#include "../../inc/IEncryptionMng.h"
#include "../../inc/IGateMng.h"
#include "../../inc/IRegistryMng.h"
#include "../../inc/IMainInfoMng.h"
#include "../../inc/IMainExInfoMng.h"
#include "../../inc/IMasterDataMng.h"
#include "./CodeOcx.h"
#include "../../inc/ISignalMng.h"
#include "../../inc/ISkinColorMng.h"
#include "../../inc/IPCTRMng.h"
#include "../../inc/LoadDrds.h"
#include "../../inc/LoadOrderMng.h"
#include "../../inc/IDataUtilMng.h"				//No.24 IDataUtilManager
#include "../../inc/Packet.h"
#include "../../inc/IAUPCOrderMng.h"			//No.25

//#include "../../inc/ITreeListMng.h"			//No.26
#include "../../inc/LoadTreeListMng.h"

//#include "../../inc/IExtCodeWndMng.h"			//No.27 주식코드입력창
#include "../../inc/LoadExtCodeWndMng.h"
//}}

#include "../../inc/IAUDataMonitorMng.h"		//No.28 데이터모니터링 인터페이스
#include "../../inc/IClipboardMng.h"			//No.29
#include "../../inc/IAUPlatformDispMng.h"		//No.30, 31, 32, 33, 34, 35, 36, 37

#include "../../inc/IRunStateMng.h"				//No.38
#include "../../Inc/IControlPosMng.h"			//No.39	컨트롤 사이즈 관리.
// #include "../../inc/ICustomMng.h"			//No.40	ICumstomManager
#include "../../Inc/ISTMng.h"					//No.41 ISTManager

#include "../../inc/ISystemUtilMng.h"			//No.42 ISystemUtilManager
#include "../../inc/ICompanyMng.h"				//No.43 ICompanyManager

#include "../../inc/IAUChartUtilMng.h"			//No.44 IAUChartUtilManager
#include "../../inc/LoadMChartUtil.h"			// CChartUtilManager에서 사용

#include "../../inc/INewsSignalMng.h"			//No.45 INewsSignalManager
#include "./NewsMoniterWnd.h"

#include "../../inc/LoadPartnerMng.h"			//No.50 IPartnerSvrManager
												//No.51 IChartBozoMsgManager
#include "../../inc/LoadPctrWinix.h"

//#include "../../inc/IPacketConvertMng.h"		//No.51 IPacketConvertManager

//@Solomon090819#include "../../inc/IKSLibMng.h"				//No.50 소리마치 LIB인터페이스
//@Solomon090819#include "../../../../../CommonSrc/LinkDataManager.h"
//@Solomon090819#include "../../../../../ItemMaster/IItemMaster.h"
//@Solomon090819#include "../../../../../CommonSrc/IntrApiHelper.h"

/////////////////////////////////////////////////////////////////////////////
//
// #define MAX_INTERFACE	51
typedef struct 
{
	LONG pInterface;
} STUA_INTERFACE;

/////////////////////////////////////////////////////////////////////////////
// CSkinColorManager
//#include "../../inc/ISkinColorMng.h"

typedef CList<ISkinColorSite*, ISkinColorSite*>	LIST_SKINCOLORSITE;
class CSkinColorManager : public ISkinColorManager
{
public:
	CSkinColorManager();
	~CSkinColorManager();
	void LoadSkinInfo();
	

public:	// Implementation
	STDMETHOD_(COLORREF, GetColor)(LPCSTR szKeyName);
	STDMETHOD_(int, GetIndex)(LPCSTR szKeyName);
	STDMETHOD_(COLORREF, GetColor)(int nIndex);
	STDMETHOD_(long, GetColorTableArray)(); 
	STDMETHOD_(long, AdviseSite)(ISkinColorSite* pSite);
	STDMETHOD_(BOOL, UnadviseSite)(long dwKey);
	STDMETHOD_(BOOL, LoadColorTable)(LPCSTR szTableFile, BOOL bNotify=TRUE);
	STDMETHOD_(BOOL, LoadDefaultColorTable)(LPCSTR szTableFile); // 기본 스킨테이블 로드 ojtaso
	STDMETHOD_(void, NotifyToSite)();
	STDMETHOD_(BOOL, SetSkinConfig)(LPCSTR szCfgFileName) ;
	STDMETHOD_(int, GetSkinCount)() ;
	STDMETHOD_(BOOL, GetSkinInfo)(int nIndex, ST_SKININFO& skInfo) ;
	STDMETHOD_(COLORREF, GetDefaultColor)(LPCSTR szKeyName);
	STDMETHOD_(int, GetDefaultIndex)(LPCSTR szKeyName);
	STDMETHOD_(COLORREF, GetDefaultColor)(int nIndex);
	STDMETHOD_(long, GetDefaultColorTableArray)(); 

public:	// Variable
	ARR_COLOR_TABLE  m_ArrColorTable;
//@유진삭제	ARR_COLOR_TABLE  m_ArrColorTableDefault;		// 기본 스킨 테이블 ojtaso
	LIST_SKINCOLORSITE  m_ListSkinSite;
	ARRAY_ST_SKININFO  m_ArrSkinInfo;
	BOOL m_bLoadedCfgInfo;
	CString m_szSkinCfgFile;
};


/////////////////////////////////////////////////////////////////////////////
// CPCTRManager
//#include "../../inc/IPCTRMng.h"

class CPCTRManager : public IPCTRManager
{
public:
	CPCTRManager();
	~CPCTRManager();
	
public:	// Implementation
	STDMETHOD_(LPCSTR, GetData)(int nID);
	STDMETHOD_(BOOL, SetData)(int nID, LPCSTR szNewData);
	STDMETHOD_(COleVariant*, GetDataEx)(int nID);
	STDMETHOD_(BOOL, SetDataEx)(int nID, COleVariant* pNewData);
	STDMETHOD_(COleVariant*, GetDataEx)(LPCSTR szKey);
	STDMETHOD_(BOOL, SetDataEx)(LPCSTR szKey, COleVariant* pNewData);
	STDMETHOD_(LONG, GetTrData)(char szTR, int nCount, LPCSTR szData, long dwKey=0, LPCSTR szKey=NULL);

	STDMETHOD_(LONG, Advise)(int nKey, IPCTRSite* pSite);
	STDMETHOD_(void, UnAdvise)(LONG pKey);
	STDMETHOD_(LONG, AdviseEx)(int nKey, IPCTRSite* pSite);
	STDMETHOD_(void, UnAdviseEx)(LONG pKey);
public:	// Variable
};

/////////////////////////////////////////////////////////////////////////////
// CGDIManager
//#include "../../inc/IAUGDIMng.h"
class CBaseCount
{
public:
	CBaseCount(){m_nRefer = 0;};
	int			m_nRefer;
	virtual int GetCount() { return m_nRefer; };
	virtual int AddCount() { m_nRefer++; return m_nRefer;	};
	virtual int ReleaseCount() { m_nRefer--; return m_nRefer; };
};
class CGDIFontInfo : public CObject
{
private: // Variable
	CFontKey*	m_pKey;
	int			m_nRefCount;
	CFont*		m_pFont;

private: // Implementation
	BOOL		CreateFont();

public: // Variable
	CFontKey*	GetKey();
	CFont*		GetFont();
	LONG		GetRefCount();
	CFont*		AddRef();
	LONG		ReleaseRef();

public: // Implementation
	CGDIFontInfo();
	CGDIFontInfo(LONG Bold, LONG Size, char* szFontName);
	virtual ~CGDIFontInfo();
};
typedef CList<CGDIFontInfo*, CGDIFontInfo*> GDIM_LIST_FONTINFO;


class CGDIBitmapInfo : public CBaseCount
{
public:
	CGDIBitmapInfo() { Init(); }
	CGDIBitmapInfo(LPCSTR szResoureName, int nType=IMAGE_BITMAP) 
	{
		Init(nType);
		Create(szResoureName); 
	}
	~CGDIBitmapInfo()
	{
		//if(m_pObject)delete m_pObject;
		if(m_nType==IMAGE_BITMAP)		::DeleteObject(m_hObject);
		else if(m_nType==IMAGE_CURSOR)	::DestroyCursor((HICON)m_hObject);
		else if(m_nType==IMAGE_ICON)	::DestroyIcon((HICON)m_hObject);
	}
	
	void Init(int nType=IMAGE_BITMAP) 
	{
		//m_pObject=NULL;
		m_hObject = NULL;
		m_nType = nType;
	}

	HANDLE GetHandle() { return m_hObject; }
	BOOL Create(LPCSTR szResoureName, int nType=IMAGE_BITMAP)
	{
		m_hObject = ::LoadImage(NULL, szResoureName, nType, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);
		if(!m_hObject) return FALSE;

		AddCount();
		return TRUE;
	}

	int			m_nType; // IMAGE_BITMAP, IMAGE_CURSOR, IMAGE_ICON
//	CBitmap*	m_pObject;
	HANDLE		m_hObject;
	CString		szResourceName;
};
typedef CList<CGDIBitmapInfo*, CGDIBitmapInfo*> GDIM_LIST_BITMAPINFO;

class CGDIPenInfo
{
public:
	CGDIPenInfo() { m_nRefer=0; m_pObject=NULL;}
	CPen*		m_pObject;
	int			m_nRefer;
};
typedef CList<CGDIPenInfo*, CGDIPenInfo*> GDIM_LIST_PENINFO;

class CGDIBrushInfo : public CBaseCount
{
public:
	CGDIBrushInfo() {}
	//CBrush*		m_pObject;
	CBrush		m_Brush;
	COLORREF	m_crColor;
	CGDIBrushInfo(COLORREF crColor)
	{
		Create(crColor);
	}
	~CGDIBrushInfo() 
	{ 
		if(m_Brush.GetSafeHandle())
			m_Brush.DeleteObject();
	}
	
	BOOL Create(COLORREF crColor)
	{
		m_Brush.CreateSolidBrush(crColor);
		m_crColor = crColor;
		AddCount();
		return TRUE;
	}
	HBRUSH GetHandle() { return (HBRUSH)m_Brush.GetSafeHandle() ; }

};
typedef CList<CGDIBrushInfo*, CGDIBrushInfo*> GDIM_LIST_BRUSHINFO;

//2005.11.16 by LYH
class CGDIImageListInfo : public CBaseCount
{
public:
	CGDIImageListInfo() { Init(); }
	CGDIImageListInfo(LPCSTR szResoureName, int nWidth=0, int nHeight=0) 
	{
		Init();
		Create(szResoureName, nWidth, nHeight); 
	}
	~CGDIImageListInfo()
	{
		m_ImageList.DeleteImageList();
	}
	
	void Init() 
	{
		m_szResourceName = "";
	}

	CImageList* GetHandle() { return &m_ImageList; }
	BOOL Create(LPCSTR szResourceName, int nWidth, int nHeight)
	{

		CBitmap bitmap;
		BOOL bOK = bitmap.Attach ((HBITMAP)::LoadImage(NULL, szResourceName, IMAGE_BITMAP, 0, 0, 
		LR_LOADFROMFILE | LR_CREATEDIBSECTION | LR_DEFAULTSIZE));

		if ( !m_ImageList.Create( nWidth, nHeight, ILC_COLORDDB, 1, 1 ) )
		{
			AfxMessageBox ( "Can not create ImageList" );
			return FALSE;
		};

		m_ImageList.Add(&bitmap, RGB(250, 250, 250));
		TRACE1("%d", m_ImageList.GetImageCount());
		bitmap.DeleteObject();

		m_szResourceName = szResourceName;

		AddCount();
		return TRUE;
	}

	CImageList	m_ImageList;
	CString		m_szResourceName;
};
typedef CList<CGDIImageListInfo*, CGDIImageListInfo*> GDIM_LIST_IMAGELISTINFO;
/////////////////////////////////////////

class CGDIManager : public IAUGDIManager
{
public:
	CGDIManager();
	~CGDIManager();

	GDIM_LIST_FONTINFO		m_ListFont;
	void RemoveAllFont();
//	CGDIFontInfo*  FindFont();
	BOOL CompareKey(CFontKey& Key, CFontKey *pKey);
	
	//기본 폰트 정보
	CFont *m_pBaseNormalFont,	*m_pOldBaseNormalFont;
	CFont *m_pBaseBoldFont,		*m_pOldBaseBoldFont;

	GDIM_LIST_BITMAPINFO	m_ListBitmap;
	void RemoveAllBitmap();
	CGDIBitmapInfo*  FindBitmap(LPCSTR szResourceName);

	GDIM_LIST_PENINFO		m_ListPen;
	void RemoveAllPen();
	
	GDIM_LIST_BRUSHINFO		m_ListBrush;
	void RemoveAllBrush();
	CGDIBrushInfo*  FindBrush(COLORREF crColor);

//2005.11.16 by LYH
	GDIM_LIST_IMAGELISTINFO	m_ListImageList;
	void RemoveAllImageList();
	CGDIImageListInfo*  FindImageList(LPCSTR szResourceName);
//end
	
public:	// Implementation
	// 기존 리소스매니저를 위해  ..... start
	virtual LONG UnAdviseFont(CFontKey &key);
	virtual LONG AdviseFont(CFontKey &key);
	virtual LONG GetBaseFont(LONG lStyle);
	virtual BOOL CreateBaseFont(char* sFontName, int nSize);
	virtual BOOL DeleteBaseFont();
	virtual LONG AdviseResource(UINT nResource, long lData) ;
	virtual LONG UnAdviseResource(UINT nResource, long lData);
	// 기존 리소스매니저를 위해  ..... end

/*
	// CResourceManager .....start
	STDMETHOD_(LONG, AdviseFont)(CFontKey &key) ;
	STDMETHOD_(LONG, UnAdviseFont)(CFontKey &key) ;
	STDMETHOD_(LONG, GetBaseFont)(LONG lStyle) ;
	STDMETHOD_(BOOL, CreateBaseFont)(char* sFontName, int nSize) ;
	STDMETHOD_(BOOL, DeleteBaseFont)() ;
//	STDMETHOD_(LONG, AdviseResource)(UINT nResource, long lData) ;
//	STDMETHOD_(LONG, UnAdviseResource)(UINT nResource, long lData) ;
	// CResourceManager .....end
*/
	// ---------------------------------------------------------------------
	// Font GDI-Object
	// ---------------------------------------------------------------------
	// Input Param: LOGFONT 정보
//	STDMETHOD_(CFont*, CreateFontIndirect)(const LOGFONT* lpLogFont ) ;
//	STDMETHOD_(HFONT,  CreateFontIndirectHandle)(const LOGFONT* lpLogFont ) ;

	// Input Param: 폰트명+Point크기+Weight+CDC
//	STDMETHOD_(CFont*, CreatePointFont)( int nPointSize, LPCTSTR lpszFaceName, CDC* pDC = NULL ) ;
//	STDMETHOD_(HFONT, CreatePointFontHandle)( int nPointSize, LPCTSTR lpszFaceName, CDC* pDC = NULL ) ;

	// Input Param: Height+Width+
//	STDMETHOD_(CFont*, CreateFont)(int nHeight, int nWidth, int nEscapement, int nOrientation,
//						int nWeight, BYTE byItalic, BYTE byUnderLine, BYTE byStrikeOut,
//						BYTE byCharSet, BYTE byOutputPrecision, BYTE byClipPrecision,
//						BYTE byQuality, BYTE byPitchAndFamily, LPCTSTR lpszFontName) ;
//
//	STDMETHOD_(HFONT, CreateFontHandle)(int nHeight, int nWidth, int nEscapement, int nOrientation,
//						int nWeight, BYTE byItalic, BYTE byUnderLine, BYTE byStrikeOut,
//						BYTE byCharSet, BYTE byOutputPrecision, BYTE byClipPrecision,
//						BYTE byQuality, BYTE byPitchAndFamily, LPCTSTR lpszFontName) ;
//
//	STDMETHOD_(CFont*, CreatePointFontIndirect)( const LOGFONT* lpLogFont, CDC* pDC = NULL ) ;
//	STDMETHOD_(HFONT, CreatePointFontIndirectHandle)( const LOGFONT* lpLogFont, CDC* pDC = NULL ) ;

	// Input Param: 폰트명+Height+Weight
//	STDMETHOD_(CFont*, CreateFont)(LPCTSTR lpszFontName, int nHeight, int nWeight) ;
//	STDMETHOD_(HFONT, CreateFontHandle)(LPCTSTR lpszFontName, int nHeight, int nWeight) ;

	// Input Param: 사용한 CFont* 반환
	// Reference가 0일때 삭제된다.
//	STDMETHOD_(BOOL, DeleteFont)(CFont* pObject) ;
//	STDMETHOD_(BOOL, DeleteFont)(HFONT  hObject) ;

	// ---------------------------------------------------------------------
	// Image GDI-Object
	// CBitmap 에서 제공하는 함수 제공
	// ---------------------------------------------------------------------
	// Input Param: 이미지파일명
	STDMETHOD_(HANDLE, AdviseBitmap)(LPCTSTR lpszResourceName) ;
	STDMETHOD_(HANDLE, AdviseCursor)(LPCTSTR lpszResourceName) ;
	STDMETHOD_(HANDLE, AdviseIcon)(LPCTSTR lpszResourceName) ;

	//2005.11.16 by LYH
	STDMETHOD_(CImageList*, AdviseImageList)(LPCTSTR lpszResourceName, int nWidth, int nHeight) ;
	STDMETHOD_(BOOL, UnadviseImageList)(LPCTSTR lpszResourceName) ;
	//end
//	STDMETHOD_(CBitmap*, GetBitmap)(LPCTSTR lpszResourceName) ;
//	STDMETHOD_(HBITMAP, GetBitmapHandle)(LPCTSTR lpszResourceName) ;

	// Resource ID
//	STDMETHOD_(CBitmap*, GetBitmap)(UINT nIDResource) ;
//	STDMETHOD_(HBITMAP, GetBitmapHandle)(UINT nIDResource) ;
//
//	STDMETHOD_(CBitmap*, LoadOEMBitmap)( UINT nIDBitmap ) ;
//	STDMETHOD_(HBITMAP, LoadOEMBitmapHandle)( UINT nIDBitmap ) ;
//
//	STDMETHOD_(CBitmap*, LoadMappedBitmap)(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0) ;
//	STDMETHOD_(HBITMAP,  LoadMappedBitmapHandle)(UINT nIDBitmap, UINT nFlags = 0, LPCOLORMAP lpColorMap = NULL, int nMapSize = 0) ;
//
//	STDMETHOD_(CBitmap*, CreateBitmap)( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits ) ;
//	STDMETHOD_(HBITMAP, CreateBitmapHandle)( int nWidth, int nHeight, UINT nPlanes, UINT nBitcount, const void* lpBits ) ;
//
//	STDMETHOD_(CBitmap*, CreateBitmapIndirect)( LPBITMAP lpBitmap ) ;
//	STDMETHOD_(HBITMAP, CreateBitmapIndirectHandle)( LPBITMAP lpBitmap ) ;
//	
//	STDMETHOD_(CBitmap*, CreateCompatibleBitmap)( CDC* pDC, int nWidth, int nHeight ) ;
//	STDMETHOD_(HBITMAP, CreateCompatibleBitmapHandle)( CDC* pDC, int nWidth, int nHeight ) ;
//
//	STDMETHOD_(CBitmap*, CreateDiscardableBitmap)( CDC* pDC, int nWidth, int nHeight ) ;
//	STDMETHOD_(HBITMAP, CreateDiscardableBitmapHandle)( CDC* pDC, int nWidth, int nHeight ) ;

	// Input Param: 사용한 CBitmap* 반환
//	STDMETHOD_(BOOL, DeleteBitmap)(CBitmap* pObject) ;
//	STDMETHOD_(BOOL, DeleteBitmap)(HBITMAP  hObject) ;
	STDMETHOD_(BOOL, UnadviseBitmap)(HANDLE  hObject) ;
	STDMETHOD_(BOOL, UnadviseCursor)(HANDLE  hObject) ;
	STDMETHOD_(BOOL, UnadviseIcon)(HANDLE  hObject) ;

	// ---------------------------------------------------------------------
	// Pen GDI-Object
	// ---------------------------------------------------------------------
	// Pen를 얻는다.
	STDMETHOD_(CPen*, AdvisePen)( int nPenStyle, int nWidth, COLORREF crColor ) ;
	STDMETHOD_(HPEN, AdvisePenHandle)( int nPenStyle, int nWidth, COLORREF crColor ) ;

	STDMETHOD_(CPen*, AdvisePen)( int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL ) ;
	STDMETHOD_(HPEN, AdvisePenHandle)( int nPenStyle, int nWidth, const LOGBRUSH* pLogBrush, int nStyleCount = 0, const DWORD* lpStyle = NULL ) ;

	STDMETHOD_(CPen*, AdvisePenIndirect)( LPLOGPEN lpLogPen ) ;
	STDMETHOD_(HPEN, AdvisePenIndirectHandle)( LPLOGPEN lpLogPen ) ;

	// 사용이 끝난 brush를 반환
	STDMETHOD_(BOOL, UnadvisePen)(CPen* pObject) ;
	STDMETHOD_(BOOL, UnadvisePen)(HPEN  hObject) ;

	
	// ---------------------------------------------------------------------
	// Brush GDI-Object
	// ---------------------------------------------------------------------
	STDMETHOD_(CBrush*, AdviseSolidBrush)( COLORREF crColor ) ;
	STDMETHOD_(HBRUSH,  AdviseSolidBrushHandle)( COLORREF crColor ) ;
	STDMETHOD_(CBrush*, AdviseSolidBrush)( UINT uStyle, COLORREF crColor ) ;
	STDMETHOD_(HBRUSH,  AdviseSolidBrushHandle)( UINT uStyle, COLORREF crColor ) ;

//	STDMETHOD_(CBrush*, AdviseHatchBrush)( int nIndex, COLORREF crColor ) ;
//	STDMETHOD_(HBRUSH,  AdviseHatchBrushHandle)( int nIndex, COLORREF crColor ) ;
//
//	STDMETHOD_(CBrush*, AdviseBrushIndirect)( const LOGBRUSH* lpLogBrush ) ;
//	STDMETHOD_(HBRUSH,  AdviseBrushIndirectHandle)( const LOGBRUSH* lpLogBrush ) ;
//
//	STDMETHOD_(CBrush*, AdvisePatternBrush)( CBitmap* pBitmap ) ;
//	STDMETHOD_(HBRUSH,  AdvisePatternBrushHandle)( CBitmap* pBitmap ) ;
//
//	STDMETHOD_(CBrush*, AdviseDIBPatternBrush)( HGLOBAL hPackedDIB, UINT nUsage ) ;
//	STDMETHOD_(HBRUSH,  AdviseDIBPatternBrushHandle)( HGLOBAL hPackedDIB, UINT nUsage ) ;
//
//	STDMETHOD_(CBrush*, AdviseDIBPatternBrush)( const void* lpPackedDIB, UINT nUsage ) ;
//	STDMETHOD_(HBRUSH,  AdviseDIBPatternBrushHandle)( const void* lpPackedDIB, UINT nUsage ) ;
//
//	STDMETHOD_(CBrush*, AdviseSysColorBrush)( int nIndex ) ;
//	STDMETHOD_(HBRUSH,  AdviseSysColorBrushHandle)( int nIndex ) ;

	// 사용이 끝난 brush를 반환
	STDMETHOD_(BOOL, UnAdviseBrush)(CBrush* pObject) ;
	STDMETHOD_(BOOL, UnAdviseBrushHandle)(HBRUSH  hObject) ;

	// ---------------------------------------------------------------------
	// CPalette GDI-Object
	// ---------------------------------------------------------------------
	// Palette 얻는다.
//	STDMETHOD_(CPalette*, GetPalette)(LPLOGPALETTE lpLogPalette) ;
//	STDMETHOD_(HPALETTE, GetPaletteHandle)(LPLOGPALETTE lpLogPalette) ;
//
//	STDMETHOD_(CPalette*, CreateHalftonePalette)( CDC* pDC ) ;
//	STDMETHOD_(HPALETTE, CreateHalftonePaletteHandle)( CDC* pDC ) ;
//
//	// 사용이 끝난 Palette 핸들을 반환
//	STDMETHOD_(BOOL, DeletePalette)(CPalette* pObject) ;
//	STDMETHOD_(BOOL, DeletePalette)(HPALETTE  hObject) ;

public:	// Variable

};

typedef CList<IAUGDIManager*, IAUGDIManager*> LIST_IAUGDIManager;
class CMainResouceManager : public IMainResouceManager
{
public:
	~CMainResouceManager();
	void RemoveAll();

	LIST_IAUGDIManager	m_List;

public:
	STDMETHOD_(IAUGDIManager*, CreateGDIManager)(long dwKey=0) ;
	STDMETHOD_(void, DeleteGDIManager)(IAUGDIManager*) ;
};

/////////////////////////////////////////////////////////////////////////////
// CTrCommManager
// #include "../../inc/IAUTrCommMng.h"
// #include "TrComm.h"

class CTrCommImpl;
typedef CList<CTrCommImpl*, CTrCommImpl*> LISTTRCOMM;
class CTrCommManager : public IAUTrCommManager
{
public:
	CTrCommManager();
	~CTrCommManager();
	
public:	// Implementation
	STDMETHOD_(ITrComm*, AdviseTrComm)() ;
	STDMETHOD_(void, UnAdviseTrComm)(ITrComm*) ;

	// 메인이 디폴트로 생성한 소켓의 핸들.
	// I-PARAM : 
	// O-PARAM : 
	STDMETHOD_(HWND, GetSocketHandle)(LPCSTR szType="MAIN") ;
	STDMETHOD_(void, SetSocketHandle)(LPCSTR szType, HWND hSocket) ;


public:// Variable
	LISTTRCOMM m_TrCommList;
	HWND m_hSocket;
};

//class CTrCommImpl : public ITrComm
class CTrCommImpl : public ITrCommLast
{
public:
	CTrCommImpl(HWND hSocket=NULL);
	~CTrCommImpl();
	
public:	// Implementation
	STDMETHOD_(BOOL, Send2Server)(void* szData, int nLen, int nKey) ;
	STDMETHOD_(void, SetTR)(LPCSTR szTR, BOOL bCert = FALSE) ;
	STDMETHOD_(void, ChangeCallback)(ITrCommSite* pSite) ;

	STDMETHOD_(void, SetBaseData) (LPCSTR szKey, LPCSTR szValue);
	STDMETHOD_(void, GetBaseData) (LPCSTR szKey, CString &rValue);
	STDMETHOD_(void, SetBaseDataLong) (LPCSTR szKey, LONG dwValue);

public:	// Variable
	HWND m_hSocket;
	CTRComm* m_pTrWnd;
	CCriticalSection	m_CommCritical;
};


/////////////////////////////////////////////////////////////////////////////
// CMainInfoManager
//#include "../../inc/IMainInfoMng.h"
class CMainInfoManager : public IMainInfoManager
{
public:
	CMainInfoManager();
	~CMainInfoManager();
	
public:	// Implementation
	UINT AFXAPI HashDataAXIS(LPCSTR key);
	STDMETHOD_(LPCSTR, GetDataString)(int nKey) ;
	STDMETHOD_(BOOL   , SetDataString)(int nKey, LPCSTR szData) ;

	STDMETHOD_(LPCSTR, GetUserID)() ;
	STDMETHOD_(LPCSTR, GetEncUserID)() ;

	STDMETHOD_(LPCSTR, GetRootDir)();
	STDMETHOD_(LPCSTR, GetDataDir)();
	STDMETHOD_(LPCSTR, GetEncUserDir)(LPCSTR szKey=NULL);
	STDMETHOD_(CString, GetLoginInfo)(LPCSTR szKey);

	//-->@.환경변수 처럼 관리
	STDMETHOD_(BOOL, SetEnvironmentVariable)(
				LPCTSTR lpName,  // environment variable name
				LPCTSTR lpValue  // new value for variable
			) ;

	STDMETHOD_(DWORD, GetEnvironmentVariable)(
				LPCTSTR lpName,		// environment variable name
				CString &rValue		// CString for variable value
			) ;
	STDMETHOD_(BOOL, FreeEnvironmentStrings)(
				LPTSTR lpName  // environment variable name
			) ;

	STDMETHOD_(DWORD, GetEnvironmentStrings)(CStringList& szBufferList) ;

	STDMETHOD_(int, IniToEnvironmentTable)(LPCSTR szIniPath, LPCSTR szSect);

	STDMETHOD_(int,	GetCountOfAccount)();
	STDMETHOD_(BOOL, GetUserAccount)(CStringArray& saAccount);
	STDMETHOD_(CString, GetAccountName)(LPTSTR lpszAccount);
	STDMETHOD_(CString, GetAccountPass)(LPTSTR lpszAccount);
	//<--

	STDMETHOD_(int, GetHtsMode)();

	STDMETHOD_(CString, GetLoginPass)();

public:	// Variable
//	CMapStringToString	m_MapData; 
	CStringArray		m_MapData;
	CString				m_szTmp;
	CMapStringToString	m_mapEnvData;
//@Solomon090819	CLinkDataManager	m_linkDataMng;
};



/////////////////////////////////////////////////////////////////////////////
// CMainExInfoManager
//#include "../../inc/IMainExInfoMng.h"
typedef struct 
{
	CString szDataKey;
	BYTE*	pData;
	int		nDataLen;
} ST_FOR_UserData;
typedef CList<ST_FOR_UserData*, ST_FOR_UserData*> LIST_ST_FOR_UserData;
class CMainExInfoManager : public IMainExInfoManager3
{
public:
	CMainExInfoManager();
	~CMainExInfoManager();
	void CheckAutoDelete();
	void DeleteImsiGroup(long dwData);
	void DeleteData(int nIndex);
	void PreUnLoad();
	void* SaveImgiGroup(long dwData);
	void fnClearAll();
	
public:	// Implementation
	STDMETHOD_(COleVariant*, GetData)(LPCSTR szKey);
	STDMETHOD_(BOOL , SetData)(LPCSTR szKey, const COleVariant* pvData);

	STDMETHOD_(COleVariant* , GetData)(int dwKey);
	STDMETHOD_(BOOL, SetData)(int dwKey, const COleVariant* pvData);
	

	//{{ DISPID 속도계산을 위한 루틴...
	// #SiteDependency.cpp 에서 구현함...
	STDMETHOD_(LONG, GetDispid)(int nID) ;
	STDMETHOD_(void, SetDispid)(INT nID) ;
	//}}

	//--> IMainExInfoManager2에서 추가된 인터페이스
	STDMETHOD_(LONG, GetUserExData)(LONG nID) ;
	STDMETHOD_(BOOL, SetUserExData)(LONG nID, LONG dwValue) ;
	STDMETHOD_(BOOL, DeleteUserExData)(LONG nID) ;
	STDMETHOD_(BOOL, IsGetUserExData)(LONG nID) ;
	//<--

	//--> IMainExInfoManager3에서 추가된 인터페이스
	STDMETHOD_(BOOL, GetUserStringData)(LPCSTR szKey, CString& rValue);
	STDMETHOD_(BOOL, SetUserStringData)(LPCSTR szKey, LPSTR szData);

	STDMETHOD_(POSITION, GetUserData)(LPCSTR szKey, LPBYTE *pData, int &nDataLen) ;
	STDMETHOD_(BOOL, SetUserData)(LPCSTR szKey, long pData, int nDataLen) ;
	//<--

public:	// Variable
	CMapStringToPtr	m_MapStringData; 
	CPtrArray		m_MapByteData;
	CPtrArray		m_MapDisp;

	CMapPtrToPtr	m_MapPtrToPtr;	// 이 형태로 관리되는 데이터는 사용하는 쪽에서 삭제처리도 해줘야함.
									// IMainExInfoManager2 지원을 위해서 추가됨.
	CMapStringToString	m_MapString_I3;	//IMainExInfoManager3의 GetUserStringData(..), SetUserStringData(..)지원
	LIST_ST_FOR_UserData	m_UserData;
	CMapStringToString	m_UserKeyData;		//GetUserData/SetUserData관련해서 특별히 관리하는 데이타.
	bool _DoSaveUserData(LPCSTR szKey, LPBYTE pData, int nDataLen);
	bool _DoGetUserData(LPCSTR szKey, LPBYTE *pData, int &nDataLen);

	CCriticalSection	m_MainExCritical;
};

// #include "../../inc/IGateMng.h"
class CGateManager : public IGateManager
{
public:
	
public:	// Implementation
	STDMETHOD_(LONG, GetInterface)(int nID, long dwUnUse=0);

public:	// Variable

};

//#include "../../inc/IAUDrdsMng.h"
//#include "../../inc/LoadDrds.h"

typedef CList<IDrdsLib*, IDrdsLib*> LISTDRDSLIB;
class CAUDrdsManager : public IAUDrdsManager
{
public:
	CAUDrdsManager();
	~CAUDrdsManager();
	LISTDRDSLIB  m_List;

public:	// Advise/Unadvise
// Advise, Unadvise
	STDMETHOD_(IDrdsLib*, AdviseDrdsLib)() ;
	STDMETHOD_(void, UnadviseDrdsLib)(IDrdsLib*) ;
};

class CDrdsLib : public IDrdsLib
{
public:
	CDrdsLib();
	~CDrdsLib();

public:	// Implementation
	STDMETHOD_(void*, DrdsInit)(HWND hWnd) ;		
	STDMETHOD_(BOOL, DrdsTerm)() ;
	STDMETHOD_(BOOL, DrdsConnect)(LPSTR szDataName) ;
	STDMETHOD_(BOOL, DrdsClose)(LPSTR szDataName) ;
	STDMETHOD_(BOOL, DrdsAdvise)(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName) ;
	STDMETHOD_(BOOL, DrdsUnadvise)(UINT id, LPSTR szDataName, LPSTR FieldName, LPSTR KeyName) ;

	STDMETHOD_(BOOL, DrdsSetSocketHandle)(HWND hWnd) ;
	STDMETHOD_(void, DrdsSendCode)() ;

	STDMETHOD_(BOOL, AdviseRealCodeData)(BOOL bIsAdd, int nCount, LPSTR sPacketName, LPSTR sCodeList) ;
	STDMETHOD_(int,  SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType);
	STDMETHOD_(BOOL, DrdsAdviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName) ;
	STDMETHOD_(BOOL, DrdsUnadviseEx)(int nComponentType, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName, LPSTR ObjectName) ;
	STDMETHOD_(BOOL, DrdsRequest)(void *Link, HWND hWnd, UINT id, LPSTR DataName, LPSTR FieldName, LPSTR KeyName) ;
	STDMETHOD_(void, DrdsPropertySet)() ;
	STDMETHOD_(void, DrdsTraceOn)() ;
	STDMETHOD_(void, DrdsTraceOff)() ;
	STDMETHOD_(void, DrdsSetFlag)(bool bTrace) ;
	STDMETHOD_(void, UDPCreate)() ;
	STDMETHOD_(void, UDPCreate2)(LPSTR strIP, LPSTR strPort) ;
	STDMETHOD_(void, UDPDestroy)() ;
	STDMETHOD_(void, UdpCreate3)(LPSTR sCfgFullPathName) ;
	STDMETHOD_(int , SetUserDefData)(LPSTR sKeyName, LPSTR sData) ;

	STDMETHOD_(BOOL, Drds_OpenSignalMng)(LPSTR sCfgFullPathName,long lMainHwnd) ;
	STDMETHOD_(int,  Drds_SignalMng_SetNewRealData)(LPSTR pPacket, int nPacketLen, int nComponentType, int nBrocastType) ;
	STDMETHOD_(long, Drds_GetInterface)(UINT nID) ;
	STDMETHOD_(BOOL, Drds_SetInterface)(UINT nID, long dwData) ;
	STDMETHOD_(int, ConvertToRealStruct)(WPARAM wParam, LPARAM lParam, char* pStruct, int nRealType=-1) ;

	STDMETHOD_(BOOL, GetRealStructData)(WPARAM wParam, LPARAM lParam, char*& prValue, int &nRealTpe);

public:	// Variable
	HWND 	m_hHandle;
	void*   m_pDrds;
	//CDrdsLoadLib m_DrdsDLL;

//@Solomon090819	CCommApiHelper*		m_pCommApiHelper;
//@Solomon090819	int					m_nConnectID;
//@Solomon090819	int					m_nRqID;
	
	
//	LONG GetCommRealTranInfo(LPARAM lParam);
//	long GetRealIDData(LPARAM lParam, LONG dwCommRealTranInfo, int nDataID, char* rValue, int& rSize);
//	int Convert_KXH1_01(LPARAM lParam, LONG dwCommRealTranInfo, char* pStruct);
//	int Convert_KXS3_21(LPARAM lParam, LONG dwCommRealTranInfo, char* pStruct);
};

/////////////////////////////////////////////////////////////////////////////
// CSkinColorManager
//#include "../../inc/IRegistryMng.h"

class CRegistryManager : public IRegistryManager
{
public:
	STDMETHOD_(void,   SetStrValue)(HKEY hHkey,LPCSTR strKey, LPCSTR strSubKey, LPCSTR strKeyValue) ;
	STDMETHOD_(LPCSTR, GetStrValue)(HKEY hHkey,LPCSTR strkey, LPCSTR strsubkey, LPCSTR strDefault) ;	
};

//#include "../../inc/IEncryptionMng.h"
// CEncryptionManager
class CEncryptionManager : public IEncryptionManager
{
public:
	STDMETHOD_(void, Encryption)(int key, LPCSTR buff, LPSTR encdata, int len) ;
	STDMETHOD_(void, Decryption)(int key, LPCSTR buff, LPSTR encdata, int len) ;
	STDMETHOD_(void, EncryptCharacterz)(int nKey, char cCharacter, LPSTR szDestination) ;
	STDMETHOD_(void, DecryptCharacterz)(LPSTR szEncCharacter, LPSTR szDestination) ;
	STDMETHOD_(BOOL, EX_EncryptWord)(int nKey, LPSTR szSource, LPSTR szDestination) ;
	STDMETHOD_(BOOL, EX_DecryptWord)(LPSTR szSource, LPSTR szDestination) ;

public:
	int lsrand(int key);
};

class CMDM_SubItemA
{
public:
	CString m_szKey1;
	CString m_szKey2;
	
	BOOL DoFind(LPCSTR szKey, CString &rKey)
	{
		if(strlen(szKey)==3)
		{
			if(m_szKey1.Compare(szKey)==0)
			{
				rKey = m_szKey2;
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			if(m_szKey2.Compare(szKey)==0)
			{
				rKey = m_szKey1;
				return TRUE;
			}
			return FALSE;
		}
	}
};
typedef CList<CMDM_SubItemA*, CMDM_SubItemA*> List_CMDM_SubItemA;

typedef struct 
{
	CString m_szKey;
	BYTE	m_Value;
} ST_STRINGBYTE;
typedef CList<ST_STRINGBYTE*, ST_STRINGBYTE*> List_ST_STRINGBYTE;

typedef struct 
{
	CString m_szKey;
	CString	m_szValue;
} ST_STRINGSTRING;
typedef CList<ST_STRINGSTRING*, ST_STRINGSTRING*> List_ST_STRINGSTRING;

#define		RecentCodeTypeList_Max		10
#define		RecentMarketTypeList_Max	10
#define		RecentGetCodeName_Max		10
#define		RecentGetChxUpCode_Max		10
class CMasterDataManager : public IMasterDataManagerLast
{
protected:
	List_ST_STRINGBYTE m_RecentCodeTypeList;
	BYTE _DoPreGetCodeTypeFromCode(LPCSTR strCode);
	void _DoAddToCodeType(LPCSTR strCode, BYTE bType);

	List_ST_STRINGBYTE m_RecentMarketTypeList;
	BYTE _DoPreChkCodeSB(List_ST_STRINGBYTE* pListST, LPCSTR strCode);
	void _DoAddChkCodeSB(List_ST_STRINGBYTE* pListST, int nMaxCnt, LPCSTR strCode, BYTE bType);

	List_ST_STRINGSTRING m_RecentCodeNameList;
	List_ST_STRINGSTRING m_RecentChxUpCodeList;
	BOOL _DoPreChkCodeSS(List_ST_STRINGSTRING* pListST, LPCSTR strCode, CString &szCodeName);
	void _DoAddChkCodeSS(List_ST_STRINGSTRING* pListST, int nMaxCnt, LPCSTR strCode, LPCSTR szCodeName);

	void ClearListSTRINGBYTE(List_ST_STRINGBYTE* pList);
	void ClearListSTRINGSTRING(List_ST_STRINGSTRING* pList);

public:
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, LPCSTR szData) ;
	STDMETHOD_(void, SetKeyData)(LPCSTR szKey, long   dwData) ;

	// {{ Code.ocx
	STDMETHOD_(long, GetBaseCodeTable)();
	STDMETHOD_(void, SetInfo)(long _dwInfo);
	STDMETHOD_(long, ShowCodeTable)(LPCTSTR _szCodeKey, const VARIANT& _hWnd);
	STDMETHOD_(long, ShowModeCodeTable)(LPCTSTR _szCodeKey, const VARIANT& _hWnd);
	STDMETHOD_(void, SetDataString)(short nKey, LPCTSTR szData);
	STDMETHOD_(BOOL, InitData)(LPCTSTR szRootPath);
	STDMETHOD_(CString, GetCodeInfo)(LPCTSTR _szCodeKey, short _nFlag, LPCTSTR _szKey);
	STDMETHOD_(void, SetPosition)(LPCTSTR szKey, short nX, short nY);
	STDMETHOD_(CString, GetMaxHangSaGa)();
	STDMETHOD_(long, ShowCodeTableWithPosition)(LPCTSTR _szCodeKey, const VARIANT& _hWnd, short nX, short nY);
	STDMETHOD_(BOOL, RemoveAllListData)();
	STDMETHOD_(BOOL, InsertHistory)(short nMaxCount, long nType, LPCTSTR Code, LPCTSTR CodeName);
	STDMETHOD_(CString, GetHistoryList)(short nMaxCount, long nType);
	STDMETHOD_(long, GetGlobalDataTypeList)();
	STDMETHOD_(long, GetCodePointer)(LPCTSTR _szCodeKey);	
	// }} Code.ocx

	// 주식종목마스터에서 업종에 해당하는 종목구하기
//	STDMETHOD_(int, GetJMaster_Upcode)(LPCTSTR szUpcode, long pReturnList) ;

	// 코스닥종목마스터에서 업종에 해당하는 종목구하기
//	STDMETHOD_(int, GetKMaster_Upcode)(LPCTSTR szUpcode, long pReturnList) ;

	// Koscom 옵션마스터데이터 구하기
	STDMETHOD_(BOOL, GetOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, 
										CStringArray *&pastrJongEngName, CStringArray *&pastrEventPrice, 
										CStringArray *&pastrFinalYM, CStringArray *&pastrValueGubun ) ;

	// Koscom 선물마스터데이터 구하기
	STDMETHOD_(BOOL, GetFutureJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;

	// 코스피업종마스터데이터 구하기
	STDMETHOD_(BOOL, GetKSPUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) ;

	// 코스닥업종마스터마스터데이터 구하기
	STDMETHOD_(BOOL, GetKSQUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) ;

	// 해외주요지수 마스터데이터 구하기
	STDMETHOD_(BOOL, GetFRMst)(CStringArray *&pastrCode, CStringArray *&pastrName) ;

	// 해외국가 마스터데이터 구하기
	STDMETHOD_(BOOL, GetFRNationMst)(CStringArray *&pastrCode, CStringArray *&pastrName) ;

	// ELW 마스터데이터 구하기
	STDMETHOD_(BOOL, GetELWMst)(CStringArray *&pastrCode, CStringArray *&pastrName) ;

	// 그룹사 마스터데이터 구하기
	STDMETHOD_(BOOL, GetGroupMst)(CStringArray *&pastrCode, CStringArray *&pastrName, int nOption=0) ;
	
	// KRX업종 마스터데이터 구하기
	STDMETHOD_(BOOL, GetKRXUpMst)(CStringArray *&pastrUpCode, CStringArray *&pastrUpName, CStringArray *&pastrUpEngName, int nOption=0) ;
	
	// 종목상세정보 구하기
	STDMETHOD_(CString, GetStockDetailInfo)(CString strCode, int nType) ;

	// 마지막 종목코드 구하기
	STDMETHOD_(CString, GetLastCode)(CString strMarketCode) ;

	// 미니 선물 마스터데이터 구하기
	STDMETHOD_(BOOL, GetMiniFutureJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;

	// 미니 옵션 마스터데이터 구하기
	STDMETHOD_(BOOL, GetMiniOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName ) ;

	//{{ For JMaster/KMaster
//	STDMETHOD_(int,  LoadMASTER_J_K)();
//	STDMETHOD_(void, UnLoadMASTER_J_K)();
//	STDMETHOD_(CMaster_J_K*, J_K_GetMaster)();
// 	STDMETHOD_(int, J_K_JMasterCount)() ;
// 	STDMETHOD_(int, J_K_KMasterCount)() ;
//	STDMETHOD_(CMaster_J_K*, J_K_SortByName)();
//	STDMETHOD_(CMaster_J_K*, J_K_SortByCode)();
	//}} For JMaster/KMaster
	
//	STDMETHOD_(void, CodeSort)(LPCSTR szMaster, void* pList ,int nCount);

// 	STDMETHOD_(BOOL, IsKOSPI) (LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsKosdaq)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsStock) (LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsFuture)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsSpread)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsStockOption)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsOption)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsCall)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsPut)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsKP200)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, IsKQ50)(LPCSTR szCode);
// 	STDMETHOD_(BOOL, CheckKP200Sect)(LPCSTR szCode);
	// Koscom 코스피업종마스터데이터 구하기
	STDMETHOD_(BOOL, GetK200UpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
	STDMETHOD_(BOOL, GetK100UpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
	STDMETHOD_(BOOL, GetKSTRUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
	STDMETHOD_(BOOL, GetFREEUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) ;

	// 해외
	STDMETHOD_(BOOL, GetForeignMst)(int nKind, CString strCode, CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
	// FX
	STDMETHOD_(BOOL, GetFXMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName) ;
// 	STDMETHOD_(BOOL, GetUSUPMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetDow30Master)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetNasdaq100Master)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetSP500Master)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetCMEFutMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetETCFutMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetMoneyRateMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetLiborRateMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetADRMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetGDRMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetGovMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetSemiMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;
// 	STDMETHOD_(BOOL, GetChangeRateMaster)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName) ;

	// 선물,옵션 종목코드 만기월지났을 경우 확인을 위해서 ...
	// 만기월 지나면 FALSE이고 rszCode에 가장 최근월물 리턴.
	// 만기월이 안지났으면 TRUE리턴하고 rszCode에 szCode를 그대로 넘겨줌.
	STDMETHOD_(BOOL, GetRecentCode)(CString szCode, CString& rszCode, int nCodeType);
//	STDMETHOD_(BOOL, GetMasterData)(CString szType, CStringArray &arrCode, CStringArray &arrName);
	STDMETHOD_(BOOL, GetMasterDataInUpDetails)(CString szType, CStringArray &arrCode, CStringArray &arrName, CString strKey);
	STDMETHOD_(BYTE, GetMarketTypeFromCode)(LPCSTR strCode) ;
//	STDMETHOD_(CString, GetJongmokNameFromCode)(CString strCode) ;
	STDMETHOD_(BOOL, GetRecentCodeNameArray)(int nMarketType, CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetAllStock)(CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetStockCodeWithType)(int nType, CStringArray &arrCode, CStringArray &arrName);		// 2008.11.11 -hidden		
	STDMETHOD_(BOOL, GetJFMaster)(CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetJPMaster)(CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetWPMaster)(CStringArray& arrCode, CStringArray& arrName);
//	STDMETHOD_(BOOL, GetFreeBoardMst)(CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetELWIndexMaster)(CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetELWIssueMaster)(CStringArray &arrCode, CStringArray &arrName) ;
	STDMETHOD_(BOOL, GetELWIndexCode)(CString strELWCode, CString& strIndexCode, CString& strIndexName) ;
	STDMETHOD_(CString, GetCodeName)(LPCTSTR lpszItemCode) ;

	STDMETHOD_(CString, GetItemExpCode)(LPCTSTR lpszCode) ;

	STDMETHOD_(BOOL, GetThemeGroup)(CStringArray& arCode, CStringArray& arName);
	STDMETHOD_(BOOL, GetThemeItemList)(LPCTSTR szThemeCode, CStringArray &arrCode, CStringArray &arrName);

	STDMETHOD_(BOOL, GetSiteUpKode)(LPCSTR szCode, CString& rCode);

	STDMETHOD_(BYTE, GetCodeTypeFromCode)(LPCSTR strCode);

//	STDMETHOD_(BOOL, GetFOFLMaster)(CStringArray &arrCode, CStringArray &arrName) ;
//	STDMETHOD_(BOOL, GetFOFLSubMaster)(CStringArray &arrCode, CStringArray &arrName) ;

	// 코넥스업종마스터마스터데이터 구하기
	STDMETHOD_(BOOL, GetKNXUpMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0) ;
	STDMETHOD_(BOOL, GetETNMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0);
	STDMETHOD_(BOOL, GetVFutureMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0);
	STDMETHOD_(BOOL, GetSFutureMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName, CStringArray *&pastrJongEngName, int nOption=0);


	// 미니 옵션 마스터데이터 구하기
	STDMETHOD_(BOOL, GetWeeklyOptionJongMst)(CStringArray *&pastrJongCode, CStringArray *&pastrJongName );


public:  // Variable
	CMasterDataManager();
	~CMasterDataManager();
	// CCodeOcx*		m_pCodeOcx;
//	CDispCodeOcx	m_CodeOcx;

	//CMaster_J_K* m_ppJKArray;
// 	int		m_nJ_K_Count,
// 			m_nJ_K_Count_J,
// 			m_nJ_K_Count_K;
//	static  int  _SortName(const void* p1, const void* p2);
//	static  int  _SortCode(const void* p1, const void* p2);
//	void  CodeSort_JMaster(void *, size_t, size_t, int (__cdecl *)     (const void *, const void *));

	//HWND m_hWinixWnd;	//@Solomon

//@Solomon090819	IItemMaster		m_itemMaster;
//@Solomon090819	IItemMaster*	GetItemMaster()		{ return &m_itemMaster; }
	HWND GetHWND(); //@Solomon
	HWND GetCodeMasterHWND();
	HWND m_hCodeMasterWnd;

protected:	//For Solomon	
	void _DoParsingCode(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert=false, int nStartIdx=0);
	void _DoParsingCodeForeign(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);
	void _DoParsingCode2(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);
	void _DoParsingCodePMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);
	void _DoParsingCodeJPMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);	//주식옵션용.
	void _DoParsingCodeDPMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);	//20100319_JS.Kim FX용
	void _DoParsingCodeFX(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName, bool bInsert=false, int nStartIdx=0);
	void _DoParsingCodeMiniMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);
	void _DoParsingCodeWeeklyMaster(CString& szCodeList, CStringArray &arrCode, CStringArray &arrName);
	void _DoSortCode(CStringArray &arrCode, CStringArray &arrName);
	BOOL _DoGetOldFutureMaster(CStringArray &arrCode, CStringArray &arrName);

	List_CMDM_SubItemA m_UpKeyList;
	void _DoClearList();	

	CMapStringToString	m_RecentFutureCodeTable;
};


// 기존에 Util.h와 Util.cpp에 있던 내용을 그대로 가져옴.
class CDataUtilManager : public IDataUtilManagerLast
{
public:
	// 소수점 형식으로
	STDMETHOD_(CString, GetFutureString)(CString strData, BOOL bJisu);
	STDMETHOD_(void, AddDashToAccount)(LPCSTR _lpIn, CString& szOut);
	STDMETHOD_(CString, GetSignSimbol)(int sign);
	STDMETHOD_(BOOL, GetRegKey)(LPCSTR szSection, LPCSTR szKey, CString& rData);
	STDMETHOD_(CString, AdjustMask)(CString mask, int nValue);
	STDMETHOD_(CString, AdjustMask)(CString mask,CString str);
	STDMETHOD_(void, TrimRight)(char* pIn);
	STDMETHOD_(void, RemoveCharAll)(CString& szData, TCHAR ch);
	STDMETHOD_(BOOL, ValidNumber)(LPCSTR strIn);
	STDMETHOD_(BOOL, IsSign)(LPCSTR pData);
	STDMETHOD_(void, GetWord)( LPSTR, LPSTR, TCHAR );
	STDMETHOD_(int, GetString)( CString&, CString&, TCHAR );
	STDMETHOD_(BOOL, IsNumber)(UINT nChar);
	STDMETHOD_(BOOL, IsOnlyNumber)( LPCTSTR szData);
	STDMETHOD_(int, Remove)(CString &strIn, TCHAR ch, int nIndex=1); 

	// nIndex -1:All , 0: noremove , 몇번째 단어가 제거 될것인지
	STDMETHOD_(int, RemoveAll)(CString &strIn, TCHAR ch=' ');
	STDMETHOD_(char*, RemoveFront)(LPCSTR, TCHAR);
	STDMETHOD_(CString, GetComma)(LPCSTR szData);
	STDMETHOD_(CString, GetOnlyNumber)( LPCSTR szData );
	STDMETHOD_(CString, GetNumber)(LPCSTR szData );
	STDMETHOD_(CString, GetMakeText)(TCHAR ch, int nCnt);
	STDMETHOD_(CString, SetLengthString)(LPCTSTR pData, int nCnt);
	STDMETHOD_(CString, RemoveChar)(LPCSTR pData, TCHAR ch);
	
	// {{ -- 추가 2005.12.03(Sat)
	STDMETHOD_(int, Str2Int)(CString& str);
	STDMETHOD_(long, Str2Long)(CString& str);
	STDMETHOD_(CRect, Str2Rect)(CString& str);
	STDMETHOD_(COLORREF, Str2RGBColor)(CString& str);
	STDMETHOD_(COLORREF, Str2RGBColor)(char* pchData);
	STDMETHOD_(CString, Color2String)(COLORREF color);
	STDMETHOD_(BOOL, Str2Boolean)(CString& str);
	STDMETHOD_(CSize, Str2Size)(CString& str);
	STDMETHOD_(void, String2Olechar)(LPCSTR _szFrom, OLECHAR& szTo, int nLen);
	STDMETHOD_(LOGFONT, Str2LogFont)(CString& str);
	// }} -- 추가 2005.12.03(Sat)

	STDMETHOD_(CString, ConvertIntToPoint)(CString strData, int nPointPosition, int nPointCurPosition=-1) ;
	STDMETHOD_(CString, ConvertPointToInt)(CString strData,int nPointPositoin) ;
	STDMETHOD_(void, InsertComma)(CString & str1) ;

	STDMETHOD_(int, chars_atoi)(LPSTR lpstrSrc, UINT uSize);
	STDMETHOD_(double, chars_atof)(LPSTR lpstrSrc, UINT uSize);
	STDMETHOD_(CString, DoubleToStrComma)(double dVal, int nSosu, BOOL bShowZero=TRUE);
	STDMETHOD_(CString, IntToStrComma)(int nVal, BOOL bShowZero=TRUE);
	STDMETHOD_(int, Str2CommaStr)(LPSTR dest, LPSTR src, UINT uSize);
	STDMETHOD_(void, StrToStrComma)(LPCTSTR lpstrSrc, CString & strReturn);

	STDMETHOD_(CString, GetStringData)(CString& data, const CString& trimStr, const bool bIsInside = true);
	STDMETHOD_(int, GetFillString)(CString& szGetStr, int nStart, int nEnd, int nOneWordSize);

	STDMETHOD_(int, DoParsing)(char cParser, LPTSTR pData, int &nPos, int nEndPos, char* aString);
};


//////////////////////////////////////////////////////////////////////////
// CAccountManager
// #include "../../inc/IAccountMng.h"
//-----------------------------------------------------------------------------
// Author		: ojtaso 	Date : 2005/11/30
// Explain		: 기존에 사용하던 CAcctHelper 클래스를 인터페이스로 변경
// Using way	: 메인에서 계좌정보를 세팅하고 dll화면에서 사용
//-----------------------------------------------------------------------------

//#include "../../../CONTENTS/INCLUDE_CONTENTS/Account.h"

//class CAccountManager : public IAccountManager
//{
//public:
//	CList<CAcct*, CAcct*> m_List;
//
//public:
//	CAccountManager();
//	~CAccountManager();
//
//public:
//	STDMETHOD_(void, ClearAll)();
//	STDMETHOD_(void, Add)(CString _szData, BOOL& bSavePass);
//	STDMETHOD_(int, GetKeyString)(CString _szKey, CString& rString);
//	STDMETHOD_(int, GetKeyString2)(CString _szKey, CString rOrigin, CString& rString);
//	STDMETHOD_(int, GetAllString)(CString& rString);
//	STDMETHOD_(int, GetAllString2)(CString& rString);
//	STDMETHOD_(CString, GetKey)(CString szAcct);
//	STDMETHOD_(void, MoveToHead)(CString _szKey, CString szAcct, CString& rString, BOOL bAddValue);
//	STDMETHOD_(CString, GetAcctPasswd)(CString szAcct);
//	STDMETHOD_(void, SetAcctPasswd)(CString szAcct, CString szPasswd);
//	STDMETHOD_(void, Copy)(LPVOID pOrig);
//};

//////////////////////////////////////////////////////////////////////////
// CAUBGManager
// #include "../../inc/IAUBGMng.h"
class CBKInfo
{
public:
	CString   m_szMoudle;
	HINSTANCE m_hLib;
	int		  m_nRefer;

	CBKInfo();
	~CBKInfo();

	HINSTANCE LoadLib();
	void UnLoadLib();
	HINSTANCE GetInterface(int nKey, long &pInterface);
	long GetInterface(int nKey);
	BOOL SetInterface(int nKey, long pInterface);
	int  GetReference() { return m_nRefer;	}
	void Terminate();

protected:
	BKMNG_GetInterface m_fnGetInterface;
	BKMNG_SetInterface m_fnSetInterface;
	BKMNG_Terminate    m_fnTerminate;

	int AddRefer();
	int ReleaseRefer();
};

typedef CList<CBKInfo*, CBKInfo*> List_CBKInfo;

class CAUBGManager : public IAUBGManager
{
public:
	STDMETHOD_(HINSTANCE, IsBackGround)(LPCSTR szModuleName) ;

	STDMETHOD_(HINSTANCE, AddBackGround)(LPCSTR szModuleName) ;

	// hLib 리턴값도 동일.
	STDMETHOD_(HINSTANCE, AddBackGround)(HINSTANCE hLib) ;

	// 
	STDMETHOD_(HINSTANCE, DelBackGround)(LPCSTR szModuleName) ;

	// hLib 리턴값도 동일.
	STDMETHOD_(HINSTANCE, DelBackGround)(HINSTANCE hLib) ;
	
	// DLL과 약속한 인터페이스를 넘겨준다.
	// DLL이 로드가 안되어 있으면 DLL을 로드한다.
	// nKey에 해당하는 인터페이스를 pInterface에 리턴한다.
	STDMETHOD_(HINSTANCE, GetInterface)(LPCSTR szModuleName, int nKey, long &pInterface) ;

	// hLib를 알고 있을 때..
	// nKey에 해단하는 정보를 받는다.
	STDMETHOD_(long, GetInterface)(HINSTANCE hLib, int nKey) ;

	// hLib를 알고 있을 때..
	// nKey와 pInterface 조합으로 데이터를 넘겨준다.
	STDMETHOD_(BOOL, SetInterface)(HINSTANCE hLib, int nKey, long pInterface) ;

public:
	CAUBGManager();
	~CAUBGManager();
	void UnLoadLibAll();
	void ClearAllList();
	CBKInfo* FindModule(LPCSTR szModule);
	CBKInfo* FindModule(HINSTANCE hLib);
	void PreUnLoad();

	List_CBKInfo  m_List;
};

//////////////////////////////////////////////////////////////////////////
// COrderMonitorManager
//#include "../../inc/IAUOrderMonitorMng.h"
class COrderMonitorManager : public IOrderMonitorManager
{
public:
	STDMETHOD_(BOOL, SetState)(DSBM_RUNSTATE* pState, long dwLong=NULL) ;

	// 조건의 동작상태를 나타낸다.
	STDMETHOD_(BOOL, SetCondition)(DSBM_RUN* pState, long dwLong=NULL) ;

	// 주문로그를 남긴다.
	STDMETHOD_(BOOL, SetTradeLog)(DSBM_TRADELOG* pState, long dwLong=NULL) ;	

public:
	COrderMonitorManager();
	HWND  GetHandle();
	HWND  CheckOpenMoniterServer();
	HWND  m_hATHwnd;  // 주문주문 모니터링의 핸들.
	CString m_szMapID;
};

// IAUGwanManager
// 관심그룹 관련 인터페이스.
// 	class CAUGwanManager : public IAUGwanManager
// 	{
// 	// public:
// 		STDMETHOD_(long, Advise)(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc);
// 		STDMETHOD_(void, UnAdvise)(long dwRet);
// 		STDMETHOD_(void, NotifyData)(LPCSTR szKey, long dwSubKey, long dwNotifyData);
// 
// 	public:
// 
// 		STDMETHOD_(long, GetGwansimDataAll)(int nType, LPCSTR szGroup=NULL);
// 
// 		STDMETHOD_(void, GetGwansimGroupName)(CStringArray &arrKey, CStringArray &arrName);
// 		STDMETHOD_(void, GetGwansimItem)(CString strKey, CStringArray& arrCode, CStringArray& arrName);
// 		STDMETHOD_(int,  ShowGwansimDlg)(CWnd* pCaller, CString strCodeList="");	
// 		STDMETHOD_(BOOL, ADDGwasimItem)(CString strGroup, CString strItem, BOOL bFirst=FALSE);
// 	//	STDMETHOD_(BOOL, ADDGwasimItems)(CString strGroup, CStringArray& arrItem);
// 		STDMETHOD_(int,  SetRegGwansim)(CWnd* pComboWnd, int nType);
// 		STDMETHOD_(int,  RegiestToImsiGroup)(CStringArray& arrCode, CWnd* pWnd);
// 
// 		//그룹의 키값리턴
// 		STDMETHOD_(LPCSTR, GetGwanKeyFromName)(LPCSTR szGwanGroupName);
// 		STDMETHOD_(int,  ShowGwansimDlgWithList)(CWnd* pCaller, CStringArray& arrItem);
// 
// 	public:
// 		CAUGwanManager();
// 		~CAUGwanManager();
// 
// 		//void		ConnectGwansimLib(BOOL bReconnect=FALSE);
// 	//	BOOL		CreateGwanWnd();
// 	//	void		PreUnLoad();
// 	//	CGwanWnd	m_GwanWnd;
// 	//@Solomon090819	CIntrApiHelper m_intrMngr;
// 
// 		//@Solomon-->
// 		IAUGwanManager* m_pWinixGwanMng;
// 		long _GetInnerInterface();
// 		//@Solomon<--
// 	};


//////////////////////////////////////////////////////////////////////////
// CSignalManager
//#include "../../inc/ISignalMng.h"

class CSignalManager : public ISignalManager
{
public:
	CSignalManager();
	void LoadProcAddress();

	HMODULE	m_hSignalLib;
	FpSetLibOpen		m_fpSetLibOpen;
	FpSetLibClose		m_fpSetLibClose;
	FpGetSignalData		m_fpGetSignalData;
	FpSetSignalData		m_fpSetSignalData;
	FpGetFilterData		m_fpGetFilterData;
	FpSetFilterData		m_fpSetFilterData;
	FpConvertRealData	m_fpConvertRealData;

public:
	//# DRDS에서 Signal DLL을로드한 후 세팅하는 경우
	STDMETHOD_(BOOL, SetLibHandle)(HMODULE hLib);

	//# Open / Close! => 반드시 Open 후 Colse!
	STDMETHOD_(int, _DLL_SetLibOpen)(CWnd* pwndMain) ;
	STDMETHOD_(void, _DLL_SetLibClose)();

	//# 신호 설정 관련 함수!
	STDMETHOD_(void, _DLL_GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet);
	STDMETHOD_(void, _DLL_SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet);

	STDMETHOD_(void, _DLL_GetFilterData)(long& stFilterInfo);
	STDMETHOD_(void, _DLL_SetFilterData)(long* pstFilterInfo);

	//# DRDS 외부 함수
	STDMETHOD_(int, ConvertRealData)(int nPacketName, LPSTR &sRealData);
};


//////////////////////////////////////////////////////////////////////////
// CAUPCOrderManager
//#include "../../inc/IPCOrderMng.h"
class CAUPCOrderManager : public IAUPCOrderManager
{
public:
};

//////////////////////////////////////////////////////////////////////////
// CDebugManager
//#include "../../inc/IDebugMng.h"
//#include "Tracer.h"
class CDebugManager : public IDebugManagerLast
{
protected:
	CDRDebugerLoader*	m_pDRDebuger;
public:
	STDMETHOD_(void, Trace)(LPCTSTR lpszFormat, ...);
	STDMETHOD_(void, Trace)(int nMsgKey, LPCTSTR lpszFormat, ...);
	
	STDMETHOD_(void, WriteLogFile)(void* sData, int nDataLen, LPCSTR szFileName, BOOL bInsertCR=TRUE, LPCSTR szLogDir="c:\\Logs");
	STDMETHOD_(void, TraceEx)(BYTE cMessageTag, BYTE cIO, LPCSTR pDataName, DWORD lDataSize, LPBYTE pData);
	
	STDMETHOD_(void, TraceTran)(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData);
	STDMETHOD_(void, TraceReal)(LPCSTR pRealName, DWORD dwSize, LPBYTE pData);
	STDMETHOD_(void, TraceSR)(BYTE cAU, BYTE cIO, LPCSTR pTranName, HWND hReceiver, WORD dwOnCopyDataKey);

	STDMETHOD_(void, ShowOutputDebugString)(LPCTSTR lpszFormat, ...);

	//Data 비교Tool를 위한 기능
	STDMETHOD_(BOOL, TraceCompData)(BYTE cIO, LPCSTR pTranName, DWORD dwSize, LPBYTE pData) ;

	//IDebugManager3
	STDMETHOD_(LONG, GetDRDebuger());

public:
	CDebugManager();
	~CDebugManager();
	
	void DoMessageToDebugWnd(BYTE nType, BYTE cIO, LPCSTR pDataName, DWORD dwSize, LPBYTE pData);

public: //Variable
	BOOL DoMessageToCompDataWnd(BYTE nType, BYTE cIO, LPCSTR pDataName, DWORD dwSize, LPBYTE pData);
//<--
};

//////////////////////////////////////////////////////////////////////////
// CDMMngInfo 
// #include "../../inc/IAUDataMonitorMng.h"
class CDMMngInfo
{
public:
	CString m_szKey;
	long	m_pThisPoint;
	long	m_dwKey;
	DataMonitorCallbackProc m_pCallFunc;

public:
	CDMMngInfo();
	CDMMngInfo(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pFunc);
};
typedef CList<CDMMngInfo, CDMMngInfo> LIST_CDMMngInfo;

//////////////////////////////////////////////////////////////////////////
// CAUDataMonitorManager 
class CAUDataMonitorManager : public IAUDataMonitorManager
{
public:
	STDMETHOD_(long, Advise)(LPCSTR szKey, long pThisPoint, long dwKey, DataMonitorCallbackProc pCallFunc);
	STDMETHOD_(void, UnAdvise)(long dwRet);

	STDMETHOD_(void, NotifyData)(LPCSTR szKey, long dwSubKey, long dwNotifyData);

public:
	CAUDataMonitorManager();
	~CAUDataMonitorManager();
	void ClearListAll();

	LIST_CDMMngInfo	m_List;
};


/////////////////////////////////////////////////////////////////////////////
// CClipboardManager
class CClipboardManager : public IClipboardManager
{
public:
	STDMETHOD_(BOOL ,IsDeskClipboardFormatAvailable) (UINT uFormat) ;
	STDMETHOD_(BOOL ,GetData)(UINT uFormat, LPVOID lpszBuffer ,int nBufSize ,HWND &hWnd) ;
	STDMETHOD_(BOOL ,GetDataLength)(UINT uFormat, unsigned long *pnSize, HWND &hWnd) ;	
	STDMETHOD_(BOOL ,SetData)(UINT uFormat, LPVOID lpszBuffer, unsigned long nSize, HWND &hWnd) ;
	STDMETHOD_(BOOL ,Empty)(UINT uFormat, HWND &hWnd) ;
};

/////////////////////////////////////////////////////////////////////////////
// CFormOpenManager
class CFormOpenDispManager : public IAUFormOpenDispManager
{
public:
	//CDispFormOpen*
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;

	STDMETHOD_(void ,ReLoadMap)(LONG dwKey ,LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	STDMETHOD_(void ,OpenMap)(LONG dwKey ,LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)  ;
	STDMETHOD_(void ,LoadMap)(LONG dwKey ,LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen)  ;
	STDMETHOD_(void ,DialogMap)(LONG dwKey ,LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen);
	STDMETHOD_(void ,PopupMap)(LONG dwKey ,LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) ;
	STDMETHOD_(void ,CloseMap)(LONG dwKey ) ;
	STDMETHOD_(void ,SinglePopupMap)(LONG dwKey ,LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen) ;

public:
};


/////////////////////////////////////////////////////////////////////////////
// CFormDataManager
class CFormDataDispManager : public IAUFormDataDispManager
{
public:
	//CDispFormData*
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;

	STDMETHOD_(BOOL ,SaveData)(LONG dwKey ,LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue) ;
	STDMETHOD_(BOOL ,LoadData)(LONG dwKey ,LPCTSTR _szFileName, LPCTSTR _szKey, const VARIANT FAR& _szValue) ;
	STDMETHOD_(BOOL ,SaveIniData)(LONG dwKey ,LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, const VARIANT FAR& _szValue) ;
	STDMETHOD_(BOOL ,LoadIniData)(LONG dwKey ,LPCTSTR _szFileName, LPCTSTR _szSection, LPCTSTR _szKey, VARIANT FAR* _szValue) ;
	STDMETHOD_(BOOL ,SaveStringDataToCfg)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szValue) ;
	STDMETHOD_(BOOL ,SaveIntDataToCfg)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szKey, short nValue) ;
	STDMETHOD_(BOOL ,LoadIntDataInCfg)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szKey, short nDefault, VARIANT FAR* nValue) ;
	STDMETHOD_(CString ,GetMapCfgFile)(LONG dwKey ) ;
	STDMETHOD_(CString ,GetUserDataPath)(LONG dwKey ,LPCTSTR szKey) ;
	STDMETHOD_(short ,WritePrivateDataString)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile) ;
	STDMETHOD_(CString ,GetPrivateDataString)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile) ;
	STDMETHOD_(void ,WritePrivateDelete)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szFile) ;
	STDMETHOD_(CString ,LoadStringDataInCfg)(LONG dwKey ,LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault) ;
};


/////////////////////////////////////////////////////////////////////////////
// CDeskManager
class CDeskDispManager : public IAUDeskDispManager
{
public:
	//CDeskManager*
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;

	STDMETHOD_(short ,SendTR)(LONG dwKey ,LPCTSTR szTrName) ;
	STDMETHOD_(short ,ReceiveTR)(LONG dwKey ,LPCTSTR szTrName, long szData, short nDataLen) ;
	STDMETHOD_(void ,SetSocketHandle)(LONG dwKey ,LPCTSTR szSocket, long dwHandle) ;
	STDMETHOD_(long ,GetSocketHandle)(LONG dwKey ,LPCTSTR szSocket) ;
	STDMETHOD_(long ,SetSharedInterface)(LONG dwKey ,long dwInterface) ;
	STDMETHOD_(void ,UnLoadScript)(LONG dwKey) ;
	STDMETHOD_(void ,Blank)(LONG dwKey) ;
	STDMETHOD_(void ,ShowTrMessage)(LONG dwKey ,LPCTSTR _szMessage) ;
	STDMETHOD_(short ,ShowMessageBox)(LONG dwKey ,short _nType, LPCTSTR _szMessage) ;
	STDMETHOD_(CString ,GetTrList)(LONG dwKey) ;
	STDMETHOD_(CString ,GetMapName)(LONG dwKey) ;
	STDMETHOD_(BOOL ,GetMapInfo)(LONG dwKey ,LPCTSTR _szKey, VARIANT FAR* _vData) ;
	STDMETHOD_(long ,GetCtrlHwndFromName)(LONG dwKey ,LPCTSTR _szCtrlName) ;
	STDMETHOD_(CString ,GetCtrlNameFromHwnd)(LONG dwKey ,long _hWnd) ;
	STDMETHOD_(CString ,GetCtrlTypeFromHwnd)(LONG dwKey ,long _hWnd) ;
	STDMETHOD_(CString ,GetCtrlTypeFromName)(LONG dwKey ,LPCTSTR _szCtrlName) ;
	STDMETHOD_(BOOL ,CallFunction)(LONG dwKey ,LPCTSTR _szFunction, LPCTSTR _szParam, short _nParamLen) ;
	STDMETHOD_(void ,SetParentDispatch)(LONG dwKey ,LPDISPATCH _pDisp) ;
	STDMETHOD_(LPDISPATCH ,GetDispFromName)(LONG dwKey ,LPCTSTR _szCtrlName) ;
	STDMETHOD_(CString ,GetRootPath)(LONG dwKey ,LPCTSTR _szSubDir) ;
	STDMETHOD_(BOOL ,GetFormTrData)(LONG dwKey ,LPCTSTR _szTr, BSTR* _szData, short* _nDataLen) ;
	STDMETHOD_(BOOL ,SetFormTrData)(LONG dwKey ,LPCTSTR _szTr, LPCTSTR _szData, short _nDataLen) ;
	STDMETHOD_(void ,FormUnLoad)(LONG dwKey) ;
	STDMETHOD_(void ,SetOpenMapData)(LONG dwKey ,short nMapType, LPCTSTR szMap, LPCTSTR szOpenData, short nDataLen, LPCTSTR aOpener) ;
	STDMETHOD_(BOOL ,SendDataToOpener)(LONG dwKey ,LPCTSTR szData, short nDataLen) ;
	STDMETHOD_(void ,CallCommand)(LONG dwKey ,LPCTSTR szCmd) ;

	// vData 에 vData->vt = VT_CString 속성으로 데이터를 얻어온다.
	// vData->CStringVal 값에 실제 데이터 들어감.
	// 출력값long은 데이터의 길이.
	STDMETHOD_(CString ,GetOpenMapData)(LONG dwKey) ;
	STDMETHOD_(void ,SetOpenMapData2)(LONG dwKey ,long dwClipFormat) ;
	STDMETHOD_(void ,NotifyFromMain)(LONG dwKey ,LPCTSTR szKey, LPCTSTR szData) ;
	STDMETHOD_(BOOL ,EnableFocus)(LONG dwKey ,LPCTSTR szCtrlName, short bEnable) ;
	STDMETHOD_(void ,GotoFocus)(LONG dwKey ,LPCTSTR szCtrlName) ;
	STDMETHOD_(void ,SetKeyData)(LONG dwKey ,LPCTSTR szKey, long dwData, short nCount) ;
	STDMETHOD_(long ,GetKeyData)(LONG dwKey ,LPCTSTR szKey, short FAR* nCount) ;
	STDMETHOD_(void ,SetGSharedNotifyFlag)(LONG dwKey ,short bNofity) ;
	STDMETHOD_(BOOL ,LoadXfmEx)(LONG dwKey ,long dwLoadInfo) ;
	STDMETHOD_(void ,ResizeFont)(LONG dwKey ,short nWidth, short nHeight, LPCTSTR szFontName) ;
	STDMETHOD_(BOOL ,GetMapSize)(LONG dwKey ,VARIANT FAR* vrWidth, VARIANT FAR* vrHeight) ;
	STDMETHOD_(void ,SetInitFont)(LONG dwKey ,short nWidth, short nHeight, LPCTSTR szFontName) ;
	STDMETHOD_(void ,SetTabStop)(LONG dwKey ,short bNew) ;
	STDMETHOD_(void ,NotifyGShareData)(LONG dwKey ,LPCTSTR szKey, LPCTSTR szValue) ;
	STDMETHOD_(short ,SetSendCountFlag)(LONG dwKey ,short nNew) ;
	STDMETHOD_(void ,DebugTrData)(LONG dwKey ,LPCTSTR szFileName) ;
	STDMETHOD_(short ,SendTrDirect)(LONG dwKey ,LPCTSTR szTrName, LPCTSTR szData, short nDataLen) ;
	STDMETHOD_(void ,SetFlag)(LONG dwKey ,LPCTSTR szKey, short szValue) ;
	STDMETHOD_(void ,ChangeTRInOut)(LONG dwKey ,LPCTSTR szTRName, LPCTSTR szInData, LPCTSTR szOutData, short nFlag) ;
	STDMETHOD_(OLE_COLOR ,GetUserColor)(LONG dwKey ,short nIndex, LPCTSTR szName, short nFlag) ;
	STDMETHOD_(CString ,GetCurrentTrMessage)(LONG dwKey) ;
	STDMETHOD_(CString ,GetCurrentMsgCode)(LONG dwKey) ;
	STDMETHOD_(short ,SelectPacketCommand)(LONG dwKey ,short nNew) ;
	STDMETHOD_(void ,SetRscMng)(LONG dwKey ,long _dwKey) ;
	STDMETHOD_(void ,MainKeyPress)(LONG dwKey ,LPCTSTR szKey) ;
	STDMETHOD_(void ,SetMonitoringHandle)(LONG dwKey ,long pMonitoringHandle) ;
	STDMETHOD_(long ,AddUserScriptItem)(LONG dwKey ,LPCTSTR pstrItemName, LPDISPATCH pIDispatch) ;
	STDMETHOD_(long ,ExcuteScript)(LONG dwKey ,LPCTSTR sVBName, LPCTSTR sEventName, LPCTSTR sParam) ;
	STDMETHOD_(CString ,GetMapData)(LONG dwKey) ;
	STDMETHOD_(long ,SetNotifyCommand)(LONG dwKey ,LPCTSTR sKey) ;
	STDMETHOD_(BOOL ,ResizeControl)(LONG dwKey ,LPCTSTR strCtrlName, long nLRPlusSize, long nUDPlusSize) ;
	STDMETHOD_(BOOL ,MoveControl)(LONG dwKey ,LPCTSTR strCtrlName, long nXDir, long nYDir) ;
	STDMETHOD_(BOOL ,SetCtrlResizeFlag)(LONG dwKey ,LPCTSTR strCtrlName, BOOL bMove_X, BOOL bMove_Y, BOOL bResize_X, BOOL bResize_Y) ;
	STDMETHOD_(short ,SendTRForCert)(LONG dwKey ,LPCTSTR szTrName) ;
	STDMETHOD_(BOOL ,IsDeskActive)(LONG dwKey);
};

/////////////////////////////////////////////////////////////////////////////
// CMainDispManager
// No.33 IAUMainDispManager
class CMainDispManager : public IAUMainDispManager
{
public:
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;
	//--------------------------------------------

	STDMETHOD_(CString, GetMapInfo)(LONG dwKey, short nFlag);
	STDMETHOD_(CString, GetUserInfo)(LONG dwKey, short nType);
	STDMETHOD_(short, MsgBox)(LONG dwKey, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags);
	STDMETHOD_(BOOL, ChangeTabProperty)(LONG dwKey, short nUnitScr, short nTabPage, short nType, short nValue);
	STDMETHOD_(void, SetGShareData)(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue, short nFlag);
	STDMETHOD_(CString, GetGShareData)(LONG dwKey, LPCTSTR szKey);
	STDMETHOD_(void, SetFrameVariable)(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue);
	STDMETHOD_(CString, GetFrameVariable)(LONG dwKey, LPCTSTR szKey);
	STDMETHOD_(BOOL, RemoveGlobalData)(LONG dwKey, LPCTSTR szKey, LPCTSTR szValue);
	STDMETHOD_(void, CloseMain)(LONG dwKey, short nFlag);
	STDMETHOD_(void, Restart)(LONG dwKey);
	STDMETHOD_(CString, GetAcctList)(LONG dwKey);
	STDMETHOD_(void, SetOpenMapStyle)(LONG dwKey, short nType, short nInfo1, short nInfo2, short nMessageBar);
	STDMETHOD_(CString, GetGwanList)(LONG dwKey);
	STDMETHOD_(CString, GetGwanCode)(LONG dwKey, LPCTSTR szKey);
	STDMETHOD_(void, ClearMessageBar)(LONG dwKey);
	STDMETHOD_(short, ShowDialog)(LONG dwKey, LPCTSTR szKey, short nParam1, short nParam2);
	STDMETHOD_(void, NotifyEvent)(LONG dwKey, LPCTSTR szKey, LPCTSTR szData);
	STDMETHOD_(CString, GetGwanCodeOneGroup)(LONG dwKey, LPCTSTR szKey, LPCTSTR szOption);
	STDMETHOD_(void, SetAcctPasswd)(LONG dwKey, LPCTSTR szAcct, LPCTSTR szPasswd);
	STDMETHOD_(CString, GetAcctPasswd)(LONG dwKey, LPCTSTR szAcct);
	STDMETHOD_(void, SetBatchAcctInfo)(LONG dwKey, LPCTSTR szAcctInfo);
	STDMETHOD_(CString, GetBatchAcctInfo)(LONG dwKey, LPCTSTR szAcct);
//	STDMETHOD_(CString, GetMasterData)(LONG dwKey, short nFlag);
	STDMETHOD_(void, SetOpenMapStyle2)(LONG dwKey, short nType, short nInfo1, short nInfo2, short nMessageBar, short nPostOpen);
	STDMETHOD_(short, IHWriteSectionData)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szData, LPCTSTR szFile);
	STDMETHOD_(CString, IHGetSectionData)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szKey, LPCTSTR szDefault, LPCTSTR szFile);
	STDMETHOD_(CString, IHGetSectionNames)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szFile);
	STDMETHOD_(short, IHAddSection)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile);
	STDMETHOD_(short, IHDeleteSection)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szSection, LPCTSTR szFile);
	STDMETHOD_(short, IHRenameSection)(LONG dwKey, LPCTSTR szGroup, LPCTSTR szOldSecName, LPCTSTR szNewSecName, LPCTSTR szFile);
	STDMETHOD_(void, ChangeFrameSize)(LONG dwKey, short nFlag, short nParam1, short nParam2);
	STDMETHOD_(CString, GetFrameSize)(LONG dwKey, short nFlag);
	STDMETHOD_(void, ReportCall)(LONG dwKey, LPCTSTR szFile, long lStreamID, LPCTSTR szData);
};


/////////////////////////////////////////////////////////////////////////////
// CFrameDispManager
// No.34 IAUFrameDispManager
class CFrameDispManager : public IAUFrameDispManager
{
public:
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;
	//--------------------------------------------

	STDMETHOD_(short ,GetDataSaveType)(LONG dwKey ) ;
	STDMETHOD_(CString ,GetFrameInfoFile)(LONG dwKey ) ;
	STDMETHOD_(short ,GetFontSize)(LONG dwKey) ;
	STDMETHOD_(CString ,GetFrameScrNum)(LONG dwKey ) ;
	STDMETHOD_(void ,ChangeFrameSize)(LONG dwKey ,short nFlag, short nParam1, short nParam2) ;
};

/////////////////////////////////////////////////////////////////////////////
// CViewDispManager
// No.35 IAUViewDispManager
class CViewDispManager //: public IAUViewDispManager
{
public:
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;
	//--------------------------------------------

	STDMETHOD_(short ,ShowDialog)(LONG dwKey ,LPCTSTR szKey, short nIndex) ;
	STDMETHOD_(short ,MsgBoxID)(LONG dwKey ,short nID, LPCTSTR szMessage, LPCTSTR szCaption, short nFlags) ;
};

/////////////////////////////////////////////////////////////////////////////
// CPrimLibDispManager
// No.36 IAUPrimLibDispManager
class CPrimLibDispManager : public IAUPrimLibDispManager
{
public:
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;
	//--------------------------------------------
	STDMETHOD_(short, IsUserDefinedMode)(LONG dwKey, LPCTSTR strMode, short nConID) ;
	STDMETHOD_(void, ReLoadOtherMap)(LONG dwKey, LPCTSTR szPath) ;
	STDMETHOD_(void, SetMessage)(LONG dwKey, LPCTSTR nId) ;
	STDMETHOD_(void, MapSwitch)(LONG dwKey, LPCTSTR strLR) ;
	STDMETHOD_(void, InsertSplit)(LONG dwKey, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPublic, LPCTSTR szGroup, LPCTSTR szSameXfmResize = _T("")) ;
	STDMETHOD_(void, DeleteSplit)(LONG dwKey, LPCTSTR szPath) ;
	STDMETHOD_(void, TabToggle)(LONG dwKey, LPCTSTR strUpDown) ;
	STDMETHOD_(void, DefaultSetUp)(LONG dwKey, LPCTSTR szLR, LPCTSTR szUpDown, LPCTSTR szHoga, LPCTSTR szPath1, LPCTSTR szPath2, LPCTSTR szPath3, LPCTSTR szExt1, LPCTSTR szExt2) ; 
	STDMETHOD_(void, SetActiveTab)(LONG dwKey, LPCTSTR szTabID) ;
	STDMETHOD_(void, ChangeInputRangeResize)(LONG dwKey, LPCTSTR szChange) ;
	STDMETHOD_(void, ToggleWindow)(LONG dwKey, LPCTSTR strID, LPCTSTR strDIR, LPCTSTR strSUBDIR) ;
	STDMETHOD_(void, SplitResizeOption)(LONG dwKey, LPCTSTR strID, LPCTSTR RightDIR, LPCTSTR BottomDIR) ;	

};

/////////////////////////////////////////////////////////////////////////////
// CPrimScrDispManager
// No.37 IAUPrimScrDispManager
class CPrimScrDispManager : public IAUPrimScrDispManager
{
public:
	STDMETHOD_(LONG ,Advise)(LPUNKNOWN pUnKnown) ;
	STDMETHOD_(void, UnAdvise)(LONG dwKey) ;
	//--------------------------------------------

};


/////////////////////////////////////////////////////////////////////////////
// CRunStateManager
// No.38 IRunStateManager
class CRunStateManager : public IRunStateManager
{
public:
	STDMETHOD_(BOOL, NotifyState)(LPCSTR szKey, long dwState, long dwStateData=NULL);

public:
	CRunStateManager();
	~CRunStateManager();
	
public:
	long  m_dwInterface[MaxRunState];
};


/////////////////////////////////////////////////////////////////////////////
// CControlPosManager
// No.39 IControlPosManager
class CControlPosManager : public IControlPosManager
{
public:
	STDMETHOD_(BOOL, Advise)(CWnd* pParent = NULL);
	STDMETHOD_(BOOL, UnAdvise)();
	STDMETHOD_(void,  SetParent)(CWnd* pParent, CSize* pSize);
	STDMETHOD_(BOOL, AddControl)(CWnd* pControl, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);
	STDMETHOD_(BOOL, AddControl)(const UINT& unId, const DWORD& dwStyle = CP_MOVE_HORIZONTAL);
	STDMETHOD_(BOOL, AddControl)(CWnd* pControl, const DWORD& dwStyle, CString csVBName);
	STDMETHOD_(BOOL, AddControl)(const UINT& unId, const DWORD& dwStyle, CString csVBName);

	STDMETHOD_(BOOL, RemoveControl)(CWnd* pControl);
	STDMETHOD_(BOOL, RemoveControl)(const UINT& unId);
	STDMETHOD_(void, ResetControls)(void);
	//	virtual void MoveControls(void);
	STDMETHOD_(void, ResizeControls)(int oldwidth, int oldheight, int newwidth, int newheight);
	STDMETHOD_(void, SetNegativeMoves)(const BOOL& fNegativeMoves = TRUE);
	STDMETHOD_(BOOL, GetNegativeMoves)(void) const;

	//	virtual void UpdateParentSize(void);
};


/////////////////////////////////////////////////////////////////////////////
// CCumstomManager
// No.40 ICumstomManager
/*
#include "../../../Shared/src/TAupCommonDll/TAupQueryAgentCtrl.h"
#include "../../../Shared/src/TAupCommonDll/TAupTrAgent.h"
#define		QACNT_MAX			3
class CCumstomManager : public ICumstomManager
{
public:
	STDMETHOD_(LONG, GetInterface)(int nID, long dwUnUse) ;
	STDMETHOD_(LONG, GetInterface)(LPCSTR szID, long dwUnUse) ;
	STDMETHOD_(int,  GetInterfaceID)(LPCSTR szID, long dwUnUse);

public:
	// 인터페이스 관리 변수(Array)
	STUA_INTERFACE m_uaInterface[MAXICumstomManager];
	CTRAgentWnd	m_TRAgentWnd;

	CCumstomManager(); 

	HRESULT	ReceiveQueryData(WORD wQAIndex, LPVOID lpHederInfo, LPVOID lpTRData, long dwTRDateLen);
	HRESULT	ReceiveRealData(WORD wQAIndex, CAutoToTR * lpAutoToTr);

public:
	class CCumstomSite_Info: ICumstomSite_Info
	{
	public:
		STDMETHOD_(BOOL, Initialize)(CWnd* pParent) ;
		STDMETHOD_(LONG, GetHoga)(LPCSTR szCode);

	public:
		CCumstomSite_Info();
		~CCumstomSite_Info();

		// CTAupQueryAgentCtrl * m_pQA;
		long	m_pQA;

		// CTAupTrAgent *	m_pTrAgent;
		long	m_pTrAgent;

		CWnd*	m_pParent;

		BOOL	m_bInitialize;

		CTAupQueryAgentCtrl* m_arrQA [QACNT_MAX];

	}m_xCumstomSite_Info;
	friend class CCumstomSite_Info;

	//< CTAupTrAgent에 수신된 데이타를 함수 콜로 받아오기 위한 Interface
	class CTAupITrAgentSite : public ITrAgentSite
	{
	public:
		STDMETHOD(ReceiveQueryData) (WORD wQAIndex, LPVOID lpHederInfo, LPVOID lpTRData, long dwTRDateLen)
		{
			MYMETHOD_PROLOGUE(CCumstomManager, TAupITrAgentSite)
				return pThis->ReceiveQueryData(wQAIndex, lpHederInfo, lpTRData, dwTRDateLen);
		}

		STDMETHOD(ReceiveRealData) (WORD wQAIndex, CAutoToTR * lpAutoToTr)
		{
			MYMETHOD_PROLOGUE(CCumstomManager, TAupITrAgentSite)
				return pThis->ReceiveRealData(wQAIndex, lpAutoToTr);
		}

	} m_xTAupITrAgentSite;
	friend class CTAupITrAgentSite;

	class CTRAgentWnd_Receive : public ITRAgentWnd_Receive
	{
		// CTRAgentWnd에서 OnReceiveTR(..)호출될 때 호출된다.
		STDMETHOD_(LRESULT, OnReceiveTR)(WPARAM wp, LPARAM lp);
	} m_xTRAgentWnd_Receive;
	friend class CTRAgentWnd_Receive;
};
*/


/////////////////////////////////////////////////////////////////////////////
// CSTManager
// No.41 ISTManager
class CSTManager_Info
{
public:
	CString m_szSTPath;
	int		m_nCount;
	
public:
	CSTManager_Info() : m_nCount(0)
	{
	}
};
typedef CList<CSTManager_Info*, CSTManager_Info*> CSTManager_List;

class CSTManager : public ISTManager
{
public:
	// 전략실행여부를 세팅한다.
	// I-PARAM : szSTPath 전략FullPath, bAdd:TRUE이면 +, FALSE이면 -
	// O-PARAM : 
	STDMETHOD_(int, AddSTRunINfo)(LPCSTR szSTPath, BOOL bAdd) ;

	// 전략실행 정보를 알아온다.
	// I-PARAM : szSTPath 전략FullPath
	// O-PARAM : 실행되고 있는 카운트, 실행되고 있으면 0이상
	STDMETHOD_(int, GetSTRunINfo)(LPCSTR szSTPath) ;

public:
	~CSTManager();
//	CSTManager_Info* FindItem(LPCSTR szSTPath);
	POSITION FindItem(LPCSTR szSTPath, CSTManager_Info*& pInfo);
	void RemoveAll();

	CSTManager_List		m_List;
};


/////////////////////////////////////////////////////////////////////////////
// CSystemUtilManager
// No.42 ISystemUtilManager
class CSystemUtilManager : public ISystemUtilManager2
{
	BOOL CheckExeRun( const CString &szCmdLine, BOOL _bShow );
public:
	STDMETHOD_(BOOL, IsRegistered)(LPCSTR szProgID, CLSID &clsid) ;
	STDMETHOD_(int, DLLRegister)(LPCSTR szFileName) ;
	STDMETHOD_(BOOL, IsRegisteredEx)(LPCSTR szProgID, CLSID &clsid, LPCSTR szOcxName, LPCSTR szFullPath=NULL);
};


/////////////////////////////////////////////////////////////////////////////
// CCompanyManager
// No.43 ICompanyManager, ICompanyManager_SS
#include "MonitorWnd.h"
class CCompanyManager : public ICompanyManager_SS
{
public:
	CCompanyManager();
	~CCompanyManager();

	STDMETHOD_(int, GetID)();
	STDMETHOD_(LPCSTR, GetName)(int nID);

	// ICompanyManager_SS
	STDMETHOD_(int, GetAcctCode)(CStringList& listAcct, CStringList& listAcctName);
	STDMETHOD_(CString, GetPwd)();
//	STDMETHOD_(long, OnGetData)(WPARAM wParam, LPARAM lParam);
//	STDMETHOD_(int, RqNewOrder)(HWND hWnd, long pSave, int p_nUserType);			//COrderSave* pSave
//	STDMETHOD_(int, RqNewOrderStockBuy)(HWND hWnd, long _pSave, int p_nUserType);	// 주식매수주문.
//	STDMETHOD_(int, RqNewOrderStockSell)(HWND hWnd, long _pSave, int p_nUserType);	// 주식매도주문.
//	STDMETHOD_(BOOL, GetLastMessage)(CString& szMsgCode, CString& szMsg);
	STDMETHOD_(int, GetSysEdu)(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc);
	STDMETHOD_(void, RemoveSysEdu)(LPCSTR szKey, long pThisPoint, long dwKey, SysEduCallbackProc pCallFunc);

	STDMETHOD_(int, GetAcctCodeEx)(LIST_STACC_SSDATA &ListAcctInfo, int nReqType) ;
	STDMETHOD_(LPCSTR, GetAcctInfo)(LPCSTR szAcct, int nAcctSize, int nInfoType);

public:
	int GetSamsungAcctCode(CStringList& listAcct, CStringList& listAcctName);
	int GetSamsungAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType);
	LPCSTR GetSamsungAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType);
	int GetEugeneAcctCode(CStringList& listAcct, CStringList& listAcctName);
	int GetEugeneAcctCodeEx(LIST_STACC_SSDATA &ListAcctInfo, int nReqType);
	LPCSTR GetEugeneAcctInfo(LPCSTR szAcct, int nAcctSize, int nInfoType);

	BOOL CreateChkSysWnd();

public:
	CString m_szLastMsg, m_szLastMsgCode;
//	CTRHelperWnd m_SysWnd;
	int	m_nSysEdu;
};

// No.44
class CAUChartUtilManager : public IAUChartUtilManager
{
public:
	CAUChartUtilManager();
	~CAUChartUtilManager();

	STDMETHOD_(IChartFloatDlg*, AdviseChartFloatDlg)() ;
	STDMETHOD_(void, UnAdviseChartFloatDlg)(IChartFloatDlg*) ;

public:

	CMChartUtilLoadLib	*m_pMChartDll;
	UINT	m_ChartMngID[ID_IAUChartUtilManager_Max];
};

typedef struct 
{
	char cType;
	LONG dwTypeValue;
} ST_NewsSignalMng;
typedef CList<ST_NewsSignalMng*, ST_NewsSignalMng*> List_ST_NewsSignalMng;
// No.45
class CNewsSignalManager : public INewsSignalManager
{
public:
	CNewsSignalManager();
	~CNewsSignalManager();

	CNewsMoniterWnd*	m_pNewsMoniterWnd;
	int		m_nAddRef;
	List_ST_NewsSignalMng	m_List;

public:

//	STDMETHOD_(BOOL, IsNewsSignalLoaded)();
	STDMETHOD_(LONG, AdviseNewsSignal)(char cType, LONG dwTypeValue);
	STDMETHOD_(BOOL, UnAdviseNewsSignal)(LONG dwAdviseKey);
	STDMETHOD_(int, GetAdviseCount)();

	STDMETHOD_(int, SetNewsSignal)(BOOL bAdvise, CWnd* pScreenWnd, CString strType, CString strKey) ;
	STDMETHOD_(void, GetSignalData)(CString strType, CMapStringToPtr &mapSignalSet);
	STDMETHOD_(void, GetFilterData)(long &stFilterInfo);
	
	STDMETHOD_(void, SetSignalData)(CString strType, CMapStringToPtr *pmapSignalSet) ;
	STDMETHOD_(void, SetFilterData)(long *pstFilterInfo) ;
	STDMETHOD_(int,  GetSiganlTable)(CString strType, CStringArray &saSignalTable ) ;

	STDMETHOD_(HWND, GetDummyHwnd)() ;
};

/////////////////////////////////////////////////////////////////////////////
// No.50
//@Solomon090819class CKSLibManager : public IKSLibManager
//@Solomon090819{
//@Solomon090819public:
// 	STDMETHOD_(BOOL, Lib_KSLink_SetLinkTagData)( LPCTSTR sTag, LPCTSTR sValue) ;
// 	STDMETHOD_(BOOL, Lib_KSLink_SetFormLinkTagData)(CString sTag, CString sValue ) ;
// 	STDMETHOD_(CString, Lib_KSLink_GetFileHeader)() ;
// 	STDMETHOD_(CString, Lib_KSLink_GetLoginInfo)(CString strTag);
// 	STDMETHOD_(int,		Lib_KSLink_GetAccountCount)();
// 	STDMETHOD_(CString, Lib_KSLink_GetAccount)(int nIndex);
// 	STDMETHOD_(BYTE,	Lib_CommRequestData)(HWND hFormWnd, LONG pRqCommInfo, BYTE* lpData, int nDataLen, 
// 					 BOOL bDownLoad=FALSE, int nProgressType=-1, BOOL bWaitCursor=TRUE);
// 	STDMETHOD_(CString, Lib_KSLink_GetAccInfo)(CString strAccount, int nType);
// 	STDMETHOD_(long, Lib_KSLink_GetAccInfo)(CString strAccount);
// 	STDMETHOD_(COLORREF, Lib_KSLink_GetColor)(short nIndex, BOOL bUseOrgIndex = FALSE );
// 	STDMETHOD_(long, Lib_KSLink_GetEnvInfo)(int nType);
//@Solomon090819
//@Solomon090819public:
//@Solomon090819	CKSLibManager();
//@Solomon090819
//@Solomon090819	long m_pColorMap;	//CColorMap* m_pColorMap
//@Solomon090819};

/////////////////////////////////////////////////////////////////////////////
// CPcTrMngApp
// See PcTrMng.cpp for the implementation of this class
//


class CPcTrMngApp : public CWinApp
{
public:
	CPcTrMngApp();
	~CPcTrMngApp();
	int SetHanaroEnvSetting(HWND hMainWnd, LPCSTR szFormatKey);
	void SetHanaroGateMngHandle();

	CString m_szRootDir, m_szDrfnBinDir;
	CString m_szTemp;
	
	HWND	m_hMainHwnd;
	HWND PctrGetMainHandle();
	CWnd* PctrGetMainWnd();	

	void LoadDefaultColorTable();
// Overrides
	// ClassWizard generated STDMETHOD_(function overrides
	//{{AFX_VIRTUAL(CPcTrMngApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CPcTrMngApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:	// Variable
	// No.0 IGageManager
	CGateManager		m_GateMng;

	// No.1 IPCTRanager
	CPCTRManager		m_PcTrMng;

	// No.2 ITrCommManager
	CTrCommManager*		m_pTrCommMng;

	// No.3 ISocketManager

	// No4. IMainResouceManager , IGDIManager
	//	CGDIManager			m_GdiMng;
//	CMainResouceManager	m_GdiMainMng;

	// No.5 IMasterDataManager
	CMasterDataManager	m_MasterMng;

	// No.6 ISkinColorManager
	CSkinColorManager	m_SkinColorMng;

	// No.7 IAUHideManager
//	CAUBGManager		m_BGManager;

	// No.8 IAUAutoOrderManager
//	COrderMonitorManager		m_OrderMonMng;

	// No.9 IAUGwanManager
//	CAUGwanManager			m_GwanMng; -> PctrWinix로 이동.

	// No.10 IAUSignalManager
//	CSignalManager		m_SigMng;

	// No.11 IOrderMngLoader, IOrderMngLoader
//	COrderMngLoader		m_OrderLoader;
//	IAUOrderManager		*m_pOderMng;

	// No.12 IAUTooltipManager

	// No.13 ILogManager

	// No.14 IDebugManager
	CDebugManager		m_DebugMng;

	// No.15 IAccountManager
//	CAccountManager		m_AccountMng;

	// No.16 ISharedDataManager

	// No.17 IRegistryManager
//	CRegistryManager	m_RegMng;

	// No.18 IAUChartToolManager

	// No.19 IAUDrdsManager
	CAUDrdsManager		m_DrdsMng;

	// No.20 IVersionManager

	// No.21 IMainInfoManager
	CMainInfoManager	m_MainInfoMng;

	// No.22 IMainExInfoManager
	CMainExInfoManager	m_MainExinfoMng;

	// No.23 IEncryptionManager
//	CEncryptionManager	m_EncMng;

	// No.24 IDataUtilManager
	CDataUtilManager	m_DataUtilMng;

	// No.25 IAUPCOrderManager
//	CAUPCOrderManager	m_PCOrderMng;

	// 인터페이스 관리 변수(Array)
	STUA_INTERFACE m_uaInterface[MAX_INTERFACE];

//	IAUDrdsManager* GetNewIAUDrdsManager();

	// No.26 ICodeListManager
//	CTreeListManager*	m_pCodeListMng;
	CTreeListMngLoader		*m_pTLMngLoader;

	// No.27 IExtCodeWndManager
//	IExtCodeWndManager*		m_pExtCodeWndMng;
//	CExtCodeWndMngLoader*	m_pECWMngLoader;

	// No.28
	CAUDataMonitorManager*	m_pDataMonMng;
	
	// No.29 IClipboardManager
	CClipboardManager*		m_pClipMng;

	// No.30 IAUFormOpenManager
//	CFormOpenDispManager		m_FormOpenDispMng;

	// No.31 IAUFormDataManager
//	CFormDataDispManager		m_FormDataDispMng;

	// No.32 IAUDispManager
//	CDeskDispManager			m_DeskDispMng;

	// No.33 IAUMainDispManager
//	CMainDispManager			m_MainDispMng;

	// No.34 IAUFrameDispManager
//	CFrameDispManager			m_FrameDispMng;

	// No.35 IAUViewDispManager
//	CViewDispManager			m_ViewDispMng;

	// No.36 IAUPrimLibDispManager
//	CPrimLibDispManager			m_PrimLibDispMng;

	// No.37 IAUPrimScrDispManager
//	CPrimScrDispManager			m_PrimScrDispMng;

	// No.38
//	CRunStateManager			m_StateMng;

	// No.39
//	CControlPosManager			m_CtlPosMng;

	// No.40
	//CCumstomManager				m_CumtomMng;

	// No.41
	CSTManager					m_StMng;

	// No.42 
	CSystemUtilManager			m_SysUtilMng;

	// No.43 
	CCompanyManager				m_CompanyMng;

	// No.44
//	CAUChartUtilManager			m_ChartUtilMng;

	// No.45
	CNewsSignalManager			m_NewsSignalMng;

	// No.50
//	CKSLibManager				m_KSLibMng;
	CPartnerMngLoader			*m_pPtMngLoader;
	CPctrWinixLoader			*m_pPctrWinixLoader;
	
	// No.51
	//IPacketConvertManager		*m_pPacketCtMng;	

	// {{
	HINSTANCE m_hTestLib;
	LONG	m_Key;
	void LoadTestDLL();
	void UnLoadTestDLL();
	// }}
	BOOL DOChartOCXRegister();

	// 메인프레임 되신에 PCTR에서 메인역활을 하는 윈도우
	CWnd*  m_pPCTrMainWnd;	

public:
	CString _WinixVariant(HWND hWnd, int comm, CString strData = "");

	//alzioyes:100611
	CString m_WorkDate;
	void _DoPrepareData();
	void _DoClearDataFile();
};

extern CPcTrMngApp theApp;


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCTRMNG_H__DEAC0189_E61B__BY_JUNOK_LEE_INCLUDED_)
