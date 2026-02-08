#ifndef __AXCOMMON_H__
#define __AXCOMMON_H__

#ifdef _AXCOMMON
	#define AXIS_EXT_DATA __declspec(dllexport)
	#define AXIS_EXT_CLASS __declspec(dllexport)
	#define AXIS_EXT_API __declspec(dllexport)
#else
	#define AXIS_EXT_DATA  __declspec(dllimport)
	#define AXIS_EXT_CLASS __declspec(dllimport)
	#define AXIS_EXT_API __declspec(dllimport)
#endif // _AXCOMMON

#include <afxtempl.h>
#include <memory>
#include "../../h/axisfire.h"
#include "../../h/axisvar.h"
 
#define DEFAULT_FONT_NAME		"±¼¸²Ã¼"
#define DEFAULT_FONT_SIZE		9
#define DEFAULT_ROW_HEIGHT		18

//** profile kind
enum ProfileKind {
	pkAxis,
	pkAxisMenu,
	pkAxisUser,
	pkAxisTab,
	pkAxisTicker,
	pkPalette,
	pkVTCode,
	pkEnvironment,
	pkUserSetup,
	pkUserConfig,
	pkLink,
	pkLinkBar,
	pkLinkName,
	pkManageSetup,
	pkOrderSetup,
	pkUserTool,
	pkScreenReg,
	pkUserProgram,
	pkFirewall,
	pkSTInfo,
	pkYellowPage,
	pkHotKey,
	pkPortfolio
};


//** CAxis
class AXIS_EXT_CLASS CAxisCommon {
private:
	CWnd* m_wnd; // parent window handle
	CWnd* m_inter;
	CString m_home;
	CFont* m_font;
	CFont* m_fontBold;
public:
	CAxisCommon(CWnd * const pWnd) { 
		ASSERT(pWnd);
		m_wnd = pWnd;
		m_home = Variant(homeCC);
		if (m_home[m_home.GetLength() - 1] != '\\')
			m_home += '\\';

		m_font = GetAxFont();
		m_fontBold = GetAxFont(DEFAULT_FONT_NAME, DEFAULT_FONT_SIZE, true);
	}

	~CAxisCommon() {};
	CWnd*		GetView();
	void		SetInterestControl(CWnd* inter)
	{
		m_inter = inter;
	}
public:
	CString		Variant(int comm, const char* data = "");
	CFont*		GetAxFont(const char* fName = DEFAULT_FONT_NAME, int point = DEFAULT_FONT_SIZE, bool bBold = false, bool bItalic = false);
	CFont*		GetAxFont(struct _fontR* font);
	CBrush*		GetAxBrush(COLORREF clr);	
	CPen*		GetAxPen(COLORREF clr, int width = 1, int style = PS_SOLID);
	CPen*		GetAxPen(struct _penR* pen);
	COLORREF	GetAxColor(int colorIndex);	
	CBitmap*	GetAxBitmap(LPCTSTR lpszBitmapFileName);
	CString		GetUser();
	
	int			GetCodeType(const char* code);
	CString		GetItemName(const char* code);

	CFont*		GetDefaultFont() const {
		return m_font;
	}

	CFont*		GetDefaultBoldFont() const {
		return m_fontBold;
	}

	const CString& GetHome() const
	{
		return m_home;
	}

	LRESULT		SendMessage(WPARAM wParam, LPARAM lParam);
	LRESULT		SendMessage2(WPARAM wParam, LPARAM lParam);

	size_t GetDefaultFontSize() const {
		return DEFAULT_FONT_SIZE;
	}

	size_t GetDefaultRowHeight() const {
		return DEFAULT_ROW_HEIGHT;
	}

	void GetLogFont(LOGFONT* lf, LOGFONT* lfB = NULL);
};

//** CProfile class
class AXIS_EXT_CLASS CProfile
{
private:
	CString m_fileName;
public:
	CString GetString(const char* section, const char* item, const char* defValue = "");
	int     GetInt(const char* section, const char* item, int defValue = 0);
	double  GetDouble(const char* section, const char* item, double defValue = 0);

	void	Write(const char* section, const char* item, const char* value);
	void	Write(const char* section, const char* item, const int value);
	void	Write(const char* section, const char* item, const double value);

	//CString GetSection(const char* section);
	CString GetSectionLF(const char* section);

	void	WriteSection(const char* section, const char* data);

	const char* GetFileName() const {
		return (const char*)m_fileName;
	}
	void	SetFileName(const char* fileName);
	void	SetFileName(ProfileKind pk);
public:
	CProfile(const char* fileName);
	CProfile(ProfileKind pk);
};


