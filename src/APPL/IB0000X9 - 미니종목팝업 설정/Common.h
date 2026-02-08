#pragma once


#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;

// define message
#define WX_CLOSE_WINDOW		(WM_USER+0x2000)
#define WX_SET_TRACKING		(WM_USER+0x2001)
#define WX_LASTPOS_UPDATE	(WM_USER+0x2002)
#define WX_EDIT_KEYPRESS	(WM_USER+0x2003)
#define WX_EDIT_KILLFOCUS	(WM_USER+0x2004)
#define WX_LBOX_KILLFOCUS   (WM_USER+0x2005)
#define WX_CHANGE_CODE		(WM_USER+0x2006)
#define WX_ADD_NEWCODE		(WM_USER+0x2007)
#define WX_ALIGN_V_UPPER	(WM_USER+0x2008)
#define WX_ALIGN_V_LOWER	(WM_USER+0x2009)
#define WX_ALIGN_H_UPPER	(WM_USER+0x200A)
#define WX_ALIGN_H_LOWER	(WM_USER+0x200B)
#define WX_VIEW_PROPERTY	(WM_USER+0x200C)

#define WD_ADD_CODE			(WM_USER+0x3000)
#define WD_CLOSE_CODE		(WM_USER+0x3001)

#define WF_VIEWTYPE			(WM_USER+0x3002)
#define WF_SIMPLECOLOR		(WM_USER+0x3003)
#define WF_TOPMOST			(WM_USER+0x3004)
#define WF_BKCOLOR			(WM_USER+0x3005)
#define WF_FLASHCOLOR		(WM_USER+0x3006)
#define WF_FIELDCHANGE		(WM_USER+0x3007)
#define WF_FLASH			(WM_USER+0x3008)

// define Color!
#define COLOR_LINE			93
#define COLOR_TABLE			75
#define COLOR_BK			64
#define COLOR_GRIDHEAD		74
#define COLOR_GUIDEBACK		213
#define COLOR_PANELBACK		66
#define COLOR_BODYBACK		181
#define COLOR_GRIDHEADTXT	76
#define COLOR_DATA			99
#define COLOR_TEXT			69
#define COLOR_TABLEHEAD		96
#define	COLOR_MDB			171		// 매도 back
#define	COLOR_MSB			170		// 매수 back
#define COLOR_PLUSTXT		94
#define COLOR_MINUSTXT		95
#define COLOR_FOCUS			78
#define COLOR_CONTROLBACK	66
#define COLOR_DEF_POPUP_BACK	0xDEF1EB
#define COLOR_DEF_POPUP_FLASH	0xBFBFBF

#define DEF_POPUP_POSX		50
#define DEF_POPUP_POSY		50

#define PADDING_VALUE	(6)

// Control 윈도우 높이
#define POPUP_HEIGHT	(20)

#define MAX_FHOGA			(5)
#define FCODE_LEN			(8)
#define DOUBLE_PREFIX		(0.00005)

typedef enum { CT_NONE=0, CT_STOCK, CT_JISU, CT_FUTURE, CT_OPTION, CT_SPREAD } CODE_TYPE;
typedef enum { VT_HORIZONTAL, VT_VERTICAL } VIEW_TYPE;

struct FIELD_INFO {
	LPCSTR symb;	// 조회 Symbol
	LPCSTR rsym;	// 실시간 Symbol
	LPCSTR name;	// 필드명
	BOOL   must;	// 필수여부
	BOOL   sort;	// 정렬가능여부
	DWORD  width;	// width
};
#define MAX_FIELD_WIDTH		(80)


#ifndef WS_EX_LAYERED 
#define WS_EX_LAYERED  0x00080000
#define LWA_COLORKEY   0x0000001
#define LWA_ALPHA      0x0000002
#endif
typedef BOOL (WINAPI *lpSLWA)(HWND hWnd, COLORREF cr, BYTE bAlpha, DWORD dwFlags);
class CLayer 
{
public:
	CLayer(void){}
	~CLayer(void){}
	
	static bool SetTransparent(CWnd *pWnd, int transPercent, DWORD *color=NULL)
	{
		HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
		if(hUser32 != NULL)
		{
			lpSLWA pSetLayeredWindowAttributes = (lpSLWA)GetProcAddress(hUser32,"SetLayeredWindowAttributes");
			if(pSetLayeredWindowAttributes != NULL)
			{
				const int wndStyle = GetWindowLong(pWnd->GetSafeHwnd(),GWL_EXSTYLE);
				if(!(wndStyle & WS_EX_LAYERED)) 
					SetWindowLong(pWnd->GetSafeHwnd(), GWL_EXSTYLE, wndStyle|WS_EX_LAYERED);
				
				if(color != NULL)
					pSetLayeredWindowAttributes(pWnd->GetSafeHwnd() , *color,(255*transPercent)/100, LWA_ALPHA|LWA_COLORKEY);
				else 
					pSetLayeredWindowAttributes(pWnd->GetSafeHwnd() , 0, (255*transPercent)/100, LWA_ALPHA);
				return true;
			}
		}  
		return false;
	}
};

typedef pair<CString,CString> SORT_OBJECT;
struct stringCompare
{
protected:
	bool m_bUpper;

public:
	struct stringCompare(bool bUpper) : m_bUpper(bUpper) {}
	bool operator()(SORT_OBJECT &s1, SORT_OBJECT &s2)
	{
		LPCSTR name1= (LPCSTR)s1.second;  
		LPCSTR name2= (LPCSTR)s2.second;  
		const int len1= s1.second.GetLength();  
		const int len2= s2.second.GetLength();  
		const int len= len1<len2 ? len1 : len2;  
		for(int i=0; i<len; i++) {  
			char ch1= name1[i];  
			char ch2= name2[i];  
			if(ch1&0x80 && ch2&0x80) { //한-한  
				if(ch1 < ch2) return m_bUpper ? true : false;
				else if(ch1 > ch2) return m_bUpper ? false : true;
				else if(ch1 == ch2) {  
					ch1= name1[i+1];  
					ch2= name2[i+1];  
					if(ch1 < ch2) return m_bUpper ? true : false;  
					else if(ch1 > ch2) return m_bUpper ? false : true;
				}  
				i++;  
				continue;  
			}  
			else if(ch1&0x80 && !(ch2&0x80)) return m_bUpper ? false : true; //한-영  
			else if(!(ch1&0x80) && ch2&0x80) return m_bUpper ? true : false; //영-한  
			//영-영  
			if('a'<=ch1 && ch1<='z') ch1= ch1 - 0x20;  
			if('a'<=ch2 && ch2<='z') ch2= ch2 - 0x20;  
			if(ch1 < ch2) return m_bUpper ? true : false;  
			else if(ch1 > ch2) return m_bUpper ? false : true;  
		}  
		return false; 
	}
};

struct priceCompare
{
private:
	bool m_bRemoveSign, m_bUpper;
public:
	priceCompare(bool bUpper, bool removeSign) : m_bUpper(bUpper), m_bRemoveSign(removeSign) {}
	bool operator()(SORT_OBJECT &s1, SORT_OBJECT &s2)
	{
		CString sV1 = s1.second, sV2 = s2.second;
		if (m_bRemoveSign)
		{
			sV1.Replace("+", ""); sV1.Replace("-", "");
			sV2.Replace("+", ""); sV2.Replace("-", "");
		}
		sV1.Remove(','); sV2.Remove(',');
		return m_bUpper ? atof(sV1)<atof(sV2) : atof(sV1)>atof(sV2);
	}
};

