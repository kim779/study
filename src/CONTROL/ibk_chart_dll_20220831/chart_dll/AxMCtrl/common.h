#ifndef __COMMON_H__
#define __COMMON_H__

static const char* EMPTYSTR = _T("");

static const char  TAB   = 0x09;
static const char  NL    = 0x13;
static const char  RF    = 0x10;
static const char  SPACE = 0x20;
static const char  DEL   = 0x7F;

static const char  TABSTR[]			= { TAB, 0x00 };
static const char  NLSTR[]			= { NL, 0x00 };
static const char  RFSTR[]			= { RF, 0x00 };
static const char  SPACESTR[]		= { SPACE, 0x00 };
static const char  DELSTR[]			= { DEL, 0x00 }; 

#define ARRAYSIZE(a) (sizeof(a) / sizeof(a[0]))
#define ARRAYSTRING(a) CString(a, ARRAYSIZE(a))
#define ARRAYSTRINGSIZE(a, b) CString(a, b)
#define ARRAY_SIZE(a) ARRAYSIZE(a)

#define SAFE_DELETE(a) { if (a) { delete a; a = NULL; } }
#define SAFE_DELETE_WND(a) { if (a) { a->DestroyWindow(); delete a; a = NULL; }}

#define NOTIFY(msg, wParam, lParam) { if (m_messageHandler && ::IsWindow(m_messageHandler->m_hWnd)) return m_messageHandler->SendMessage(msg, wParam, lParam); \
																							  else return 0L; }
static CWnd* msg_map_wnd;
// string routine
#define TRIM(a) { a.TrimLeft(); a.TrimRight(); }

AFX_EXT_API int		XParseString(LPCTSTR data, char delim, CStringList& list);
AFX_EXT_API CString XParse(CString& srcstr, char delim);
AFX_EXT_API CString XParse(CString& srcstr, LPCTSTR delim);

// memory routine
void	XZero(LPVOID buffer, int len);
void	XCopy(LPVOID dest, LPVOID src, int len);
void	XFill(LPVOID buffer, char ch, int len);
void	XSpace(LPVOID buffer, int len);

#define XFILL(a, b) XFill(a, b, ARRAYSIZE(a))
#define XSPACE(a)   XSpace(a, ARRAYSIZE(a))

int		XStrLen(LPCTSTR str);
LPSTR	XStrCat(LPSTR str, LPCTSTR catStr);
LPSTR	XStrCpy(LPSTR dest, LPCTSTR src);
LPSTR   XStrNCpy(LPSTR dest, LPCTSTR src, int len, BOOL nullTerminate = TRUE);

LPCTSTR XTruncateZero(CString& str);
LPCTSTR XCommaString(CString& str);

// file handling routine
AFX_EXT_API BOOL	IsFileExist(LPCTSTR lpszFileName);

// window handling routine
AFX_EXT_API BOOL	XRegisterWindowClass(LPCTSTR lpszClassName, HBRUSH hbrBackground = (HBRUSH)(COLOR_3DFACE + 1));
void	XCenterWindow(CWnd* pWnd, CWnd* pParent = NULL);
CRect	XCenterWindowRect(CWnd* pParent, int cx, int cy);

// misc
BOOL	IsControlKey(BYTE key);

// common color constants
#define clBlack			0x00000000
#define clMaroon		0x00000080
#define clGreen			0x00008000
#define clOlive			0x00008080
#define clNavy			0x00800000
#define clPurple		0x00800080
#define clTeal			0x00808000
#define clGray			0x00808080
#define clSilver		0x00C0C0C0
#define clRed			0x000000FF
#define clLime			0x0000FF00
#define clYellow		0x0000FFFF
#define clBlue			0x00FF0000
#define clFuchsia		0x00FF00FF
#define clAqua			0x00FFFF00
#define clWhite			0x00FFFFFF
#define clMoneyGreen	0x00C0DCC0
#define clSkyBlue		0x00F0CAA6
#define clCream			0x00F0FBFF
#define clMedGray		0x00A4A0A0
#define	clBACK			RGB(243,249,254)

// control alignment
typedef enum { AL_NONE = 0 , AL_TOP, AL_LEFT, AL_RIGHT, AL_BOTTOM, AL_CLIENT } ALIGNMENT;



#endif // __COMMON_H__