//** CMemDC class
class AXIS_EXT_CLASS CMemoryDC : public CDC
{
public:
	// constructor sets up the memory DC
	CMemoryDC(CDC* pDC) : CDC()
	{
		ASSERT(pDC != NULL);

		m_pDC = pDC;
		m_oldBitmap = NULL;
		m_bMemDC = !pDC->IsPrinting();
              
		if (m_bMemDC)    // Create a Memory DC
		{
			pDC->GetClipBox(&m_rect);
			CreateCompatibleDC(pDC);
			pDC->LPtoDP(&m_rect);

			m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
			m_oldBitmap = SelectObject(&m_bitmap);

			SetMapMode(pDC->GetMapMode());
			SetWindowExt(pDC->GetWindowExt());
			SetViewportExt(pDC->GetViewportExt());

			pDC->DPtoLP(&m_rect);
			SetWindowOrg(m_rect.left, m_rect.top);
		}
		else        // Make a copy of the relevent parts of the current DC for printing
		{
			m_bPrinting = pDC->m_bPrinting;
			m_hDC       = pDC->m_hDC;
			m_hAttribDC = pDC->m_hAttribDC;
		}
	}
    
	// Destructor copies the contents of the mem DC to the original DC
	~CMemoryDC()
	{
		if (m_bMemDC) 
		{    
			m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
						this, m_rect.left, m_rect.top, SRCCOPY);
			SelectObject(m_oldBitmap);
		}
		else
		{
			m_hDC = m_hAttribDC = NULL;
		}
	}

	// Allow usage as a pointer
	CMemoryDC* operator->() { return this; }

	// Allow usage as a pointer
	operator CMemoryDC*() { return this; }

private:
	CBitmap  m_bitmap;
	CBitmap* m_oldBitmap;
	CDC*     m_pDC;
	CRect    m_rect;
	BOOL     m_bMemDC;
};

//** static function
AXIS_EXT_API CString	Format(const char* formatString, ...);
AXIS_EXT_API void	FormatCopy(void* buffer, const char* formatString, ...);
AXIS_EXT_API CString	Parse(CString& src, const char* delim = "\t");
AXIS_EXT_API BOOL	IsFileExist(const char* fileName);
AXIS_EXT_API CString	NormalizeFolderName(CString& folder);
AXIS_EXT_API CString	GetPasswordChar(const char* szPassword);
AXIS_EXT_API CString	EncodeAcctName(const char* name, const char* alias);
AXIS_EXT_API void	DecodeAcctName(const char* encodedName, CString& name, CString& alias);
AXIS_EXT_API CString	EncodeAcctNo(const char* szAcct);
AXIS_EXT_API CString	DecodeAcctNo(const char* szAcct);


//** CFileBitmap class 
class AXIS_EXT_CLASS CFileBitmap : public CBitmap
{
public:
	CFileBitmap(const char* home, const char* fileName);
	CFileBitmap(const char* filePath);
	virtual ~CFileBitmap();

	CSize	GetSize();
	void	Open(const char* filePath);
	void	Close();
};

#define MMI_MINWIDTH		120
#define MMI_MINHEIGHT		100

AXIS_EXT_API void	ConditionCopyMinMaxInfo(MINMAXINFO* src, MINMAXINFO* dest);


typedef struct _bitmap_res {
	BOOL bskin;
	BOOL bStaff;
	CBitmap* bitmap;
} bitmap_res;

#define DF_USESTL_COMMON1
//** static class Axis
class AXIS_EXT_CLASS Axis
{
private:
	static CBitmap  bmpBandTerm;
	static CMap <CString, LPCTSTR, bitmap_res, bitmap_res> m_bitmaps;
	static CMap <CString, LPCTSTR, COLORREF, COLORREF> m_colors;
	static CArray <int, int>		 m_newsKey;
	static CArray <LPCTSTR, LPCTSTR> m_newsName;

	static BOOL ReloadFileBitmap( CBitmap* p, const char* bmpName);
#ifdef DF_USESTL_COMMON
	static std::shared_ptr<CBitmap> LoadFileBitmap(const char* bmpName);
#else
	static CBitmap* LoadFileBitmap(const char* bmpName);
#endif
	
public:
	static CString	home;  
	static CString	user;
	static CString	userNM;
	static CString	userID;
	static CString	userIP;
	static CString  skinName;
	static COLORREF dialogBgColor;
	static CBrush	dialogBgBrush;
	static int		interFrame;
	static CSize	reviseFrameSize;	
	static HCURSOR  handCursor;
	static BOOL		isCustomer;
	static COLORREF	maskColor;
	static CWnd*	mainWnd;
	static BOOL		devMode;
	static CFont	font;
	static CFont	fondBold;
	static BOOL		isVista;
	static int		WinTheme;
	static CString sFiller;  //vc2019
public:
	static void Initialize();
	static void ReloadSkin( void );
	static void Release();
	static void SkinReady();
	static void SetSkin(const char* skin);
	static int GetTheme();

	static void DrawBandTerm(CDC* pDC, int x);
	static HBRUSH GetCtlBrush(CWnd* pWnd);
	static CBitmap* GetBitmap(const char* fileName, const char* key = NULL, bool bStaff = false );
	static CBitmap* GetBitmap(UINT resourceID);
	static CBitmap* GetSkinBitmap(const char* fileName,bool bStaff = false);
	static COLORREF GetColor(int index);

	//** news source methods 
	static int		GetNewsCount();
	static LPCTSTR	GetNewsName(int index);
	static int		GetNewsKey(int index);

	//** message box
	static int		MessageBox(CWnd* parent, const char* message, UINT button = MB_OK);
	static int 		MessageBox(const char* message, UINT button = MB_OK);
	static int		MessageBox(UINT stringResourceID, UINT button = MB_OK);

	//** Parse data
	static CString  Parse(CString& source, char delimiter = '\t');
	static CString  Parse(CString& source, CString delimiter);
};
/*  //vc2019
void Axis::DrawBandTerm(CDC* pDC, int x);
void Axis::Initialize();
void Axis::Release();
void Axis::ReloadSkin();
int Axis::GetTheme();
HBRUSH Axis::GetCtlBrush(CWnd* pWnd);
CBitmap* Axis::GetBitmap(const char* fileName, const char* key, bool bStaff );
CBitmap* Axis::GetBitmap(UINT resourceID);
CBitmap* Axis::GetSkinBitmap(const char* fileName, bool bStaff);
CBitmap* Axis::LoadFileBitmap(const char* fileName);
COLORREF Axis::GetColor(int index);

int  Axis::GetNewsCount();
int  Axis::GetNewsKey(int index);
LPCTSTR Axis::GetNewsName(int index);

int Axis::MessageBox(CWnd* parent, const char* message, UINT button);
int Axis::MessageBox(const char* message, UINT button);
int Axis::MessageBox(UINT stringResourceID, UINT button);

CString Axis::Parse(CString& source, char delimiter);
CString Axis::Parse(CString& source, CString delimiter);
*/

AXIS_EXT_API CString GetProfileFileName(ProfileKind pk = pkAxis);


/************************************************************************/
/* CControlChanger class                                                */
/************************************************************************/
class AXIS_EXT_CLASS CControlChanger
{
private:
	CWnd* m_parent;
	CArray<CWnd*, CWnd*> m_controls;
	CFont m_font;
	HBITMAP	m_btnBitmap[3];	// normal, down, hover
	CBitmap m_hdrBitmap[3]; // start, span, end
	void Initialize();
	void Release();
	void ChangeControl(CWnd* wnd);
public:
	CControlChanger(CWnd* parent);
	~CControlChanger();
};


//** macro
#define Extract(a)		CString(a, sizeof(a) / sizeof(a[0]))
#define Trim(a)			{a.TrimLeft(), a.TrimRight();}
#define ArraySize(a)	(sizeof(a) / sizeof(a[0]))


//** constant
#define LEN_ACCT	11

#define acctLeft	3
#define acctMid		2
#define acctRight	6

//** external symbols
extern AXIS_EXT_DATA const char* szLastStat;
extern AXIS_EXT_DATA const char* szStartMap;
extern AXIS_EXT_DATA const char* szRoot;
extern AXIS_EXT_DATA const char* szETC;
extern AXIS_EXT_DATA const char* szScreen;
extern AXIS_EXT_DATA const char* szMessage;
extern AXIS_EXT_DATA const char* szBkNotice;
extern AXIS_EXT_DATA const char* szSystem;
extern AXIS_EXT_DATA const char* szTabView;
extern AXIS_EXT_DATA const char* szCommon;
extern AXIS_EXT_DATA const char* szUserSN;
extern AXIS_EXT_DATA const char* szUserEN;
extern AXIS_EXT_DATA const char* szGeneral;
extern AXIS_EXT_DATA const char* szWebLink;
extern AXIS_EXT_DATA const char* szSecure;
extern AXIS_EXT_DATA const char* szUserTool;
extern AXIS_EXT_DATA const char* szSelect;
extern AXIS_EXT_DATA const char* szShowBar;
extern AXIS_EXT_DATA const char* szFire;
extern AXIS_EXT_DATA const char* szLogCheck;
extern AXIS_EXT_DATA const char* szDTAccouncement;
extern AXIS_EXT_DATA const char* szDTKeyboard;
extern AXIS_EXT_DATA const char* szAgreeDuplicate;
extern AXIS_EXT_DATA const char* szHKey;
extern AXIS_EXT_DATA const char* szToolFunc;
extern AXIS_EXT_DATA const char* szSName;
extern AXIS_EXT_DATA const char* szPalette;
extern AXIS_EXT_DATA const char* szPalettes;
extern AXIS_EXT_DATA const char* szAux;

extern AXIS_EXT_DATA const char* szSite;


#endif // __AXCOMMON_H__